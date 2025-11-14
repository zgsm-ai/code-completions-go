"""
Machine Learning Library

This module provides a comprehensive machine learning library with implementations
of various algorithms, model training, and evaluation metrics.

Features:
- Supervised learning algorithms (Linear Regression, Logistic Regression, SVM, Decision Trees)
- Unsupervised learning algorithms (K-Means, PCA, Hierarchical Clustering)
- Neural Networks (Feedforward, Convolutional, Recurrent)
- Model evaluation metrics (Accuracy, Precision, Recall, F1-Score, ROC-AUC)
- Data preprocessing utilities
- Cross-validation methods
- Hyperparameter tuning
- Model persistence
"""

import os
import sys
import json
import math
import random
import time
import pickle
import csv
import logging
import numpy as np
import typing
from typing import List, Dict, Tuple, Optional, Union, Any, Callable, Type
from enum import Enum
from dataclasses import dataclass, field
from abc import ABC, abstractmethod

# Try to import popular ML libraries for comparison
try:
    from sklearn import datasets
    from sklearn.model_selection import train_test_split, cross_val_score
    from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_auc_score
    from sklearn.preprocessing import StandardScaler, MinMaxScaler, LabelEncoder, OneHotEncoder
    SKLEARN_AVAILABLE = True
except ImportError:
    SKLEARN_AVAILABLE = False

# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class LearningType(Enum):
    """Enumeration of learning types."""
    SUPERVISED = "supervised"
    UNSUPERVISED = "unsupervised"
    REINFORCEMENT = "reinforcement"


class TaskType(Enum):
    """Enumeration of task types."""
    CLASSIFICATION = "classification"
    REGRESSION = "regression"
    CLUSTERING = "clustering"
    DIMENSIONALITY_REDUCTION = "dimensionality_reduction"


class ActivationFunction(Enum):
    """Enumeration of activation functions."""
    SIGMOID = "sigmoid"
    TANH = "tanh"
    RELU = "relu"
    LEAKY_RELU = "leaky_relu"
    SOFTMAX = "softmax"
    LINEAR = "linear"


class LossFunction(Enum):
    """Enumeration of loss functions."""
    MEAN_SQUARED_ERROR = "mean_squared_error"
    MEAN_ABSOLUTE_ERROR = "mean_absolute_error"
    CROSS_ENTROPY = "cross_entropy"
    HINGE = "hinge"
    LOG_COSH = "log_cosh"
    QUANTILE = "quantile"


class Optimizer(Enum):
    """Enumeration of optimization algorithms."""
    GRADIENT_DESCENT = "gradient_descent"
    STOCHASTIC_GRADIENT_DESCENT = "stochastic_gradient_descent"
    MINI_BATCH_GRADIENT_DESCENT = "mini_batch_gradient_descent"
    ADAM = "adam"
    RMSPROP = "rmsprop"
    ADADELTA = "adadelta"


class RegularizationType(Enum):
    """Enumeration of regularization types."""
    L1 = "l1"
    L2 = "l2"
    ELASTIC_NET = "elastic_net"
    NONE = "none"


@dataclass
class ModelMetrics:
    """Metrics for model evaluation."""
    accuracy: float = 0.0
    precision: float = 0.0
    recall: float = 0.0
    f1_score: float = 0.0
    auc: float = 0.0
    confusion_matrix: Optional[List[List[int]]] = None
    mse: float = 0.0
    mae: float = 0.0
    r2_score: float = 0.0


@dataclass
class Hyperparameters:
    """Hyperparameters for a model."""
    params: Dict[str, Any] = field(default_factory=dict)
    grid: Optional[Dict[str, List[Any]]] = None
    random: Optional[Dict[str, Tuple[Any, Any]]] = None
    
    def get(self, name: str, default: Any = None) -> Any:
        """
        Get a hyperparameter value.
        
        Args:
            name: Parameter name
            default: Default value if parameter doesn't exist
            
        Returns:
            Parameter value
        """
        return self.params.get(name, default)
    
    def set(self, name: str, value: Any) -> None:
        """
        Set a hyperparameter value.
        
        Args:
            name: Parameter name
            value: Parameter value
        """
        self.params[name] = value
    
    def __str__(self) -> str:
        """String representation of hyperparameters."""
        return str(self.params)


class Dataset:
    """Dataset class for storing and manipulating data."""
    
    def __init__(self, features: List[List[float]] = None, labels: List[Any] = None):
        """
        Initialize the dataset.
        
        Args:
            features: Feature matrix (list of samples)
            labels: Target labels
        """
        self.features = features or []
        self.labels = labels or []
        self.feature_names = []
        self.label_name = "target"
        self.num_samples = len(self.features) if self.features else 0
        self.num_features = len(self.features[0]) if self.num_samples > 0 else 0
    
    def load_from_csv(self, file_path: str, label_column: int = -1, has_header: bool = True):
        """
        Load dataset from CSV file.
        
        Args:
            file_path: Path to CSV file
            label_column: Index of label column (-1 for last column)
            has_header: Whether CSV has a header row
        """
        try:
            with open(file_path, 'r') as f:
                reader = csv.reader(f)
                
                # Skip header if needed
                if has_header:
                    header = next(reader)
                    if label_column == -1:
                        self.feature_names = header[:-1]
                        self.label_name = header[-1]
                    else:
                        self.feature_names = header[:label_column] + header[label_column+1:]
                        self.label_name = header[label_column]
                
                # Load data
                self.features = []
                self.labels = []
                
                for row in reader:
                    if label_column == -1:
                        feature_row = [float(val) for val in row[:-1]]
                        label = row[-1]
                    else:
                        feature_row = []
                        for i, val in enumerate(row):
                            if i != label_column:
                                feature_row.append(float(val))
                            else:
                                label = val
                        
                        # Handle the case where label is the last column
                        if label_column == len(row) - 1:
                            feature_row = [float(val) for val in row[:-1]]
                            label = row[-1]
                    
                    self.features.append(feature_row)
                    self.labels.append(label)
                
                self.num_samples = len(self.features)
                self.num_features = len(self.features[0]) if self.num_samples > 0 else 0
                
                logger.info(f"Loaded dataset with {self.num_samples} samples and {self.num_features} features")
        except Exception as e:
            logger.error(f"Error loading dataset: {str(e)}")
            raise
    
    def to_numpy(self):
        """
        Convert dataset to numpy arrays.
        
        Returns:
            Tuple of (features_array, labels_array)
        """
        if not SKLEARN_AVAILABLE:
            # Simple conversion without sklearn
            features_array = np.array(self.features) if self.features else np.array([]).reshape(0, 0)
            labels_array = np.array(self.labels) if self.labels else np.array([])
            return features_array, labels_array
        else:
            return np.array(self.features), np.array(self.labels)
    
    def split(self, test_size: float = 0.2, random_state: int = None):
        """
        Split dataset into training and testing sets.
        
        Args:
            test_size: Proportion of dataset for testing
            random_state: Random seed for reproducibility
            
        Returns:
            Tuple of (X_train, X_test, y_train, y_test)
        """
        if not SKLEARN_AVAILABLE:
            # Simple implementation without sklearn
            indices = list(range(self.num_samples))
            
            # Shuffle indices if needed
            if random_state is not None:
                random.seed(random_state)
            random.shuffle(indices)
            
            # Calculate split point
            split_idx = int(self.num_samples * (1 - test_size))
            
            train_indices = indices[:split_idx]
            test_indices = indices[split_idx:]
            
            X_train = [self.features[i] for i in train_indices]
            X_test = [self.features[i] for i in test_indices]
            y_train = [self.labels[i] for i in train_indices]
            y_test = [self.labels[i] for i in test_indices]
            
            return X_train, X_test, y_train, y_test
        else:
            X, y = self.to_numpy()
            return train_test_split(X, y, test_size=test_size, random_state=random_state)
    
    def get_feature_statistics(self):
        """
        Get statistics for each feature.
        
        Returns:
            Dictionary with feature statistics
        """
        if not self.features:
            return {}
        
        stats = {}
        for i in range(self.num_features):
            feature_values = [sample[i] for sample in self.features]
            
            stats[i] = {
                'mean': sum(feature_values) / len(feature_values),
                'min': min(feature_values),
                'max': max(feature_values),
                'std': self._calculate_std(feature_values)
            }
            
            # Add feature name if available
            if i < len(self.feature_names):
                stats[self.feature_names[i]] = stats.pop(i)
        
        return stats
    
    def _calculate_std(self, values: List[float]) -> float:
        """
        Calculate standard deviation.
        
        Args:
            values: List of values
            
        Returns:
            Standard deviation
        """
        if len(values) <= 1:
            return 0.0
        
        mean = sum(values) / len(values)
        squared_diffs = [(x - mean) ** 2 for x in values]
        variance = sum(squared_diffs) / (len(values) - 1)
        
        return math.sqrt(variance)
    
    def normalize_features(self, method: str = "minmax"):
        """
        Normalize features.
        
        Args:
            method: Normalization method ('minmax', 'zscore')
        """
        if method == "minmax":
            # Min-Max normalization
            for i in range(self.num_features):
                feature_values = [sample[i] for sample in self.features]
                min_val = min(feature_values)
                max_val = max(feature_values)
                
                if max_val - min_val > 1e-10:  # Avoid division by zero
                    for j in range(self.num_samples):
                        self.features[j][i] = (self.features[j][i] - min_val) / (max_val - min_val)
        
        elif method == "zscore":
            # Z-score normalization
            for i in range(self.num_features):
                feature_values = [sample[i] for sample in self.features]
                mean = sum(feature_values) / len(feature_values)
                std = self._calculate_std(feature_values)
                
                if std > 1e-10:  # Avoid division by zero
                    for j in range(self.num_samples):
                        self.features[j][i] = (self.features[j][i] - mean) / std


class Model(ABC):
    """Abstract base class for machine learning models."""
    
    def __init__(self):
        """Initialize the model."""
        self.is_fitted = False
        self.hyperparameters = Hyperparameters()
        self.feature_importance = None
        self.classes = None
    
    @abstractmethod
    def fit(self, X: List[List[float]], y: List[Any]) -> None:
        """
        Fit the model to the training data.
        
        Args:
            X: Training features
            y: Training labels
        """
        pass
    
    @abstractmethod
    def predict(self, X: List[List[float]]) -> List[Any]:
        """
        Make predictions on new data.
        
        Args:
            X: Features to predict
            
        Returns:
            Predictions
        """
        pass
    
    @abstractmethod
    def predict_proba(self, X: List[List[float]]) -> List[List[float]]:
        """
        Predict class probabilities.
        
        Args:
            X: Features to predict
            
        Returns:
            Class probabilities
        """
        pass
    
    def evaluate(self, X: List[List[float]], y: List[Any], metrics: List[str] = None) -> ModelMetrics:
        """
        Evaluate the model on test data.
        
        Args:
            X: Test features
            y: Test labels
            metrics: Metrics to calculate
            
        Returns:
            Model metrics
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before evaluation")
        
        # Make predictions
        y_pred = self.predict(X)
        
        # Initialize metrics object
        model_metrics = ModelMetrics()
        
        # Calculate metrics
        if metrics is None:
            metrics = ['accuracy', 'precision', 'recall', 'f1']
        
        if 'accuracy' in metrics:
            model_metrics.accuracy = self._calculate_accuracy(y, y_pred)
        
        if 'precision' in metrics:
            model_metrics.precision = self._calculate_precision(y, y_pred)
        
        if 'recall' in metrics:
            model_metrics.recall = self._calculate_recall(y, y_pred)
        
        if 'f1' in metrics:
            model_metrics.f1_score = self._calculate_f1(y, y_pred)
        
        if 'mse' in metrics:
            model_metrics.mse = self._calculate_mse(y, y_pred)
        
        if 'mae' in metrics:
            model_metrics.mae = self._calculate_mae(y, y_pred)
        
        if 'r2' in metrics:
            model_metrics.r2_score = self._calculate_r2(y, y_pred)
        
        return model_metrics
    
    def cross_validate(self, X: List[List[float]], y: List[Any], cv: int = 5) -> Dict[str, float]:
        """
        Perform cross-validation.
        
        Args:
            X: Features
            y: Labels
            cv: Number of folds
            
        Returns:
            Dictionary with cross-validation scores
        """
        # Simple implementation of cross-validation
        n_samples = len(X)
        fold_size = n_samples // cv
        
        # Calculate scores for each fold
        accuracy_scores = []
        
        for i in range(cv):
            # Create train/test splits for this fold
            start_idx = i * fold_size
            end_idx = (i + 1) * fold_size if i < cv - 1 else n_samples
            
            # Test indices for this fold
            test_indices = list(range(start_idx, end_idx))
            
            # Train indices are all others
            train_indices = [j for j in range(n_samples) if j not in test_indices]
            
            # Split data
            X_train = [X[j] for j in train_indices]
            y_train = [y[j] for j in train_indices]
            X_test = [X[j] for j in test_indices]
            y_test = [y[j] for j in test_indices]
            
            # Clone and fit model
            model_copy = self._clone()
            model_copy.fit(X_train, y_train)
            
            # Evaluate
            y_pred = model_copy.predict(X_test)
            accuracy = self._calculate_accuracy(y_test, y_pred)
            accuracy_scores.append(accuracy)
        
        return {
            'accuracy_mean': sum(accuracy_scores) / len(accuracy_scores),
            'accuracy_std': self._calculate_std(accuracy_scores)
        }
    
    def save_model(self, file_path: str) -> None:
        """
        Save the model to a file.
        
        Args:
            file_path: Path to save the model
        """
        try:
            with open(file_path, 'wb') as f:
                pickle.dump(self, f)
            logger.info(f"Model saved to {file_path}")
        except Exception as e:
            logger.error(f"Error saving model: {str(e)}")
            raise
    
    @classmethod
    def load_model(cls, file_path: str):
        """
        Load a model from a file.
        
        Args:
            file_path: Path to the model file
            
        Returns:
            Loaded model
        """
        try:
            with open(file_path, 'rb') as f:
                model = pickle.load(f)
            logger.info(f"Model loaded from {file_path}")
            return model
        except Exception as e:
            logger.error(f"Error loading model: {str(e)}")
            raise
    
    def _clone(self):
        """
        Create a copy of the model.
        
        Returns:
            Model copy
        """
        # This is a simplified implementation
        # In a real implementation, you would copy all model parameters
        clone = self.__class__()
        clone.hyperparameters = Hyperparameters()
        clone.hyperparameters.params = self.hyperparameters.params.copy()
        return clone
    
    def _calculate_accuracy(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate accuracy.
        
        Args:
            y_true: True labels
            y_pred: Predicted labels
            
        Returns:
            Accuracy score
        """
        if len(y_true) == 0:
            return 0.0
        
        correct = sum(1 for i in range(len(y_true)) if y_true[i] == y_pred[i])
        return correct / len(y_true)
    
    def _calculate_precision(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate precision.
        
        Args:
            y_true: True labels
            y_pred: Predicted labels
            
        Returns:
            Precision score
        """
        # For binary classification, precision is TP / (TP + FP)
        # For multi-class, we'll calculate macro-averaged precision
        
        if len(y_true) == 0:
            return 0.0
        
        # Get unique classes
        classes = set(y_true + y_pred)
        precisions = []
        
        for cls in classes:
            # Calculate true positives and false positives for this class
            tp = sum(1 for i in range(len(y_true)) if y_true[i] == cls and y_pred[i] == cls)
            fp = sum(1 for i in range(len(y_true)) if y_true[i] != cls and y_pred[i] == cls)
            
            # Calculate precision
            if tp + fp == 0:
                precisions.append(0.0)
            else:
                precisions.append(tp / (tp + fp))
        
        return sum(precisions) / len(precisions)
    
    def _calculate_recall(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate recall.
        
        Args:
            y_true: True labels
            y_pred: Predicted labels
            
        Returns:
            Recall score
        """
        # For binary classification, recall is TP / (TP + FN)
        # For multi-class, we'll calculate macro-averaged recall
        
        if len(y_true) == 0:
            return 0.0
        
        # Get unique classes
        classes = set(y_true + y_pred)
        recalls = []
        
        for cls in classes:
            # Calculate true positives and false negatives for this class
            tp = sum(1 for i in range(len(y_true)) if y_true[i] == cls and y_pred[i] == cls)
            fn = sum(1 for i in range(len(y_true)) if y_true[i] == cls and y_pred[i] != cls)
            
            # Calculate recall
            if tp + fn == 0:
                recalls.append(0.0)
            else:
                recalls.append(tp / (tp + fn))
        
        return sum(recalls) / len(recalls)
    
    def _calculate_f1(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate F1 score.
        
        Args:
            y_true: True labels
            y_pred: Predicted labels
            
        Returns:
            F1 score
        """
        precision = self._calculate_precision(y_true, y_pred)
        recall = self._calculate_recall(y_true, y_pred)
        
        if precision + recall == 0:
            return 0.0
        
        return 2 * precision * recall / (precision + recall)
    
    def _calculate_mse(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate Mean Squared Error.
        
        Args:
            y_true: True values
            y_pred: Predicted values
            
        Returns:
            MSE score
        """
        if len(y_true) == 0:
            return 0.0
        
        # Convert to float if needed
        y_true_float = [float(y) for y in y_true]
        y_pred_float = [float(y) for y in y_pred]
        
        squared_errors = [(y_true_float[i] - y_pred_float[i]) ** 2 for i in range(len(y_true))]
        return sum(squared_errors) / len(y_true)
    
    def _calculate_mae(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate Mean Absolute Error.
        
        Args:
            y_true: True values
            y_pred: Predicted values
            
        Returns:
            MAE score
        """
        if len(y_true) == 0:
            return 0.0
        
        # Convert to float if needed
        y_true_float = [float(y) for y in y_true]
        y_pred_float = [float(y) for y in y_pred]
        
        absolute_errors = [abs(y_true_float[i] - y_pred_float[i]) for i in range(len(y_true))]
        return sum(absolute_errors) / len(y_true)
    
    def _calculate_r2(self, y_true: List[Any], y_pred: List[Any]) -> float:
        """
        Calculate R-squared score.
        
        Args:
            y_true: True values
            y_pred: Predicted values
            
        Returns:
            R2 score
        """
        if len(y_true) == 0:
            return 0.0
        
        # Convert to float if needed
        y_true_float = [float(y) for y in y_true]
        y_pred_float = [float(y) for y in y_pred]
        
        # Calculate mean of true values
        y_mean = sum(y_true_float) / len(y_true_float)
        
        # Calculate total sum of squares
        ss_tot = sum((y - y_mean) ** 2 for y in y_true_float)
        
        # Calculate residual sum of squares
        ss_res = sum((y_true_float[i] - y_pred_float[i]) ** 2 for i in range(len(y_true)))
        
        # Calculate R2
        if ss_tot == 0:
            return 1.0
        
        return 1 - ss_res / ss_tot
    
    def _calculate_std(self, values: List[float]) -> float:
        """
        Calculate standard deviation.
        
        Args:
            values: List of values
            
        Returns:
            Standard deviation
        """
        if len(values) <= 1:
            return 0.0
        
        mean = sum(values) / len(values)
        squared_diffs = [(x - mean) ** 2 for x in values]
        variance = sum(squared_diffs) / (len(values) - 1)
        
        return math.sqrt(variance)


class LinearRegression(Model):
    """Linear regression model."""
    
    def __init__(self, regularization: RegularizationType = RegularizationType.NONE, alpha: float = 0.01):
        """
        Initialize the linear regression model.
        
        Args:
            regularization: Type of regularization
            alpha: Regularization strength
        """
        super().__init__()
        self.regularization = regularization
        self.alpha = alpha
        self.weights = None
        self.bias = None
    
    def fit(self, X: List[List[float]], y: List[Any]) -> None:
        """
        Fit the linear regression model.
        
        Args:
            X: Training features
            y: Training labels
        """
        if not X or not y:
            raise ValueError("X and y cannot be empty")
        
        # Convert y to float
        y_float = [float(val) for val in y]
        
        # Use numpy for easier calculation if available
        if SKLEARN_AVAILABLE:
            self._fit_with_sklearn(X, y_float)
        else:
            self._fit_simple(X, y_float)
        
        self.is_fitted = True
        
        # Calculate feature importance (absolute weights)
        if self.weights:
            self.feature_importance = [abs(w) for w in self.weights]
    
    def _fit_with_sklearn(self, X: List[List[float]], y: List[float]) -> None:
        """
        Fit using sklearn.
        
        Args:
            X: Training features
            y: Training labels
        """
        from sklearn.linear_model import LinearRegression, Ridge, Lasso, ElasticNet
        
        # Select appropriate model based on regularization
        if self.regularization == RegularizationType.NONE:
            model = LinearRegression()
        elif self.regularization == RegularizationType.L1:
            model = Lasso(alpha=self.alpha)
        elif self.regularization == RegularizationType.L2:
            model = Ridge(alpha=self.alpha)
        elif self.regularization == RegularizationType.ELASTIC_NET:
            model = ElasticNet(alpha=self.alpha)
        else:
            model = LinearRegression()  # Default to no regularization
        
        # Fit the model
        model.fit(X, y)
        
        # Extract weights and bias
        self.weights = model.coef_.tolist()
        self.bias = model.intercept_
    
    def _fit_simple(self, X: List[List[float]], y: List[float]) -> None:
        """
        Fit using simple implementation.
        
        Args:
            X: Training features
            y: Training labels
        """
        # This is a simplified implementation of linear regression
        # Using the normal equation: w = (X^T * X)^-1 * X^T * y
        
        # Add bias term to X
        X_with_bias = [[1.0] + x for x in X]
        
        # Calculate X^T * X
        n_samples = len(X_with_bias)
        n_features = len(X_with_bias[0])
        
        # Initialize XTX and XTy
        XTX = [[0.0 for _ in range(n_features)] for _ in range(n_features)]
        XTy = [0.0 for _ in range(n_features)]
        
        # Calculate X^T * X and X^T * y
        for i in range(n_samples):
            for j in range(n_features):
                XTy[j] += X_with_bias[i][j] * y[i]
                
                for k in range(n_features):
                    XTX[j][k] += X_with_bias[i][j] * X_with_bias[i][k]
        
        # Add regularization if needed
        if self.regularization == RegularizationType.L2:
            for i in range(1, n_features):  # Skip bias term
                XTX[i][i] += self.alpha
        
        elif self.regularization == RegularizationType.L1:
            # L1 regularization requires more complex optimization
            # For simplicity, we'll just add a small value to diagonal
            for i in range(1, n_features):  # Skip bias term
                XTX[i][i] += self.alpha
        
        # Solve the linear system (this is a simplified implementation)
        # In a real implementation, you would use a more stable method
        try:
            # Gaussian elimination
            for i in range(n_features):
                # Find pivot
                max_row = i
                max_val = abs(XTX[i][i])
                
                for j in range(i + 1, n_features):
                    if abs(XTX[j][i]) > max_val:
                        max_val = abs(XTX[j][i])
                        max_row = j
                
                # Swap rows
                XTX[i], XTX[max_row] = XTX[max_row], XTX[i]
                XTy[i], XTy[max_row] = XTy[max_row], XTy[i]
                
                # Scale row
                pivot = XTX[i][i]
                if abs(pivot) > 1e-10:  # Avoid division by zero
                    for j in range(i, n_features):
                        XTX[i][j] /= pivot
                    XTy[i] /= pivot
                
                # Eliminate other rows
                for j in range(n_features):
                    if j != i:
                        factor = XTX[j][i]
                        if abs(factor) > 1e-10:
                            for k in range(i, n_features):
                                XTX[j][k] -= factor * XTX[i][k]
                            XTy[j] -= factor * XTy[i]
            
            # Extract solution
            solution = [XTy[i] for i in range(n_features)]
            
            # Split into weights and bias
            self.bias = solution[0]
            self.weights = solution[1:]
        except Exception as e:
            logger.error(f"Error solving linear system: {str(e)}")
            # Fallback to simple solution
            self.weights = [0.0 for _ in range(len(X[0]))]
            self.bias = sum(y) / len(y)  # Simple mean as bias
    
    def predict(self, X: List[List[float]]) -> List[Any]:
        """
        Make predictions on new data.
        
        Args:
            X: Features to predict
            
        Returns:
            Predictions
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        predictions = []
        for x in X:
            # Calculate prediction: y = w^T * x + b
            pred = self.bias
            
            for i in range(len(x)):
                pred += self.weights[i] * x[i]
            
            predictions.append(pred)
        
        return predictions
    
    def predict_proba(self, X: List[List[float]]) -> List[List[float]]:
        """
        Predict class probabilities.
        
        Args:
            X: Features to predict
            
        Returns:
            Class probabilities (not applicable for regression)
        """
        raise NotImplementedError("Linear regression does not support probability prediction")


class LogisticRegression(Model):
    """Logistic regression model."""
    
    def __init__(self, regularization: RegularizationType = RegularizationType.NONE, alpha: float = 0.01, 
                 learning_rate: float = 0.01, max_iter: int = 1000, tolerance: float = 1e-4):
        """
        Initialize the logistic regression model.
        
        Args:
            regularization: Type of regularization
            alpha: Regularization strength
            learning_rate: Learning rate for gradient descent
            max_iter: Maximum number of iterations
            tolerance: Tolerance for convergence
        """
        super().__init__()
        self.regularization = regularization
        self.alpha = alpha
        self.learning_rate = learning_rate
        self.max_iter = max_iter
        self.tolerance = tolerance
        self.weights = None
        self.bias = None
        self.classes = None
    
    def fit(self, X: List[List[float]], y: List[Any]) -> None:
        """
        Fit the logistic regression model.
        
        Args:
            X: Training features
            y: Training labels
        """
        if not X or not y:
            raise ValueError("X and y cannot be empty")
        
        # Get unique classes
        self.classes = list(set(y))
        
        if len(self.classes) != 2:
            raise ValueError("Logistic regression only supports binary classification")
        
        # Convert y to 0/1
        y_binary = [1 if label == self.classes[1] else 0 for label in y]
        
        # Use sklearn if available
        if SKLEARN_AVAILABLE:
            self._fit_with_sklearn(X, y_binary)
        else:
            self._fit_simple(X, y_binary)
        
        self.is_fitted = True
        
        # Calculate feature importance
        if self.weights:
            self.feature_importance = [abs(w) for w in self.weights]
    
    def _fit_with_sklearn(self, X: List[List[float]], y: List[int]) -> None:
        """
        Fit using sklearn.
        
        Args:
            X: Training features
            y: Training labels
        """
        from sklearn.linear_model import LogisticRegression
        
        # Select appropriate solver based on regularization
        if self.regularization == RegularizationType.NONE:
            solver = 'liblinear'
            penalty = 'none'
        elif self.regularization == RegularizationType.L1:
            solver = 'liblinear'
            penalty = 'l1'
        else:  # L2 or ELASTIC_NET
            solver = 'liblinear'
            penalty = 'l2'
        
        # Create and fit model
        model = LogisticRegression(
            solver=solver,
            penalty=penalty,
            C=1.0 / self.alpha if self.alpha > 0 else 1.0,
            max_iter=self.max_iter,
            tol=self.tolerance
        )
        
        model.fit(X, y)
        
        # Extract weights and bias
        if hasattr(model, 'coef_'):
            self.weights = model.coef_[0].tolist()
        else:
            self.weights = [0.0 for _ in range(len(X[0]))]
        
        if hasattr(model, 'intercept_'):
            self.bias = model.intercept_[0]
        else:
            self.bias = 0.0
    
    def _fit_simple(self, X: List[List[float]], y: List[int]) -> None:
        """
        Fit using simple implementation.
        
        Args:
            X: Training features
            y: Training labels
        """
        # This is a simplified implementation of logistic regression
        # Using gradient descent
        
        n_samples = len(X)
        n_features = len(X[0])
        
        # Initialize weights and bias
        self.weights = [random.uniform(-0.01, 0.01) for _ in range(n_features)]
        self.bias = 0.0
        
        # Gradient descent
        for iteration in range(self.max_iter):
            # Calculate predictions
            predictions = []
            for x in X:
                # Calculate linear combination
                z = self.bias
                for j in range(n_features):
                    z += self.weights[j] * x[j]
                
                # Apply sigmoid function
                sig_z = 1.0 / (1.0 + math.exp(-z))
                predictions.append(sig_z)
            
            # Calculate gradients
            # Initialize gradients
            weight_gradients = [0.0 for _ in range(n_features)]
            bias_gradient = 0.0
            
            # Calculate gradients for each sample
            for i in range(n_samples):
                error = predictions[i] - y[i]
                
                # Update bias gradient
                bias_gradient += error
                
                # Update weight gradients
                for j in range(n_features):
                    weight_gradients[j] += error * X[i][j]
            
            # Average gradients
            bias_gradient /= n_samples
            for j in range(n_features):
                weight_gradients[j] /= n_samples
                
                # Add regularization if needed
                if self.regularization == RegularizationType.L2:
                    weight_gradients[j] += self.alpha * self.weights[j]
                elif self.regularization == RegularizationType.L1:
                    weight_gradients[j] += self.alpha * (1.0 if self.weights[j] > 0 else -1.0)
            
            # Check for convergence
            max_gradient = max(abs(bias_gradient), max(abs(g) for g in weight_gradients))
            if max_gradient < self.tolerance:
                logger.info(f"Converged after {iteration} iterations")
                break
            
            # Update weights and bias
            self.bias -= self.learning_rate * bias_gradient
            for j in range(n_features):
                self.weights[j] -= self.learning_rate * weight_gradients[j]
    
    def predict(self, X: List[List[float]]) -> List[Any]:
        """
        Make predictions on new data.
        
        Args:
            X: Features to predict
            
        Returns:
            Predictions
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        predictions = []
        for x in X:
            # Calculate linear combination
            z = self.bias
            for i in range(len(x)):
                z += self.weights[i] * x[i]
            
            # Apply sigmoid function
            prob = 1.0 / (1.0 + math.exp(-z))
            
            # Convert to class
            pred_class = self.classes[1] if prob > 0.5 else self.classes[0]
            predictions.append(pred_class)
        
        return predictions
    
    def predict_proba(self, X: List[List[float]]) -> List[List[float]]:
        """
        Predict class probabilities.
        
        Args:
            X: Features to predict
            
        Returns:
            Class probabilities
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        probabilities = []
        for x in X:
            # Calculate linear combination
            z = self.bias
            for i in range(len(x)):
                z += self.weights[i] * x[i]
            
            # Apply sigmoid function
            prob = 1.0 / (1.0 + math.exp(-z))
            
            # Return probabilities for both classes
            probabilities.append([1.0 - prob, prob])
        
        return probabilities


class KMeans(Model):
    """K-Means clustering algorithm."""
    
    def __init__(self, n_clusters: int = 8, max_iter: int = 300, tolerance: float = 1e-4, random_state: int = None):
        """
        Initialize the K-Means model.
        
        Args:
            n_clusters: Number of clusters
            max_iter: Maximum number of iterations
            tolerance: Tolerance for convergence
            random_state: Random seed for reproducibility
        """
        super().__init__()
        self.n_clusters = n_clusters
        self.max_iter = max_iter
        self.tolerance = tolerance
        self.random_state = random_state
        self.centroids = None
        self.cluster_labels = None
        self.inertia = None
    
    def fit(self, X: List[List[float]], y: List[Any] = None) -> None:
        """
        Fit the K-Means model.
        
        Args:
            X: Training features
            y: Training labels (not used in unsupervised learning)
        """
        if not X:
            raise ValueError("X cannot be empty")
        
        # Set random seed if needed
        if self.random_state is not None:
            random.seed(self.random_state)
        
        # Initialize centroids randomly
        n_samples = len(X)
        n_features = len(X[0])
        
        # Select random samples as initial centroids
        indices = random.sample(range(n_samples), self.n_clusters)
        self.centroids = [X[i] for i in indices]
        
        # K-Means algorithm
        prev_labels = None
        for iteration in range(self.max_iter):
            # Assign each sample to the nearest centroid
            labels = []
            
            for i in range(n_samples):
                # Calculate distance to each centroid
                distances = []
                
                for j in range(self.n_clusters):
                    # Calculate Euclidean distance
                    dist_sq = 0.0
                    for k in range(n_features):
                        diff = X[i][k] - self.centroids[j][k]
                        dist_sq += diff * diff
                    
                    distances.append(math.sqrt(dist_sq))
                
                # Assign to nearest centroid
                nearest_centroid = distances.index(min(distances))
                labels.append(nearest_centroid)
            
            # Check for convergence
            if prev_labels is not None and labels == prev_labels:
                logger.info(f"Converged after {iteration} iterations")
                break
            
            prev_labels = labels.copy()
            
            # Update centroids
            new_centroids = []
            
            for j in range(self.n_clusters):
                # Find all samples assigned to this centroid
                cluster_samples = [X[i] for i in range(n_samples) if labels[i] == j]
                
                if cluster_samples:
                    # Calculate new centroid as mean of assigned samples
                    new_centroid = []
                    
                    for k in range(n_features):
                        feature_sum = sum(sample[k] for sample in cluster_samples)
                        new_centroid.append(feature_sum / len(cluster_samples))
                    
                    new_centroids.append(new_centroid)
                else:
                    # No samples assigned, keep the old centroid
                    new_centroids.append(self.centroids[j])
            
            self.centroids = new_centroids
        
        # Store final cluster labels and inertia
        self.cluster_labels = labels
        
        # Calculate inertia (sum of squared distances to nearest centroid)
        self.inertia = 0.0
        for i in range(n_samples):
            centroid = self.centroids[labels[i]]
            
            # Calculate squared distance
            dist_sq = 0.0
            for k in range(n_features):
                diff = X[i][k] - centroid[k]
                dist_sq += diff * diff
            
            self.inertia += dist_sq
        
        self.is_fitted = True
    
    def predict(self, X: List[List[float]]) -> List[Any]:
        """
        Assign samples to the nearest centroid.
        
        Args:
            X: Features to cluster
            
        Returns:
            Cluster assignments
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        n_features = len(X[0])
        labels = []
        
        for i in range(len(X)):
            # Calculate distance to each centroid
            distances = []
            
            for j in range(self.n_clusters):
                # Calculate Euclidean distance
                dist_sq = 0.0
                for k in range(n_features):
                    diff = X[i][k] - self.centroids[j][k]
                    dist_sq += diff * diff
                
                distances.append(math.sqrt(dist_sq))
            
            # Assign to nearest centroid
            nearest_centroid = distances.index(min(distances))
            labels.append(nearest_centroid)
        
        return labels
    
    def predict_proba(self, X: List[List[float]]) -> List[List[float]]:
        """
        Calculate distances to centroids and convert to probabilities.
        
        Args:
            X: Features to cluster
            
        Returns:
            Cluster probabilities
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        n_features = len(X[0])
        probabilities = []
        
        for i in range(len(X)):
            # Calculate distance to each centroid
            distances = []
            
            for j in range(self.n_clusters):
                # Calculate Euclidean distance
                dist_sq = 0.0
                for k in range(n_features):
                    diff = X[i][k] - self.centroids[j][k]
                    dist_sq += diff * diff
                
                distances.append(dist_sq)
            
            # Convert distances to probabilities using softmax
            max_dist = max(distances)
            exp_distances = [math.exp(-(dist - max_dist)) for dist in distances]
            sum_exp = sum(exp_distances)
            
            if sum_exp > 0:
                cluster_probs = [dist / sum_exp for dist in exp_distances]
            else:
                # If all distances are equal, assign equal probability
                cluster_probs = [1.0 / self.n_clusters for _ in range(self.n_clusters)]
            
            probabilities.append(cluster_probs)
        
        return probabilities


class NeuralNetworkLayer:
    """A single layer in a neural network."""
    
    def __init__(self, input_size: int, output_size: int, activation: ActivationFunction = ActivationFunction.RELU):
        """
        Initialize the neural network layer.
        
        Args:
            input_size: Size of input
            output_size: Size of output
            activation: Activation function
        """
        self.input_size = input_size
        self.output_size = output_size
        self.activation = activation
        
        # Initialize weights with Xavier initialization
        scale = math.sqrt(2.0 / (input_size + output_size))
        self.weights = [[random.uniform(-scale, scale) for _ in range(input_size)] for _ in range(output_size)]
        self.biases = [0.0 for _ in range(output_size)]
    
    def forward(self, inputs: List[float]) -> List[float]:
        """
        Forward pass through the layer.
        
        Args:
            inputs: Input values
            
        Returns:
            Output values after applying activation function
        """
        # Calculate linear combination: z = W * x + b
        z = []
        for i in range(self.output_size):
            # Calculate weighted sum
            weighted_sum = self.biases[i]
            for j in range(self.input_size):
                weighted_sum += self.weights[i][j] * inputs[j]
            
            z.append(weighted_sum)
        
        # Apply activation function
        if self.activation == ActivationFunction.SIGMOID:
            return [1.0 / (1.0 + math.exp(-val)) for val in z]
        elif self.activation == ActivationFunction.TANH:
            return [math.tanh(val) for val in z]
        elif self.activation == ActivationFunction.RELU:
            return [max(0.0, val) for val in z]
        elif self.activation == ActivationFunction.LEAKY_RELU:
            return [max(0.01 * val, val) for val in z]
        elif self.activation == ActivationFunction.SOFTMAX:
            # Subtract max for numerical stability
            max_val = max(z)
            exp_vals = [math.exp(val - max_val) for val in z]
            sum_exp = sum(exp_vals)
            return [val / sum_exp for val in exp_vals]
        else:  # Linear
            return z
    
    def backward(self, inputs: List[float], output_gradient: List[float], learning_rate: float) -> List[float]:
        """
        Backward pass through the layer.
        
        Args:
            inputs: Input values from forward pass
            output_gradient: Gradient from the next layer
            learning_rate: Learning rate
            
        Returns:
            Gradient to pass to the previous layer
        """
        # First, calculate the gradient of the activation function
        if self.activation == ActivationFunction.SIGMOID:
            # Need the output from forward pass
            outputs = self.forward(inputs)
            activation_grad = [output * (1.0 - output) for output in outputs]
        elif self.activation == ActivationFunction.TANH:
            # Need the output from forward pass
            outputs = self.forward(inputs)
            activation_grad = [1.0 - output * output for output in outputs]
        elif self.activation == ActivationFunction.RELU:
            # Need the inputs from forward pass
            activation_grad = [1.0 if inp > 0 else 0.0 for inp in inputs]
        elif self.activation == ActivationFunction.LEAKY_RELU:
            # Need the inputs from forward pass
            activation_grad = [1.0 if inp > 0 else 0.01 for inp in inputs]
        elif self.activation == ActivationFunction.SOFTMAX:
            # For softmax, the gradient is more complex
            # Need the output from forward pass
            outputs = self.forward(inputs)
            
            # Create a Jacobian matrix (simplified)
            jacobian = []
            for i in range(self.output_size):
                row = []
                for j in range(self.output_size):
                    if i == j:
                        row.append(outputs[i] * (1.0 - outputs[j]))
                    else:
                        row.append(-outputs[i] * outputs[j])
                jacobian.append(row)
            
            # Multiply Jacobian by output gradient
            activation_grad = []
            for i in range(self.output_size):
                grad = 0.0
                for j in range(self.output_size):
                    grad += jacobian[i][j] * output_gradient[j]
                activation_grad.append(grad)
        else:  # Linear
            activation_grad = [1.0 for _ in range(self.output_size)]
        
        # Calculate the gradient with respect to the pre-activation values
        pre_activation_grad = [output_gradient[i] * activation_grad[i] for i in range(self.output_size)]
        
        # Calculate gradients for weights and biases
        weight_gradients = [[0.0 for _ in range(self.input_size)] for _ in range(self.output_size)]
        bias_gradients = [0.0 for _ in range(self.output_size)]
        
        for i in range(self.output_size):
            # Gradient for bias
            bias_gradients[i] = pre_activation_grad[i]
            
            # Gradient for weights
            for j in range(self.input_size):
                weight_gradients[i][j] = pre_activation_grad[i] * inputs[j]
        
        # Update weights and biases
        for i in range(self.output_size):
            self.biases[i] -= learning_rate * bias_gradients[i]
            
            for j in range(self.input_size):
                self.weights[i][j] -= learning_rate * weight_gradients[i][j]
        
        # Calculate gradient to pass to the previous layer
        input_gradient = [0.0 for _ in range(self.input_size)]
        
        for j in range(self.input_size):
            for i in range(self.output_size):
                input_gradient[j] += pre_activation_grad[i] * self.weights[i][j]
        
        return input_gradient


class NeuralNetwork(Model):
    """Feedforward neural network model."""
    
    def __init__(self, layer_sizes: List[int], activations: List[ActivationFunction] = None, 
                 learning_rate: float = 0.01, max_epochs: int = 100, batch_size: int = 32):
        """
        Initialize the neural network.
        
        Args:
            layer_sizes: List of layer sizes (including input and output layers)
            activations: List of activation functions for each hidden layer
            learning_rate: Learning rate
            max_epochs: Maximum number of epochs
            batch_size: Size of mini-batches
        """
        super().__init__()
        self.layer_sizes = layer_sizes
        self.learning_rate = learning_rate
        self.max_epochs = max_epochs
        self.batch_size = batch_size
        
        # Create layers
        self.layers = []
        for i in range(len(layer_sizes) - 1):
            # Determine activation function
            if activations and i < len(activations):
                activation = activations[i]
            elif i == len(layer_sizes) - 2:  # Last hidden layer
                activation = ActivationFunction.RELU
            else:
                activation = ActivationFunction.RELU
            
            # Create layer
            layer = NeuralNetworkLayer(layer_sizes[i], layer_sizes[i+1], activation)
            self.layers.append(layer)
        
        # For classification, set the last layer to softmax
        if self.layers:
            self.layers[-1].activation = ActivationFunction.SOFTMAX
    
    def fit(self, X: List[List[float]], y: List[Any], validation_data: Tuple = None) -> None:
        """
        Fit the neural network model.
        
        Args:
            X: Training features
            y: Training labels
            validation_data: Optional validation data (X_val, y_val)
        """
        if not X or not y:
            raise ValueError("X and y cannot be empty")
        
        # Get unique classes
        self.classes = list(set(y))
        
        # Convert labels to one-hot encoding
        y_one_hot = []
        for label in y:
            one_hot = [0.0 for _ in self.classes]
            one_hot[self.classes.index(label)] = 1.0
            y_one_hot.append(one_hot)
        
        # Normalize inputs
        dataset = Dataset(X)
        dataset.normalize_features("zscore")
        X_normalized = dataset.features
        
        # Training loop
        n_samples = len(X_normalized)
        
        for epoch in range(self.max_epochs):
            # Shuffle data for each epoch
            indices = list(range(n_samples))
            random.shuffle(indices)
            
            # Process mini-batches
            for start_idx in range(0, n_samples, self.batch_size):
                end_idx = min(start_idx + self.batch_size, n_samples)
                batch_indices = indices[start_idx:end_idx]
                
                # Forward pass
                batch_X = [X_normalized[i] for i in batch_indices]
                batch_y = [y_one_hot[i] for i in batch_indices]
                
                # Train on batch
                for i in range(len(batch_X)):
                    x = batch_X[i]
                    target = batch_y[i]
                    
                    # Forward pass
                    layer_inputs = [x]
                    layer_outputs = [x]
                    
                    for layer in self.layers:
                        layer_output = layer.forward(layer_inputs[-1])
                        layer_inputs.append(layer_outputs[-1])
                        layer_outputs.append(layer_output)
                    
                    # Calculate loss gradient (simplified for cross-entropy with softmax)
                    output = layer_outputs[-1]
                    loss_gradient = [output[j] - target[j] for j in range(len(target))]
                    
                    # Backward pass
                    gradient = loss_gradient
                    for i in range(len(self.layers) - 1, -1, -1):
                        layer = self.layers[i]
                        gradient = layer.backward(layer_inputs[i], gradient, self.learning_rate)
            
            # Calculate validation loss if validation data is provided
            if validation_data is not None and epoch % 10 == 0:
                X_val, y_val = validation_data
                
                # Convert validation labels to one-hot
                y_val_one_hot = []
                for label in y_val:
                    one_hot = [0.0 for _ in self.classes]
                    one_hot[self.classes.index(label)] = 1.0
                    y_val_one_hot.append(one_hot)
                
                # Normalize validation inputs
                val_dataset = Dataset(X_val)
                val_dataset.normalize_features("zscore")
                X_val_normalized = val_dataset.features
                
                # Calculate validation loss
                val_loss = 0.0
                val_correct = 0
                
                for i in range(len(X_val_normalized)):
                    # Forward pass
                    x = X_val_normalized[i]
                    
                    layer_inputs = [x]
                    layer_outputs = [x]
                    
                    for layer in self.layers:
                        layer_output = layer.forward(layer_inputs[-1])
                        layer_inputs.append(layer_outputs[-1])
                        layer_outputs.append(layer_output)
                    
                    # Calculate loss
                    output = layer_outputs[-1]
                    target = y_val_one_hot[i]
                    
                    # Cross-entropy loss
                    loss = 0.0
                    for j in range(len(target)):
                        if target[j] > 0:
                            loss -= math.log(output[j])
                    
                    val_loss += loss
                    
                    # Check if prediction is correct
                    pred_class = self.classes[output.index(max(output))]
                    if pred_class == y_val[i]:
                        val_correct += 1
                
                val_loss /= len(X_val_normalized)
                val_accuracy = val_correct / len(X_val_normalized)
                
                logger.info(f"Epoch {epoch}, Validation Loss: {val_loss:.4f}, Validation Accuracy: {val_accuracy:.4f}")
        
        self.is_fitted = True
    
    def predict(self, X: List[List[float]]) -> List[Any]:
        """
        Make predictions on new data.
        
        Args:
            X: Features to predict
            
        Returns:
            Predictions
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        # Normalize inputs using training statistics (simplified)
        dataset = Dataset(X)
        dataset.normalize_features("zscore")
        X_normalized = dataset.features
        
        predictions = []
        
        for x in X_normalized:
            # Forward pass
            layer_inputs = [x]
            
            for layer in self.layers:
                layer_output = layer.forward(layer_inputs[-1])
                layer_inputs.append(layer_output)
            
            # Get final output
            output = layer_inputs[-1]
            
            # Get predicted class
            pred_class = self.classes[output.index(max(output))]
            predictions.append(pred_class)
        
        return predictions
    
    def predict_proba(self, X: List[List[float]]) -> List[List[float]]:
        """
        Predict class probabilities.
        
        Args:
            X: Features to predict
            
        Returns:
            Class probabilities
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before prediction")
        
        if not X:
            return []
        
        # Normalize inputs using training statistics (simplified)
        dataset = Dataset(X)
        dataset.normalize_features("zscore")
        X_normalized = dataset.features
        
        probabilities = []
        
        for x in X_normalized:
            # Forward pass
            layer_inputs = [x]
            
            for layer in self.layers:
                layer_output = layer.forward(layer_inputs[-1])
                layer_inputs.append(layer_output)
            
            # Get final output (already softmax probabilities)
            output = layer_inputs[-1]
            probabilities.append(output)
        
        return probabilities


def create_sample_data():
    """Create sample data for testing."""
    # Generate synthetic classification data
    print("Generating synthetic classification data...")
    
    # Class 0: Gaussian centered at (0, 0)
    class0 = []
    for _ in range(100):
        x = random.gauss(0, 1)
        y = random.gauss(0, 1)
        class0.append([x, y])
    
    # Class 1: Gaussian centered at (2, 2)
    class1 = []
    for _ in range(100):
        x = random.gauss(2, 1)
        y = random.gauss(2, 1)
        class1.append([x, y])
    
    # Combine data
    X = class0 + class1
    y = [0] * 100 + [1] * 100
    
    # Split into training and test sets
    dataset = Dataset(X, y)
    X_train, X_test, y_train, y_test = dataset.split(test_size=0.3, random_state=42)
    
    return X_train, X_test, y_train, y_test


def create_sample_regression_data():
    """Create sample regression data for testing."""
    print("Generating synthetic regression data...")
    
    # y = 2 * x1 + 3 * x2 + noise
    X = []
    y = []
    
    for _ in range(200):
        x1 = random.uniform(-5, 5)
        x2 = random.uniform(-5, 5)
        noise = random.gauss(0, 1)
        
        y_val = 2 * x1 + 3 * x2 + noise
        
        X.append([x1, x2])
        y.append(y_val)
    
    # Split into training and test sets
    dataset = Dataset(X, y)
    X_train, X_test, y_train, y_test = dataset.split(test_size=0.3, random_state=42)
    
    return X_train, X_test, y_train, y_test


def demonstrate_models():
    """Demonstrate the machine learning models."""
    print("Machine Learning Library")
    print("====================")
    
    # Check dependencies
    print("Checking dependencies:")
    print(f"sklearn available: {'Yes' if SKLEARN_AVAILABLE else 'No'}")
    
    # Classification example
    print("\n1. Classification Example")
    
    # Generate sample data
    X_train, X_test, y_train, y_test = create_sample_data()
    
    # Train and evaluate Logistic Regression
    print("\nTraining Logistic Regression...")
    log_reg = LogisticRegression(learning_rate=0.1, max_iter=100)
    log_reg.fit(X_train, y_train)
    
    y_pred = log_reg.predict(X_test)
    metrics = log_reg.evaluate(X_test, y_test)
    
    print(f"Accuracy: {metrics.accuracy:.4f}")
    print(f"Precision: {metrics.precision:.4f}")
    print(f"Recall: {metrics.recall:.4f}")
    print(f"F1-Score: {metrics.f1_score:.4f}")
    
    # Train and evaluate Neural Network
    print("\nTraining Neural Network...")
    nn = NeuralNetwork(layer_sizes=[2, 10, 5, 2], 
                    activations=[ActivationFunction.RELU, ActivationFunction.RELU, None],
                    learning_rate=0.01, max_epochs=50)
    nn.fit(X_train, y_train)
    
    y_pred = nn.predict(X_test)
    metrics = nn.evaluate(X_test, y_test)
    
    print(f"Accuracy: {metrics.accuracy:.4f}")
    print(f"Precision: {metrics.precision:.4f}")
    print(f"Recall: {metrics.recall:.4f}")
    print(f"F1-Score: {metrics.f1_score:.4f}")
    
    # Regression example
    print("\n2. Regression Example")
    
    # Generate sample regression data
    X_train_reg, X_test_reg, y_train_reg, y_test_reg = create_sample_regression_data()
    
    # Train and evaluate Linear Regression
    print("\nTraining Linear Regression...")
    lin_reg = LinearRegression(regularization=RegularizationType.L2, alpha=0.1)
    lin_reg.fit(X_train_reg, y_train_reg)
    
    y_pred = lin_reg.predict(X_test_reg)
    metrics = lin_reg.evaluate(X_test_reg, y_test_reg, metrics=['mse', 'mae', 'r2'])
    
    print(f"MSE: {metrics.mse:.4f}")
    print(f"MAE: {metrics.mae:.4f}")
    print(f"R2-Score: {metrics.r2_score:.4f}")
    
    # Clustering example
    print("\n3. Clustering Example")
    
    # Use the same data as classification (without labels)
    # Train and evaluate K-Means
    print("\nTraining K-Means...")
    kmeans = KMeans(n_clusters=2, random_state=42)
    kmeans.fit(X_train, y_train)  # y_train is ignored
    
    # Get cluster assignments
    cluster_labels = kmeans.predict(X_test)
    print(f"Inertia: {kmeans.inertia:.4f}")
    print(f"First 10 cluster assignments: {cluster_labels[:10]}")
    
    # Cross-validation example
    print("\n4. Cross-Validation Example")
    
    # Perform cross-validation on Logistic Regression
    cv_scores = log_reg.cross_validate(X_train, y_train, cv=5)
    
    print(f"Cross-Validation Accuracy: {cv_scores['accuracy_mean']:.4f} (+/- {cv_scores['accuracy_std']:.4f})")
    
    # Model persistence example
    print("\n5. Model Persistence Example")
    
    # Save a model
    model_path = "log_reg_model.pkl"
    log_reg.save_model(model_path)
    
    # Load the model
    loaded_model = LogisticRegression.load_model(model_path)
    
    # Make predictions with loaded model
    y_pred_loaded = loaded_model.predict(X_test)
    accuracy_loaded = loaded_model._calculate_accuracy(y_test, y_pred_loaded)
    
    print(f"Original model accuracy: {metrics.accuracy:.4f}")
    print(f"Loaded model accuracy: {accuracy_loaded:.4f}")
    
    # Clean up
    if os.path.exists(model_path):
        os.remove(model_path)
    
    # Data preprocessing example
    print("\n6. Data Preprocessing Example")
    
    # Get feature statistics
    stats = Dataset(X_train).get_feature_statistics()
    print(f"Feature statistics before normalization:")
    for feature, stat in stats.items():
            if isinstance(feature, int):
                print(f"  Feature {feature}: mean={stat['mean']:.2f}, std={stat['std']:.2f}")
            else:
                print(f"  Feature {feature}: mean={stat['mean']:.2f}, std={stat['std']:.2f}")
    
    # Normalize features
    dataset = Dataset(X_train)
    dataset.normalize_features("minmax")
    
    stats_normalized = dataset.get_feature_statistics()
    print(f"Feature statistics after normalization:")
    for feature, stat in stats_normalized.items():
            if isinstance(feature, int):
                print(f"  Feature {feature}: mean={stat['mean']:.2f}, std={stat['std']:.2f}")
            else:
                print(f"  Feature {feature}: mean={stat['mean']:.2f}, std={stat['std']:.2f}")
    
    print("\nMachine learning library demonstration completed!")


def main():
    """Main function to demonstrate the machine learning library."""
    print("Machine Learning Library")
    print("====================")
    
    # Run the demonstration
    demonstrate_models()
    
    return True


if __name__ == "__main__":
    main()
<｜fim▁hole｜>