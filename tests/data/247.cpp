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
#include <random>

/**
 * Task Management System
 * 
 * This C++ program implements a comprehensive task management system
 * with projects, tasks, subtasks, assignments, and deadlines.
 */

// Enum for task status
enum class TaskStatus {
    TODO,
    IN_PROGRESS,
    REVIEW,
    COMPLETED,
    CANCELLED
};

// Enum for task priority
enum class TaskPriority {
    LOW,
    MEDIUM,
    HIGH,
    URGENT
};

// Utility functions
std::string taskStatusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::TODO: return "To Do";
        case TaskStatus::IN_PROGRESS: return "In Progress";
        case TaskStatus::REVIEW: return "Review";
        case TaskStatus::COMPLETED: return "Completed";
        case TaskStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

std::string taskPriorityToString(TaskPriority priority) {
    switch (priority) {
        case TaskPriority::LOW: return "Low";
        case TaskPriority::MEDIUM: return "Medium";
        case TaskPriority::HIGH: return "High";
        case TaskPriority::URGENT: return "Urgent";
        default: return "Unknown";
    }
}

int priorityToInt(TaskPriority priority) {
    switch (priority) {
        case TaskPriority::LOW: return 1;
        case TaskPriority::MEDIUM: return 2;
        case TaskPriority::HIGH: return 3;
        case TaskPriority::URGENT: return 4;
        default: return 0;
    }
}

// Date struct for simplicity (in real code, use proper date library)
struct Date {
    int day;
    int month;
    int year;
    
    Date(int d = 1, int m = 1, int y = 2025) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    // Comparison operators
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    bool operator>(const Date& other) const {
        return other < *this;
    }
    
    // Calculate days between dates (simplified)
    int daysTo(const Date& other) const {
        // Simplified calculation (doesn't account for leap years, etc.)
        int thisDays = year * 365 + month * 30 + day;
        int otherDays = other.year * 365 + other.month * 30 + other.day;
        return otherDays - thisDays;
    }
};

// Forward declarations
class User;
class Project;

// Task class
class Task : public std::enable_shared_from_this<Task> {
private:
    std::string id;
    std::string title;
    std::string description;
    TaskStatus status;
    TaskPriority priority;
    Date creationDate;
    Date dueDate;
    std::shared_ptr<User> assignee;
    std::shared_ptr<Project> project;
    std::vector<std::shared_ptr<Task>> subtasks;
    std::shared_ptr<Task> parentTask;
    std::vector<std::string> tags;
    int estimatedHours;
    int loggedHours;
    
public:
    Task(const std::string& title, const std::string& description = "", 
         TaskPriority priority = TaskPriority::MEDIUM)
        : id("T" + std::to_string(std::rand() % 10000)), title(title), 
          description(description), status(TaskStatus::TODO), priority(priority),
          creationDate(Date()), estimatedHours(0), loggedHours(0) {}
    
    // Getters
    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    TaskStatus getStatus() const { return status; }
    TaskPriority getPriority() const { return priority; }
    Date getCreationDate() const { return creationDate; }
    Date getDueDate() const { return dueDate; }
    std::shared_ptr<User> getAssignee() const { return assignee; }
    std::shared_ptr<Project> getProject() const { return project; }
    std::vector<std::shared_ptr<Task>> getSubtasks() const { return subtasks; }
    std::shared_ptr<Task> getParentTask() const { return parentTask; }
    std::vector<std::string> getTags() const { return tags; }
    int getEstimatedHours() const { return estimatedHours; }
    int getLoggedHours() const { return loggedHours; }
    
    // Setters
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setDescription(const std::string& newDesc) { description = newDesc; }
    void setStatus(TaskStatus newStatus) { status = newStatus; }
    void setPriority(TaskPriority newPriority) { priority = newPriority; }
    void setDueDate(const Date& date) { dueDate = date; }
    void setAssignee(std::shared_ptr<User> user) { assignee = user; }
    void setProject(std::shared_ptr<Project> proj) { project = proj; }
    void setEstimatedHours(int hours) { estimatedHours = hours; }
    
    // Subtask management
    void addSubtask(std::shared_ptr<Task> subtask) {
        subtask->parentTask = shared_from_this();
        subtasks.push_back(subtask);
    }
    
    void removeSubtask(const std::string& taskId) {
        subtasks.erase(
            std::remove_if(subtasks.begin(), subtasks.end(),
                          [taskId](const std::shared_ptr<Task>& task) {
                              return task->getId() == taskId;
                          }),
            subtasks.end()
        );
    }
    
    bool hasSubtasks() const { return !subtasks.empty(); }
    
    // Tag management
    void addTag(const std::string& tag) {
        if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
        }
    }
    
    void removeTag(const std::string& tag) {
        tags.erase(
            std::remove(tags.begin(), tags.end(), tag),
            tags.end()
        );
    }
    
    // Time tracking
    void logHours(int hours) {
        loggedHours += hours;
    }
    
    int getRemainingHours() const {
        return std::max(0, estimatedHours - loggedHours);
    }
    
    // Progress calculation
    float getProgress() const {
        if (estimatedHours <= 0) return 0.0f;
        return std::min(100.0f, (static_cast<float>(loggedHours) / estimatedHours) * 100.0f);
    }
    
    bool isOverdue() const {
        if (status == TaskStatus::COMPLETED || status == TaskStatus::CANCELLED) {
            return false;
        }
        
        Date today = Date();
        return today > dueDate;
    }
    
    // Display methods
    void displayDetails(bool showSubtasks = true) const {
        std::cout << "Task ID: " << id << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Status: " << taskStatusToString(status) << std::endl;
        std::cout << "Priority: " << taskPriorityToString(priority) << std::endl;
        std::cout << "Created: " << creationDate.toString() << std::endl;
        std::cout << "Due: " << dueDate.toString() << std::endl;
        
        if (assignee) {
            std::cout << "Assignee: " << assignee->getName() << std::endl;
        } else {
            std::cout << "Assignee: Unassigned" << std::endl;
        }
        
        if (project) {
            std::cout << "Project: " << project->getName() << std::endl;
        }
        
        if (estimatedHours > 0) {
            std::cout << "Time: " << loggedHours << " / " << estimatedHours 
                      << " hours (" << getProgress() << "%)" << std::endl;
        }
        
        if (isOverdue()) {
            std::cout << "STATUS: OVERDUE!" << std::endl;
        }
        
        if (!tags.empty()) {
            std::cout << "Tags: ";
            for (const auto& tag : tags) {
                std::cout << "#" << tag << " ";
            }
            std::cout << std::endl;
        }
        
        if (showSubtasks && !subtasks.empty()) {
            std::cout << "\nSubtasks (" << subtasks.size() << "):" << std::endl;
            for (const auto& subtask : subtasks) {
                std::cout << "  - " << subtask->getTitle() << " (" 
                          << taskStatusToString(subtask->getStatus()) << ")" << std::endl;
            }
        }
    }
    
    std::string getSummary() const {
        return title + " (" + taskStatusToString(status) + ", " + 
               taskPriorityToString(priority) + ")";
    }
};

// User class
class User : public std::enable_shared_from_this<User> {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string department;
    std::vector<std::shared_ptr<Task>> assignedTasks;
    std::vector<std::shared_ptr<Project>> projects;
    
public:
    User(const std::string& name, const std::string& email, const std::string& department = "")
        : id("U" + std::to_string(std::rand() % 10000)), name(name), 
          email(email), department(department) {}
    
    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getDepartment() const { return department; }
    std::vector<std::shared_ptr<Task>> getAssignedTasks() const { return assignedTasks; }
    std::vector<std::shared_ptr<Project>> getProjects() const { return projects; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setDepartment(const std::string& newDept) { department = newDept; }
    
    // Task assignment
    void assignTask(std::shared_ptr<Task> task) {
        task->setAssignee(shared_from_this());
        assignedTasks.push_back(task);
    }
    
    void unassignTask(const std::string& taskId) {
        assignedTasks.erase(
            std::remove_if(assignedTasks.begin(), assignedTasks.end(),
                          [taskId](const std::shared_ptr<Task>& task) {
                              if (task->getId() == taskId) {
                                  task->setAssignee(nullptr);
                                  return true;
                              }
                              return false;
                          }),
            assignedTasks.end()
        );
    }
    
    // Project participation
    void addProject(std::shared_ptr<Project> project) {
        if (std::find(projects.begin(), projects.end(), project) == projects.end()) {
            projects.push_back(project);
        }
    }
    
    void removeProject(std::shared_ptr<Project> project) {
        projects.erase(
            std::remove(projects.begin(), projects.end(), project),
            projects.end()
        );
    }
    
    // Task statistics
    int getTaskCountByStatus(TaskStatus status) const {
        return std::count_if(assignedTasks.begin(), assignedTasks.end(),
                           [status](const std::shared_ptr<Task>& task) {
                               return task->getStatus() == status;
                           });
    }
    
    int getOverdueTaskCount() const {
        return std::count_if(assignedTasks.begin(), assignedTasks.end(),
                           [](const std::shared_ptr<Task>& task) {
                               return task->isOverdue();
                           });
    }
    
    int getTotalTaskCount() const {
        return assignedTasks.size();
    }
    
    // Display methods
    void displayDetails() const {
        std::cout << "User ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Department: " << department << std::endl;
        std::cout << "Assigned Tasks: " << assignedTasks.size() << std::endl;
        std::cout << "Projects: " << projects.size() << std::endl;
    }
    
    void displayTasks() const {
        std::cout << name << "'s Tasks (" << assignedTasks.size() << "):" << std::endl;
        for (const auto& task : assignedTasks) {
            std::cout << "- " << task->getSummary() << std::endl;
        }
    }
};

// Project class
class Project : public std::enable_shared_from_this<Project> {
private:
    std::string id;
    std::string name;
    std::string description;
    Date startDate;
    Date endDate;
    std::shared_ptr<User> manager;
    std::vector<std::shared_ptr<User>> teamMembers;
    std::vector<std::shared_ptr<Task>> tasks;
    TaskStatus overallStatus;
    
public:
    Project(const std::string& name, const std::string& description = "")
        : id("P" + std::to_string(std::rand() % 10000)), name(name), 
          description(description), overallStatus(TaskStatus::TODO) {}
    
    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    std::shared_ptr<User> getManager() const { return manager; }
    std::vector<std::shared_ptr<User>> getTeamMembers() const { return teamMembers; }
    std::vector<std::shared_ptr<Task>> getTasks() const { return tasks; }
    TaskStatus getOverallStatus() const { return overallStatus; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDesc) { description = newDesc; }
    void setStartDate(const Date& date) { startDate = date; }
    void setEndDate(const Date& date) { endDate = date; }
    void setManager(std::shared_ptr<User> user) { manager = user; }
    
    // Team management
    void addTeamMember(std::shared_ptr<User> user) {
        if (std::find(teamMembers.begin(), teamMembers.end(), user) == teamMembers.end()) {
            teamMembers.push_back(user);
            user->addProject(shared_from_this());
        }
    }
    
    void removeTeamMember(std::shared_ptr<User> user) {
        teamMembers.erase(
            std::remove(teamMembers.begin(), teamMembers.end(), user),
            teamMembers.end()
        );
        user->removeProject(shared_from_this());
    }
    
    // Task management
    void addTask(std::shared_ptr<Task> task) {
        task->setProject(shared_from_this());
        tasks.push_back(task);
        updateOverallStatus();
    }
    
    void removeTask(const std::string& taskId) {
        tasks.erase(
            std::remove_if(tasks.begin(), tasks.end(),
                          [taskId](const std::shared_ptr<Task>& task) {
                              if (task->getId() == taskId) {
                                  task->setProject(nullptr);
                                  return true;
                              }
                              return false;
                          }),
            tasks.end()
        );
        updateOverallStatus();
    }
    
    // Status calculation
    void updateOverallStatus() {
        if (tasks.empty()) {
            overallStatus = TaskStatus::TODO;
            return;
        }
        
        // Count tasks by status
        int completedCount = 0;
        int cancelledCount = 0;
        
        for (const auto& task : tasks) {
            switch (task->getStatus()) {
                case TaskStatus::COMPLETED:
                    completedCount++;
                    break;
                case TaskStatus::CANCELLED:
                    cancelledCount++;
                    break;
                default:
                    break;
            }
        }
        
        // Determine overall status
        if (completedCount == tasks.size()) {
            overallStatus = TaskStatus::COMPLETED;
        } else if (cancelledCount == tasks.size()) {
            overallStatus = TaskStatus::CANCELLED;
        } else if (completedCount > 0) {
            overallStatus = TaskStatus::IN_PROGRESS;
        } else {
            overallStatus = TaskStatus::TODO;
        }
    }
    
    // Progress calculation
    float getProgress() const {
        if (tasks.empty()) return 0.0f;
        
        int completedCount = 0;
        for (const auto& task : tasks) {
            if (task->getStatus() == TaskStatus::COMPLETED) {
                completedCount++;
            }
        }
        
        return (static_cast<float>(completedCount) / tasks.size()) * 100.0f;
    }
    
    // Display methods
    void displayDetails() const {
        std::cout << "Project ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Status: " << taskStatusToString(overallStatus) << std::endl;
        std::cout << "Progress: " << getProgress() << "%" << std::endl;
        
        if (startDate.year > 0) {
            std::cout << "Start: " << startDate.toString() << std::endl;
        }
        
        if (endDate.year > 0) {
            std::cout << "End: " << endDate.toString() << std::endl;
        }
        
        if (manager) {
            std::cout << "Manager: " << manager->getName() << std::endl;
        }
        
        std::cout << "Team Members: " << teamMembers.size() << std::endl;
        std::cout << "Tasks: " << tasks.size() << std::endl;
    }
    
    void displayTeamMembers() const {
        std::cout << "Team Members for " << name << ":" << std::endl;
        if (manager) {
            std::cout << "- " << manager->getName() << " (Manager)" << std::endl;
        }
        
        for (const auto& member : teamMembers) {
            std::cout << "- " << member->getName() << std::endl;
        }
    }
    
    void displayTasks() const {
        std::cout << "Tasks for " << name << " (" << tasks.size() << "):" << std::endl;
        for (const auto& task : tasks) {
            std::cout << "- " << task->getSummary() << std::endl;
        }
    }
};

// Task Management System class
class TaskManagementSystem {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::unordered_map<std::string, std::shared_ptr<Project>> projects;
    std::unordered_map<std::string, std::shared_ptr<Task>> tasks;
    
public:
    // User management
    std::shared_ptr<User> createUser(const std::string& name, const std::string& email, 
                                   const std::string& department = "") {
        auto user = std::make_shared<User>(name, email, department);
        users[user->getId()] = user;
        return user;
    }
    
    std::shared_ptr<User> getUser(const std::string& userId) {
        if (users.find(userId) != users.end()) {
            return users[userId];
        }
        return nullptr;
    }
    
    // Project management
    std::shared_ptr<Project> createProject(const std::string& name, const std::string& description = "") {
        auto project = std::make_shared<Project>(name, description);
        projects[project->getId()] = project;
        return project;
    }
    
    std::shared_ptr<Project> getProject(const std::string& projectId) {
        if (projects.find(projectId) != projects.end()) {
            return projects[projectId];
        }
        return nullptr;
    }
    
    // Task management
    std::shared_ptr<Task> createTask(const std::string& title, const std::string& description = "", 
                                   TaskPriority priority = TaskPriority::MEDIUM) {
        auto task = std::make_shared<Task>(title, description, priority);
        tasks[task->getId()] = task;
        return task;
    }
    
    std::shared_ptr<Task> getTask(const std::string& taskId) {
        if (tasks.find(taskId) != tasks.end()) {
            return tasks[taskId];
        }
        return nullptr;
    }
    
    // Search functionality
    std::vector<std::shared_ptr<Task>> searchTasksByStatus(TaskStatus status) {
        std::vector<std::shared_ptr<Task>> results;
        for (const auto& pair : tasks) {
            if (pair.second->getStatus() == status) {
                results.push_back(pair.second);
            }
        }
        return results;
    }
    
    std::vector<std::shared_ptr<Task>> searchTasksByPriority(TaskPriority priority) {
        std::vector<std::shared_ptr<Task>> results;
        for (const auto& pair : tasks) {
            if (pair.second->getPriority() == priority) {
                results.push_back(pair.second);
            }
        }
        return results;
    }
    
    std::vector<std::shared_ptr<Task>> searchTasksByAssignee(const std::string& userId) {
        std::vector<std::shared_ptr<Task>> results;
        for (const auto& pair : tasks) {
            auto assignee = pair.second->getAssignee();
            if (assignee && assignee->getId() == userId) {
                results.push_back(pair.second);
            }
        }
        return results;
    }
    
    std::vector<std::shared_ptr<Task>> searchOverdueTasks() {
        std::vector<std::shared_ptr<Task>> results;
        for (const auto& pair : tasks) {
            if (pair.second->isOverdue()) {
                results.push_back(pair.second);
            }
        }
        return results;
    }
    
    std::vector<std::shared_ptr<Task>> searchTasksByKeyword(const std::string& keyword) {
        std::vector<std::shared_ptr<Task>> results;
        std::string lowerKeyword = keyword;
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
        
        for (const auto& pair : tasks) {
            const auto& task = pair.second;
            
            // Check title and description
            std::string title = task->getTitle();
            std::string description = task->getDescription();
            std::transform(title.begin(), title.end(), title.begin(), ::tolower);
            std::transform(description.begin(), description.end(), description.begin(), ::tolower);
            
            if (title.find(lowerKeyword) != std::string::npos ||
                description.find(lowerKeyword) != std::string::npos) {
                results.push_back(task);
                continue;
            }
            
            // Check tags
            for (const auto& tag : task->getTags()) {
                std::string lowerTag = tag;
                std::transform(lowerTag.begin(), lowerTag.end(), lowerTag.begin(), ::tolower);
                if (lowerTag.find(lowerKeyword) != std::string::npos) {
                    results.push_back(task);
                    break;
                }
            }
        }
        
        return results;
    }
    
    // Reporting
    void generateUserWorkloadReport() const {
        std::cout << "=== User Workload Report ===" << std::endl;
        std::cout << "User\t\tTotal Tasks\tTodo\tIn Progress\tReview\tCompleted\tOverdue" << std::endl;
        std::cout << "--------------------------------------------------------------------" << std::endl;
        
        for (const auto& pair : users) {
            const auto& user = pair.second;
            
            int todoCount = user->getTaskCountByStatus(TaskStatus::TODO);
            int inProgressCount = user->getTaskCountByStatus(TaskStatus::IN_PROGRESS);
            int reviewCount = user->getTaskCountByStatus(TaskStatus::REVIEW);
            int completedCount = user->getTaskCountByStatus(TaskStatus::COMPLETED);
            int overdueCount = user->getOverdueTaskCount();
            int totalCount = user->getTotalTaskCount();
            
            std::cout << user->getName() << "\t" << totalCount << "\t\t"
                      << todoCount << "\t" << inProgressCount << "\t"
                      << reviewCount << "\t" << completedCount << "\t"
                      << overdueCount << std::endl;
        }
    }
    
    void generateProjectProgressReport() const {
        std::cout << "\n=== Project Progress Report ===" << std::endl;
        std::cout << "Project\t\tProgress\tStatus\tManager\t\tTeam Size" << std::endl;
        std::cout << "--------------------------------------------------------------------" << std::endl;
        
        for (const auto& pair : projects) {
            const auto& project = pair.second;
            
            std::cout << project->getName() << "\t" << project->getProgress() << "%\t\t"
                      << taskStatusToString(project->getOverallStatus()) << "\t"
                      << (project->getManager() ? project->getManager()->getName() : "Unassigned") << "\t\t"
                      << project->getTeamMembers().size() << std::endl;
        }
    }
    
    void displaySystemStats() const {
        std::cout << "\n=== System Statistics ===" << std::endl;
        std::cout << "Total Users: " << users.size() << std::endl;
        std::cout << "Total Projects: " << projects.size() << std::endl;
        std::cout << "Total Tasks: " << tasks.size() << std::endl;
        
        // Task status breakdown
        int todoCount = 0, inProgressCount = 0, reviewCount = 0, completedCount = 0, cancelledCount = 0;
        
        for (const auto& pair : tasks) {
            switch (pair.second->getStatus()) {
                case TaskStatus::TODO:
                    todoCount++;
                    break;
                case TaskStatus::IN_PROGRESS:
                    inProgressCount++;
                    break;
                case TaskStatus::REVIEW:
                    reviewCount++;
                    break;
                case TaskStatus::COMPLETED:
                    completedCount++;
                    break;
                case TaskStatus::CANCELLED:
                    cancelledCount++;
                    break;
            }
        }
        
        std::cout << "\nTask Status Breakdown:" << std::endl;
        std::cout << "To Do: " << todoCount << std::endl;
        std::cout << "In Progress: " << inProgressCount << std::endl;
        std::cout << "Review: " << reviewCount << std::endl;
        std::cout << "Completed: " << completedCount << std::endl;
        std::cout << "Cancelled: " << cancelledCount << std::endl;
    }
};

int main() {
    // Create task management system
    TaskManagementSystem system;
    
    // Create users
    auto john = system.createUser("John Smith", "john@example.com", "Engineering");
    auto jane = system.createUser("Jane Doe", "jane@example.com", "Design");
    auto bob = system.createUser("Bob Johnson", "bob@example.com", "Marketing");
    auto alice = system.createUser("Alice Williams", "alice@example.com", "Engineering");
    
    // Create projects
    auto websiteProject = system.createProject("Website Redesign", "Redesign company website with modern UI");
    auto mobileAppProject = system.createProject("Mobile App", "Develop new mobile application");
    
    // Set project managers
    websiteProject->setManager(jane);
    mobileAppProject->setManager(john);
    
    // Add team members
    websiteProject->addTeamMember(john);
    websiteProject->addTeamMember(alice);
    
    mobileAppProject->addTeamMember(jane);
    mobileAppProject->addTeamMember(bob);
    mobileAppProject->addTeamMember(alice);
    
    // Set project dates
    websiteProject->setStartDate(Date(1, 12, 2025));
    websiteProject->setEndDate(Date(31, 1, 2026));
    
    mobileAppProject->setStartDate(Date(15, 11, 2025));
    mobileAppProject->setEndDate(Date(15, 3, 2026));
    
    // Create tasks for website project
    auto uiDesign = system.createTask("UI Design", "Create modern UI design mockups", TaskPriority::HIGH);
    uiDesign->setProject(websiteProject);
    uiDesign->setAssignee(jane);
    uiDesign->setDueDate(Date(15, 12, 2025));
    uiDesign->setEstimatedHours(40);
    uiDesign->addTag("design");
    uiDesign->addTag("ui");
    
    auto backendDev = system.createTask("Backend Development", "Implement new backend services", TaskPriority::HIGH);
    backendDev->setProject(websiteProject);
    backendDev->setAssignee(john);
    backendDev->setDueDate(Date(20, 12, 2025));
    backendDev->setEstimatedHours(60);
    backendDev->addTag("development");
    backendDev->addTag("backend");
    
    auto frontendDev = system.createTask("Frontend Development", "Implement frontend components", TaskPriority::MEDIUM);
    frontendDev->setProject(websiteProject);
    frontendDev->setAssignee(alice);
    frontendDev->setDueDate(Date(25, 12, 2025));
    frontendDev->setEstimatedHours(50);
    frontendDev->addTag("development");
    frontendDev->addTag("frontend");
    
    // Add to project
    websiteProject->addTask(uiDesign);
    websiteProject->addTask(backendDev);
    websiteProject->addTask(frontendDev);
    
    // Create tasks for mobile app project
    auto research = system.createTask("Market Research", "Research competitor apps and market trends", TaskPriority::MEDIUM);
    research->setProject(mobileAppProject);
    research->setAssignee(bob);
    research->setDueDate(Date(30, 11, 2025));
    research->setEstimatedHours(20);
    research->addTag("research");
    
    <｜fim▁hole›>
    auto prototype = system.createTask("App Prototype", "Create interactive prototype", TaskPriority::HIGH);
    prototype->setProject(mobileAppProject);
    prototype->setAssignee(jane);
    prototype->setDueDate(Date(15, 12, 2025));
    prototype->setEstimatedHours(30);
    prototype->addTag("design");
    prototype->addTag("prototype");
    
    // Add to project
    mobileAppProject->addTask(research);
    mobileAppProject->addTask(prototype);
    
    // Create subtasks
    auto backendSubtask1 = system.createTask("Database Design", "Design database schema");
    backendSubtask1->setAssignee(john);
    backendSubtask1->setDueDate(Date(17, 12, 2025));
    backendSubtask1->setStatus(TaskStatus::IN_PROGRESS);
    backendSubtask1->setEstimatedHours(15);
    backendDev->addSubtask(backendSubtask1);
    
    auto backendSubtask2 = system.createTask("API Development", "Develop RESTful API endpoints");
    backendSubtask2->setAssignee(john);
    backendSubtask2->setDueDate(Date(20, 12, 2025));
    backendSubtask2->setEstimatedHours(25);
    backendDev->addSubtask(backendSubtask2);
    
    // Update task progress
    research->setStatus(TaskStatus::IN_PROGRESS);
    research->logHours(10);
    
    prototype->setStatus(TaskStatus::TODO);
    
    uiDesign->setStatus(TaskStatus::COMPLETED);
    uiDesign->logHours(45);  // Overestimated but completed
    
    frontendDev->setStatus(TaskStatus::IN_PROGRESS);
    frontendDev->logHours(20);
    
    // Display system information
    std::cout << "=== Task Management System ===" << std::endl;
    
    // Display projects
    std::cout << "\n=== Projects ===" << std::endl;
    websiteProject->displayDetails();
    std::cout << std::endl;
    mobileAppProject->displayDetails();
    
    // Display project tasks
    std::cout << "\n=== Website Project Tasks ===" << std::endl;
    websiteProject->displayTasks();
    
    std::cout << "\n=== Mobile App Project Tasks ===" << std::endl;
    mobileAppProject->displayTasks();
    
    // Display detailed task information
    std::cout << "\n=== Detailed Task Information ===" << std::endl;
    backendDev->displayDetails();
    
    // Display user workload
    std::cout << "\n=== User Information ===" << std::endl;
    john->displayTasks();
    std::cout << std::endl;
    jane->displayTasks();
    
    // Search functionality
    std::cout << "\n=== Search Results ===" << std::endl;
    auto highPriorityTasks = system.searchTasksByPriority(TaskPriority::HIGH);
    std::cout << "High priority tasks (" << highPriorityTasks.size() << "):" << std::endl;
    for (const auto& task : highPriorityTasks) {
        std::cout << "- " << task->getSummary() << std::endl;
    }
    
    auto inProgressTasks = system.searchTasksByStatus(TaskStatus::IN_PROGRESS);
    std::cout << "\nIn progress tasks (" << inProgressTasks.size() << "):" << std::endl;
    for (const auto& task : inProgressTasks) {
        std::cout << "- " << task->getSummary() << std::endl;
    }
    
    auto designTasks = system.searchTasksByKeyword("design");
    std::cout << "\nDesign-related tasks (" << designTasks.size() << "):" << std::endl;
    for (const auto& task : designTasks) {
        std::cout << "- " << task->getSummary() << std::endl;
    }
    
    // Generate reports
    system.generateUserWorkloadReport();
    system.generateProjectProgressReport();
    system.displaySystemStats();
    
    return 0;
}