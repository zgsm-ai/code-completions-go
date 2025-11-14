#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <map>
#include <set>

/**
 * Smart Home System
 * 
 * This C++ program implements a basic smart home system that can manage
 * various smart devices, create scenes, and automate home functions.
 */

// Enum for device types
enum class DeviceType {
    LIGHT,
    THERMOSTAT,
    DOOR_LOCK,
    CAMERA,
    MOTION_SENSOR,
    SMART_SWITCH,
    SMART_PLUG
};

// Enum for device status
enum class DeviceStatus {
    ON,
    OFF,
    STANDBY,
    ERROR
};

// Base class for all smart devices
class SmartDevice {
protected:
    std::string deviceId;
    std::string name;
    DeviceType type;
    DeviceStatus status;
    std::string location;
    
public:
    SmartDevice(const std::string& id, const std::string& name, 
                DeviceType type, const std::string& location)
        : deviceId(id), name(name), type(type), status(DeviceStatus::OFF), 
          location(location) {}
    
    virtual ~SmartDevice() = default;
    
    // Getters
    std::string getId() const { return deviceId; }
    std::string getName() const { return name; }
    DeviceType getType() const { return type; }
    DeviceStatus getStatus() const { return status; }
    std::string getLocation() const { return location; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setLocation(const std::string& newLocation) { location = newLocation; }
    
    // Virtual methods for device control
    virtual bool turnOn() {
        status = DeviceStatus::ON;
        std::cout << name << " is now ON" << std::endl;
        return true;
    }
    
    virtual bool turnOff() {
        status = DeviceStatus::OFF;
        std::cout << name << " is now OFF" << std::endl;
        return true;
    }
    
    virtual void getStatusReport() const {
        std::cout << "Device: " << name << " (ID: " << deviceId << ")" << std::endl;
        std::cout << "Location: " << location << std::endl;
        std::cout << "Status: " << static_cast<int>(status) << std::endl;
    }
    
    virtual std::string getDeviceInfo() const {
        return "ID: " + deviceId + ", Name: " + name + ", Location: " + location + 
               ", Status: " + std::to_string(static_cast<int>(status));
    }
};

// Light class with brightness and color
class SmartLight : public SmartDevice {
private:
    int brightness; // 0-100
    std::string color; // Hex color code
    
public:
    SmartLight(const std::string& id, const std::string& name, 
               const std::string& location, int brightness = 100, 
               const std::string& color = "#FFFFFF")
        : SmartDevice(id, name, DeviceType::LIGHT, location), 
          brightness(brightness), color(color) {}
    
    // Setters
    void setBrightness(int level) {
        brightness = std::max(0, std::min(100, level));
        std::cout << name << " brightness set to " << brightness << "%" << std::endl;
    }
    
    void setColor(const std::string& hexColor) {
        color = hexColor;
        std::cout << name << " color changed to " << color << std::endl;
    }
    
    // Override methods
    void getStatusReport() const override {
        SmartDevice::getStatusReport();
        std::cout << "Brightness: " << brightness << "%" << std::endl;
        std::cout << "Color: " << color << std::endl;
    }
    
    std::string getDeviceInfo() const override {
        return SmartDevice::getDeviceInfo() + 
               ", Brightness: " + std::to_string(brightness) + "%" + 
               ", Color: " + color;
    }
};

// Thermostat class with temperature control
class SmartThermostat : public SmartDevice {
private:
    float currentTemp;
    float targetTemp;
    std::string mode; // Heat, Cool, Auto
    
public:
    SmartThermostat(const std::string& id, const std::string& name, 
                    const std::string& location, float currentTemp = 22.0f, 
                    float targetTemp = 22.0f, const std::string& mode = "Auto")
        : SmartDevice(id, name, DeviceType::THERMOSTAT, location), 
          currentTemp(currentTemp), targetTemp(targetTemp), mode(mode) {}
    
    // Setters
    void setTargetTemperature(float temp) {
        targetTemp = std::max(10.0f, std::min(35.0f, temp));
        std::cout << name << " target temperature set to " << targetTemp << "°C" << std::endl;
    }
    
    void setMode(const std::string& newMode) {
        mode = newMode;
        std::cout << name << " mode changed to " << mode << std::endl;
    }
    
    void updateCurrentTemperature(float temp) {
        currentTemp = temp;
    }
    
    // Override methods
    void getStatusReport() const override {
        SmartDevice::getStatusReport();
        std::cout << "Current Temperature: " << currentTemp << "°C" << std::endl;
        std::cout << "Target Temperature: " << targetTemp << "°C" << std::endl;
        std::cout << "Mode: " << mode << std::endl;
    }
    
    std::string getDeviceInfo() const override {
        return SmartDevice::getDeviceInfo() + 
               ", Current Temp: " + std::to_string(currentTemp) + "°C" + 
               ", Target Temp: " + std::to_string(targetTemp) + "°C" + 
               ", Mode: " + mode;
    }
};

// Smart Door Lock class
class SmartDoorLock : public SmartDevice {
private:
    bool isLocked;
    std::string accessCode;
    std::set<std::string> authorizedUsers;
    
public:
    SmartDoorLock(const std::string& id, const std::string& name, 
                  const std::string& location, const std::string& accessCode = "1234")
        : SmartDevice(id, name, DeviceType::DOOR_LOCK, location), 
          isLocked(true), accessCode(accessCode) {}
    
    // Setters
    bool lock() {
        isLocked = true;
        std::cout << name << " is now LOCKED" << std::endl;
        return true;
    }
    
    bool unlock(const std::string& code) {
        if (code == accessCode || authorizedUsers.find(code) != authorizedUsers.end()) {
            isLocked = false;
            std::cout << name << " is now UNLOCKED" << std::endl;
            return true;
        } else {
            std::cout << "Invalid access code for " << name << std::endl;
            return false;
        }
    }
    
    void addAuthorizedUser(const std::string& userId) {
        authorizedUsers.insert(userId);
        std::cout << "Added authorized user to " << name << std::endl;
    }
    
    void removeAuthorizedUser(const std::string& userId) {
        authorizedUsers.erase(userId);
        std::cout << "Removed authorized user from " << name << std::endl;
    }
    
    // Override methods
    void getStatusReport() const override {
        SmartDevice::getStatusReport();
        std::cout << "Lock Status: " << (isLocked ? "Locked" : "Unlocked") << std::endl;
        std::cout << "Authorized Users: " << authorizedUsers.size() << std::endl;
    }
    
    std::string getDeviceInfo() const override {
        return SmartDevice::getDeviceInfo() + 
               ", Lock Status: " + (isLocked ? "Locked" : "Unlocked") + 
               ", Authorized Users: " + std::to_string(authorizedUsers.size());
    }
};

// Scene class for device groups
class Scene {
private:
    std::string sceneName;
    std::vector<std::shared_ptr<SmartDevice>> devices;
    std::map<std::string, std::string> deviceStates; // Device ID -> State info
    
public:
    Scene(const std::string& name) : sceneName(name) {}
    
    void addDevice(std::shared_ptr<SmartDevice> device, const std::string& state = "") {
        devices.push_back(device);
        deviceStates[device->getId()] = state;
        std::cout << "Added " << device->getName() << " to scene " << sceneName << std::endl;
    }
    
    void activate() {
        std::cout << "Activating scene: " << sceneName << std::endl;
        for (const auto& device : devices) {
            if (deviceStates.find(device->getId()) != deviceStates.end()) {
                // Apply specific state if defined
                std::cout << "Setting state for " << device->getName() << std::endl;
            } else {
                // Default: turn on device
                device->turnOn();
            }
        }
    }
    
    void deactivate() {
        std::cout << "Deactivating scene: " << sceneName << std::endl;
        for (const auto& device : devices) {
            device->turnOff();
        }
    }
    
    void listDevices() const {
        std::cout << "Devices in scene '" << sceneName << "':" << std::endl;
        for (const auto& device : devices) {
            std::cout << "- " << device->getName() << " (" << device->getId() << ")" << std::endl;
        }
    }
};

// Smart Home System class
class SmartHomeSystem {
private:
    std::map<std::string, std::shared_ptr<SmartDevice>> devices;
    std::map<std::string, std::shared_ptr<Scene>> scenes;
    std::map<std::string, std::vector<std::string>> locationMap; // Location -> Device IDs
    
public:
    // Device management
    void addDevice(std::shared_ptr<SmartDevice> device) {
        devices[device->getId()] = device;
        locationMap[device->getLocation()].push_back(device->getId());
        std::cout << "Added device: " << device->getName() << " to system" << std::endl;
    }
    
    void removeDevice(const std::string& deviceId) {
        if (devices.find(deviceId) != devices.end()) {
            std::string location = devices[deviceId]->getLocation();
            
            // Remove from location map
            auto& locationDevices = locationMap[location];
            locationDevices.erase(
                std::remove(locationDevices.begin(), locationDevices.end(), deviceId),
                locationDevices.end()
            );
            
            devices.erase(deviceId);
            std::cout << "Removed device with ID: " << deviceId << std::endl;
        }
    }
    
    std::shared_ptr<SmartDevice> getDevice(const std::string& deviceId) {
        if (devices.find(deviceId) != devices.end()) {
            return devices[deviceId];
        }
        return nullptr;
    }
    
    // Scene management
    void addScene(std::shared_ptr<Scene> scene) {
        scenes[scene->sceneName] = scene;
        std::cout << "Added scene: " << scene->sceneName << std::endl;
    }
    
    void activateScene(const std::string& sceneName) {
        if (scenes.find(sceneName) != scenes.end()) {
            scenes[sceneName]->activate();
        } else {
            std::cout << "Scene '" << sceneName << "' not found" << std::endl;
        }
    }
    
    void deactivateScene(const std::string& sceneName) {
        if (scenes.find(sceneName) != scenes.end()) {
            scenes[sceneName]->deactivate();
        } else {
            std::cout << "Scene '" << sceneName << "' not found" << std::endl;
        }
    }
    
    // Location-based operations
    std::vector<std::shared_ptr<SmartDevice>> getDevicesByLocation(const std::string& location) {
        std::vector<std::shared_ptr<SmartDevice>> result;
        if (locationMap.find(location) != locationMap.end()) {
            for (const auto& deviceId : locationMap[location]) {
                if (devices.find(deviceId) != devices.end()) {
                    result.push_back(devices[deviceId]);
                }
            }
        }
        return result;
    }
    
    void turnOffAllDevices() {
        std::cout << "Turning off all devices..." << std::endl;
        for (const auto& pair : devices) {
            pair.second->turnOff();
        }
    }
    
    void turnOnAllDevices() {
        std::cout << "Turning on all devices..." << std::endl;
        for (const auto& pair : devices) {
            pair.second->turnOn();
        }
    }
    
    // Reporting
    void listAllDevices() const {
        std::cout << "All Smart Devices:" << std::endl;
        for (const auto& pair : devices) {
            std::cout << "- " << pair.second->getName() << " (" << pair.first << ")" << std::endl;
        }
    }
    
    void listAllScenes() const {
        std::cout << "All Scenes:" << std::endl;
        for (const auto& pair : scenes) {
            std::cout << "- " << pair.first << std::endl;
        }
    }
    
    void generateReport() const {
        std::cout << "=== Smart Home System Report ===" << std::endl;
        std::cout << "Total Devices: " << devices.size() << std::endl;
        std::cout << "Total Scenes: " << scenes.size() << std::endl;
        
        std::cout << "\nDevice Details:" << std::endl;
        for (const auto& pair : devices) {
            pair.second->getStatusReport();
            std::cout << std::endl;
        }
    }
};

int main() {
    // Create smart home system
    SmartHomeSystem homeSystem;
    
    // Create devices
    auto livingRoomLight = std::make_shared<SmartLight>("LGT001", "Living Room Light", "Living Room", 75, "#FFDD88");
    auto kitchenLight = std::make_shared<SmartLight>("LGT002", "Kitchen Light", "Kitchen", 100, "#FFFFFF");
    auto bedroomLight = std::make_shared<SmartLight>("LGT003", "Bedroom Light", "Bedroom", 50, "#FF99AA");
    
    auto mainThermostat = std::make_shared<SmartThermostat>("THR001", "Main Thermostat", "Hallway", 22.0f, 22.0f, "Auto");
    
    auto frontDoorLock = std::make_shared<SmartDoorLock>("LCK001", "Front Door Lock", "Front Door", "1234");
    auto backDoorLock = std::make_shared<SmartDoorLock>("LCK002", "Back Door Lock", "Back Door", "5678");
    
    // Add devices to system
    homeSystem.addDevice(livingRoomLight);
    homeSystem.addDevice(kitchenLight);
    homeSystem.addDevice(bedroomLight);
    homeSystem.addDevice(mainThermostat);
    homeSystem.addDevice(frontDoorLock);
    homeSystem.addDevice(backDoorLock);
    
    // Create scenes
    auto eveningScene = std::make_shared<Scene>("Evening");
    auto morningScene = std::make_shared<Scene>("Morning");
    auto awayScene = std::make_shared<Scene>("Away");
    
    // Add devices to scenes
    eveningScene->addDevice(livingRoomLight);
    eveningScene->addDevice(kitchenLight);
    
    morningScene->addDevice(kitchenLight);
    morningScene->addDevice(mainThermostat);
    
    awayScene->addDevice(frontDoorLock);
    
    // Add scenes to system
    homeSystem.addScene(eveningScene);
    homeSystem.addScene(morningScene);
    homeSystem.addScene(awayScene);
    
    // Perform some operations
    std::cout << "\n=== Testing Basic Operations ===" << std::endl;
    
    // Turn on some devices
    livingRoomLight->turnOn();
    livingRoomLight->setBrightness(80);
    livingRoomLight->setColor("#FFCC66");
    
    kitchenLight->turnOn();
    
    mainThermostat->turnOn();
    mainThermostat->setTargetTemperature(24.0f);
    
    frontDoorLock->turnOn();
    frontDoorLock->addAuthorizedUser("USER123");
    
    // Test scenes
    <｜fim▁hole｜>homeSystem.activateScene("Morning");
    
    std::cout << "\n=== Generating Report ===" << std::endl;
    homeSystem.generateReport();
    
    return 0;
}