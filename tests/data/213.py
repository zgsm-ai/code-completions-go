"""
Web Scraping and Data Mining Library

This module implements a comprehensive web scraping and data mining library
that can extract information from websites, process the data, and perform
various analysis tasks.

Features:
- Web scraping with different methods (requests, selenium, beautiful soup)
- Data extraction and cleaning
- Text processing and sentiment analysis
- Data storage in various formats
- Crawling multiple pages with rate limiting
"""

import requests
import json
import time
import re
import os
import csv
from urllib.parse import urljoin, urlparse
from bs4 import BeautifulSoup
from collections import defaultdict, Counter
from typing import List, Dict, Tuple, Optional, Union, Any, Callable
import html
import math
import random
from datetime import datetime
import sqlite3
import hashlib
import pickle

# For environments where external libraries might not be available
try:
    from selenium import webdriver
    from selenium.webdriver.common.by import By
    from selenium.webdriver.common.keys import Keys
    from selenium.webdriver.support.ui import WebDriverWait
    from selenium.webdriver.support import expected_conditions as EC
    from selenium.webdriver.chrome.options import Options as ChromeOptions
    SELENIUM_AVAILABLE = True
except ImportError:
    SELENIUM_AVAILABLE = False

try:
    import nltk
    from nltk.corpus import stopwords
    from nltk.tokenize import word_tokenize, sent_tokenize
    from nltk.stem import PorterStemmer, WordNetLemmatizer
    from nltk.sentiment import SentimentIntensityAnalyzer
    NLTK_AVAILABLE = True
except ImportError:
    NLTK_AVAILABLE = False

try:
    import pandas as pd
    PANDAS_AVAILABLE = True
except ImportError:
    PANDAS_AVAILABLE = False


class RateLimiter:
    """Rate limiter to prevent overwhelming servers."""
    
    def __init__(self, requests_per_second=1):
        self.requests_per_second = requests_per_second
        self.last_request_time = 0
    
    def wait(self):
        """Wait if necessary to respect rate limit."""
        current_time = time.time()
        time_since_last = current_time - self.last_request_time
        min_interval = 1.0 / self.requests_per_second
        
        if time_since_last < min_interval:
            time.sleep(min_interval - time_since_last)
        
        self.last_request_time = time.time()


class CacheManager:
    """Cache manager for storing and retrieving scraped data."""
    
    def __init__(self, cache_dir="cache"):
        self.cache_dir = cache_dir
        os.makedirs(cache_dir, exist_ok=True)
        self.memory_cache = {}
    
    def _get_cache_path(self, url):
        """Get the file path for a cached URL."""
        url_hash = hashlib.md5(url.encode()).hexdigest()
        return os.path.join(self.cache_dir, f"{url_hash}.cache")
    
    def get(self, url, use_memory=True):
        """Get cached content for a URL."""
        # Check memory cache first
        if use_memory and url in self.memory_cache:
            return self.memory_cache[url]
        
        # Check file cache
        cache_path = self._get_cache_path(url)
        if os.path.exists(cache_path):
            try:
                with open(cache_path, 'rb') as f:
                    data = pickle.load(f)
                    
                    # Store in memory cache for faster access
                    if use_memory:
                        self.memory_cache[url] = data
                    
                    return data
            except:
                pass  # If error loading, ignore and return None
        
        return None
    
    def set(self, url, data, ttl=3600):
        """Cache content for a URL."""
        # Store in memory cache
        self.memory_cache[url] = {
            'data': data,
            'timestamp': time.time(),
            'ttl': ttl
        }
        
        # Store in file cache
        cache_path = self._get_cache_path(url)
        try:
            with open(cache_path, 'wb') as f:
                pickle.dump({
                    'data': data,
                    'timestamp': time.time(),
                    'ttl': ttl
                }, f)
        except:
            pass  # If error saving, ignore
    
    def is_valid(self, cached_item):
        """Check if a cached item is still valid."""
        if not cached_item:
            return False
        
        return time.time() - cached_item['timestamp'] < cached_item['ttl']
    
    def clean(self, max_age=86400):
        """Clean up expired cache entries."""
        # Clean memory cache
        expired_keys = [
            url for url, item in self.memory_cache.items()
            if not self.is_valid(item)
        ]
        
        for key in expired_keys:
            del self.memory_cache[key]
        
        # Clean file cache
        for filename in os.listdir(self.cache_dir):
            if filename.endswith('.cache'):
                cache_path = os.path.join(self.cache_dir, filename)
                
                try:
                    with open(cache_path, 'rb') as f:
                        cached_item = pickle.load(f)
                    
                    if not self.is_valid(cached_item):
                        os.remove(cache_path)
                except:
                    pass  # If error reading, try to remove anyway
                    try:
                        os.remove(cache_path)
                    except:
                        pass


class WebScraper:
    """Main web scraper class."""
    
    def __init__(self, rate_limiter=None, cache_manager=None, headers=None, timeout=30):
        """
        Initialize the web scraper.
        
        Args:
            rate_limiter: RateLimiter instance
            cache_manager: CacheManager instance
            headers: Default HTTP headers
            timeout: Request timeout in seconds
        """
        self.rate_limiter = rate_limiter or RateLimiter()
        self.cache_manager = cache_manager or CacheManager()
        self.session = requests.Session()
        
        # Set default headers
        self.headers = headers or {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
            'Accept-Language': 'en-US,en;q=0.9',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Connection': 'keep-alive',
        }
        
        self.session.headers.update(self.headers)
        self.timeout = timeout
        
        # WebDriver for JavaScript-heavy sites (if available)
        self.driver = None
    
    def _get_cached_response(self, url):
        """Get response from cache if valid."""
        cached = self.cache_manager.get(url)
        if cached and self.cache_manager.is_valid(cached):
            return cached['data']
        return None
    
    def _cache_response(self, url, response):
        """Cache a response."""
        self.cache_manager.set(url, {
            'status_code': response.status_code,
            'headers': dict(response.headers),
            'content': response.content,
            'url': response.url,
            'timestamp': time.time()
        })
    
    def _make_request(self, url, method='GET', params=None, data=None, 
                    allow_redirects=True, use_cache=True):
        """
        Make an HTTP request with caching and rate limiting.
        
        Args:
            url: URL to request
            method: HTTP method
            params: Query parameters
            data: Request body data
            allow_redirects: Whether to follow redirects
            use_cache: Whether to use cache
            
        Returns:
            requests.Response object
        """
        # Check cache first
        if use_cache and method == 'GET':
            cached_response = self._get_cached_response(url)
            if cached_response:
                # Convert cached data back to Response-like object
                response = requests.Response()
                response.status_code = cached_response['status_code']
                response.headers = cached_response['headers']
                response._content = cached_response['content']
                response.url = cached_response['url']
                return response
        
        # Apply rate limiting
        self.rate_limiter.wait()
        
        try:
            response = self.session.request(
                method=method,
                url=url,
                params=params,
                data=data,
                allow_redirects=allow_redirects,
                timeout=self.timeout
            )
            
            # Cache successful GET requests
            if use_cache and method == 'GET' and response.status_code == 200:
                self._cache_response(url, response)
            
            return response
        except Exception as e:
            print(f"Request failed: {str(e)}")
            return None
    
    def get_page(self, url, use_cache=True):
        """
        Get a page's HTML content.
        
        Args:
            url: URL to fetch
            use_cache: Whether to use cache
            
        Returns:
            BeautifulSoup object or None if failed
        """
        response = self._make_request(url, use_cache=use_cache)
        
        if response and response.status_code == 200:
            try:
                return BeautifulSoup(response.content, 'html.parser')
            except Exception as e:
                print(f"Failed to parse HTML: {str(e)}")
        
        return None
    
    def get_json(self, url, use_cache=True):
        """
        Get JSON data from a URL.
        
        Args:
            url: URL to fetch
            use_cache: Whether to use cache
            
        Returns:
            Parsed JSON data or None if failed
        """
        response = self._make_request(url, use_cache=use_cache)
        
        if response and response.status_code == 200:
            try:
                return response.json()
            except Exception as e:
                print(f"Failed to parse JSON: {str(e)}")
        
        return None
    
    def download_file(self, url, file_path, chunk_size=8192):
        """
        Download a file from a URL.
        
        Args:
            url: URL to download from
            file_path: Local path to save the file
            chunk_size: Size of chunks to download
            
        Returns:
            bool: True if successful, False otherwise
        """
        self.rate_limiter.wait()
        
        try:
            response = self.session.get(url, stream=True, timeout=self.timeout)
            
            if response.status_code == 200:
                # Ensure directory exists
                os.makedirs(os.path.dirname(file_path), exist_ok=True)
                
                with open(file_path, 'wb') as f:
                    for chunk in response.iter_content(chunk_size=chunk_size):
                        f.write(chunk)
                
                return True
            else:
                print(f"Failed to download file. Status code: {response.status_code}")
                return False
        
        except Exception as e:
            print(f"Download failed: {str(e)}")
            return False
    
    def setup_selenium(self, driver_type="chrome", headless=True):
        """
        Setup Selenium WebDriver for JavaScript-heavy sites.
        
        Args:
            driver_type: Type of WebDriver to use
            headless: Whether to run in headless mode
            
        Returns:
            bool: True if setup successful, False otherwise
        """
        if not SELENIUM_AVAILABLE:
            print("Selenium not available. Install with: pip install selenium")
            return False
        
        try:
            if driver_type == "chrome":
                options = ChromeOptions()
                if headless:
                    options.add_argument("--headless")
                options.add_argument("--no-sandbox")
                options.add_argument("--disable-dev-shm-usage")
                options.add_argument("--disable-gpu")
                
                self.driver = webdriver.Chrome(options=options)
            else:
                print(f"Unsupported driver type: {driver_type}")
                return False
            
            return True
        
        except Exception as e:
            print(f"Failed to setup Selenium: {str(e)}")
            return False
    
    def get_page_selenium(self, url, wait_time=10, wait_until=None):
        """
        Get a page using Selenium WebDriver.
        
        Args:
            url: URL to fetch
            wait_time: Time to wait for element
            wait_until: Condition to wait for
            
        Returns:
            BeautifulSoup object or None if failed
        """
        if not self.driver:
            print("Selenium WebDriver not initialized. Call setup_selenium() first.")
            return None
        
        try:
            self.driver.get(url)
            
            if wait_until:
                WebDriverWait(self.driver, wait_time).until(wait_until)
            else:
                time.sleep(wait_time)
            
            page_source = self.driver.page_source
            return BeautifulSoup(page_source, 'html.parser')
        
        except Exception as e:
            print(f"Selenium request failed: {str(e)}")
            return None
    
    def close_selenium(self):
        """Close the Selenium WebDriver."""
        if self.driver:
            self.driver.quit()
            self.driver = None


class DataExtractor:
    """Class for extracting data from HTML elements."""
    
    @staticmethod
    def extract_text(element, clean=True):
        """
        Extract text from a BeautifulSoup element.
        
        Args:
            element: BeautifulSoup element
            clean: Whether to clean whitespace
            
        Returns:
            Extracted text string
        """
        if element is None:
            return ""
        
        text = element.get_text(strip=True)
        
        if clean:
            # Clean up whitespace
            text = re.sub(r'\s+', ' ', text).strip()
        
        return text
    
    @staticmethod
    def extract_links(soup, base_url=None, filter_func=None):
        """
        Extract all links from a page.
        
        Args:
            soup: BeautifulSoup object
            base_url: Base URL for relative links
            filter_func: Function to filter links
            
        Returns:
            List of (link_text, href) tuples
        """
        links = []
        
        for a_tag in soup.find_all('a', href=True):
            href = a_tag.get('href')
            text = DataExtractor.extract_text(a_tag)
            
            # Convert relative URLs to absolute
            if base_url and not bool(urlparse(href).netloc):
                href = urljoin(base_url, href)
            
            # Apply filter function if provided
            if filter_func and not filter_func(href, text):
                continue
            
            links.append((text, href))
        
        return links
    
    @staticmethod
    def extract_images(soup, base_url=None, filter_func=None):
        """
        Extract all images from a page.
        
        Args:
            soup: BeautifulSoup object
            base_url: Base URL for relative links
            filter_func: Function to filter images
            
        Returns:
            List of (alt_text, src) tuples
        """
        images = []
        
        for img_tag in soup.find_all('img', src=True):
            src = img_tag.get('src')
            alt = img_tag.get('alt', '')
            
            # Convert relative URLs to absolute
            if base_url and not bool(urlparse(src).netloc):
                src = urljoin(base_url, src)
            
            # Apply filter function if provided
            if filter_func and not filter_func(src, alt):
                continue
            
            images.append((alt, src))
        
        return images
    
    @staticmethod
    def extract_tables(soup, headers=True):
        """
        Extract all tables from a page.
        
        Args:
            soup: BeautifulSoup object
            headers: Whether first row contains headers
            
        Returns:
            List of tables as lists of lists
        """
        tables = []
        
        for table in soup.find_all('table'):
            table_data = []
            
            # Extract rows
            for row in table.find_all('tr'):
                row_data = []
                
                # Extract cells
                for cell in row.find_all(['td', 'th']):
                    cell_text = DataExtractor.extract_text(cell)
                    row_data.append(cell_text)
                
                if row_data:  # Only add non-empty rows
                    table_data.append(row_data)
            
            # Convert first row to headers if requested
            if table_data and headers and len(table_data) > 1:
                header_row = table_data[0]
                data_rows = table_data[1:]
                
                # Create list of dictionaries
                table_dict = []
                for row in data_rows:
                    row_dict = {}
                    for i, value in enumerate(row):
                        if i < len(header_row):
                            row_dict[header_row[i]] = value
                        else:
                            row_dict[f"Column_{i}"] = value
                    
                    table_dict.append(row_dict)
                
                tables.append(table_dict)
            else:
                tables.append(table_data)
        
        return tables
    
    @staticmethod
    def extract_metadata(soup):
        """
        Extract metadata from a page.
        
        Args:
            soup: BeautifulSoup object
            
        Returns:
            Dictionary of metadata
        """
        metadata = {}
        
        # Extract title
        title_tag = soup.find('title')
        if title_tag:
            metadata['title'] = DataExtractor.extract_text(title_tag)
        
        # Extract meta description
        desc_tag = soup.find('meta', attrs={'name': 'description'})
        if desc_tag:
            metadata['description'] = desc_tag.get('content', '')
        
        # Extract meta keywords
        keywords_tag = soup.find('meta', attrs={'name': 'keywords'})
        if keywords_tag:
            metadata['keywords'] = keywords_tag.get('content', '')
        
        # Extract Open Graph data
        for prop in ['og:title', 'og:description', 'og:image', 'og:url']:
            tag = soup.find('meta', attrs={'property': prop})
            if tag:
                metadata[prop] = tag.get('content', '')
        
        # Extract canonical URL
        canonical_tag = soup.find('link', attrs={'rel': 'canonical'})
        if canonical_tag:
            metadata['canonical_url'] = canonical_tag.get('href', '')
        
        # Extract language
        html_tag = soup.find('html')
        if html_tag and html_tag.get('lang'):
            metadata['language'] = html_tag.get('lang')
        
        return metadata


class TextProcessor:
    """Text processing utilities."""
    
    def __init__(self):
        """Initialize text processor."""
        # Setup NLTK components if available
        self.stemmer = PorterStemmer() if NLTK_AVAILABLE else None
        self.lemmatizer = WordNetLemmatizer() if NLTK_AVAILABLE else None
        self.stop_words = set(stopwords.words('english')) if NLTK_AVAILABLE else set()
        
        # Add custom stop words
        self.stop_words.update([
            'www', 'http', 'https', 'com', 'org', 'net', 'edu',
            'said', 'would', 'could', 'should'
        ])
    
    def clean_text(self, text, remove_html=True, remove_urls=True, 
                remove_emails=True, remove_special_chars=True):
        """
        Clean text by removing unwanted elements.
        
        Args:
            text: Input text
            remove_html: Whether to remove HTML tags
            remove_urls: Whether to remove URLs
            remove_emails: Whether to remove email addresses
            remove_special_chars: Whether to remove special characters
            
        Returns:
            Cleaned text
        """
        if not text:
            return ""
        
        # Remove HTML tags
        if remove_html:
            text = re.sub(r'<[^>]+>', ' ', text)
        
        # Remove URLs
        if remove_urls:
            text = re.sub(r'https?://\S+|www\.\S+', ' ', text)
        
        # Remove email addresses
        if remove_emails:
            text = re.sub(r'\S+@\S+', ' ', text)
        
        # Remove special characters (keep letters, numbers and basic punctuation)
        if remove_special_chars:
            text = re.sub(r'[^\w\s\.\!\?\,\;\:\-\'\"\/\(\)\[\]]', ' ', text)
        
        # Normalize whitespace
        text = re.sub(r'\s+', ' ', text).strip()
        
        return text
    
    def tokenize(self, text, lower=True, remove_stop_words=True, 
                stem=False, lemmatize=False):
        """
        Tokenize text into words.
        
        Args:
            text: Input text
            lower: Whether to convert to lowercase
            remove_stop_words: Whether to remove stop words
            stem: Whether to apply stemming
            lemmatize: Whether to apply lemmatization
            
        Returns:
            List of tokens
        """
        # Convert to lowercase
        if lower:
            text = text.lower()
        
        # Tokenize
        if NLTK_AVAILABLE:
            tokens = word_tokenize(text)
        else:
            # Simple tokenization if NLTK not available
            tokens = re.findall(r'\b\w+\b', text)
        
        # Remove stop words
        if remove_stop_words and self.stop_words:
            tokens = [token for token in tokens if token not in self.stop_words]
        
        # Apply stemming
        if stem and self.stemmer:
            tokens = [self.stemmer.stem(token) for token in tokens]
        
        # Apply lemmatization
        if lemmatize and self.lemmatizer:
            tokens = [self.lemmatizer.lemmatize(token) for token in tokens]
        
        return tokens
    
    def extract_entities(self, text):
        """
        Extract named entities from text.
        
        Args:
            text: Input text
            
        Returns:
            Dictionary of entities by type
        """
        # This is a simplified implementation
        # A full implementation would use NLP libraries like spaCy
        
        entities = {
            'people': [],
            'organizations': [],
            'locations': [],
            'dates': [],
            'emails': [],
            'urls': [],
            'phone_numbers': []
        }
        
        # Extract emails
        email_pattern = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b'
        entities['emails'] = re.findall(email_pattern, text)
        
        # Extract URLs
        url_pattern = r'https?://(?:[-\w.])+(?:[:\d]+)?(?:/(?:[\w/_.])*(?:\?(?:[\w&=%.])*)?(?:#(?:\w*))?)?'
        entities['urls'] = re.findall(url_pattern, text)
        
        # Extract phone numbers (simplified)
        phone_pattern = r'(?:(?:\+?1[-\s]?)?(?:\(\d{3}\)|\d{3})[-\s]?\d{3}[-\s]?\d{4})'
        entities['phone_numbers'] = re.findall(phone_pattern, text)
        
        # Extract dates (simplified)
        date_pattern = r'\b\d{1,2}[/-]\d{1,2}[/-]\d{2,4}\b|\b\d{4}[/-]\d{1,2}[/-]\d{1,2}\b'
        entities['dates'] = re.findall(date_pattern, text)
        
        return entities
    
    def extract_keywords(self, text, n_keywords=10):
        """
        Extract keywords from text.
        
        Args:
            text: Input text
            n_keywords: Number of keywords to extract
            
        Returns:
            List of (keyword, score) tuples
        """
        # Clean and tokenize text
        clean_text = self.clean_text(text)
        tokens = self.tokenize(clean_text, remove_stop_words=True)
        
        if not tokens:
            return []
        
        # Calculate TF-IDF scores (simplified)
        token_counts = Counter(tokens)
        total_tokens = len(tokens)
        
        # Calculate TF (term frequency)
        tf_scores = {token: count / total_tokens for token, count in token_counts.items()}
        
        # For IDF, we would need a corpus
        # For now, we'll use a simplified score based on frequency and length
        keyword_scores = []
        for token, tf in tf_scores.items():
            # Boost score for longer words
            length_bonus = min(len(token) / 5, 1.5)
            # Boost score for capitalized words (potential proper nouns)
            capital_bonus = 1.2 if token[0].isupper() else 1.0
            
            score = tf * length_bonus * capital_bonus
            keyword_scores.append((token, score))
        
        # Sort by score and return top N
        keyword_scores.sort(key=lambda x: x[1], reverse=True)
        return keyword_scores[:n_keywords]


class SentimentAnalyzer:
    """Sentiment analysis utilities."""
    
    def __init__(self):
        """Initialize sentiment analyzer."""
        # Try to use NLTK's sentiment analyzer
        if NLTK_AVAILABLE:
            self.nltk_analyzer = SentimentIntensityAnalyzer()
        else:
            self.nltk_analyzer = None
        
        # Simple word-based sentiment lexicon as fallback
        self.positive_words = {
            'good', 'great', 'excellent', 'positive', 'amazing', 'awesome', 'fantastic',
            'wonderful', 'perfect', 'love', 'like', 'enjoy', 'happy', 'pleased',
            'satisfied', 'delighted', 'thrilled', 'excited', 'beautiful', 'nice',
            'awesome', 'best', 'better', 'improve', 'success', 'successful'
        }
        
        self.negative_words = {
            'bad', 'terrible', 'awful', 'horrible', 'negative', 'poor', 'worst',
            'hate', 'dislike', 'disappointed', 'sad', 'angry', 'frustrated',
            'annoyed', 'upset', 'unhappy', 'dissatisfied', 'terrible', 'wrong',
            'fail', 'failed', 'failure', 'problem', 'issue', 'issues', 'broken'
        }
    
    def analyze_sentiment(self, text, use_nltk=True):
        """
        Analyze sentiment of text.
        
        Args:
            text: Input text
            use_nltk: Whether to use NLTK's analyzer if available
            
        Returns:
            Dictionary with sentiment scores
        """
        # Use NLTK if available and requested
        if use_nltk and self.nltk_analyzer:
            scores = self.nltk_analyzer.polarity_scores(text)
            
            # Convert to our format
            return {
                'positive': scores['pos'],
                'negative': scores['neg'],
                'neutral': scores['neu'],
                'compound': scores['compound'],
                'label': self._get_sentiment_label(scores['compound'])
            }
        
        # Fallback to word-based analysis
        return self._word_based_sentiment(text)
    
    def _word_based_sentiment(self, text):
        """
        Simple word-based sentiment analysis.
        
        Args:
            text: Input text
            
        Returns:
            Dictionary with sentiment scores
        """
        # Clean and tokenize text
        text_processor = TextProcessor()
        clean_text = text_processor.clean_text(text)
        tokens = text_processor.tokenize(clean_text, lower=True)
        
        # Count positive and negative words
        positive_count = sum(1 for token in tokens if token in self.positive_words)
        negative_count = sum(1 for token in tokens if token in self.negative_words)
        total_count = len(tokens)
        
        # Calculate scores
        if total_count == 0:
            return {
                'positive': 0.0,
                'negative': 0.0,
                'neutral': 1.0,
                'compound': 0.0,
                'label': 'neutral'
            }
        
        positive_score = positive_count / total_count
        negative_score = negative_count / total_count
        
        # Compound score (simplified)
        compound = positive_score - negative_score
        
        return {
            'positive': positive_score,
            'negative': negative_score,
            'neutral': max(0, 1 - positive_score - negative_score),
            'compound': compound,
            'label': self._get_sentiment_label(compound)
        }
    
    def _get_sentiment_label(self, compound_score):
        """
        Get sentiment label based on compound score.
        
        Args:
            compound_score: Compound sentiment score
            
        Returns:
            String sentiment label
        """
        if compound_score >= 0.05:
            return 'positive'
        elif compound_score <= -0.05:
            return 'negative'
        else:
            return 'neutral'
    
    def analyze_batch(self, texts, use_nltk=True):
        """
        Analyze sentiment for multiple texts.
        
        Args:
            texts: List of texts to analyze
            use_nltk: Whether to use NLTK's analyzer if available
            
        Returns:
            List of sentiment dictionaries
        """
        return [self.analyze_sentiment(text, use_nltk) for text in texts]


class DataStorage:
    """Class for storing extracted data in various formats."""
    
    def __init__(self, base_dir="data"):
        """
        Initialize data storage.
        
        Args:
            base_dir: Base directory for storing data
        """
        self.base_dir = base_dir
        os.makedirs(base_dir, exist_ok=True)
    
    def save_json(self, data, filename):
        """
        Save data as JSON.
        
        Args:
            data: Data to save
            filename: Filename (without extension)
            
        Returns:
            bool: True if successful, False otherwise
        """
        filepath = os.path.join(self.base_dir, f"{filename}.json")
        
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                json.dump(data, f, indent=2, ensure_ascii=False)
            return True
        except Exception as e:
            print(f"Failed to save JSON: {str(e)}")
            return False
    
    def save_csv(self, data, filename, fieldnames=None):
        """
        Save data as CSV.
        
        Args:
            data: List of dictionaries to save
            filename: Filename (without extension)
            fieldnames: List of field names (keys in dictionaries)
            
        Returns:
            bool: True if successful, False otherwise
        """
        filepath = os.path.join(self.base_dir, f"{filename}.csv")
        
        try:
            with open(filepath, 'w', newline='', encoding='utf-8') as f:
                if data and isinstance(data[0], dict):
                    # Determine fieldnames if not provided
                    if not fieldnames:
                        fieldnames = set()
                        for item in data:
                            fieldnames.update(item.keys())
                        fieldnames = sorted(fieldnames)
                    
                    writer = csv.DictWriter(f, fieldnames=fieldnames)
                    writer.writeheader()
                    writer.writerows(data)
                else:
                    # Simple list data
                    writer = csv.writer(f)
                    writer.writerows(data)
            
            return True
        except Exception as e:
            print(f"Failed to save CSV: {str(e)}")
            return False
    
    def save_sqlite(self, data, filename, table_name="data"):
        """
        Save data to SQLite database.
        
        Args:
            data: List of dictionaries to save
            filename: Database filename (without extension)
            table_name: Table name to use
            
        Returns:
            bool: True if successful, False otherwise
        """
        filepath = os.path.join(self.base_dir, f"{filename}.db")
        
        try:
            conn = sqlite3.connect(filepath)
            cursor = conn.cursor()
            
            if not data:
                return False
            
            # Determine columns from first item
            columns = list(data[0].keys())
            
            # Create table
            columns_sql = ", ".join([f'"{col}" TEXT' for col in columns])
            cursor.execute(f'CREATE TABLE IF NOT EXISTS "{table_name}" ({columns_sql})')
            
            # Insert data
            placeholders = ", ".join(["?" for _ in columns])
            columns_list = ", ".join([f'"{col}"' for col in columns])
            
            insert_sql = f'INSERT INTO "{table_name}" ({columns_list}) VALUES ({placeholders})'
            
            for item in data:
                values = [item.get(col, '') for col in columns]
                cursor.execute(insert_sql, values)
            
            conn.commit()
            conn.close()
            
            return True
        except Exception as e:
            print(f"Failed to save SQLite: {str(e)}")
            return False
    
    def save_pandas(self, data, filename, format='csv'):
        """
        Save data using pandas if available.
        
        Args:
            data: Data to save
            filename: Filename (without extension)
            format: Format ('csv', 'excel', 'json')
            
        Returns:
            bool: True if successful, False otherwise
        """
        if not PANDAS_AVAILABLE:
            print("Pandas not available. Install with: pip install pandas")
            return False
        
        try:
            # Convert to DataFrame if not already
            if not isinstance(data, pd.DataFrame):
                df = pd.DataFrame(data)
            else:
                df = data
            
            # Save based on format
            if format == 'csv':
                filepath = os.path.join(self.base_dir, f"{filename}.csv")
                df.to_csv(filepath, index=False)
            elif format == 'excel':
                filepath = os.path.join(self.base_dir, f"{filename}.xlsx")
                df.to_excel(filepath, index=False)
            elif format == 'json':
                filepath = os.path.join(self.base_dir, f"{filename}.json")
                df.to_json(filepath, orient='records', indent=2)
            else:
                print(f"Unsupported format: {format}")
                return False
            
            return True
        except Exception as e:
            print(f"Failed to save with pandas: {str(e)}")
            return False


class WebCrawler:
    """Web crawler for exploring multiple pages."""
    
    def __init__(self, scraper=None, max_depth=2, max_pages=100, 
                 allowed_domains=None, disallowed_patterns=None):
        """
        Initialize web crawler.
        
        Args:
            scraper: WebScraper instance
            max_depth: Maximum depth to crawl
            max_pages: Maximum number of pages to crawl
            allowed_domains: List of allowed domains
            disallowed_patterns: List of regex patterns for URLs to skip
        """
        self.scraper = scraper or WebScraper()
        self.max_depth = max_depth
        self.max_pages = max_pages
        self.allowed_domains = allowed_domains or []
        self.disallowed_patterns = [re.compile(pattern) for pattern in (disallowed_patterns or [])]
        
        self.visited_urls = set()
        self.to_visit = []
        self.results = []
    
    def add_url(self, url, depth=0):
        """Add a URL to the queue if appropriate."""
        # Skip if already visited
        if url in self.visited_urls:
            return
        
        # Skip if we've reached max pages
        if len(self.visited_urls) >= self.max_pages:
            return
        
        # Skip if depth exceeds max
        if depth > self.max_depth:
            return
        
        # Check domain restrictions
        if self.allowed_domains:
            parsed_url = urlparse(url)
            if not any(domain in parsed_url.netloc for domain in self.allowed_domains):
                return
        
        # Check disallowed patterns
        for pattern in self.disallowed_patterns:
            if pattern.search(url):
                return
        
        # Add to queue
        self.to_visit.append((url, depth))
    
    def is_valid_url(self, url):
        """Check if a URL is valid for crawling."""
        # Parse URL
        parsed = urlparse(url)
        
        # Must have scheme and netloc
        if not parsed.scheme or not parsed.netloc:
            return False
        
        # Skip common non-HTML files
        path = parsed.path.lower()
        if any(path.endswith(ext) for ext in ['.pdf', '.jpg', '.png', '.gif', '.zip', '.exe']):
            return False
        
        return True
    
    def crawl(self, start_url):
        """
        Start crawling from a seed URL.
        
        Args:
            start_url: Starting URL
            
        Returns:
            List of crawled page data
        """
        if not self.is_valid_url(start_url):
            print(f"Invalid start URL: {start_url}")
            return self.results
        
        # Add start URL to queue
        self.add_url(start_url)
        
        # Process queue
        while self.to_visit and len(self.visited_urls) < self.max_pages:
            url, depth = self.to_visit.pop(0)
            
            # Skip if already visited
            if url in self.visited_urls:
                continue
            
            # Mark as visited
            self.visited_urls.add(url)
            
            print(f"Crawling (depth {depth}): {url}")
            
            # Fetch page
            soup = self.scraper.get_page(url)
            if not soup:
                continue
            
            # Extract data
            page_data = {
                'url': url,
                'depth': depth,
                'title': DataExtractor.extract_text(soup.find('title')),
                'meta': DataExtractor.extract_metadata(soup),
                'links': [(text, href) for text, href in DataExtractor.extract_links(soup, url)],
                'images': [(alt, src) for alt, src in DataExtractor.extract_images(soup, url)],
                'timestamp': datetime.now().isoformat()
            }
            
            self.results.append(page_data)
            
            # Add links to queue if not at max depth
            if depth < self.max_depth:
                for _, link in page_data['links']:
                    if self.is_valid_url(link):
                        self.add_url(link, depth + 1)
        
        return self.results


def create_sample_usage():
    """Create sample usage of the web scraping library."""
    
    # Initialize components
    scraper = WebScraper(rate_limiter=RateLimiter(1))
    text_processor = TextProcessor()
    sentiment_analyzer = SentimentAnalyzer()
    storage = DataStorage()
    
    # Example 1: Scrape a single page
    print("Example 1: Scraping a single page")
    url = "https://example.com"
    soup = scraper.get_page(url)
    
    if soup:
        # Extract metadata
        metadata = DataExtractor.extract_metadata(soup)
        print(f"Page title: {metadata.get('title', 'No title')}")
        
        # Extract text content
        page_text = DataExtractor.extract_text(soup.find('main') or soup)
        
        # Process text
        clean_text = text_processor.clean_text(page_text)
        tokens = text_processor.tokenize(clean_text)
        keywords = text_processor.extract_keywords(clean_text)
        
        # Analyze sentiment
        sentiment = sentiment_analyzer.analyze_sentiment(clean_text)
        
        # Save results
        results = {
            'url': url,
            'title': metadata.get('title'),
            'description': metadata.get('description'),
            'keywords': keywords,
            'sentiment': sentiment,
            'token_count': len(tokens),
            'scraped_at': datetime.now().isoformat()
        }
        
        storage.save_json(results, "page_analysis")
        print("Page analysis saved to JSON")
    
    # Example 2: Crawl a small site
    print("\nExample 2: Crawling a small site")
    crawler = WebCrawler(scraper, max_depth=1, max_pages=5)
    crawl_results = crawler.crawl("https://example.com")
    
    if crawl_results:
        # Process each crawled page
        processed_results = []
        
        for page in crawl_results:
            # Extract and process text
            page_soup = scraper.get_page(page['url'])
            if not page_soup:
                continue
            
            # Find main content area (simplified)
            content = page_soup.find('main') or page_soup.find('div', class_='content')
            if content:
                content_text = DataExtractor.extract_text(content)
                clean_content = text_processor.clean_text(content_text)
                sentiment = sentiment_analyzer.analyze_sentiment(clean_content)
                
                processed_results.append({
                    'url': page['url'],
                    'depth': page['depth'],
                    'title': page['title'],
                    'sentiment': sentiment,
                    'link_count': len(page['links']),
                    'image_count': len(page['images']),
                    'content_length': len(content_text)
                })
        
        # Save results
        storage.save_csv(processed_results, "crawl_results")
        storage.save_json(processed_results, "crawl_results")
        print(f"Crawled {len(processed_results)} pages and saved results")
    
    # Example 3: Extract and analyze reviews (simulated)
    print("\nExample 3: Extract and analyze reviews")
    
    # Simulated review data (in a real scenario, this would be scraped)
    reviews = [
        "This product is amazing! I love it so much.",
        "Terrible quality, completely broke after one day.",
        "It's okay, nothing special but does the job.",
        "Excellent value for money. Highly recommended!",
        "Not what I expected. Very disappointed."
    ]
    
    # Analyze sentiment for all reviews
    review_sentiments = sentiment_analyzer.analyze_batch(reviews)
    
    # Process and save results
    review_results = []
    for i, (review, sentiment) in enumerate(zip(reviews, review_sentiments)):
        keywords = text_processor.extract_keywords(review)
        
        review_results.append({
            'review_id': i + 1,
            'review_text': review,
            'sentiment': sentiment,
            'keywords': keywords,
            'processed_at': datetime.now().isoformat()
        })
    
    # Save results
    storage.save_json(review_results, "review_sentiment")
    storage.save_csv(review_results, "review_sentiment")
    print(f"Analyzed {len(review_results)} reviews and saved results")
    
    # Example 4: Extract tables from a page
    print("\nExample 4: Extract tables from a page")
    
    # Try to find a page with tables
    table_url = "https://example.com/tables"  # This is a hypothetical URL
    table_soup = scraper.get_page(table_url)
    
    if table_soup:
        tables = DataExtractor.extract_tables(table_soup)
        
        if tables:
            # Save first table
            storage.save_csv(tables[0], "extracted_table")
            print(f"Extracted {len(tables)} tables, saved first table")
        else:
            print("No tables found on the page")


def main():
    """Main function to demonstrate the web scraping library."""
    print("Web Scraping and Data Mining Library")
    print("=====================================")
    
    # Check if external dependencies are available
    print("Checking dependencies:")
    print(f"Selenium available: {'Yes' if SELENIUM_AVAILABLE else 'No'}")
    print(f"NLTK available: {'Yes' if NLTK_AVAILABLE else 'No'}")
    print(f"Pandas available: {'Yes' if PANDAS_AVAILABLE else 'No'}")
    
    # Download NLTK data if available
    if NLTK_AVAILABLE:
        try:
            nltk.download('stopwords', quiet=True)
            nltk.download('punkt', quiet=True)
            nltk.download('vader_lexicon', quiet=True)
            print("NLTK data downloaded successfully")
        except:
            print("Failed to download NLTK data")
    
    # Run examples
    try:
        create_sample_usage()
        
        print("\nExamples completed successfully!")
        print("Check the 'data' directory for saved results.")
        
    except Exception as e:
        print(f"Error running examples: {str(e)}")


if __name__ == "__main__":
    main()
<｜fim▁hole｜>