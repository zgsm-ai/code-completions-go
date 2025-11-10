/**
 * Real-Time Weather Monitoring System
 * 
 * This module implements a comprehensive weather monitoring system that can track
 * weather conditions from multiple sources, provide forecasts, and alert users
 * about severe weather conditions.
 * 
 * Features:
 * - Multi-source weather data aggregation
 * - Real-time weather monitoring
 * - Weather forecasting
 * - Severe weather alerts
 * - Historical data analysis
 * - Weather station management
 * - Data visualization
 */

// WeatherStation class represents a weather data source
class WeatherStation {
    constructor(id, name, location, coordinates, elevation) {
        this.id = id;
        this.name = name;
        this.location = location;
        this.coordinates = coordinates; // { latitude, longitude }
        this.elevation = elevation; // in meters
        this.isActive = false;
        this.lastUpdate = null;
        this.data = null;
        this.sensors = new Map();
        this.maintenanceSchedule = null;
        this.isOnline = false;
        this.apiEndpoint = null;
        this.apiKey = null;
        this.updateInterval = 600000; // 10 minutes
        this.updateTimer = null;
    }
    
    /**
     * Add sensor to weather station
     * @param {string} sensorId - Sensor ID
     * @param {string} sensorType - Type of sensor
     * @param {Object} sensorConfig - Sensor configuration
     */
    addSensor(sensorId, sensorType, sensorConfig = {}) {
        this.sensors.set(sensorId, {
            id: sensorId,
            type: sensorType,
            config: sensorConfig,
            lastReading: null,
            calibrationOffset: sensorConfig.calibrationOffset || 0,
            accuracy: sensorConfig.accuracy || null,
            isCalibrated: false
        });
    }
    
    /**
     * Remove sensor from weather station
     * @param {string} sensorId - Sensor ID to remove
     */
    removeSensor(sensorId) {
        return this.sensors.delete(sensorId);
    }
    
    /**
     * Get sensor by ID
     * @param {string} sensorId - Sensor ID
     */
    getSensor(sensorId) {
        return this.sensors.get(sensorId);
    }
    
    /**
     * Get all sensors
     */
    getSensors() {
        return Array.from(this.sensors.values());
    }
    
    /**
     * Calibrate sensor
     * @param {string} sensorId - Sensor ID
     * @param {number} offset - Calibration offset
     */
    calibrateSensor(sensorId, offset) {
        const sensor = this.sensors.get(sensorId);
        if (sensor) {
            sensor.calibrationOffset = offset;
            sensor.isCalibrated = true;
            return true;
        }
        return false;
    }
    
    /**
     * Start station monitoring
     */
    start() {
        if (this.isActive) {
            return false;
        }
        
        this.isActive = true;
        this.isOnline = true;
        this.lastUpdate = new Date();
        
        // Start update timer
        this.startUpdateTimer();
        
        // Perform initial update
        this.updateData();
        
        return true;
    }
    
    /**
     * Stop station monitoring
     */
    stop() {
        if (!this.isActive) {
            return false;
        }
        
        this.isActive = false;
        this.isOnline = false;
        
        // Stop update timer
        if (this.updateTimer) {
            clearInterval(this.updateTimer);
            this.updateTimer = null;
        }
        
        return true;
    }
    
    /**
     * Start update timer
     */
    startUpdateTimer() {
        if (this.updateTimer) {
            clearInterval(this.updateTimer);
        }
        
        this.updateTimer = setInterval(() => {
            this.updateData();
        }, this.updateInterval);
    }
    
    /**
     * Update weather data
     */
    async updateData() {
        try {
            // In a real implementation, this would fetch data from API or sensors
            // For now, we'll simulate data generation
            const data = this.generateWeatherData();
            
            this.data = data;
            this.lastUpdate = new Date();
            
            // Update sensor readings
            this.updateSensorReadings(data);
            
            return data;
        } catch (error) {
            console.error(`Error updating data for station ${this.id}:`, error);
            this.isOnline = false;
            return null;
        }
    }
    
    /**
     * Generate simulated weather data
     */
    generateWeatherData() {
        const now = new Date();
        const temp = 20 + Math.random() * 15; // 20-35°C
        const humidity = 40 + Math.random() * 40; // 40-80%
        const pressure = 1000 + Math.random() * 30; // 1000-1030 hPa
        const windSpeed = Math.random() * 20; // 0-20 km/h
        const windDirection = Math.random() * 360; // 0-360°
        const precipitation = Math.random() * 5; // 0-5 mm
        const visibility = 5 + Math.random() * 15; // 5-20 km
        
        return {
            timestamp: now,
            temperature: temp,
            humidity: humidity,
            pressure: pressure,
            windSpeed: windSpeed,
            windDirection: windDirection,
            precipitation: precipitation,
            visibility: visibility,
            uvIndex: Math.floor(Math.random() * 11), // 0-10
            cloudCover: Math.floor(Math.random() * 100), // 0-100%
            conditions: this.determineWeatherConditions(temp, humidity, precipitation, cloudCover)
        };
    }
    
    /**
     * Determine weather conditions based on parameters
     * @param {number} temp - Temperature
     * @param {number} humidity - Humidity
     * @param {number} precipitation - Precipitation
     * @param {number} cloudCover - Cloud cover
     */
    determineWeatherConditions(temp, humidity, precipitation, cloudCover) {
        if (precipitation > 2) {
            return 'Rain';
        } else if (precipitation > 0) {
            return 'Light Rain';
        } else if (cloudCover > 80) {
            return 'Overcast';
        } else if (cloudCover > 40) {
            return 'Partly Cloudy';
        } else {
            return 'Clear';
        }
    }
    
    /**
     * Update sensor readings
     * @param {Object} data - Weather data
     */
    updateSensorReadings(data) {
        this.sensors.forEach(sensor => {
            let reading = null;
            
            switch (sensor.type) {
                case 'temperature':
                    reading = data.temperature + sensor.calibrationOffset;
                    break;
                case 'humidity':
                    reading = data.humidity + sensor.calibrationOffset;
                    break;
                case 'pressure':
                    reading = data.pressure + sensor.calibrationOffset;
                    break;
                case 'windSpeed':
                    reading = data.windSpeed + sensor.calibrationOffset;
                    break;
                case 'windDirection':
                    reading = data.windDirection + sensor.calibrationOffset;
                    break;
                case 'precipitation':
                    reading = data.precipitation + sensor.calibrationOffset;
                    break;
                case 'visibility':
                    reading = data.visibility + sensor.calibrationOffset;
                    break;
                case 'uvIndex':
                    reading = data.uvIndex + sensor.calibrationOffset;
                    break;
            }
            
            if (reading !== null) {
                sensor.lastReading = {
                    value: reading,
                    timestamp: new Date(),
                    unit: this.getUnitForSensorType(sensor.type)
                };
            }
        });
    }
    
    /**
     * Get unit for sensor type
     * @param {string} sensorType - Type of sensor
     */
    getUnitForSensorType(sensorType) {
        const units = {
            'temperature': '°C',
            'humidity': '%',
            'pressure': 'hPa',
            'windSpeed': 'km/h',
            'windDirection': '°',
            'precipitation': 'mm',
            'visibility': 'km',
            'uvIndex': ''
        };
        
        return units[sensorType] || '';
    }
    
    /**
     * Get station summary
     */
    getSummary() {
        return {
            id: this.id,
            name: this.name,
            location: this.location,
            coordinates: this.coordinates,
            elevation: this.elevation,
            isActive: this.isActive,
            isOnline: this.isOnline,
            lastUpdate: this.lastUpdate,
            sensorCount: this.sensors.size,
            data: this.data
        };
    }
    
    /**
     * Get current weather data
     */
    getCurrentWeather() {
        if (!this.data) {
            return null;
        }
        
        return {
            ...this.data,
            station: this.getSummary()
        };
    }
}

// WeatherData class represents processed weather data
class WeatherData {
    constructor(timestamp, location, dataPoints) {
        this.timestamp = timestamp;
        this.location = location;
        this.dataPoints = dataPoints; // Array of data from different stations
        this.aggregatedData = null;
        this.quality = 'unknown'; // 'excellent', 'good', 'fair', 'poor'
        this.sources = [];
        this.confidence = 0;
        
        this.processData();
    }
    
    /**
     * Process and aggregate data from multiple sources
     */
    processData() {
        if (this.dataPoints.length === 0) {
            this.quality = 'poor';
            return;
        }
        
        // Extract sources
        this.sources = this.dataPoints.map(point => point.station.id);
        
        // Aggregate temperature (weighted by station quality)
        let totalTemp = 0;
        let totalWeight = 0;
        
        for (const point of this.dataPoints) {
            const weight = this.calculateStationWeight(point.station);
            totalTemp += point.data.temperature * weight;
            totalWeight += weight;
        }
        
        const avgTemp = totalWeight > 0 ? totalTemp / totalWeight : 0;
        
        // Simple averaging for other metrics
        const avgHumidity = this.dataPoints.reduce((sum, point) => sum + point.data.humidity, 0) / this.dataPoints.length;
        const avgPressure = this.dataPoints.reduce((sum, point) => sum + point.data.pressure, 0) / this.dataPoints.length;
        const avgWindSpeed = this.dataPoints.reduce((sum, point) => sum + point.data.windSpeed, 0) / this.dataPoints.length;
        const avgPrecipitation = this.dataPoints.reduce((sum, point) => sum + point.data.precipitation, 0) / this.dataPoints.length;
        
        // Determine conditions
        const conditions = this.determineDominantConditions();
        
        this.aggregatedData = {
            temperature: avgTemp,
            humidity: avgHumidity,
            pressure: avgPressure,
            windSpeed: avgWindSpeed,
            windDirection: this.calculateAverageWindDirection(),
            precipitation: avgPrecipitation,
            visibility: this.dataPoints.reduce((sum, point) => sum + point.data.visibility, 0) / this.dataPoints.length,
            uvIndex: Math.floor(this.dataPoints.reduce((sum, point) => sum + point.data.uvIndex, 0) / this.dataPoints.length),
            cloudCover: Math.floor(this.dataPoints.reduce((sum, point) => sum + point.data.cloudCover, 0) / this.dataPoints.length),
            conditions: conditions
        };
        
        // Calculate quality based on data consistency and source count
        this.calculateDataQuality();
        
        // Calculate confidence
        this.calculateConfidence();
    }
    
    /**
     * Calculate station weight for data aggregation
     * @param {Object} station - Weather station
     */
    calculateStationWeight(station) {
        // In a real implementation, this would consider station accuracy,
        // reliability, maintenance status, etc.
        // For now, we'll use a simple weight
        return station.isActive && station.isOnline ? 1.0 : 0.5;
    }
    
    /**
     * Determine dominant weather conditions
     */
    determineDominantConditions() {
        const conditionsCount = {};
        
        for (const point of this.dataPoints) {
            const condition = point.data.conditions;
            conditionsCount[condition] = (conditionsCount[condition] || 0) + 1;
        }
        
        // Find the most common condition
        let dominantCondition = 'Unknown';
        let maxCount = 0;
        
        for (const [condition, count] of Object.entries(conditionsCount)) {
            if (count > maxCount) {
                maxCount = count;
                dominantCondition = condition;
            }
        }
        
        return dominantCondition;
    }
    
    /**
     * Calculate average wind direction
     */
    calculateAverageWindDirection() {
        if (this.dataPoints.length === 0) return 0;
        
        let x = 0;
        let y = 0;
        
        for (const point of this.dataPoints) {
            const angle = point.data.windDirection * Math.PI / 180;
            x += Math.cos(angle);
            y += Math.sin(angle);
        }
        
        let avgAngle = Math.atan2(y, x) * 180 / Math.PI;
        if (avgAngle < 0) {
            avgAngle += 360;
        }
        
        return avgAngle;
    }
    
    /**
     * Calculate data quality based on source consistency
     */
    calculateDataQuality() {
        const sourceCount = this.dataPoints.length;
        
        if (sourceCount >= 5) {
            this.quality = 'excellent';
        } else if (sourceCount >= 3) {
            this.quality = 'good';
        } else if (sourceCount >= 2) {
            this.quality = 'fair';
        } else {
            this.quality = 'poor';
        }
    }
    
    /**
     * Calculate confidence in the data
     */
    calculateConfidence() {
        let confidence = 0;
        
        // Base confidence on quality
        const qualityScores = {
            'excellent': 0.9,
            'good': 0.7,
            'fair': 0.5,
            'poor': 0.3
        };
        
        confidence = qualityScores[this.quality] || 0.1;
        
        // Adjust based on data consistency
        const tempVariance = this.calculateVariance(this.dataPoints.map(p => p.data.temperature));
        if (tempVariance < 2) {
            confidence += 0.1;
        } else if (tempVariance > 5) {
            confidence -= 0.1;
        }
        
        this.confidence = Math.max(0.1, Math.min(1.0, confidence));
    }
    
    /**
     * Calculate variance of values
     * @param {Array} values - Array of values
     */
    calculateVariance(values) {
        if (values.length === 0) return 0;
        
        const mean = values.reduce((sum, val) => sum + val, 0) / values.length;
        const squaredDiffs = values.map(val => Math.pow(val - mean, 2));
        const avgSquaredDiff = squaredDiffs.reduce((sum, diff) => sum + diff, 0) / values.length;
        
        return Math.sqrt(avgSquaredDiff);
    }
    
    /**
     * Get data summary
     */
    getSummary() {
        return {
            timestamp: this.timestamp,
            location: this.location,
            aggregatedData: this.aggregatedData,
            quality: this.quality,
            sources: this.sources,
            confidence: this.confidence,
            sourceCount: this.dataPoints.length
        };
    }
}

// WeatherForecast class represents weather forecast data
class WeatherForecast {
    constructor(location, forecastData) {
        this.location = location;
        this.forecastData = forecastData; // Array of daily forecasts
        this.generatedAt = new Date();
        this.model = 'global';
        this.accuracy = null;
        this.confidence = 0;
        
        this.validateForecast();
    }
    
    /**
     * Validate forecast data
     */
    validateForecast() {
        if (!this.forecastData || this.forecastData.length === 0) {
            this.confidence = 0.1;
            return;
        }
        
        // Simple confidence calculation based on forecast period
        const forecastDays = this.forecastData.length;
        if (forecastDays <= 3) {
            this.confidence = 0.8;
        } else if (forecastDays <= 7) {
            this.confidence = 0.6;
        } else {
            this.confidence = 0.4;
        }
        
        // Validate each day's forecast
        let validDays = 0;
        for (const day of this.forecastData) {
            if (this.validateDayForecast(day)) {
                validDays++;
            }
        }
        
        // Adjust confidence based on data quality
        const validRatio = validDays / forecastDays;
        this.confidence *= validRatio;
    }
    
    /**
     * Validate a single day's forecast
     * @param {Object} dayForecast - Daily forecast data
     */
    validateDayForecast(dayForecast) {
        const requiredFields = ['date', 'highTemp', 'lowTemp', 'conditions', 'precipitationChance'];
        
        for (const field of requiredFields) {
            if (dayForecast[field] === undefined || dayForecast[field] === null) {
                return false;
            }
        }
        
        // Validate temperature ranges
        if (dayForecast.highTemp < dayForecast.lowTemp) {
            return false;
        }
        
        // Validate precipitation chance
        if (dayForecast.precipitationChance < 0 || dayForecast.precipitationChance > 100) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Get forecast for a specific date
     * @param {Date} date - Date to get forecast for
     */
    getForecastForDate(date) {
        const dateStr = date.toDateString();
        
        for (const day of this.forecastData) {
            if (new Date(day.date).toDateString() === dateStr) {
                return day;
            }
        }
        
        return null;
    }
    
    /**
     * Get forecast summary
     */
    getSummary() {
        return {
            location: this.location,
            generatedAt: this.generatedAt,
            model: this.model,
            confidence: this.confidence,
            days: this.forecastData.length,
            firstDay: this.forecastData.length > 0 ? this.forecastData[0].date : null,
            lastDay: this.forecastData.length > 0 ? this.forecastData[this.forecastData.length - 1].date : null
        };
    }
}

// WeatherAlert class represents a weather alert
class WeatherAlert {
    constructor(id, type, severity, title, description, location, startTime, endTime) {
        this.id = id;
        this.type = type; // 'temperature', 'precipitation', 'wind', 'storm', etc.
        this.severity = severity; // 'low', 'medium', 'high', 'extreme'
        this.title = title;
        this.description = description;
        this.location = location;
        this.startTime = startTime;
        this.endTime = endTime;
        this.isActive = true;
        this.affectedAreas = [];
        this.issuedAt = new Date();
        this.updatedAt = new Date();
        this.issuer = 'Weather Service';
        this.instructions = '';
        this.priority = this.calculatePriority();
    }
    
    /**
     * Calculate alert priority based on severity
     */
    calculatePriority() {
        const priorities = {
            'low': 1,
            'medium': 2,
            'high': 3,
            'extreme': 4
        };
        
        return priorities[this.severity] || 1;
    }
    
    /**
     * Add affected area
     * @param {Object} area - Area object with name and coordinates
     */
    addAffectedArea(area) {
        this.affectedAreas.push(area);
        this.updatedAt = new Date();
    }
    
    /**
     * Remove affected area
     * @param {string} areaName - Name of area to remove
     */
    removeAffectedArea(areaName) {
        this.affectedAreas = this.affectedAreas.filter(area => area.name !== areaName);
        this.updatedAt = new Date();
    }
    
    /**
     * Check if alert is active at a specific time
     * @param {Date} time - Time to check
     */
    isActiveAt(time) {
        return this.isActive && time >= this.startTime && time <= this.endTime;
    }
    
    /**
     * Deactivate alert
     */
    deactivate() {
        this.isActive = false;
        this.updatedAt = new Date();
    }
    
    /**
     * Update alert
     * @param {Object} updates - Updates to apply
     */
    update(updates) {
        if (updates.title) this.title = updates.title;
        if (updates.description) this.description = updates.description;
        if (updates.instructions) this.instructions = updates.instructions;
        if (updates.endTime) this.endTime = updates.endTime;
        if (updates.severity) {
            this.severity = updates.severity;
            this.priority = this.calculatePriority();
        }
        
        this.updatedAt = new Date();
    }
    
    /**
     * Get alert summary
     */
    getSummary() {
        return {
            id: this.id,
            type: this.type,
            severity: this.severity,
            title: this.title,
            description: this.description,
            location: this.location,
            startTime: this.startTime,
            endTime: this.endTime,
            isActive: this.isActive,
            issuedAt: this.issuedAt,
            updatedAt: this.updatedAt,
            affectedAreaCount: this.affectedAreas.length,
            priority: this.priority
        };
    }
}

// WeatherMonitoringSystem class manages the entire weather monitoring system
class WeatherMonitoringSystem {
    constructor() {
        this.stations = new Map();
        this.currentWeatherData = new Map(); // location -> WeatherData
        this.forecasts = new Map(); // location -> WeatherForecast
        this.alerts = new Map(); // alertId -> WeatherAlert
        this.historicalData = new Map(); // location -> Array of WeatherData
        this.updateInterval = 300000; // 5 minutes
        this.updateTimer = null;
        this.eventListeners = new Map();
    }
    
    /**
     * Add weather station
     * @param {WeatherStation} station - Weather station to add
     */
    addStation(station) {
        this.stations.set(station.id, station);
        
        // Set up event listeners for the station
        station.onUpdate = (data) => {
            this.handleStationUpdate(station, data);
        };
    }
    
    /**
     * Remove weather station
     * @param {string} stationId - ID of station to remove
     */
    removeStation(stationId) {
        const station = this.stations.get(stationId);
        if (station) {
            station.stop();
            return this.stations.delete(stationId);
        }
        return false;
    }
    
    /**
     * Start monitoring system
     */
    start() {
        // Start all stations
        this.stations.forEach(station => {
            station.start();
        });
        
        // Start system update timer
        if (this.updateTimer) {
            clearInterval(this.updateTimer);
        }
        
        this.updateTimer = setInterval(() => {
            this.updateSystem();
        }, this.updateInterval);
        
        // Perform initial update
        this.updateSystem();
    }
    
    /**
     * Stop monitoring system
     */
    stop() {
        // Stop all stations
        this.stations.forEach(station => {
            station.stop();
        });
        
        // Stop system update timer
        if (this.updateTimer) {
            clearInterval(this.updateTimer);
            this.updateTimer = null;
        }
    }
    
    /**
     * Update system data
     */
    updateSystem() {
        // Group stations by location
        const locationGroups = this.groupStationsByLocation();
        
        // Process each location
        for (const [location, stations] of locationGroups) {
            this.processLocationWeather(location, stations);
        }
        
        // Check for weather alerts
        this.checkForAlerts();
        
        // Emit update event
        this.emit('system-updated', {
            timestamp: new Date(),
            stationCount: this.stations.size,
            locationCount: this.currentWeatherData.size,
            alertCount: this.alerts.size
        });
    }
    
    /**
     * Group stations by location
     */
    groupStationsByLocation() {
        const locationGroups = new Map();
        
        this.stations.forEach(station => {
            // Group by location (in a real system, this might use geohashing or proximity)
            const location = station.location;
            
            if (!locationGroups.has(location)) {
                locationGroups.set(location, []);
            }
            
            locationGroups.get(location).push(station);
        });
        
        return locationGroups;
    }
    
    /**
     * Process weather data for a location
     * @param {string} location - Location name
     * @param {Array} stations - Array of stations at this location
     */
    processLocationWeather(location, stations) {
        // Get data from all active stations
        const dataPoints = [];
        
        for (const station of stations) {
            if (station.isActive && station.data) {
                dataPoints.push({
                    station: station.getSummary(),
                    data: station.data
                });
            }
        }
        
        if (dataPoints.length > 0) {
            // Create WeatherData object
            const weatherData = new WeatherData(
                new Date(),
                location,
                dataPoints
            );
            
            // Store current weather
            this.currentWeatherData.set(location, weatherData);
            
            // Store in historical data
            if (!this.historicalData.has(location)) {
                this.historicalData.set(location, []);
            }
            
            const historical = this.historicalData.get(location);
            historical.push(weatherData);
            
            // Limit historical data size
            if (historical.length > 1000) {
                historical.splice(0, 100);
            }
        }
    }
    
    /**
     * Check for weather alerts
     */
    checkForAlerts() {
        this.currentWeatherData.forEach((weatherData, location) => {
            if (!weatherData.aggregatedData) return;
            
            const data = weatherData.aggregatedData;
            
            // Check for temperature alerts
            if (data.temperature > 40) {
                this.createOrUpdateAlert(
                    'high-temp',
                    'high',
                    'Extreme Heat Warning',
                    `Temperature in ${location} has exceeded 40°C. Stay hydrated and avoid prolonged sun exposure.`,
                    location,
                    new Date(),
                    new Date(Date.now() + 24 * 60 * 60 * 1000) // 24 hours
                );
            } else if (data.temperature < -10) {
                this.createOrUpdateAlert(
                    'low-temp',
                    'high',
                    'Extreme Cold Warning',
                    `Temperature in ${location} has dropped below -10°C. Dress warmly and limit outdoor exposure.`,
                    location,
                    new Date(),
                    new Date(Date.now() + 24 * 60 * 60 * 1000) // 24 hours
                );
            }
            
            // Check for precipitation alerts
            if (data.precipitation > 50) {
                this.createOrUpdateAlert(
                    'heavy-rain',
                    'high',
                    'Heavy Rain Warning',
                    `Heavy rainfall in ${location} may cause flooding. Avoid low-lying areas and be cautious when driving.`,
                    location,
                    new Date(),
                    new Date(Date.now() + 12 * 60 * 60 * 1000) // 12 hours
                );
            }
            
            // Check for wind alerts
            if (data.windSpeed > 70) {
                this.createOrUpdateAlert(
                    'high-wind',
                    'medium',
                    'High Wind Advisory',
                    `Strong winds in ${location} may cause hazardous conditions. Secure outdoor objects and avoid travel if possible.`,
                    location,
                    new Date(),
                    new Date(Date.now() + 6 * 60 * 60 * 1000) // 6 hours
                );
            }
        });
    }
    
    /**
     * Create or update weather alert
     * @param {string} type - Alert type
     * @param {string} severity - Alert severity
     * @param {string} title - Alert title
     * @param {string} description - Alert description
     * @param {string} location - Alert location
     * @param {Date} startTime - Alert start time
     * @param {Date} endTime - Alert end time
     */
    createOrUpdateAlert(type, severity, title, description, location, startTime, endTime) {
        // Check if similar alert already exists
        for (const [id, alert] of this.alerts) {
            if (
                alert.type === type &&
                alert.location === location &&
                alert.isActiveAt(new Date())
            ) {
                // Update existing alert
                alert.update({
                    title: title,
                    description: description,
                    endTime: endTime
                });
                return;
            }
        }
        
        // Create new alert
        const alertId = this.generateAlertId();
        const alert = new WeatherAlert(
            alertId,
            type,
            severity,
            title,
            description,
            location,
            startTime,
            endTime
        );
        
        this.alerts.set(alertId, alert);
        
        // Emit alert event
        this.emit('alert-created', { alert });
    }
    
    /**
     * Handle station update
     * @param {WeatherStation} station - Updated station
     * @param {Object} data - New data
     */
    handleStationUpdate(station, data) {
        this.emit('station-updated', {
            stationId: station.id,
            data: data
        });
    }
    
    /**
     * Add weather forecast
     * @param {WeatherForecast} forecast - Forecast to add
     */
    addForecast(forecast) {
        this.forecasts.set(forecast.location, forecast);
        
        // Emit forecast event
        this.emit('forecast-added', { forecast });
    }
    
    /**
     * Get current weather for a location
     * @param {string} location - Location name
     */
    getCurrentWeather(location) {
        return this.currentWeatherData.get(location);
    }
    
    /**
     * Get forecast for a location
     * @param {string} location - Location name
     */
    getForecast(location) {
        return this.forecasts.get(location);
    }
    
    /**
     * Get active alerts for a location
     * @param {string} location - Location name
     */
    getActiveAlerts(location) {
        const alerts = [];
        
        this.alerts.forEach(alert => {
            if (alert.isActiveAt(new Date()) && alert.location === location) {
                alerts.push(alert);
            }
        });
        
        // Sort by priority (highest first)
        alerts.sort((a, b) => b.priority - a.priority);
        
        return alerts;
    }
    
    /**
     * Get historical data for a location
     * @param {string} location - Location name
     * @param {Date} startDate - Start date
     * @param {Date} endDate - End date
     */
    getHistoricalData(location, startDate, endDate) {
        const allData = this.historicalData.get(location) || [];
        
        return allData.filter(data => 
            data.timestamp >= startDate && data.timestamp <= endDate
        );
    }
    
    /**
     * Generate unique alert ID
     */
    generateAlertId() {
        return 'alert_' + Date.now().toString(36) + Math.random().toString(36).substring(2);
    }
    
    /**
     * Add event listener
     * @param {string} event - Event name
     * @param {Function} callback - Event callback
     */
    on(event, callback) {
        if (!this.eventListeners.has(event)) {
            this.eventListeners.set(event, []);
        }
        
        this.eventListeners.get(event).push(callback);
    }
    
    /**
     * Remove event listener
     * @param {string} event - Event name
     * @param {Function} callback - Event callback
     */
    off(event, callback) {
        if (this.eventListeners.has(event)) {
            const listeners = this.eventListeners.get(event);
            const index = listeners.indexOf(callback);
            if (index > -1) {
                listeners.splice(index, 1);
            }
        }
    }
    
    /**
     * Emit event
     * @param {string} event - Event name
     * @param {Object} data - Event data
     */
    emit(event, data) {
        if (this.eventListeners.has(event)) {
            this.eventListeners.get(event).forEach(callback => {
                try {
                    callback(data);
                } catch (error) {
                    console.error(`Error in event listener for '${event}':`, error);
                }
            });
        }
    }
    
    /**
     * Get system statistics
     */
    getStatistics() {
        const activeStationCount = Array.from(this.stations.values()).filter(station => station.isActive).length;
        const onlineStationCount = Array.from(this.stations.values()).filter(station => station.isOnline).length;
        const activeAlertCount = Array.from(this.alerts.values()).filter(alert => alert.isActive).length;
        
        return {
            stationCount: this.stations.size,
            activeStationCount: activeStationCount,
            onlineStationCount: onlineStationCount,
            locationCount: this.currentWeatherData.size,
            forecastCount: this.forecasts.size,
            totalAlertCount: this.alerts.size,
            activeAlertCount: activeAlertCount
        };
    }
}

// Main function to demonstrate weather monitoring system
function main() {
    console.log('Initializing Weather Monitoring System...\n');
    
    // Create weather monitoring system
    const system = new WeatherMonitoringSystem();
    
    // Create weather stations
    const station1 = new WeatherStation('station1', 'Central Park Station', 'New York', { latitude: 40.7829, longitude: -73.9654 }, 10);
    const station2 = new WeatherStation('station2', 'Brooklyn Bridge Station', 'New York', { latitude: 40.7061, longitude: -73.9969 }, 5);
    const station3 = new WeatherStation('station3', 'Times Square Station', 'New York', { latitude: 40.7580, longitude: -73.9855 }, 15);
    const station4 = new WeatherStation('station4', 'Central Station', 'Boston', { latitude: 42.3601, longitude: -71.0589 }, 8);
    const station5 = new WeatherStation('station5', 'Harvard Yard Station', 'Boston', { latitude: 42.3770, longitude: -71.1167 }, 12);
    
    // Add sensors to stations
    station1.addSensor('temp1', 'temperature', { calibrationOffset: 0.5, accuracy: 0.1 });
    station1.addSensor('hum1', 'humidity', { calibrationOffset: 0, accuracy: 1 });
    station1.addSensor('press1', 'pressure', { calibrationOffset: 1.2, accuracy: 0.5 });
    
    station2.addSensor('temp2', 'temperature', { calibrationOffset: -0.3, accuracy: 0.1 });
    station2.addSensor('hum2', 'humidity', { calibrationOffset: 2, accuracy: 1 });
    
    station3.addSensor('temp3', 'temperature', { calibrationOffset: 0.1, accuracy: 0.2 });
    station3.addSensor('press3', 'pressure', { calibrationOffset: -0.5, accuracy: 0.5 });
    
    station4.addSensor('temp4', 'temperature', { calibrationOffset: 0, accuracy: 0.1 });
    station4.addSensor('wind4', 'windSpeed', { calibrationOffset: 1.0, accuracy: 0.5 });
    
    station5.addSensor('temp5', 'temperature', { calibrationOffset: -0.2, accuracy: 0.1 });
    station5.addSensor('hum5', 'humidity', { calibrationOffset: 1, accuracy: 1 });
    
    // Add stations to system
    system.addStation(station1);
    system.addStation(station2);
    system.addStation(station3);
    system.addStation(station4);
    system.addStation(station5);
    
    console.log('Added weather stations:');
    system.stations.forEach(station => {
        console.log(`- ${station.name} (${station.id}) in ${station.location}`);
        console.log(`  Sensors: ${station.sensors.size}`);
        console.log(`  Coordinates: ${station.coordinates.latitude}, ${station.coordinates.longitude}`);
        console.log(`  Elevation: ${station.elevation}m`);
    });
    
    // Start monitoring system
    system.start();
    console.log('\nStarted weather monitoring system');
    
    // Add event listeners
    system.on('system-updated', (data) => {
        console.log(`\nSystem updated at ${data.timestamp}`);
        console.log(`- Active stations: ${data.activeStationCount}`);
        console.log(`- Locations monitored: ${data.locationCount}`);
        console.log(`- Active alerts: ${data.alertCount}`);
    });
    
    system.on('alert-created', (data) => {
        const alert = data.alert;
        console.log(`\n🚨 Weather Alert Created:`);
        console.log(`- Type: ${alert.type}`);
        console.log(`- Severity: ${alert.severity}`);
        console.log(`- Title: ${alert.title}`);
        console.log(`- Location: ${alert.location}`);
        console.log(`- Priority: ${alert.priority}`);
    });
    
    // Simulate some data updates
    setTimeout(() => {
        console.log('\nCurrent weather data:');
        system.currentWeatherData.forEach((weatherData, location) => {
            const data = weatherData.aggregatedData;
            console.log(`\n${location}:`);
            console.log(`- Temperature: ${data.temperature.toFixed(1)}°C`);
            console.log(`- Humidity: ${data.humidity.toFixed(1)}%`);
            console.log(`- Pressure: ${data.pressure.toFixed(1)} hPa`);
            console.log(`- Wind: ${data.windSpeed.toFixed(1)} km/h`);
            console.log(`- Conditions: ${data.conditions}`);
            console.log(`- Data Quality: ${weatherData.quality}`);
            console.log(`- Confidence: ${(weatherData.confidence * 100).toFixed(1)}%`);
        });
        
        // Get system statistics
        const stats = system.getStatistics();
        console.log('\nSystem Statistics:');
        console.log(`- Total Stations: ${stats.stationCount}`);
        console.log(`- Active Stations: ${stats.activeStationCount}`);
        console.log(`- Online Stations: ${stats.onlineStationCount}`);
        console.log(`- Locations Monitored: ${stats.locationCount}`);
        console.log(`- Active Alerts: ${stats.activeAlertCount}`);
        
        // Get active alerts
        system.currentWeatherData.forEach((weatherData, location) => {
            const alerts = system.getActiveAlerts(location);
            if (alerts.length > 0) {
                console.log(`\nActive alerts for ${location}:`);
                alerts.forEach(alert => {
                    console.log(`- ${alert.title} (${alert.severity})`);
                });
            }
        });
        
    }, 2000);
}

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        WeatherStation,
        WeatherData,
        WeatherForecast,
        WeatherAlert,
        WeatherMonitoringSystem
    };
}