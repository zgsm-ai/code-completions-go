#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <memory>
#include <cmath>
#include <queue>
#include <stack>
#include <limits>
#include <random>

using namespace std;

#define MAX_VEHICLES 1000
#define MAX_DRIVERS 500
#define MAX_ROUTES 100
#define MAX_BOOKINGS 5000
#define MAX_MAINTENANCE_RECORDS 1000

struct date {
    int year;
    int month;
    int day;
};

struct time {
    int hour;
    int minute;
};

class Vehicle;
class Driver;
class Route;
class Booking;
class MaintenanceRecord;

enum VehicleType {
    SEDAN = 1,
    SUV = 2,
    VAN = 3,
    BUS = 4,
    TRUCK = 5,
    MOTORCYCLE = 6
};

enum VehicleStatus {
    AVAILABLE = 1,
    IN_USE = 2,
    MAINTENANCE = 3,
    OUT_OF_SERVICE = 4
};

enum BookingStatus {
    SCHEDULED = 1,
    IN_PROGRESS = 2,
    COMPLETED = 3,
    CANCELLED = 4,
    NO_SHOW = 5
};

class Vehicle {
private:
    int vehicleId;
    string licensePlate;
    string make;
    string model;
    int year;
    VehicleType type;
    int capacity;
    double fuelEfficiency; // km per liter
    double odometer;
    string color;
    VehicleStatus status;
    double dailyRate;
    double weeklyRate;
    date lastMaintenanceDate;
    vector<int> bookingIds;
    vector<int> maintenanceRecordIds;
    
public:
    Vehicle(int vehicleId = 0, const string& licensePlate = "", const string& make = "",
            const string& model = "", int year = 0, VehicleType type = SEDAN,
            int capacity = 4, double fuelEfficiency = 15.0, double odometer = 0.0,
            const string& color = "", VehicleStatus status = AVAILABLE,
            double dailyRate = 50.0, double weeklyRate = 300.0)
        : vehicleId(vehicleId), licensePlate(licensePlate), make(make), model(model),
          year(year), type(type), capacity(capacity), fuelEfficiency(fuelEfficiency),
          odometer(odometer), color(color), status(status), dailyRate(dailyRate),
          weeklyRate(weeklyRate) {
        lastMaintenanceDate = {0, 0, 0};
    }
    
    int getVehicleId() const { return vehicleId; }
    string getLicensePlate() const { return licensePlate; }
    string getMake() const { return make; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    VehicleType getType() const { return type; }
    int getCapacity() const { return capacity; }
    double getFuelEfficiency() const { return fuelEfficiency; }
    double getOdometer() const { return odometer; }
    string getColor() const { return color; }
    VehicleStatus getStatus() const { return status; }
    double getDailyRate() const { return dailyRate; }
    double getWeeklyRate() const { return weeklyRate; }
    date getLastMaintenanceDate() const { return lastMaintenanceDate; }
    vector<int> getBookingIds() const { return bookingIds; }
    vector<int> getMaintenanceRecordIds() const { return maintenanceRecordIds; }
    
    void setVehicleId(int id) { vehicleId = id; }
    void setLicensePlate(const string& plate) { licensePlate = plate; }
    void setMake(const string& make) { this->make = make; }
    void setModel(const string& model) { this->model = model; }
    void setYear(int year) { this->year = year; }
    void setType(VehicleType type) { this->type = type; }
    void setCapacity(int capacity) { this->capacity = capacity; }
    void setFuelEfficiency(double efficiency) { fuelEfficiency = efficiency; }
    void setOdometer(double odometer) { this->odometer = odometer; }
    void setColor(const string& color) { this->color = color; }
    void setStatus(VehicleStatus status) { this->status = status; }
    void setDailyRate(double rate) { dailyRate = rate; }
    void setWeeklyRate(double rate) { weeklyRate = rate; }
    void setLastMaintenanceDate(date date) { lastMaintenanceDate = date; }
    
    void addBooking(int bookingId) {
        if (find(bookingIds.begin(), bookingIds.end(), bookingId) == bookingIds.end()) {
            bookingIds.push_back(bookingId);
        }
    }
    
    void removeBooking(int bookingId) {
        auto it = find(bookingIds.begin(), bookingIds.end(), bookingId);
        if (it != bookingIds.end()) {
            bookingIds.erase(it);
        }
    }
    
    void addMaintenanceRecord(int recordId) {
        if (find(maintenanceRecordIds.begin(), maintenanceRecordIds.end(), recordId) == maintenanceRecordIds.end()) {
            maintenanceRecordIds.push_back(recordId);
        }
    }
    
    void removeMaintenanceRecord(int recordId) {
        auto it = find(maintenanceRecordIds.begin(), maintenanceRecordIds.end(), recordId);
        if (it != maintenanceRecordIds.end()) {
            maintenanceRecordIds.erase(it);
        }
    }
    
    string getTypeString() const {
        switch (type) {
            case SEDAN: return "Sedan";
            case SUV: return "SUV";
            case VAN: return "Van";
            case BUS: return "Bus";
            case TRUCK: return "Truck";
            case MOTORCYCLE: return "Motorcycle";
            default: return "Unknown";
        }
    }
    
    string getStatusString() const {
        switch (status) {
            case AVAILABLE: return "Available";
            case IN_USE: return "In Use";
            case MAINTENANCE: return "Maintenance";
            case OUT_OF_SERVICE: return "Out of Service";
            default: return "Unknown";
        }
    }
    
    bool needsMaintenance() const {
        // Calculate days since last maintenance
        if (lastMaintenanceDate.year == 0) {
            return true; // Never had maintenance
        }
        
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        // Simple calculation - not accounting for leap years or month lengths
        int days = (today.year - lastMaintenanceDate.year) * 365;
        days += (today.month - lastMaintenanceDate.month) * 30;
        days += today.day - lastMaintenanceDate.day;
        
        return days > 90; // Needs maintenance every 90 days
    }
    
    void display() const {
        cout << "Vehicle ID: " << vehicleId << endl;
        cout << "License Plate: " << licensePlate << endl;
        cout << "Make: " << make << endl;
        cout << "Model: " << model << endl;
        cout << "Year: " << year << endl;
        cout << "Type: " << getTypeString() << endl;
        cout << "Capacity: " << capacity << " passengers" << endl;
        cout << "Fuel Efficiency: " << fuelEfficiency << " km/L" << endl;
        cout << "Odometer: " << fixed << setprecision(1) << odometer << " km" << endl;
        cout << "Color: " << color << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Daily Rate: $" << fixed << setprecision(2) << dailyRate << endl;
        cout << "Weekly Rate: $" << fixed << setprecision(2) << weeklyRate << endl;
        
        if (lastMaintenanceDate.year > 0) {
            // Format date
            tm tm = {};
            tm.tm_year = lastMaintenanceDate.year - 1900;
            tm.tm_mon = lastMaintenanceDate.month - 1;
            tm.tm_mday = lastMaintenanceDate.day;
            
            char dateStr[100];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Last Maintenance: " << dateStr << endl;
        } else {
            cout << "Last Maintenance: Never" << endl;
        }
        
        if (needsMaintenance()) {
            cout << "WARNING: Vehicle needs maintenance!" << endl;
        }
        
        cout << "Bookings: " << bookingIds.size() << endl;
        cout << "Maintenance Records: " << maintenanceRecordIds.size() << endl;
    }
};

class Driver {
private:
    int driverId;
    string firstName;
    string lastName;
    string licenseNumber;
    date licenseExpiryDate;
    string address;
    string phone;
    string email;
    date hireDate;
    bool isActive;
    vector<int> bookingIds;
    
public:
    Driver(int driverId = 0, const string& firstName = "", const string& lastName = "",
          const string& licenseNumber = "", date licenseExpiryDate = {0, 0, 0},
          const string& address = "", const string& phone = "", const string& email = "",
          date hireDate = {0, 0, 0}, bool isActive = true)
        : driverId(driverId), firstName(firstName), lastName(lastName), licenseNumber(licenseNumber),
          licenseExpiryDate(licenseExpiryDate), address(address), phone(phone), email(email),
          hireDate(hireDate), isActive(isActive) {}
    
    int getDriverId() const { return driverId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getLicenseNumber() const { return licenseNumber; }
    date getLicenseExpiryDate() const { return licenseExpiryDate; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    date getHireDate() const { return hireDate; }
    bool getIsActive() const { return isActive; }
    vector<int> getBookingIds() const { return bookingIds; }
    
    void setDriverId(int id) { driverId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setLicenseNumber(const string& number) { licenseNumber = number; }
    void setLicenseExpiryDate(date date) { licenseExpiryDate = date; }
    void setAddress(const string& address) { this->address = address; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setEmail(const string& email) { this->email = email; }
    void setHireDate(date date) { hireDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    
    void addBooking(int bookingId) {
        if (find(bookingIds.begin(), bookingIds.end(), bookingId) == bookingIds.end()) {
            bookingIds.push_back(bookingId);
        }
    }
    
    void removeBooking(int bookingId) {
        auto it = find(bookingIds.begin(), bookingIds.end(), bookingId);
        if (it != bookingIds.end()) {
            bookingIds.erase(it);
        }
    }
    
    bool isLicenseExpired() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        if (licenseExpiryDate.year > today.year) {
            return false;
        } else if (licenseExpiryDate.year == today.year && licenseExpiryDate.month > today.month) {
            return false;
        } else if (licenseExpiryDate.year == today.year && licenseExpiryDate.month == today.month && 
                   licenseExpiryDate.day >= today.day) {
            return false;
        }
        
        return true; // License is expired or expiry date is in the past
    }
    
    int getYearsOfService() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        int years = today.year - hireDate.year;
        if (today.month < hireDate.month || 
            (today.month == hireDate.month && today.day < hireDate.day)) {
            years--;
        }
        
        return years;
    }
    
    void display() const {
        cout << "Driver ID: " << driverId << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "License Number: " << licenseNumber << endl;
        
        // Format license expiry date
        tm tm = {};
        tm.tm_year = licenseExpiryDate.year - 1900;
        tm.tm_mon = licenseExpiryDate.month - 1;
        tm.tm_mday = licenseExpiryDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "License Expiry: " << dateStr;
        
        if (isLicenseExpired()) {
            cout << " (EXPIRED)" << endl;
        } else {
            cout << " (Valid)" << endl;
        }
        
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        
        // Format hire date
        tm.tm_year = hireDate.year - 1900;
        tm.tm_mon = hireDate.month - 1;
        tm.tm_mday = hireDate.day;
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Hire Date: " << dateStr << endl;
        
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Years of Service: " << getYearsOfService() << endl;
        cout << "Bookings: " << bookingIds.size() << endl;
    }
};

class Route {
private:
    int routeId;
    string name;
    string description;
    string origin;
    string destination;
    double distance; // in km
    double estimatedTime; // in hours
    double baseFare;
    vector<int> bookingIds;
    
public:
    Route(int routeId = 0, const string& name = "", const string& description = "",
          const string& origin = "", const string& destination = "",
          double distance = 0.0, double estimatedTime = 0.0, double baseFare = 10.0)
        : routeId(routeId), name(name), description(description), origin(origin),
          destination(destination), distance(distance), estimatedTime(estimatedTime), baseFare(baseFare) {}
    
    int getRouteId() const { return routeId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    string getOrigin() const { return origin; }
    string getDestination() const { return destination; }
    double getDistance() const { return distance; }
    double getEstimatedTime() const { return estimatedTime; }
    double getBaseFare() const { return baseFare; }
    vector<int> getBookingIds() const { return bookingIds; }
    
    void setRouteId(int id) { routeId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setOrigin(const string& origin) { this->origin = origin; }
    void setDestination(const string& destination) { this->destination = destination; }
    void setDistance(double distance) { this->distance = distance; }
    void setEstimatedTime(double time) { this->estimatedTime = time; }
    void setBaseFare(double fare) { this->baseFare = fare; }
    
    void addBooking(int bookingId) {
        if (find(bookingIds.begin(), bookingIds.end(), bookingId) == bookingIds.end()) {
            bookingIds.push_back(bookingId);
        }
    }
    
    void removeBooking(int bookingId) {
        auto it = find(bookingIds.begin(), bookingIds.end(), bookingId);
        if (it != bookingIds.end()) {
            bookingIds.erase(it);
        }
    }
    
    void display() const {
        cout << "Route ID: " << routeId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Origin: " << origin << endl;
        cout << "Destination: " << destination << endl;
        cout << "Distance: " << fixed << setprecision(1) << distance << " km" << endl;
        cout << "Estimated Time: " << fixed << setprecision(1) << estimatedTime << " hours" << endl;
        cout << "Base Fare: $" << fixed << setprecision(2) << baseFare << endl;
        cout << "Bookings: " << bookingIds.size() << endl;
    }
};

class Booking {
private:
    int bookingId;
    int vehicleId;
    int driverId;
    int routeId;
    date bookingDate;
    time pickupTime;
    time returnTime;
    string customerName;
    string customerPhone;
    string customerEmail;
    int passengers;
    double totalAmount;
    BookingStatus status;
    string specialRequests;
    date creationDate;
    
public:
    Booking(int bookingId = 0, int vehicleId = 0, int driverId = 0, int routeId = 0,
           date bookingDate = {0, 0, 0}, time pickupTime = {0, 0},
           time returnTime = {0, 0}, const string& customerName = "",
           const string& customerPhone = "", const string& customerEmail = "",
           int passengers = 1, double totalAmount = 0.0,
           BookingStatus status = SCHEDULED, const string& specialRequests = "",
           date creationDate = {0, 0, 0})
        : bookingId(bookingId), vehicleId(vehicleId), driverId(driverId), routeId(routeId),
          bookingDate(bookingDate), pickupTime(pickupTime), returnTime(returnTime),
          customerName(customerName), customerPhone(customerPhone), customerEmail(customerEmail),
          passengers(passengers), totalAmount(totalAmount), status(status),
          specialRequests(specialRequests), creationDate(creationDate) {}
    
    int getBookingId() const { return bookingId; }
    int getVehicleId() const { return vehicleId; }
    int getDriverId() const { return driverId; }
    int getRouteId() const { return routeId; }
    date getBookingDate() const { return bookingDate; }
    time getPickupTime() const { return pickupTime; }
    time getReturnTime() const { return returnTime; }
    string getCustomerName() const { return customerName; }
    string getCustomerPhone() const { return customerPhone; }
    string getCustomerEmail() const { return customerEmail; }
    int getPassengers() const { return passengers; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }
    string getSpecialRequests() const { return specialRequests; }
    date getCreationDate() const { return creationDate; }
    
    void setBookingId(int id) { bookingId = id; }
    void setVehicleId(int vehicleId) { this->vehicleId = vehicleId; }
    void setDriverId(int driverId) { this->driverId = driverId; }
    void setRouteId(int routeId) { this->routeId = routeId; }
    void setBookingDate(date date) { bookingDate = date; }
    void setPickupTime(time time) { pickupTime = time; }
    void setReturnTime(time time) { returnTime = time; }
    void setCustomerName(const string& name) { customerName = name; }
    void setCustomerPhone(const string& phone) { customerPhone = phone; }
    void setCustomerEmail(const string& email) { customerEmail = email; }
    void setPassengers(int passengers) { this->passengers = passengers; }
    void setTotalAmount(double amount) { totalAmount = amount; }
    void setStatus(BookingStatus status) { this->status = status; }
    void setSpecialRequests(const string& requests) { specialRequests = requests; }
    void setCreationDate(date date) { creationDate = date; }
    
    string getStatusString() const {
        switch (status) {
            case SCHEDULED: return "Scheduled";
            case IN_PROGRESS: return "In Progress";
            case COMPLETED: return "Completed";
            case CANCELLED: return "Cancelled";
            case NO_SHOW: return "No Show";
            default: return "Unknown";
        }
    }
    
    bool isToday() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        return (bookingDate.year == today.year && 
                bookingDate.month == today.month && 
                bookingDate.day == today.day);
    }
    
    bool isPast() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        if (bookingDate.year > today.year) {
            return false;
        } else if (bookingDate.year == today.year && bookingDate.month > today.month) {
            return false;
        } else if (bookingDate.year == today.year && bookingDate.month == today.month && 
                   bookingDate.day > today.day) {
            return false;
        }
        
        return true; // Booking date is in the past
    }
    
    bool isFuture() const {
        return !isToday() && !isPast();
    }
    
    void display() const {
        cout << "Booking ID: " << bookingId << endl;
        cout << "Vehicle ID: " << vehicleId << endl;
        cout << "Driver ID: " << driverId << endl;
        cout << "Route ID: " << routeId << endl;
        
        // Format booking date
        tm tm = {};
        tm.tm_year = bookingDate.year - 1900;
        tm.tm_mon = bookingDate.month - 1;
        tm.tm_mday = bookingDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Booking Date: " << dateStr << endl;
        
        cout << "Pickup Time: " << setfill('0') << setw(2) << pickupTime.hour 
             << ":" << setw(2) << pickupTime.minute << endl;
        
        cout << "Return Time: " << setfill('0') << setw(2) << returnTime.hour 
             << ":" << setw(2) << returnTime.minute << endl;
        
        cout << "Customer Name: " << customerName << endl;
        cout << "Customer Phone: " << customerPhone << endl;
        cout << "Customer Email: " << customerEmail << endl;
        cout << "Passengers: " << passengers << endl;
        cout << "Total Amount: $" << fixed << setprecision(2) << totalAmount << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Special Requests: " << specialRequests << endl;
        
        // Format creation date
        tm.tm_year = creationDate.year - 1900;
        tm.tm_mon = creationDate.month - 1;
        tm.tm_mday = creationDate.day;
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Creation Date: " << dateStr << endl;
    }
};

class MaintenanceRecord {
private:
    int recordId;
    int vehicleId;
    date maintenanceDate;
    string description;
    double cost;
    int odometerReading;
    string mechanicName;
    bool isWarrantyWork;
    
public:
    MaintenanceRecord(int recordId = 0, int vehicleId = 0, date maintenanceDate = {0, 0, 0},
                 const string& description = "", double cost = 0.0, int odometerReading = 0,
                 const string& mechanicName = "", bool isWarrantyWork = false)
        : recordId(recordId), vehicleId(vehicleId), maintenanceDate(maintenanceDate),
          description(description), cost(cost), odometerReading(odometerReading),
          mechanicName(mechanicName), isWarrantyWork(isWarrantyWork) {}
    
    int getRecordId() const { return recordId; }
    int getVehicleId() const { return vehicleId; }
    date getMaintenanceDate() const { return maintenanceDate; }
    string getDescription() const { return description; }
    double getCost() const { return cost; }
    int getOdometerReading() const { return odometerReading; }
    string getMechanicName() const { return mechanicName; }
    bool getIsWarrantyWork() const { return isWarrantyWork; }
    
    void setRecordId(int id) { recordId = id; }
    void setVehicleId(int vehicleId) { this->vehicleId = vehicleId; }
    void setMaintenanceDate(date date) { maintenanceDate = date; }
    void setDescription(const string& description) { this->description = description; }
    void setCost(double cost) { this->cost = cost; }
    void setOdometerReading(int reading) { odometerReading = reading; }
    void setMechanicName(const string& name) { mechanicName = name; }
    void setIsWarrantyWork(bool isWarrantyWork) { this->isWarrantyWork = isWarrantyWork; }
    
    void display() const {
        cout << "Maintenance Record ID: " << recordId << endl;
        cout << "Vehicle ID: " << vehicleId << endl;
        
        // Format maintenance date
        tm tm = {};
        tm.tm_year = maintenanceDate.year - 1900;
        tm.tm_mon = maintenanceDate.month - 1;
        tm.tm_mday = maintenanceDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Maintenance Date: " << dateStr << endl;
        
        cout << "Description: " << description << endl;
        cout << "Cost: $" << fixed << setprecision(2) << cost << endl;
        cout << "Odometer Reading: " << odometerReading << " km" << endl;
        cout << "Mechanic: " << mechanicName << endl;
        cout << "Warranty Work: " << (isWarrantyWork ? "Yes" : "No") << endl;
    }
};

class FleetManagementSystem {
private:
    vector<Vehicle> vehicles;
    vector<Driver> drivers;
    vector<Route> routes;
    vector<Booking> bookings;
    vector<MaintenanceRecord> maintenanceRecords;
    
    int nextVehicleId;
    int nextDriverId;
    int nextRouteId;
    int nextBookingId;
    int nextMaintenanceRecordId;
    
public:
    FleetManagementSystem() {
        nextVehicleId = 1;
        nextDriverId = 1;
        nextRouteId = 1;
        nextBookingId = 1;
        nextMaintenanceRecordId = 1;
    }
    
    // Vehicle management
    Vehicle* findVehicle(int vehicleId) {
        for (auto& vehicle : vehicles) {
            if (vehicle.getVehicleId() == vehicleId) {
                return &vehicle;
            }
        }
        return nullptr;
    }
    
    bool addVehicle(const Vehicle& vehicle) {
        if (vehicles.size() >= MAX_VEHICLES) {
            return false;
        }
        
        vehicles.push_back(vehicle);
        return true;
    }
    
    bool removeVehicle(int vehicleId) {
        for (auto it = vehicles.begin(); it != vehicles.end(); ++it) {
            if (it->getVehicleId() == vehicleId) {
                vehicles.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Vehicle*> getAllVehicles() {
        vector<Vehicle*> result;
        for (auto& vehicle : vehicles) {
            result.push_back(&vehicle);
        }
        return result;
    }
    
    vector<Vehicle*> getVehiclesByType(VehicleType type) {
        vector<Vehicle*> result;
        for (auto& vehicle : vehicles) {
            if (vehicle.getType() == type) {
                result.push_back(&vehicle);
            }
        }
        return result;
    }
    
    vector<Vehicle*> getVehiclesByStatus(VehicleStatus status) {
        vector<Vehicle*> result;
        for (auto& vehicle : vehicles) {
            if (vehicle.getStatus() == status) {
                result.push_back(&vehicle);
            }
        }
        return result;
    }
    
    vector<Vehicle*> getVehiclesNeedingMaintenance() {
        vector<Vehicle*> result;
        for (auto& vehicle : vehicles) {
            if (vehicle.needsMaintenance()) {
                result.push_back(&vehicle);
            }
        }
        return result;
    }
    
    int getNextVehicleId() { return nextVehicleId++; }
    
    // Driver management
    Driver* findDriver(int driverId) {
        for (auto& driver : drivers) {
            if (driver.getDriverId() == driverId) {
                return &driver;
            }
        }
        return nullptr;
    }
    
    bool addDriver(const Driver& driver) {
        if (drivers.size() >= MAX_DRIVERS) {
            return false;
        }
        
        drivers.push_back(driver);
        return true;
    }
    
    bool removeDriver(int driverId) {
        for (auto it = drivers.begin(); it != drivers.end(); ++it) {
            if (it->getDriverId() == driverId) {
                drivers.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Driver*> getAllDrivers() {
        vector<Driver*> result;
        for (auto& driver : drivers) {
            result.push_back(&driver);
        }
        return result;
    }
    
    vector<Driver*> searchDriversByName(const string& name) {
        vector<Driver*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& driver : drivers) {
            string fullName = driver.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&driver);
            }
        }
        return result;
    }
    
    vector<Driver*> getActiveDrivers() {
        vector<Driver*> result;
        for (auto& driver : drivers) {
            if (driver.getIsActive()) {
                result.push_back(&driver);
            }
        }
        return result;
    }
    
    vector<Driver*> getDriversWithExpiredLicenses() {
        vector<Driver*> result;
        for (auto& driver : drivers) {
            if (driver.isLicenseExpired()) {
                result.push_back(&driver);
            }
        }
        return result;
    }
    
    int getNextDriverId() { return nextDriverId++; }
    
    // Route management
    Route* findRoute(int routeId) {
        for (auto& route : routes) {
            if (route.getRouteId() == routeId) {
                return &route;
            }
        }
        return nullptr;
    }
    
    bool addRoute(const Route& route) {
        if (routes.size() >= MAX_ROUTES) {
            return false;
        }
        
        routes.push_back(route);
        return true;
    }
    
    bool removeRoute(int routeId) {
        for (auto it = routes.begin(); it != routes.end(); ++it) {
            if (it->getRouteId() == routeId) {
                routes.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Route*> getAllRoutes() {
        vector<Route*> result;
        for (auto& route : routes) {
            result.push_back(&route);
        }
        return result;
    }
    
    vector<Route*> searchRoutesByOrigin(const string& origin) {
        vector<Route*> result;
        string lowerOrigin = origin;
        transform(lowerOrigin.begin(), lowerOrigin.end(), lowerOrigin.begin(), ::tolower);
        
        for (auto& route : routes) {
            string routeOrigin = route.getOrigin();
            transform(routeOrigin.begin(), routeOrigin.end(), routeOrigin.begin(), ::tolower);
            
            if (routeOrigin.find(lowerOrigin) != string::npos) {
                result.push_back(&route);
            }
        }
        return result;
    }
    
    vector<Route*> searchRoutesByDestination(const string& destination) {
        vector<Route*> result;
        string lowerDest = destination;
        transform(lowerDest.begin(), lowerDest.end(), lowerDest.begin(), ::tolower);
        
        for (auto& route : routes) {
            string routeDest = route.getDestination();
            transform(routeDest.begin(), routeDest.end(), routeDest.begin(), ::tolower);
            
            if (routeDest.find(lowerDest) != string::npos) {
                result.push_back(&route);
            }
        }
        return result;
    }
    
    int getNextRouteId() { return nextRouteId++; }
    
    // Booking management
    Booking* findBooking(int bookingId) {
        for (auto& booking : bookings) {
            if (booking.getBookingId() == bookingId) {
                return &booking;
            }
        }
        return nullptr;
    }
    
    bool addBooking(const Booking& booking) {
        if (bookings.size() >= MAX_BOOKINGS) {
            return false;
        }
        
        bookings.push_back(booking);
        return true;
    }
    
    bool removeBooking(int bookingId) {
        for (auto it = bookings.begin(); it != bookings.end(); ++it) {
            if (it->getBookingId() == bookingId) {
                bookings.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Booking*> getAllBookings() {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            result.push_back(&booking);
        }
        return result;
    }
    
    vector<Booking*> getBookingsByVehicle(int vehicleId) {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.getVehicleId() == vehicleId) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getBookingsByDriver(int driverId) {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.getDriverId() == driverId) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getBookingsByRoute(int routeId) {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.getRouteId() == routeId) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getBookingsByDate(int year, int month, int day) {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.getBookingDate().year == year &&
                booking.getBookingDate().month == month &&
                booking.getBookingDate().day == day) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getBookingsByStatus(BookingStatus status) {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.getStatus() == status) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getTodayBookings() {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.isToday()) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getPastBookings() {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.isPast()) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    vector<Booking*> getUpcomingBookings() {
        vector<Booking*> result;
        for (auto& booking : bookings) {
            if (booking.isFuture()) {
                result.push_back(&booking);
            }
        }
        return result;
    }
    
    int getNextBookingId() { return nextBookingId++; }
    
    // Maintenance record management
    MaintenanceRecord* findMaintenanceRecord(int recordId) {
        for (auto& record : maintenanceRecords) {
            if (record.getRecordId() == recordId) {
                return &record;
            }
        }
        return nullptr;
    }
    
    bool addMaintenanceRecord(const MaintenanceRecord& record) {
        if (maintenanceRecords.size() >= MAX_MAINTENANCE_RECORDS) {
            return false;
        }
        
        maintenanceRecords.push_back(record);
        return true;
    }
    
    bool removeMaintenanceRecord(int recordId) {
        for (auto it = maintenanceRecords.begin(); it != maintenanceRecords.end(); ++it) {
            if (it->getRecordId() == recordId) {
                maintenanceRecords.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<MaintenanceRecord*> getAllMaintenanceRecords() {
        vector<MaintenanceRecord*> result;
        for (auto& record : maintenanceRecords) {
            result.push_back(&record);
        }
        return result;
    }
    
    vector<MaintenanceRecord*> getMaintenanceRecordsByVehicle(int vehicleId) {
        vector<MaintenanceRecord*> result;
        for (auto& record : maintenanceRecords) {
            if (record.getVehicleId() == vehicleId) {
                result.push_back(&record);
            }
        }
        return result;
    }
    
    vector<MaintenanceRecord*> getMaintenanceRecordsByDateRange(date startDate, date endDate) {
        vector<MaintenanceRecord*> result;
        for (auto& record : maintenanceRecords) {
            date recordDate = record.getMaintenanceDate();
            
            bool isInRange = false;
            if (recordDate.year > startDate.year || 
                (recordDate.year == startDate.year && recordDate.month > startDate.month) ||
                (recordDate.year == startDate.year && recordDate.month == startDate.month && recordDate.day >= startDate.day)) {
                
                if (recordDate.year < endDate.year || 
                    (recordDate.year == endDate.year && recordDate.month < endDate.month) ||
                    (recordDate.year == endDate.year && recordDate.month == endDate.month && recordDate.day <= endDate.day)) {
                    isInRange = true;
                }
            }
            
            if (isInRange) {
                result.push_back(&record);
            }
        }
        return result;
    }
    
    int getNextMaintenanceRecordId() { return nextMaintenanceRecordId++; }
    
    // Business operations
    bool scheduleBooking(int vehicleId, int driverId, int routeId, const date& bookingDate,
                      const time& pickupTime, const time& returnTime,
                      const string& customerName, const string& customerPhone,
                      const string& customerEmail, int passengers,
                      double totalAmount, const string& specialRequests = "") {
        // Check if vehicle and driver exist and are available
        Vehicle* vehicle = findVehicle(vehicleId);
        Driver* driver = findDriver(driverId);
        
        if (!vehicle || !driver || !driver->getIsActive() || vehicle->getStatus() != AVAILABLE) {
            return false;
        }
        
        // Check if driver has valid license
        if (driver->isLicenseExpired()) {
            return false;
        }
        
        // Get current date for creation date
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date creationDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Booking booking(getNextBookingId(), vehicleId, driverId, routeId, bookingDate, 
                        pickupTime, returnTime, customerName, customerPhone, 
                        customerEmail, passengers, totalAmount, SCHEDULED, 
                        specialRequests, creationDate);
        
        if (!addBooking(booking)) {
            return false;
        }
        
        // Add booking to vehicle and driver
        vehicle->addBooking(booking.getBookingId());
        driver->addBooking(booking.getBookingId());
        
        // Add booking to route if exists
        Route* route = findRoute(routeId);
        if (route) {
            route->addBooking(booking.getBookingId());
        }
        
        // Update vehicle status to in use during booking period
        vehicle->setStatus(IN_USE);
        
        return true;
    }
    
    bool completeBooking(int bookingId, double actualFuelConsumed = 0.0) {
        Booking* booking = findBooking(bookingId);
        if (!booking || booking->getStatus() != IN_PROGRESS) {
            return false;
        }
        
        booking->setStatus(COMPLETED);
        
        // Update vehicle status back to available
        Vehicle* vehicle = findVehicle(booking->getVehicleId());
        if (vehicle) {
            vehicle->setStatus(AVAILABLE);
            
            // Update odometer based on route distance and fuel consumed
            Route* route = findRoute(booking->getRouteId());
            if (route && actualFuelConsumed > 0.0) {
                double odometerIncrease = actualFuelConsumed * vehicle->getFuelEfficiency();
                vehicle->setOdometer(vehicle->getOdometer() + odometerIncrease);
            }
        }
        
        return true;
    }
    
    bool cancelBooking(int bookingId, const string& reason) {
        Booking* booking = findBooking(bookingId);
        if (!booking || (booking->getStatus() != SCHEDULED && booking->getStatus() != IN_PROGRESS)) {
            return false;
        }
        
        booking->setStatus(CANCELLED);
        
        // Update vehicle status back to available
        Vehicle* vehicle = findVehicle(booking->getVehicleId());
        if (vehicle) {
            vehicle->setStatus(AVAILABLE);
        }
        
        return true;
    }
    
    bool completeMaintenance(int recordId, double cost, const string& mechanicName, bool isWarrantyWork) {
        MaintenanceRecord* record = findMaintenanceRecord(recordId);
        if (!record) {
            return false;
        }
        
        record->setCost(cost);
        record->setMechanicName(mechanicName);
        record->setIsWarrantyWork(isWarrantyWork);
        
        Vehicle* vehicle = findVehicle(record->getVehicleId());
        if (vehicle) {
            // Update last maintenance date for the vehicle
            time_t now = time(0);
            tm* timeinfo = localtime(&now);
            date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
            
            vehicle->setLastMaintenanceDate(today);
            
            // Update vehicle status if it was in maintenance
            if (vehicle->getStatus() == MAINTENANCE) {
                vehicle->setStatus(AVAILABLE);
            }
        }
        
        return true;
    }
    
    // Reporting
    void generateVehicleReport(int vehicleId) {
        Vehicle* vehicle = findVehicle(vehicleId);
        if (!vehicle) {
            cout << "Vehicle not found." << endl;
            return;
        }
        
        cout << "Vehicle Report" << endl;
        cout << "===============" << endl;
        vehicle->display();
        cout << endl;
        
        // Display maintenance records
        vector<MaintenanceRecord*> vehicleMaintenanceRecords = getMaintenanceRecordsByVehicle(vehicleId);
        if (!vehicleMaintenanceRecords.empty()) {
            cout << "Maintenance Records (" << vehicleMaintenanceRecords.size() << "):" << endl;
            
            double totalMaintenanceCost = 0.0;
            for (MaintenanceRecord* record : vehicleMaintenanceRecords) {
                cout << "ID: " << record->getRecordId()
                     << ", Date: " << record->getMaintenanceDate().year 
                     << "-" << record->getMaintenanceDate().month 
                     << "-" << record->getMaintenanceDate().day
                     << ", Description: " << record->getDescription()
                     << ", Cost: $" << fixed << setprecision(2) << record->getCost() << endl;
                
                totalMaintenanceCost += record->getCost();
            }
            
            cout << "Total Maintenance Cost: $" << fixed << setprecision(2) << totalMaintenanceCost << endl;
        } else {
            cout << "No maintenance records found." << endl;
        }
        
        // Display bookings
        vector<Booking*> vehicleBookings = getBookingsByVehicle(vehicleId);
        if (!vehicleBookings.empty()) {
            cout << endl << "Bookings (" << vehicleBookings.size() << "):" << endl;
            
            double totalRevenue = 0.0;
            map<BookingStatus, int> statusCounts;
            
            for (Booking* booking : vehicleBookings) {
                cout << "ID: " << booking->getBookingId()
                     << ", Customer: " << booking->getCustomerName()
                     << ", Date: " << booking->getBookingDate().year 
                     << "-" << booking->getBookingDate().month 
                     << "-" << booking->getBookingDate().day
                     << ", Status: " << booking->getStatusString() << endl;
                
                statusCounts[booking->getStatus()]++;
                totalRevenue += booking->getTotalAmount();
            }
            
            cout << endl << "Booking Status Distribution:" << endl;
            for (const auto& pair : statusCounts) {
                cout << "  " << booking->getStatusStringStatic(pair.first) 
                     << ": " << pair.second << endl;
            }
            
            cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        } else {
            cout << "No bookings found." << endl;
        }
    }
    
    void generateDriverReport(int driverId) {
        Driver* driver = findDriver(driverId);
        if (!driver) {
            cout << "Driver not found." << endl;
            return;
        }
        
        cout << "Driver Report" << endl;
        cout << "=============" << endl;
        driver->display();
        cout << endl;
        
        // Display bookings
        vector<Booking*> driverBookings = getBookingsByDriver(driverId);
        if (!driverBookings.empty()) {
            cout << "Bookings (" << driverBookings.size() << "):" << endl;
            
            map<BookingStatus, int> statusCounts;
            
            for (Booking* booking : driverBookings) {
                cout << "ID: " << booking->getBookingId()
                     << ", Customer: " << booking->getCustomerName()
                     << ", Date: " << booking->getBookingDate().year 
                     << "-" << booking->getBookingDate().month 
                     << "-" << booking->getBookingDate().day
                     << ", Status: " << booking->getStatusString() << endl;
                
                statusCounts[booking->getStatus()]++;
            }
            
            cout << endl << "Booking Status Distribution:" << endl;
            for (const auto& pair : statusCounts) {
                cout << "  " << booking->getStatusStringStatic(pair.first) 
                     << ": " << pair.second << endl;
            }
        } else {
            cout << "No bookings found." << endl;
        }
    }
    
    void generateBookingReport(int year, int month) {
        cout << "Booking Report for " << year << "-" << month << endl;
        cout << "============================" << endl;
        
        // This would need to be implemented properly to filter by month only
        // For now, we'll just get all bookings and manually filter
        vector<Booking*> monthlyBookings;
        for (auto& booking : bookings) {
            if (booking.getBookingDate().year == year && 
                booking.getBookingDate().month == month) {
                monthlyBookings.push_back(&booking);
            }
        }
        
        if (monthlyBookings.empty()) {
            cout << "No bookings found for this month." << endl;
            return;
        }
        
        // Group bookings by status
        map<BookingStatus, int> statusCounts;
        double totalRevenue = 0.0;
        
        for (Booking* booking : monthlyBookings) {
            statusCounts[booking->getStatus()]++;
            totalRevenue += booking->getTotalAmount();
        }
        
        cout << "Booking Summary:" << endl;
        cout << "Total Bookings: " << monthlyBookings.size() << endl;
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        cout << endl;
        
        cout << "Booking Status Distribution:" << endl;
        for (const auto& pair : statusCounts) {
            cout << "  " << booking->getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        // Group bookings by vehicle
        map<int, int> vehicleCounts;
        map<int, double> vehicleRevenue;
        
        for (Booking* booking : monthlyBookings) {
            vehicleCounts[booking->getVehicleId()]++;
            vehicleRevenue[booking->getVehicleId()] += booking->getTotalAmount();
        }
        
        cout << endl << "Vehicle Usage:" << endl;
        for (const auto& pair : vehicleCounts) {
            Vehicle* vehicle = findVehicle(pair.first);
            if (vehicle) {
                cout << "  Vehicle " << vehicle->getLicensePlate() 
                     << " (" << vehicle->getMake() << " " << vehicle->getModel() << "): "
                     << pair.second << " bookings, $" 
                     << fixed << setprecision(2) << vehicleRevenue[pair.first] << " revenue" << endl;
            }
        }
    }
    
    void generateSystemReport() {
        cout << "System Report" << endl;
        cout << "=============" << endl;
        
        // Vehicle statistics
        cout << "Vehicles: " << vehicles.size() << endl;
        map<VehicleType, int> vehicleTypeCounts;
        map<VehicleStatus, int> vehicleStatusCounts;
        double totalFuelEfficiency = 0.0;
        double totalMaintenanceCost = 0.0;
        
        for (const auto& vehicle : vehicles) {
            vehicleTypeCounts[vehicle.getType()]++;
            vehicleStatusCounts[vehicle.getStatus()]++;
            totalFuelEfficiency += vehicle.getFuelEfficiency();
            
            // Calculate maintenance cost
            vector<MaintenanceRecord*> vehicleMaintenanceRecords = getMaintenanceRecordsByVehicle(vehicle.getVehicleId());
            for (const auto& record : vehicleMaintenanceRecords) {
                totalMaintenanceCost += record->getCost();
            }
        }
        
        cout << "Vehicle Type Distribution:" << endl;
        for (const auto& pair : vehicleTypeCounts) {
            Vehicle dummyVehicle;
            cout << "  " << dummyVehicle.getTypeStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        cout << "Vehicle Status Distribution:" << endl;
        for (const auto& pair : vehicleStatusCounts) {
            Vehicle dummyVehicle;
            cout << "  " << dummyVehicle.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        if (vehicles.size() > 0) {
            cout << "Average Fuel Efficiency: " << fixed << setprecision(1) 
                 << (totalFuelEfficiency / vehicles.size()) << " km/L" << endl;
        }
        
        // Driver statistics
        cout << endl << "Drivers: " << drivers.size() << endl;
        map<bool, int> driverStatusCounts;
        
        for (const auto& driver : drivers) {
            driverStatusCounts[driver.getIsActive()]++;
        }
        
        cout << "Active Drivers: " << driverStatusCounts[true] << endl;
        cout << "Inactive Drivers: " << driverStatusCounts[false] << endl;
        cout << "Drivers with Expired Licenses: " << getDriversWithExpiredLicenses().size() << endl;
        
        // Route statistics
        cout << endl << "Routes: " << routes.size() << endl;
        
        // Booking statistics
        cout << endl << "Bookings: " << bookings.size() << endl;
        map<BookingStatus, int> bookingStatusCounts;
        double totalRevenue = 0.0;
        
        for (const auto& booking : bookings) {
            bookingStatusCounts[booking->getStatus()]++;
            totalRevenue += booking->getTotalAmount();
        }
        
        cout << "Booking Status Distribution:" << endl;
        for (const auto& pair : bookingStatusCounts) {
            Booking dummyBooking;
            cout << "  " << dummyBooking.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        
        // Maintenance statistics
        cout << endl << "Maintenance Records: " << maintenanceRecords.size() << endl;
        cout << "Total Maintenance Cost: $" << fixed << setprecision(2) << totalMaintenanceCost << endl;
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save next IDs
        outFile << "NEXT_IDS" << endl;
        outFile << nextVehicleId << "|" << nextDriverId << "|" << nextRouteId << "|" 
                << nextBookingId << "|" << nextMaintenanceRecordId << endl;
        
        // Save vehicles
        outFile << "VEHICLES" << endl;
        for (const auto& vehicle : vehicles) {
            outFile << vehicle.getVehicleId() << "|" 
                    << vehicle.getLicensePlate() << "|" 
                    << vehicle.getMake() << "|" 
                    << vehicle.getModel() << "|" 
                    << vehicle.getYear() << "|" 
                    << static_cast<int>(vehicle.getType()) << "|" 
                    << vehicle.getCapacity() << "|" 
                    << fixed << setprecision(2) << vehicle.getFuelEfficiency() << "|" 
                    << fixed << setprecision(1) << vehicle.getOdometer() << "|" 
                    << vehicle.getColor() << "|" 
                    << static_cast<int>(vehicle.getStatus()) << "|" 
                    << fixed << setprecision(2) << vehicle.getDailyRate() << "|" 
                    << fixed << setprecision(2) << vehicle.getWeeklyRate() << "|" 
                    << vehicle.getLastMaintenanceDate().year << "|" 
                    << vehicle.getLastMaintenanceDate().month << "|" 
                    << vehicle.getLastMaintenanceDate().day << "|";
            
            // Save booking IDs
            vector<int> bookingIds = vehicle.getBookingIds();
            outFile << bookingIds.size();
            for (int bookingId : bookingIds) {
                outFile << "," << bookingId;
            }
            outFile << "|";
            
            // Save maintenance record IDs
            vector<int> maintenanceRecordIds = vehicle.getMaintenanceRecordIds();
            outFile << maintenanceRecordIds.size();
            for (int recordId : maintenanceRecordIds) {
                outFile << "," << recordId;
            }
            outFile << endl;
        }
        
        // Save drivers
        outFile << "DRIVERS" << endl;
        for (const auto& driver : drivers) {
            outFile << driver.getDriverId() << "|" 
                    << driver.getFirstName() << "|" 
                    << driver.getLastName() << "|" 
                    << driver.getLicenseNumber() << "|" 
                    << driver.getLicenseExpiryDate().year << "|" 
                    << driver.getLicenseExpiryDate().month << "|" 
                    << driver.getLicenseExpiryDate().day << "|" 
                    << driver.getAddress() << "|" 
                    << driver.getPhone() << "|" 
                    << driver.getEmail() << "|" 
                    << driver.getHireDate().year << "|" 
                    << driver.getHireDate().month << "|" 
                    << driver.getHireDate().day << "|" 
                    << (driver.getIsActive() ? "1" : "0") << "|";
            
            // Save booking IDs
            vector<int> bookingIds = driver.getBookingIds();
            outFile << bookingIds.size();
            for (int bookingId : bookingIds) {
                outFile << "," << bookingId;
            }
            outFile << endl;
        }
        
        // Save routes
        outFile << "ROUTES" << endl;
        for (const auto& route : routes) {
            outFile << route.getRouteId() << "|" 
                    << route.getName() << "|" 
                    << route.getDescription() << "|" 
                    << route.getOrigin() << "|" 
                    << route.getDestination() << "|" 
                    << fixed << setprecision(1) << route.getDistance() << "|" 
                    << fixed << setprecision(1) << route.getEstimatedTime() << "|" 
                    << fixed << setprecision(2) << route.getBaseFare() << "|";
            
            // Save booking IDs
            vector<int> bookingIds = route.getBookingIds();
            outFile << bookingIds.size();
            for (int bookingId : bookingIds) {
                outFile << "," << bookingId;
            }
            outFile << endl;
        }
        
        // Save bookings
        outFile << "BOOKINGS" << endl;
        for (const auto& booking : bookings) {
            outFile << booking.getBookingId() << "|" 
                    << booking.getVehicleId() << "|" 
                    << booking.getDriverId() << "|" 
                    << booking.getRouteId() << "|" 
                    << booking.getBookingDate().year << "|" 
                    << booking.getBookingDate().month << "|" 
                    << booking.getBookingDate().day << "|" 
                    << booking.getPickupTime().hour << "|" 
                    << booking.getPickupTime().minute << "|" 
                    << booking.getReturnTime().hour << "|" 
                    << booking.getReturnTime().minute << "|" 
                    << booking.getCustomerName() << "|" 
                    << booking.getCustomerPhone() << "|" 
                    << booking.getCustomerEmail() << "|" 
                    << booking.getPassengers() << "|" 
                    << fixed << setprecision(2) << booking.getTotalAmount() << "|" 
                    << static_cast<int>(booking.getStatus()) << "|" 
                    << booking.getSpecialRequests() << "|" 
                    << booking.getCreationDate().year << "|" 
                    << booking.getCreationDate().month << "|" 
                    << booking.getCreationDate().day << endl;
        }
        
        // Save maintenance records
        outFile << "MAINTENANCE_RECORDS" << endl;
        for (const auto& record : maintenanceRecords) {
            outFile << record.getRecordId() << "|" 
                    << record.getVehicleId() << "|" 
                    << record.getMaintenanceDate().year << "|" 
                    << record.getMaintenanceDate().month << "|" 
                    << record.getMaintenanceDate().day << "|" 
                    << record.getDescription() << "|" 
                    << fixed << setprecision(2) << record.getCost() << "|" 
                    << record.getOdometerReading() << "|" 
                    << record.getMechanicName() << "|" 
                    << (record.getIsWarrantyWork() ? "1" : "0") << endl;
        }
        
        outFile.close();
        return true;
    }
    
    bool loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            return false;
        }
        
        string line;
        string section = "";
        
        while (getline(inFile, line)) {
            if (line == "NEXT_IDS") {
                section = "NEXT_IDS";
                continue;
            } else if (line == "VEHICLES") {
                section = "VEHICLES";
                continue;
            } else if (line == "DRIVERS") {
                section = "DRIVERS";
                continue;
            } else if (line == "ROUTES") {
                section = "ROUTES";
                continue;
            } else if (line == "BOOKINGS") {
                section = "BOOKINGS";
                continue;
            } else if (line == "MAINTENANCE_RECORDS") {
                section = "MAINTENANCE_RECORDS";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "NEXT_IDS" && tokens.size() >= 5) {
                nextVehicleId = stoi(tokens[0]);
                nextDriverId = stoi(tokens[1]);
                nextRouteId = stoi(tokens[2]);
                nextBookingId = stoi(tokens[3]);
                nextMaintenanceRecordId = stoi(tokens[4]);
            } else if (section == "VEHICLES" && tokens.size() >= 17) {
                int vehicleId = stoi(tokens[0]);
                string licensePlate = tokens[1];
                string make = tokens[2];
                string model = tokens[3];
                int year = stoi(tokens[4]);
                VehicleType type = static_cast<VehicleType>(stoi(tokens[5]));
                int capacity = stoi(tokens[6]);
                double fuelEfficiency = stod(tokens[7]);
                double odometer = stod(tokens[8]);
                string color = tokens[9];
                VehicleStatus status = static_cast<VehicleStatus>(stoi(tokens[10]));
                double dailyRate = stod(tokens[11]);
                double weeklyRate = stod(tokens[12]);
                
                date lastMaintenanceDate;
                lastMaintenanceDate.year = stoi(tokens[13]);
                lastMaintenanceDate.month = stoi(tokens[14]);
                lastMaintenanceDate.day = stoi(tokens[15]);
                
                Vehicle vehicle(vehicleId, licensePlate, make, model, year, type, capacity,
                                fuelEfficiency, odometer, color, status,
                                dailyRate, weeklyRate);
                vehicle.setLastMaintenanceDate(lastMaintenanceDate);
                
                // Parse booking IDs
                int bookingCount = stoi(tokens[16]);
                if (bookingCount > 0) {
                    stringstream bookingSs(tokens[17].substr(1)); // Skip first character
                    string bookingToken;
                    while (getline(bookingSs, bookingToken, ',')) {
                        vehicle.addBooking(stoi(bookingToken));
                    }
                }
                
                // Parse maintenance record IDs
                if (tokens.size() > 18) {
                    int recordCount = stoi(tokens[18]);
                    if (recordCount > 0) {
                        stringstream recordSs(tokens[19].substr(1)); // Skip first character
                        string recordToken;
                        while (getline(recordSs, recordToken, ',')) {
                            vehicle.addMaintenanceRecord(stoi(recordToken));
                        }
                    }
                }
                
                vehicles.push_back(vehicle);
            } else if (section == "DRIVERS" && tokens.size() >= 15) {
                int driverId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string licenseNumber = tokens[3];
                
                date licenseExpiryDate;
                licenseExpiryDate.year = stoi(tokens[4]);
                licenseExpiryDate.month = stoi(tokens[5]);
                licenseExpiryDate.day = stoi(tokens[6]);
                
                string address = tokens[7];
                string phone = tokens[8];
                string email = tokens[9];
                
                date hireDate;
                hireDate.year = stoi(tokens[10]);
                hireDate.month = stoi(tokens[11]);
                hireDate.day = stoi(tokens[12]);
                
                bool isActive = (tokens[13] == "1");
                
                Driver driver(driverId, firstName, lastName, licenseNumber, licenseExpiryDate,
                               address, phone, email, hireDate, isActive);
                
                // Parse booking IDs
                int bookingCount = stoi(tokens[14]);
                if (bookingCount > 0) {
                    stringstream bookingSs(tokens[15].substr(1)); // Skip first character
                    string bookingToken;
                    while (getline(bookingSs, bookingToken, ',')) {
                        driver.addBooking(stoi(bookingToken));
                    }
                }
                
                drivers.push_back(driver);
            } else if (section == "ROUTES" && tokens.size() >= 10) {
                int routeId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                string origin = tokens[3];
                string destination = tokens[4];
                double distance = stod(tokens[5]);
                double estimatedTime = stod(tokens[6]);
                double baseFare = stod(tokens[7]);
                
                Route route(routeId, name, description, origin, destination, distance,
                            estimatedTime, baseFare);
                
                // Parse booking IDs
                int bookingCount = stoi(tokens[8]);
                if (bookingCount > 0) {
                    stringstream bookingSs(tokens[9].substr(1)); // Skip first character
                    string bookingToken;
                    while (getline(bookingSs, bookingToken, ',')) {
                        route.addBooking(stoi(bookingToken));
                    }
                }
                
                routes.push_back(route);
            } else if (section == "BOOKINGS" && tokens.size() >= 18) {
                int bookingId = stoi(tokens[0]);
                int vehicleId = stoi(tokens[1]);
                int driverId = stoi(tokens[2]);
                int routeId = stoi(tokens[3]);
                
                date bookingDate;
                bookingDate.year = stoi(tokens[4]);
                bookingDate.month = stoi(tokens[5]);
                bookingDate.day = stoi(tokens[6]);
                
                time pickupTime;
                pickupTime.hour = stoi(tokens[7]);
                pickupTime.minute = stoi(tokens[8]);
                
                time returnTime;
                returnTime.hour = stoi(tokens[9]);
                returnTime.minute = stoi(tokens[10]);
                
                string customerName = tokens[11];
                string customerPhone = tokens[12];
                string customerEmail = tokens[13];
                int passengers = stoi(tokens[14]);
                double totalAmount = stod(tokens[15]);
                BookingStatus status = static_cast<BookingStatus>(stoi(tokens[16]));
                string specialRequests = "";
                
                date creationDate;
                creationDate.year = stoi(tokens[17]);
                creationDate.month = stoi(tokens[18]);
                creationDate.day = stoi(tokens[19]);
                
                Booking booking(bookingId, vehicleId, driverId, routeId, bookingDate,
                               pickupTime, returnTime, customerName, customerPhone,
                               customerEmail, passengers, totalAmount, status,
                               specialRequests, creationDate);
                
                bookings.push_back(booking);
            } else if (section == "MAINTENANCE_RECORDS" && tokens.size() >= 9) {
                int recordId = stoi(tokens[0]);
                int vehicleId = stoi(tokens[1]);
                
                date maintenanceDate;
                maintenanceDate.year = stoi(tokens[2]);
                maintenanceDate.month = stoi(tokens[3]);
                maintenanceDate.day = stoi(tokens[4]);
                
                string description = tokens[5];
                double cost = stod(tokens[6]);
                int odometerReading = stoi(tokens[7]);
                string mechanicName = tokens[8];
                bool isWarrantyWork = (tokens[9] == "1");
                
                MaintenanceRecord record(recordId, vehicleId, maintenanceDate, description,
                                        cost, odometerReading, mechanicName,
                                        isWarrantyWork);
                
                maintenanceRecords.push_back(record);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        vehicles.clear();
        drivers.clear();
        routes.clear();
        bookings.clear();
        maintenanceRecords.clear();
        
        nextVehicleId = 1;
        nextDriverId = 1;
        nextRouteId = 1;
        nextBookingId = 1;
        nextMaintenanceRecordId = 1;
        
        // Create sample vehicles
        vector<Vehicle> sampleVehicles = {
            Vehicle(getNextVehicleId(), "ABC123", "Toyota", "Camry", 2020, SEDAN, 5, 15.5, 25000.0, "Blue", AVAILABLE, 50.0, 300.0),
            Vehicle(getNextVehicleId(), "XYZ789", "Ford", "Explorer", 2019, SUV, 7, 12.0, 35000.0, "Red", AVAILABLE, 60.0, 350.0),
            Vehicle(getNextVehicleId(), "LMN456", "Honda", "Civic", 2021, SEDAN, 5, 18.0, 20000.0, "White", MAINTENANCE, 45.0, 270.0),
            Vehicle(getNextVehicleId(), "PQR111", "Ford", "Transit", 2018, VAN, 8, 10.0, 50000.0, "Black", OUT_OF_SERVICE, 70.0, 400.0),
            Vehicle(getNextVehicleId(), "STU222", "Yamaha", "R1", 2015, MOTORCYCLE, 2, 25.0, 10000.0, "Red", AVAILABLE, 30.0, 150.0),
            Vehicle(getNextVehicleId(), "GHI333", "Mercedes", "Sprinter", 2020, TRUCK, 3, 8.0, 100000.0, "White", AVAILABLE, 100.0, 500.0),
            Vehicle(getNextVehicleId(), "JKL444", "Tesla", "Model 3", 2022, SEDAN, 5, 20.0, 15000.0, "Black", AVAILABLE, 80.0, 400.0)
        };
        
        for (const auto& vehicle : sampleVehicles) {
            addVehicle(vehicle);
        }
        
        // Create sample drivers
        vector<Driver> sampleDrivers = {
            Driver(getNextDriverId(), "John", "Smith", "DL123456789", {2025, 12, 31},
                    "123 Main St", "555-1234", "john.smith@email.com",
                    "456 Oak Ave", "555-5678", "john.smith@example.com",
                    {2020, 1, 15}, true),
            Driver(getNextDriverId(), "Jane", "Johnson", "DL987654321", {2022, 6, 15},
                    "789 Pine Rd", "555-9012", "jane.johnson@email.com",
                    "321 Elm St", "555-3456", "jane.johnson@example.com",
                    {2019, 3, 10}, true),
            Driver(getNextDriverId(), "Michael", "Brown", "DL456789012", {2023, 9, 20},
                    "654 Maple Dr", "555-2345", "michael.brown@email.com",
                    "987 Cedar Ln", "555-6789", "michael.brown@email.com",
                    {2018, 2, 5}, true),
            Driver(getNextDriverId(), "Sarah", "Davis", "DL789012345", {2020, 5, 10},
                    "321 Birch Way", "555-3456", "sarah.davis@email.com",
                    "654 Maple Dr", "555-6789", "sarah.davis@email.com",
                    {2021, 4, 28}, true),
            Driver(getNextDriverId(), "David", "Wilson", "DL23456789", {2019, 8, 15},
                    "147 Spruce St", "555-7890", "david.wilson@email.com",
                    "456 Oak Ave", "555-2345", "david.wilson@email.com",
                    {2022, 2, 28}, true)
        };
        
        for (const auto& driver : sampleDrivers) {
            addDriver(driver);
        }
        
        // Create sample routes
        vector<Route> sampleRoutes = {
            Route(getNextRouteId(), "Airport Shuttle", "Shuttle service to airport", 
                  "Downtown", "Airport", 25.0, 1.0, 15.0),
            Route(getNextRouteId(), "City Tour", "Sightseeing tour of the city", 
                  "Hotel", "City Center", 10.0, 2.5, 12.0),
            Route(getNextRouteId(), "Interstate Express", "Long distance route between cities", 
                  "City A", "City B", 500.0, 5.5, 50.0),
            Route(getNextRouteId(), "Shopping Center Route", "Route covering major shopping areas", 
                  "Mall", "Shopping District", 15.0, 0.5, 10.0),
            Route(getNextRouteId(), "Airport Return", "Return trip from airport to city", 
                  "Airport", "Downtown", 25.0, 1.0, 15.0),
            Route(getNextRouteId(), "Weekend Getaway", "Scenic drive for weekends", 
                  "City Center", "Countryside", 30.0, 1.5, 20.0)
        };
        
        for (const auto& route : sampleRoutes) {
            addRoute(route);
        }
        
        // Create sample bookings
        vector<Booking> sampleBookings = {
            Booking(getNextBookingId(), 1, 1, 1, {2023, 11, 15}, {9, 0}, {17, 0},
                      "John Doe", "555-1234", "john.doe@email.com", 2, 100.00, SCHEDULED,
                      "Window seat preference"),
                      {2023, 10, 20}),
            Booking(getNextBookingId(), 2, 2, 2, {2023, 11, 20}, {10, 30}, {12, 0},
                      "Jane Smith", "555-5678", "jane.smith@email.com", 4, 150.00, SCHEDULED,
                      "Child seat required"),
                      {2023, 10, 20}),
            Booking(getNextBookingId(), 3, 1, 3, {2023, 11, 25}, {8, 45}, {13, 0},
                      "Robert Johnson", "555-9012", "robert.johnson@email.com", 3, 120.00, COMPLETED),
                      ""),
                      {2023, 11, 20}),
            Booking(getNextBookingId(), 4, 4, 4, {2023, 11, 10}, {9, 15}, {14, 0},
                      "Emily Davis", "555-3456", "emily.davis@email.com", 1, 80.00, COMPLETED,
                      ""),
                      {2023, 11, 10}),
            Booking(getNextBookingId(), 5, 2, 5, {2023, 11, 30}, {9, 0}, {15, 0},
                      "Michael Brown", "555-2345", "michael.brown@email.com", 1, 75.00, CANCELLED,
                      "Customer requested different time")
        };
        
        for (const auto& booking : sampleBookings) {
            addBooking(booking);
            
            // Add booking to vehicle and driver
            findVehicle(booking.getVehicleId())->addBooking(booking.getBookingId());
            findDriver(booking.getDriverId())->addBooking(booking.getBookingId());
            
            // Add booking to route if exists
            Route* route = findRoute(booking.getRouteId());
            if (route) {
                route->addBooking(booking.getBookingId());
            }
        }
        
        // Create sample maintenance records
        vector<MaintenanceRecord> sampleMaintenanceRecords = {
            MaintenanceRecord(getNextMaintenanceRecordId(), 1, {2023, 5, 15}, "Regular oil change", 
                           45.00, 25500, "Mike's Garage", false),
            MaintenanceRecord(getNextMaintenanceRecordId(), 2, {2023, 7, 10}, "Brake pad replacement", 
                           180.00, 30000, "Quick Lube", true),
            MaintenanceRecord(getNextMaintenanceRecordId(), 3, {2023, 8, 5}, "Tire rotation and alignment", 
                           80.00, 25510, "Tire Plus", false),
            MaintenanceRecord(getNextMaintenanceRecordId(), 4, {2023, 9, 20}, "Engine tune-up", 
                           120.00, 25520, "Auto Care Center", false),
            MaintenanceRecord(getNextMaintenanceRecordId(), 5, {2023, 3, 25}, "Transmission service", 
                           500.00, 25500, "Transmission Experts", true)
        };
        
        for (const auto& record : sampleMaintenanceRecords) {
            addMaintenanceRecord(record);
            
            // Update vehicle last maintenance date
            Vehicle* vehicle = findVehicle(record.getVehicleId());
            if (vehicle) {
                vehicle->setLastMaintenanceDate(record.getMaintenanceDate());
                vehicle->setStatus(AVAILABLE);
            }
        }
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Vehicles: " << vehicles.size() << endl;
        cout << "Drivers: " << drivers.size() << endl;
        cout << "Routes: " << routes.size() << endl;
        cout << "Bookings: " << bookings.size() << endl;
        cout << "Maintenance Records: " << maintenanceRecords.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(FleetManagementSystem& system, int choice);
void vehicleMenu(FleetManagementSystem& system);
void driverMenu(FleetManagementSystem& system);
void routeMenu(FleetManagementSystem& system);
void bookingMenu(FleetManagementSystem& system);
void maintenanceMenu(FleetManagementSystem& system);
void reportMenu(FleetManagementSystem& system);
void fileMenu(FleetManagementSystem& system);

int main() {
    FleetManagementSystem system;
    
    cout << "Fleet Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("fleet_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("fleet_data.txt");
        cout << "Sample data generated and saved." << endl;
    } else {
        cout << "Existing data loaded successfully." << endl;
    }
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleMenuChoice(system, choice);
        
        cout << endl;
        system("pause");
        system("cls");
    } while (choice != 0);
    
    // Save data before exiting
    system.saveToFile("fleet_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Fleet Management System" << endl;
    cout << "1. Vehicle Management" << endl;
    cout << "2. Driver Management" << endl;
    cout << "3. Route Management" << endl;
    cout << "4. Booking Management" << endl;
    cout << "5. Maintenance Management" << endl;
    cout << "6. Reports" << endl;
    cout << "7. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(FleetManagementSystem& system, int choice) {
    switch (choice) {
        case 1:
            vehicleMenu(system);
            break;
        case 2:
            driverMenu(system);
            break;
        case 3:
            routeMenu(system);
            break;
        case 4:
            bookingMenu(system);
            break;
        case 5:
            maintenanceMenu(system);
            break;
        case 6:
            reportMenu(system);
            break;
        case 7:
            fileMenu(system);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void vehicleMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "Vehicle Management" << endl;
        cout << "1. Add Vehicle" << endl;
        cout << "2. Remove Vehicle" << endl;
        cout << "3. List All Vehicles" << endl;
        cout << "4. List Vehicles by Type" << endl;
        cout << "5. List Vehicles by Status" << endl;
        cout << "6. List Vehicles Needing Maintenance" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string licensePlate, make, model, color;
                int year, typeChoice, capacity;
                double fuelEfficiency, odometer, dailyRate, weeklyRate;
                
                cout << "Enter License Plate: ";
                getline(cin, licensePlate);
                
                cout << "Enter Make: ";
                getline(cin, make);
                
                cout << "Enter Model: ";
                getline(cin, model);
                
                cout << "Enter Year: ";
                cin >> year;
                
                cout << "Enter Vehicle Type:" << endl;
                cout << "1. Sedan" << endl;
                cout << "2. SUV" << endl;
                cout << "3. Van" << endl;
                cout << "4. Bus" << endl;
                cout << "5. Truck" << endl;
                cout << "6. Motorcycle" << endl;
                cout << "Enter choice: ";
                cin >> typeChoice;
                
                cout << "Enter Capacity: ";
                cin >> capacity;
                
                cout << "Enter Fuel Efficiency (km/L): ";
                cin >> fuelEfficiency;
                
                cout << "Enter Current Odometer (km): ";
                cin >> odometer;
                
                cout << "Enter Color: ";
                getline(cin, color);
                
                cout << "Enter Daily Rate ($): ";
                cin >> dailyRate;
                
                cout << "Enter Weekly Rate ($): ";
                cin >> weeklyRate;
                
                // Get current date for last maintenance date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                VehicleType type = static_cast<VehicleType>(typeChoice);
                
                Vehicle vehicle(system.getNextVehicleId(), licensePlate, make, model, year, type, capacity,
                               fuelEfficiency, odometer, color, AVAILABLE,
                               dailyRate, weeklyRate);
                vehicle.setLastMaintenanceDate(today);
                
                if (system.addVehicle(vehicle)) {
                    cout << "Vehicle added successfully." << endl;
                } else {
                    cout << "Failed to add vehicle. Maximum number of vehicles reached." << endl;
                }
                break;
            }
            case 2: {
                int vehicleId;
                cout << "Enter Vehicle ID to remove: ";
                cin >> vehicleId;
                
                if (system.removeVehicle(vehicleId)) {
                    cout << "Vehicle removed successfully." << endl;
                } else {
                    cout << "Failed to remove vehicle. Vehicle ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Vehicle*> vehicles = system.getAllVehicles();
                if (vehicles.empty()) {
                    cout << "No vehicles found." << endl;
                } else {
                    cout << "Vehicles (" << vehicles.size() << "):" << endl;
                    for (Vehicle* vehicle : vehicles) {
                        cout << "ID: " << vehicle->getVehicleId()
                             << ", Plate: " << vehicle->getLicensePlate()
                             << ", Make: " << vehicle->getMake()
                             << ", Model: " << vehicle->getModel()
                             << ", Year: " << vehicle->getYear()
                             << ", Type: " << vehicle->getTypeString()
                             << ", Status: " << vehicle->getStatusString() << endl;
                    }
                }
                break;
            }
            case 4: {
                int typeChoice;
                cout << "Enter Vehicle Type:" << endl;
                cout << "1. Sedan" << endl;
                cout << "2. SUV" << endl;
                cout << "3. Van" << endl;
                cout << "4. Bus" << endl;
                cout << "5. Truck" << endl;
                cout << "6. Motorcycle" << endl;
                cout << "Enter choice: ";
                cin >> typeChoice;
                
                VehicleType type = static_cast<VehicleType>(typeChoice);
                
                vector<Vehicle*> vehicles = system.getVehiclesByType(type);
                if (vehicles.empty()) {
                    cout << "No vehicles found of this type." << endl;
                } else {
                    cout << vehicles.size() << " " << vehicles[0]->getTypeString() << " Vehicles:" << endl;
                    for (Vehicle* vehicle : vehicles) {
                        cout << "ID: " << vehicle->getVehicleId()
                             << ", Plate: " << vehicle->getLicensePlate()
                             << ", Make: " << vehicle->getMake()
                             << ", Model: " << vehicle->getModel() << endl;
                    }
                }
                break;
            }
            case 5: {
                int statusChoice;
                cout << "Enter Vehicle Status:" << endl;
                cout << "1. Available" << endl;
                cout << "2. In Use" << endl;
                cout << "3. Maintenance" << endl;
                cout << "4. Out of Service" << endl;
                cout << "Enter choice: ";
                cin >> statusChoice;
                
                VehicleStatus status = static_cast<VehicleStatus>(statusChoice);
                
                vector<Vehicle*> vehicles = system.getVehiclesByStatus(status);
                if (vehicles.empty()) {
                    cout << "No vehicles found with this status." << endl;
                } else {
                    cout << vehicles.size() << " " << vehicles[0]->getStatusString() << " Vehicles:" << endl;
                    for (Vehicle* vehicle : vehicles) {
                        cout << "ID: " << vehicle->getVehicleId()
                             << ", Plate: " << vehicle->getLicensePlate()
                             << ", Make: " << vehicle->getMake()
                             << ", Model: " << vehicle->getModel() << endl;
                    }
                }
                break;
            }
            case 6: {
                vector<Vehicle*> vehicles = system.getVehiclesNeedingMaintenance();
                if (vehicles.empty()) {
                    cout << "No vehicles need maintenance." << endl;
                } else {
                    cout << "Vehicles Needing Maintenance (" << vehicles.size() << "):" << endl;
                    for (Vehicle* vehicle : vehicles) {
                        cout << "ID: " << vehicle->getVehicleId()
                             << ", Plate: " << vehicle->getLicensePlate()
                             << ", Make: " << vehicle->getMake()
                             << ", Model: " << vehicle->getModel()
                             << ", Last Maintenance: " << vehicle->getLastMaintenanceDate().year 
                             << "-" << vehicle->getLastMaintenanceDate().month 
                             << "-" << vehicle->getLastMaintenanceDate().day << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void driverMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "Driver Management" << endl;
        cout << "1. Add Driver" << endl;
        cout << "2. Remove Driver" << endl;
        cout << "3. List All Drivers" << endl;
        cout << "4. Search Drivers by Name" << endl;
        cout << "5. List Active Drivers" << endl;
        cout << "6. List Drivers with Expired Licenses" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, licenseNumber, address, phone, email;
                int year, month, day;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter License Number: ";
                getline(cin, licenseNumber);
                
                cout << "Enter Address: ";
                getline(cin, address);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter License Expiry Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                // Get current date for hire date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date hireDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                date licenseExpiryDate = {year, month, day};
                
                Driver driver(system.getNextDriverId(), firstName, lastName, licenseNumber, licenseExpiryDate,
                               address, phone, email, hireDate, true);
                
                if (system.addDriver(driver)) {
                    cout << "Driver added successfully." << endl;
                } else {
                    cout << "Failed to add driver. Maximum number of drivers reached." << endl;
                }
                break;
            }
            case 2: {
                int driverId;
                cout << "Enter Driver ID to remove: ";
                cin >> driverId;
                
                if (system.removeDriver(driverId)) {
                    cout << "Driver removed successfully." << endl;
                } else {
                    cout << "Failed to remove driver. Driver ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Driver*> drivers = system.getAllDrivers();
                if (drivers.empty()) {
                    cout << "No drivers found." << endl;
                } else {
                    cout << "Drivers (" << drivers.size() << "):" << endl;
                    for (Driver* driver : drivers) {
                        cout << "ID: " << driver->getDriverId()
                             << ", Name: " << driver->getFullName()
                             << ", License: " << driver->getLicenseNumber()
                             << ", Status: " << (driver->getIsActive() ? "Active" : "Inactive") << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                
                vector<Driver*> drivers = system.searchDriversByName(name);
                if (drivers.empty()) {
                    cout << "No drivers found matching name." << endl;
                } else {
                    cout << "Drivers matching '" << name << "' (" << drivers.size() << "):" << endl;
                    for (Driver* driver : drivers) {
                        cout << "ID: " << driver->getDriverId()
                             << ", Name: " << driver->getFullName()
                             << ", Status: " << (driver->getIsActive() ? "Active" : "Inactive") << endl;
                    }
                }
                break;
            }
            case 5: {
                vector<Driver*> drivers = system.getActiveDrivers();
                if (drivers.empty()) {
                    cout << "No active drivers found." << endl;
                } else {
                    cout << "Active Drivers (" << drivers.size() << "):" << endl;
                    for (Driver* driver : drivers) {
                        cout << "ID: " << driver->getDriverId()
                             << ", Name: " << driver->getFullName()
                             << ", Years of Service: " << driver->getYearsOfService() << endl;
                    }
                }
                break;
            }
            case 6: {
                vector<Driver*> drivers = system.getDriversWithExpiredLicenses();
                if (drivers.empty()) {
                    cout << "No drivers with expired licenses found." << endl;
                } else {
                    cout << "Drivers with Expired Licenses (" << drivers.size() << "):" << endl;
                    for (Driver* driver : drivers) {
                        cout << "ID: " << driver->getDriverId()
                             << ", Name: " << driver->getFullName()
                             << ", License Expiry: " << driver->getLicenseExpiryDate().year 
                             << "-" << driver->getLicenseExpiryDate().month 
                             << "-" << driver->get resized potionId << "& date.size() << line;
                        for (int j = 1; j < line; j++) {
                            string dateStr = line.substr(j * maxCharsPerLine, maxCharsPerLine);
                            cout << dateStr;
                            if (j < lineCount / maxCharsPerLine || j < lineCount % maxCharsPerLine) {
                                cout << dateStr << endl;
                            }
                        }
                    }
                    
                    cout << endl;
                }
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void routeMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "Route Management" << endl;
        cout << "1. Add Route" << endl;
        cout << "2. Remove Route" << endl;
        cout << "3. List All Routes" << endl;
        cout << "4. Search Routes by Origin" << endl;
        cout << "5. Search Routes by Destination" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description, origin, destination;
                double distance, estimatedTime, baseFare;
                
                cout << "Enter Route Name: ";
                getline(cin, name);
                
                cout << "Enter Description: ";
                getline(cin, description);
                
                cout << "Enter Origin: ";
                getline(cin, origin);
                
                cout << "Enter Destination: ";
                getline(cin, destination);
                
                cout << "Enter Distance (km): ";
                cin >> distance;
                
                cout << "Enter Estimated Time (hours): ";
                cin >> estimatedTime;
                
                cout << "Enter Base Fare ($): ";
                cin >> baseFare;
                
                Route route(system.getNextRouteId(), name, description, origin, destination, 
                             distance, estimatedTime, baseFare);
                
                if (system.addRoute(route)) {
                    cout << "Route added successfully." << endl;
                } else {
                    cout << "Failed to add route. Maximum number of routes reached." << endl;
                }
                break;
            }
            case 2: {
                int routeId;
                cout << "Enter Route ID to remove: ";
                cin >> routeId;
                
                if (system.removeRoute(routeId)) {
                    cout << "Route removed successfully." << endl;
                } else {
                    cout << "Failed to remove route. Route ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Route*> routes = system.getAllRoutes();
                if (routes.empty()) {
                    cout << "No routes found." << endl;
                } else {
                    cout << "Routes (" << routes.size() << "):" << endl;
                    for (Route* route : routes) {
                        cout << "ID: " << route->getRouteId()
                             << ", Name: " << route->getName()
                             << ", Origin: " << route->getOrigin()
                             << ", Destination: " << route->getDestination()
                             << ", Distance: " << fixed << setprecision(1) << route->getDistance() << " km" << endl;
                    }
                }
                break;
            }
            case 4: {
                string origin;
                cout << "Enter Origin to search: ";
                getline(cin, origin);
                
                vector<Route*> routes = system.searchRoutesByOrigin(origin);
                if (routes.empty()) {
                    cout << "No routes found matching origin." << endl;
                } else {
                    cout << "Routes matching '" << origin << "' (" << routes.size() << "):" << endl;
                    for (Route* route : routes) {
                        cout << "ID: " << route->getRouteId()
                             << ", Name: " << route->getName()
                             << ", Origin: " << route->getOrigin() << endl;
                    }
                }
                break;
            }
            case 5: {
                string destination;
                cout << "Enter Destination to search: ";
                getline(cin, destination);
                
                vector<Route*> routes = system.searchRoutesByDestination(destination);
                if (routes.empty()) {
                    cout << "No routes found matching destination." << endl;
                } else {
                    cout << "Routes matching '" << destination << "' (" << routes.size() << "):" << endl;
                    for (Route* route : routes) {
                        cout << "ID: " << route->getRouteId()
                             << ", Name: " << route->getName()
                             << ", Destination: " << route->getDestination() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void bookingMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "Booking Management" << endl;
        cout << "1. Schedule Booking" << endl;
        cout << "2. Complete Booking" << endl;
        cout << "3. Cancel Booking" << endl;
        cout << "4. List All Bookings" << endl;
        cout << "5. List Bookings by Vehicle" << endl;
        cout << "6. List Bookings by Driver" << endl;
        cout << "7. List Bookings by Route" << endl;
        cout << "8. List Bookings by Date" << endl;
        cout << "9. List Bookings by Status" << endl;
        cout << "10. List Today's Bookings" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int vehicleId, driverId, routeId, year, month, day, hour, minute;
                string customerName, customerPhone, customerEmail;
                int passengers;
                double totalAmount;
                string specialRequests;
                
                cout << "Enter Vehicle ID: ";
                cin >> vehicleId;
                
                cout << "Enter Driver ID: ";
                cin >> driverId;
                
                cout << "Enter Route ID: ";
                cin >> routeId;
                
                cout << "Enter Booking Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                cout << "Enter Pickup Time (HH MM): ";
                cin >> hour >> minute;
                
                cout << "Enter Return Time (HH MM): ";
                cin >> hour >> minute;
                
                cout << "Enter Customer Name: ";
                getline(cin, customerName);
                
                cout << "Enter Customer Phone: ";
                getline(cin, customerPhone);
                
                cout << "Enter Customer Email: ";
                getline(cin, customerEmail);
                
                cout << "Number of Passengers: ";
                cin >> passengers;
                
                cout << "Enter Total Amount ($): ";
                cin >> totalAmount;
                
                cout << "Enter Special Requests (optional): ";
                getline(cin, specialRequests);
                
                date bookingDate = {year, month, day};
                time pickupTime = {hour, minute};
                time returnTime = {hour, minute};
                
                if (system.scheduleBooking(vehicleId, driverId, routeId, bookingDate, 
                                       pickupTime, returnTime, customerName,
                                       customerPhone, customerEmail, passengers,
                                       totalAmount, specialRequests)) {
                    cout << "Booking scheduled successfully." << endl;
                } else {
                    cout << "Failed to schedule booking. Vehicle or driver may not be available or driver license is expired." << endl;
                }
                break;
            }
            case 2: {
                int bookingId;
                double actualFuelConsumed;
                
                cout << "Enter Booking ID: ";
                cin >> bookingId;
                
                cout << "Enter Actual Fuel Consumed (L): ";
                cin >> actualFuelConsumed;
                
                if (system.completeBooking(bookingId, actualFuelConsumed)) {
                    cout << "Booking completed successfully." << endl;
                } else {
                    cout << "Failed to complete booking. Booking ID may not exist or booking is not in progress." << endl;
                }
                break;
            }
            case 3: {
                int bookingId;
                string reason;
                
                cout << "Enter Booking ID: ";
                cin >> bookingId;
                cin.ignore();
                
                cout << "Enter Cancellation Reason: ";
                getline(cin, reason);
                
                if (system.cancelBooking(bookingId, reason)) {
                    cout << "Booking cancelled successfully." << endl;
                } else {
                    cout << "Failed to cancel booking. Booking ID may not exist or booking is not scheduled." << endl;
                }
                break;
            }
            case 4: {
                vector<Booking*> bookings = system.getAllBookings();
                if (bookings.empty()) {
                    cout << "No bookings found." << endl;
                } else {
                    cout << "Bookings (" << bookings.size() << "):" << endl;
                    for (Booking* booking : bookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Date: " << booking->getBookingDate().year 
                             << "-" << booking->getBookingDate().month 
                             << "-" << booking->getBookingDate().day
                             << ", Status: " << booking->getStatusString() << endl;
                    }
                }
                }
                break;
            }
            case 5: {
                int vehicleId;
                cout << "Enter Vehicle ID: ";
                cin >> vehicleId;
                
                vector<Booking*> vehicleBookings = system.getBookingsByVehicle(vehicleId);
                if (vehicleBookings.empty()) {
                    cout << "No bookings found for this vehicle." << endl;
                } else {
                    cout << "Bookings for Vehicle " << vehicleId << " (" << vehicleBookings.size() << "):" << endl;
                    for (Booking* booking : vehicleBookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Date: " << booking->getBookingDate().year 
                             << "-" << booking->getBookingDate().month 
                             << "-" << booking->getBookingDate().day
                             << ", Status: " << booking->getStatusString() << endl;
                    }
                }
                break;
            }
            case 6: {
                int driverId;
                cout << "Enter Driver ID: ";
                cin >> driverId;
                
                vector<Booking*> driverBookings = system.getBookingsByDriver(driverId);
                if (driverBookings.empty()) {
                    cout << "No bookings found for this driver." << endl;
                } else {
                    cout << "Bookings for Driver " << driverId << " (" << driverBookings.size() << "):" << endl;
                    for (Booking* booking : driverBookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Date: " << booking->getBookingDate().year 
                             << "-" << booking->getBookingDate().month 
                             << "-" << booking->getBookingDate().day
                             << ", Status: " << booking->getStatusString() << endl;
                    }
                }
                break;
            }
            case 7: {
                int routeId;
                cout << "Enter Route ID: ";
                cin >> routeId;
                
                vector<Booking*> routeBookings = system.getBookingsByRoute(routeId);
                if (routeBookings.empty()) {
                    cout << "No bookings found for this route." << endl;
                } else {
                    cout << "Bookings for Route " << routeId << " (" << routeBookings.size() << "):" << endl;
                    for (Booking* booking : routeBookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Date: " << booking->getBookingDate().year 
                             << "-" << booking->getBookingDate().month 
                             << "-" << booking->getBookingDate().day
                             << ", Status: " << booking->getStatusString() << endl;
                    }
                }
                }
                break;
            }
            case 8: {
                int year, month, day;
                cout << "Enter Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                vector<Booking*> dateBookings = system.getBookingsByDate(year, month, day);
                if (dateBookings.empty()) {
                    cout << "No bookings found for this date." << endl;
                } else {
                    cout << "Bookings for " << year << "-" << month << "-" << day << " (" << dateBookings.size() << "):" << endl;
                    for (Booking* booking : dateBookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Time: " << setfill('0') << setw(2) << booking->getPickupTime().hour 
                             << ":" << setw(2) << booking->getPickupTime().minute
                             << ", Passengers: " << booking->getPassengers() << endl;
                    }
                }
                }
                break;
            }
            case 9: {
                int statusChoice;
                cout << "Enter Status:" << endl;
                cout << "1. Scheduled" << endl;
                cout << "2. In Progress" << endl;
                cout << "3. Completed" << endl;
                cout << "4. Cancelled" << endl;
                cout << "5. No Show" << endl;
                cout << "Enter choice: ";
                cin >> statusChoice;
                
                BookingStatus status = static_cast<BookingStatus>(statusChoice);
                
                vector<Booking*> statusBookings = system.getBookingsByStatus(status);
                
                string statusStr;
                switch (status) {
                    case SCHEDULED: statusStr = "Scheduled"; break;
                    case IN_PROGRESS: statusStr = "In Progress"; break;
                    case COMPLETED: statusStr = "Completed"; break;
                    case CANCELLED: statusStr = "Cancelled"; break;
                    case NO_SHOW: statusStr = "No Show"; break;
                    default: statusStr = "Unknown"; break;
                }
                
                if (statusBookings.empty()) {
                    cout << "No " << statusStr << " bookings found." << endl;
                } else {
                    cout << statusStr << " Bookings (" << statusBookings.size() << "):" << endl;
                    for (Booking* booking : statusBookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Date: " << booking->getBookingDate().year 
                             << "-" << booking->getBookingDate().month 
                             << "-" << booking->getBookingDate().day << endl;
                    }
                }
                }
                break;
            }
            case 10: {
                vector<Booking*> todayBookings = system.getTodayBookings();
                if (todayBookings.empty()) {
                    cout << "No bookings found for today." << endl;
                } else {
                    cout << "Today's Bookings (" << todayBookings.size() << "):" << endl;
                    for (Booking* booking : todayBookings) {
                        cout << "ID: " << booking->getBookingId()
                             << ", Customer: " << booking->getCustomerName()
                             << ", Time: " << setfill('0') << setw(2) << booking->getPickupTime().hour 
                             << ":" << setw(2) << booking->getPickupTime().minute
                             << ", Status: " << booking->getStatusString() << endl;
                    }
                }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void maintenanceMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "Maintenance Management" << endl;
        cout << "1. Add Maintenance Record" << endl;
        cout << "2. Complete Maintenance" << endl;
        cout << "3. List All Maintenance Records" << endl;
        cout << "4. List Records by Vehicle" << endl;
        cout << "5. List Records by Date Range" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int vehicleId;
                string description, mechanicName;
                double cost;
                int odometerReading;
                int year, month, day;
                bool isWarrantyWork;
                
                cout << "Enter Vehicle ID: ";
                cin >> vehicleId;
                
                cout << "Enter Maintenance Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                cout << "Enter Description: ";
                getline(cin, description);
                
                cout << "Enter Cost ($): ";
                cin >> cost;
                
                cout << "Enter Odometer Reading (km): ";
                cin >> odometerReading;
                
                cout << "Enter Mechanic Name: ";
                getline(cin, mechanicName);
                
                cout << "Is Warranty Work? (0-No, 1-Yes): ";
                cin >> isWarrantyWork;
                
                date maintenanceDate = {year, month, day};
                
                MaintenanceRecord record(system.getNextMaintenanceRecordId(), vehicleId, maintenanceDate, 
                                         description, cost, odometerReading, 
                                         mechanicName, isWarrantyWork);
                
                if (system.addMaintenanceRecord(record)) {
                    cout << "Maintenance record added successfully." << endl;
                } else {
                    cout << "Failed to add maintenance record. Maximum number of records reached." << endl;
                }
                break;
            }
            case 2: {
                int recordId;
                double cost;
                string mechanicName;
                bool isWarrantyWork;
                
                cout << "Enter Maintenance Record ID: ";
                cin >> recordId;
                
                cout << "Enter Actual Cost ($): ";
                cin >> cost;
                
                cout << "Enter Mechanic Name: ";
                getline(cin, mechanicName);
                
                cout << "Is Warranty Work? (0-No, 1-Yes): ";
                cin >> isWarrantyWork;
                
                if (system.completeMaintenance(recordId, cost, mechanicName, isWarrantyWork)) {
                    cout << "Maintenance completed successfully." << endl;
                } else {
                    cout << "Failed to complete maintenance. Record ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<MaintenanceRecord*> records = system.getAllMaintenanceRecords();
                if (records.empty()) {
                    cout << "No maintenance records found." << endl;
                } else {
                    cout << "Maintenance Records (" << records.size() << "):" << endl;
                    for (MaintenanceRecord* record : records) {
                        cout << "ID: " << record->getRecordId()
                             << ", Vehicle ID: " << record->getVehicleId()
                             << ", Date: " << record->getMaintenanceDate().year 
                             << "-" << record->getMaintenanceDate().month 
                             << "-" << record->getMaintenanceDate().day
                             << ", Description: " << record->getDescription()
                             << ", Cost: $" << fixed << setprecision(2) << record->getCost()
                             << ", Odometer: " << record->getOdometerReading() << " km"
                             << ", Mechanic: " << record->getMechanicName()
                             << ", Warranty: " << (record->getIsWarrantyWork() ? "Yes" : "No") << endl;
                    }
                }
                }
                break;
            }
            case 4: {
                int vehicleId;
                cout << "Enter Vehicle ID: ";
                cin >> vehicleId;
                
                vector<MaintenanceRecord*> vehicleRecords = system.getMaintenanceRecordsByVehicle(vehicleId);
                if (vehicleRecords.empty()) {
                    cout << "No maintenance records found for this vehicle." << endl;
                } else {
                    cout << "Maintenance Records for Vehicle " << vehicleId << " (" << vehicleRecords.size() << "):" << endl;
                    
                    double totalCost = 0.0;
                    for (MaintenanceRecord* record : vehicleRecords) {
                        totalCost += record->getCost();
                    }
                    
                    cout << "Total Maintenance Cost: $" << fixed << setprecision(2) << totalCost << endl;
                    
                    for (MaintenanceRecord* record : vehicleRecords) {
                        cout << "ID: " << record->getRecordId()
                             << ", Date: " << record->getMaintenanceDate().year 
                             << "-" << record->getMaintenanceDate().month 
                             << "-" << record->getMaintenanceDate().day
                             << ", Description: " << record->getDescription() << endl;
                    }
                }
                }
                break;
            }
            case 5: {
                int startYear, startMonth, startDay, endYear, endMonth, endDay;
                cout << "Enter Start Date (YYYY MM DD): ";
                cin >> startYear >> startMonth >> startDay;
                
                cout << "Enter End Date (YYYY MM DD): ";
                cin >> endYear >> endMonth >> endDay;
                
                date startDate = {startYear, startMonth, startDay};
                date endDate = {endYear, endMonth, endDay};
                
                vector<MaintenanceRecord*> dateRangeRecords = system.getMaintenanceRecordsByDateRange(startDate, endDate);
                if (dateRangeRecords.empty()) {
                    cout << "No maintenance records found in this date range." << endl;
                } else {
                    cout << "Maintenance Records between " << startYear << "-" << startMonth 
                             << "-" << startDay << " and " << endYear << "-" << endMonth 
                             << "-" << endDay << " (" << dateRangeRecords.size() << "):" << endl;
                    
                    double totalCost = 0.0;
                    for (MaintenanceRecord* record : dateRangeRecords) {
                        totalCost += record->getCost();
                    }
                    
                    cout << "Total Cost: $" << fixed << setprecision(2) << totalCost << endl;
                    
                    for (MaintenanceRecord* record : dateRangeRecords) {
                        cout << "ID: " << record->getRecordId()
                             << ", Date: " << record->getMaintenanceDate().year 
                             << "-" << record->getMaintenanceDate().month 
                             << "-" << record->getMaintenanceDate().day 
                             << ", Description: " << record->getDescription() << endl;
                    }
                }
                }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void reportMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Generate Vehicle Report" << endl;
        cout << "2. Generate Driver Report" << endl;
        cout << "3. Generate Booking Report" << endl;
        cout << "4. Generate System Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int vehicleId;
                cout << "Enter Vehicle ID: ";
                cin >> vehicleId;
                
                system.generateVehicleReport(vehicleId);
                break;
            }
            case 2: {
                int driverId;
                cout << "Enter Driver ID: ";
                cin >> driverId;
                
                system.generateDriverReport(driverId);
                break;
            }
            case 3: {
                int year, month;
                cout << "Enter Year: ";
                cin >> year;
                
                cout << "Enter Month: ";
                cin >> month;
                
                system.generateBookingReport(year, month);
                break;
            }
            case 4: {
                system.generateSystemReport();
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void fileMenu(FleetManagementSystem& system) {
    int choice;
    do {
        cout << "File Operations" << endl;
        cout << "1. Save Data to File" << endl;
        cout << "2. Load Data from File" << endl;
        cout << "3. Generate Sample Data" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter filename to save: ";
                getline(cin, filename);
                
                if (filename.empty()) {
                    filename = "fleet_data.txt";
                }
                
                if (system.saveToFile(filename)) {
                    cout << "Data saved successfully to " << filename << endl;
                } else {
                    cout << "Failed to save data." << endl;
                }
                break;
            }
            case 2: {
                string filename;
                cout << "Enter filename to load: ";
                getline(cin, filename);
                
                if (filename.empty()) {
                    filename = "fleet_data.txt";
                }
                
                if (system.loadFromFile(filename)) {
                    cout << "Data loaded successfully from " << filename << endl;
                } else {
                    cout << "Failed to load data." << endl;
                }
                break;
            }
            case 3: {
                system.generateSampleData();
                cout << "Sample data generated." << endl;
                
                // Save the sample data
                if (system.saveToFile("fleet_data.txt")) {
                    cout << "Sample data saved." << endl;
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
    }
}
<｜fim▁hole｜>