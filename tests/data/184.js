/**
 * RESTful API Client
 * 
 * This module provides a comprehensive RESTful API client
 * that can handle authentication, requests, responses, and error handling.
 * 
 * Features:
 * - HTTP methods (GET, POST, PUT, DELETE)
 * - Request and response interceptors
 * - Authentication and authorization
 * - Error handling and retry logic
 * - File upload support
 * - Request caching
 * - Response parsing
 */

/**
 * HTTP Client class for making RESTful API requests
 */
class HttpClient {
    constructor(baseUrl, options = {}) {
        this.baseUrl = baseUrl.replace(/\/$/, ''); // Remove trailing slash
        this.defaultHeaders = options.defaultHeaders || {};
        this.timeout = options.timeout || 30000; // Default timeout: 30s
        this.retryCount = options.retryCount || 3;
        this.retryDelay = options.retryDelay || 1000; // Default retry delay: 1s
        this.interceptors = {
            request: [],
            response: []
        };
        this.cache = new Map();
        this.auth = null;
        this.responseInterceptors = [];
        this.requestInterceptors = [];
    }
    
    /**
     * Set authentication token
     * @param {string} token - Authentication token
     */
    setAuthToken(token) {
        this.auth = {
            type: 'Bearer',
            token: token
        };
    }
    
    /**
     * Clear authentication token
     */
    clearAuthToken() {
        this.auth = null;
    }
    
    /**
     * Add request interceptor
     * @param {Function} interceptor - Request interceptor function
     */
    addRequestInterceptor(interceptor) {
        this.requestInterceptors.push(interceptor);
    }
    
    /**
     * Add response interceptor
     * @param {Function} interceptor - Response interceptor function
     */
    addResponseInterceptor(interceptor) {
        this.responseInterceptors.push(interceptor);
    }
    
    /**
     * Get headers for request
     * @param {Object} customHeaders - Custom headers
     */
    getHeaders(customHeaders = {}) {
        const headers = { ...this.defaultHeaders, ...customHeaders };
        
        if (this.auth) {
            headers.Authorization = `${this.auth.type} ${this.auth.token}`;
        }
        
        return headers;
    }
    
    /**
     * Make HTTP request with specified method and options
     * @param {string} method - HTTP method
     * @param {string} endpoint - API endpoint
     * @param {Object} options - Request options
     */
    async request(method, endpoint, options = {}) {
        const url = `${this.baseUrl}/${endpoint}`;
        
        // Build request config
        const config = {
            method: method.toUpperCase(),
            headers: this.getHeaders(options.headers),
            ...options
        };
        
        // Apply request interceptors
        for (const interceptor of this.requestInterceptors) {
            const result = await interceptor(config);
            if (result === false) {
                throw new Error('Request blocked by interceptor');
            }
            Object.assign(config, result);
        }
        
        // Handle file upload
        if (options.data instanceof FormData) {
            config.body = options.data;
            delete config.headers['Content-Type']; // Let browser set it for FormData
        } else if (options.data) {
            config.body = JSON.stringify(options.data);
            config.headers['Content-Type'] = 'application/json';
        }
        
        // Check cache for GET requests
        if (method.toUpperCase() === 'GET' && options.cache !== false) {
            const cacheKey = this.getCacheKey(url, config);
            const cachedResponse = this.cache.get(cacheKey);
            
            if (cachedResponse && !this.isCacheExpired(cachedResponse)) {
                return Promise.resolve(cachedResponse.data);
            }
        }
        
        try {
            // Make request with timeout
            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), this.timeout);
            
            config.signal = controller.signal;
            
            const response = await fetch(url, config);
            clearTimeout(timeoutId);
            
            // Clone response for caching
            const responseClone = response.clone();
            const responseData = await this.parseResponse(responseClone);
            
            // Cache GET responses
            if (method.toUpperCase() === 'GET' && options.cache !== false) {
                const cacheKey = this.getCacheKey(url, config);
                this.cache.set(cacheKey, {
                    data: responseData,
                    timestamp: Date.now(),
                    expires: this.getCacheExpiration(options.cacheExpiry)
                });
            }
            
            // Apply response interceptors
            let modifiedResponse = {
                ...responseData,
                status: response.status,
                statusText: response.statusText,
                headers: response.headers
            };
            
            for (const interceptor of this.responseInterceptors) {
                const result = await interceptor(modifiedResponse, config);
                if (result) {
                    modifiedResponse = result;
                }
            }
            
            return modifiedResponse;
        } catch (error) {
            // Handle request failure with retry logic
            if (options.retry !== false && this.shouldRetry(error)) {
                return this.retryRequest(method, endpoint, {
                    ...options,
                    _retryCount: (options._retryCount || 0) + 1
                });
            }
            
            throw error;
        }
    }
    
    /**
     * Parse response based on content type
     * @param {Response} response - Fetch response
     */
    async parseResponse(response) {
        const contentType = response.headers.get('content-type') || '';
        
        if (contentType.includes('application/json')) {
            return await response.json();
        } else if (contentType.includes('text/')) {
            return await response.text();
        } else if (contentType.includes('image/') || contentType.includes('application/octet-stream')) {
            return await response.blob();
        }
        
        return await response.text();
    }
    
    /**
     * Get cache key for request
     * @param {string} url - Request URL
     * @param {Object} config - Request config
     */
    getCacheKey(url, config) {
        return `${url}:${JSON.stringify(config)}`;
    }
    
    /**
     * Check if cached response is expired
     * @param {Object} cachedResponse - Cached response
     */
    isCacheExpired(cachedResponse) {
        return Date.now() > cachedResponse.expires;
    }
    
    /**
     * Get cache expiration timestamp
     * @param {number} minutes - Minutes to expire cache
     */
    getCacheExpiration(minutes = 5) {
        return Date.now() + (minutes * 60 * 1000);
    }
    
    /**
     * Check if request should be retried
     * @param {Error} error - Request error
     */
    shouldRetry(error) {
        // Don't retry on 4xx status codes except 429 Too Many Requests
        if (error.status >= 400 && error.status < 500 && error.status !== 429) {
            return false;
        }
        
        // Don't retry on network errors
        if (!error.status) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Retry request with delay
     * @param {string} method - HTTP method
     * @param {string} endpoint - API endpoint
     * @param {Object} options - Request options
     */
    async retryRequest(method, endpoint, options) {
        if (options._retryCount >= this.retryCount) {
            throw new Error(`Request failed after ${options._retryCount} retries`);
        }
        
        // Wait before retry
        await new Promise(resolve => {
            setTimeout(resolve, this.retryDelay * options._retryCount);
        });
        
        return this.request(method, endpoint, options);
    }
    
    /**
     * Convenience method for GET requests
     * @param {string} endpoint - API endpoint
     * @param {Object} options - Request options
     */
    get(endpoint, options = {}) {
        return this.request('GET', endpoint, options);
    }
    
    /**
     * Convenience method for POST requests
     * @param {string} endpoint - API endpoint
     * @param {Object} options - Request options
     */
    post(endpoint, options = {}) {
        return this.request('POST', endpoint, options);
    }
    
    /**
     * Convenience method for PUT requests
     * @param {string} endpoint - API endpoint
     * @param {Object} options - Request options
     */
    put(endpoint, options = {}) {
        return this.request('PUT', endpoint, options);
    }
    
    /**
     * Convenience method for DELETE requests
     * @param {string} endpoint - API endpoint
     * @param {Object} options - Request options
     */
    delete(endpoint, options = {}) {
        return this.request('DELETE', endpoint, options);
    }
    
    /**
     * Convenience method for file upload
     * @param {string} endpoint - API endpoint
     * @param {FormData} formData - Form data with files
     * @param {Object} options - Request options
     */
    upload(endpoint, formData, options = {}) {
        return this.request('POST', endpoint, {
            ...options,
            data: formData,
            cache: false
        });
    }
}

/**
 * API Service class for specific API operations
 */
class ApiService {
    constructor(baseUrl) {
        this.client = new HttpClient(baseUrl, {
            timeout: 30000,
            defaultHeaders: {
                'Content-Type': 'application/json',
                'Accept': 'application/json'
            },
            retryCount: 3,
            retryDelay: 1000
        });
        
        this.setupInterceptors();
    }
    
    /**
     * Setup request and response interceptors
     */
    setupInterceptors() {
        // Add request interceptor for logging
        this.client.addRequestInterceptor(config => {
            console.log(`${config.method} ${config.url}`);
            return config;
        });
        
        // Add response interceptor for error handling
        this.client.addResponseInterceptor(response => {
            if (response.status >= 400) {
                console.error(`API Error: ${response.status} ${response.statusText}`);
                
                // Handle specific error cases
                if (response.status === 401) {
                    // Redirect to login
                    console.log('Authentication required, redirecting to login');
                } else if (response.status === 403) {
                    // Handle forbidden access
                    console.error('Access forbidden: insufficient permissions');
                }
            }
            
            return response;
        });
        
        // Add response interceptor for logging
        this.client.addResponseInterceptor(response => {
            console.log(`Response: ${response.status} for ${response.config.url}`);
            return response;
        });
    }
    
    /**
     * Authentication methods
     */
    
    /**
     * Login with email and password
     * @param {string} email - User email
     * @param {string} password - User password
     */
    async login(email, password) {
        const response = await this.client.post('auth/login', {
            data: {
                email,
                password
            },
            cache: false
        });
        
        if (response.status === 200 && response.data.token) {
            this.client.setAuthToken(response.data.token);
            localStorage.setItem('authToken', response.data.token);
            localStorage.setItem('user', JSON.stringify(response.data.user));
            return response.data;
        }
        
        throw new Error(`Login failed: ${response.statusText}`);
    }
    
    /**
     * Logout user
     */
    logout() {
        this.client.clearAuthToken();
        localStorage.removeItem('authToken');
        localStorage.removeItem('user');
    }
    
    /**
     * Get current user
     */
    getCurrentUser() {
        const userJson = localStorage.getItem('user');
        return userJson ? JSON.parse(userJson) : null;
    }
    
    /**
     * Check if user is authenticated
     */
    isAuthenticated() {
        return !!this.client.auth && !!localStorage.getItem('authToken');
    }
    
    /**
     * Refresh authentication token
     */
    async refreshToken() {
        const refreshToken = localStorage.getItem('refreshToken');
        if (!refreshToken) {
            throw new Error('No refresh token available');
        }
        
        const response = await this.client.post('auth/refresh', {
            data: {
                refreshToken
            },
            cache: false
        });
        
        if (response.status === 200 && response.data.token) {
            this.client.setAuthToken(response.data.token);
            localStorage.setItem('authToken', response.data.token);
            return response.data.token;
        }
        
        throw new Error('Token refresh failed');
    }
    
    /**
     * User resource methods
     */
    
    /**
     * Get current user profile
     */
    async getUserProfile() {
        const response = await this.client.get('users/profile');
        return response.data;
    }
    
    /**
     * Update user profile
     * @param {Object} profileData - Profile data to update
     */
    async updateUserProfile(profileData) {
        const response = await this.client.put('users/profile', {
            data: profileData
        });
        
        if (response.status === 200) {
            // Update local user data
            const user = this.getCurrentUser();
            if (user) {
                const updatedUser = { ...user, ...profileData };
                localStorage.setItem('user', JSON.stringify(updatedUser));
            }
        }
        
        return response.data;
    }
    
    /**
     * Change password
     * @param {string} currentPassword - Current password
     * @param {string} newPassword - New password
     */
    async changePassword(currentPassword, newPassword) {
        const response = await this.client.post('users/change-password', {
            data: {
                currentPassword,
                newPassword
            },
            cache: false
        });
        
        return response.data;
    }
    
    /**
     * Product resource methods
     */
    
    /**
     * Get all products
     * @param {Object} params - Query parameters
     */
    async getProducts(params = {}) {
        const response = await this.client.get('products', {
            params
        });
        
        return response.data;
    }
    
    /**
     * Get product by ID
     * @param {string} productId - Product ID
     */
    async getProduct(productId) {
        const response = await this.client.get(`products/${productId}`);
        
        return response.data;
    }
    
    /**
     * Create new product
     * @param {Object} productData - Product data
     */
    async createProduct(productData) {
        const response = await this.client.post('products', {
            data: productData
        });
        
        return response.data;
    }
    
    /**
     * Update existing product
     * @param {string} productId - Product ID
     * @param {Object} productData - Product data to update
     */
    async updateProduct(productId, productData) {
        const response = await this.client.put(`products/${productId}`, {
            data: productData
        });
        
        return response.data;
    }
    
    /**
     * Delete product
     * @param {string} productId - Product ID
     */
    async deleteProduct(productId) {
        const response = await this.client.delete(`products/${productId}`);
        
        return response.data;
    }
    
    /**
     * Order resource methods
     */
    
    /**
     * Get user orders
     * @param {Object} params - Query parameters
     */
    async getOrders(params = {}) {
        const response = await this.client.get('orders', {
            params
        });
        
        return response.data;
    }
    
    /**
     * Get order by ID
     * @param {string} orderId - Order ID
     */
    async getOrder(orderId) {
        const response = await this.client.get(`orders/${orderId}`);
        
        return response.data;
    }
    
    /**
     * Create new order
     * @param {Object} orderData - Order data
     */
    async createOrder(orderData) {
        const response = await this.client.post('orders', {
            data: orderData
        });
        
        return response.data;
    }
    
    /**
     * Update order status
     * @param {string} orderId - Order ID
     * @param {string} status - New status
     */
    async updateOrderStatus(orderId, status) {
        const response = await this.client.patch(`orders/${orderId}`, {
            data: { status }
        });
        
        return response.data;
    }
    
    /**
     * File upload methods
     */
    
    /**
     * Upload file
     * @param {string} endpoint - Upload endpoint
     * @param {File} file - File to upload
     * @param {Object} options - Upload options
     */
    async uploadFile(endpoint, file, options = {}) {
        const formData = new FormData();
        formData.append('file', file);
        
        if (options.fields) {
            Object.keys(options.fields).forEach(key => {
                formData.append(key, options.fields[key]);
            });
        }
        
        const response = await this.client.upload(endpoint, formData, options);
        
        return response.data;
    }
    
    /**
     * Upload multiple files
     * @param {string} endpoint - Upload endpoint
     * @param {File[]} files - Files to upload
     * @param {Object} options - Upload options
     */
    async uploadFiles(endpoint, files, options = {}) {
        const formData = new FormData();
        
        files.forEach((file, index) => {
            formData.append(`files[${index}]`, file);
        });
        
        if (options.fields) {
            Object.keys(options.fields).forEach(key => {
                formData.append(key, options.fields[key]);
            });
        }
        
        const response = await this.client.upload(endpoint, formData, options);
        
        return response.data;
    }
}

/**
 * API Builder class for building complex API requests
 */
class ApiBuilder {
    constructor(client) {
        this.client = client;
        this.request = {
            method: 'GET',
            endpoint: '',
            params: {},
            data: null,
            headers: {},
            files: null,
            cache: true,
            retry: true
        };
    }
    
    /**
     * Set HTTP method
     * @param {string} method - HTTP method
     */
    method(method) {
        this.request.method = method.toUpperCase();
        return this;
    }
    
    /**
     * Set endpoint
     * @param {string} endpoint - API endpoint
     */
    endpoint(endpoint) {
        this.request.endpoint = endpoint;
        return this;
    }
    
    /**
     * Set query parameters
     * @param {Object} params - Query parameters
     */
    params(params) {
        this.request.params = { ...this.request.params, ...params };
        return this;
    }
    
    /**
     * Set request body
     * @param {Object} data - Request body data
     */
    data(data) {
        this.request.data = data;
        return this;
    }
    
    /**
     * Set headers
     * @param {Object} headers - Request headers
     */
    headers(headers) {
        this.request.headers = { ...this.request.headers, ...headers };
        return this;
    }
    
    /**
     * Add file to request
     * @param {File} file - File to upload
     * @param {string} fieldName - Field name for the file
     */
    file(file, fieldName = 'file') {
        if (!this.request.files) {
            this.request.files = {};
        }
        
        this.request.files[fieldName] = file;
        return this;
    }
    
    /**
     * Add multiple files to request
     * @param {File[]} files - Files to upload
     * @param {string} fieldName - Field name for the files
     */
    files(files, fieldName = 'files') {
        if (!this.request.files) {
            this.request.files = {};
        }
        
        this.request.files[fieldName] = files;
        return this;
    }
    
    /**
     * Set cache option
     * @param {boolean} cache - Whether to cache response
     */
    cache(cache) {
        this.request.cache = cache;
        return this;
    }
    
    /**
     * Set retry option
     * @param {boolean} retry - Whether to retry on failure
     */
    retry(retry) {
        this.request.retry = retry;
        return this;
    }
    
    /**
     * Execute the built request
     */
    async execute() {
        const options = {
            params: this.request.params,
            data: this.request.data,
            headers: this.request.headers,
            cache: this.request.cache,
            retry: this.request.retry
        };
        
        if (this.request.files) {
            if (Object.keys(this.request.files).length === 1) {
                // Single file upload
                const fieldName = Object.keys(this.request.files)[0];
                return this.client.upload(this.request.endpoint, this.request.files[fieldName], options);
            } else {
                // Multiple files upload
                return this.client.uploadFiles(this.request.endpoint, this.request.files, options);
            }
        }
        
        // Regular request
        switch (this.request.method) {
            case 'GET':
                return this.client.get(this.request.endpoint, options);
            case 'POST':
                return this.client.post(this.request.endpoint, options);
            case 'PUT':
                return this.client.put(this.request.endpoint, options);
            case 'DELETE':
                return this.client.delete(this.request.endpoint, options);
            default:
                return this.client.request(this.request.method, this.request.endpoint, options);
        }
    }
}

/**
 * Main function to demonstrate the RESTful API client
 */
async function main() {
    // Create API service instance
    const apiService = new ApiService('https://api.example.com/v1');
    
    try {
        // Login user
        console.log('Logging in...');
        const loginResponse = await apiService.login('user@example.com', 'password123');
        console.log('Login successful:', loginResponse.user.fullName);
        
        // Get all products
        console.log('Fetching products...');
        const products = await apiService.getProducts({
            page: 1,
            limit: 10,
            category: 'electronics'
        });
        console.log('Products:', products.map(p => p.name));
        
        // Get product details
        if (products.length > 0) {
            console.log('Fetching product details...');
            const product = await apiService.getProduct(products[0].id);
            console.log('Product details:', product.name, '-', product.description);
            
            // Update product
            console.log('Updating product...');
            const updatedProduct = await apiService.updateProduct(products[0].id, {
                name: product.name,
                description: product.description + ' (Updated)',
                price: product.price * 1.1
            });
            console.log('Product updated:', updatedProduct.name);
        }
        
        // Get user orders
        console.log('Fetching user orders...');
        const orders = await apiService.getOrders({
            status: 'pending',
            limit: 5
        });
        console.log('Orders:', orders.map(o => `#${o.id}: ${o.total}`));
        
        // Create new order using builder
        console.log('Creating new order...');
        const orderBuilder = new ApiBuilder(apiService.client)
            .method('POST')
            .endpoint('orders')
            .data({
                items: [
                    {
                        productId: products.length > 0 ? products[0].id : 'unknown',
                        quantity: 2,
                        price: products.length > 0 ? products[0].price : 99.99
                    }
                ],
                shippingAddress: {
                    street: '123 Main St',
                    city: 'Anytown',
                    state: 'CA',
                    zip: '12345',
                    country: 'USA'
                },
                paymentMethod: 'credit_card'
            })
            .cache(false);
        
        const newOrder = await orderBuilder.execute();
        console.log('New order created:', `#${newOrder.id}`);
        
        // Get order details
        console.log('Fetching order details...');
        const orderDetails = await apiService.getOrder(newOrder.id);
        console.log('Order details:', orderDetails.status);
        
        // Update order status
        console.log('Updating order status...');
        const updatedOrder = await apiService.updateOrderStatus(newOrder.id, 'processing');
        console.log('Order updated:', updatedOrder.status);
        
        // File upload example
        console.log('Uploading file...');
        
        // Create a mock file for demonstration
        const content = 'This is a test file content.';
        const blob = new Blob([content], { type: 'text/plain' });
        const file = new File([blob], 'test.txt', { type: 'text/plain' });
        
        const fileResponse = await apiService.uploadFile('files/upload', file, {
            fields: {
                description: 'Test file upload'
            }
        });
        console.log('File uploaded:', fileResponse.filename);
        
        // Get user profile
        console.log('Fetching user profile...');
        const userProfile = await apiService.getUserProfile();
        console.log('User profile:', userProfile.email);
        
        // Update user profile
        console.log('Updating user profile...');
        const updatedProfile = await apiService.updateUserProfile({
            firstName: 'John',
            lastName: 'Doe',
            phone: '+1234567890'
        });
        console.log('Profile updated:', updatedProfile.firstName);
        
    } catch (error) {
        console.error('API Error:', error.message);
    } finally {
        // Logout
        console.log('Logging out...');
        apiService.logout();
        console.log('Logged out');
    }
}

// Run the main function
main();

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        HttpClient,
        ApiService,
        ApiBuilder
    };
}