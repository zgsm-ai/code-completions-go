"""
Natural Language Processing Library

This module implements a comprehensive natural language processing library
that can handle various NLP tasks including tokenization, stemming,
lemmatization, part-of-speech tagging, named entity recognition,
and text classification.

Features:
- Text preprocessing and cleaning
- Tokenization for different languages
- Stemming and lemmatization
- Part-of-speech tagging
- Named entity recognition
- Text classification
- Sentiment analysis
- Topic modeling
"""

import re
import math
import random
import json
import os
import pickle
from collections import Counter, defaultdict
from typing import List, Dict, Tuple, Optional, Union, Any, Callable
import time
from datetime import datetime

# Try to import NLTK for additional functionality
try:
    import nltk
    from nltk.corpus import stopwords, wordnet
    from nltk.tokenize import word_tokenize, sent_tokenize
    from nltk.stem import PorterStemmer, LancasterStemmer, SnowballStemmer
    from nltk.stem import WordNetLemmatizer
    from nltk.tag import pos_tag
    from nltk.chunk import ne_chunk
    from nltk.classify import NaiveBayesClassifier
    from nltk.probability import ELEProbDist
    NLTK_AVAILABLE = True
except ImportError:
    NLTK_AVAILABLE = False

# Try to import scikit-learn for advanced models
try:
    from sklearn.feature_extraction.text import TfidfVectorizer, CountVectorizer
    from sklearn.model_selection import train_test_split
    from sklearn.naive_bayes import MultinomialNB
    from sklearn.linear_model import LogisticRegression
    from sklearn.svm import SVC
    from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
    from sklearn.decomposition import LatentDirichletAllocation
    from sklearn.cluster import KMeans
    SKLEARN_AVAILABLE = True
except ImportError:
    SKLEARN_AVAILABLE = False


class TextPreprocessor:
    """
    Text preprocessing class for cleaning and normalizing text data.
    """
    
    def __init__(self, language='english'):
        """
        Initialize the text preprocessor.
        
        Args:
            language: Language for preprocessing
        """
        self.language = language
        self.stop_words = set()
        
        # Load stopwords if NLTK is available
        if NLTK_AVAILABLE and language == 'english':
            try:
                self.stop_words = set(stopwords.words('english'))
            except:
                self.stop_words = self._get_default_stopwords()
        else:
            self.stop_words = self._get_default_stopwords()
    
    def _get_default_stopwords(self):
        """Get default stopwords if NLTK is not available."""
        # Basic English stopwords
        return {
            'i', 'me', 'my', 'myself', 'we', 'our', 'ours', 'ourselves', 'you', 'your',
            'yours', 'yourself', 'yourselves', 'he', 'him', 'his', 'himself',
            'she', 'her', 'hers', 'herself', 'it', 'its', 'itself', 'they',
            'them', 'their', 'theirs', 'themselves', 'what', 'which', 'who',
            'whom', 'this', 'that', 'these', 'those', 'am', 'is', 'are',
            'was', 'were', 'be', 'been', 'being', 'have', 'has', 'had',
            'having', 'do', 'does', 'did', 'doing', 'a', 'an', 'the',
            'and', 'but', 'if', 'or', 'because', 'as', 'until', 'while',
            'of', 'at', 'by', 'for', 'with', 'through', 'during', 'before',
            'after', 'above', 'below', 'up', 'down', 'in', 'out', 'on', 'off',
            'over', 'under', 'again', 'further', 'then', 'once'
        }
    
    def clean_text(self, text, lowercase=True, remove_html=True, 
                 remove_urls=True, remove_emails=True, remove_numbers=False,
                 remove_punctuation=False, remove_extra_whitespace=True):
        """
        Clean text by applying various preprocessing steps.
        
        Args:
            text: Input text to clean
            lowercase: Whether to convert to lowercase
            remove_html: Whether to remove HTML tags
            remove_urls: Whether to remove URLs
            remove_emails: Whether to remove email addresses
            remove_numbers: Whether to remove numbers
            remove_punctuation: Whether to remove punctuation
            remove_extra_whitespace: Whether to remove extra whitespace
            
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
        
        # Remove numbers
        if remove_numbers:
            text = re.sub(r'\d+', ' ', text)
        
        # Remove punctuation
        if remove_punctuation:
            text = re.sub(r'[^\w\s]', ' ', text)
        
        # Convert to lowercase
        if lowercase:
            text = text.lower()
        
        # Remove extra whitespace
        if remove_extra_whitespace:
            text = re.sub(r'\s+', ' ', text).strip()
        
        return text
    
    def tokenize(self, text, method='simple', language=None):
        """
        Tokenize text into words or sentences.
        
        Args:
            text: Input text to tokenize
            method: Tokenization method ('simple', 'nltk', 'whitespace', 'regex')
            language: Language for tokenization (if supported by method)
            
        Returns:
            List of tokens
        """
        if not text:
            return []
        
        # Use language parameter or fall back to instance language
        language = language or self.language
        
        if method == 'nltk' and NLTK_AVAILABLE:
            if language == 'english':
                return word_tokenize(text)
            else:
                # For non-English, NLTK might need additional data
                return word_tokenize(text)
        
        elif method == 'whitespace':
            return text.split()
        
        elif method == 'regex':
            # Basic word regex pattern
            return re.findall(r'\b\w+\b', text)
        
        else:  # Default simple method
            # Basic implementation without external libraries
            # Handle contractions
            contractions = {
                "won't": "will not",
                "can't": "cannot",
                "n't": " not",
                "'re": " are",
                "'ve": " have",
                "'ll": " will",
                "'d": " would",
                "'m": " am"
            }
            
            for contraction, expansion in contractions.items():
                text = text.replace(contraction, expansion)
            
            # Split on whitespace and punctuation
            tokens = re.split(r'[\s\.\,\!\?\:\;\-\(\)\[\]"\{\}\/\"]+', text)
            
            # Remove empty tokens
            return [token for token in tokens if token]
    
    def remove_stopwords(self, tokens, custom_stopwords=None):
        """
        Remove stopwords from token list.
        
        Args:
            tokens: List of tokens
            custom_stopwords: Additional custom stopwords to remove
            
        Returns:
            List of tokens with stopwords removed
        """
        if custom_stopwords:
            stopword_set = self.stop_words.union(set(custom_stopwords))
        else:
            stopword_set = self.stop_words
        
        return [token for token in tokens if token.lower() not in stopword_set]
    
    def normalize_tokens(self, tokens):
        """
        Normalize tokens by handling variations and capitalization.
        
        Args:
            tokens: List of tokens to normalize
            
        Returns:
            List of normalized tokens
        """
        normalized = []
        
        for token in tokens:
            # Basic normalization: lowercase
            normalized_token = token.lower()
            
            # Remove leading/trailing punctuation
            normalized_token = normalized_token.strip('.,!?;:"()[]{}\'"')
            
            # Skip empty tokens
            if normalized_token:
                normalized.append(normalized_token)
        
        return normalized


class Stemmer:
    """
    Stemmer class for reducing words to their root form.
    """
    
    def __init__(self, stemmer_type='porter'):
        """
        Initialize the stemmer.
        
        Args:
            stemmer_type: Type of stemmer ('porter', 'lancaster', 'snowball')
        """
        self.stemmer_type = stemmer_type
        
        # Initialize stemmers if NLTK is available
        if NLTK_AVAILABLE:
            if stemmer_type == 'porter':
                self.stemmer = PorterStemmer()
            elif stemmer_type == 'lancaster':
                self.stemmer = LancasterStemmer()
            elif stemmer_type == 'snowball':
                self.stemmer = SnowballStemmer('english')
            else:
                raise ValueError(f"Unknown stemmer type: {stemmer_type}")
        else:
            self.stemmer = None
    
    def stem(self, tokens):
        """
        Apply stemming to a list of tokens.
        
        Args:
            tokens: List of tokens to stem
            
        Returns:
            List of stemmed tokens
        """
        if not self.stemmer:
            # Fallback to a simple rule-based stemmer
            return [self._simple_stem(token) for token in tokens]
        
        return [self.stemmer.stem(token) for token in tokens]
    
    def _simple_stem(self, token):
        """
        Simple rule-based stemmer as fallback.
        
        Args:
            token: Token to stem
            
        Returns:
            Stemmed token
        """
        if len(token) <= 3:
            return token
        
        # Basic stemming rules
        if token.endswith('ies'):
            return token[:-3] + 'y'
        elif token.endswith('ied'):
            return token[:-3] + 'y'
        elif token.endswith('s'):
            return token[:-1]
        elif token.endswith('ed'):
            return token[:-2]
        elif token.endswith('ing'):
            return token[:-3] + 'e'
        elif token.endswith('ly'):
            return token[:-2]
        else:
            return token


class Lemmatizer:
    """
    Lemmatizer class for reducing words to their dictionary form.
    """
    
    def __init__(self, pos_tagging=False):
        """
        Initialize the lemmatizer.
        
        Args:
            pos_tagging: Whether to use POS tags for lemmatization
        """
        self.pos_tagging = pos_tagging
        
        # Initialize lemmatizer if NLTK is available
        if NLTK_AVAILABLE:
            self.lemmatizer = WordNetLemmatizer()
        else:
            self.lemmatizer = None
    
    def lemmatize(self, tokens):
        """
        Apply lemmatization to a list of tokens.
        
        Args:
            tokens: List of tokens to lemmatize
            
        Returns:
            List of lemmatized tokens
        """
        if not self.lemmatizer:
            # Fallback to basic rules
            return [self._simple_lemmatize(token) for token in tokens]
        
        if self.pos_tagging and NLTK_AVAILABLE:
            # Tag parts of speech first
            pos_tags = pos_tag(tokens)
            
            # Lemmatize with POS tags
            lemmatized = []
            for i, token in enumerate(tokens):
                try:
                    if i < len(pos_tags):
                        pos = self._get_wordnet_pos(pos_tags[i][1])
                        lemmatized_token = self.lemmatizer.lemmatize(token, pos=pos)
                    else:
                        lemmatized_token = self.lemmatizer.lemmatize(token)
                    
                    lemmatized.append(lemmatized_token)
                except:
                    # If error, fallback to token without POS
                    lemmatized.append(self.lemmatizer.lemmatize(token))
        else:
            # Simple lemmatization without POS tags
            lemmatized = [self.lemmatizer.lemmatize(token) for token in tokens]
        
        return lemmatized
    
    def _get_wordnet_pos(self, treebank_tag):
        """
        Convert Treebank POS tags to WordNet POS tags.
        
        Args:
            treebank_tag: Treebank POS tag
            
        Returns:
            WordNet POS tag
        """
        # Simple mapping from Treebank to WordNet POS tags
        if treebank_tag.startswith('J'):
            return 'a'  # Adjective
        elif treebank_tag.startswith('V'):
            return 'v'  # Verb
        elif treebank_tag.startswith('N'):
            return 'n'  # Noun
        elif treebank_tag.startswith('R'):
            return 'r'  # Adverb
        else:
            return 'n'  # Default to noun
    
    def _simple_lemmatize(self, token):
        """
        Simple rule-based lemmatizer as fallback.
        
        Args:
            token: Token to lemmatize
            
        Returns:
            Lemmatized token
        """
        # Basic lemmatization rules
        if token.endswith('ies'):
            return token[:-3] + 'y'
        elif token.endswith('ied'):
            return token[:-3] + 'y'
        elif token.endswith('s') and len(token) > 2 and token[-2] != 'ss':
            return token[:-1]
        elif token.endswith('ed') and len(token) > 3:
            return token[:-2]
        elif token.endswith('ing') and len(token) > 4:
            return token[:-3] + 'e'
        else:
            return token


class POSTagger:
    """
    Part-of-speech tagger for identifying grammatical categories of words.
    """
    
    def __init__(self, tagger_type='simple'):
        """
        Initialize the POS tagger.
        
        Args:
            tagger_type: Type of POS tagger ('nltk', 'simple')
        """
        self.tagger_type = tagger_type
        
        # Initialize NLTK POS tagger if available
        self.nltk_tagger = None
        if NLTK_AVAILABLE and tagger_type == 'nltk':
            try:
                self.nltk_tagger = nltk.data.load('taggers/averaged_perceptron_tagger/averaged_perceptron_tagger.pickle')
            except:
                self.nltk_tagger = nltk.pos_tag  # Fallback to default
    
    def tag(self, tokens):
        """
        Tag a list of tokens with their parts of speech.
        
        Args:
            tokens: List of tokens to tag
            
        Returns:
            List of (word, tag) tuples
        """
        if not tokens:
            return []
        
        if self.tagger_type == 'nltk' and self.nltk_tagger:
            return self.nltk_tagger(tokens)
        
        # Simple rule-based POS tagger as fallback
        return [(token, self._simple_pos_tag(token)) for token in tokens]
    
    def _simple_pos_tag(self, token):
        """
        Simple rule-based POS tagger as fallback.
        
        Args:
            token: Token to tag
            
        Returns:
            POS tag
        """
        token_lower = token.lower()
        
        # Noun patterns
        if token.endswith(('ness', 'ment', 'tion', 'sion', 'ity', 'er', 'ist')):
            return 'NN'
        
        # Verb patterns
        elif token.endswith(('ing', 'ed', 'ize')):
            return 'VB'
        
        # Adjective patterns
        elif token.endswith(('ful', 'ous', 'able', 'ive', 'less', 'est')):
            return 'JJ'
        
        # Adverb patterns
        elif token.endswith('ly'):
            return 'RB'
        
        # Default to noun
        return 'NN'


class NERecognizer:
    """
    Named Entity Recognition for identifying entities like people, organizations, and locations.
    """
    
    def __init__(self, use_nltk=True):
        """
        Initialize the NER recognizer.
        
        Args:
            use_nltk: Whether to use NLTK's NER if available
        """
        self.use_nltk = use_nltk
        
        # Simple patterns for entity recognition
        self.patterns = {
            'email': re.compile(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b'),
            'url': re.compile(r'https?://(?:[-\w.]])+(?:[:\d]+)?(?:/(?:[\w/_.])*(?:\?(?:[\w&=%.])*)?(?:#(?:\w*))?)?'),
            'phone': re.compile(r'(?:(?:\+?1[-\s]?)?(?:\(\d{3}\)|\d{3})[-\s]?\d{3}[-\s]?\d{4})'),
            'date': re.compile(r'\b\d{1,2}[/-]\d{1,2}[/-]\d{2,4}\b|\b\d{4}[/-]\d{1,2}[/-]\d{1,2}\b'),
            'money': re.compile(r'\$\d+(?:\.\d{2})?|\$\d,\d{3}(?:\.\d{2})?'),
            'time': re.compile(r'\b\d{1,2}:\d{2}(?::\d{2})?(?:\s(?:AM|PM|am|pm))?\b'),
            'percentage': re.compile(r'\b\d+(?:\.\d+)?%\b'),
            'person': re.compile(r'\b(?:Mr|Mrs|Ms|Dr|Prof)\.?\s[A-Z][a-z]+(?:\s[A-Z][a-z]+)*(?:\s(?:Jr|Sr|II|III|IV))?\b'),
            'organization': re.compile(r'\b[A-Z][a-z]+(?:\s(?:Inc|Ltd|LLC|Corp|Co|Corporation|Incorporated))?\b'),
            'location': re.compile(r'\b[A-Z][a-z]+(?:\s[A-Z][a-z]+)*(?:,\s[A-Z][a-z]+)*\b')
        }
    
    def extract_entities(self, text):
        """
        Extract named entities from text.
        
        Args:
            text: Input text
            
        Returns:
            Dictionary with entity types and their occurrences
        """
        entities = {
            'PERSON': [],
            'ORGANIZATION': [],
            'LOCATION': [],
            'DATE': [],
            'TIME': [],
            'MONEY': [],
            'PERCENTAGE': [],
            'PHONE': [],
            'EMAIL': [],
            'URL': []
        }
        
        # Try NLTK's named entity recognizer first
        if self.use_nltk and NLTK_AVAILABLE:
            try:
                tokens = word_tokenize(text)
                pos_tags = pos_tag(tokens)
                
                # Create chunks for NER
                chunks = ne_chunk(pos_tags)
                
                # Extract entities from chunks
                for chunk in chunks:
                    entity_type = None
                    entity_text = ""
                    
                    for word, tag in chunk:
                        entity_text += word + " "
                        
                        # Determine entity type based on chunk label
                        if isinstance(chunk, nltk.tree.Tree):
                            chunk_label = chunk.label()
                            
                            if chunk_label == 'PERSON':
                                entity_type = 'PERSON'
                            elif chunk_label == 'ORGANIZATION':
                                entity_type = 'ORGANIZATION'
                            elif chunk_label == 'GPE':
                                entity_type = 'LOCATION'
                            elif chunk_label == 'DATE':
                                entity_type = 'DATE'
                            elif chunk_label == 'TIME':
                                entity_type = 'TIME'
                            elif chunk_label in ('MONEY', 'PERCENT'):
                                entity_type = chunk_label
                    
                    if entity_type and entity_text.strip():
                        entities[entity_type].append(entity_text.strip())
                
                # Use pattern-based extraction as fallback
                self._extract_patterns(text, entities)
                return entities
            
            except:
                # Fallback to pattern-based extraction if NLTK fails
                pass
        
        # Pattern-based extraction
        self._extract_patterns(text, entities)
        
        return entities
    
    def _extract_patterns(self, text, entities):
        """Extract entities using regex patterns."""
        # Extract entities using patterns
        for entity_type, pattern in self.patterns.items():
            if entity_type in entities:  # Only for entity types we're looking for
                matches = pattern.findall(text)
                entities[entity_type].extend(matches)


class TextClassifier:
    """
    Text classification using various machine learning algorithms.
    """
    
    def __init__(self, algorithm='naive_bayes'):
        """
        Initialize the text classifier.
        
        Args:
            algorithm: Classification algorithm ('naive_bayes', 'logistic_regression', 'svm')
        """
        self.algorithm = algorithm
        self.vectorizer = None
        self.classifier = None
        self.is_trained = False
        self.label_encoder = {}
        self.label_decoder = {}
        
        # Initialize model based on algorithm
        if SKLEARN_AVAILABLE:
            if algorithm == 'naive_bayes':
                self.vectorizer = CountVectorizer()
                self.classifier = MultinomialNB()
            elif algorithm == 'logistic_regression':
                self.vectorizer = TfidfVectorizer()
                self.classifier = LogisticRegression()
            elif algorithm == 'svm':
                self.vectorizer = TfidfVectorizer()
                self.classifier = SVC(probability=True)
            else:
                raise ValueError(f"Unknown algorithm: {algorithm}")
        
        # Fallback to simple implementation if scikit-learn is not available
        elif NLTK_AVAILABLE and algorithm == 'naive_bayes':
            self.classifier = NaiveBayesClassifier(train=True)
    
    def _encode_labels(self, labels):
        """
        Encode string labels to integers.
        
        Args:
            labels: List of string labels
            
        Returns:
            List of encoded labels
        """
        encoded = []
        
        for label in labels:
            if label not in self.label_encoder:
                # Assign a new integer to this label
                self.label_encoder[label] = len(self.label_encoder)
                self.label_decoder[len(self.label_decoder)] = label
            
            encoded.append(self.label_encoder[label])
        
        return encoded
    
    def _decode_labels(self, encoded_labels):
        """
        Decode integer labels back to strings.
        
        Args:
            encoded_labels: List of encoded labels
            
        Returns:
            List of string labels
        """
        return [self.label_decoder[label] for label in encoded_labels]
    
    def train(self, texts, labels):
        """
        Train the classifier on the given texts and labels.
        
        Args:
            texts: List of training texts
            labels: List of corresponding labels
            
        Returns:
            bool: True if training successful, False otherwise
        """
        if not texts or not labels or len(texts) != len(labels):
            return False
        
        try:
            # Encode labels to integers
            encoded_labels = self._encode_labels(labels)
            
            if SKLEARN_AVAILABLE and self.vectorizer and self.classifier:
                # Vectorize texts
                X = self.vectorizer.fit_transform(texts)
                
                # Train classifier
                self.classifier.fit(X, encoded_labels)
                
            elif NLTK_AVAILABLE and self.algorithm == 'naive_bayes':
                # Prepare features for NLTK classifier
                train_data = []
                
                for text, label in zip(texts, encoded_labels):
                    # Simple word features
                    words = word_tokenize(text.lower())
                    words = [word for word in words if len(word) > 2]  # Filter short words
                    
                    features = {}
                    for word in words:
                        features[f'contains({word})'] = True
                    
                    train_data.append((features, label))
                
                # Train NLTK classifier
                self.classifier.train(train_data)
            else:
                return False
            
            self.is_trained = True
            return True
        
        except Exception as e:
            print(f"Training failed: {str(e)}")
            return False
    
    def predict(self, texts, return_probabilities=False):
        """
        Predict labels for the given texts.
        
        Args:
            texts: List of texts to classify
            return_probabilities: Whether to return probability estimates
            
        Returns:
            List of predicted labels or probabilities
        """
        if not self.is_trained:
            raise ValueError("Classifier has not been trained yet.")
        
        try:
            if SKLEARN_AVAILABLE and self.vectorizer and self.classifier:
                # Vectorize texts
                X = self.vectorizer.transform(texts)
                
                # Predict
                if return_probabilities and hasattr(self.classifier, 'predict_proba'):
                    encoded_predictions = self.classifier.predict_proba(X)
                else:
                    encoded_predictions = self.classifier.predict(X)
            
            elif NLTK_AVAILABLE and self.algorithm == 'naive_bayes':
                # Predict with NLTK classifier
                predictions = []
                
                for text in texts:
                    words = word_tokenize(text.lower())
                    words = [word for word in words if len(word) > 2]
                    
                    features = {}
                    for word in words:
                        features[f'contains({word})'] = True
                    
                    prediction = self.classifier.classify(features)
                    predictions.append(prediction)
                
                if return_probabilities:
                    # NLTK classifier doesn't provide probabilities easily
                    # We'll return one-hot encoded predictions
                    encoded_predictions = []
                    
                    for prediction in predictions:
                        proba = [0.0] * len(self.label_encoder)
                        proba[prediction] = 1.0
                        encoded_predictions.append(proba)
                else:
                    encoded_predictions = predictions
            else:
                return None
            
            # Decode labels
            return self._decode_labels(encoded_predictions)
        
        except Exception as e:
            print(f"Prediction failed: {str(e)}")
            return None
    
    def evaluate(self, test_texts, test_labels):
        """
        Evaluate the classifier on test data.
        
        Args:
            test_texts: List of test texts
            test_labels: List of true labels for the test texts
            
        Returns:
            Dictionary with evaluation metrics
        """
        if not self.is_trained:
            raise ValueError("Classifier has not been trained yet.")
        
        # Predict labels for test texts
        predicted_labels = self.predict(test_texts)
        
        if not predicted_labels:
            return None
        
        # Calculate evaluation metrics
        encoded_true = self._encode_labels(test_labels)
        encoded_pred = self._encode_labels(predicted_labels)
        
        # Calculate accuracy
        correct = sum(1 for true, pred in zip(encoded_true, encoded_pred) if true == pred)
        total = len(encoded_true)
        accuracy = correct / total if total > 0 else 0
        
        # Calculate precision, recall, F1 per class
        report = {}
        
        for label in self.label_encoder:
            label_idx = self.label_encoder[label]
            
            true_positives = sum(1 for true, pred in zip(encoded_true, encoded_pred) 
                             if true == label_idx and pred == label_idx)
            false_positives = sum(1 for true, pred in zip(encoded_true, encoded_pred) 
                             if true != label_idx and pred == label_idx)
            false_negatives = sum(1 for true, pred in zip(encoded_true, encoded_pred) 
                             if true != label_idx and pred != label_idx)
            true_negatives = sum(1 for true, pred in zip(encoded_true, encoded_pred) 
                             if true == label_idx and pred != label_idx)
            
            precision = true_positives / (true_positives + false_positives) if (true_positives + false_positives) > 0 else 0
            recall = true_positives / (true_positives + false_negatives) if (true_positives + false_negatives) > 0 else 0
            f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
            
            report[self.label_decoder[label_idx]] = {
                'precision': precision,
                'recall': recall,
                'f1_score': f1,
                'support': sum(1 for true in encoded_true if true == label_idx)
            }
        
        return {
            'accuracy': accuracy,
            'class_report': report
        }


class SentimentAnalyzer:
    """
    Sentiment analysis for determining the emotional tone of text.
    """
    
    def __init__(self, method='lexicon'):
        """
        Initialize the sentiment analyzer.
        
        Args:
            method: Sentiment analysis method ('lexicon', 'nltk')
        """
        self.method = method
        
        # Initialize lexicon-based approach
        if method == 'lexicon':
            self.positive_words = {
                'good', 'great', 'excellent', 'positive', 'amazing', 'awesome', 'fantastic',
                'wonderful', 'perfect', 'love', 'like', 'enjoy', 'happy', 'pleased',
                'satisfied', 'delighted', 'thrilled', 'excited', 'beautiful', 'nice',
                'awesome', 'best', 'better', 'improve', 'success', 'successful',
                'advantage', 'benefit', 'gain', 'win', 'achieve', 'accomplish'
            }
            
            self.negative_words = {
                'bad', 'terrible', 'awful', 'horrible', 'negative', 'poor', 'worst',
                'hate', 'dislike', 'disappointed', 'sad', 'angry', 'frustrated',
                'annoyed', 'upset', 'unhappy', 'dissatisfied', 'terrible', 'wrong',
                'fail', 'failed', 'failure', 'problem', 'issue', 'issues', 'broken',
                'disadvantage', 'loss', 'lose', 'waste', 'harm', 'hurt'
            }
        
        # Initialize NLTK's sentiment analyzer if available
        elif method == 'nltk' and NLTK_AVAILABLE:
            try:
                from nltk.sentiment import SentimentIntensityAnalyzer
                self.nltk_analyzer = SentimentIntensityAnalyzer()
            except ImportError:
                self.nltk_analyzer = None
                self.method = 'lexicon'  # Fallback to lexicon
        else:
            self.nltk_analyzer = None
    
    def analyze_sentiment(self, text):
        """
        Analyze the sentiment of the given text.
        
        Args:
            text: Input text to analyze
            
        Returns:
            Dictionary with sentiment scores and label
        """
        if not text:
            return {
                'polarity': 0.0,
                'subjectivity': 0.0,
                'label': 'neutral',
                'confidence': 0.0
            }
        
        if self.method == 'nltk' and self.nltk_analyzer:
            try:
                # Use NLTK's sentiment analyzer
                scores = self.nltk_analyzer.polarity_scores(text)
                
                return {
                    'polarity': scores['compound'],
                    'subjectivity': scores['subjectivity'],
                    'label': self._get_sentiment_label(scores['compound']),
                    'confidence': abs(scores['compound'])  # Using absolute compound score as confidence
                }
            except:
                # Fallback to lexicon if NLTK fails
                return self._lexicon_sentiment(text)
        else:
            # Use lexicon-based approach
            return self._lexicon_sentiment(text)
    
    def _lexicon_sentiment(self, text):
        """
        Lexicon-based sentiment analysis.
        
        Args:
            text: Input text to analyze
            
        Returns:
            Dictionary with sentiment scores and label
        """
        # Preprocess text
        preprocessor = TextPreprocessor()
        clean_text = preprocessor.clean_text(text)
        tokens = preprocessor.tokenize(clean_text, method='simple')
        tokens = [token.lower() for token in tokens]
        
        # Count positive and negative words
        positive_count = sum(1 for token in tokens if token in self.positive_words)
        negative_count = sum(1 for token in tokens if token in self.negative_words)
        
        total_tokens = len(tokens)
        
        # Skip if no tokens
        if total_tokens == 0:
            return {
                'polarity': 0.0,
                'subjectivity': 0.0,
                'label': 'neutral',
                'confidence': 0.0
            }
        
        # Calculate sentiment score
        positive_score = positive_count / total_tokens
        negative_score = negative_count / total_tokens
        
        # Calculate polarity (-1 to 1 range)
        polarity = positive_score - negative_score
        
        # Calculate subjectivity (0 to 1 range)
        subjectivity = (positive_count + negative_count) / total_tokens
        
        return {
            'polarity': polarity,
            'subjectivity': subjectivity,
            'label': self._get_sentiment_label(polarity),
            'confidence': abs(polarity)  # Using absolute polarity as confidence
        }
    
    def _get_sentiment_label(self, polarity):
        """
        Get sentiment label from polarity score.
        
        Args:
            polarity: Polarity score
            
        Returns:
            Sentiment label
        """
        if polarity > 0.1:
            return 'positive'
        elif polarity < -0.1:
            return 'negative'
        else:
            return 'neutral'
    
    def analyze_batch(self, texts):
        """
        Analyze sentiment for a batch of texts.
        
        Args:
            texts: List of texts to analyze
            
        Returns:
            List of sentiment analysis results
        """
        return [self.analyze_sentiment(text) for text in texts]


class TopicModeler:
    """
    Topic modeling for discovering abstract topics in documents.
    """
    
    def __init__(self, algorithm='lda', num_topics=5):
        """
        Initialize the topic modeler.
        
        Args:
            algorithm: Topic modeling algorithm ('lda')
            num_topics: Number of topics to discover
        """
        self.algorithm = algorithm
        self.num_topics = num_topics
        self.vectorizer = None
        self.model = None
        self.is_trained = False
        self.topic_word_dist = None
        
        # Initialize model based on algorithm
        if SKLEARN_AVAILABLE and algorithm == 'lda':
            self.vectorizer = CountVectorizer(max_df=0.8, min_df=5, stop_words='english')
            self.model = LatentDirichletAllocation(
                n_components=num_topics,
                max_iter=10,
                learning_method='online',
                random_state=42
            )
    
    def train(self, documents):
        """
        Train the topic model on the given documents.
        
        Args:
            documents: List of documents to train on
            
        Returns:
            bool: True if training successful, False otherwise
        """
        if not documents:
            return False
        
        try:
            if SKLEARN_AVAILABLE and self.algorithm == 'lda':
                # Vectorize documents
                X = self.vectorizer.fit_transform(documents)
                
                # Train LDA model
                self.model.fit(X)
                
                # Get topic-word distributions
                self.topic_word_dist = {}
                
                for topic_idx, topic in enumerate(self.model.components_):
                    # Get top words for this topic
                    top_words_idx = topic.argsort()[-10:][::-1]
                    top_words = [self.vectorizer.get_feature_names()[i] for i in top_words_idx]
                    self.topic_word_dist[topic_idx] = top_words
            else:
                return False
            
            self.is_trained = True
            return True
        
        except Exception as e:
            print(f"Training failed: {str(e)}")
            return False
    
    def get_document_topics(self, documents):
        """
        Get the topic distribution for documents.
        
        Args:
            documents: List of documents to analyze
            
        Returns:
            List of topic distributions for documents
        """
        if not self.is_trained:
            raise ValueError("Topic model has not been trained yet.")
        
        try:
            if SKLEARN_AVAILABLE and self.algorithm == 'lda':
                # Vectorize documents
                X = self.vectorizer.transform(documents)
                
                # Get topic distributions
                doc_topic_dist = self.model.transform(X)
                
                return doc_topic_dist.tolist()
            else:
                return None
        
        except Exception as e:
            print(f"Failed to get document topics: {str(e)}")
            return None
    
    def get_topic_words(self, topic_idx=None, n_words=10):
        """
        Get the top words for a topic.
        
        Args:
            topic_idx: Index of the topic (None for all topics)
            n_words: Number of top words to return
            
        Returns:
            Dictionary of topics and their top words
        """
        if not self.is_trained:
            raise ValueError("Topic model has not been trained yet.")
        
        if topic_idx is not None:
            if topic_idx < 0 or topic_idx >= self.num_topics:
                raise ValueError(f"Invalid topic index: {topic_idx}")
            
            return {topic_idx: self.topic_word_dist.get(topic_idx, [])[:n_words]}
        
        return self.topic_word_dist


class NLPipeline:
    """
    Complete NLP pipeline that combines multiple NLP components.
    """
    
    def __init__(self, task='text_classification', config=None):
        """
        Initialize the NLP pipeline.
        
        Args:
            task: Main NLP task ('text_classification', 'sentiment_analysis', 'ner', 'topic_modeling')
            config: Configuration dictionary for the pipeline
        """
        self.task = task
        self.config = config or {}
        
        # Initialize components based on task
        if task == 'text_classification':
            algorithm = self.config.get('algorithm', 'naive_bayes')
            self.text_classifier = TextClassifier(algorithm)
        
        elif task == 'sentiment_analysis':
            method = self.config.get('method', 'lexicon')
            self.sentiment_analyzer = SentimentAnalyzer(method)
        
        elif task == 'ner':
            use_nltk = self.config.get('use_nltk', True)
            self.ner_recognizer = NERecognizer(use_nltk)
        
        elif task == 'topic_modeling':
            algorithm = self.config.get('algorithm', 'lda')
            num_topics = self.config.get('num_topics', 5)
            self.topic_modeler = TopicModeler(algorithm, num_topics)
        
        # Initialize common components
        self.preprocessor = TextPreprocessor(language=self.config.get('language', 'english'))
        self.stemmer = Stemmer(stemmer_type=self.config.get('stemmer', 'porter'))
        self.lemmatizer = Lemmatizer(pos_tagging=self.config.get('pos_tagging', False))
        self.pos_tagger = POSTagger(tagger_type=self.config.get('pos_tagger', 'simple'))
    
    def process(self, texts):
        """
        Process texts based on the pipeline task.
        
        Args:
            texts: Texts to process
            
        Returns:
            Results based on the pipeline task
        """
        if not texts:
            return []
        
        if isinstance(texts, str):
            texts = [texts]
        
        # Preprocess texts
        preprocessed_texts = [self.preprocessor.clean_text(text) for text in texts]
        
        # Task-specific processing
        if self.task == 'text_classification':
            return self._classify_texts(preprocessed_texts, texts)
        
        elif self.task == 'sentiment_analysis':
            return self._analyze_sentiment(preprocessed_texts)
        
        elif self.task == 'ner':
            return self._extract_entities(preprocessed_texts, texts)
        
        elif self.task == 'topic_modeling':
            return self._discover_topics(preprocessed_texts)
        
        return []
    
    def _classify_texts(self, preprocessed_texts, original_texts):
        """Classify texts using the text classifier."""
        if hasattr(self, 'text_classifier'):
            # Train if not already trained
            if not self.text_classifier.is_trained and 'train_data' in self.config:
                train_texts = self.config['train_data']['texts']
                train_labels = self.config['train_data']['labels']
                self.text_classifier.train(train_texts, train_labels)
            
            # Predict
            return self.text_classifier.predict(preprocessed_texts)
        return []
    
    def _analyze_sentiment(self, preprocessed_texts):
        """Analyze sentiment of texts."""
        if hasattr(self, 'sentiment_analyzer'):
            return self.sentiment_analyzer.analyze_batch(preprocessed_texts)
        return []
    
    def _extract_entities(self, preprocessed_texts, original_texts):
        """Extract entities from texts."""
        if hasattr(self, 'ner_recognizer'):
            return [self.ner_recognizer.extract_entities(text) for text in original_texts]
        return []
    
    def _discover_topics(self, preprocessed_texts):
        """Discover topics in texts."""
        if hasattr(self, 'topic_modeler'):
            # Train if not already trained
            if not self.topic_modeler.is_trained:
                self.topic_modeler.train(preprocessed_texts)
            
            # Get document topics
            return self.topic_modeler.get_document_topics(preprocessed_texts)
        return []
    
    def save_model(self, filepath):
        """
        Save the trained model to a file.
        
        Args:
            filepath: Path to save the model
            
        Returns:
            bool: True if saved successfully, False otherwise
        """
        try:
            model_data = {
                'task': self.task,
                'config': self.config,
                'is_trained': False
            }
            
            # Save model data based on task
            if self.task == 'text_classification' and hasattr(self, 'text_classifier'):
                if self.text_classifier.is_trained:
                    model_data['is_trained'] = True
                    model_data['classifier'] = self.text_classifier.classifier
                    model_data['vectorizer'] = self.text_classifier.vectorizer
                    model_data['label_encoder'] = self.text_classifier.label_encoder
                    model_data['label_decoder'] = self.text_classifier.label_decoder
            
            elif self.task == 'topic_modeling' and hasattr(self, 'topic_modeler'):
                if self.topic_modeler.is_trained:
                    model_data['is_trained'] = True
                    model_data['model'] = self.topic_modeler.model
                    model_data['vectorizer'] = self.topic_modeler.vectorizer
                    model_data['topic_word_dist'] = self.topic_modeler.topic_word_dist
            
            with open(filepath, 'wb') as f:
                pickle.dump(model_data, f)
            
            return True
        
        except Exception as e:
            print(f"Failed to save model: {str(e)}")
            return False
    
    def load_model(self, filepath):
        """
        Load a saved model from a file.
        
        Args:
            filepath: Path to the saved model
            
        Returns:
            bool: True if loaded successfully, False otherwise
        """
        try:
            with open(filepath, 'rb') as f:
                model_data = pickle.load(f)
            
            # Check if the model is compatible with this pipeline
            if model_data.get('task') != self.task:
                print(f"Model task mismatch: {model_data.get('task')} != {self.task}")
                return False
            
            self.config = model_data.get('config', {})
            
            # Load model data based on task
            if self.task == 'text_classification' and hasattr(self, 'text_classifier'):
                if model_data.get('is_trained'):
                    self.text_classifier.classifier = model_data.get('classifier')
                    self.text_classifier.vectorizer = model_data.get('vectorizer')
                    self.text_classifier.label_encoder = model_data.get('label_encoder', {})
                    self.text_classifier.label_decoder = model_data.get('label_decoder', {})
                    self.text_classifier.is_trained = True
            
            elif self.task == 'topic_modeling' and hasattr(self, 'topic_modeler'):
                if model_data.get('is_trained'):
                    self.topic_modeler.model = model_data.get('model')
                    self.topic_modeler.vectorizer = model_data.get('vectorizer')
                    self.topic_modeler.topic_word_dist = model_data.get('topic_word_dist', {})
                    self.topic_modeler.is_trained = True
            
            return True
        
        except Exception as e:
            print(f"Failed to load model: {str(e)}")
            return False


def create_sample_usage():
    """Create sample usage of the NLP library."""
    
    print("NLP Library Sample Usage")
    print("=========================")
    
    # Sample texts for analysis
    texts = [
        "I love this product! It's amazing and works perfectly.",
        "This is terrible. The quality is poor and it broke after one day.",
        "It's okay, nothing special but does the job.",
        "Excellent value for money. Highly recommended!",
        "Not what I expected. Very disappointed."
    ]
    
    # Text preprocessing
    print("\n1. Text Preprocessing")
    preprocessor = TextPreprocessor()
    
    for i, text in enumerate(texts):
        cleaned = preprocessor.clean_text(text)
        tokens = preprocessor.tokenize(cleaned)
        filtered_tokens = preprocessor.remove_stopwords(tokens)
        
        print(f"Text {i+1}:")
        print(f"  Original: {text}")
        print(f"  Cleaned: {cleaned}")
        print(f"  Tokens: {tokens}")
        print(f"  Filtered: {filtered_tokens}")
    
    # Stemming and lemmatization
    print("\n2. Stemming and Lemmatization")
    stemmer = Stemmer('porter')
    lemmatizer = Lemmatizer()
    
    sample_tokens = ['running', 'ran', 'easily', 'studies', 'studying']
    
    print("Original tokens:", sample_tokens)
    print("Stemmed:", stemmer.stem(sample_tokens))
    
    if NLTK_AVAILABLE:
        print("Lemmatized:", lemmatizer.lemmatize(sample_tokens))
    else:
        print("Lemmatized:", [lemmatizer._simple_lemmatize(token) for token in sample_tokens])
    
    # Part-of-speech tagging
    print("\n3. Part-of-Speech Tagging")
    pos_tagger = POSTagger('simple')
    pos_tags = pos_tagger.tag(['The', 'quick', 'brown', 'fox', 'jumps', 'over', 'the', 'lazy', 'dog'])
    print("Tokens with POS tags:", pos_tags)
    
    # Named entity recognition
    print("\n4. Named Entity Recognition")
    ner = NERecognizer()
    sample_text = "Apple Inc. is based in Cupertino, California. Contact Tim Cook at tim@apple.com or visit https://www.apple.com."
    entities = ner.extract_entities(sample_text)
    print("Text:", sample_text)
    print("Entities:", entities)
    
    # Sentiment analysis
    print("\n5. Sentiment Analysis")
    sentiment_analyzer = SentimentAnalyzer()
    for i, text in enumerate(texts):
        result = sentiment_analyzer.analyze_sentiment(text)
        print(f"Text {i+1}: {result['label']} (polarity: {result['polarity']:.3f})")
    
    # Text classification
    print("\n6. Text Classification")
    classifier = TextClassifier('naive_bayes')
    train_texts = ["I love this movie", "I hate this movie", "This movie is okay", "Best movie ever!"]
    train_labels = ["positive", "negative", "neutral", "positive"]
    
    print("Training classifier...")
    if classifier.train(train_texts, train_labels):
        print("Training successful!")
        
        test_texts = ["This is great", "This is terrible", "It's not bad", "Amazing experience"]
        predictions = classifier.predict(test_texts)
        
        print("Test texts and predictions:")
        for text, prediction in zip(test_texts, predictions):
            print(f"  {text} -> {prediction}")
    else:
        print("Training failed!")
    
    # Topic modeling
    print("\n7. Topic Modeling")
    topic_modeler = TopicModeler('lda', num_topics=3)
    documents = [
        "The sky is blue and beautiful",
        "The sun is bright and yellow",
        "The ocean is deep and mysterious",
        "Trees are green and tall",
        "Mountains are majestic and high",
        "Rivers flow to the sea"
    ]
    
    print("Training topic model...")
    if topic_modeler.train(documents):
        print("Training successful!")
        
        doc_topics = topic_modeler.get_document_topics(documents)
        topic_words = topic_modeler.get_topic_words()
        
        print("Document topics:")
        for i, topic_dist in enumerate(doc_topics):
            dominant_topic = topic_dist.index(max(topic_dist))
            print(f"  Document {i+1}: Dominant topic {dominant_topic}")
        
        print("\nTopic words:")
        for topic_idx, words in topic_words.items():
            print(f"  Topic {topic_idx}: {', '.join(words)}")
    else:
        print("Training failed!")
    
    # Complete pipeline
    print("\n8. Complete NLP Pipeline")
    
    # Classification pipeline
    classification_config = {
        'train_data': {
            'texts': train_texts,
            'labels': train_labels
        },
        'algorithm': 'naive_bayes'
    }
    
    classification_pipeline = NLPipeline('text_classification', classification_config)
    pipeline_predictions = classification_pipeline.process(test_texts)
    
    print("Pipeline predictions:", pipeline_predictions)
    
    # Sentiment analysis pipeline
    sentiment_config = {
        'method': 'lexicon'
    }
    
    sentiment_pipeline = NLPipeline('sentiment_analysis', sentiment_config)
    pipeline_sentiments = sentiment_pipeline.process(test_texts)
    
    print("Pipeline sentiments:", [s['label'] for s in pipeline_sentiments])
    
    # Save and load models
    print("\n9. Model Persistence")
    
    # Save classification model
    if classification_pipeline.text_classifier.is_trained:
        classification_pipeline.save_model('classification_model.pkl')
        print("Classification model saved")
    
    # Save topic model
    if topic_modeler.is_trained:
        topic_modeler.save_model('topic_model.pkl')
        print("Topic model saved")
    
    # Load models in a new pipeline
    print("\n10. Loading Models")
    
    new_classification_pipeline = NLPipeline('text_classification', classification_config)
    if new_classification_pipeline.load_model('classification_model.pkl'):
        print("Classification model loaded successfully")
        
        new_predictions = new_classification_pipeline.process(["This is good", "This is bad"])
        print("New pipeline predictions:", new_predictions)
    
    new_topic_pipeline = NLPipeline('topic_modeling', {'num_topics': 3})
    if new_topic_pipeline.load_model('topic_model.pkl'):
        print("Topic model loaded successfully")


def main():
    """Main function to demonstrate the NLP library."""
    print("Natural Language Processing Library")
    print("===============================")
    
    # Check availability of external libraries
    print("Checking dependencies:")
    print(f"NLTK available: {'Yes' if NLTK_AVAILABLE else 'No'}")
    print(f"Scikit-learn available: {'Yes' if SKLEARN_AVAILABLE else 'No'}")
    
    # Download NLTK data if available
    if NLTK_AVAILABLE:
        try:
            nltk.download('stopwords', quiet=True)
            nltk.download('punkt', quiet=True)
            nltk.download('averaged_perceptron_tagger', quiet=True)
            nltk.download('wordnet', quiet=True)
            nltk.download('maxent_ne_chunker', quiet=True)
            print("NLTK data downloaded successfully")
        except:
            print("Failed to download NLTK data")
    
    # Run sample usage
    try:
        create_sample_usage()
        print("\nNLP library demonstration completed successfully!")
    except Exception as e:
        print(f"Error running demonstration: {str(e)}")


if __name__ == "__main__":
    main()
<｜fim▁hole｜>