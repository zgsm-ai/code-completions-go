#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <functional>
#include <cmath>
#include <fstream>
#include <sstream>

/**
 * File System Simulation
 * 
 * This C++ program implements a basic file system simulation with
 * directories, files, and navigation commands similar to a shell.
 */

// Enum for file types
enum class FileType {
    REGULAR_FILE,
    DIRECTORY,
    SYMLINK
};

// Utility function to convert file type to string
std::string fileTypeToString(FileType type) {
    switch (type) {
        case FileType::REGULAR_FILE: return "File";
        case FileType::DIRECTORY: return "Directory";
        case FileType::SYMLINK: return "Symlink";
        default: return "Unknown";
    }
}

// Forward declaration for Directory class
class Directory;

// Base class for all file system entities
class FileSystemEntity {
protected:
    std::string name;
    std::string parentPath;
    FileType type;
    size_t size;
    std::time_t creationTime;
    std::time_t modificationTime;
    std::shared_ptr<Directory> parent;
    
public:
    FileSystemEntity(const std::string& name, FileType type, 
                     const std::string& parentPath = "/", size_t size = 0)
        : name(name), type(type), parentPath(parentPath), size(size) {
        creationTime = std::time(nullptr);
        modificationTime = creationTime;
    }
    
    virtual ~FileSystemEntity() = default;
    
    // Getters
    std::string getName() const { return name; }
    std::string getParentPath() const { return parentPath; }
    FileType getType() const { return type; }
    size_t getSize() const { return size; }
    std::time_t getCreationTime() const { return creationTime; }
    std::time_t getModificationTime() const { return modificationTime; }
    std::shared_ptr<Directory> getParent() const { return parent; }
    
    // Setters
    void setName(const std::string& newName) { 
        name = newName; 
        updateModificationTime();
    }
    
    void setSize(size_t newSize) { 
        size = newSize; 
        updateModificationTime();
    }
    
    void setParent(std::shared_ptr<Directory> newParent) { 
        parent = newParent; 
        if (newParent) {
            parentPath = newParent->getFullPath();
        }
    }
    
    // Helper methods
    void updateModificationTime() {
        modificationTime = std::time(nullptr);
    }
    
    // Virtual methods
    virtual std::string getFullPath() const {
        if (parentPath == "/") {
            return "/" + name;
        } else {
            return parentPath + "/" + name;
        }
    }
    
    virtual void displayInfo(bool detailed = false) const {
        std::cout << fileTypeToString(type) << ": " << name;
        if (detailed) {
            std::cout << " (Size: " << size << " bytes, Modified: " << modificationTime << ")";
        }
        std::cout << std::endl;
    }
    
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

// Regular file class
class File : public FileSystemEntity {
private:
    std::string content;
    
public:
    File(const std::string& name, const std::string& content = "", 
          const std::string& parentPath = "/")
        : FileSystemEntity(name, FileType::REGULAR_FILE, parentPath, content.length()), 
          content(content) {}
    
    // Content management
    const std::string& getContent() const { return content; }
    
    void setContent(const std::string& newContent) {
        content = newContent;
        size = content.length();
        updateModificationTime();
    }
    
    void appendContent(const std::string& text) {
        content += text;
        size = content.length();
        updateModificationTime();
    }
    
    // File operations
    bool writeToFile(const std::string& filePath) const {
        std::ofstream file(filePath);
        if (file.is_open()) {
            file << content;
            file.close();
            return true;
        }
        return false;
    }
    
    bool readFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            content = buffer.str();
            size = content.length();
            updateModificationTime();
            file.close();
            return true;
        }
        return false;
    }
    
    // Display methods
    void displayInfo(bool detailed = false) const override {
        std::cout << fileTypeToString(type) << ": " << name;
        if (detailed) {
            std::cout << " (Size: " << size << " bytes, Modified: " << modificationTime << ")";
        }
        std::cout << std::endl;
    }
    
    void displayContent() const {
        std::cout << "Content of " << name << ":" << std::endl;
        std::cout << "-------------------" << std::endl;
        std::cout << content << std::endl;
        std::cout << "-------------------" << std::endl;
    }
    
    // Serialization
    std::string serialize() const override {
        return "FILE|" + name + "|" + std::to_string(size) + "|" + 
               std::to_string(creationTime) + "|" + std::to_string(modificationTime) + 
               "|" + parentPath + "|" + content;
    }
    
    void deserialize(const std::string& data) override {
        // Implementation would parse the serialized data
        // Simplified for this example
        std::cout << "Deserializing file data for " << name << std::endl;
    }
};

// Directory class
class Directory : public FileSystemEntity, public std::enable_shared_from_this<Directory> {
private:
    std::unordered_map<std::string, std::shared_ptr<FileSystemEntity>> children;
    
public:
    Directory(const std::string& name, const std::string& parentPath = "/")
        : FileSystemEntity(name, FileType::DIRECTORY, parentPath, 0) {}
    
    // Child management
    bool addChild(std::shared_ptr<FileSystemEntity> entity) {
        if (!entity) {
            return false;
        }
        
        std::string entityName = entity->getName();
        
        // Check if child with same name already exists
        if (children.find(entityName) != children.end()) {
            return false;
        }
        
        // Add to children
        children[entityName] = entity;
        
        // Set parent and update path
        if (entity->getType() != FileType::DIRECTORY) {
            entity->setParent(shared_from_this());
        } else {
            auto dirEntity = std::dynamic_pointer_cast<Directory>(entity);
            if (dirEntity) {
                dirEntity->parent = shared_from_this();
                dirEntity->parentPath = getFullPath();
            }
        }
        
        return true;
    }
    
    bool removeChild(const std::string& name) {
        if (children.find(name) != children.end()) {
            children.erase(name);
            return true;
        }
        return false;
    }
    
    std::shared_ptr<FileSystemEntity> getChild(const std::string& name) {
        if (children.find(name) != children.end()) {
            return children[name];
        }
        return nullptr;
    }
    
    std::shared_ptr<Directory> getSubdirectory(const std::string& name) {
        auto entity = getChild(name);
        if (entity && entity->getType() == FileType::DIRECTORY) {
            return std::dynamic_pointer_cast<Directory>(entity);
        }
        return nullptr;
    }
    
    std::shared_ptr<File> getFile(const std::string& name) {
        auto entity = getChild(name);
        if (entity && entity->getType() == FileType::REGULAR_FILE) {
            return std::dynamic_pointer_cast<File>(entity);
        }
        return nullptr;
    }
    
    // Directory information
    std::vector<std::shared_ptr<FileSystemEntity>> getChildren() const {
        std::vector<std::shared_ptr<FileSystemEntity>> result;
        for (const auto& pair : children) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    std::vector<std::shared_ptr<Directory>> getSubdirectories() const {
        std::vector<std::shared_ptr<Directory>> result;
        for (const auto& pair : children) {
            if (pair.second->getType() == FileType::DIRECTORY) {
                result.push_back(std::dynamic_pointer_cast<Directory>(pair.second));
            }
        }
        return result;
    }
    
    std::vector<std::shared_ptr<File>> getFiles() const {
        std::vector<std::shared_ptr<File>> result;
        for (const auto& pair : children) {
            if (pair.second->getType() == FileType::REGULAR_FILE) {
                result.push_back(std::dynamic_pointer_cast<File>(pair.second));
            }
        }
        return result;
    }
    
    size_t getDirectorySize() const {
        size_t totalSize = 0;
        for (const auto& pair : children) {
            totalSize += pair.second->getSize();
            if (pair.second->getType() == FileType::DIRECTORY) {
                auto dir = std::dynamic_pointer_cast<Directory>(pair.second);
                if (dir) {
                    totalSize += dir->getDirectorySize();
                }
            }
        }
        return totalSize;
    }
    
    int getChildCount() const {
        return children.size();
    }
    
    bool isEmpty() const {
        return children.empty();
    }
    
    // Navigation methods
    std::shared_ptr<Directory> navigateTo(const std::string& path) {
        if (path.empty() || path == ".") {
            return shared_from_this();
        }
        
        if (path == "..") {
            return parent;
        }
        
        // Check if it's a direct child
        auto child = getSubdirectory(path);
        if (child) {
            return child;
        }
        
        // Parse path
        std::stringstream ss(path);
        std::string segment;
        std::shared_ptr<Directory> current = shared_from_this();
        
        while (std::getline(ss, segment, '/')) {
            if (segment.empty() || segment == ".") {
                continue;
            }
            
            if (segment == "..") {
                if (current->parent) {
                    current = current->parent;
                }
                continue;
            }
            
            auto nextDir = current->getSubdirectory(segment);
            if (!nextDir) {
                return nullptr; // Path doesn't exist
            }
            current = nextDir;
        }
        
        return current;
    }
    
    std::string resolvePath(const std::string& path) const {
        if (path.empty()) {
            return getFullPath();
        }
        
        if (path[0] == '/') {
            // Absolute path
            return path;
        } else {
            // Relative path
            return getFullPath() + "/" + path;
        }
    }
    
    // Display methods
    void displayInfo(bool detailed = false) const override {
        std::cout << fileTypeToString(type) << ": " << name;
        if (detailed) {
            std::cout << " (Size: " << getDirectorySize() << " bytes, ";
            std::cout << "Children: " << children.size() << ", ";
            std::cout << "Modified: " << modificationTime << ")";
        }
        std::cout << std::endl;
    }
    
    void listChildren(bool detailed = false) const {
        std::cout << "Contents of " << name << ":" << std::endl;
        for (const auto& pair : children) {
            if (detailed) {
                std::cout << "  ";
                pair.second->displayInfo(true);
            } else {
                std::cout << "  " << pair.second->getName();
                if (pair.second->getType() == FileType::DIRECTORY) {
                    std::cout << "/";
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Tree display
    void displayTree(const std::string& prefix = "") const {
        std::cout << prefix;
        if (!prefix.empty()) {
            std::cout << "├── ";
        }
        std::cout << name << "/ (" << children.size() << " items)" << std::endl;
        
        for (auto it = children.begin(); it != children.end(); ++it) {
            bool isLast = (std::next(it) == children.end());
            std::string newPrefix = prefix + (isLast ? "    " : "│   ");
            
            if (it->second->getType() == FileType::DIRECTORY) {
                auto dir = std::dynamic_pointer_cast<Directory>(it->second);
                if (dir) {
                    dir->displayTree(newPrefix);
                }
            } else {
                std::cout << newPrefix << "├── " << it->second->getName();
                std::cout << " (" << it->second->getSize() << " bytes)" << std::endl;
            }
        }
    }
    
    // Serialization
    std::string serialize() const override {
        return "DIR|" + name + "|" + std::to_string(0) + "|" + 
               std::to_string(creationTime) + "|" + std::to_string(modificationTime) + 
               "|" + parentPath;
    }
    
    void deserialize(const std::string& data) override {
        // Implementation would parse the serialized data
        // Simplified for this example
        std::cout << "Deserializing directory data for " << name << std::endl;
    }
};

// Symlink class
class Symlink : public FileSystemEntity {
private:
    std::string targetPath;
    
public:
    Symlink(const std::string& name, const std::string& targetPath, 
            const std::string& parentPath = "/")
        : FileSystemEntity(name, FileType::SYMLINK, parentPath, 0), 
          targetPath(targetPath) {}
    
    const std::string& getTargetPath() const { return targetPath; }
    void setTargetPath(const std::string& path) { 
        targetPath = path; 
        updateModificationTime();
    }
    
    std::string serialize() const override {
        return "SYMLINK|" + name + "|" + std::to_string(0) + "|" + 
               std::to_string(creationTime) + "|" + std::to_string(modificationTime) + 
               "|" + parentPath + "|" + targetPath;
    }
    
    void deserialize(const std::string& data) override {
        // Implementation would parse the serialized data
        // Simplified for this example
        std::cout << "Deserializing symlink data for " << name << std::endl;
    }
};

// File System class
class FileSystem {
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentDirectory;
    std::string workingDirectory;
    
public:
    FileSystem() {
        // Create root directory
        root = std::make_shared<Directory>("/");
        currentDirectory = root;
        workingDirectory = "/";
    }
    
    // Navigation commands
    bool changeDirectory(const std::string& path) {
        std::shared_ptr<Directory> target;
        
        if (path.empty()) {
            target = root;
        } else if (path[0] == '/') {
            // Absolute path
            target = root->navigateTo(path.substr(1));
        } else {
            // Relative path
            target = currentDirectory->navigateTo(path);
        }
        
        if (target) {
            currentDirectory = target;
            workingDirectory = currentDirectory->getFullPath();
            return true;
        }
        
        return false;
    }
    
    std::string getCurrentPath() const {
        return workingDirectory;
    }
    
    std::shared_ptr<Directory> getCurrentDirectory() const {
        return currentDirectory;
    }
    
    bool goUp() {
        if (currentDirectory != root && currentDirectory->getParent()) {
            currentDirectory = currentDirectory->getParent();
            workingDirectory = currentDirectory->getFullPath();
            return true;
        }
        return false;
    }
    
    void goHome() {
        currentDirectory = root;
        workingDirectory = "/";
    }
    
    // Directory commands
    std::shared_ptr<Directory> makeDirectory(const std::string& name) {
        if (name.empty() || name.find('/') != std::string::npos) {
            return nullptr;
        }
        
        auto newDir = std::make_shared<Directory>(name, currentDirectory->getFullPath());
        if (currentDirectory->addChild(newDir)) {
            return newDir;
        }
        
        return nullptr;
    }
    
    bool removeDirectory(const std::string& name) {
        auto dir = currentDirectory->getSubdirectory(name);
        if (dir && dir->isEmpty()) {
            return currentDirectory->removeChild(name);
        }
        return false;
    }
    
    bool removeDirectoryRecursive(const std::string& name) {
        auto dir = currentDirectory->getSubdirectory(name);
        if (dir) {
            return currentDirectory->removeChild(name);
        }
        return false;
    }
    
    // File commands
    std::shared_ptr<File> createFile(const std::string& name, const std::string& content = "") {
        if (name.empty() || name.find('/') != std::string::npos) {
            return nullptr;
        }
        
        auto newFile = std::make_shared<File>(name, content, currentDirectory->getFullPath());
        if (currentDirectory->addChild(newFile)) {
            return newFile;
        }
        
        return nullptr;
    }
    
    bool removeFile(const std::string& name) {
        return currentDirectory->removeChild(name);
    }
    
    // Display commands
    void listCurrentDirectory(bool detailed = false) const {
        currentDirectory->listChildren(detailed);
    }
    
    void listDirectory(const std::string& path, bool detailed = false) {
        std::shared_ptr<Directory> target;
        
        if (path.empty()) {
            target = currentDirectory;
        } else if (path[0] == '/') {
            target = root->navigateTo(path.substr(1));
        } else {
            target = currentDirectory->navigateTo(path);
        }
        
        if (target) {
            target->listChildren(detailed);
        } else {
            std::cout << "Directory not found: " << path << std::endl;
        }
    }
    
    void printWorkingDirectory() const {
        std::cout << workingDirectory << std::endl;
    }
    
    void displayTree(const std::string& path = "") {
        std::shared_ptr<Directory> target;
        
        if (path.empty()) {
            target = currentDirectory;
        } else if (path[0] == '/') {
            target = root->navigateTo(path.substr(1));
        } else {
            target = currentDirectory->navigateTo(path);
        }
        
        if (target) {
            target->displayTree();
        } else {
            std::cout << "Directory not found: " << path << std::endl;
        }
    }
    
    // File operations
    bool readFile(const std::string& filename) {
        auto file = currentDirectory->getFile(filename);
        if (file) {
            file->displayContent();
            return true;
        }
        
        std::cout << "File not found: " << filename << std::endl;
        return false;
    }
    
    bool writeFile(const std::string& filename, const std::string& content) {
        auto file = currentDirectory->getFile(filename);
        if (file) {
            file->setContent(content);
            return true;
        }
        
        std::cout << "File not found: " << filename << std::endl;
        return false;
    }
    
    bool appendToFile(const std::string& filename, const std::string& text) {
        auto file = currentDirectory->getFile(filename);
        if (file) {
            file->appendContent(text);
            return true;
        }
        
        std::cout << "File not found: " << filename << std::endl;
        return false;
    }
    
    // Find command
    std::vector<std::shared_ptr<FileSystemEntity>> find(const std::string& name, bool recursive = true) {
        std::vector<std::shared_ptr<FileSystemEntity>> results;
        findInDirectory(currentDirectory, name, recursive, results);
        return results;
    }
    
    // Utility methods
    void displayFileSystemInfo() const {
        std::cout << "File System Information:" << std::endl;
        std::cout << "Current Directory: " << workingDirectory << std::endl;
        std::cout << "Current Directory Size: " << currentDirectory->getDirectorySize() << " bytes" << std::endl;
        std::cout << "Current Directory Children: " << currentDirectory->getChildCount() << std::endl;
    }
    
private:
    void findInDirectory(std::shared_ptr<Directory> dir, const std::string& name, 
                         bool recursive, std::vector<std::shared_ptr<FileSystemEntity>>& results) {
        if (!dir) {
            return;
        }
        
        // Search in current directory
        for (const auto& pair : dir->getChildren()) {
            if (pair.second->getName() == name) {
                results.push_back(pair.second);
            }
        }
        
        // Recursively search subdirectories
        if (recursive) {
            for (const auto& subdir : dir->getSubdirectories()) {
                findInDirectory(subdir, name, recursive, results);
            }
        }
    }
};

// Shell interface for the file system
class FileShell {
private:
    std::shared_ptr<FileSystem> fs;
    bool running;
    
public:
    FileShell() : fs(std::make_shared<FileSystem>()), running(true) {}
    
    void run() {
        std::cout << "File System Shell" << std::endl;
        std::cout << "Type 'help' for available commands" << std::endl;
        
        while (running) {
            std::cout << fs->getCurrentPath() << " $ ";
            std::string command;
            std::getline(std::cin, command);
            
            if (command.empty()) {
                continue;
            }
            
            executeCommand(command);
        }
        
        std::cout << "Exiting File System Shell" << std::endl;
    }
    
private:
    void executeCommand(const std::string& command) {
        std::stringstream ss(command);
        std::string cmd;
        ss >> cmd;
        
        if (cmd == "help") {
            showHelp();
        } else if (cmd == "ls") {
            std::string arg;
            ss >> arg;
            if (arg == "-l" || arg == "--long") {
                fs->listCurrentDirectory(true);
            } else if (!arg.empty()) {
                fs->listDirectory(arg, false);
            } else {
                fs->listCurrentDirectory(false);
            }
        } else if (cmd == "cd") {
            std::string path;
            ss >> path;
            if (path.empty()) {
                fs->goHome();
            } else if (!fs->changeDirectory(path)) {
                std::cout << "Directory not found: " << path << std::endl;
            }
        } else if (cmd == "pwd") {
            fs->printWorkingDirectory();
        } else if (cmd == "mkdir") {
            std::string name;
            ss >> name;
            if (!name.empty()) {
                auto dir = fs->makeDirectory(name);
                if (!dir) {
                    std::cout << "Failed to create directory: " << name << std::endl;
                }
            }
        } else if (cmd == "touch") {
            std::string name;
            ss >> name;
            if (!name.empty()) {
                auto file = fs->createFile(name);
                if (!file) {
                    std::cout << "Failed to create file: " << name << std::endl;
                }
            }
        } else if (cmd == "cat") {
            std::string filename;
            ss >> filename;
            if (!filename.empty()) {
                fs->readFile(filename);
            }
        } else if (cmd == "tree") {
            std::string path;
            ss >> path;
            fs->displayTree(path);
        } else if (cmd == "find") {
            std::string name;
            ss >> name;
            if (!name.empty()) {
                auto results = fs->find(name);
                for (const auto& entity : results) {
                    std::cout << entity->getFullPath() << std::endl;
                }
            }
        } else if (cmd == "info") {
            fs->displayFileSystemInfo();
        } else if (cmd == "exit" || cmd == "quit") {
            running = false;
        } else {
            std::cout << "Unknown command: " << cmd << std::endl;
            std::cout << "Type 'help' for available commands" << std::endl;
        }
    }
    
    void showHelp() {
        std::cout << "Available commands:" << std::endl;
        std::cout << "  help              - Show this help message" << std::endl;
        std::cout << "  ls [-l] [path]    - List directory contents" << std::endl;
        std::cout << "  cd [path]         - Change directory" << std::endl;
        std::cout << "  pwd               - Print working directory" << std::endl;
        std::cout << "  mkdir <name>      - Create directory" << std::endl;
        std::cout << "  touch <name>      - Create file" << std::endl;
        std::cout << "  cat <file>        - Display file contents" << std::endl;
        std::cout << "  tree [path]       - Display directory tree" << std::endl;
        std::cout << "  find <name>       - Find files/directories" << std::endl;
        std::cout << "  info              - Display file system information" << std::endl;
        std::cout << "  exit/quit         - Exit the shell" << std::endl;
    }
};

int main() {
    // Create file system
    FileSystem fs;
    
    // Create initial structure
    auto homeDir = fs.makeDirectory("home");
    auto docsDir = fs.makeDirectory("documents");
    auto picsDir = fs.makeDirectory("pictures");
    auto musicDir = fs.makeDirectory("music");
    
    // Change to home directory
    fs.changeDirectory("home");
    
    // Create some files
    auto readme = fs.createFile("README.txt", "Welcome to the file system simulation!\n");
    auto notes = fs.createFile("notes.txt", "These are my personal notes.\n");
    
    // Change to documents directory
    fs.changeDirectory("documents");
    
    // Create some documents
    auto report = fs.createFile("report.docx", "This is a report document.\n");
    auto resume = fs.createFile("resume.pdf", "My professional resume.\n");
    auto presentation = fs.createFile("presentation.pptx", "Important presentation slides.\n");
    
    // Go back to home
    fs.goUp();
    
    // Change to pictures directory
    fs.changeDirectory("pictures");
    
    // Create some image files
    auto vacation = fs.createFile("vacation.jpg", "Binary image data");
    auto family = fs.createFile("family.png", "Binary image data");
    
    // Create a subdirectory in pictures
    auto holidayDir = fs.makeDirectory("holiday");
    
    // Go to holiday directory
    fs.changeDirectory("holiday");
    
    // Create some holiday photos
    auto beach = fs.createFile("beach.jpg", "Binary image data");
    auto sunset = fs.createFile("sunset.jpg", "Binary image data");
    
    // Go back to root
    fs.goHome();
    
    // Display file system information
    std::cout << "=== File System Structure ===" << std::endl;
    fs.displayTree();
    
    std::cout << "\n=== Current Directory ===" << std::endl;
    fs.printWorkingDirectory();
    fs.listCurrentDirectory();
    
    std::cout << "\n=== Find Operations ===" << std::endl;
    auto foundFiles = fs.find("vacation.jpg", true);
    std::cout << "Found " << foundFiles.size() << " files matching 'vacation.jpg':" << std::endl;
    for (const auto& file : foundFiles) {
        std::cout << "- " << file->getFullPath() << std::endl;
    }
    
    <｜fim▁hole›>
    // Read a file
    std::cout << "\n=== Reading File Content ===" << std::endl;
    fs.changeDirectory("home");
    fs.readFile("README.txt");
    
    std::cout << "\n=== File System Statistics ===" << std::endl;
    fs.displayFileSystemInfo();
    
    return 0;
}