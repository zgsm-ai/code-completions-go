/**
 * Advanced Data Visualization Dashboard
 * 
 * This module creates an interactive data visualization dashboard that can handle
 * various types of charts and real-time data updates. It provides a flexible framework
 * for visualizing complex datasets with interactive controls.
 * 
 * Features:
 * - Multiple chart types (bar, line, pie, scatter, heatmap)
 * - Real-time data updates
 * - Interactive filters and controls
 * - Responsive design
 * - Export functionality
 * - Customizable themes
 * - Data aggregation and analysis
 */

// ChartManager class manages all chart instances and configurations
class ChartManager {
    constructor(containerId, options = {}) {
        this.containerId = containerId;
        this.container = document.getElementById(containerId);
        this.charts = new Map();
        this.dataSources = new Map();
        this.filters = new Map();
        this.themes = this.initializeThemes();
        this.currentTheme = options.theme || 'default';
        this.options = {
            responsive: options.responsive !== false,
            animation: options.animation !== false,
            autoUpdate: options.autoUpdate !== false,
            updateInterval: options.updateInterval || 5000,
            ...options
        };
        
        this.init();
    }
    
    /**
     * Initialize chart manager
     */
    init() {
        if (!this.container) {
            throw new Error(`Container with ID '${this.containerId}' not found`);
        }
        
        // Create dashboard layout
        this.createLayout();
        
        // Apply theme
        this.applyTheme(this.currentTheme);
        
        // Start auto-update if enabled
        if (this.options.autoUpdate) {
            this.startAutoUpdate();
        }
    }
    
    /**
     * Create dashboard layout
     */
    createLayout() {
        // Clear container
        this.container.innerHTML = '';
        
        // Create header
        const header = document.createElement('div');
        header.className = 'dashboard-header';
        header.innerHTML = `
            <div class="dashboard-title">
                <h1>Advanced Data Visualization Dashboard</h1>
            </div>
            <div class="dashboard-controls">
                <div class="theme-selector">
                    <label for="theme-select">Theme:</label>
                    <select id="theme-select">
                        ${Object.keys(this.themes).map(theme => 
                            `<option value="${theme}" ${theme === this.currentTheme ? 'selected' : ''}>${theme}</option>`
                        ).join('')}
                    </select>
                </div>
                <div class="export-controls">
                    <button id="export-btn" class="btn btn-primary">Export Data</button>
                    <button id="reset-btn" class="btn btn-secondary">Reset Filters</button>
                </div>
            </div>
        `;
        
        // Create filters panel
        const filtersPanel = document.createElement('div');
        filtersPanel.className = 'dashboard-filters';
        filtersPanel.innerHTML = `
            <h3>Data Filters</h3>
            <div id="filters-container" class="filters-content"></div>
            <button id="apply-filters" class="btn btn-primary">Apply Filters</button>
        `;
        
        // Create charts container
        const chartsContainer = document.createElement('div');
        chartsContainer.className = 'dashboard-charts';
        chartsContainer.innerHTML = `
            <div class="charts-grid" id="charts-grid"></div>
        `;
        
        // Create status bar
        const statusBar = document.createElement('div');
        statusBar.className = 'dashboard-status';
        statusBar.innerHTML = `
            <div class="status-item">
                <span id="last-update">Last Update: Never</span>
            </div>
            <div class="status-item">
                <span id="data-count">Data Points: 0</span>
            </div>
            <div class="status-item">
                <span id="chart-count">Charts: 0</span>
            </div>
        `;
        
        // Append elements to container
        this.container.appendChild(header);
        this.container.appendChild(filtersPanel);
        this.container.appendChild(chartsContainer);
        this.container.appendChild(statusBar);
        
        // Add event listeners
        this.addEventListeners();
    }
    
    /**
     * Add event listeners
     */
    addEventListeners() {
        // Theme selector
        const themeSelect = document.getElementById('theme-select');
        if (themeSelect) {
            themeSelect.addEventListener('change', (e) => {
                this.applyTheme(e.target.value);
            });
        }
        
        // Export button
        const exportBtn = document.getElementById('export-btn');
        if (exportBtn) {
            exportBtn.addEventListener('click', () => {
                this.exportData();
            });
        }
        
        // Reset filters button
        const resetBtn = document.getElementById('reset-btn');
        if (resetBtn) {
            resetBtn.addEventListener('click', () => {
                this.resetFilters();
            });
        }
        
        // Apply filters button
        const applyFiltersBtn = document.getElementById('apply-filters');
        if (applyFiltersBtn) {
            applyFiltersBtn.addEventListener('click', () => {
                this.applyFilters();
            });
        }
    }
    
    /**
     * Initialize available themes
     */
    initializeThemes() {
        return {
            default: {
                name: 'Default',
                colors: ['#3498db', '#2ecc71', '#f39c12', '#e74c3c', '#9b59b6', '#1abc9c'],
                background: '#ffffff',
                foreground: '#2c3e50',
                grid: '#ecf0f1'
            },
            dark: {
                name: 'Dark',
                colors: ['#3498db', '#2ecc71', '#f39c12', '#e74c3c', '#9b59b6', '#1abc9c'],
                background: '#2c3e50',
                foreground: '#ecf0f1',
                grid: '#34495e'
            },
            colorful: {
                name: 'Colorful',
                colors: ['#FF6384', '#36A2EB', '#FFCE56', '#4BC0C0', '#9966FF', '#FF9F40'],
                background: '#f8f9fa',
                foreground: '#212529',
                grid: '#e9ecef'
            }
        };
    }
    
    /**
     * Apply theme to dashboard
     * @param {string} themeName - Name of the theme
     */
    applyTheme(themeName) {
        if (!this.themes[themeName]) {
            console.error(`Theme '${themeName}' not found`);
            return;
        }
        
        this.currentTheme = themeName;
        const theme = this.themes[themeName];
        
        // Update CSS variables
        document.documentElement.style.setProperty('--theme-background', theme.background);
        document.documentElement.style.setProperty('--theme-foreground', theme.foreground);
        document.documentElement.style.setProperty('--theme-grid', theme.grid);
        
        // Update theme colors for charts
        this.updateChartColors(theme.colors);
        
        // Save theme preference
        localStorage.setItem('dashboard-theme', themeName);
    }
    
    /**
     * Update chart colors
     * @param {Array} colors - Array of color values
     */
    updateChartColors(colors) {
        this.charts.forEach(chart => {
            if (chart.updateColors) {
                chart.updateColors(colors);
            }
        });
    }
    
    /**
     * Add data source
     * @param {string} id - Data source ID
     * @param {string} url - Data source URL
     * @param {Object} options - Data source options
     */
    addDataSource(id, url, options = {}) {
        this.dataSources.set(id, {
            url: url,
            options: options,
            data: null,
            lastUpdate: null,
            isLoading: false
        });
        
        // Load initial data
        this.loadDataSource(id);
    }
    
    /**
     * Load data from source
     * @param {string} id - Data source ID
     */
    async loadDataSource(id) {
        const source = this.dataSources.get(id);
        if (!source) {
            console.error(`Data source '${id}' not found`);
            return;
        }
        
        try {
            source.isLoading = true;
            
            const response = await fetch(source.url, source.options);
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            
            const data = await response.json();
            
            source.data = data;
            source.lastUpdate = new Date();
            
            // Update charts that use this data source
            this.updateChartsUsingDataSource(id);
            
            // Update status
            this.updateStatus();
            
        } catch (error) {
            console.error(`Error loading data source '${id}':`, error);
        } finally {
            source.isLoading = false;
        }
    }
    
    /**
     * Create chart
     * @param {string} id - Chart ID
     * @param {string} type - Chart type
     * @param {string} dataSourceId - Data source ID
     * @param {Object} config - Chart configuration
     */
    createChart(id, type, dataSourceId, config = {}) {
        const chartContainer = document.createElement('div');
        chartContainer.className = 'chart-container';
        chartContainer.id = `chart-${id}`;
        
        // Create chart header
        const chartHeader = document.createElement('div');
        chartHeader.className = 'chart-header';
        chartHeader.innerHTML = `
            <h3>${config.title || 'Chart'}</h3>
            <div class="chart-controls">
                <button class="chart-maximize" data-chart-id="${id}">⛶</button>
                <button class="chart-refresh" data-chart-id="${id}">↻</button>
                <button class="chart-close" data-chart-id="${id}">×</button>
            </div>
        `;
        
        // Create chart canvas
        const chartCanvas = document.createElement('canvas');
        chartCanvas.id = `canvas-${id}`;
        
        // Create chart
        chartContainer.appendChild(chartHeader);
        chartContainer.appendChild(chartCanvas);
        
        // Add to charts grid
        const chartsGrid = document.getElementById('charts-grid');
        if (chartsGrid) {
            chartsGrid.appendChild(chartContainer);
        }
        
        // Create chart instance
        let chart;
        switch (type.toLowerCase()) {
            case 'bar':
                chart = new BarChart(chartCanvas, dataSourceId, config);
                break;
            case 'line':
                chart = new LineChart(chartCanvas, dataSourceId, config);
                break;
            case 'pie':
                chart = new PieChart(chartCanvas, dataSourceId, config);
                break;
            case 'scatter':
                chart = new ScatterChart(chartCanvas, dataSourceId, config);
                break;
            case 'heatmap':
                chart = new HeatmapChart(chartCanvas, dataSourceId, config);
                break;
            default:
                chart = new BarChart(chartCanvas, dataSourceId, config);
        }
        
        // Apply current theme colors
        chart.updateColors(this.themes[this.currentTheme].colors);
        
        // Store chart
        this.charts.set(id, chart);
        
        // Add chart event listeners
        this.addChartEventListeners(chart);
        
        // Update chart count
        this.updateStatus();
        
        return chart;
    }
    
    /**
     * Add event listeners to chart
     * @param {Chart} chart - Chart instance
     */
    addChartEventListeners(chart) {
        const chartId = chart.id;
        
        // Maximize button
        const maximizeBtn = document.querySelector(`.chart-maximize[data-chart-id="${chartId}"]`);
        if (maximizeBtn) {
            maximizeBtn.addEventListener('click', () => {
                this.toggleChartMaximization(chartId);
            });
        }
        
        // Refresh button
        const refreshBtn = document.querySelector(`.chart-refresh[data-chart-id="${chartId}"]`);
        if (refreshBtn) {
            refreshBtn.addEventListener('click', () => {
                this.refreshChart(chartId);
            });
        }
        
        // Close button
        const closeBtn = document.querySelector(`.chart-close[data-chart-id="${chartId}"]`);
        if (closeBtn) {
            closeBtn.addEventListener('click', () => {
                this.removeChart(chartId);
            });
        }
    }
    
    /**
     * Toggle chart maximization
     * @param {string} chartId - Chart ID
     */
    toggleChartMaximization(chartId) {
        const chartContainer = document.getElementById(`chart-${chartId}`);
        if (chartContainer) {
            chartContainer.classList.toggle('chart-maximized');
            
            // Resize chart after animation
            setTimeout(() => {
                const chart = this.charts.get(chartId);
                if (chart) {
                    chart.resize();
                }
            }, 300);
        }
    }
    
    /**
     * Refresh chart
     * @param {string} chartId - Chart ID
     */
    refreshChart(chartId) {
        const chart = this.charts.get(chartId);
        if (chart) {
            // Reload data source
            this.loadDataSource(chart.dataSourceId);
        }
    }
    
    /**
     * Remove chart
     * @param {string} chartId - Chart ID
     */
    removeChart(chartId) {
        const chartContainer = document.getElementById(`chart-${chartId}`);
        if (chartContainer) {
            chartContainer.remove();
            this.charts.delete(chartId);
            this.updateStatus();
        }
    }
    
    /**
     * Update charts that use a specific data source
     * @param {string} dataSourceId - Data source ID
     */
    updateChartsUsingDataSource(dataSourceId) {
        this.charts.forEach(chart => {
            if (chart.dataSourceId === dataSourceId) {
                chart.update();
            }
        });
    }
    
    /**
     * Add filter
     * @param {string} id - Filter ID
     * @param {string} name - Filter name
     * @param {string} type - Filter type
     * @param {Object} options - Filter options
     */
    addFilter(id, name, type, options = {}) {
        const filtersContainer = document.getElementById('filters-container');
        if (!filtersContainer) return;
        
        const filterContainer = document.createElement('div');
        filterContainer.className = 'filter-container';
        filterContainer.id = `filter-${id}`;
        
        let filterInput = '';
        
        switch (type.toLowerCase()) {
            case 'text':
                filterInput = `<input type="text" id="filter-input-${id}" placeholder="${options.placeholder || 'Enter value'}">`;
                break;
            case 'select':
                const optionsHtml = options.values.map(value => 
                    `<option value="${value}">${value}</option>`
                ).join('');
                filterInput = `<select id="filter-input-${id}">${optionsHtml}</select>`;
                break;
            case 'date':
                filterInput = `<input type="date" id="filter-input-${id}">`;
                break;
            case 'daterange':
                filterInput = `
                    <div class="date-range">
                        <input type="date" id="filter-input-start-${id}" placeholder="Start date">
                        <input type="date" id="filter-input-end-${id}" placeholder="End date">
                    </div>
                `;
                break;
            case 'number':
                filterInput = `<input type="number" id="filter-input-${id}" placeholder="${options.placeholder || 'Enter number'}" ${options.min !== undefined ? `min="${options.min}"` : ''} ${options.max !== undefined ? `max="${options.max}"` : ''}>`;
                break;
            case 'checkbox':
                filterInput = `<input type="checkbox" id="filter-input-${id}">`;
                break;
        }
        
        filterContainer.innerHTML = `
            <label for="filter-input-${id}">${name}:</label>
            ${filterInput}
        `;
        
        filtersContainer.appendChild(filterContainer);
        
        // Store filter configuration
        this.filters.set(id, {
            name: name,
            type: type,
            options: options,
            getValue: () => this.getFilterValue(id)
        });
    }
    
    /**
     * Get filter value
     * @param {string} filterId - Filter ID
     */
    getFilterValue(filterId) {
        const filter = this.filters.get(filterId);
        if (!filter) return null;
        
        const type = filter.type;
        
        switch (type.toLowerCase()) {
            case 'text':
            case 'select':
            case 'date':
            case 'number':
                const input = document.getElementById(`filter-input-${filterId}`);
                return input ? input.value : null;
            case 'daterange':
                const startInput = document.getElementById(`filter-input-start-${filterId}`);
                const endInput = document.getElementById(`filter-input-end-${filterId}`);
                return {
                    start: startInput ? startInput.value : null,
                    end: endInput ? endInput.value : null
                };
            case 'checkbox':
                const checkbox = document.getElementById(`filter-input-${filterId}`);
                return checkbox ? checkbox.checked : false;
        }
        
        return null;
    }
    
    /**
     * Apply filters to all charts
     */
    applyFilters() {
        const activeFilters = {};
        
        this.filters.forEach((filter, id) => {
            const value = filter.getValue();
            if (value !== null && value !== '' && value !== false) {
                activeFilters[id] = {
                    name: filter.name,
                    type: filter.type,
                    value: value
                };
            }
        });
        
        // Apply filters to all charts
        this.charts.forEach(chart => {
            if (chart.applyFilters) {
                chart.applyFilters(activeFilters);
            }
        });
        
        // Update last update time
        this.updateLastUpdateTime();
    }
    
    /**
     * Reset all filters
     */
    resetFilters() {
        this.filters.forEach((filter, id) => {
            const type = filter.type;
            
            switch (type.toLowerCase()) {
                case 'text':
                case 'select':
                case 'date':
                case 'number':
                    const input = document.getElementById(`filter-input-${id}`);
                    if (input) input.value = '';
                    break;
                case 'daterange':
                    const startInput = document.getElementById(`filter-input-start-${id}`);
                    const endInput = document.getElementById(`filter-input-end-${id}`);
                    if (startInput) startInput.value = '';
                    if (endInput) endInput.value = '';
                    break;
                case 'checkbox':
                    const checkbox = document.getElementById(`filter-input-${id}`);
                    if (checkbox) checkbox.checked = false;
                    break;
            }
        });
        
        // Apply empty filters to reset charts
        this.applyFilters();
    }
    
    /**
     * Export data from dashboard
     */
    exportData() {
        const exportData = {
            charts: [],
            filters: [],
            metadata: {
                exportTime: new Date().toISOString(),
                theme: this.currentTheme,
                totalCharts: this.charts.size
            }
        };
        
        // Collect chart data
        this.charts.forEach((chart, id) => {
            exportData.charts.push({
                id: id,
                type: chart.type,
                title: chart.config.title || 'Chart',
                data: chart.getExportData()
            });
        });
        
        // Collect active filters
        this.filters.forEach((filter, id) => {
            const value = filter.getValue();
            if (value !== null && value !== '' && value !== false) {
                exportData.filters.push({
                    id: id,
                    name: filter.name,
                    type: filter.type,
                    value: value
                });
            }
        });
        
        // Create download link
        const dataStr = JSON.stringify(exportData, null, 2);
        const dataUri = 'data:application/json;charset=utf-8,'+ encodeURIComponent(dataStr);
        
        const exportFileDefaultName = `dashboard-export-${new Date().toISOString().slice(0, 10)}.json`;
        
        const linkElement = document.createElement('a');
        linkElement.setAttribute('href', dataUri);
        linkElement.setAttribute('download', exportFileDefaultName);
        linkElement.click();
    }
    
    /**
     * Start auto update
     */
    startAutoUpdate() {
        if (this.updateInterval) {
            clearInterval(this.updateInterval);
        }
        
        this.updateInterval = setInterval(() => {
            this.updateAllDataSources();
        }, this.options.updateInterval);
    }
    
    /**
     * Stop auto update
     */
    stopAutoUpdate() {
        if (this.updateInterval) {
            clearInterval(this.updateInterval);
            this.updateInterval = null;
        }
    }
    
    /**
     * Update all data sources
     */
    updateAllDataSources() {
        this.dataSources.forEach((source, id) => {
            this.loadDataSource(id);
        });
    }
    
    /**
     * Update status bar
     */
    updateStatus() {
        // Update last update time
        this.updateLastUpdateTime();
        
        // Update data count
        let totalDataPoints = 0;
        this.dataSources.forEach(source => {
            if (source.data && Array.isArray(source.data)) {
                totalDataPoints += source.data.length;
            }
        });
        
        const dataCountElement = document.getElementById('data-count');
        if (dataCountElement) {
            dataCountElement.textContent = `Data Points: ${totalDataPoints}`;
        }
        
        // Update chart count
        const chartCountElement = document.getElementById('chart-count');
        if (chartCountElement) {
            chartCountElement.textContent = `Charts: ${this.charts.size}`;
        }
    }
    
    /**
     * Update last update time
     */
    updateLastUpdateTime() {
        const lastUpdateElement = document.getElementById('last-update');
        if (lastUpdateElement) {
            lastUpdateElement.textContent = `Last Update: ${new Date().toLocaleTimeString()}`;
        }
    }
    
    /**
     * Destroy dashboard
     */
    destroy() {
        // Stop auto update
        this.stopAutoUpdate();
        
        // Clear all charts
        this.charts.forEach(chart => {
            if (chart.destroy) {
                chart.destroy();
            }
        });
        
        // Clear container
        if (this.container) {
            this.container.innerHTML = '';
        }
        
        // Clear references
        this.charts.clear();
        this.dataSources.clear();
        this.filters.clear();
    }
}

// Base Chart class
class BaseChart {
    constructor(canvas, dataSourceId, config = {}) {
        this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.dataSourceId = dataSourceId;
        this.id = config.id || 'chart-' + Date.now();
        this.type = config.type || 'base';
        this.config = config;
        this.colors = ['#3498db', '#2ecc71', '#f39c12', '#e74c3c', '#9b59b6', '#1abc9c'];
        this.data = null;
        this.filteredData = null;
        this.canvasWidth = canvas.width || 300;
        this.canvasHeight = canvas.height || 200;
        
        this.init();
    }
    
    /**
     * Initialize chart
     */
    init() {
        // Set canvas dimensions
        this.canvas.width = this.canvasWidth;
        this.canvas.height = this.canvasHeight;
        
        // Add event listeners
        this.addEventListeners();
        
        // Load initial data
        this.loadInitialData();
    }
    
    /**
     * Add event listeners to canvas
     */
    addEventListeners() {
        this.canvas.addEventListener('mousemove', (e) => {
            this.handleMouseMove(e);
        });
        
        this.canvas.addEventListener('click', (e) => {
            this.handleClick(e);
        });
        
        this.canvas.addEventListener('mouseout', (e) => {
            this.handleMouseOut(e);
        });
    }
    
    /**
     * Load initial data
     */
    async loadInitialData() {
        // This would be implemented in a real application
        // For now, we'll create sample data
        this.data = this.generateSampleData();
        this.filteredData = this.data;
        this.render();
    }
    
    /**
     * Generate sample data
     */
    generateSampleData() {
        return [];
    }
    
    /**
     * Update chart with new data
     */
    update() {
        // This would load data from the data source
        // For now, we'll just re-render with existing data
        this.render();
    }
    
    /**
     * Apply filters to chart data
     * @param {Object} filters - Active filters
     */
    applyFilters(filters) {
        if (!this.data) return;
        
        this.filteredData = this.data.filter(item => {
            // This would implement actual filtering logic
            // For now, we'll just return all data
            return true;
        });
        
        this.render();
    }
    
    /**
     * Update chart colors
     * @param {Array} colors - New color palette
     */
    updateColors(colors) {
        this.colors = colors;
        this.render();
    }
    
    /**
     * Handle mouse move events
     * @param {MouseEvent} event - Mouse event
     */
    handleMouseMove(event) {
        // To be implemented in child classes
    }
    
    /**
     * Handle click events
     * @param {MouseEvent} event - Click event
     */
    handleClick(event) {
        // To be implemented in child classes
    }
    
    /**
     * Handle mouse out events
     * @param {MouseEvent} event - Mouse out event
     */
    handleMouseOut(event) {
        // To be implemented in child classes
    }
    
    /**
     * Render chart
     */
    render() {
        // Clear canvas
        this.ctx.clearRect(0, 0, this.canvasWidth, this.canvasHeight);
        
        // To be implemented in child classes
    }
    
    /**
     * Resize chart
     */
    resize() {
        // Update canvas dimensions
        this.canvasWidth = this.canvas.width;
        this.canvasHeight = this.canvas.height;
        
        // Re-render chart
        this.render();
    }
    
    /**
     * Get export data
     */
    getExportData() {
        return this.filteredData || this.data || [];
    }
    
    /**
     * Destroy chart
     */
    destroy() {
        // Clean up resources
        // To be implemented in child classes if needed
    }
}

// Bar Chart class
class BarChart extends BaseChart {
    constructor(canvas, dataSourceId, config = {}) {
        super(canvas, dataSourceId, { ...config, type: 'bar' });
    }
    
    /**
     * Generate sample data
     */
    generateSampleData() {
        const labels = ['January', 'February', 'March', 'April', 'May', 'June'];
        return labels.map(label => ({
            label: label,
            value: Math.floor(Math.random() * 100) + 10
        }));
    }
    
    /**
     * Render bar chart
     */
    render() {
        super.render();
        
        if (!this.filteredData || this.filteredData.length === 0) return;
        
        const padding = 40;
        const chartWidth = this.canvasWidth - padding * 2;
        const chartHeight = this.canvasHeight - padding * 2;
        
        // Find max value for scaling
        const maxValue = Math.max(...this.filteredData.map(item => item.value));
        
        // Draw bars
        const barWidth = chartWidth / this.filteredData.length * 0.8;
        const barSpacing = chartWidth / this.filteredData.length * 0.2;
        
        this.filteredData.forEach((item, index) => {
            const barHeight = (item.value / maxValue) * chartHeight;
            const x = padding + index * (barWidth + barSpacing) + barSpacing / 2;
            const y = this.canvasHeight - padding - barHeight;
            
            // Draw bar
            this.ctx.fillStyle = this.colors[index % this.colors.length];
            this.ctx.fillRect(x, y, barWidth, barHeight);
            
            // Draw label
            this.ctx.fillStyle = '#333';
            this.ctx.font = '12px Arial';
            this.ctx.textAlign = 'center';
            this.ctx.fillText(item.label, x + barWidth / 2, this.canvasHeight - padding + 20);
            
            // Draw value
            this.ctx.fillText(item.value.toString(), x + barWidth / 2, y - 5);
        });
        
        // Draw axes
        this.ctx.strokeStyle = '#333';
        this.ctx.lineWidth = 1;
        this.ctx.beginPath();
        this.ctx.moveTo(padding, padding);
        this.ctx.lineTo(padding, this.canvasHeight - padding);
        this.ctx.lineTo(this.canvasWidth - padding, this.canvasHeight - padding);
        this.ctx.stroke();
    }
}

// Line Chart class
class LineChart extends BaseChart {
    constructor(canvas, dataSourceId, config = {}) {
        super(canvas, dataSourceId, { ...config, type: 'line' });
    }
    
    /**
     * Generate sample data
     */
    generateSampleData() {
        const labels = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun'];
        return labels.map(label => ({
            label: label,
            value: Math.floor(Math.random() * 100) + 10
        }));
    }
    
    /**
     * Render line chart
     */
    render() {
        super.render();
        
        if (!this.filteredData || this.filteredData.length === 0) return;
        
        const padding = 40;
        const chartWidth = this.canvasWidth - padding * 2;
        const chartHeight = this.canvasHeight - padding * 2;
        
        // Find max value for scaling
        const maxValue = Math.max(...this.filteredData.map(item => item.value));
        
        // Draw line
        this.ctx.strokeStyle = this.colors[0];
        this.ctx.lineWidth = 2;
        this.ctx.beginPath();
        
        this.filteredData.forEach((item, index) => {
            const x = padding + (index / (this.filteredData.length - 1)) * chartWidth;
            const y = this.canvasHeight - padding - (item.value / maxValue) * chartHeight;
            
            if (index === 0) {
                this.ctx.moveTo(x, y);
            } else {
                this.ctx.lineTo(x, y);
            }
        });
        
        this.ctx.stroke();
        
        // Draw data points
        this.filteredData.forEach((item, index) => {
            const x = padding + (index / (this.filteredData.length - 1)) * chartWidth;
            const y = this.canvasHeight - padding - (item.value / maxValue) * chartHeight;
            
            this.ctx.fillStyle = this.colors[0];
            this.ctx.beginPath();
            this.ctx.arc(x, y, 4, 0, Math.PI * 2);
            this.ctx.fill();
            
            // Draw label
            this.ctx.fillStyle = '#333';
            this.ctx.font = '12px Arial';
            this.ctx.textAlign = 'center';
            this.ctx.fillText(item.label, x, this.canvasHeight - padding + 20);
            
            // Draw value
            this.ctx.fillText(item.value.toString(), x, y - 10);
        });
        
        // Draw axes
        this.ctx.strokeStyle = '#333';
        this.ctx.lineWidth = 1;
        this.ctx.beginPath();
        this.ctx.moveTo(padding, padding);
        this.ctx.lineTo(padding, this.canvasHeight - padding);
        this.ctx.lineTo(this.canvasWidth - padding, this.canvasHeight - padding);
        this.ctx.stroke();
    }
}

// Pie Chart class
class PieChart extends BaseChart {
    constructor(canvas, dataSourceId, config = {}) {
        super(canvas, dataSourceId, { ...config, type: 'pie' });
    }
    
    /**
     * Generate sample data
     */
    generateSampleData() {
        return [
            { label: 'Category A', value: 30 },
            { label: 'Category B', value: 20 },
            { label: 'Category C', value: 25 },
            { label: 'Category D', value: 15 },
            { label: 'Category E', value: 10 }
        ];
    }
    
    /**
     * Render pie chart
     */
    render() {
        super.render();
        
        if (!this.filteredData || this.filteredData.length === 0) return;
        
        const centerX = this.canvasWidth / 2;
        const centerY = this.canvasHeight / 2;
        const radius = Math.min(this.canvasWidth, this.canvasHeight) / 2 - 40;
        
        // Calculate total value
        const totalValue = this.filteredData.reduce((sum, item) => sum + item.value, 0);
        
        // Draw pie slices
        let startAngle = 0;
        
        this.filteredData.forEach((item, index) => {
            const sliceAngle = (item.value / totalValue) * Math.PI * 2;
            
            // Draw slice
            this.ctx.fillStyle = this.colors[index % this.colors.length];
            this.ctx.beginPath();
            this.ctx.moveTo(centerX, centerY);
            this.ctx.arc(centerX, centerY, radius, startAngle, startAngle + sliceAngle);
            this.ctx.closePath();
            this.ctx.fill();
            
            // Draw label
            const labelAngle = startAngle + sliceAngle / 2;
            const labelX = centerX + Math.cos(labelAngle) * (radius * 0.7);
            const labelY = centerY + Math.sin(labelAngle) * (radius * 0.7);
            
            this.ctx.fillStyle = '#fff';
            this.ctx.font = '12px Arial';
            this.ctx.textAlign = 'center';
            this.ctx.fillText(`${item.label}: ${item.value}`, labelX, labelY);
            
            startAngle += sliceAngle;
        });
    }
}

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        ChartManager,
        BaseChart,
        BarChart,
        LineChart,
        PieChart
    };
}