"""
Data Analytics and Visualization System

This module implements a comprehensive data analytics and visualization system
that can handle various data formats, perform statistical analysis, and generate
visualizations.

Features:
- Data loading from various sources (CSV, JSON, Excel)
- Data cleaning and preprocessing
- Statistical analysis (descriptive stats, correlation, regression)
- Data visualization (plots, charts, graphs)
- Machine learning models (classification, clustering)
- Export results to various formats
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.linear_model import LinearRegression, LogisticRegression
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, mean_squared_error, r2_score
from scipy import stats
from scipy.stats import pearsonr, spearmanr
import json
import os
import re
import datetime as dt
import warnings
from typing import List, Dict, Tuple, Optional, Union, Any

warnings.filterwarnings('ignore')

class DataAnalyzer:
    """
    A comprehensive data analysis class that provides various methods for
    data loading, cleaning, analysis, and visualization.
    """
    
    def __init__(self, data=None):
        """
        Initialize the DataAnalyzer with optional data.
        
        Args:
            data: Initial data (pandas DataFrame, numpy array, or dict)
        """
        self.data = None
        self.original_data = None
        self.data_history = []
        self.column_types = {}
        self.missing_values = {}
        self.outliers = {}
        self.analysis_results = {}
        
        if data is not None:
            self.load_data(data)
    
    def load_data(self, data, source_type="auto", **kwargs):
        """
        Load data from various sources.
        
        Args:
            data: Data source (file path, URL, or data object)
            source_type: Type of data source ("auto", "csv", "json", "excel", "dataframe", "array")
            **kwargs: Additional arguments for data loading
            
        Returns:
            bool: True if data loaded successfully, False otherwise
        """
        try:
            if source_type == "auto":
                if isinstance(data, str):
                    # Determine source type from file extension
                    if data.endswith('.csv'):
                        source_type = "csv"
                    elif data.endswith('.json'):
                        source_type = "json"
                    elif data.endswith(('.xls', '.xlsx')):
                        source_type = "excel"
                    else:
                        raise ValueError("Unsupported file format")
                elif isinstance(data, pd.DataFrame):
                    source_type = "dataframe"
                elif isinstance(data, np.ndarray):
                    source_type = "array"
                else:
                    raise ValueError("Unsupported data type")
            
            if source_type == "csv":
                self.data = pd.read_csv(data, **kwargs)
            elif source_type == "json":
                self.data = pd.read_json(data, **kwargs)
            elif source_type == "excel":
                self.data = pd.read_excel(data, **kwargs)
            elif source_type == "dataframe":
                self.data = data.copy()
            elif source_type == "array":
                self.data = pd.DataFrame(data)
            else:
                raise ValueError(f"Unsupported source type: {source_type}")
            
            # Store original data and update metadata
            self.original_data = self.data.copy()
            self._update_metadata()
            
            return True
        except Exception as e:
            print(f"Error loading data: {str(e)}")
            return False
    
    def save_data(self, filepath, format="csv", **kwargs):
        """
        Save the current data to file.
        
        Args:
            filepath: Path to save the data
            format: Format to save in ("csv", "json", "excel")
            **kwargs: Additional arguments for saving
            
        Returns:
            bool: True if data saved successfully, False otherwise
        """
        try:
            if format == "csv":
                self.data.to_csv(filepath, index=False, **kwargs)
            elif format == "json":
                self.data.to_json(filepath, **kwargs)
            elif format == "excel":
                self.data.to_excel(filepath, index=False, **kwargs)
            else:
                raise ValueError(f"Unsupported format: {format}")
            
            return True
        except Exception as e:
            print(f"Error saving data: {str(e)}")
            return False
    
    def get_data_info(self):
        """
        Get information about the loaded data.
        
        Returns:
            dict: Dictionary containing data information
        """
        if self.data is None:
            return "No data loaded"
        
        info = {
            "shape": self.data.shape,
            "columns": list(self.data.columns),
            "dtypes": self.data.dtypes.to_dict(),
            "memory_usage": self.data.memory_usage(deep=True).sum(),
            "column_types": self.column_types,
            "missing_values": self.missing_values,
            "outliers": self.outliers
        }
        
        return info
    
    def display_data(self, rows=5):
        """
        Display the first n rows of the data.
        
        Args:
            rows: Number of rows to display
        """
        if self.data is None:
            print("No data loaded")
            return
        
        print(f"Shape: {self.data.shape}")
        print(f"Columns: {list(self.data.columns)}")
        print("\nFirst few rows:")
        print(self.data.head(rows))
    
    def _update_metadata(self):
        """Update metadata about the data."""
        if self.data is None:
            return
        
        # Update column types
        for col in self.data.columns:
            if pd.api.types.is_numeric_dtype(self.data[col]):
                self.column_types[col] = "numeric"
            elif pd.api.types.is_datetime64_dtype(self.data[col]):
                self.column_types[col] = "datetime"
            else:
                self.column_types[col] = "categorical"
        
        # Update missing values
        self.missing_values = self.data.isnull().sum().to_dict()
        
        # Update outliers (using IQR method)
        self.outliers = {}
        for col in self.data.select_dtypes(include=[np.number]).columns:
            Q1 = self.data[col].quantile(0.25)
            Q3 = self.data[col].quantile(0.75)
            IQR = Q3 - Q1
            lower_bound = Q1 - 1.5 * IQR
            upper_bound = Q3 + 1.5 * IQR
            
            outliers = self.data[(self.data[col] < lower_bound) | (self.data[col] > upper_bound)]
            self.outliers[col] = len(outliers)
    
    def clean_data(self, options=None):
        """
        Clean the data based on specified options.
        
        Args:
            options: Dictionary of cleaning options
                - "drop_missing": columns with missing values (list or "all")
                - "fill_missing": how to fill missing values ("mean", "median", "mode", "forward", "backward")
                - "remove_duplicates": True/False
                - "convert_types": True/False
                - "remove_outliers": columns to remove outliers from (list or "all")
                - "outlier_method": method to detect outliers ("iqr", "zscore", "isolation")
        
        Returns:
            bool: True if cleaning successful, False otherwise
        """
        if self.data is None:
            print("No data loaded")
            return False
        
        # Save current state before making changes
        self.data_history.append(self.data.copy())
        
        # Default options
        if options is None:
            options = {
                "drop_missing": [],
                "fill_missing": None,
                "remove_duplicates": True,
                "convert_types": True,
                "remove_outliers": [],
                "outlier_method": "iqr"
            }
        
        try:
            # Drop columns with missing values
            if options["drop_missing"]:
                if options["drop_missing"] == "all":
                    self.data = self.data.dropna(axis=1)
                else:
                    self.data = self.data.drop(columns=options["drop_missing"])
            
            # Fill missing values
            if options["fill_missing"]:
                if options["fill_missing"] == "mean":
                    self.data = self.data.fillna(self.data.mean())
                elif options["fill_missing"] == "median":
                    self.data = self.data.fillna(self.data.median())
                elif options["fill_missing"] == "mode":
                    self.data = self.data.fillna(self.data.mode().iloc[0])
                elif options["fill_missing"] == "forward":
                    self.data = self.data.fillna(method='ffill')
                elif options["fill_missing"] == "backward":
                    self.data = self.data.fillna(method='bfill')
                else:
                    self.data = self.data.fillna(options["fill_missing"])
            
            # Remove duplicates
            if options["remove_duplicates"]:
                self.data = self.data.drop_duplicates()
            
            # Convert types
            if options["convert_types"]:
                for col in self.data.columns:
                    if self.data[col].dtype == 'object':
                        # Try to convert to datetime
                        try:
                            self.data[col] = pd.to_datetime(self.data[col])
                        except:
                            # Try to convert to numeric
                            try:
                                self.data[col] = pd.to_numeric(self.data[col])
                            except:
                                pass  # Keep as object
            
            # Remove outliers
            if options["remove_outliers"]:
                if options["remove_outliers"] == "all":
                    columns = self.data.select_dtypes(include=[np.number]).columns
                else:
                    columns = options["remove_outliers"]
                
                method = options.get("outlier_method", "iqr")
                
                for col in columns:
                    if method == "iqr":
                        Q1 = self.data[col].quantile(0.25)
                        Q3 = self.data[col].quantile(0.75)
                        IQR = Q3 - Q1
                        lower_bound = Q1 - 1.5 * IQR
                        upper_bound = Q3 + 1.5 * IQR
                        
                        self.data = self.data[
                            (self.data[col] >= lower_bound) & 
                            (self.data[col] <= upper_bound)
                        ]
                    elif method == "zscore":
                        z_scores = np.abs(stats.zscore(self.data[col]))
                        self.data = self.data[z_scores < 3]
            
            # Update metadata
            self._update_metadata()
            
            return True
        except Exception as e:
            print(f"Error cleaning data: {str(e)}")
            return False
    
    def undo_last_operation(self):
        """
        Undo the last data operation.
        
        Returns:
            bool: True if undo successful, False otherwise
        """
        if not self.data_history:
            print("No operations to undo")
            return False
        
        self.data = self.data_history.pop()
        self._update_metadata()
        return True
    
    def describe_data(self, columns=None):
        """
        Generate descriptive statistics for the data.
        
        Args:
            columns: List of columns to analyze (None for all)
            
        Returns:
            dict: Dictionary of descriptive statistics
        """
        if self.data is None:
            return "No data loaded"
        
        if columns is None:
            columns = list(self.data.columns)
        
        result = {}
        
        for col in columns:
            if col not in self.data.columns:
                continue
                
            if self.column_types[col] == "numeric":
                desc = {
                    "count": self.data[col].count(),
                    "mean": self.data[col].mean(),
                    "median": self.data[col].median(),
                    "mode": self.data[col].mode().iloc[0] if not self.data[col].mode().empty else None,
                    "std": self.data[col].std(),
                    "min": self.data[col].min(),
                    "25%": self.data[col].quantile(0.25),
                    "50%": self.data[col].quantile(0.5),
                    "75%": self.data[col].quantile(0.75),
                    "max": self.data[col].max(),
                    "skewness": self.data[col].skew(),
                    "kurtosis": self.data[col].kurtosis(),
                    "range": self.data[col].max() - self.data[col].min()
                }
            elif self.column_types[col] == "datetime":
                desc = {
                    "count": self.data[col].count(),
                    "min": self.data[col].min(),
                    "max": self.data[col].max(),
                    "range_days": (self.data[col].max() - self.data[col].min()).days
                }
            else:  # categorical
                desc = {
                    "count": self.data[col].count(),
                    "unique": self.data[col].nunique(),
                    "top": self.data[col].mode().iloc[0] if not self.data[col].mode().empty else None,
                    "freq": self.data[col].value_counts().iloc[0] if not self.data[col].value_counts().empty else 0
                }
            
            result[col] = desc
        
        self.analysis_results["descriptive_stats"] = result
        return result
    
    def correlation_analysis(self, method="pearson", columns=None):
        """
        Perform correlation analysis on numeric columns.
        
        Args:
            method: Correlation method ("pearson", "spearman", "kendall")
            columns: List of columns to analyze (None for all numeric)
            
        Returns:
            DataFrame: Correlation matrix
        """
        if self.data is None:
            return "No data loaded"
        
        numeric_cols = self.data.select_dtypes(include=[np.number]).columns
        
        if columns:
            numeric_cols = [col for col in numeric_cols if col in columns]
        
        if len(numeric_cols) < 2:
            return "Not enough numeric columns for correlation analysis"
        
        correlation_matrix = self.data[numeric_cols].corr(method=method)
        
        self.analysis_results[f"correlation_{method}"] = correlation_matrix
        return correlation_matrix
    
    def correlation_significance(self, method="pearson", alpha=0.05, columns=None):
        """
        Calculate correlation coefficients and p-values.
        
        Args:
            method: Correlation method ("pearson", "spearman")
            alpha: Significance level
            columns: List of columns to analyze (None for all numeric)
            
        Returns:
            dict: Dictionary with correlation coefficients and p-values
        """
        if self.data is None:
            return "No data loaded"
        
        numeric_cols = self.data.select_dtypes(include=[np.number]).columns
        
        if columns:
            numeric_cols = [col for col in numeric_cols if col in columns]
        
        if len(numeric_cols) < 2:
            return "Not enough numeric columns for correlation analysis"
        
        result = {}
        
        for i, col1 in enumerate(numeric_cols):
            for j, col2 in enumerate(numeric_cols):
                if i >= j:  # Skip diagonal and lower triangle
                    continue
                
                # Drop missing values for this pair
                valid_data = self.data[[col1, col2]].dropna()
                
                if len(valid_data) < 3:  # Need at least 3 points
                    continue
                
                if method == "pearson":
                    corr, p_value = pearsonr(valid_data[col1], valid_data[col2])
                elif method == "spearman":
                    corr, p_value = spearmanr(valid_data[col1], valid_data[col2])
                else:
                    return f"Unsupported method: {method}"
                
                key = f"{col1} vs {col2}"
                result[key] = {
                    "correlation": corr,
                    "p_value": p_value,
                    "significant": p_value < alpha
                }
        
        self.analysis_results[f"correlation_significance_{method}"] = result
        return result
    
    def hypothesis_test(self, test_type, column, value=None, column2=None, alpha=0.05):
        """
        Perform hypothesis testing.
        
        Args:
            test_type: Type of test ("ttest_1samp", "ttest_ind", "chi2", "anova", "mannwhitney")
            column: Column name for the test
            value: Value to test against (for 1-sample tests)
            column2: Second column for 2-sample tests
            alpha: Significance level
            
        Returns:
            dict: Dictionary with test results
        """
        if self.data is None:
            return "No data loaded"
        
        try:
            if test_type == "ttest_1samp":
                if column not in self.data.columns:
                    return f"Column '{column}' not found"
                
                if value is None:
                    return "Value parameter required for 1-sample t-test"
                
                # Remove missing values
                data = self.data[column].dropna()
                
                if len(data) < 2:
                    return "Not enough data for t-test"
                
                t_stat, p_value = stats.ttest_1samp(data, value)
                
                result = {
                    "test": "1-Sample t-test",
                    "column": column,
                    "test_value": value,
                    "t_statistic": t_stat,
                    "p_value": p_value,
                    "significant": p_value < alpha,
                    "sample_mean": data.mean(),
                    "sample_std": data.std()
                }
            
            elif test_type == "ttest_ind":
                if column not in self.data.columns or (column2 and column2 not in self.data.columns):
                    return "One or both columns not found"
                
                # Remove missing values
                data1 = self.data[column].dropna()
                data2 = self.data[column2].dropna() if column2 else None
                
                if len(data1) < 2 or (data2 is not None and len(data2) < 2):
                    return "Not enough data for independent t-test"
                
                if data2 is None:
                    # If only one column, split into two groups
                    median = data1.median()
                    group1 = data1[data1 >= median]
                    group2 = data1[data1 < median]
                else:
                    group1, group2 = data1, data2
                
                t_stat, p_value = stats.ttest_ind(group1, group2)
                
                result = {
                    "test": "Independent t-test",
                    "column1": column,
                    "column2": column2 or f"{column} (split by median)",
                    "t_statistic": t_stat,
                    "p_value": p_value,
                    "significant": p_value < alpha,
                    "group1_mean": group1.mean(),
                    "group2_mean": group2.mean()
                }
            
            elif test_type == "chi2":
                if column not in self.data.columns or (column2 and column2 not in self.data.columns):
                    return "One or both columns not found"
                
                if column2 is None:
                    return "Second column required for chi-square test"
                
                # Create contingency table
                contingency_table = pd.crosstab(self.data[column], self.data[column2])
                
                chi2, p_value, dof, expected = stats.chi2_contingency(contingency_table)
                
                result = {
                    "test": "Chi-Square Test",
                    "column1": column,
                    "column2": column2,
                    "chi2": chi2,
                    "p_value": p_value,
                    "degrees_of_freedom": dof,
                    "significant": p_value < alpha,
                    "contingency_table": contingency_table.to_dict()
                }
            
            elif test_type == "anova":
                if column not in self.data.columns:
                    return f"Column '{column}' not found"
                
                if column2 is None:
                    return "Grouping column required for ANOVA"
                
                if column2 not in self.data.columns:
                    return f"Column '{column2}' not found"
                
                # Get groups
                groups = [group[column].dropna() for name, group in self.data.groupby(column2)]
                
                if len(groups) < 2:
                    return "Not enough groups for ANOVA"
                
                f_stat, p_value = stats.f_oneway(*groups)
                
                result = {
                    "test": "ANOVA",
                    "column": column,
                    "group_by": column2,
                    "f_statistic": f_stat,
                    "p_value": p_value,
                    "significant": p_value < alpha,
                    "groups": {name: len(group) for name, group in self.data.groupby(column2)}
                }
            
            elif test_type == "mannwhitney":
                if column not in self.data.columns or (column2 and column2 not in self.data.columns):
                    return "One or both columns not found"
                
                # Remove missing values
                data1 = self.data[column].dropna()
                data2 = self.data[column2].dropna() if column2 else None
                
                if len(data1) < 2 or (data2 is not None and len(data2) < 2):
                    return "Not enough data for Mann-Whitney U test"
                
                if data2 is None:
                    # If only one column, split into two groups
                    median = data1.median()
                    group1 = data1[data1 >= median]
                    group2 = data1[data1 < median]
                else:
                    group1, group2 = data1, data2
                
                u_stat, p_value = stats.mannwhitneyu(group1, group2)
                
                result = {
                    "test": "Mann-Whitney U Test",
                    "column1": column,
                    "column2": column2 or f"{column} (split by median)",
                    "u_statistic": u_stat,
                    "p_value": p_value,
                    "significant": p_value < alpha,
                    "group1_median": group1.median(),
                    "group2_median": group2.median()
                }
            
            else:
                return f"Unsupported test type: {test_type}"
            
            self.analysis_results[f"hyp_test_{test_type}"] = result
            return result
        
        except Exception as e:
            return f"Error in hypothesis test: {str(e)}"
    
    def regression_analysis(self, target_col, predictor_cols=None, test_size=0.3, random_state=42):
        """
        Perform linear regression analysis.
        
        Args:
            target_col: Target column name
            predictor_cols: List of predictor column names (None for all other numeric)
            test_size: Fraction of data to use for testing
            random_state: Random seed for reproducibility
            
        Returns:
            dict: Dictionary with regression results
        """
        if self.data is None:
            return "No data loaded"
        
        if target_col not in self.data.columns:
            return f"Target column '{target_col}' not found"
        
        # Default to all other numeric columns
        if predictor_cols is None:
            predictor_cols = [col for col in self.data.columns 
                            if col != target_col and self.column_types[col] == "numeric"]
        
        if not predictor_cols:
            return "No valid predictor columns"
        
        # Check if all columns exist
        missing_cols = [col for col in predictor_cols if col not in self.data.columns]
        if missing_cols:
            return f"Predictor columns not found: {missing_cols}"
        
        try:
            # Prepare data
            data = self.data[[target_col] + predictor_cols].dropna()
            
            X = data[predictor_cols]
            y = data[target_col]
            
            # Split data
            X_train, X_test, y_train, y_test = train_test_split(
                X, y, test_size=test_size, random_state=random_state
            )
            
            # Fit model
            model = LinearRegression()
            model.fit(X_train, y_train)
            
            # Make predictions
            y_train_pred = model.predict(X_train)
            y_test_pred = model.predict(X_test)
            
            # Calculate metrics
            train_r2 = r2_score(y_train, y_train_pred)
            test_r2 = r2_score(y_test, y_test_pred)
            train_mse = mean_squared_error(y_train, y_train_pred)
            test_mse = mean_squared_error(y_test, y_test_pred)
            
            # Create results dictionary
            result = {
                "model": "Linear Regression",
                "target": target_col,
                "predictors": predictor_cols,
                "coefficients": dict(zip(predictor_cols, model.coef_)),
                "intercept": model.intercept_,
                "train_r2": train_r2,
                "test_r2": test_r2,
                "train_mse": train_mse,
                "test_mse": test_mse,
                "train_samples": len(X_train),
                "test_samples": len(X_test)
            }
            
            self.analysis_results["regression"] = result
            return result
        
        except Exception as e:
            return f"Error in regression analysis: {str(e)}"
    
    def classification_analysis(self, target_col, predictor_cols=None, test_size=0.3, 
                             random_state=42, max_iter=1000):
        """
        Perform logistic regression classification.
        
        Args:
            target_col: Target column name
            predictor_cols: List of predictor column names (None for all other numeric)
            test_size: Fraction of data to use for testing
            random_state: Random seed for reproducibility
            max_iter: Maximum iterations for logistic regression
            
        Returns:
            dict: Dictionary with classification results
        """
        if self.data is None:
            return "No data loaded"
        
        if target_col not in self.data.columns:
            return f"Target column '{target_col}' not found"
        
        # Default to all other numeric columns
        if predictor_cols is None:
            predictor_cols = [col for col in self.data.columns 
                            if col != target_col and self.column_types[col] == "numeric"]
        
        if not predictor_cols:
            return "No valid predictor columns"
        
        # Check if all columns exist
        missing_cols = [col for col in predictor_cols if col not in self.data.columns]
        if missing_cols:
            return f"Predictor columns not found: {missing_cols}"
        
        try:
            # Prepare data
            data = self.data[[target_col] + predictor_cols].dropna()
            
            X = data[predictor_cols]
            y = data[target_col]
            
            # Check if target is binary
            unique_values = y.nunique()
            if unique_values != 2:
                return f"Target column must have exactly 2 unique values, found {unique_values}"
            
            # Split data
            X_train, X_test, y_train, y_test = train_test_split(
                X, y, test_size=test_size, random_state=random_state
            )
            
            # Standardize features
            scaler = StandardScaler()
            X_train_scaled = scaler.fit_transform(X_train)
            X_test_scaled = scaler.transform(X_test)
            
            # Fit model
            model = LogisticRegression(max_iter=max_iter)
            model.fit(X_train_scaled, y_train)
            
            # Make predictions
            y_train_pred = model.predict(X_train_scaled)
            y_test_pred = model.predict(X_test_scaled)
            
            # Calculate metrics
            train_accuracy = accuracy_score(y_train, y_train_pred)
            test_accuracy = accuracy_score(y_test, y_test_pred)
            
            # Create results dictionary
            result = {
                "model": "Logistic Regression",
                "target": target_col,
                "predictors": predictor_cols,
                "coefficients": dict(zip(predictor_cols, model.coef_[0])),
                "intercept": model.intercept_[0],
                "train_accuracy": train_accuracy,
                "test_accuracy": test_accuracy,
                "train_samples": len(X_train),
                "test_samples": len(X_test),
                "classes": list(model.classes_)
            }
            
            self.analysis_results["classification"] = result
            return result
        
        except Exception as e:
            return f"Error in classification analysis: {str(e)}"
    
    def clustering_analysis(self, columns=None, n_clusters=3, random_state=42):
        """
        Perform K-means clustering analysis.
        
        Args:
            columns: List of columns to use for clustering (None for all numeric)
            n_clusters: Number of clusters to create
            random_state: Random seed for reproducibility
            
        Returns:
            dict: Dictionary with clustering results
        """
        if self.data is None:
            return "No data loaded"
        
        # Default to all numeric columns
        if columns is None:
            columns = [col for col in self.data.columns if self.column_types[col] == "numeric"]
        
        if len(columns) < 2:
            return "Need at least 2 numeric columns for clustering"
        
        # Check if all columns exist
        missing_cols = [col for col in columns if col not in self.data.columns]
        if missing_cols:
            return f"Columns not found: {missing_cols}"
        
        try:
            # Prepare data
            data = self.data[columns].dropna()
            
            # Standardize features
            scaler = StandardScaler()
            data_scaled = scaler.fit_transform(data)
            
            # Fit model
            model = KMeans(n_clusters=n_clusters, random_state=random_state)
            cluster_labels = model.fit_predict(data_scaled)
            
            # Add cluster labels to original data
            clustered_data = data.copy()
            clustered_data['cluster'] = cluster_labels
            
            # Calculate cluster statistics
            cluster_stats = {}
            for cluster_id in range(n_clusters):
                cluster_data = clustered_data[clustered_data['cluster'] == cluster_id]
                cluster_stats[cluster_id] = {
                    "size": len(cluster_data),
                    "percentage": len(cluster_data) / len(clustered_data) * 100
                }
                
                for col in columns:
                    cluster_stats[cluster_id][f"{col}_mean"] = cluster_data[col].mean()
            
            # Calculate inertia and silhouette score
            inertia = model.inertia_
            
            # Create results dictionary
            result = {
                "model": "K-Means",
                "n_clusters": n_clusters,
                "columns": columns,
                "inertia": inertia,
                "cluster_centers": model.cluster_centers_.tolist(),
                "cluster_stats": cluster_stats,
                "cluster_labels": cluster_labels.tolist(),
                "scaled_data": data_scaled.tolist()
            }
            
            self.analysis_results["clustering"] = result
            return result
        
        except Exception as e:
            return f"Error in clustering analysis: {str(e)}"
    
    def create_visualization(self, chart_type, columns=None, **kwargs):
        """
        Create various types of visualizations.
        
        Args:
            chart_type: Type of chart ("histogram", "scatter", "box", "bar", "line", "heatmap", "pair")
            columns: List of columns to visualize
            **kwargs: Additional parameters for the visualization
            
        Returns:
            matplotlib.figure.Figure: The created figure object
        """
        if self.data is None:
            print("No data loaded")
            return None
        
        # Default columns
        if columns is None:
            if chart_type == "heatmap":
                columns = [col for col in self.data.columns if self.column_types[col] == "numeric"]
            elif chart_type in ["scatter", "line"]:
                # Select first two numeric columns
                numeric_cols = [col for col in self.data.columns if self.column_types[col] == "numeric"]
                if len(numeric_cols) >= 2:
                    columns = numeric_cols[:2]
                else:
                    print("Not enough numeric columns for this chart type")
                    return None
            elif chart_type == "pair":
                # Select first few numeric columns
                columns = [col for col in self.data.columns if self.column_types[col] == "numeric"][:4]
            else:
                columns = [self.data.columns[0]]
        
        # Check if columns exist
        if isinstance(columns, list):
            missing_cols = [col for col in columns if col not in self.data.columns]
            if missing_cols:
                print(f"Columns not found: {missing_cols}")
                return None
        
        try:
            plt.figure(figsize=kwargs.get('figsize', (10, 6)))
            
            if chart_type == "histogram":
                if len(columns) < 1:
                    return None
                
                column = columns[0]
                bins = kwargs.get('bins', 30)
                alpha = kwargs.get('alpha', 0.7)
                
                plt.hist(self.data[column].dropna(), bins=bins, alpha=alpha)
                plt.title(f'Histogram of {column}')
                plt.xlabel(column)
                plt.ylabel('Frequency')
                plt.grid(True)
            
            elif chart_type == "scatter":
                if len(columns) < 2:
                    return None
                
                x_col, y_col = columns[0], columns[1]
                alpha = kwargs.get('alpha', 0.6)
                
                plt.scatter(self.data[x_col], self.data[y_col], alpha=alpha)
                plt.title(f'{x_col} vs {y_col}')
                plt.xlabel(x_col)
                plt.ylabel(y_col)
                plt.grid(True)
            
            elif chart_type == "box":
                if len(columns) < 1:
                    return None
                
                # Create box plots
                if len(columns) == 1:
                    column = columns[0]
                    plt.boxplot(self.data[column].dropna())
                    plt.title(f'Box Plot of {column}')
                    plt.ylabel(column)
                else:
                    data_to_plot = [self.data[col].dropna() for col in columns]
                    plt.boxplot(data_to_plot, labels=columns)
                    plt.title('Box Plot Comparison')
                    plt.ylabel('Values')
                
                plt.grid(True)
            
            elif chart_type == "bar":
                if len(columns) < 1:
                    return None
                
                column = columns[0]
                
                # If categorical column with numeric column
                if len(columns) >= 2 and self.column_types[columns[0]] == "categorical":
                    group_col, value_col = columns[0], columns[1]
                    grouped_data = self.data.groupby(group_col)[value_col].mean()
                    
                    plt.bar(grouped_data.index, grouped_data.values)
                    plt.title(f'Average {value_col} by {group_col}')
                    plt.xlabel(group_col)
                    plt.ylabel(f'Average {value_col}')
                    
                    # Rotate x-axis labels if many categories
                    if len(grouped_data) > 5:
                        plt.xticks(rotation=45)
                else:
                    # Value counts for categorical column
                    if self.column_types[column] == "categorical":
                        value_counts = self.data[column].value_counts()
                        plt.bar(value_counts.index, value_counts.values)
                        plt.title(f'Frequency of {column}')
                        plt.xlabel(column)
                        plt.ylabel('Frequency')
                        
                        # Rotate x-axis labels if many categories
                        if len(value_counts) > 5:
                            plt.xticks(rotation=45)
                    else:
                        print("For bar chart, use categorical column")
                        return None
                
                plt.grid(True)
            
            elif chart_type == "line":
                if len(columns) < 1:
                    return None
                
                if len(columns) == 1:
                    # If single column, plot against index
                    column = columns[0]
                    plt.plot(self.data.index, self.data[column])
                    plt.title(f'Line Plot of {column}')
                    plt.xlabel('Index')
                    plt.ylabel(column)
                else:
                    # If two columns, first is x-axis, second is y-axis
                    x_col, y_col = columns[0], columns[1]
                    
                    # Sort by x-axis if it's numeric
                    if self.column_types[x_col] == "numeric":
                        sorted_data = self.data.sort_values(x_col)
                        plt.plot(sorted_data[x_col], sorted_data[y_col])
                    else:
                        plt.plot(self.data[x_col], self.data[y_col])
                    
                    plt.title(f'{x_col} vs {y_col}')
                    plt.xlabel(x_col)
                    plt.ylabel(y_col)
                
                plt.grid(True)
            
            elif chart_type == "heatmap":
                if len(columns) < 2:
                    return None
                
                # Calculate correlation matrix
                correlation_matrix = self.data[columns].corr()
                
                sns.heatmap(
                    correlation_matrix, 
                    annot=kwargs.get('annot', True), 
                    cmap=kwargs.get('cmap', 'coolwarm'),
                    center=kwargs.get('center', 0)
                )
                plt.title('Correlation Heatmap')
            
            elif chart_type == "pair":
                if len(columns) < 2:
                    return None
                
                # Create pair plot using seaborn
                sns.pairplot(self.data[columns].dropna())
                plt.suptitle('Pair Plot', y=1.02)
            
            else:
                print(f"Unsupported chart type: {chart_type}")
                return None
            
            plt.tight_layout()
            return plt.gcf()
        
        except Exception as e:
            print(f"Error creating visualization: {str(e)}")
            return None
    
    def save_visualization(self, figure, filepath, dpi=300, bbox_inches='tight'):
        """
        Save a visualization to file.
        
        Args:
            figure: Matplotlib figure object
            filepath: Path to save the figure
            dpi: Resolution in dots per inch
            bbox_inches: Bounding box setting
            
        Returns:
            bool: True if saved successfully, False otherwise
        """
        try:
            figure.savefig(filepath, dpi=dpi, bbox_inches=bbox_inches)
            return True
        except Exception as e:
            print(f"Error saving visualization: {str(e)}")
            return False
    
    def export_analysis_results(self, filepath, format="json"):
        """
        Export analysis results to file.
        
        Args:
            filepath: Path to save the results
            format: Export format ("json", "csv", "excel")
            
        Returns:
            bool: True if exported successfully, False otherwise
        """
        if not self.analysis_results:
            print("No analysis results to export")
            return False
        
        try:
            if format == "json":
                with open(filepath, 'w') as f:
                    # Convert numpy arrays to lists for JSON serialization
                    json_results = {}
                    for key, value in self.analysis_results.items():
                        if isinstance(value, dict):
                            json_results[key] = {}
                            for subkey, subvalue in value.items():
                                if hasattr(subvalue, 'tolist'):
                                    json_results[key][subkey] = subvalue.tolist()
                                else:
                                    json_results[key][subkey] = subvalue
                        else:
                            json_results[key] = value
                    
                    json.dump(json_results, f, indent=2, default=str)
            
            elif format == "csv":
                # For CSV, flatten the results and save as a table
                rows = []
                for analysis_type, results in self.analysis_results.items():
                    if isinstance(results, dict):
                        for key, value in results.items():
                            if not isinstance(value, (dict, list, np.ndarray)):
                                rows.append({
                                    'analysis_type': analysis_type,
                                    'parameter': key,
                                    'value': str(value)
                                })
                
                result_df = pd.DataFrame(rows)
                result_df.to_csv(filepath, index=False)
            
            elif format == "excel":
                # For Excel, save each analysis as a separate sheet
                with pd.ExcelWriter(filepath) as writer:
                    for analysis_type, results in self.analysis_results.items():
                        if isinstance(results, pd.DataFrame):
                            results.to_excel(writer, sheet_name=analysis_type, index=False)
                        elif isinstance(results, dict):
                            # Convert dictionary to DataFrame
                            df = pd.DataFrame(
                                [(k, v) for k, v in results.items()],
                                columns=['Parameter', 'Value']
                            )
                            df.to_excel(writer, sheet_name=analysis_type, index=False)
            
            return True
        
        except Exception as e:
            print(f"Error exporting results: {str(e)}")
            return False
    
    def get_recommendations(self):
        """
        Generate data analysis recommendations based on the data characteristics.
        
        Returns:
            dict: Dictionary with recommendations
        """
        if self.data is None:
            return {"error": "No data loaded"}
        
        recommendations = {
            "data_quality": [],
            "analysis_suggestions": [],
            "visualization_suggestions": []
        }
        
        # Data quality recommendations
        for col in self.data.columns:
            missing_pct = (self.missing_values.get(col, 0) / len(self.data)) * 100
            if missing_pct > 0:
                if missing_pct < 5:
                    recommendations["data_quality"].append(
                        f"Consider filling missing values in '{col}' ({missing_pct:.1f}% missing)"
                    )
                elif missing_pct < 30:
                    recommendations["data_quality"].append(
                        f"Column '{col}' has significant missing values ({missing_pct:.1f}%), consider imputation"
                    )
                else:
                    recommendations["data_quality"].append(
                        f"Column '{col}' has high missing values ({missing_pct:.1f}%), consider removal"
                    )
            
            # Outlier recommendations
            if col in self.outliers and self.outliers[col] > 0:
                outlier_pct = (self.outliers[col] / len(self.data)) * 100
                if outlier_pct > 5:
                    recommendations["data_quality"].append(
                        f"Column '{col}' has many outliers ({outlier_pct:.1f}%), investigate data quality"
                    )
                else:
                    recommendations["data_quality"].append(
                        f"Consider handling outliers in '{col}' ({self.outliers[col]} detected)"
                    )
        
        # Analysis suggestions
        numeric_cols = [col for col in self.data.columns if self.column_types[col] == "numeric"]
        categorical_cols = [col for col in self.data.columns if self.column_types[col] == "categorical"]
        datetime_cols = [col for col in self.data.columns if self.column_types[col] == "datetime"]
        
        if len(numeric_cols) >= 2:
            recommendations["analysis_suggestions"].append(
                f"Consider correlation analysis between {numeric_cols[0]} and {numeric_cols[1]}"
            )
            recommendations["analysis_suggestions"].append(
                "Consider regression analysis with potential target variable"
            )
        
        if len(numeric_cols) >= 3:
            recommendations["analysis_suggestions"].append(
                "Consider clustering analysis to identify natural groups"
            )
        
        if len(categorical_cols) >= 1 and len(numeric_cols) >= 1:
            recommendations["analysis_suggestions"].append(
                f"Consider group-by analysis on {categorical_cols[0]} with numeric variables"
            )
        
        if len(categorical_cols) >= 2:
            recommendations["analysis_suggestions"].append(
                "Consider chi-square test to assess relationships between categorical variables"
            )
        
        if datetime_cols:
            recommendations["analysis_suggestions"].append(
                "Consider time series analysis or trend analysis"
            )
        
        # Visualization suggestions
        if numeric_cols:
            recommendations["visualization_suggestions"].append(
                f"Create histogram for {numeric_cols[0]} to understand distribution"
            )
        
        if len(numeric_cols) >= 2:
            recommendations["visualization_suggestions"].append(
                f"Create scatter plot between {numeric_cols[0]} and {numeric_cols[1]}"
            )
            recommendations["visualization_suggestions"].append(
                "Create correlation heatmap for all numeric variables"
            )
        
        if len(numeric_cols) >= 3:
            recommendations["visualization_suggestions"].append(
                "Create pair plots to visualize relationships between multiple variables"
            )
        
        if categorical_cols:
            recommendations["visualization_suggestions"].append(
                f"Create bar chart for {categorical_cols[0]} to show value counts"
            )
        
        if len(categorical_cols) >= 1 and len(numeric_cols) >= 1:
            recommendations["visualization_suggestions"].append(
                f"Create grouped bar chart showing {numeric_cols[0]} by {categorical_cols[0]}"
            )
        
        return recommendations


class DataVisualizationApp:
    """
    Interactive data visualization application using matplotlib.
    """
    
    def __init__(self, analyzer=None):
        """
        Initialize the application with optional DataAnalyzer.
        
        Args:
            analyzer: DataAnalyzer instance
        """
        self.analyzer = analyzer if analyzer else DataAnalyzer()
        self.figures = []
    
    def run_interactive_mode(self):
        """Run the application in interactive mode."""
        print("Data Visualization App")
        print("====================")
        print("Commands:")
        print("  load <file> - Load data from file")
        print("  info - Display data information")
        print("  describe - Get descriptive statistics")
        print("  correlation - Perform correlation analysis")
        print("  regression <target> - Perform regression analysis")
        print("  classification <target> - Perform classification analysis")
        print("  clustering - Perform clustering analysis")
        print("  visualize <chart_type> <columns> - Create visualization")
        print("  recommendations - Get analysis recommendations")
        print("  save <filename> - Save current data")
        print("  export_results <filename> - Export analysis results")
        print("  quit - Exit the application")
        
        while True:
            try:
                command = input("\nEnter command: ").strip()
                parts = command.split()
                
                if not parts:
                    continue
                
                cmd = parts[0].lower()
                
                if cmd == "quit":
                    print("Exiting application...")
                    break
                
                elif cmd == "load":
                    if len(parts) < 2:
                        print("Please specify a file to load")
                        continue
                    
                    file_path = " ".join(parts[1:])
                    if os.path.exists(file_path):
                        if self.analyzer.load_data(file_path):
                            print(f"Data loaded from {file_path}")
                            self.analyzer.display_data()
                        else:
                            print("Failed to load data")
                    else:
                        print(f"File not found: {file_path}")
                
                elif cmd == "info":
                    info = self.analyzer.get_data_info()
                    if isinstance(info, dict):
                        print("Data Information:")
                        for key, value in info.items():
                            if key == "shape":
                                print(f"  {key}: {value[0]} rows, {value[1]} columns")
                            elif key == "memory_usage":
                                print(f"  {key}: {value / (1024*1024):.2f} MB")
                            else:
                                print(f"  {key}: {value}")
                    else:
                        print(info)
                
                elif cmd == "describe":
                    stats = self.analyzer.describe_data()
                    if isinstance(stats, dict):
                        for col, desc in stats.items():
                            print(f"\nColumn: {col}")
                            for stat, value in desc.items():
                                if isinstance(value, float):
                                    print(f"  {stat}: {value:.4f}")
                                else:
                                    print(f"  {stat}: {value}")
                    else:
                        print(stats)
                
                elif cmd == "correlation":
                    corr = self.analyzer.correlation_analysis()
                    if isinstance(corr, pd.DataFrame):
                        print("Correlation Matrix:")
                        print(corr.round(3))
                    else:
                        print(corr)
                
                elif cmd == "regression":
                    if len(parts) < 2:
                        print("Please specify a target column")
                        continue
                    
                    target_col = " ".join(parts[1:])
                    result = self.analyzer.regression_analysis(target_col)
                    
                    if isinstance(result, dict):
                        print(f"\nRegression Analysis Results:")
                        print(f"  Target: {result['target']}")
                        print(f"  R² (train): {result['train_r2']:.4f}")
                        print(f"  R² (test): {result['test_r2']:.4f}")
                        print(f"  MSE (train): {result['train_mse']:.4f}")
                        print(f"  MSE (test): {result['test_mse']:.4f}")
                        
                        print("\nCoefficients:")
                        for pred, coef in result['coefficients'].items():
                            print(f"  {pred}: {coef:.4f}")
                    else:
                        print(result)
                
                elif cmd == "classification":
                    if len(parts) < 2:
                        print("Please specify a target column")
                        continue
                    
                    target_col = " ".join(parts[1:])
                    result = self.analyzer.classification_analysis(target_col)
                    
                    if isinstance(result, dict):
                        print(f"\nClassification Analysis Results:")
                        print(f"  Target: {result['target']}")
                        print(f"  Accuracy (train): {result['train_accuracy']:.4f}")
                        print(f"  Accuracy (test): {result['test_accuracy']:.4f}")
                        
                        print("\nCoefficients:")
                        for pred, coef in result['coefficients'].items():
                            print(f"  {pred}: {coef:.4f}")
                    else:
                        print(result)
                
                elif cmd == "clustering":
                    result = self.analyzer.clustering_analysis()
                    
                    if isinstance(result, dict):
                        print(f"\nClustering Analysis Results:")
                        print(f"  Number of clusters: {result['n_clusters']}")
                        print(f"  Inertia: {result['inertia']:.4f}")
                        
                        print("\nCluster Statistics:")
                        for cluster_id, stats in result['cluster_stats'].items():
                            print(f"  Cluster {cluster_id}:")
                            for stat, value in stats.items():
                                if isinstance(value, float):
                                    print(f"    {stat}: {value:.4f}")
                                else:
                                    print(f"    {stat}: {value}")
                    else:
                        print(result)
                
                elif cmd == "visualize":
                    if len(parts) < 2:
                        print("Please specify a chart type")
                        continue
                    
                    chart_type = parts[1].lower()
                    columns = None
                    
                    if len(parts) > 2:
                        columns = parts[2:]
                    
                    figure = self.analyzer.create_visualization(chart_type, columns)
                    if figure:
                        self.figures.append(figure)
                        plt.show()
                    else:
                        print("Failed to create visualization")
                
                elif cmd == "recommendations":
                    recommendations = self.analyzer.get_recommendations()
                    
                    for category, items in recommendations.items():
                        print(f"\n{category.title()}:")
                        for item in items:
                            print(f"  - {item}")
                
                elif cmd == "save":
                    if len(parts) < 2:
                        print("Please specify a filename")
                        continue
                    
                    filename = " ".join(parts[1:])
                    if self.analyzer.save_data(filename):
                        print(f"Data saved to {filename}")
                    else:
                        print("Failed to save data")
                
                elif cmd == "export_results":
                    if len(parts) < 2:
                        print("Please specify a filename")
                        continue
                    
                    filename = " ".join(parts[1:])
                    if self.analyzer.export_analysis_results(filename):
                        print(f"Results exported to {filename}")
                    else:
                        print("Failed to export results")
                
                else:
                    print(f"Unknown command: {cmd}")
            
            except KeyboardInterrupt:
                print("\nOperation cancelled")
            except Exception as e:
                print(f"Error: {str(e)}")
    
    def batch_visualization(self, config):
        """
        Create multiple visualizations based on a configuration.
        
        Args:
            config: Configuration dictionary specifying visualizations
        """
        if not config.get("visualizations"):
            print("No visualizations specified in config")
            return
        
        output_dir = config.get("output_dir", "visualizations")
        os.makedirs(output_dir, exist_ok=True)
        
        for i, viz_config in enumerate(config["visualizations"]):
            chart_type = viz_config.get("type")
            columns = viz_config.get("columns")
            filename = viz_config.get("filename", f"chart_{i+1}")
            
            figure = self.analyzer.create_visualization(chart_type, columns, **viz_config)
            
            if figure:
                filepath = os.path.join(output_dir, f"{filename}.png")
                if self.analyzer.save_visualization(figure, filepath):
                    print(f"Saved {chart_type} chart to {filepath}")
                else:
                    print(f"Failed to save {chart_type} chart")


def create_sample_data(filepath, data_type="sales"):
    """
    Create sample data for testing the data analyzer.
    
    Args:
        filepath: Path to save the data
        data_type: Type of data to create ("sales", "customers", "products", "mixed")
    """
    np.random.seed(42)
    
    if data_type == "sales":
        # Sales data
        n_records = 1000
        data = {
            "order_id": range(1, n_records + 1),
            "product_id": np.random.randint(1, 100, n_records),
            "customer_id": np.random.randint(1, 200, n_records),
            "order_date": pd.date_range("2022-01-01", periods=n_records, freq="D"),
            "quantity": np.random.randint(1, 10, n_records),
            "unit_price": np.random.uniform(10, 500, n_records),
            "discount_percent": np.random.uniform(0, 0.2, n_records),
            "shipping_cost": np.random.uniform(5, 20, n_records)
        }
        
        # Calculate total amount
        df = pd.DataFrame(data)
        df["total_amount"] = df["quantity"] * df["unit_price"] * (1 - df["discount_percent"]) + df["shipping_cost"]
        
        # Add some missing values
        mask = np.random.random(len(df)) < 0.05
        df.loc[mask, "discount_percent"] = np.nan
        
    elif data_type == "customers":
        # Customer data
        n_records = 500
        data = {
            "customer_id": range(1, n_records + 1),
            "name": [f"Customer {i}" for i in range(1, n_records + 1)],
            "email": [f"customer{i}@example.com" for i in range(1, n_records + 1)],
            "age": np.random.randint(18, 80, n_records),
            "gender": np.random.choice(["Male", "Female", "Other"], n_records),
            "city": np.random.choice(["New York", "Los Angeles", "Chicago", "Houston", "Phoenix"], n_records),
            "state": np.random.choice(["NY", "CA", "IL", "TX", "AZ"], n_records),
            "country": np.random.choice(["USA", "Canada", "Mexico"], n_records, p=[0.8, 0.15, 0.05]),
            "income": np.random.normal(50000, 15000, n_records),
            "join_date": pd.date_range("2020-01-01", periods=n_records, freq="W"),
            "last_purchase_date": pd.date_range("2022-01-01", periods=n_records, freq="D"),
            "total_purchases": np.random.randint(1, 100, n_records),
            "total_spent": np.random.uniform(100, 10000, n_records)
        }
        
        df = pd.DataFrame(data)
        
        # Add some missing values
        mask = np.random.random(len(df)) < 0.1
        df.loc[mask, "income"] = np.nan
        
    elif data_type == "products":
        # Product data
        n_records = 200
        categories = ["Electronics", "Clothing", "Home & Garden", "Sports", "Books", "Toys"]
        brands = ["BrandA", "BrandB", "BrandC", "BrandD", "BrandE"]
        
        data = {
            "product_id": range(1, n_records + 1),
            "name": [f"Product {i}" for i in range(1, n_records + 1)],
            "category": np.random.choice(categories, n_records),
            "brand": np.random.choice(brands, n_records),
            "price": np.random.uniform(5, 500, n_records),
            "cost": np.random.uniform(2, 250, n_records),
            "weight": np.random.uniform(0.1, 10, n_records),
            "height": np.random.uniform(1, 50, n_records),
            "width": np.random.uniform(1, 50, n_records),
            "depth": np.random.uniform(1, 50, n_records),
            "in_stock": np.random.choice([True, False], n_records, p=[0.9, 0.1]),
            "quantity": np.random.randint(0, 1000, n_records),
            "rating": np.random.uniform(1, 5, n_records),
            "reviews": np.random.randint(0, 500, n_records),
            "added_date": pd.date_range("2021-01-01", periods=n_records, freq="W"),
            "last_modified": pd.date_range("2022-01-01", periods=n_records, freq="D")
        }
        
        df = pd.DataFrame(data)
        
        # Add some missing values
        mask = np.random.random(len(df)) < 0.05
        df.loc[mask, "rating"] = np.nan
        
    else:  # mixed data
        # Mixed data with various types
        n_records = 1000
        data = {
            "id": range(1, n_records + 1),
            "date": pd.date_range("2020-01-01", periods=n_records, freq="D"),
            "category": np.random.choice(["A", "B", "C", "D", "E"], n_records),
            "value1": np.random.normal(50, 10, n_records),
            "value2": np.random.exponential(2, n_records),
            "value3": np.random.uniform(0, 100, n_records),
            "flag": np.random.choice([True, False], n_records, p=[0.3, 0.7]),
            "description": [f"Description for item {i}" for i in range(1, n_records + 1)]
        }
        
        df = pd.DataFrame(data)
        
        # Add some missing values and outliers
        mask = np.random.random(len(df)) < 0.05
        df.loc[mask, "value1"] = np.nan
        
        # Add outliers
        outlier_mask = np.random.random(len(df)) < 0.02
        df.loc[outlier_mask, "value2"] = np.random.uniform(20, 30)
    
    # Save data
    df.to_csv(filepath, index=False)
    print(f"Sample {data_type} data created and saved to {filepath}")


def main():
    """
    Main function to demonstrate the DataAnalytics system.
    """
    print("Data Analytics and Visualization System")
    print("=====================================")
    
    # Create sample data if it doesn't exist
    data_file = "sample_sales_data.csv"
    if not os.path.exists(data_file):
        create_sample_data(data_file, "sales")
    
    # Initialize the analyzer
    analyzer = DataAnalyzer()
    
    # Load data
    analyzer.load_data(data_file)
    analyzer.display_data()
    
    # Get data information
    info = analyzer.get_data_info()
    print("\nData Information:")
    for key, value in info.items():
        if key == "shape":
            print(f"  {key}: {value[0]} rows, {value[1]} columns")
        elif key == "memory_usage":
            print(f"  {key}: {value / (1024*1024):.2f} MB")
        else:
            print(f"  {key}: {value}")
    
    # Get descriptive statistics
    print("\nDescriptive Statistics:")
    stats = analyzer.describe_data()
    for col, desc in list(stats.items())[:3]:  # Show first 3 columns
        print(f"\nColumn: {col}")
        for stat, value in list(desc.items())[:5]:  # Show first 5 stats
            if isinstance(value, float):
                print(f"  {stat}: {value:.4f}")
            else:
                print(f"  {stat}: {value}")
    
    # Correlation analysis
    print("\nCorrelation Analysis:")
    corr = analyzer.correlation_analysis()
    if isinstance(corr, pd.DataFrame):
        print(corr.round(3))
    
    # Create visualizations
    print("\nCreating visualizations...")
    
    # Histogram
    fig1 = analyzer.create_visualization("histogram", ["total_amount"], bins=30)
    if fig1:
        analyzer.save_visualization(fig1, "histogram_total_amount.png")
        print("Histogram saved")
    
    # Scatter plot
    fig2 = analyzer.create_visualization("scatter", ["quantity", "total_amount"])
    if fig2:
        analyzer.save_visualization(fig2, "scatter_quantity_vs_total.png")
        print("Scatter plot saved")
    
    # Box plot
    fig3 = analyzer.create_visualization("box", ["quantity", "unit_price"])
    if fig3:
        analyzer.save_visualization(fig3, "boxplot_numeric_vars.png")
        print("Box plot saved")
    
    # Correlation heatmap
    numeric_cols = [col for col in analyzer.data.columns if analyzer.column_types[col] == "numeric"][:5]
    if len(numeric_cols) >= 2:
        fig4 = analyzer.create_visualization("heatmap", numeric_cols)
        if fig4:
            analyzer.save_visualization(fig4, "correlation_heatmap.png")
            print("Correlation heatmap saved")
    
    # Regression analysis
    print("\nRegression Analysis:")
    reg_result = analyzer.regression_analysis("total_amount", ["quantity", "unit_price"])
    if isinstance(reg_result, dict):
        print(f"Target: {reg_result['target']}")
        print(f"R² (test): {reg_result['test_r2']:.4f}")
        print("Coefficients:")
        for pred, coef in reg_result['coefficients'].items():
            print(f"  {pred}: {coef:.4f}")
    
    # Get recommendations
    print("\nRecommendations:")
    recommendations = analyzer.get_recommendations()
    for category, items in recommendations.items():
        print(f"\n{category.title()}:")
        for item in items[:3]:  # Show first 3 recommendations
            print(f"  - {item}")
    
    # Export results
    analyzer.export_analysis_results("analysis_results.json")
    print("\nAnalysis results exported to analysis_results.json")
    
    # Interactive mode prompt
    print("\nWould you like to run in interactive mode? (y/n)")
    if input().lower() == 'y':
        app = DataVisualizationApp(analyzer)
        app.run_interactive_mode()


if __name__ == "__main__":
    main()
<｜fim▁hole｜>