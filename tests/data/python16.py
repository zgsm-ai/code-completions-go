"""
Computer Vision and Image Processing Library

This module implements a comprehensive computer vision and image processing library
that can handle various image analysis, manipulation, and computer vision tasks.

Features:
- Image I/O and basic operations
- Image filtering and enhancement
- Edge detection and feature extraction
- Object detection using classical methods
- Face detection and recognition
- Image segmentation
- Optical character recognition (OCR)
- Color space conversions
- Geometric transformations
- Image visualization
"""

import os
import math
import random
from typing import List, Dict, Tuple, Optional, Union, Any, Callable
import struct
import time

# Try to import PIL for image processing
try:
    from PIL import Image, ImageFilter, ImageEnhance, ImageDraw, ImageFont, ImageStat
    PIL_AVAILABLE = True
except ImportError:
    try:
        import pillow
        from PIL import Image, ImageFilter, ImageEnhance, ImageDraw, ImageFont, ImageStat
        PIL_AVAILABLE = True
    except ImportError:
        PIL_AVAILABLE = False

# Try to import numpy for numerical operations
try:
    import numpy as np
    NUMPY_AVAILABLE = True
except ImportError:
    NUMPY_AVAILABLE = False


class ImageProcessor:
    """
    Core image processing and manipulation class.
    """
    
    def __init__(self, image_path=None):
        """
        Initialize the image processor.
        
        Args:
            image_path: Path to load an image
        """
        self.image = None
        self.image_data = None
        self.width = 0
        self.height = 0
        self.channels = 0
        self.format = None
        
        if image_path and self.load_image(image_path):
            self._update_image_info()
    
    def load_image(self, image_path):
        """
        Load an image from file.
        
        Args:
            image_path: Path to the image file
            
        Returns:
            bool: True if loaded successfully, False otherwise
        """
        try:
            if not PIL_AVAILABLE:
                raise ImportError("PIL is not available. Install with: pip install Pillow")
            
            self.image = Image.open(image_path)
            self.image_data = np.array(self.image) if NUMPY_AVAILABLE else None
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error loading image: {str(e)}")
            return False
    
    def save_image(self, output_path, quality=95):
        """
        Save the current image to a file.
        
        Args:
            output_path: Path to save the image
            quality: Compression quality for JPEG
            
        Returns:
            bool: True if saved successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            # Ensure directory exists
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            
            # Convert to RGB if necessary for JPEG
            if self.image.mode in ('RGBA', 'LA', 'P'):
                rgb_image = Image.new('RGB', self.image.size)
                rgb_image.paste(self.image, mask=self.image.split()[-1] if self.image.mode == 'RGBA' else None)
                image_to_save = rgb_image
            else:
                image_to_save = self.image
            
            # Save with specified quality
            if output_path.lower().endswith('.jpg') or output_path.lower().endswith('.jpeg'):
                image_to_save.save(output_path, quality=quality)
            else:
                image_to_save.save(output_path)
            
            return True
        except Exception as e:
            print(f"Error saving image: {str(e)}")
            return False
    
    def resize(self, width, height, resample=Image.LANCZOS):
        """
        Resize the image to specified dimensions.
        
        Args:
            width: New width in pixels
            height: New height in pixels
            resample: Resampling filter to use
            
        Returns:
            bool: True if resized successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            self.image = self.image.resize((width, height), resample)
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error resizing image: {str(e)}")
            return False
    
    def rotate(self, angle, expand=True, fillcolor=None):
        """
        Rotate the image by specified angle.
        
        Args:
            angle: Rotation angle in degrees
            expand: Whether to expand the image canvas
            fillcolor: Color to use for empty pixels
            
        Returns:
            bool: True if rotated successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            self.image = self.image.rotate(angle, expand=expand, fillcolor=fillcolor)
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error rotating image: {str(e)}")
            return False
    
    def crop(self, left, top, right, bottom):
        """
        Crop the image to specified rectangular area.
        
        Args:
            left: Left coordinate
            top: Top coordinate
            right: Right coordinate
            bottom: Bottom coordinate
            
        Returns:
            bool: True if cropped successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            self.image = self.image.crop((left, top, right, bottom))
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error cropping image: {str(e)}")
            return False
    
    def flip(self, direction='horizontal'):
        """
        Flip the image in specified direction.
        
        Args:
            direction: 'horizontal' or 'vertical'
            
        Returns:
            bool: True if flipped successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            if direction == 'horizontal':
                self.image = self.image.transpose(Image.FLIP_LEFT_RIGHT)
            elif direction == 'vertical':
                self.image = self.image.transpose(Image.FLIP_TOP_BOTTOM)
            else:
                return False
            
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error flipping image: {str(e)}")
            return False
    
    def adjust_brightness(self, factor):
        """
        Adjust image brightness.
        
        Args:
            factor: Brightness adjustment factor (1.0 = no change)
            
        Returns:
            bool: True if adjusted successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            enhancer = ImageEnhance.Brightness(self.image)
            self.image = enhancer.enhance(factor)
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error adjusting brightness: {str(e)}")
            return False
    
    def adjust_contrast(self, factor):
        """
        Adjust image contrast.
        
        Args:
            factor: Contrast adjustment factor (1.0 = no change)
            
        Returns:
            bool: True if adjusted successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            enhancer = ImageEnhance.Contrast(self.image)
            self.image = enhancer.enhance(factor)
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error adjusting contrast: {str(e)}")
            return False
    
    def convert_color_space(self, mode):
        """
        Convert the image to a different color mode.
        
        Args:
            mode: Target color mode ('L', 'RGB', 'RGBA', 'CMYK', 'YCbCr', etc.)
            
        Returns:
            bool: True if converted successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            self.image = self.image.convert(mode)
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error converting color space: {str(e)}")
            return False
    
    def apply_filter(self, filter_name, **kwargs):
        """
        Apply a filter to the image.
        
        Args:
            filter_name: Name of the filter
            **kwargs: Additional arguments for the filter
            
        Returns:
            bool: True if filter applied successfully, False otherwise
        """
        if not self.image or not PIL_AVAILABLE:
            return False
        
        try:
            # Get the filter
            if filter_name == 'blur':
                filter = ImageFilter.BLUR
                if 'radius' in kwargs:
                    filter = ImageFilter.GaussianBlur(radius=kwargs['radius'])
            elif filter_name == 'edge':
                filter = ImageFilter.FIND_EDGES
            elif filter_name == 'contour':
                filter = ImageFilter.CONTOUR
            elif filter_name == 'sharp':
                filter = ImageFilter.SHARPEN
            elif filter_name == 'emboss':
                filter = ImageFilter.EMBOSS
            else:
                # Try to get filter by name
                try:
                    filter = getattr(ImageFilter, filter_name.upper())
                except AttributeError:
                    filter = ImageFilter.BLUR  # Default
            
            # Apply the filter
            self.image = self.image.filter(filter)
            self._update_image_info()
            return True
        except Exception as e:
            print(f"Error applying filter: {str(e)}")
            return False
    
    def get_thumbnail(self, size=(128, 128)):
        """
        Generate a thumbnail of the image.
        
        Args:
            size: Size of the thumbnail (width, height)
            
        Returns:
            Thumbnail image or None if failed
        """
        if not self.image or not PIL_AVAILABLE:
            return None
        
        try:
            thumbnail = self.image.copy()
            thumbnail.thumbnail(size)
            return thumbnail
        except Exception as e:
            print(f"Error creating thumbnail: {str(e)}")
            return None
    
    def get_histogram(self, bins=256):
        """
        Calculate the histogram of the image.
        
        Args:
            bins: Number of histogram bins
            
        Returns:
            Dictionary with histogram data
        """
        if not self.image or not PIL_AVAILABLE:
            return {}
        
        try:
            # Calculate histogram for each channel
            histogram = self.image.histogram(bins=bins)
            
            if self.image.mode == 'RGB':
                return {
                    'red': histogram[0:256],
                    'green': histogram[256:512],
                    'blue': histogram[512:768],
                    'rgb': histogram
                }
            else:
                # Grayscale image
                return {'grayscale': histogram}
        except Exception as e:
            print(f"Error calculating histogram: {str(e)}")
            return {}
    
    def get_info(self):
        """
        Get basic information about the image.
        
        Returns:
            Dictionary with image information
        """
        return {
            'width': self.width,
            'height': self.height,
            'channels': self.channels,
            'mode': self.format,
            'size': self.width * self.height
        }
    
    def _update_image_info(self):
        """Update internal image information."""
        if self.image:
            self.width, self.height = self.image.size
            self.channels = len(self.image.getbands())
            self.format = self.image.mode


class EdgeDetector:
    """
    Edge detection using various algorithms.
    """
    
    def __init__(self):
        """Initialize the edge detector."""
        pass
    
    def detect_edges_sobel(self, image_data):
        """
        Detect edges using Sobel operator.
        
        Args:
            image_data: Image data as numpy array
            
        Returns:
            Edge-detected image as numpy array
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Sobel kernels
            kernel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
            kernel_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])
            
            # Apply convolution
            edges_x = np.zeros_like(image_data)
            edges_y = np.zeros_like(image_data)
            
            # Manual convolution
            for i in range(1, image_data.shape[0]-1):
                for j in range(1, image_data.shape[1]-1):
                    for k in range(3):
                        edges_x[i, j] += image_data[i+k-1, j+k-1] * kernel_x[k]
                        edges_y[i, j] += image_data[i+k-1, j+k-1] * kernel_y[k]
            
            # Calculate gradient magnitude
            edges = np.sqrt(edges_x**2 + edges_y**2)
            
            # Normalize and threshold
            edges = np.uint8(edges * 255 / np.max(edges))
            
            return edges
        except Exception as e:
            print(f"Error in Sobel edge detection: {str(e)}")
            return None
    
    def detect_edges_canny(self, image_data, low_threshold=50, high_threshold=150):
        """
        Detect edges using Canny edge detector.
        
        Args:
            image_data: Image data as numpy array
            low_threshold: Lower threshold for hysteresis
            high_threshold: Upper threshold for hysteresis
            
        Returns:
            Edge-detected image as numpy array
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Simplified Canny edge detection
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Apply Gaussian blur
            kernel_size = 5
            kernel = np.ones((kernel_size, kernel_size), np.float32) / (kernel_size * kernel_size)
            kernel[2, 2] = 4.0  # Sigma
            
            blurred = np.zeros_like(image_data, dtype=np.float32)
            
            # Apply convolution
            for i in range(kernel_size//2, image_data.shape[0]-kernel_size//2):
                for j in range(kernel_size//2, image_data.shape[1]-kernel_size//2):
                    for k in range(kernel_size):
                        for l in range(kernel_size):
                            blurred[i+kernel_size//2, j+kernel_size//2] += image_data[i:i+kernel_size, j:j+kernel_size] * kernel[k, l]
            
            # Canny edge detection algorithm
            edges = np.zeros_like(image_data)
            
            # Non-maximum suppression
            for i in range(image_data.shape[0]-2):
                for j in range(image_data.shape[1]-2):
                    # Current pixel
                    current = blurred[i+1, j+1]
                    
                    # 8-connected neighbors
                    neighbors = [
                        blurred[i, j], blurred[i, j+1], blurred[i+1, j], blurred[i+1, j+2],
                        blurred[i+2, j], blurred[i+2, j+1], blurred[i+2, j+2]
                    ]
                    
                    # Check if current pixel is a local maximum
                    if current >= low_threshold and np.any(neighbor >= high_threshold):
                        edges[i+1, j+1] = 255
                    elif current >= high_threshold and not np.any(neighbor >= high_threshold):
                        edges[i+1, j+1] = 0
                    elif current > low_threshold:
                        if np.any(neighbor >= high_threshold):
                            edges[i+1, j+1] = 0
                    else:
                        if np.any(neighbor >= low_threshold):
                            edges[i+1, j+1] = 255
            
            return edges
        except Exception as e:
            print(f"Error in Canny edge detection: {str(e)}")
            return None
    
    def detect_edges_prewitt(self, image_data, threshold=30):
        """
        Detect edges using Prewitt operator.
        
        Args:
            image_data: Image data as numpy array
            threshold: Threshold for edge detection
            
        Returns:
            Edge-detected image as numpy array
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Prewitt kernels
            kernels = self._get_prewitt_kernels()
            
            # Apply Prewitt operator
            edges = np.zeros_like(image_data)
            
            for kernel in kernels:
                edges_x = np.zeros_like(image_data)
                edges_y = np.zeros_like(image_data)
                
                # Manual convolution
                for i in range(1, image_data.shape[0]-1):
                    for j in range(1, image_data.shape[1]-1):
                        for k in range(3):
                            for l in range(3):
                                edges_x[i, j] += image_data[i+k-1, j+l-1] * kernel[k][l]
                                edges_y[i, j] += image_data[i+k-1, j+l-1] * kernel[k][l]
                
                # Calculate gradient magnitude
                edges = np.sqrt(edges_x**2 + edges_y**2)
                
                # Update if this kernel gives stronger edges
                if np.max(edges) > np.max(edges):
                    edges = np.maximum(edges, np.zeros_like(edges))
            
            # Apply threshold
            edges = np.where(edges > threshold, 255, 0).astype(np.uint8)
            
            return edges
        except Exception as e:
            print(f"Error in Prewitt edge detection: {str(e)}")
            return None
    
    def _rgb_to_grayscale(self, image_data):
        """
        Convert RGB image to grayscale.
        
        Args:
            image_data: RGB image data as numpy array
            
        Returns:
            Grayscale image data as numpy array
        """
        if len(image_data.shape) == 3:
            # Use weighted sum for RGB to grayscale conversion
            return (0.299 * image_data[:,:,0] + 
                    0.587 * image_data[:,:,1] + 
                    0.114 * image_data[:,:,2]).astype(np.uint8)
        else:
            # Already grayscale
            return image_data
    
    def _get_prewitt_kernels(self):
        """
        Get Prewitt kernels for edge detection.
        
        Returns:
            List of 3x3 kernels
        """
        return [
            np.array([[-1, -1, -1], [1, 0, 0], [0, 1, 1]),
            np.array([[-1, -1, -1], [1, -2, 1], [0, 0, 1]),
            np.array([[-1, -1, -1], [0, 1, 0], [1, 1, 0]),
            np.array([[-1, -1, -1], [0, 0, 1], [1, 0, 0])
        ]


class FeatureExtractor:
    """
    Feature extraction from images using classical computer vision methods.
    """
    
    def __init__(self):
        """Initialize the feature extractor."""
        pass
    
    def extract_harris_corners(self, image_data, k=0.04, window_size=3):
        """
        Detect corners using Harris corner detection.
        
        Args:
            image_data: Image data as numpy array
            k: Harris detector sensitivity parameter
            window_size: Size of the window for corner detection
            
        Returns:
            List of detected corners
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Calculate image gradients
            ix_x = np.zeros_like(image_data, dtype=np.float64)
            iy_x = np.zeros_like(image_data, dtype=np.float64)
            ix_y = np.zeros_like(image_data, dtype=np.float64)
            
            # Sobel kernels
            kernel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
            kernel_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])
            
            # Apply Sobel operator
            for i in range(1, image_data.shape[0]-1):
                for j in range(1, image_data.shape[1]-1):
                    for k in range(3):
                        for l in range(3):
                            ix_x[i, j] += image_data[i+k-1, j+l-1] * kernel_x[k, l]
                            iy_x[i, j] += image_data[i+k-1, j+l-1] * kernel_y[k, l]
            
            # Calculate Harris response function
            R = np.zeros_like(image_data, dtype=np.float64)
            for i in range(1, image_data.shape[0]-1):
                for j in range(1, image_data.shape[1]-1):
                    for k in range(3):
                        for l in range(3):
                            R[i+k, j+l] = (ix_x[i+k, j+l] * iy_x[i+k, j+l]) - (ix_y[i+k, j+l])**2
                            R[i+k, j+l] = (ix_y[i+k, j+l] + (ix_x[i+k, j+l])**2
            
            # Apply Gaussian blur to reduce noise
            R = ndimage.gaussian_filter(R, sigma=1.0)
            
            # Harris corner response
            R = R + k**2 * np.mean(R)  # Add small constant to avoid zero
            
            # Find local maxima
            corners = []
            local_max = ndimage.maximum_filter(R, size=window_size)
            
            for i in range(window_size//2, image_data.shape[0]-window_size//2):
                for j in range(window_size//2, image_data.shape[1]-window_size//2):
                    if local_max[i, j] == R[i, j]:
                        # Check if it's a local maximum in its neighborhood
                        valid = True
                        for l in range(3):
                            for m in range(3):
                                for n in range(3):
                                    if local_max[i+l, j+m] > R[i+l, j+n]:
                                        valid = False
                        if not valid:
                            corners.append((j, i, R[i, j]))
            
            return corners
        except Exception as e:
            print(f"Error in Harris corner detection: {str(e)}")
            return []
    
    def extract_sift_features(self, image_data, num_features=100, contrast_threshold=0.04):
        """
        Extract SIFT-like features (simplified implementation).
        
        Args:
            image_data: Image data as numpy array
            num_features: Maximum number of features to extract
            contrast_threshold: Threshold for keypoint detection
            
        Returns:
            List of detected features
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Simplified SIFT-like feature extraction
            # This is a very simplified version of SIFT
            features = []
            
            # Calculate image gradients
            ix = np.zeros_like(image_data, dtype=np.float64)
            iy = np.zeros_like(image_data, dtype=np.float64)
            
            # Sobel kernels
            kernel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
            kernel_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])
            
            # Apply Sobel operator
            for i in range(1, image_data.shape[0]-1):
                for j in range(1, image_data.shape[1]-1):
                    for k in range(3):
                        for l in range(3):
                            ix_x[i, j] += image_data[i+k-1, j+l-1] * kernel_x[k, l]
                            iy_x[i, j] += image_data[i+k-1, j+l-1] * kernel_y[k, l]
            
            # Calculate gradient magnitude and orientation
            G = np.sqrt(ix_x**2 + iy_x**2)
            theta = np.arctan2(iy_x, ix_x)
            
            # Find keypoints using contrast thresholding
            for i in range(1, image_data.shape[0]-1):
                for j in range(1, image_data.shape[1]-1):
                    G2 = ix_x[i, j]**2 + iy_x[i, j]**2
                    if G2 > contrast_threshold:
                        # Found a keypoint
                        features.append({
                            'x': j,
                            'y': i,
                            'scale': self._calculate_keypoint_scale(G, 8, k),
                            'orientation': theta[i, j],
                            'magnitude': G2[i, j]
                        })
            
            # Limit to specified number of features
            if len(features) > num_features:
                features = features[:num_features]
            
            return features
        except Exception as e:
            print(f"Error in SIFT feature extraction: {str(e)}")
            return []
    
    def _calculate_keypoint_scale(self, G, octave_size, k):
        """
        Calculate the scale of a keypoint.
        
        Args:
            G: Gradient magnitude
            octave_size: Size of the image patch
            k: Parameter for scale calculation
            
        Returns:
            Scale value
        """
        # Simplified scale calculation
        return k * octave_size / 8.0
    
    def _rgb_to_grayscale(self, image_data):
        """
        Convert RGB image to grayscale.
        
        Args:
            image_data: RGB image data as numpy array
            
        Returns:
            Grayscale image data as numpy array
        """
        if len(image_data.shape) == 3:
            # Use weighted sum for RGB to grayscale conversion
            return (0.299 * image_data[:,:,0] + 
                    0.587 * image_data[:,:,1] + 
                    0.114 * image_data[:,:,2]).astype(np.uint8)
        else:
            # Already grayscale
            return image_data


class FaceDetector:
    """
    Face detection using simplified template matching.
    """
    
    def __init__(self):
        """Initialize the face detector."""
        self.face_templates = self._create_face_templates()
        self.min_face_size = 50
        self.max_face_size = 500
    
    def _create_face_templates(self):
        """
        Create simple face templates for detection.
        
        Returns:
            List of face templates
        """
        # This is a simplified face template
        # In a real implementation, you would use more sophisticated templates
        templates = []
        
        # Simple rectangular face template
        face = np.zeros((50, 50), dtype=np.uint8)
        
        # Create a simple face pattern
        # Eyes
        face[15:20] = 255
        face[25:20] = 255
        face[20:30] = 255
        face[20:10] = 255
        
        # Nose
        face[20:25] = 255
        face[18:35] = 255
        
        # Mouth
        face[25:35] = 255
        face[27:45] = 255
        face[22:50] = 255
        
        templates.append(('frontal', face))
        
        # Profile view (simplified)
        profile = np.zeros((50, 50), dtype=np.uint8)
        
        # Outline of face
        cv2.rectangle(profile, (10, 10), (40, 40), 2)
        
        # Eyes
        cv2.circle(profile, (25, 20), 5, 2)
        cv2.circle(profile, (25, 30), 5, 2)
        
        # Mouth
        cv2.ellipse(profile, (25, 40), (15, 10), 10, 5, 2)
        
        templates.append(('profile', profile))
        
        return templates
    
    def detect_faces(self, image_data, threshold=0.8):
        """
        Detect faces in the image using template matching.
        
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Convert to grayscale if needed
        if len(image_data.shape) == 3:
            image_data = self._rgb_to_grayscale(image_data)
        
        faces = []
        
        # Try each template
        for template_type, template in self.face_templates:
            # Simple template matching using correlation
            result = ndimage.correlate(image_data, template)
            
            # Find the maximum correlation value
            min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(result)
            
            # Check if the match is above threshold
            if max_val > threshold and min_val < 0.9 * max_val:
                # Calculate face bounding box
                h, w = template.shape
                x = max_loc[1]
                y = max_loc[0]
                
                # Scale the template to the found face size
                face_size = np.sqrt((x2 + y2))
                scale = face_size / template.shape[0]
                
                # Calculate face bounding box
                face_x = x * scale
                face_y = y * scale
                face_w = w * scale
                face_h = h * scale
                
                faces.append({
                    'type': template_type,
                    'x': face_x,
                    'y': face_y,
                    'width': face_w,
                    'height': face_h,
                    'confidence': max_val,
                    'template': template
                })
        
        # Filter faces by size
        valid_faces = []
        for face in faces:
            size = face['width'] * face['height']
            if self.min_face_size <= size <= self.max_face_size:
                valid_faces.append(face)
        
        return valid_faces
    
    def detect_eyes(self, face_region):
        """
        Detect eyes in a face region.
        
        Args:
            face_region: Region of the face
            
        Returns:
            List of detected eye positions
        """
        if not NUMPY_AVAILABLE:
            return []
        
        # This is a simplified eye detection
        # In a real implementation, you would use more sophisticated methods
        eyes = []
        
        # Calculate the eye level (roughly in the upper part of the face)
        eye_y = face_region.shape[0] // 3
        eye_height = face_region.shape[1] // 4
        eye_width = face_region.shape[1] // 6
        
        # Look for dark regions that might be eyes
        for i in range(eye_y):
            for j in range(face_region.shape[1]):
                region_start = i
                region_end = min(i + eye_height, face_region.shape[1])
                region = face_region[i:i+eye_height, :]
                
                # Calculate average intensity in this region
                avg_intensity = np.mean(region)
                
                # If the region is significantly darker than the face average
                if avg_intensity < 0.7:  # Arbitrary threshold
                    center_x = j + eye_width // 2
                    center_y = i + eye_height // 2
                    
                    eyes.append({
                        'x': center_x,
                        'y': center_y,
                        'radius': min(eye_width, eye_height) // 2
                    })
        
        return eyes


class ImageSegmenter:
    """
    Image segmentation using various algorithms.
    """
    
    def __init__(self):
        """Initialize the image segmenter."""
        pass
    
    def segment_kmeans(self, image_data, k=4):
        """
        Segment the image using K-means clustering.
        
        Args:
            image_data: Image data as numpy array
            k: Number of clusters
            
        Returns:
            Segmented image and cluster assignments
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Reshape image to 1D array
            data = image_data.reshape((-1, image_data.shape[0] * image_data.shape[1])).astype(np.float64)
            
            # Apply K-means clustering
            from sklearn.cluster import KMeans
            kmeans = KMeans(n_clusters=k, random_state=42)
            labels = kmeans.fit_predict(data)
            
            # Reshape labels back to 2D image
            segmented = labels.reshape((image_data.shape[0], image_data.shape[1]))
            
            return segmented, kmeans.cluster_centers_
        except ImportError:
            print("scikit-learn not available. Using simple thresholding instead")
            # Fallback to simple thresholding
            return self._threshold_segmentation(image_data, k)
        except Exception as e:
            print(f"Error in K-means segmentation: {str(e)}")
            return None, None
    
    def threshold_segmentation(self, image_data, k=3):
        """
        Segment the image using thresholding.
        
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Simple Otsu's method
            # Find thresholds manually
            thresholds = [85, 170, 255]
            
            # Use the thresholds
            segmented = np.zeros_like(image_data)
            for threshold in thresholds:
                segmented[image_data > threshold] = len(thresholds) - 1
            
            # Apply K-means to find the best segmentation
            from sklearn.cluster import KMeans
            kmeans = KMeans(n_clusters=k, random_state=42)
            
            # Reshape image to 1D array
            data = image_data.reshape((-1, image_data.shape[0] * image_data.shape[1])).astype(np.float64)
            
            # Fit K-means and predict
            kmeans.fit(data)
            labels = kmeans.predict(data)
            
            # Use K-means labels if they're better than thresholding
            # Calculate silhouette scores for thresholding
            from sklearn.metrics import silhouette_score
            threshold_scores = [silhouette_score(data, np.array(thresholds) - len(thresholds) + 1)]
            kmeans_score = silhouette_score(data, labels)
            
            if kmeans_score > max(threshold_scores):
                segmented = labels.reshape((image_data.shape[0], image_data.shape[1]))
            
            return segmented, kmeans.cluster_centers_
        except ImportError:
            print("scikit-learn not available. Using simple thresholding")
            return None, None
    
    def region_growing(self, image_data, seed_point, threshold=85):
        """
        Segment the image using region growing.
        
        Args:
            image_data: Image data as numpy array
            seed_point: Starting point for segmentation
            threshold: Threshold for region growth
            
        Returns:
            Segmented image
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        try:
            # Convert to grayscale if needed
            if len(image_data.shape) == 3:
                image_data = self._rgb_to_grayscale(image_data)
            
            # Initialize segmented image
            segmented = np.zeros_like(image_data)
            
            # Simple region growing implementation
            # In a real implementation, you would use more sophisticated methods
            region = 255  # Start with the seed point
            segmented[seed_point[1], seed_point[0]] = region
            
            # Grow region iteratively
            changed = True
            while changed:
                changed = False
                
                # Check border pixels
                for i in range(1, image_data.shape[0]-1):
                    for j in range(1, image_data.shape[1]-1):
                        # Check if this pixel is in the region
                        if segmented[i, j] == 255:
                            # Check neighbors
                            for di in [-1, 0, 1]:
                                for dj in [-1, 0, 1]:
                                    if (0 <= i+di < image_data.shape[0]) and \
                                       (0 <= j+dj < image_data.shape[1]) and \
                                       segmented[i+di, j+dj] == 255:
                                        # Add border pixel if it meets the condition
                                        if image_data[i+di, j+dj] > threshold:
                                            segmented[i+di, j+dj] = 255
                                            changed = True
            
                # Stop if no changes in this iteration
                if not changed:
                    break
            
            return segmented
        except Exception as e:
            print(f"Error in region growing: {str(e)}")
            return None


class OCRProcessor:
    """
    Optical Character Recognition for extracting text from images.
    """
    
    def __init__(self):
        """Initialize the OCR processor."""
        self.characters = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        self.character_templates = self._create_character_templates()
    
    def _create_character_templates(self):
        """
        Create simple character templates for OCR.
        
        Returns:
            Dictionary of character templates
        """
        templates = {}
        
        # Create simple templates for digits
        for digit in '0123456789':
            templates[digit] = np.zeros((10, 6), dtype=np.uint8)
            # Draw the digit
            templates[digit][2:3] = 255  # Top
            templates[digit][5:0] = 255  # Bottom
            templates[digit][7:2] = 255
            templates[digit][8:2] = 255
            templates[digit][3:0] = 255
            templates[digit][3:3] = 255
            templates[digit][4:1] = 255
            templates[digit][4:2] = 255
            templates[digit][4:4] = 255
            
            templates[digit][5:3] = 255
            templates[digit][5:5] = 255
        
        # Create simple templates for some letters
        simple_letters = 'AEIOU'
        for letter in simple_letters:
            templates[letter] = np.zeros((10, 6), dtype=np.uint8)
            # Draw the letter
            templates[letter][2:4] = 255  # Top
            templates[letter][7:1] = 255  # Bottom
            templates[letter][7:4] = 255
            
            # Fill in the letter
            templates[letter][3:2:255] = 255
            templates[letter][3:3] = 255
            templates[letter][4:3] = 255
            templates[letter][4:1] = 255
        
        return templates
    
    def recognize_characters(self, image_data):
        """
        Recognize characters from the image.
        
        Args:
            image_data: Image data as numpy array
            
        Returns:
            List of recognized characters
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Preprocess the image
        if len(image_data.shape) == 3:
            image_data = self._rgb_to_grayscale(image_data)
        
        # Invert the image (dark text on light background)
        inverted_image = 255 - image_data
        
        recognized_chars = []
        
        # Try each character template
        for char, template in self.character_templates.items():
            # Use template matching
            result = ndimage.correlate(inverted_image, template)
            
            # Find the maximum correlation value
            _, max_val, _, _ = cv2.minMaxLoc(result)
            
            # Check if the match is above threshold
            if max_val > 0.5:
                # Calculate character position
                h, w = template.shape
                x = _[1]
                y = _[0]
                
                # Scale the template to the found character size
                char_size = np.sqrt((x2 + y2))
                scale = char_size / template.shape[0]
                
                # Calculate character bounding box
                char_x = x * scale
                char_y = y * scale
                char_w = w * scale
                char_h = h * scale
                
                recognized_chars.append({
                    'character': char,
                    'x': char_x,
                    'y': char_y,
                    'width': char_w,
                    'height': char_h,
                    'confidence': max_val
                })
        
        # Sort by confidence
        recognized_chars.sort(key=lambda x: x['confidence'], reverse=True)
        
        return recognized_chars
    
    def extract_text_from_boxes(self, text_boxes):
        """
        Extract text from a list of character bounding boxes.
        
        Args:
            text_boxes: List of character bounding boxes
            
        Returns:
            Extracted text string
        """
        if not text_boxes:
            return ""
        
        # Sort boxes by x coordinate
        text_boxes.sort(key=lambda x: x['x'])
        
        # Simple text extraction
        text = ""
        prev_box = None
        
        for box in text_boxes:
            if prev_box and abs(box['x'] - prev_box['x'] - prev_box['width']) > 20:
                # Add space between words
                text += " "
            
            text += box['character']
            prev_box = box
        
        return text


class ImageVisualizer:
    """
    Image visualization utility class.
    """
    
    def __init__(self):
        """Initialize the image visualizer."""
        pass
    
    def draw_rectangle(self, image_data, x, y, width, height, color=(0, 255, 0), thickness=1):
        """
        Draw a rectangle on the image.
        
        Args:
            image_data: Image data to draw on
            x, y: Top-left corner of the rectangle
            width, height: Width and height of the rectangle
            color: RGB color as (R, G, B)
            thickness: Line thickness
            
        Returns:
            Image with rectangle drawn
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Convert to PIL if needed
        if not PIL_AVAILABLE and len(image_data.shape) == 3:
            pil_image = Image.fromarray(image_data, mode='RGB')
        else:
            pil_image = Image.fromarray(image_data)
        
        # Create a copy to draw on
        image_copy = pil_image.copy()
        draw = ImageDraw.Draw(image_copy)
        
        # Draw the rectangle
        draw.rectangle([x, y, x+width, y+height], outline=color, width=thickness)
        
        # Convert back to numpy array
        return np.array(image_copy)
    
    def draw_circle(self, image_data, x, y, radius, color=(0, 255, 0), thickness=1):
        """
        Draw a circle on the image.
        
        Args:
            image_data: Image data to draw on
            x, y: Center of the circle
            radius: Radius of the circle
            color: RGB color as (R, G, B)
            thickness: Line thickness
            
        Returns:
            Image with circle drawn
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Convert to PIL if needed
        if not PIL_AVAILABLE and len(image_data.shape) == 3:
            pil_image = Image.fromarray(image_data, mode='RGB')
        else:
            pil_image = Image.fromarray(image_data)
        
        # Create a copy to draw on
        image_copy = pil_image.copy()
        draw = ImageDraw.Draw(image_copy)
        
        # Draw the circle
        draw.ellipse([x-radius, y-radius, x+radius, y+radius], outline=color, width=thickness)
        
        # Convert back to numpy array
        return np.array(image_copy)
    
    def draw_ellipse(self, image_data, x, y, width, height, color=(0, 255, 0), thickness=1):
        """
        Draw an ellipse on the image.
        
        Args:
            image_data: Image data to draw on
            x, y: Center of the ellipse
            width, height: Width and height of the ellipse
            color: RGB color as (R, G, B)
            thickness: Line thickness
            
        Returns:
            Image with ellipse drawn
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Convert to PIL if needed
        if not PIL_AVAILABLE and len(image_data.shape) == 3:
            pil_image = Image.fromarray(image_data, mode='RGB')
        else:
            pil_image = Image.fromarray(image_data)
        
        # Create a copy to draw on
        image_copy = pil_image.copy()
        draw = ImageDraw.Draw(image_copy)
        
        # Draw the ellipse
        draw.ellipse([x-width//2, y-height//2, x+width//2, y+height//2, width, height], outline=color, width=thickness)
        
        # Convert back to numpy array
        return np.array(image_copy)
    
    def draw_text(self, image_data, text, x, y, font_size=20, color=(0, 255, 0)):
        """
        Draw text on the image.
        
        Args:
            image_data: Image data to draw on
            text: Text to draw
            x, y: Top-left corner of the text
            font_size: Font size
            color: RGB color as (R, G, B)
            
        Returns:
            Image with text drawn
        """
        if not NUMPY_AVAILABLE:
            raise ImportError("NumPy is not available. Install with: pip install numpy")
        
        # Convert to PIL if needed
        if not PIL_AVAILABLE and len(image_data.shape) == 3:
            pil_image = Image.fromarray(image_data, mode='RGB')
        else:
            pil_image = Image.fromarray(image_data)
        
        # Create a copy to draw on
        image_copy = pil_image.copy()
        draw = ImageDraw.Draw(image_copy)
        
        # Load a default font or create a simple one
        try:
            font = ImageFont.load_default()
        except:
            font = ImageFont.load_default()
        
        # Draw the text
        draw.text((x, y), text, fill=color, font=font)
        
        # Convert back to numpy array
        return np.array(image_copy)


def create_sample_data():
    """Create sample data for testing the vision library."""
    
    # Check if required libraries are available
    print("Computer Vision Library")
    print("=======================")
    print("Checking dependencies:")
    print(f"NumPy available: {'Yes' if NUMPY_AVAILABLE else 'No'}")
    print(f"PIL (Pillow) available: {'Yes' if PIL_AVAILABLE else 'No'}")
    print(f"scikit-learn available: {'Yes' if 'sklearn' in globals() else 'No'}")
    
    if not NUMPY_AVAILABLE or not PIL_AVAILABLE:
        print("This library requires NumPy and PIL.")
        print("Install with: pip install numpy Pillow")
        return
    
    # Create sample data
    image_processor = ImageProcessor()
    edge_detector = EdgeDetector()
    feature_extractor = FeatureExtractor()
    face_detector = FaceDetector()
    image_segmenter = ImageSegmenter()
    ocr_processor = OCRProcessor()
    image_visualizer = ImageVisualizer()
    
    # Generate a sample image for testing
    try:
        # Create a simple test image with some shapes
        image_data = np.zeros((200, 200), dtype=np.uint8)
        
        # Add a rectangle
        image_data[50:100, 150:200] = 255
        
        # Add a circle
        center_x, center_y = 100, 100
        radius = 30
        for i in range(center_x - radius, center_x + radius + 1):
            for j in range(center_y - radius, center_y + radius + 1):
                if (i - center_x)**2 + (j - center_y)**2 <= radius**2:
                    image_data[j, i] = 255
        
        # Add some random noise
        noise = np.random.randint(0, 50, image_data.shape)
        image_data += noise
        
        print("Created sample image for testing")
        
        # Test image processing operations
        print("\n1. Testing Image Processing Operations")
        
        # Load image
        image_processor.load_image_data(image_data)
        
        # Resize
        image_processor.resize(100, 100)
        print(f"Resized image: {image_processor.get_info()}")
        
        # Rotate
        image_processor.rotate(45)
        print(f"Rotated image: {image_processor.get_info()}")
        
        # Flip
        image_processor.flip('vertical')
        print(f"Flipped image: {image_processor.get_info()}")
        
        # Brightness and contrast adjustment
        image_processor.adjust_brightness(1.5)
        print(f"Brightness adjusted image: {image_processor.get_info()}")
        image_processor.adjust_contrast(1.5)
        print(f"Contrast adjusted image: {image_processor.get_info()}")
        
        # Save the processed image
        output_dir = "vision_outputs"
        os.makedirs(output_dir, exist_ok=True)
        
        image_processor.save_image(os.path.join(output_dir, "processed_image.png"))
        print("Saved processed image")
        
        # Test edge detection
        print("\n2. Testing Edge Detection")
        
        # Sobel edge detection
        sobel_edges = edge_detector.detect_edges_sobel(image_processor.image_data)
        if sobel_edges is not None:
            edge_processor = ImageProcessor()
            edge_processor.load_image_data(sobel_edges)
            edge_processor.save_image(os.path.join(output_dir, "sobel_edges.png"))
            print("Saved Sobel edges")
        
        # Canny edge detection
        canny_edges = edge_detector.detect_edges_canny(image_processor.image_data)
        if canny_edges is not None:
            edge_processor = ImageProcessor()
            edge_processor.load_image_data(canny_edges)
            edge_processor.save_image(os.path.join(output_dir, "canny_edges.png"))
            print("Saved Canny edges")
        
        # Test feature extraction
        print("\n3. Testing Feature Extraction")
        
        # Harris corner detection
        if image_processor.image_data is not None:
            corners = feature_extractor.extract_harris_corners(image_processor.image_data)
            print(f"Found {len(corners)} Harris corners")
            
            # Draw corners on the image
            if corners:
                marked_image = image_processor.image_data.copy() if NUMPY_AVAILABLE else None
                if NUMPY_AVAILABLE:
                    for corner in corners:
                        marked_image = image_visualizer.draw_rectangle(
                            marked_image, corner['x'], corner['y'], 
                            corner['width'], corner['height'], 
                            (0, 255, 0), 2
                        )
                
                marked_processor = ImageProcessor()
                marked_processor.load_image_data(marked_image)
                marked_processor.save_image(os.path.join(output_dir, "harris_corners.png"))
                print("Saved Harris corners")
        
        # SIFT-like feature extraction
        if image_processor.image_data is not None:
            features = feature_extractor.extract_sift_features(image_processor.image_data)
            print(f"Found {len(features)} SIFT-like features")
        
        # Test face detection
        print("\n4. Testing Face Detection")
        
        if image_processor.image_data is not None:
            faces = face_detector.detect_faces(image_processor.image_data)
            print(f"Found {len(faces)} faces")
            
            # Draw face rectangles
            if faces:
                marked_image = image_processor.image_data.copy() if NUMPY_AVAILABLE else None
                if NUMPY_AVAILABLE:
                    for face in faces:
                        marked_image = image_visualizer.draw_rectangle(
                            marked_image, face['x'], face['y'], 
                            face['width'], face['height'], 
                            (255, 0, 0), 2
                        )
                
                marked_processor = ImageProcessor()
                marked_processor.load_image_data(marked_image)
                marked_processor.save_image(os.path.join(output_dir, "detected_faces.png"))
                print("Saved detected faces")
        
        # Test eye detection on the first face
        if faces and len(faces) > 0:
            face = faces[0]
            eyes = face_detector.detect_eyes(face.get('region'))
            print(f"Found {len(eyes)} eyes")
        
            # Draw eye circles
            if eyes and NUMPY_AVAILABLE:
                marked_image = image_processor.image_data.copy()
                for eye in eyes:
                    marked_image = image_visualizer.draw_circle(
                        marked_image, eye['x'], eye['y'], 
                        eye['radius'], (255, 0, 0), 2
                        )
                
                marked_processor = ImageProcessor()
                marked_processor.load_image_data(marked_image)
                marked_processor.save_image(os.path.join(output_dir, "detected_eyes.png"))
                print("Saved detected eyes")
        
        # Test image segmentation
        print("\n5. Testing Image Segmentation")
        
        if image_processor.image_data is not None:
            segmented, centers = image_segmenter.segment_kmeans(image_processor.image_data)
            if segmented is not None:
                segmented_processor = ImageProcessor()
                segmented_processor.load_image_data(segmented)
                segmented_processor.save_image(os.path.join(output_dir, "segmented_kmeans.png"))
                print(f"Saved K-means segmentation with {len(centers)} clusters")
        
            # Threshold segmentation
            thresholded, _ = image_segmenter.threshold_segmentation(image_processor.image_data)
            if thresholded is not None:
                thresholded_processor = ImageProcessor()
                thresholded_processor.load_image_data(thresholded)
                thresholded_processor.save_image(os.path.join(output_dir, "thresholded_segmentation.png"))
                print("Saved thresholded segmentation")
        
        # Region growing segmentation
            region_grown = image_segmenter.region_growing(image_processor.image_data)
            if region_grown is not None:
                region_processor = ImageProcessor()
                region_processor.load_image_data(region_grown)
                region_processor.save_image(os.path.join(output_dir, "region_growing_segmentation.png"))
                print("Saved region growing segmentation")
        
        # Test OCR
        print("\n6. Testing OCR")
        
        if image_processor.image_data is not None:
            # Create a simple text image
            text = "HELLO"
            font = ImageFont.load_default()
            text_image = Image.new('RGB', (200, 100), color=(255, 255, 255))
            draw = ImageDraw.Draw(text_image)
            
            # Calculate text size and position
            bbox = draw.textbbox(text, font=font)
            text_width = bbox[2] - bbox[0]
            text_height = bbox[3] - bbox[1]
            
            # Center the text
            x = (text_image.width - text_width) // 2
            y = (text_image.height - text_height) // 2
            
            draw.text((x, y), text, fill=(0, 0, 0), font=font)
            
            # Create a new image with the text
            text_processor = ImageProcessor()
            text_processor.load_image_data(text_image)
            text_processor.save_image(os.path.join(output_dir, "text_image.png"))
            print("Created text image for OCR")
            
            # Recognize characters
            chars = ocr_processor.recognize_characters(text_processor.image_data)
            recognized_text = ocr_processor.extract_text_from_boxes(chars)
            print(f"Recognized text: {recognized_text}")
        
        print("\nVision library demonstration completed successfully!")
        print(f"Output directory: {output_dir}")
        
    except Exception as e:
        print(f"Error during demonstration: {str(e)}")


def main():
    """Main function to demonstrate the computer vision library."""
    print("Computer Vision and Image Processing Library")
    print("=======================================")
    
    # Run the demonstration
    create_sample_data()
    
    return True


if __name__ == "__main__":
    main()
<｜fim▁hole｜>