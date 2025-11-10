"""
Machine Learning Library Implementation

This module implements a comprehensive machine learning library with various
algorithms for classification, regression, clustering, and dimensionality reduction.
It includes preprocessing tools, model evaluation metrics, and utilities
for hyperparameter tuning.

Features:
- Supervised learning (Linear/Logistic Regression, Decision Trees, Random Forest, SVM)
- Unsupervised learning (K-Means, Hierarchical Clustering, PCA)
- Data preprocessing (Normalization, Encoding, Feature Selection)
- Model evaluation and cross-validation
- Hyperparameter optimization
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from collections import Counter, defaultdict
from typing import List, Dict, Tuple, Optional, Union, Any, Callable
import math
import random
import itertools
import time
import os
import json
from abc import ABC, abstractmethod


class BaseEstimator(ABC):
    """Abstract base class for all estimators."""
    
    @abstractmethod
    def fit(self, X, y=None):
        """Fit the model to the training data."""
        pass
    
    @abstractmethod
    def predict(self, X):
        """Make predictions for new data."""
        pass


class Transformer(ABC):
    """Abstract base class for all transformers."""
    
    @abstractmethod
    def fit(self, X, y=None):
        """Fit the transformer to the data."""
        pass
    
    @abstractmethod
    def transform(self, X):
        """Transform the data."""
        pass
    
    def fit_transform(self, X, y=None):
        """Fit the transformer and transform the data."""
        self.fit(X, y)
        return self.transform(X)


class StandardScaler(Transformer):
    """Standardize features by removing the mean and scaling to unit variance."""
    
    def __init__(self):
        self.mean_ = None
        self.std_ = None
    
    def fit(self, X, y=None):
        """Compute mean and standard deviation for scaling."""
        X = np.asarray(X)
        self.mean_ = np.mean(X, axis=0)
        self.std_ = np.std(X, axis=0, ddof=0)
        # Avoid division by zero
        self.std_[self.std_ == 0] = 1
        return self
    
    def transform(self, X):
        """Apply standardization to X."""
        if self.mean_ is None or self.std_ is None:
            raise ValueError("StandardScaler has not been fitted yet.")
        
        X = np.asarray(X)
        return (X - self.mean_) / self.std_


class MinMaxScaler(Transformer):
    """Scale features to a given range (default [0, 1])."""
    
    def __init__(self, feature_range=(0, 1)):
        self.feature_range = feature_range
        self.min_ = None
        self.scale_ = None
    
    def fit(self, X, y=None):
        """Compute minimum and maximum for scaling."""
        X = np.asarray(X)
        self.min_ = np.min(X, axis=0)
        self.scale_ = (self.feature_range[1] - self.feature_range[0]) / (np.max(X, axis=0) - self.min_)
        # Avoid division by zero
        self.scale_[np.max(X, axis=0) == self.min_] = 0
        return self
    
    def transform(self, X):
        """Apply min-max scaling to X."""
        if self.min_ is None or self.scale_ is None:
            raise ValueError("MinMaxScaler has not been fitted yet.")
        
        X = np.asarray(X)
        return (X - self.min_) * self.scale_ + self.feature_range[0]


class OneHotEncoder(Transformer):
    """Encode categorical integer features as a one-hot numeric array."""
    
    def __init__(self, handle_unknown='error'):
        self.handle_unknown = handle_unknown
        self.categories_ = None
        self.n_features_out_ = None
    
    def fit(self, X, y=None):
        """Determine categories for each feature."""
        X = np.asarray(X)
        self.categories_ = []
        self.n_features_out_ = 0
        
        for i in range(X.shape[1]):
            unique_values = np.unique(X[:, i])
            self.categories_.append(unique_values)
            self.n_features_out_ += len(unique_values)
        
        return self
    
    def transform(self, X):
        """Transform X to one-hot encoding."""
        if self.categories_ is None:
            raise ValueError("OneHotEncoder has not been fitted yet.")
        
        X = np.asarray(X)
        n_samples = X.shape[0]
        n_features_out = self.n_features_out_
        
        result = np.zeros((n_samples, n_features_out))
        col_idx = 0
        
        for i, categories in enumerate(self.categories_):
            for category in categories:
                result[:, col_idx] = (X[:, i] == category).astype(int)
                col_idx += 1
        
        return result


class LabelEncoder(Transformer):
    """Encode target labels with value between 0 and n_classes-1."""
    
    def __init__(self):
        self.classes_ = None
        self.class_to_index_ = None
    
    def fit(self, y):
        """Fit the encoder to the target labels."""
        y = np.asarray(y)
        self.classes_ = np.unique(y)
        self.class_to_index_ = {label: i for i, label in enumerate(self.classes_)}
        return self
    
    def transform(self, y):
        """Transform labels to normalized encoding."""
        if self.class_to_index_ is None:
            raise ValueError("LabelEncoder has not been fitted yet.")
        
        return np.array([self.class_to_index_[label] for label in y])
    
    def fit_transform(self, y):
        """Fit the encoder and transform the labels."""
        return self.fit(y).transform(y)
    
    def inverse_transform(self, y):
        """Transform labels back to original encoding."""
        if self.classes_ is None:
            raise ValueError("LabelEncoder has not been fitted yet.")
        
        return np.array([self.classes_[label] for label in y])


class LinearRegression(BaseEstimator):
    """Ordinary least squares Linear Regression."""
    
    def __init__(self, learning_rate=0.01, n_iterations=1000, fit_intercept=True, 
                 regularization=None, reg_strength=0.1, tol=1e-4):
        self.learning_rate = learning_rate
        self.n_iterations = n_iterations
        self.fit_intercept = fit_intercept
        self.regularization = regularization  # None, 'l1', 'l2'
        self.reg_strength = reg_strength
        self.tol = tol
        
        self.weights_ = None
        self.bias_ = None
        self.loss_history_ = []
    
    def _initialize_parameters(self, n_features):
        """Initialize model parameters."""
        self.weights_ = np.random.randn(n_features) * 0.01
        self.bias_ = 0.0 if self.fit_intercept else None
    
    def _compute_loss(self, X, y):
        """Compute mean squared error loss."""
        predictions = self._predict_internal(X)
        mse = np.mean((predictions - y) ** 2)
        
        # Add regularization
        if self.regularization == 'l1':
            mse += self.reg_strength * np.sum(np.abs(self.weights_))
        elif self.regularization == 'l2':
            mse += self.reg_strength * 0.5 * np.sum(self.weights_ ** 2)
        
        return mse
    
    def _predict_internal(self, X):
        """Internal prediction method."""
        predictions = np.dot(X, self.weights_)
        if self.bias_ is not None:
            predictions += self.bias_
        return predictions
    
    def fit(self, X, y):
        """Fit the model to the training data."""
        X = np.asarray(X)
        y = np.asarray(y)
        
        n_samples, n_features = X.shape
        self._initialize_parameters(n_features)
        
        self.loss_history_ = []
        
        for i in range(self.n_iterations):
            # Make predictions
            predictions = self._predict_internal(X)
            
            # Compute gradients
            error = predictions - y
            dw = (1 / n_samples) * np.dot(X.T, error)
            db = (1 / n_samples) * np.sum(error) if self.fit_intercept else 0
            
            # Add regularization to gradients
            if self.regularization == 'l1':
                dw += self.reg_strength * np.sign(self.weights_)
            elif self.regularization == 'l2':
                dw += self.reg_strength * self.weights_
            
            # Update parameters
            self.weights_ -= self.learning_rate * dw
            if self.bias_ is not None:
                self.bias_ -= self.learning_rate * db
            
            # Compute loss
            loss = self._compute_loss(X, y)
            self.loss_history_.append(loss)
            
            # Check for convergence
            if i > 0 and abs(self.loss_history_[-2] - loss) < self.tol:
                break
        
        return self
    
    def predict(self, X):
        """Make predictions for new data."""
        if self.weights_ is None:
            raise ValueError("Model has not been fitted yet.")
        
        X = np.asarray(X)
        return self._predict_internal(X)


class LogisticRegression(BaseEstimator):
    """Logistic Regression classifier."""
    
    def __init__(self, learning_rate=0.01, n_iterations=1000, fit_intercept=True,
                 regularization=None, reg_strength=0.1, tol=1e-4):
        self.learning_rate = learning_rate
        self.n_iterations = n_iterations
        self.fit_intercept = fit_intercept
        self.regularization = regularization  # None, 'l1', 'l2'
        self.reg_strength = reg_strength
        self.tol = tol
        
        self.weights_ = None
        self.bias_ = None
        self.loss_history_ = []
    
    def _sigmoid(self, z):
        """Sigmoid activation function."""
        # Clip values to avoid overflow
        z = np.clip(z, -500, 500)
        return 1 / (1 + np.exp(-z))
    
    def _initialize_parameters(self, n_features):
        """Initialize model parameters."""
        self.weights_ = np.random.randn(n_features) * 0.01
        self.bias_ = 0.0 if self.fit_intercept else None
    
    def _compute_loss(self, X, y):
        """Compute binary cross-entropy loss."""
        predictions = self._predict_proba_internal(X)
        
        # Avoid log(0)
        predictions = np.clip(predictions, 1e-15, 1 - 1e-15)
        
        # Binary cross-entropy
        bce = -np.mean(y * np.log(predictions) + (1 - y) * np.log(1 - predictions))
        
        # Add regularization
        if self.regularization == 'l1':
            bce += self.reg_strength * np.sum(np.abs(self.weights_))
        elif self.regularization == 'l2':
            bce += self.reg_strength * 0.5 * np.sum(self.weights_ ** 2)
        
        return bce
    
    def _predict_proba_internal(self, X):
        """Internal probability prediction method."""
        logits = np.dot(X, self.weights_)
        if self.bias_ is not None:
            logits += self.bias_
        return self._sigmoid(logits)
    
    def fit(self, X, y):
        """Fit the model to the training data."""
        X = np.asarray(X)
        y = np.asarray(y)
        
        n_samples, n_features = X.shape
        self._initialize_parameters(n_features)
        
        self.loss_history_ = []
        
        for i in range(self.n_iterations):
            # Make predictions
            predictions = self._predict_proba_internal(X)
            
            # Compute gradients
            error = predictions - y
            dw = (1 / n_samples) * np.dot(X.T, error)
            db = (1 / n_samples) * np.sum(error) if self.fit_intercept else 0
            
            # Add regularization to gradients
            if self.regularization == 'l1':
                dw += self.reg_strength * np.sign(self.weights_)
            elif self.regularization == 'l2':
                dw += self.reg_strength * self.weights_
            
            # Update parameters
            self.weights_ -= self.learning_rate * dw
            if self.bias_ is not None:
                self.bias_ -= self.learning_rate * db
            
            # Compute loss
            loss = self._compute_loss(X, y)
            self.loss_history_.append(loss)
            
            # Check for convergence
            if i > 0 and abs(self.loss_history_[-2] - loss) < self.tol:
                break
        
        return self
    
    def predict_proba(self, X):
        """Predict class probabilities for new data."""
        if self.weights_ is None:
            raise ValueError("Model has not been fitted yet.")
        
        X = np.asarray(X)
        proba = self._predict_proba_internal(X)
        return np.vstack([1 - proba, proba]).T
    
    def predict(self, X, threshold=0.5):
        """Make binary predictions for new data."""
        proba = self.predict_proba(X)
        return (proba[:, 1] >= threshold).astype(int)


class KMeans(BaseEstimator):
    """K-Means clustering algorithm."""
    
    def __init__(self, n_clusters=8, max_iter=300, tol=1e-4, random_state=None):
        self.n_clusters = n_clusters
        self.max_iter = max_iter
        self.tol = tol
        self.random_state = random_state
        
        self.cluster_centers_ = None
        self.labels_ = None
        self.inertia_ = None
        self.n_iter_ = 0
    
    def _initialize_centroids(self, X):
        """Initialize cluster centroids using k-means++."""
        n_samples, n_features = X.shape
        
        # Set random seed
        if self.random_state is not None:
            np.random.seed(self.random_state)
        
        # First centroid is a random point
        centroids = [X[np.random.randint(0, n_samples)]]
        
        # Initialize remaining centroids
        for _ in range(1, self.n_clusters):
            # Compute distances to nearest centroid
            distances = np.min([
                np.sum((X - centroid) ** 2, axis=1)
                for centroid in centroids
            ], axis=0)
            
            # Probability proportional to distance squared
            probabilities = distances / np.sum(distances)
            
            # Choose next centroid
            next_centroid_idx = np.random.choice(n_samples, p=probabilities)
            centroids.append(X[next_centroid_idx])
        
        return np.array(centroids)
    
    def _assign_clusters(self, X):
        """Assign each point to the nearest centroid."""
        distances = np.array([
            np.sum((X - centroid) ** 2, axis=1)
            for centroid in self.cluster_centers_
        ]).T
        
        return np.argmin(distances, axis=1)
    
    def _update_centroids(self, X):
        """Update centroids based on assigned points."""
        new_centroids = np.zeros((self.n_clusters, X.shape[1]))
        
        for i in range(self.n_clusters):
            cluster_points = X[self.labels_ == i]
            if len(cluster_points) > 0:
                new_centroids[i] = np.mean(cluster_points, axis=0)
            else:
                # Keep old centroid if no points assigned
                new_centroids[i] = self.cluster_centers_[i]
        
        return new_centroids
    
    def _compute_inertia(self, X):
        """Compute within-cluster sum of squares."""
        inertia = 0.0
        for i in range(self.n_clusters):
            cluster_points = X[self.labels_ == i]
            if len(cluster_points) > 0:
                inertia += np.sum((cluster_points - self.cluster_centers_[i]) ** 2)
        return inertia
    
    def fit(self, X, y=None):
        """Fit the model to the data."""
        X = np.asarray(X)
        
        # Initialize centroids
        self.cluster_centers_ = self._initialize_centroids(X)
        
        prev_inertia = float('inf')
        
        for self.n_iter_ in range(self.max_iter):
            # Assign points to clusters
            self.labels_ = self._assign_clusters(X)
            
            # Update centroids
            self.cluster_centers_ = self._update_centroids(X)
            
            # Compute inertia
            self.inertia_ = self._compute_inertia(X)
            
            # Check for convergence
            if abs(prev_inertia - self.inertia_) < self.tol:
                break
                
            prev_inertia = self.inertia_
        
        return self
    
    def predict(self, X):
        """Predict cluster labels for new data."""
        if self.cluster_centers_ is None:
            raise ValueError("Model has not been fitted yet.")
        
        X = np.asarray(X)
        distances = np.array([
            np.sum((X - centroid) ** 2, axis=1)
            for centroid in self.cluster_centers_
        ]).T
        
        return np.argmin(distances, axis=1)


class PCA(Transformer):
    """Principal Component Analysis for dimensionality reduction."""
    
    def __init__(self, n_components=None):
        self.n_components = n_components
        self.components_ = None
        self.explained_variance_ = None
        self.explained_variance_ratio_ = None
        self.mean_ = None
    
    def fit(self, X, y=None):
        """Fit the model with X."""
        X = np.asarray(X)
        
        # Center data
        self.mean_ = np.mean(X, axis=0)
        X_centered = X - self.mean_
        
        # Compute covariance matrix
        cov_matrix = np.cov(X_centered, rowvar=False)
        
        # Compute eigenvalues and eigenvectors
        eigenvalues, eigenvectors = np.linalg.eigh(cov_matrix)
        
        # Sort eigenvectors by decreasing eigenvalues
        sorted_idx = np.argsort(eigenvalues)[::-1]
        eigenvalues = eigenvalues[sorted_idx]
        eigenvectors = eigenvectors[:, sorted_idx]
        
        # Determine number of components
        if self.n_components is None:
            self.n_components = X.shape[1]
        
        # Store components and explained variance
        self.components_ = eigenvectors[:, :self.n_components]
        self.explained_variance_ = eigenvalues[:self.n_components]
        self.explained_variance_ratio_ = eigenvalues[:self.n_components] / np.sum(eigenvalues)
        
        return self
    
    def transform(self, X):
        """Apply dimensionality reduction to X."""
        if self.components_ is None:
            raise ValueError("PCA has not been fitted yet.")
        
        X = np.asarray(X)
        X_centered = X - self.mean_
        return np.dot(X_centered, self.components_)
    
    def fit_transform(self, X, y=None):
        """Fit the model with X and apply dimensionality reduction."""
        return self.fit(X, y).transform(X)


class DecisionTreeClassifier(BaseEstimator):
    """Decision Tree classifier."""
    
    def __init__(self, max_depth=None, min_samples_split=2, min_samples_leaf=1, 
                 criterion='gini', random_state=None):
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.min_samples_leaf = min_samples_leaf
        self.criterion = criterion  # 'gini' or 'entropy'
        self.random_state = random_state
        
        self.tree_ = None
        self.classes_ = None
        self.n_features_ = None
        self.feature_importances_ = None
    
    class Node:
        """Node class for decision tree."""
        
        def __init__(self, feature_idx=None, threshold=None, value=None, 
                     left=None, right=None, is_leaf=False):
            self.feature_idx = feature_idx
            self.threshold = threshold
            self.value = value
            self.left = left
            self.right = right
            self.is_leaf = is_leaf
    
    def _gini_impurity(self, y):
        """Calculate Gini impurity."""
        classes, counts = np.unique(y, return_counts=True)
        probabilities = counts / len(y)
        return 1 - np.sum(probabilities ** 2)
    
    def _entropy(self, y):
        """Calculate entropy."""
        classes, counts = np.unique(y, return_counts=True)
        probabilities = counts / len(y)
        return -np.sum(probabilities * np.log2(probabilities))
    
    def _impurity(self, y):
        """Calculate impurity based on criterion."""
        if self.criterion == 'gini':
            return self._gini_impurity(y)
        elif self.criterion == 'entropy':
            return self._entropy(y)
        else:
            raise ValueError(f"Unknown criterion: {self.criterion}")
    
    def _find_best_split(self, X, y):
        """Find the best split for a node."""
        n_samples, n_features = X.shape
        best_impurity = float('inf')
        best_feature_idx = None
        best_threshold = None
        
        for feature_idx in range(n_features):
            # Sort unique values for this feature
            unique_values = np.unique(X[:, feature_idx])
            
            if len(unique_values) <= 1:
                continue
            
            # Try thresholds between values
            thresholds = (unique_values[:-1] + unique_values[1:]) / 2
            
            for threshold in thresholds:
                # Split data
                left_mask = X[:, feature_idx] <= threshold
                right_mask = ~left_mask
                
                # Skip if one side is empty
                if np.sum(left_mask) == 0 or np.sum(right_mask) == 0:
                    continue
                
                # Calculate weighted impurity
                n_left = np.sum(left_mask)
                n_right = np.sum(right_mask)
                n_total = n_left + n_right
                
                impurity_left = self._impurity(y[left_mask])
                impurity_right = self._impurity(y[right_mask])
                
                weighted_impurity = (n_left / n_total) * impurity_left + \
                                 (n_right / n_total) * impurity_right
                
                # Update if better
                if weighted_impurity < best_impurity:
                    best_impurity = weighted_impurity
                    best_feature_idx = feature_idx
                    best_threshold = threshold
        
        return best_feature_idx, best_threshold, best_impurity
    
    def _create_leaf(self, y):
        """Create a leaf node."""
        classes, counts = np.unique(y, return_counts=True)
        most_common_idx = np.argmax(counts)
        return self.Node(value=classes[most_common_idx], is_leaf=True)
    
    def _build_tree(self, X, y, depth=0):
        """Recursively build the decision tree."""
        n_samples, n_features = X.shape
        
        # Check stopping conditions
        if (self.max_depth is not None and depth >= self.max_depth) or \
           n_samples < self.min_samples_split or \
           len(np.unique(y)) == 1:
            return self._create_leaf(y)
        
        # Find best split
        feature_idx, threshold, impurity = self._find_best_split(X, y)
        
        # If no valid split found
        if feature_idx is None:
            return self._create_leaf(y)
        
        # Split data
        left_mask = X[:, feature_idx] <= threshold
        right_mask = ~left_mask
        
        # Check minimum samples in leaf
        if (np.sum(left_mask) < self.min_samples_leaf or 
            np.sum(right_mask) < self.min_samples_leaf):
            return self._create_leaf(y)
        
        # Build subtrees
        left_subtree = self._build_tree(X[left_mask], y[left_mask], depth + 1)
        right_subtree = self._build_tree(X[right_mask], y[right_mask], depth + 1)
        
        return self.Node(feature_idx=feature_idx, threshold=threshold,
                      left=left_subtree, right=right_subtree)
    
    def _predict_sample(self, x, node=None):
        """Predict class for a single sample."""
        if node is None:
            node = self.tree_
        
        if node.is_leaf:
            return node.value
        
        if x[node.feature_idx] <= node.threshold:
            return self._predict_sample(x, node.left)
        else:
            return self._predict_sample(x, node.right)
    
    def _calculate_feature_importance(self):
        """Calculate feature importance based on impurity reduction."""
        self.feature_importances_ = np.zeros(self.n_features_)
        self._calculate_node_importance(self.tree_)
        
        # Normalize
        total = np.sum(self.feature_importances_)
        if total > 0:
            self.feature_importances_ /= total
    
    def _calculate_node_importance(self, node, parent_impurity=0):
        """Recursively calculate importance for each node."""
        if node.is_leaf:
            return
        
        # Calculate impurity reduction
        # This is a simplified calculation
        self.feature_importances_[node.feature_idx] += 1
        
        # Recurse
        self._calculate_node_importance(node.left)
        self._calculate_node_importance(node.right)
    
    def fit(self, X, y):
        """Fit the decision tree classifier."""
        X = np.asarray(X)
        y = np.asarray(y)
        
        # Set random seed
        if self.random_state is not None:
            np.random.seed(self.random_state)
        
        self.n_features_ = X.shape[1]
        self.classes_ = np.unique(y)
        
        # Build tree
        self.tree_ = self._build_tree(X, y)
        
        # Calculate feature importance
        self._calculate_feature_importance()
        
        return self
    
    def predict(self, X):
        """Predict class for X."""
        if self.tree_ is None:
            raise ValueError("Model has not been fitted yet.")
        
        X = np.asarray(X)
        predictions = [self._predict_sample(x) for x in X]
        return np.array(predictions)


class RandomForestClassifier(BaseEstimator):
    """Random Forest classifier."""
    
    def __init__(self, n_estimators=100, max_depth=None, min_samples_split=2,
                 min_samples_leaf=1, criterion='gini', max_features=None,
                 random_state=None):
        self.n_estimators = n_estimators
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.min_samples_leaf = min_samples_leaf
        self.criterion = criterion
        self.max_features = max_features
        self.random_state = random_state
        
        self.trees_ = None
        self.classes_ = None
        self.feature_importances_ = None
    
    def _bootstrap_sample(self, X, y):
        """Create bootstrap sample."""
        n_samples = X.shape[0]
        indices = np.random.choice(n_samples, n_samples, replace=True)
        return X[indices], y[indices]
    
    def fit(self, X, y):
        """Fit the Random Forest classifier."""
        X = np.asarray(X)
        y = np.asarray(y)
        
        # Set random seed
        if self.random_state is not None:
            np.random.seed(self.random_state)
        
        n_samples, n_features = X.shape
        self.classes_ = np.unique(y)
        
        # Determine max_features if not specified
        if self.max_features is None:
            self.max_features = int(np.sqrt(n_features))
        
        # Create and fit trees
        self.trees_ = []
        
        for _ in range(self.n_estimators):
            # Bootstrap sample
            X_boot, y_boot = self._bootstrap_sample(X, y)
            
            # Create and fit tree
            tree = DecisionTreeClassifier(
                max_depth=self.max_depth,
                min_samples_split=self.min_samples_split,
                min_samples_leaf=self.min_samples_leaf,
                criterion=self.criterion,
                random_state=self.random_state
            )
            
            tree.fit(X_boot, y_boot)
            self.trees_.append(tree)
        
        # Calculate feature importance
        self.feature_importances_ = np.mean(
            [tree.feature_importances_ for tree in self.trees_],
            axis=0
        )
        
        return self
    
    def predict(self, X):
        """Predict class for X."""
        if self.trees_ is None:
            raise ValueError("Model has not been fitted yet.")
        
        X = np.asarray(X)
        
        # Get predictions from all trees
        tree_predictions = np.array([tree.predict(X) for tree in self.trees_])
        
        # Majority vote
        predictions = []
        for i in range(X.shape[0]):
            votes = tree_predictions[:, i]
            most_common = Counter(votes).most_common(1)[0][0]
            predictions.append(most_common)
        
        return np.array(predictions)


class GridSearchCV:
    """Grid search for hyperparameter tuning."""
    
    def __init__(self, estimator, param_grid, cv=5, scoring=None):
        self.estimator = estimator
        self.param_grid = param_grid
        self.cv = cv
        self.scoring = scoring or 'accuracy'
        
        self.best_params_ = None
        self.best_score_ = None
        self.best_estimator_ = None
        self.cv_results_ = []
    
    def _get_param_combinations(self):
        """Get all combinations of parameters."""
        keys = list(self.param_grid.keys())
        values = list(self.param_grid.values())
        combinations = list(itertools.product(*values))
        
        return [dict(zip(keys, combination)) for combination in combinations]
    
    def _cross_validate(self, X, y, params):
        """Perform cross-validation."""
        n_samples = X.shape[0]
        indices = np.arange(n_samples)
        np.random.shuffle(indices)
        
        fold_size = n_samples // self.cv
        scores = []
        
        for i in range(self.cv):
            # Create train/test split for this fold
            start, end = i * fold_size, (i + 1) * fold_size
            if i == self.cv - 1:  # Last fold includes remaining samples
                end = n_samples
            
            test_idx = indices[start:end]
            train_idx = np.concatenate([indices[:start], indices[end:]])
            
            X_train, X_test = X[train_idx], X[test_idx]
            y_train, y_test = y[train_idx], y[test_idx]
            
            # Clone estimator and fit with current parameters
            estimator = type(self.estimator)(**{**self.estimator.__dict__, **params})
            estimator.fit(X_train, y_train)
            
            # Evaluate
            predictions = estimator.predict(X_test)
            
            if self.scoring == 'accuracy':
                score = np.mean(predictions == y_test)
            elif self.scoring == 'f1':
                # Simple F1 calculation for binary classification
                tp = np.sum((predictions == 1) & (y_test == 1))
                fp = np.sum((predictions == 1) & (y_test == 0))
                fn = np.sum((predictions == 0) & (y_test == 1))
                
                if tp + fp + fn == 0:
                    score = 0
                else:
                    precision = tp / (tp + fp) if (tp + fp) > 0 else 0
                    recall = tp / (tp + fn) if (tp + fn) > 0 else 0
                    score = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
            else:
                raise ValueError(f"Unknown scoring metric: {self.scoring}")
            
            scores.append(score)
        
        return np.mean(scores)
    
    def fit(self, X, y):
        """Perform grid search."""
        X = np.asarray(X)
        y = np.asarray(y)
        
        # Get all parameter combinations
        param_combinations = self._get_param_combinations()
        
        # Try each combination
        self.cv_results_ = []
        best_score = -float('inf')
        best_params = None
        
        for params in param_combinations:
            # Cross-validate
            score = self._cross_validate(X, y, params)
            
            # Store results
            result = params.copy()
            result['score'] = score
            self.cv_results_.append(result)
            
            # Update best
            if score > best_score:
                best_score = score
                best_params = params
        
        # Store best parameters and score
        self.best_params_ = best_params
        self.best_score_ = best_score
        
        # Fit best estimator on full data
        self.best_estimator_ = type(self.estimator)(**{**self.estimator.__dict__, **best_params})
        self.best_estimator_.fit(X, y)
        
        return self


class ModelEvaluation:
    """Tools for evaluating model performance."""
    
    @staticmethod
    def confusion_matrix(y_true, y_pred):
        """Compute confusion matrix."""
        # Get unique classes
        classes = np.unique(np.concatenate([y_true, y_pred]))
        n_classes = len(classes)
        
        # Initialize matrix
        matrix = np.zeros((n_classes, n_classes), dtype=int)
        
        # Fill matrix
        for i in range(n_classes):
            for j in range(n_classes):
                matrix[i, j] = np.sum((y_true == classes[i]) & (y_pred == classes[j]))
        
        return matrix
    
    @staticmethod
    def classification_report(y_true, y_pred):
        """Generate classification report."""
        # Compute confusion matrix
        cm = ModelEvaluation.confusion_matrix(y_true, y_pred)
        classes = np.unique(y_true)
        
        report = {}
        
        # Calculate metrics for each class
        for i, cls in enumerate(classes):
            tp = cm[i, i]  # True positives
            fp = np.sum(cm[:, i]) - tp  # False positives
            fn = np.sum(cm[i, :]) - tp  # False negatives
            tn = np.sum(cm) - tp - fp - fn  # True negatives
            
            # Calculate metrics
            precision = tp / (tp + fp) if (tp + fp) > 0 else 0
            recall = tp / (tp + fn) if (tp + fn) > 0 else 0
            f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
            support = np.sum(y_true == cls)
            
            report[str(cls)] = {
                'precision': precision,
                'recall': recall,
                'f1-score': f1,
                'support': support
            }
        
        return report
    
    @staticmethod
    def accuracy_score(y_true, y_pred):
        """Calculate accuracy."""
        return np.mean(y_true == y_pred)
    
    @staticmethod
    def mean_squared_error(y_true, y_pred):
        """Calculate mean squared error."""
        return np.mean((y_true - y_pred) ** 2)
    
    @staticmethod
    def r2_score(y_true, y_pred):
        """Calculate R-squared score."""
        ss_res = np.sum((y_true - y_pred) ** 2)
        ss_tot = np.sum((y_true - np.mean(y_true)) ** 2)
        
        return 1 - (ss_res / ss_tot) if ss_tot > 0 else 0


def train_test_split(X, y, test_size=0.3, random_state=None):
    """Split arrays into random train and test subsets."""
    if random_state is not None:
        np.random.seed(random_state)
    
    n_samples = X.shape[0]
    n_test = int(n_samples * test_size)
    
    # Generate random indices
    indices = np.arange(n_samples)
    np.random.shuffle(indices)
    
    test_idx = indices[:n_test]
    train_idx = indices[n_test:]
    
    return X[train_idx], X[test_idx], y[train_idx], y[test_idx]


def create_classification_dataset(n_samples=1000, n_features=10, n_classes=2, 
                             random_state=42, noise=0.1):
    """Create a synthetic classification dataset."""
    if random_state is not None:
        np.random.seed(random_state)
    
    # Generate random centers
    n_centers = n_classes
    centers = np.random.randn(n_centers, n_features)
    
    # Generate labels and data
    labels = np.random.randint(0, n_centers, n_samples)
    X = np.zeros((n_samples, n_features))
    
    for i in range(n_samples):
        # Point based on its label's center
        center = centers[labels[i]]
        
        # Add some noise
        X[i] = center + np.random.randn(n_features) * noise
    
    return X, labels


def create_regression_dataset(n_samples=1000, n_features=10, noise=0.1, random_state=42):
    """Create a synthetic regression dataset."""
    if random_state is not None:
        np.random.seed(random_state)
    
    # Generate random coefficients
    coeffs = np.random.randn(n_features)
    
    # Generate features
    X = np.random.randn(n_samples, n_features)
    
    # Generate target variable
    y = np.dot(X, coeffs) + np.random.randn(n_samples) * noise
    
    return X, y


def main():
    """Main function to demonstrate the machine learning library."""
    print("Machine Learning Library Demo")
    print("=============================")
    
    # Create synthetic datasets
    print("Creating datasets...")
    X_cls, y_cls = create_classification_dataset(n_samples=500, n_features=10, n_classes=2)
    X_reg, y_reg = create_regression_dataset(n_samples=500, n_features=10)
    
    # Split data
    X_train_cls, X_test_cls, y_train_cls, y_test_cls = train_test_split(
        X_cls, y_cls, test_size=0.3, random_state=42
    )
    X_train_reg, X_test_reg, y_train_reg, y_test_reg = train_test_split(
        X_reg, y_reg, test_size=0.3, random_state=42
    )
    
    # Standardize data
    scaler_cls = StandardScaler()
    X_train_cls = scaler_cls.fit_transform(X_train_cls)
    X_test_cls = scaler_cls.transform(X_test_cls)
    
    scaler_reg = StandardScaler()
    X_train_reg = scaler_reg.fit_transform(X_train_reg)
    X_test_reg = scaler_reg.transform(X_test_reg)
    
    # Classification models
    print("\n=== Classification ===")
    
    # Logistic Regression
    print("Training Logistic Regression...")
    lr = LogisticRegression(learning_rate=0.1, n_iterations=1000)
    lr.fit(X_train_cls, y_train_cls)
    lr_pred = lr.predict(X_test_cls)
    lr_acc = ModelEvaluation.accuracy_score(y_test_cls, lr_pred)
    print(f"Logistic Regression Accuracy: {lr_acc:.4f}")
    
    # Decision Tree
    print("Training Decision Tree...")
    dt = DecisionTreeClassifier(max_depth=5, random_state=42)
    dt.fit(X_train_cls, y_train_cls)
    dt_pred = dt.predict(X_test_cls)
    dt_acc = ModelEvaluation.accuracy_score(y_test_cls, dt_pred)
    print(f"Decision Tree Accuracy: {dt_acc:.4f}")
    
    # Random Forest
    print("Training Random Forest...")
    rf = RandomForestClassifier(n_estimators=100, max_depth=5, random_state=42)
    rf.fit(X_train_cls, y_train_cls)
    rf_pred = rf.predict(X_test_cls)
    rf_acc = ModelEvaluation.accuracy_score(y_test_cls, rf_pred)
    print(f"Random Forest Accuracy: {rf_acc:.4f}")
    
    # Classification Report for best model
    print("\nRandom Forest Classification Report:")
    report = ModelEvaluation.classification_report(y_test_cls, rf_pred)
    for cls, metrics in report.items():
        print(f"  Class {cls}:")
        print(f"    Precision: {metrics['precision']:.4f}")
        print(f"    Recall: {metrics['recall']:.4f}")
        print(f"    F1-score: {metrics['f1-score']:.4f}")
        print(f"    Support: {metrics['support']}")
    
    # Regression models
    print("\n=== Regression ===")
    
    # Linear Regression
    print("Training Linear Regression...")
    lin_reg = LinearRegression(learning_rate=0.01, n_iterations=1000)
    lin_reg.fit(X_train_reg, y_train_reg)
    lin_reg_pred = lin_reg.predict(X_test_reg)
    lin_reg_mse = ModelEvaluation.mean_squared_error(y_test_reg, lin_reg_pred)
    lin_reg_r2 = ModelEvaluation.r2_score(y_test_reg, lin_reg_pred)
    print(f"Linear Regression MSE: {lin_reg_mse:.4f}")
    print(f"Linear Regression R²: {lin_reg_r2:.4f}")
    
    # Clustering
    print("\n=== Clustering ===")
    
    # K-Means
    print("Training K-Means...")
    kmeans = KMeans(n_clusters=2, random_state=42)
    kmeans.fit(X_cls)
    kmeans_labels = kmeans.predict(X_cls)
    print(f"K-Means Inertia: {kmeans.inertia_:.4f}")
    print(f"K-Means Iterations: {kmeans.n_iter_}")
    
    # Dimensionality Reduction
    print("\n=== Dimensionality Reduction ===")
    
    # PCA
    print("Applying PCA...")
    pca = PCA(n_components=2)
    X_pca = pca.fit_transform(X_cls)
    print(f"Original Dimensions: {X_cls.shape[1]}")
    print(f"Reduced Dimensions: {X_pca.shape[1]}")
    print(f"Explained Variance Ratio: {pca.explained_variance_ratio_}")
    
    # Hyperparameter Tuning
    print("\n=== Hyperparameter Tuning ===")
    
    # Grid Search for Decision Tree
    print("Performing Grid Search for Decision Tree...")
    param_grid = {
        'max_depth': [3, 5, 7],
        'min_samples_split': [2, 5, 10],
        'criterion': ['gini', 'entropy']
    }
    
    base_dt = DecisionTreeClassifier()
    grid_search = GridSearchCV(base_dt, param_grid, cv=3)
    grid_search.fit(X_train_cls, y_train_cls)
    
    print(f"Best Parameters: {grid_search.best_params_}")
    print(f"Best Score: {grid_search.best_score_:.4f}")
    
    # Evaluate best model
    best_pred = grid_search.best_estimator_.predict(X_test_cls)
    best_acc = ModelEvaluation.accuracy_score(y_test_cls, best_pred)
    print(f"Best Model Test Accuracy: {best_acc:.4f}")
    
    # Feature Importance
    print("\n=== Feature Importance ===")
    
    if hasattr(rf, 'feature_importances_'):
        print("Random Forest Feature Importance:")
        for i, importance in enumerate(rf.feature_importances_):
            print(f"  Feature {i}: {importance:.4f}")
    
    if hasattr(dt, 'feature_importances_'):
        print("Decision Tree Feature Importance:")
        for i, importance in enumerate(dt.feature_importances_):
            print(f"  Feature {i}: {importance:.4f}")
    
    # Learning Curves
    print("\n=== Learning Curves ===")
    
    # Plot loss curves for models that track them
    if hasattr(lr, 'loss_history_'):
        plt.figure(figsize=(12, 5))
        
        plt.subplot(1, 2, 1)
        plt.plot(lr.loss_history_)
        plt.title('Logistic Regression Loss Curve')
        plt.xlabel('Iteration')
        plt.ylabel('Loss')
        
        if hasattr(lin_reg, 'loss_history_'):
            plt.subplot(1, 2, 2)
            plt.plot(lin_reg.loss_history_)
            plt.title('Linear Regression Loss Curve')
            plt.xlabel('Iteration')
            plt.ylabel('Loss')
        
        plt.tight_layout()
        plt.savefig('loss_curves.png')
        print("Loss curves saved to 'loss_curves.png'")
    
    # Visualize PCA results
    plt.figure(figsize=(10, 8))
    for cls in np.unique(y_cls):
        mask = y_cls == cls
        plt.scatter(X_pca[mask, 0], X_pca[mask, 1], label=f'Class {cls}')
    plt.title('PCA Visualization')
    plt.xlabel('Principal Component 1')
    plt.ylabel('Principal Component 2')
    plt.legend()
    plt.savefig('pca_visualization.png')
    print("PCA visualization saved to 'pca_visualization.png'")
    
    # Visualize clustering results
    plt.figure(figsize=(10, 8))
    for cluster in np.unique(kmeans_labels):
        mask = kmeans_labels == cluster
        plt.scatter(X_cls[mask, 0], X_cls[mask, 1], label=f'Cluster {cluster}')
    
    # Plot centroids
    if hasattr(kmeans, 'cluster_centers_'):
        plt.scatter(kmeans.cluster_centers_[:, 0], kmeans.cluster_centers_[:, 1],
                   marker='x', s=100, c='red', label='Centroids')
    
    plt.title('K-Means Clustering Results')
    plt.xlabel('Feature 1')
    plt.ylabel('Feature 2')
    plt.legend()
    plt.savefig('kmeans_clustering.png')
    print("K-Means clustering visualization saved to 'kmeans_clustering.png'")
    
    print("\nDemo completed successfully!")


if __name__ == "__main__":
    main()
<｜fim▁hole｜>