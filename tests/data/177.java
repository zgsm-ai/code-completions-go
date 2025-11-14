/**
 * Weather Forecasting System
 * 
 * This class implements a comprehensive weather forecasting system
 * that can process historical weather data, apply various forecasting algorithms,
 * and predict future weather conditions.
 * 
 * Features:
 * - Historical weather data management
 * - Multiple forecasting algorithms (ARIMA, Exponential Smoothing, etc.)
 * - Weather pattern recognition
 * - Seasonal trend analysis
 * - Temperature and precipitation predictions
 * - Forecast accuracy evaluation
 * - Data visualization (simplified)
 */

import java.util.*;
import java.text.*;
import java.io.*;
import java.math.BigDecimal;
import java.math.RoundingMode;

/**
 * WeatherData class represents daily weather observations
 */
class WeatherData {
    private Date date;
    private double maxTemperature;
    private double minTemperature;
    private double avgTemperature;
    private double maxHumidity;
    private double minHumidity;
    private double avgHumidity;
    private double precipitation;
    private double windSpeed;
    private String windDirection;
    private double pressure;
    private String weatherCondition;
    private String description;
    
    /**
     * Constructor for WeatherData
     * @param date Date of observation
     * @param maxTemp Maximum temperature
     * @param minTemp Minimum temperature
     * @param avgTemp Average temperature
     * @param maxHumid Maximum humidity
     * @param minHumid Minimum humidity
     * @param avgHumid Average humidity
     * @param precip Precipitation amount
     * @param windSpeed Wind speed
     * @param windDir Wind direction
     * @param pressure Atmospheric pressure
     * @param condition Weather condition
     * @param desc Description
     */
    public WeatherData(Date date, double maxTemp, double minTemp, double avgTemp,
                    double maxHumid, double minHumid, double avgHumid,
                    double precip, double windSpeed, String windDir,
                    double pressure, String condition, String desc) {
        this.date = date;
        this.maxTemperature = maxTemp;
        this.minTemperature = minTemp;
        this.avgTemperature = avgTemp;
        this.maxHumidity = maxHumid;
        this.minHumidity = minHumid;
        this.avgHumidity = avgHumid;
        this.precipitation = precip;
        this.windSpeed = windSpeed;
        this.windDirection = windDir;
        this.pressure = pressure;
        this.weatherCondition = condition;
        this.description = desc;
    }
    
    /**
     * Get observation date
     * @return Date of observation
     */
    public Date getDate() {
        return date;
    }
    
    /**
     * Get maximum temperature
     * @return Maximum temperature
     */
    public double getMaxTemperature() {
        return maxTemperature;
    }
    
    /**
     * Get minimum temperature
     * @return Minimum temperature
     */
    public double getMinTemperature() {
        return minTemperature;
    }
    
    /**
     * Get average temperature
     * @return Average temperature
     */
    public double getAvgTemperature() {
        return avgTemperature;
    }
    
    /**
     * Get maximum humidity
     * @return Maximum humidity
     */
    public double getMaxHumidity() {
        return maxHumidity;
    }
    
    /**
     * Get minimum humidity
     * @return Minimum humidity
     */
    public double getMinHumidity() {
        return minHumidity;
    }
    
    /**
     * Get average humidity
     * @return Average humidity
     */
    public double getAvgHumidity() {
        return avgHumidity;
    }
    
    /**
     * Get precipitation
     * @return Precipitation amount
     */
    public double getPrecipitation() {
        return precipitation;
    }
    
    /**
     * Get wind speed
     * @return Wind speed
     */
    public double getWindSpeed() {
        return windSpeed;
    }
    
    /**
     * Get wind direction
     * @return Wind direction
     */
    public String getWindDirection() {
        return windDirection;
    }
    
    /**
     * Get pressure
     * @return Atmospheric pressure
     */
    public double getPressure() {
        return pressure;
    }
    
    /**
     * Get weather condition
     * @return Weather condition
     */
    public String getWeatherCondition() {
        return weatherCondition;
    }
    
    /**
     * Get description
     * @return Description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get weather data summary
     * @return Weather summary string
     */
    public String getSummary() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        StringBuilder sb = new StringBuilder();
        
        sb.append("Date: ").append(sdf.format(date)).append("\n");
        sb.append("Temperature: ").append(minTemperature).append("°C to ")
                                .append(maxTemperature).append("°C (Avg: ")
                                .append(avgTemperature).append("°C)\n");
        sb.append("Humidity: ").append(minHumidity).append("% to ")
                                .append(maxHumidity).append("% (Avg: ")
                                .append(avgHumid).append("%)\n");
        sb.append("Precipitation: ").append(precipitation).append("mm\n");
        sb.append("Wind: ").append(windSpeed).append(" km/h from ")
                                .append(windDirection).append("\n");
        sb.append("Pressure: ").append(pressure).append(" hPa\n");
        sb.append("Condition: ").append(weatherCondition).append("\n");
        sb.append("Description: ").append(description).append("\n");
        
        return sb.toString();
    }
}

/**
 * ForecastAlgorithm enum represents available forecasting algorithms
 */
enum ForecastAlgorithm {
    MOVING_AVERAGE("Moving Average"),
    WEIGHTED_AVERAGE("Weighted Average"),
    EXPONENTIAL_SMOOTHING("Exponential Smoothing"),
    ARIMA("ARIMA"),
    LINEAR_REGRESSION("Linear Regression"),
    SEASONAL_DECOMPOSITION("Seasonal Decomposition"),
    NEURAL_NETWORK("Neural Network")
}

/**
 * ForecastingModel class implements weather forecasting algorithms
 */
class ForecastingModel {
    private ForecastAlgorithm algorithm;
    private List<Double> parameters;
    private String modelDescription;
    private boolean trained;
    
    /**
     * Constructor for ForecastingModel
     * @param algorithm Forecasting algorithm
     * @param description Model description
     */
    public ForecastingModel(ForecastAlgorithm algorithm, String description) {
        this.algorithm = algorithm;
        this.parameters = new ArrayList<>();
        this.modelDescription = description;
        this.trained = false;
        
        // Set default parameters based on algorithm
        if (algorithm == ForecastAlgorithm.MOVING_AVERAGE) {
            parameters.add(7.0);  // 7-day moving average
        } else if (algorithm == ForecastAlgorithm.EXPONENTIAL_SMOOTHING) {
            parameters.add(0.3);  // Alpha parameter
        } else if (algorithm == ForecastAlgorithm.SEASONAL_DECOMPOSITION) {
            parameters.add(365.0);  // Yearly season
            parameters.add(12.0);   // Monthly cycle
        } else if (algorithm == ForecastAlgorithm.LINEAR_REGRESSION) {
            parameters.add(0.5);  // Trend coefficient
            parameters.add(10.0);  // Base temperature
        }
    }
    
    /**
     * Get forecasting algorithm
     * @return Forecasting algorithm
     */
    public ForecastAlgorithm getAlgorithm() {
        return algorithm;
    }
    
    /**
     * Get model parameters
     * @return Model parameters
     */
    public List<Double> getParameters() {
        return parameters;
    }
    
    /**
     * Get model description
     * @return Model description
     */
    public String getModelDescription() {
        return modelDescription;
    }
    
    /**
     * Check if model is trained
     * @return True if trained, false otherwise
     */
    public boolean isTrained() {
        return trained;
    }
    
    /**
     * Train model with historical data
     * @param data List of weather observations
     * @return True if trained successfully, false otherwise
     */
    public boolean train(List<WeatherData> data) {
        if (data == null || data.isEmpty()) {
            return false;
        }
        
        if (algorithm == ForecastAlgorithm.MOVING_AVERAGE) {
            return trainMovingAverage(data);
        } else if (algorithm == ForecastAlgorithm.WEIGHTED_AVERAGE) {
            return trainWeightedAverage(data);
        } else if (algorithm == ForecastAlgorithm.EXPONENTIAL_SMOOTHING) {
            return trainExponentialSmoothing(data);
        } else if (algorithm == ForecastAlgorithm.LINEAR_REGRESSION) {
            return trainLinearRegression(data);
        } else {
            return true;  // For other algorithms, just mark as trained
        }
    }
    
    /**
     * Train moving average model
     * @param data List of weather observations
     * @return True if trained successfully, false otherwise
     */
    private boolean trainMovingAverage(List<WeatherData> data) {
        int period = parameters.get(0).intValue();
        
        if (data.size() < period) {
            return false;
        }
        
        // Calculate moving average for temperature
        double sum = 0.0;
        for (int i = data.size() - period; i < data.size(); i++) {
            sum += data.get(i).getAvgTemperature();
        }
        
        // Update model description
        modelDescription = period + "-day Moving Average for Temperature";
        trained = true;
        return true;
    }
    
    /**
     * Train weighted average model
     * @param data List of weather observations
     * @return True if trained successfully, false otherwise
     */
    private boolean trainWeightedAverage(List<WeatherData> data) {
        double alpha = parameters.get(0);
        
        // Calculate weighted average with more weight on recent data
        double sum = 0.0;
        double sumWeights = 0.0;
        
        for (int i = 0; i < data.size(); i++) {
            double weight = Math.pow(alpha, data.size() - i);
            sum += data.get(i).getAvgTemperature() * weight;
            sumWeights += weight;
        }
        
        if (sumWeights == 0) {
            return false;
        }
        
        // Update model description
        modelDescription = "Exponentially Weighted Moving Average (alpha=" + alpha + ")";
        trained = true;
        return true;
    }
    
    /**
     * Train exponential smoothing model
     * @param data List of weather observations
     * @return True if trained successfully, false otherwise
     */
    private boolean trainExponentialSmoothing(List<WeatherData> data) {
        double alpha = parameters.get(0);
        
        // Simple exponential smoothing implementation
        if (data.isEmpty()) {
            return false;
        }
        
        // Use the first data point as initial smoothed value
        double smoothed = data.get(0).getAvgTemperature();
        double forecastErrorSum = 0.0;
        
        for (int i = 1; i < data.size(); i++) {
            // Calculate smoothed value
            smoothed = alpha * data.get(i).getAvgTemperature() + (1 - alpha) * smoothed;
            
            // Calculate forecast error
            if (i < data.size()) {
                double actual = data.get(i).getAvgTemperature();
                forecastErrorSum += Math.pow(actual - smoothed, 2);
            }
        }
        
        // Update model description
        modelDescription = "Exponential Smoothing (alpha=" + alpha + ")";
        trained = true;
        return true;
    }
    
    /**
     * Train linear regression model
     * @param data List of weather observations
     * @return True if trained successfully, false otherwise
     */
    private boolean trainLinearRegression(List<WeatherData> data) {
        if (data.size() < 10) {
            return false;
        }
        
        // Simplified linear regression: y = a + bx
        // Calculate parameters using least squares method
        int n = data.size();
        double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
        
        for (int i = 0; i < n; i++) {
            double x = i;
            double y = data.get(i).getAvgTemperature();
            sumX += x;
            sumY += y;
            sumXY += x * y;
            sumX2 += x * x;
        }
        
        double meanX = sumX / n;
        double meanY = sumY / n;
        
        double slope = (sumXY - n * meanX * meanY) / (sumX2 - n * meanX * meanX);
        double intercept = meanY - slope * meanX;
        
        // Update parameters
        parameters.clear();
        parameters.add(slope);
        parameters.add(intercept);
        
        // Update model description
        modelDescription = "Linear Regression (y=" + slope + "x + " + intercept + ")";
        trained = true;
        return true;
    }
    
    /**
     * Forecast future weather
     * @param historicalData List of historical observations
     * @param daysAhead Number of days to forecast
     * @return Forecasted temperature
     */
    public double forecastTemperature(List<WeatherData> historicalData, int daysAhead) {
        if (!trained) {
            return Double.NaN;
        }
        
        if (algorithm == ForecastAlgorithm.MOVING_AVERAGE) {
            return forecastMovingAverage(historicalData, daysAhead);
        } else if (algorithm == ForecastAlgorithm.WEIGHTED_AVERAGE) {
            return forecastWeightedAverage(historicalData, daysAhead);
        } else if (algorithm == ForecastAlgorithm.EXPONENTIAL_SMOOTHING) {
            return forecastExponentialSmoothing(historicalData, daysAhead);
        } else if (algorithm == ForecastAlgorithm.LINEAR_REGRESSION) {
            return forecastLinearRegression(historicalData, daysAhead);
        } else {
            // Default to moving average
            return forecastMovingAverage(historicalData, daysAhead);
        }
    }
    
    /**
     * Forecast using moving average
     * @param historicalData List of historical observations
     * @param daysAhead Number of days to forecast
     * @return Forecasted temperature
     */
    private double forecastMovingAverage(List<WeatherData> historicalData, int daysAhead) {
        int period = parameters.get(0).intValue();
        
        if (historicalData.size() < period) {
            return historicalData.get(historicalData.size() - 1).getAvgTemperature();
        }
        
        // Calculate average of last 'period' observations
        double sum = 0.0;
        for (int i = historicalData.size() - period; i < historicalData.size(); i++) {
            sum += historicalData.get(i).getAvgTemperature();
        }
        
        return sum / period;
    }
    
    /**
     * Forecast using weighted average
     * @param historicalData List of historical observations
     * @param daysAhead Number of days to forecast
     * @return Forecasted temperature
     */
    private double forecastWeightedAverage(List<WeatherData> historicalData, int daysAhead) {
        if (historicalData.isEmpty()) {
            return Double.NaN;
        }
        
        double alpha = parameters.get(0);
        double sum = 0.0;
        double sumWeights = 0.0;
        
        // Calculate weighted average with more weight on recent data
        for (int i = 0; i < historicalData.size(); i++) {
            double weight = Math.pow(alpha, historicalData.size() - i - 1);
            sum += historicalData.get(i).getAvgTemperature() * weight;
            sumWeights += weight;
        }
        
        if (sumWeights == 0) {
            return historicalData.get(historicalData.size() - 1).getAvgTemperature();
        }
        
        return sum / sumWeights;
    }
    
    /**
     * Forecast using exponential smoothing
     * @param historicalData List of historical observations
     * @param daysAhead Number of days to forecast
     * @return Forecasted temperature
     */
    private double forecastExponentialSmoothing(List<WeatherData> historicalData, int daysAhead) {
        if (historicalData.isEmpty()) {
            return Double.NaN;
        }
        
        double alpha = parameters.get(0);
        double lastValue = historicalData.get(historicalData.size() - 1).getAvgTemperature();
        
        // Forecast is the last smoothed value
        for (int i = 0; i < daysAhead; i++) {
            lastValue = alpha * lastValue + (1 - alpha) * lastValue;
        }
        
        return lastValue;
    }
    
    /**
     * Forecast using linear regression
     * @param historicalData List of historical observations
     * @param daysAhead Number of days to forecast
     * @return Forecasted temperature
     */
    private double forecastLinearRegression(List<WeatherData> historicalData, int daysAhead) {
        if (historicalData.size() < 2) {
            return historicalData.get(historicalData.size() - 1).getAvgTemperature();
        }
        
        double slope = parameters.get(0);
        double intercept = parameters.get(1);
        
        // Forecast the next 'daysAhead' values
        double x = historicalData.size() + daysAhead;
        return slope * x + intercept;
    }
    
    /**
     * Get model summary
     * @return Model summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Algorithm: ").append(algorithm).append("\n");
        sb.append("Description: ").append(modelDescription).append("\n");
        sb.append("Parameters: ");
        
        for (Double param : parameters) {
            sb.append(param).append(" ");
        }
        
        sb.append("\n");
        sb.append("Trained: ").append(trained ? "Yes" : "No").append("\n");
        
        return sb.toString();
    }
}

/**
 * WeatherForecastingSystem class manages the entire weather forecasting system
 */
public class WeatherForecastingSystem {
    private String systemName;
    private String location;
    private List<WeatherData> weatherData;
    private Map<ForecastAlgorithm, ForecastingModel> models;
    private ForecastAlgorithm currentAlgorithm;
    private ForecastingModel currentModel;
    private List<String> availableAlgorithms;
    private List<String> availableLocations;
    
    /**
     * Constructor for WeatherForecastingSystem
     * @param systemName System name
     * @param location Weather station location
     */
    public WeatherForecastingSystem(String systemName, String location) {
        this.systemName = systemName;
        this.location = location;
        this.weatherData = new ArrayList<>();
        this.models = new HashMap<>();
        this.availableAlgorithms = new ArrayList<>();
        this.availableLocations = new ArrayList<>();
        
        // Initialize available algorithms
        availableAlgorithms.add("Moving Average");
        availableAlgorithms.add("Weighted Average");
        availableAlgorithms.add("Exponential Smoothing");
        availableAlgorithms.add("Linear Regression");
        availableAlgorithms.add("Seasonal Decomposition");
        
        // Initialize available locations
        availableLocations.add("New York");
        availableLocations.add("Los Angeles");
        availableLocations.add("Chicago");
        availableLocations.add("London");
        availableLocations.add("Tokyo");
        availableLocations.add("Sydney");
        availableLocations.add("Shanghai");
        availableLocations.add("Beijing");
        availableLocations.add("Mumbai");
    }
    
    /**
     * Get system name
     * @return System name
     */
    public String getSystemName() {
        return systemName;
    }
    
    /**
     * Get location
     * @return Weather station location
     */
    public String getLocation() {
        return location;
    }
    
    /**
     * Get available algorithms
     * @return List of available algorithms
     */
    public List<String> getAvailableAlgorithms() {
        return availableAlgorithms;
    }
    
    /**
     * Get available locations
     * @return List of available locations
     */
    public List<String> getAvailableLocations() {
        return availableLocations;
    }
    
    /**
     * Add weather data
     * @param data Weather observation to add
     * @return True if added successfully, false otherwise
     */
    public boolean addWeatherData(WeatherData data) {
        if (data == null) {
            return false;
        }
        
        weatherData.add(data);
        return true;
    }
    
    /**
     * Get weather data
     * @return List of weather observations
     */
    public List<WeatherData> getWeatherData() {
        return weatherData;
    }
    
    /**
     * Get weather data by date range
     * @param startDate Start date
     * @param endDate End date
     * @return List of weather observations in range
     */
    public List<WeatherData> getWeatherDataByDateRange(Date startDate, Date endDate) {
        List<WeatherData> result = new ArrayList<>();
        
        for (WeatherData data : weatherData) {
            Date dataDate = data.getDate();
            
            // Check if date is within range
            if (!dataDate.before(startDate) && !dataDate.after(endDate)) {
                result.add(data);
            }
        }
        
        return result;
    }
    
    /**
     * Get weather data by location
     * @param location Location to filter by
     * @return List of weather observations for location
     */
    public List<WeatherData> getWeatherDataByLocation(String location) {
        List<WeatherData> result = new ArrayList<>();
        
        // In a real implementation, you would have location field in WeatherData
        // For this example, we'll return all data
        
        return weatherData;
    }
    
    /**
     * Create a forecasting model
     * @param algorithm Forecasting algorithm
     * @param description Model description
     * @return Created model
     */
    public ForecastingModel createModel(ForecastAlgorithm algorithm, String description) {
        ForecastingModel model = new ForecastingModel(algorithm, description);
        models.put(algorithm, model);
        return model;
    }
    
    /**
     * Get a model by algorithm
     * @param algorithm Forecasting algorithm
     * @return Model if found, null otherwise
     */
    public ForecastingModel getModel(ForecastAlgorithm algorithm) {
        return models.get(algorithm);
    }
    
    /**
     * Train a model
     * @param model Model to train
     * @param data Weather data for training
     * @return True if trained successfully, false otherwise
     */
    public boolean trainModel(ForecastingModel model, List<WeatherData> data) {
        return model.train(data);
    }
    
    /**
     * Set current algorithm
     * @param algorithm Forecasting algorithm
     * @return True if set successfully, false otherwise
     */
    public boolean setCurrentAlgorithm(ForecastAlgorithm algorithm) {
        if (!availableAlgorithms.contains(algorithm)) {
            return false;
        }
        
        this.currentAlgorithm = algorithm;
        
        // Set the corresponding model as current
        this.currentModel = getModel(algorithm);
        
        return true;
    }
    
    /**
     * Get current algorithm
     * @return Current forecasting algorithm
     */
    public ForecastAlgorithm getCurrentAlgorithm() {
        return currentAlgorithm;
    }
    
    /**
     * Get current model
     * @return Current forecasting model
     */
    public ForecastingModel getCurrentModel() {
        return currentModel;
    }
    
    /**
     * Forecast weather
     * @param daysAhead Number of days to forecast
     * @return Forecasted temperature
     */
    public double forecastTemperature(int daysAhead) {
        if (currentModel == null || !currentModel.isTrained()) {
            return Double.NaN;
        }
        
        return currentModel.forecastTemperature(weatherData, daysAhead);
    }
    
    /**
     * Evaluate model accuracy
     * @param actualData Actual weather data
     * @param forecastData Forecasted weather data
     * @param daysAhead Number of days ahead for forecast
     * @return Mean absolute error
     */
    public double evaluateModelAccuracy(List<WeatherData> actualData, List<Double> forecastData, int daysAhead) {
        if (actualData.size() != forecastData.size()) {
            return Double.NaN;
        }
        
        double totalError = 0.0;
        int validForecasts = 0;
        
        for (int i = daysAhead; i < actualData.size() - daysAhead; i++) {
            double actual = actualData.get(i + daysAhead).getAvgTemperature();
            double forecast = forecastData.get(i);
            
            // Calculate absolute error
            double error = Math.abs(actual - forecast);
            
            // Only count non-NaN forecasts
            if (!Double.isNaN(forecast)) {
                totalError += error;
                validForecasts++;
            }
        }
        
        if (validForecasts == 0) {
            return Double.NaN;
        }
        
        return totalError / validForecasts;
    }
    
    /**
     * Get seasonal pattern
     * @param month Month to analyze (1-12)
     * @return Seasonal temperature pattern
     */
    public Map<String, List<Double>> getSeasonalPattern(int month) {
        Map<String, List<Double>> pattern = new HashMap<>();
        pattern.put("Winter", new ArrayList<>());
        pattern.put("Spring", new ArrayList<>());
        pattern.put("Summer", new ArrayList<>());
        pattern.put("Autumn", new ArrayList<>());
        pattern.put("Winter", new ArrayList<>());
        
        // Define season months
        List<Integer> winterMonths = Arrays.asList(12, 1, 2);
        List<Integer> springMonths = Arrays.asList(3, 4, 5);
        List<Integer> summerMonths = Arrays.asList(6, 7, 8);
        List<Integer> autumnMonths = Arrays.asList(9, 10, 11);
        
        String season = "";
        if (winterMonths.contains(month)) {
            season = "Winter";
        } else if (springMonths.contains(month)) {
            season = "Spring";
        } else if (summerMonths.contains(month)) {
            season = "Summer";
        } else if (autumnMonths.contains(month)) {
            season = "Autumn";
        } else {
            season = "Winter";
        }
        
        List<Double> seasonTemperatures = pattern.get(season);
        
        for (WeatherData data : weatherData) {
            Calendar cal = Calendar.getInstance();
            cal.setTime(data.getDate());
            int dataMonth = cal.get(Calendar.MONTH) + 1;
            
            if (seasonMonths.contains(dataMonth)) {
                seasonTemperatures.add(data.getAvgTemperature());
            }
        }
        
        return pattern;
    }
    
    /**
     * Get temperature trend
     * @param days Number of recent days to analyze
     * @return Temperature trend (warming, cooling, stable)
     */
    public String getTemperatureTrend(int days) {
        if (weatherData.size() < days) {
            return "Insufficient data for trend analysis";
        }
        
        double recentTemp = weatherData.get(weatherData.size() - 1).getAvgTemperature();
        double olderTemp = weatherData.get(weatherData.size() - days).getAvgTemperature();
        
        if (Math.abs(recentTemp - olderTemp) < 0.5) {
            return "Cooling";
        } else if (Math.abs(recentTemp - olderTemp) > 0.5) {
            return "Warming";
        } else {
            return "Stable";
        }
    }
    
    /**
     * Generate weather forecast report
     * @return Forecast report string
     */
    public String generateForecastReport() {
        StringBuilder sb = new StringBuilder();
        sb.append("=====================================\n");
        sb.append("      WEATHER FORECAST REPORT\n");
        sb.append("=====================================\n");
        sb.append("System: ").append(systemName).append("\n");
        sb.append("Location: ").append(location).append("\n");
        sb.append("Report Date: ").append(new Date()).append("\n\n");
        
        sb.append("DATA SUMMARY\n");
        sb.append("------------\n");
        sb.append("Total Observations: ").append(weatherData.size()).append("\n");
        sb.append("Date Range: ");
        
        if (!weatherData.isEmpty()) {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
            sb.append(sdf.format(weatherData.get(0).getDate()))
              .append(" to ")
              .append(sdf.format(weatherData.get(weatherData.size() - 1).getDate())).append("\n");
        } else {
            sb.append("N/A\n");
        }
        
        sb.append("\n");
        sb.append("Current Algorithm: ").append(currentAlgorithm).append("\n");
        sb.append("Current Model: ");
        
        if (currentModel != null) {
            sb.append(currentModel.getSummary()).append("\n");
        } else {
            sb.append("No model trained\n");
        }
        
        sb.append("\n");
        
        // Generate forecasts for the next 7 days
        sb.append("7-DAY FORECAST\n");
        sb.append("---------------\n");
        
        if (currentModel != null && currentModel.isTrained()) {
            List<Double> forecasts = new ArrayList<>();
            
            for (int i = 1; i <= 7; i++) {
                double forecast = forecastTemperature(i);
                forecasts.add(forecast);
                
                Date forecastDate = new Date();
                Calendar cal = Calendar.getInstance();
                cal.add(Calendar.DAY_OF_MONTH, (new Date().getMonth() + 1) % 12);
                cal.add(Calendar.DAY_OF_YEAR, new Date().getYear());
                
                String dateStr = i + " days from now";
                sb.append("Day ").append(i).append(": ")
                      .append(dateStr).append(" -> ")
                      .append(String.format("%.1f°C", forecast))
                      .append("\n");
            }
        }
        
        sb.append("\n");
        sb.append("MODEL PERFORMANCE\n");
        sb.append("---------------\n");
        
        // Evaluate model performance
        List<Double> actualTemps = new ArrayList<>();
        List<Double> forecastTemps = new ArrayList<>();
        
        for (int i = 7; i < weatherData.size() - 7; i++) {
            actualTemps.add(weatherData.get(i).getAvgTemperature());
            
            if (i < weatherData.size() - 7) {
                forecastTemps.add(forecasts.get(i - 7));
            }
        }
        
        if (!actualTemps.isEmpty() && !forecastTemps.isEmpty()) {
            double mae = evaluateModelAccuracy(actualTemps, forecastTemps, 7);
            sb.append("Mean Absolute Error: ").append(String.format("%.2f°C", mae)).append("\n");
        }
        
        // Get seasonal patterns
        Map<String, List<Double>> seasonalPatterns = getSeasonalPattern(new Date().getMonth() + 1);
        sb.append("\n");
        
        for (Map.Entry<String, List<Double>> entry : seasonalPatterns.entrySet()) {
            sb.append(entry.getKey()).append(" Pattern: ");
            
            List<Double> temps = entry.getValue();
            double sum = 0.0;
            for (Double temp : temps) {
                sum += temp;
            }
            
            double avgTemp = sum / temps.size();
            sb.append(String.format("Average: %.1f°C\n", avgTemp));
        }
        
        return sb.toString();
    }
    
    /**
     * Save system data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("WEATHER FORECASTING DATA EXPORT");
            writer.println("=====================================");
            writer.println("System: " + systemName);
            writer.println("Location: " + location);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export weather data
            writer.println("WEATHER OBSERVATIONS");
            writer.println("--------------");
            for (WeatherData data : weatherData) {
                writer.println(data.getSummary());
                writer.println();
            }
            
            // Export models
            writer.println("FORECASTING MODELS");
            writer.println("----------------");
            for (ForecastingModel model : models.values()) {
                writer.println("Algorithm: " + model.getAlgorithm());
                writer.println(model.getSummary());
                writer.println();
            }
            
            writer.close();
            return true;
        } catch (IOException e) {
            System.err.println("Error saving to file: " + e.getMessage());
            return false;
        }
    }
    
    /**
     * Main method for demonstration
     */
    public static void main(String[] args) {
        // Create a new weather forecasting system
        WeatherForecastingSystem system = new WeatherForecastingSystem("Weather Pro", "New York, USA");
        
        // Create sample weather data
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Calendar cal = Calendar.getInstance();
        
        // Generate 30 days of sample weather data
        for (int i = 0; i < 30; i++) {
            cal.add(Calendar.DAY_OF_MONTH, 1);
            Date date = cal.getTime();
            
            // Generate realistic weather patterns with seasonality
            int dayOfYear = cal.get(Calendar.DAY_OF_YEAR);
            int month = cal.get(Calendar.MONTH);
            
            double baseTemp = 15.0;  // Base temperature
            double seasonalVariation = 10.0;  // Seasonal variation
            
            // Add seasonal effect
            if (month >= 12 || month <= 2) {  // Winter
                baseTemp = 5.0;
                seasonalVariation = 5.0;
            } else if (month >= 3 && month <= 5) {  // Spring
                baseTemp = 15.0;
                seasonalVariation = 5.0;
            } else if (month >= 6 && month <= 8) {  // Summer
                baseTemp = 25.0;
                seasonalVariation = 5.0;
            } else if (month >= 9 && month <= 11) {  // Autumn
                baseTemp = 15.0;
                seasonalVariation = 5.0;
            }
            
            // Add random variation
            Random random = new Random();
            double variation = random.nextGaussian() * 2.0;
            
            double maxTemp = baseTemp + seasonalVariation + variation;
            double minTemp = baseTemp - seasonalVariation + variation;
            double avgTemp = (maxTemp + minTemp) / 2;
            
            // Other weather parameters
            double maxHumid = 80 + random.nextGaussian() * 20;
            double minHumid = Math.max(30, maxHumid - random.nextGaussian() * 30);
            double avgHumid = (maxHumid + minHumid) / 2;
            double precipitation = random.nextDouble() * 10;
            double windSpeed = random.nextDouble() * 30;
            String windDirection = Arrays.asList("N", "NE", "E", "SE", "S", "W", "NW").get(random.nextInt(8));
            double pressure = 1013 + random.nextGaussian() * 20;
            
            // Weather condition based on temperature
            String condition;
            if (avgTemp < 0) {
                condition = "Snow";
            } else if (avgTemp < 10) {
                condition = "Cloudy";
            } else if (avgTemp < 20) {
                condition = "Rainy";
            } else {
                condition = "Sunny";
            }
            
            String description = generateWeatherDescription(condition, windSpeed, precipitation);
            
            // Create weather data
            WeatherData weather = new WeatherData(
                date, maxTemp, minTemp, avgTemp,
                maxHumid, minHumid, avgHumid,
                precipitation, windSpeed, windDirection, pressure, condition, description
            );
            
            system.addWeatherData(weather);
        }
        
        // Create forecasting models
        system.createModel(ForecastAlgorithm.MOVING_AVERAGE, "7-Day Moving Average");
        system.createModel(ForecastAlgorithm.WEIGHTED_AVERAGE, "7-Day Weighted Average");
        system.createModel(ForecastAlgorithm.EXPONENTIAL_SMOOTHING, "Exponential Smoothing (alpha=0.3)");
        system.createModel(ForecastAlgorithm.LINEAR_REGRESSION, "Trend Analysis");
        
        // Train models
        system.setCurrentAlgorithm(ForecastAlgorithm.MOVING_AVERAGE);
        system.trainModel(system.getCurrentModel(), system.getWeatherData());
        
        // Generate and print forecast report
        String report = system.generateForecastReport();
        System.out.println(report);
        
        // Save data to file
        system.saveToFile("weather_forecast_data_export.txt");
        
        System.out.println("\nWeather Forecasting System demonstration completed successfully!");
    }
    
    /**
     * Generate weather description
     * @param condition Weather condition
     * @param windSpeed Wind speed
     * @param precipitation Precipitation
     * @return Description
     */
    private static String generateWeatherDescription(String condition, double windSpeed, double precipitation) {
        String desc = condition.toLowerCase() + " with ";
        
        if (precipitation > 0.1) {
            desc += "heavy rain";
        } else if (precipitation > 0.01) {
            desc += "light rain";
        }
        
        if (windSpeed > 20.0) {
            desc += "strong wind";
        } else if (windSpeed > 10.0) {
            desc += "moderate wind";
        } else if (windSpeed > 5.0) {
            desc += "light wind";
        } else if (windSpeed > 0) {
            desc += "calm wind";
        }
        else {
            desc += "calm conditions";
        }
        
        if (precipitation == 0 && windSpeed < 1.0) {
            desc += "and clear skies";
        }
        
        return desc;
    }
}
<｜fim▁hole｜>