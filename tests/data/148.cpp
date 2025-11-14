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

#define MAX_TASKS 500
#define MAX_USERS 100
#define MAX_PROJECTS 50
#define MAX_COMMENTS 1000

struct date {
    int year;
    int month;
    int day;
};

class User;
class Task;
class Comment;
class Project;

class User {
private:
    int userId;
    string username;
    string email;
    string firstName;
    string lastName;
    string role;
    date joinDate;
    bool isActive;
    vector<int> assignedTaskIds;
    vector<int> createdTaskIds;
    
public:
    User(int userId = 0, const string& username = "", const string& email = "", 
         const string& firstName = "", const string& lastName = "", const string& role = "Developer",
         date joinDate = {0, 0, 0}, bool isActive = true)
        : userId(userId), username(username), email(email), firstName(firstName), 
          lastName(lastName), role(role), joinDate(joinDate), isActive(isActive) {}
    
    int getUserId() const { return userId; }
    string getUsername() const { return username; }
    string getEmail() const { return email; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getRole() const { return role; }
    date getJoinDate() const { return joinDate; }
    bool getIsActive() const { return isActive; }
    vector<int> getAssignedTaskIds() const { return assignedTaskIds; }
    vector<int> getCreatedTaskIds() const { return createdTaskIds; }
    
    void setUserId(int id) { userId = id; }
    void setUsername(const string& username) { this->username = username; }
    void setEmail(const string& email) { this->email = email; }
    void setFirstName(const string& firstName) { this->firstName = firstName; }
    void setLastName(const string& lastName) { this->lastName = lastName; }
    void setRole(const string& role) { this->role = role; }
    void setJoinDate(date date) { joinDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    
    void addAssignedTask(int taskId) {
        if (find(assignedTaskIds.begin(), assignedTaskIds.end(), taskId) == assignedTaskIds.end()) {
            assignedTaskIds.push_back(taskId);
        }
    }
    
    void removeAssignedTask(int taskId) {
        auto it = find(assignedTaskIds.begin(), assignedTaskIds.end(), taskId);
        if (it != assignedTaskIds.end()) {
            assignedTaskIds.erase(it);
        }
    }
    
    void addCreatedTask(int taskId) {
        if (find(createdTaskIds.begin(), createdTaskIds.end(), taskId) == createdTaskIds.end()) {
            createdTaskIds.push_back(taskId);
        }
    }
    
    void removeCreatedTask(int taskId) {
        auto it = find(createdTaskIds.begin(), createdTaskIds.end(), taskId);
        if (it != createdTaskIds.end()) {
            createdTaskIds.erase(it);
        }
    }
    
    void display() const {
        cout << "User ID: " << userId << endl;
        cout << "Username: " << username << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "Email: " << email << endl;
        cout << "Role: " << role << endl;
        
        // Format date
        tm tm = {};
        tm.tm_year = joinDate.year - 1900;
        tm.tm_mon = joinDate.month - 1;
        tm.tm_mday = joinDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Join Date: " << dateStr << endl;
        
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Assigned Tasks: " << assignedTaskIds.size() << endl;
        cout << "Created Tasks: " << createdTaskIds.size() << endl;
    }
};

class Project {
private:
    int projectId;
    string name;
    string description;
    date startDate;
    date endDate;
    string status; // Planning, Active, On Hold, Completed, Cancelled
    int creatorId;
    vector<int> memberIds;
    vector<int> taskIds;
    
public:
    Project(int projectId = 0, const string& name = "", const string& description = "",
            date startDate = {0, 0, 0}, date endDate = {0, 0, 0}, 
            const string& status = "Planning", int creatorId = 0)
        : projectId(projectId), name(name), description(description), 
          startDate(startDate), endDate(endDate), status(status), creatorId(creatorId) {}
    
    int getProjectId() const { return projectId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    date getStartDate() const { return startDate; }
    date getEndDate() const { return endDate; }
    string getStatus() const { return status; }
    int getCreatorId() const { return creatorId; }
    vector<int> getMemberIds() const { return memberIds; }
    vector<int> getTaskIds() const { return taskIds; }
    
    void setProjectId(int id) { projectId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setStartDate(date date) { startDate = date; }
    void setEndDate(date date) { endDate = date; }
    void setStatus(const string& status) { this->status = status; }
    void setCreatorId(int creatorId) { this->creatorId = creatorId; }
    
    void addMember(int userId) {
        if (find(memberIds.begin(), memberIds.end(), userId) == memberIds.end()) {
            memberIds.push_back(userId);
        }
    }
    
    void removeMember(int userId) {
        auto it = find(memberIds.begin(), memberIds.end(), userId);
        if (it != memberIds.end()) {
            memberIds.erase(it);
        }
    }
    
    void addTask(int taskId) {
        if (find(taskIds.begin(), taskIds.end(), taskId) == taskIds.end()) {
            taskIds.push_back(taskId);
        }
    }
    
    void removeTask(int taskId) {
        auto it = find(taskIds.begin(), taskIds.end(), taskId);
        if (it != taskIds.end()) {
            taskIds.erase(it);
        }
    }
    
    bool isMember(int userId) const {
        return find(memberIds.begin(), memberIds.end(), userId) != memberIds.end();
    }
    
    void display() const {
        cout << "Project ID: " << projectId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        
        // Format dates
        tm tm = {};
        
        tm.tm_year = startDate.year - 1900;
        tm.tm_mon = startDate.month - 1;
        tm.tm_mday = startDate.day;
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Start Date: " << dateStr << endl;
        
        tm.tm_year = endDate.year - 1900;
        tm.tm_mon = endDate.month - 1;
        tm.tm_mday = endDate.day;
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "End Date: " << dateStr << endl;
        
        cout << "Status: " << status << endl;
        cout << "Creator ID: " << creatorId << endl;
        cout << "Members: " << memberIds.size() << endl;
        cout << "Tasks: " << taskIds.size() << endl;
    }
};

enum TaskPriority {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    URGENT = 4
};

enum TaskStatus {
    TODO = 1,
    IN_PROGRESS = 2,
    REVIEW = 3,
    DONE = 4
};

class Task {
private:
    int taskId;
    string title;
    string description;
    TaskPriority priority;
    TaskStatus status;
    date createdDate;
    date dueDate;
    date completedDate;
    int creatorId;
    int assigneeId;
    int projectId;
    vector<int> commentIds;
    int estimatedHours;
    int actualHours;
    
public:
    Task(int taskId = 0, const string& title = "", const string& description = "",
         TaskPriority priority = MEDIUM, TaskStatus status = TODO,
         date createdDate = {0, 0, 0}, date dueDate = {0, 0, 0},
         int creatorId = 0, int assigneeId = 0, int projectId = 0)
        : taskId(taskId), title(title), description(description), 
          priority(priority), status(status), createdDate(createdDate), 
          dueDate(dueDate), creatorId(creatorId), assigneeId(assigneeId), 
          projectId(projectId) {
        completedDate = {0, 0, 0};
        estimatedHours = 0;
        actualHours = 0;
    }
    
    int getTaskId() const { return taskId; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    TaskPriority getPriority() const { return priority; }
    TaskStatus getStatus() const { return status; }
    date getCreatedDate() const { return createdDate; }
    date getDueDate() const { return dueDate; }
    date getCompletedDate() const { return completedDate; }
    int getCreatorId() const { return creatorId; }
    int getAssigneeId() const { return assigneeId; }
    int getProjectId() const { return projectId; }
    vector<int> getCommentIds() const { return commentIds; }
    int getEstimatedHours() const { return estimatedHours; }
    int getActualHours() const { return actualHours; }
    
    void setTaskId(int id) { taskId = id; }
    void setTitle(const string& title) { this->title = title; }
    void setDescription(const string& description) { this->description = description; }
    void setPriority(TaskPriority priority) { this->priority = priority; }
    void setStatus(TaskStatus status) { this->status = status; }
    void setCreatedDate(date date) { createdDate = date; }
    void setDueDate(date date) { dueDate = date; }
    void setCompletedDate(date date) { completedDate = date; }
    void setCreatorId(int creatorId) { this->creatorId = creatorId; }
    void setAssigneeId(int assigneeId) { this->assigneeId = assigneeId; }
    void setProjectId(int projectId) { this->projectId = projectId; }
    void setEstimatedHours(int hours) { estimatedHours = hours; }
    void setActualHours(int hours) { actualHours = hours; }
    
    void addComment(int commentId) {
        if (find(commentIds.begin(), commentIds.end(), commentId) == commentIds.end()) {
            commentIds.push_back(commentId);
        }
    }
    
    void removeComment(int commentId) {
        auto it = find(commentIds.begin(), commentIds.end(), commentId);
        if (it != commentIds.end()) {
            commentIds.erase(it);
        }
    }
    
    string getPriorityString() const {
        switch (priority) {
            case LOW: return "Low";
            case MEDIUM: return "Medium";
            case HIGH: return "High";
            case URGENT: return "Urgent";
            default: return "Unknown";
        }
    }
    
    string getStatusString() const {
        switch (status) {
            case TODO: return "To Do";
            case IN_PROGRESS: return "In Progress";
            case REVIEW: return "Review";
            case DONE: return "Done";
            default: return "Unknown";
        }
    }
    
    bool isOverdue() const {
        if (status == DONE) return false;
        if (dueDate.year == 0) return false;
        
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        if (today.year > dueDate.year) {
            return true;
        } else if (today.year == dueDate.year && today.month > dueDate.month) {
            return true;
        } else if (today.year == dueDate.year && today.month == dueDate.month && today.day > dueDate.day) {
            return true;
        }
        
        return false;
    }
    
    void display() const {
        cout << "Task ID: " << taskId << endl;
        cout << "Title: " << title << endl;
        cout << "Description: " << description << endl;
        cout << "Priority: " << getPriorityString() << endl;
        cout << "Status: " << getStatusString() << endl;
        
        // Format dates
        tm tm = {};
        
        tm.tm_year = createdDate.year - 1900;
        tm.tm_mon = createdDate.month - 1;
        tm.tm_mday = createdDate.day;
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Created Date: " << dateStr << endl;
        
        if (dueDate.year > 0) {
            tm.tm_year = dueDate.year - 1900;
            tm.tm_mon = dueDate.month - 1;
            tm.tm_mday = dueDate.day;
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Due Date: " << dateStr;
            if (isOverdue()) {
                cout << " (OVERDUE)";
            }
            cout << endl;
        } else {
            cout << "Due Date: Not set" << endl;
        }
        
        if (completedDate.year > 0) {
            tm.tm_year = completedDate.year - 1900;
            tm.tm_mon = completedDate.month - 1;
            tm.tm_mday = completedDate.day;
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Completed Date: " << dateStr << endl;
        } else {
            cout << "Completed Date: Not completed" << endl;
        }
        
        cout << "Creator ID: " << creatorId << endl;
        cout << "Assignee ID: " << assigneeId << endl;
        cout << "Project ID: " << projectId << endl;
        cout << "Comments: " << commentIds.size() << endl;
        
        if (estimatedHours > 0) {
            cout << "Estimated Hours: " << estimatedHours << endl;
        }
        
        if (actualHours > 0) {
            cout << "Actual Hours: " << actualHours << endl;
        }
    }
};

class Comment {
private:
    int commentId;
    int taskId;
    int userId;
    string content;
    date createdDate;
    bool isEdited;
    date editedDate;
    
public:
    Comment(int commentId = 0, int taskId = 0, int userId = 0, const string& content = "",
           date createdDate = {0, 0, 0}, bool isEdited = false, date editedDate = {0, 0, 0})
        : commentId(commentId), taskId(taskId), userId(userId), content(content), 
          createdDate(createdDate), isEdited(isEdited), editedDate(editedDate) {}
    
    int getCommentId() const { return commentId; }
    int getTaskId() const { return taskId; }
    int getUserId() const { return userId; }
    string getContent() const { return content; }
    date getCreatedDate() const { return createdDate; }
    bool getIsEdited() const { return isEdited; }
    date getEditedDate() const { return editedDate; }
    
    void setCommentId(int id) { commentId = id; }
    void setTaskId(int taskId) { this->taskId = taskId; }
    void setUserId(int userId) { this->userId = userId; }
    void setContent(const string& content) { this->content = content; }
    void setCreatedDate(date date) { createdDate = date; }
    void setIsEdited(bool isEdited) { this->isEdited = isEdited; }
    void setEditedDate(date date) { editedDate = date; }
    
    void edit(const string& newContent, date editDate) {
        content = newContent;
        isEdited = true;
        editedDate = editDate;
    }
    
    void display() const {
        cout << "Comment ID: " << commentId << endl;
        cout << "Task ID: " << taskId << endl;
        cout << "User ID: " << userId << endl;
        cout << "Content: " << content << endl;
        
        // Format dates
        tm tm = {};
        
        tm.tm_year = createdDate.year - 1900;
        tm.tm_mon = createdDate.month - 1;
        tm.tm_mday = createdDate.day;
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Created Date: " << dateStr << endl;
        
        if (isEdited && editedDate.year > 0) {
            tm.tm_year = editedDate.year - 1900;
            tm.tm_mon = editedDate.month - 1;
            tm.tm_mday = editedDate.day;
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Edited Date: " << dateStr << endl;
        }
    }
};

class TaskManagementSystem {
private:
    vector<User> users;
    vector<Project> projects;
    vector<Task> tasks;
    vector<Comment> comments;
    
    int nextUserId;
    int nextProjectId;
    int nextTaskId;
    int nextCommentId;
    
public:
    TaskManagementSystem() {
        nextUserId = 1;
        nextProjectId = 1;
        nextTaskId = 1;
        nextCommentId = 1;
    }
    
    // User management
    User* findUser(int userId) {
        for (auto& user : users) {
            if (user.getUserId() == userId) {
                return &user;
            }
        }
        return nullptr;
    }
    
    bool addUser(const User& user) {
        if (users.size() >= MAX_USERS) {
            return false;
        }
        
        users.push_back(user);
        return true;
    }
    
    bool removeUser(int userId) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getUserId() == userId) {
                users.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<User*> getAllUsers() {
        vector<User*> result;
        for (auto& user : users) {
            result.push_back(&user);
        }
        return result;
    }
    
    vector<User*> searchUsersByName(const string& name) {
        vector<User*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& user : users) {
            string fullName = user.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&user);
            }
        }
        return result;
    }
    
    int getNextUserId() { return nextUserId++; }
    
    // Project management
    Project* findProject(int projectId) {
        for (auto& project : projects) {
            if (project.getProjectId() == projectId) {
                return &project;
            }
        }
        return nullptr;
    }
    
    bool addProject(const Project& project) {
        if (projects.size() >= MAX_PROJECTS) {
            return false;
        }
        
        projects.push_back(project);
        return true;
    }
    
    bool removeProject(int projectId) {
        for (auto it = projects.begin(); it != projects.end(); ++it) {
            if (it->getProjectId() == projectId) {
                projects.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Project*> getAllProjects() {
        vector<Project*> result;
        for (auto& project : projects) {
            result.push_back(&project);
        }
        return result;
    }
    
    vector<Project*> getProjectsByUser(int userId) {
        vector<Project*> result;
        for (auto& project : projects) {
            if (project.isMember(userId) || project.getCreatorId() == userId) {
                result.push_back(&project);
            }
        }
        return result;
    }
    
    int getNextProjectId() { return nextProjectId++; }
    
    // Task management
    Task* findTask(int taskId) {
        for (auto& task : tasks) {
            if (task.getTaskId() == taskId) {
                return &task;
            }
        }
        return nullptr;
    }
    
    bool addTask(const Task& task) {
        if (tasks.size() >= MAX_TASKS) {
            return false;
        }
        
        tasks.push_back(task);
        return true;
    }
    
    bool removeTask(int taskId) {
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
            if (it->getTaskId() == taskId) {
                tasks.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Task*> getAllTasks() {
        vector<Task*> result;
        for (auto& task : tasks) {
            result.push_back(&task);
        }
        return result;
    }
    
    vector<Task*> getTasksByProject(int projectId) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.getProjectId() == projectId) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByAssignee(int userId) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.getAssigneeId() == userId) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByCreator(int userId) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.getCreatorId() == userId) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByStatus(TaskStatus status) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.getStatus() == status) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByPriority(TaskPriority priority) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.getPriority() == priority) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getOverdueTasks() {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isOverdue()) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> searchTasksByTitle(const string& title) {
        vector<Task*> result;
        string lowerTitle = title;
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        
        for (auto& task : tasks) {
            string taskTitle = task.getTitle();
            transform(taskTitle.begin(), taskTitle.end(), taskTitle.begin(), ::tolower);
            
            if (taskTitle.find(lowerTitle) != string::npos) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    int getNextTaskId() { return nextTaskId++; }
    
    // Comment management
    Comment* findComment(int commentId) {
        for (auto& comment : comments) {
            if (comment.getCommentId() == commentId) {
                return &comment;
            }
        }
        return nullptr;
    }
    
    bool addComment(const Comment& comment) {
        if (comments.size() >= MAX_COMMENTS) {
            return false;
        }
        
        comments.push_back(comment);
        return true;
    }
    
    bool removeComment(int commentId) {
        for (auto it = comments.begin(); it != comments.end(); ++it) {
            if (it->getCommentId() == commentId) {
                comments.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Comment*> getCommentsByTask(int taskId) {
        vector<Comment*> result;
        for (auto& comment : comments) {
            if (comment.getTaskId() == taskId) {
                result.push_back(&comment);
            }
        }
        return result;
    }
    
    int getNextCommentId() { return nextCommentId++; }
    
    // Business operations
    bool assignTask(int taskId, int userId) {
        Task* task = findTask(taskId);
        User* user = findUser(userId);
        
        if (!task || !user) {
            return false;
        }
        
        // Remove task from previous assignee if exists
        if (task->getAssigneeId() > 0) {
            User* previousAssignee = findUser(task->getAssigneeId());
            if (previousAssignee) {
                previousAssignee->removeAssignedTask(taskId);
            }
        }
        
        // Assign task to new user
        task->setAssigneeId(userId);
        user->addAssignedTask(taskId);
        
        return true;
    }
    
    bool completeTask(int taskId, int actualHours = 0) {
        Task* task = findTask(taskId);
        if (!task) {
            return false;
        }
        
        task->setStatus(DONE);
        task->setActualHours(actualHours);
        
        // Set completion date to today
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date completedDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        task->setCompletedDate(completedDate);
        
        return true;
    }
    
    bool addCommentToTask(int taskId, int userId, const string& content) {
        Task* task = findTask(taskId);
        User* user = findUser(userId);
        
        if (!task || !user) {
            return false;
        }
        
        // Get current date
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date createdDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Comment comment(getNextCommentId(), taskId, userId, content, createdDate);
        
        if (!addComment(comment)) {
            return false;
        }
        
        task->addComment(comment.getCommentId());
        
        return true;
    }
    
    bool addMemberToProject(int projectId, int userId) {
        Project* project = findProject(projectId);
        User* user = findUser(userId);
        
        if (!project || !user) {
            return false;
        }
        
        project->addMember(userId);
        
        return true;
    }
    
    bool removeMemberFromProject(int projectId, int userId) {
        Project* project = findProject(projectId);
        if (!project) {
            return false;
        }
        
        project->removeMember(userId);
        
        // Unassign user from tasks in this project
        vector<Task*> projectTasks = getTasksByProject(projectId);
        for (Task* task : projectTasks) {
            if (task->getAssigneeId() == userId) {
                User* user = findUser(userId);
                if (user) {
                    user->removeAssignedTask(task->getTaskId());
                }
                task->setAssigneeId(0);
            }
        }
        
        return true;
    }
    
    // Reporting
    void generateUserReport(int userId) {
        User* user = findUser(userId);
        if (!user) {
            cout << "User not found." << endl;
            return;
        }
        
        cout << "User Report" << endl;
        cout << "============" << endl;
        user->display();
        cout << endl;
        
        // Display assigned tasks
        vector<Task*> assignedTasks = getTasksByAssignee(userId);
        if (!assignedTasks.empty()) {
            cout << "Assigned Tasks:" << endl;
            for (Task* task : assignedTasks) {
                cout << "  ID: " << task->getTaskId()
                     << ", Title: " << task->getTitle()
                     << ", Priority: " << task->getPriorityString()
                     << ", Status: " << task->getStatusString();
                
                if (task->isOverdue()) {
                    cout << " (OVERDUE)";
                }
                cout << endl;
            }
        } else {
            cout << "No assigned tasks." << endl;
        }
        
        // Display created tasks
        vector<Task*> createdTasks = getTasksByCreator(userId);
        if (!createdTasks.empty()) {
            cout << endl << "Created Tasks:" << endl;
            for (Task* task : createdTasks) {
                cout << "  ID: " << task->getTaskId()
                     << ", Title: " << task->getTitle()
                     << ", Status: " << task->getStatusString() << endl;
            }
        } else {
            cout << endl << "No created tasks." << endl;
        }
        
        // Display projects
        vector<Project*> userProjects = getProjectsByUser(userId);
        if (!userProjects.empty()) {
            cout << endl << "Projects:" << endl;
            for (Project* project : userProjects) {
                cout << "  ID: " << project->getProjectId()
                     << ", Name: " << project->getName()
                     << ", Status: " << project->getStatus() << endl;
            }
        } else {
            cout << endl << "No projects." << endl;
        }
    }
    
    void generateProjectReport(int projectId) {
        Project* project = findProject(projectId);
        if (!project) {
            cout << "Project not found." << endl;
            return;
        }
        
        cout << "Project Report" << endl;
        cout << "===============" << endl;
        project->display();
        cout << endl;
        
        // Display project tasks
        vector<Task*> projectTasks = getTasksByProject(projectId);
        if (!projectTasks.empty()) {
            cout << "Project Tasks:" << endl;
            
            // Group tasks by status
            map<TaskStatus, vector<Task*>> tasksByStatus;
            for (Task* task : projectTasks) {
                tasksByStatus[task->getStatus()].push_back(task);
            }
            
            for (const auto& pair : tasksByStatus) {
                cout << endl << "Status: " << Task().getStatusStringStatic(pair.first) << " (" << pair.second.size() << ")" << endl;
                for (Task* task : pair.second) {
                    cout << "  ID: " << task->getTaskId()
                         << ", Title: " << task->getTitle()
                         << ", Priority: " << task->getPriorityString();
                    
                    if (task->isOverdue()) {
                        cout << " (OVERDUE)";
                    }
                    cout << endl;
                }
            }
        } else {
            cout << "No tasks in this project." << endl;
        }
        
        // Calculate statistics
        if (!projectTasks.empty()) {
            int todoCount = 0;
            int inProgressCount = 0;
            int reviewCount = 0;
            int doneCount = 0;
            int overdueCount = 0;
            
            for (Task* task : projectTasks) {
                switch (task->getStatus()) {
                    case TODO: todoCount++; break;
                    case IN_PROGRESS: inProgressCount++; break;
                    case REVIEW: reviewCount++; break;
                    case DONE: doneCount++; break;
                }
                
                if (task->isOverdue()) {
                    overdueCount++;
                }
            }
            
            cout << endl << "Project Statistics:" << endl;
            cout << "Total Tasks: " << projectTasks.size() << endl;
            cout << "To Do: " << todoCount << endl;
            cout << "In Progress: " << inProgressCount << endl;
            cout << "Review: " << reviewCount << endl;
            cout << "Done: " << doneCount << endl;
            cout << "Overdue: " << overdueCount << endl;
            
            if (doneCount > 0) {
                double completionRate = (double)doneCount / projectTasks.size() * 100.0;
                cout << "Completion Rate: " << fixed << setprecision(1) << completionRate << "%" << endl;
            } else {
                cout << "Completion Rate: 0.0%" << endl;
            }
        }
        
        // Display project members
        cout << endl << "Project Members:" << endl;
        cout << "Creator ID: " << project->getCreatorId() << endl;
        
        vector<int> memberIds = project->getMemberIds();
        if (!memberIds.empty()) {
            for (int memberId : memberIds) {
                User* member = findUser(memberId);
                if (member) {
                    cout << "  ID: " << member->getUserId()
                         << ", Name: " << member->getFullName()
                         << ", Role: " << member->getRole() << endl;
                }
            }
        } else {
            cout << "No additional members." << endl;
        }
    }
    
    void generateTaskReport(int taskId) {
        Task* task = findTask(taskId);
        if (!task) {
            cout << "Task not found." << endl;
            return;
        }
        
        cout << "Task Report" << endl;
        cout << "===========" << endl;
        task->display();
        cout << endl;
        
        // Display task details
        User* creator = findUser(task->getCreatorId());
        if (creator) {
            cout << "Creator: " << creator->getFullName() << endl;
        }
        
        User* assignee = findUser(task->getAssigneeId());
        if (assignee) {
            cout << "Assignee: " << assignee->getFullName() << endl;
        }
        
        Project* project = findProject(task->getProjectId());
        if (project) {
            cout << "Project: " << project->getName() << endl;
        }
        
        // Display comments
        vector<Comment*> taskComments = getCommentsByTask(taskId);
        if (!taskComments.empty()) {
            cout << endl << "Comments (" << taskComments.size() << "):" << endl;
            for (Comment* comment : taskComments) {
                User* commentUser = findUser(comment->getUserId());
                cout << "  Comment by " << (commentUser ? commentUser->getFullName() : "Unknown User") << ":" << endl;
                cout << "  " << comment->getContent() << endl;
                
                // Format date
                tm tm = {};
                tm.tm_year = comment->getCreatedDate().year - 1900;
                tm.tm_mon = comment->getCreatedDate().month - 1;
                tm.tm_mday = comment->getCreatedDate().day;
                char dateStr[100];
                strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
                cout << "  Posted on: " << dateStr;
                
                if (comment->getIsEdited()) {
                    tm.tm_year = comment->getEditedDate().year - 1900;
                    tm.tm_mon = comment->getEditedDate().month - 1;
                    tm.tm_mday = comment->getEditedDate().day;
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
                    cout << " (Edited on: " << dateStr << ")";
                }
                cout << endl << endl;
            }
        } else {
            cout << "No comments." << endl;
        }
    }
    
    void generateSystemReport() {
        cout << "System Report" << endl;
        cout << "=============" << endl;
        
        // User statistics
        cout << "Users: " << users.size() << endl;
        int activeUsers = 0;
        for (const auto& user : users) {
            if (user.getIsActive()) {
                activeUsers++;
            }
        }
        cout << "Active Users: " << activeUsers << endl;
        
        // Project statistics
        cout << "Projects: " << projects.size() << endl;
        map<string, int> projectStatusCount;
        for (const auto& project : projects) {
            projectStatusCount[project.getStatus()]++;
        }
        cout << "Project Status Distribution:" << endl;
        for (const auto& pair : projectStatusCount) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
        
        // Task statistics
        cout << "Tasks: " << tasks.size() << endl;
        map<TaskStatus, int> taskStatusCount;
        map<TaskPriority, int> taskPriorityCount;
        int overdueCount = 0;
        
        for (const auto& task : tasks) {
            taskStatusCount[task.getStatus()]++;
            taskPriorityCount[task.getPriority()]++;
            
            if (task.isOverdue()) {
                overdueCount++;
            }
        }
        
        cout << "Task Status Distribution:" << endl;
        for (const auto& pair : taskStatusCount) {
            cout << "  " << Task().getStatusStringStatic(pair.first) << ": " << pair.second << endl;
        }
        
        cout << "Task Priority Distribution:" << endl;
        for (const auto& pair : taskPriorityCount) {
            cout << "  " << Task().getPriorityStringStatic(pair.first) << ": " << pair.second << endl;
        }
        
        cout << "Overdue Tasks: " << overdueCount << endl;
        
        // Comment statistics
        cout << "Comments: " << comments.size() << endl;
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save next IDs
        outFile << "NEXT_IDS" << endl;
        outFile << nextUserId << "|" << nextProjectId << "|" << nextTaskId << "|" << nextCommentId << endl;
        
        // Save users
        outFile << "USERS" << endl;
        for (const auto& user : users) {
            outFile << user.getUserId() << "|" 
                    << user.getUsername() << "|" 
                    << user.getEmail() << "|" 
                    << user.getFirstName() << "|" 
                    << user.getLastName() << "|" 
                    << user.getRole() << "|" 
                    << user.getJoinDate().year << "|" 
                    << user.getJoinDate().month << "|" 
                    << user.getJoinDate().day << "|" 
                    << (user.getIsActive() ? "1" : "0") << "|";
            
            // Save assigned task IDs
            vector<int> assignedTaskIds = user.getAssignedTaskIds();
            outFile << assignedTaskIds.size();
            for (int taskId : assignedTaskIds) {
                outFile << "," << taskId;
            }
            outFile << "|";
            
            // Save created task IDs
            vector<int> createdTaskIds = user.getCreatedTaskIds();
            outFile << createdTaskIds.size();
            for (int taskId : createdTaskIds) {
                outFile << "," << taskId;
            }
            outFile << endl;
        }
        
        // Save projects
        outFile << "PROJECTS" << endl;
        for (const auto& project : projects) {
            outFile << project.getProjectId() << "|" 
                    << project.getName() << "|" 
                    << project.getDescription() << "|" 
                    << project.getStartDate().year << "|" 
                    << project.getStartDate().month << "|" 
                    << project.getStartDate().day << "|" 
                    << project.getEndDate().year << "|" 
                    << project.getEndDate().month << "|" 
                    << project.getEndDate().day << "|" 
                    << project.getStatus() << "|" 
                    << project.getCreatorId() << "|";
            
            // Save member IDs
            vector<int> memberIds = project.getMemberIds();
            outFile << memberIds.size();
            for (int memberId : memberIds) {
                outFile << "," << memberId;
            }
            outFile << "|";
            
            // Save task IDs
            vector<int> taskIds = project.getTaskIds();
            outFile << taskIds.size();
            for (int taskId : taskIds) {
                outFile << "," << taskId;
            }
            outFile << endl;
        }
        
        // Save tasks
        outFile << "TASKS" << endl;
        for (const auto& task : tasks) {
            outFile << task.getTaskId() << "|" 
                    << task.getTitle() << "|" 
                    << task.getDescription() << "|" 
                    << static_cast<int>(task.getPriority()) << "|" 
                    << static_cast<int>(task.getStatus()) << "|" 
                    << task.getCreatedDate().year << "|" 
                    << task.getCreatedDate().month << "|" 
                    << task.getCreatedDate().day << "|" 
                    << task.getDueDate().year << "|" 
                    << task.getDueDate().month << "|" 
                    << task.getDueDate().day << "|" 
                    << task.getCompletedDate().year << "|" 
                    << task.getCompletedDate().month << "|" 
                    << task.getCompletedDate().day << "|" 
                    << task.getCreatorId() << "|" 
                    << task.getAssigneeId() << "|" 
                    << task.getProjectId() << "|";
            
            // Save comment IDs
            vector<int> commentIds = task.getCommentIds();
            outFile << commentIds.size();
            for (int commentId : commentIds) {
                outFile << "," << commentId;
            }
            outFile << "|" 
                    << task.getEstimatedHours() << "|" 
                    << task.getActualHours() << endl;
        }
        
        // Save comments
        outFile << "COMMENTS" << endl;
        for (const auto& comment : comments) {
            outFile << comment.getCommentId() << "|" 
                    << comment.getTaskId() << "|" 
                    << comment.getUserId() << "|" 
                    << comment.getContent() << "|" 
                    << comment.getCreatedDate().year << "|" 
                    << comment.getCreatedDate().month << "|" 
                    << comment.getCreatedDate().day << "|" 
                    << (comment.getIsEdited() ? "1" : "0") << "|" 
                    << comment.getEditedDate().year << "|" 
                    << comment.getEditedDate().month << "|" 
                    << comment.getEditedDate().day << endl;
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
            } else if (line == "USERS") {
                section = "USERS";
                continue;
            } else if (line == "PROJECTS") {
                section = "PROJECTS";
                continue;
            } else if (line == "TASKS") {
                section = "TASKS";
                continue;
            } else if (line == "COMMENTS") {
                section = "COMMENTS";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "NEXT_IDS" && tokens.size() >= 4) {
                nextUserId = stoi(tokens[0]);
                nextProjectId = stoi(tokens[1]);
                nextTaskId = stoi(tokens[2]);
                nextCommentId = stoi(tokens[3]);
            } else if (section == "USERS" && tokens.size() >= 12) {
                int userId = stoi(tokens[0]);
                string username = tokens[1];
                string email = tokens[2];
                string firstName = tokens[3];
                string lastName = tokens[4];
                string role = tokens[5];
                
                date joinDate;
                joinDate.year = stoi(tokens[6]);
                joinDate.month = stoi(tokens[7]);
                joinDate.day = stoi(tokens[8]);
                
                bool isActive = (tokens[9] == "1");
                
                User user(userId, username, email, firstName, lastName, role, joinDate, isActive);
                
                // Parse assigned task IDs
                int assignedCount = stoi(tokens[10]);
                if (assignedCount > 0) {
                    stringstream assignedSs(tokens[11].substr(1)); // Skip first character
                    string assignedToken;
                    while (getline(assignedSs, assignedToken, ',')) {
                        user.addAssignedTask(stoi(assignedToken));
                    }
                }
                
                // Parse created task IDs
                if (tokens.size() > 12) {
                    int createdCount = stoi(tokens[12]);
                    if (createdCount > 0) {
                        stringstream createdSs(tokens[13].substr(1)); // Skip first character
                        string createdToken;
                        while (getline(createdSs, createdToken, ',')) {
                            user.addCreatedTask(stoi(createdToken));
                        }
                    }
                }
                
                users.push_back(user);
            } else if (section == "PROJECTS" && tokens.size() >= 16) {
                int projectId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                
                date startDate;
                startDate.year = stoi(tokens[3]);
                startDate.month = stoi(tokens[4]);
                startDate.day = stoi(tokens[5]);
                
                date endDate;
                endDate.year = stoi(tokens[6]);
                endDate.month = stoi(tokens[7]);
                endDate.day = stoi(tokens[8]);
                
                string status = tokens[9];
                int creatorId = stoi(tokens[10]);
                
                Project project(projectId, name, description, startDate, endDate, status, creatorId);
                
                // Parse member IDs
                int memberCount = stoi(tokens[11]);
                if (memberCount > 0) {
                    stringstream memberSs(tokens[12].substr(1)); // Skip first character
                    string memberToken;
                    while (getline(memberSs, memberToken, ',')) {
                        project.addMember(stoi(memberToken));
                    }
                }
                
                // Parse task IDs
                if (tokens.size() > 13) {
                    int taskCount = stoi(tokens[13]);
                    if (taskCount > 0) {
                        stringstream taskSs(tokens[14].substr(1)); // Skip first character
                        string taskToken;
                        while (getline(taskSs, taskToken, ',')) {
                            project.addTask(stoi(taskToken));
                        }
                    }
                }
                
                projects.push_back(project);
            } else if (section == "TASKS" && tokens.size() >= 20) {
                int taskId = stoi(tokens[0]);
                string title = tokens[1];
                string description = tokens[2];
                TaskPriority priority = static_cast<TaskPriority>(stoi(tokens[3]));
                TaskStatus status = static_cast<TaskStatus>(stoi(tokens[4]));
                
                date createdDate;
                createdDate.year = stoi(tokens[5]);
                createdDate.month = stoi(tokens[6]);
                createdDate.day = stoi(tokens[7]);
                
                date dueDate;
                dueDate.year = stoi(tokens[8]);
                dueDate.month = stoi(tokens[9]);
                dueDate.day = stoi(tokens[10]);
                
                date completedDate;
                completedDate.year = stoi(tokens[11]);
                completedDate.month = stoi(tokens[12]);
                completedDate.day = stoi(tokens[13]);
                
                int creatorId = stoi(tokens[14]);
                int assigneeId = stoi(tokens[15]);
                int projectId = stoi(tokens[16]);
                
                Task task(taskId, title, description, priority, status, createdDate, dueDate, 
                         creatorId, assigneeId, projectId);
                task.setCompletedDate(completedDate);
                
                // Parse comment IDs
                int commentCount = stoi(tokens[17]);
                if (commentCount > 0) {
                    stringstream commentSs(tokens[18].substr(1)); // Skip first character
                    string commentToken;
                    while (getline(commentSs, commentToken, ',')) {
                        task.addComment(stoi(commentToken));
                    }
                }
                
                if (tokens.size() > 19) {
                    task.setEstimatedHours(stoi(tokens[19]));
                }
                
                if (tokens.size() > 20) {
                    task.setActualHours(stoi(tokens[20]));
                }
                
                tasks.push_back(task);
            } else if (section == "COMMENTS" && tokens.size() >= 12) {
                int commentId = stoi(tokens[0]);
                int taskId = stoi(tokens[1]);
                int userId = stoi(tokens[2]);
                string content = tokens[3];
                
                date createdDate;
                createdDate.year = stoi(tokens[4]);
                createdDate.month = stoi(tokens[5]);
                createdDate.day = stoi(tokens[6]);
                
                bool isEdited = (tokens[7] == "1");
                
                date editedDate;
                editedDate.year = stoi(tokens[8]);
                editedDate.month = stoi(tokens[9]);
                editedDate.day = stoi(tokens[10]);
                
                Comment comment(commentId, taskId, userId, content, createdDate, isEdited, editedDate);
                
                comments.push_back(comment);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        users.clear();
        projects.clear();
        tasks.clear();
        comments.clear();
        
        nextUserId = 1;
        nextProjectId = 1;
        nextTaskId = 1;
        nextCommentId = 1;
        
        // Create sample users
        vector<User> sampleUsers = {
            User(getNextUserId(), "john_doe", "john@example.com", "John", "Doe", "Project Manager", {2023, 1, 15}, true),
            User(getNextUserId(), "jane_smith", "jane@example.com", "Jane", "Smith", "Developer", {2023, 1, 20}, true),
            User(getNextUserId(), "bob_jones", "bob@example.com", "Bob", "Jones", "Developer", {2023, 2, 1}, true),
            User(getNextUserId(), "alice_wilson", "alice@example.com", "Alice", "Wilson", "Designer", {2023, 2, 10}, true),
            User(getNextUserId(), "charlie_brown", "charlie@example.com", "Charlie", "Brown", "Tester", {2023, 2, 15}, true)
        };
        
        for (const auto& user : sampleUsers) {
            addUser(user);
        }
        
        // Create sample projects
        vector<Project> sampleProjects = {
            Project(getNextProjectId(), "Website Redesign", "Redesign company website", {2023, 3, 1}, {2023, 6, 30}, "Active", 1),
            Project(getNextProjectId(), "Mobile App Development", "Develop new mobile application", {2023, 3, 15}, {2023, 9, 30}, "Active", 1),
            Project(getNextProjectId(), "Database Migration", "Migrate database to new system", {2023, 4, 1}, {2023, 5, 15}, "Active", 1)
        };
        
        for (const auto& project : sampleProjects) {
            addProject(project);
        }
        
        // Add members to projects
        addMemberToProject(1, 2); // Jane to Website Redesign
        addMemberToProject(1, 4); // Alice to Website Redesign
        addMemberToProject(2, 2); // Jane to Mobile App
        addMemberToProject(2, 3); // Bob to Mobile App
        addMemberToProject(2, 4); // Alice to Mobile App
        addMemberToProject(3, 3); // Bob to Database Migration
        addMemberToProject(3, 5); // Charlie to Database Migration
        
        // Create sample tasks
        vector<Task> sampleTasks = {
            Task(getNextTaskId(), "Create Homepage Mockup", "Design mockup for the new homepage", HIGH, TODO, {2023, 3, 5}, {2023, 3, 15}, 1, 4, 1),
            Task(getNextTaskId(), "Implement Homepage", "Implement the homepage design", MEDIUM, TODO, {2023, 3, 10}, {2023, 3, 25}, 1, 2, 1),
            Task(getNextTaskId(), "Design App Navigation", "Design navigation flow for the mobile app", HIGH, TODO, {2023, 3, 15}, {2023, 4, 5}, 1, 4, 2),
            Task(getNextTaskId(), "Implement User Authentication", "Implement login and registration", MEDIUM, TODO, {2023, 3, 20}, {2023, 4, 15}, 1, 2, 2),
            Task(getNextTaskId(), "Database Schema Design", "Design new database schema", HIGH, TODO, {2023, 4, 1}, {2023, 4, 10}, 1, 3, 3),
            Task(getNextTaskId(), "Migrate User Data", "Migrate user accounts to new database", MEDIUM, TODO, {2023, 4, 5}, {2023, 4, 20}, 1, 3, 3),
            Task(getNextTaskId(), "Test Website Performance", "Test website performance after redesign", MEDIUM, TODO, {2023, 3, 20}, {2023, 4, 10}, 1, 5, 1),
            Task(getNextTaskId(), "Test Mobile App Features", "Test all features of the mobile app", LOW, TODO, {2023, 4, 20}, {2023, 5, 5}, 1, 5, 2)
        };
        
        for (const auto& task : sampleTasks) {
            addTask(task);
            findProject(task.getProjectId())->addTask(task.getTaskId());
            findUser(task.getCreatorId())->addCreatedTask(task.getTaskId());
            
            if (task.getAssigneeId() > 0) {
                findUser(task.getAssigneeId())->addAssignedTask(task.getTaskId());
            }
        }
        
        // Create sample comments
        vector<Comment> sampleComments = {
            Comment(getNextCommentId(), 1, 1, "Please use our brand colors and logo", {2023, 3, 6}),
            Comment(getNextCommentId(), 1, 4, "Will do. I'll have the first draft ready by Monday", {2023, 3, 7}),
            Comment(getNextCommentId(), 3, 1, "Keep the navigation simple and intuitive", {2023, 3, 16}),
            Comment(getNextCommentId(), 5, 3, "Should we include user roles in the new schema?", {2023, 4, 2})
        };
        
        for (const auto& comment : sampleComments) {
            addComment(comment);
            findTask(comment.getTaskId())->addComment(comment.getCommentId());
        }
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Users: " << users.size() << endl;
        cout << "Projects: " << projects.size() << endl;
        cout << "Tasks: " << tasks.size() << endl;
        cout << "Comments: " << comments.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(TaskManagementSystem& system, int choice);
void userMenu(TaskManagementSystem& system);
void projectMenu(TaskManagementSystem& system);
void taskMenu(TaskManagementSystem& system);
void commentMenu(TaskManagementSystem& system);
void reportMenu(TaskManagementSystem& system);
void fileMenu(TaskManagementSystem& system);

int main() {
    TaskManagementSystem system;
    
    cout << "Task Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("task_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("task_data.txt");
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
    system.saveToFile("task_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Task Management System" << endl;
    cout << "1. User Management" << endl;
    cout << "2. Project Management" << endl;
    cout << "3. Task Management" << endl;
    cout << "4. Comment Management" << endl;
    cout << "5. Reports" << endl;
    cout << "6. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(TaskManagementSystem& system, int choice) {
    switch (choice) {
        case 1:
            userMenu(system);
            break;
        case 2:
            projectMenu(system);
            break;
        case 3:
            taskMenu(system);
            break;
        case 4:
            commentMenu(system);
            break;
        case 5:
            reportMenu(system);
            break;
        case 6:
            fileMenu(system);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void userMenu(TaskManagementSystem& system) {
    int choice;
    do {
        cout << "User Management" << endl;
        cout << "1. Add User" << endl;
        cout << "2. Remove User" << endl;
        cout << "3. List All Users" << endl;
        cout << "4. Search Users by Name" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string username, email, firstName, lastName, role;
                
                cout << "Enter Username: ";
                getline(cin, username);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Role: ";
                getline(cin, role);
                
                // Get current date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date joinDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                User user(system.getNextUserId(), username, email, firstName, lastName, role, joinDate);
                
                if (system.addUser(user)) {
                    cout << "User added successfully." << endl;
                } else {
                    cout << "Failed to add user. Maximum number of users reached." << endl;
                }
                break;
            }
            case 2: {
                int userId;
                cout << "Enter User ID to remove: ";
                cin >> userId;
                
                if (system.removeUser(userId)) {
                    cout << "User removed successfully." << endl;
                } else {
                    cout << "Failed to remove user. User ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<User*> users = system.getAllUsers();
                if (users.empty()) {
                    cout << "No users found." << endl;
                } else {
                    cout << "Users (" << users.size() << "):" << endl;
                    for (User* user : users) {
                        cout << "ID: " << user->getUserId()
                             << ", Username: " << user->getUsername()
                             << ", Name: " << user->getFullName()
                             << ", Role: " << user->getRole()
                             << ", Status: " << (user->getIsActive() ? "Active" : "Inactive") << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                
                vector<User*> users = system.searchUsersByName(name);
                if (users.empty()) {
                    cout << "No users found matching name." << endl;
                } else {
                    cout << "Users matching '" << name << "' (" << users.size() << "):" << endl;
                    for (User* user : users) {
                        cout << "ID: " << user->getUserId()
                             << ", Username: " << user->getUsername()
                             << ", Name: " << user->getFullName()
                             << ", Role: " << user->getRole() << endl;
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

void projectMenu(TaskManagementSystem& system) {
    int choice;
    do {
        cout << "Project Management" << endl;
        cout << "1. Add Project" << endl;
        cout << "2. Remove Project" << endl;
        cout << "3. List All Projects" << endl;
        cout << "4. Add Member to Project" << endl;
        cout << "5. Remove Member from Project" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description, status;
                int creatorId;
                date startDate, endDate;
                
                cout << "Enter Project Name: ";
                getline(cin, name);
                
                cout << "Enter Project Description: ";
                getline(cin, description);
                
                cout << "Enter Start Date (YYYY MM DD): ";
                cin >> startDate.year >> startDate.month >> startDate.day;
                
                cout << "Enter End Date (YYYY MM DD): ";
                cin >> endDate.year >> endDate.month >> endDate.day;
                
                cout << "Enter Status: ";
                getline(cin, status);
                
                cout << "Enter Creator ID: ";
                cin >> creatorId;
                
                Project project(system.getNextProjectId(), name, description, startDate, endDate, status, creatorId);
                
                if (system.addProject(project)) {
                    cout << "Project added successfully." << endl;
                } else {
                    cout << "Failed to add project. Maximum number of projects reached." << endl;
                }
                break;
            }
            case 2: {
                int projectId;
                cout << "Enter Project ID to remove: ";
                cin >> projectId;
                
                if (system.removeProject(projectId)) {
                    cout << "Project removed successfully." << endl;
                } else {
                    cout << "Failed to remove project. Project ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Project*> projects = system.getAllProjects();
                if (projects.empty()) {
                    cout << "No projects found." << endl;
                } else {
                    cout << "Projects (" << projects.size() << "):" << endl;
                    for (Project* project : projects) {
                        cout << "ID: " << project->getProjectId()
                             << ", Name: " << project->getName()
                             << ", Status: " << project->getStatus()
                             << ", Members: " << project->getMemberIds().size() << endl;
                    }
                }
                break;
            }
            case 4: {
                int projectId, userId;
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                cout << "Enter User ID to add: ";
                cin >> userId;
                
                if (system.addMemberToProject(projectId, userId)) {
                    cout << "User added to project successfully." << endl;
                } else {
                    cout << "Failed to add user to project. Project or user may not exist." << endl;
                }
                break;
            }
            case 5: {
                int projectId, userId;
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                cout << "Enter User ID to remove: ";
                cin >> userId;
                
                if (system.removeMemberFromProject(projectId, userId)) {
                    cout << "User removed from project successfully." << endl;
                } else {
                    cout << "Failed to remove user from project. Project may not exist." << endl;
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

void taskMenu(TaskManagementSystem& system) {
    int choice;
    do {
        cout << "Task Management" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Remove Task" << endl;
        cout << "3. List All Tasks" << endl;
        cout << "4. Assign Task" << endl;
        cout << "5. Complete Task" << endl;
        cout << "6. Search Tasks by Title" << endl;
        cout << "7. List Tasks by Project" << endl;
        cout << "8. List Tasks by Assignee" << endl;
        cout << "9. List Tasks by Status" << endl;
        cout << "10. List Overdue Tasks" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string title, description;
                int priority, creatorId, assigneeId, projectId;
                date createdDate, dueDate;
                
                cout << "Enter Task Title: ";
                getline(cin, title);
                
                cout << "Enter Task Description: ";
                getline(cin, description);
                
                cout << "Enter Priority (1-Low, 2-Medium, 3-High, 4-Urgent): ";
                cin >> priority;
                
                cout << "Enter Creator ID: ";
                cin >> creatorId;
                
                cout << "Enter Assignee ID (0 for unassigned): ";
                cin >> assigneeId;
                
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                cout << "Enter Due Date (YYYY MM DD, 0 0 0 for no due date): ";
                cin >> dueDate.year >> dueDate.month >> dueDate.day;
                
                // Get current date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                createdDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                Task task(system.getNextTaskId(), title, description, 
                         static_cast<TaskPriority>(priority), TODO, 
                         createdDate, dueDate, creatorId, assigneeId, projectId);
                
                if (system.addTask(task)) {
                    cout << "Task added successfully." << endl;
                    
                    // Update project and user references
                    Project* project = system.findProject(projectId);
                    if (project) {
                        project->addTask(task.getTaskId());
                    }
                    
                    User* creator = system.findUser(creatorId);
                    if (creator) {
                        creator->addCreatedTask(task.getTaskId());
                    }
                    
                    if (assigneeId > 0) {
                        User* assignee = system.findUser(assigneeId);
                        if (assignee) {
                            assignee->addAssignedTask(task.getTaskId());
                        }
                    }
                } else {
                    cout << "Failed to add task. Maximum number of tasks reached." << endl;
                }
                break;
            }
            case 2: {
                int taskId;
                cout << "Enter Task ID to remove: ";
                cin >> taskId;
                
                if (system.removeTask(taskId)) {
                    cout << "Task removed successfully." << endl;
                } else {
                    cout << "Failed to remove task. Task ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Task*> tasks = system.getAllTasks();
                if (tasks.empty()) {
                    cout << "No tasks found." << endl;
                } else {
                    cout << "Tasks (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Priority: " << task->getPriorityString()
                             << ", Status: " << task->getStatusString();
                        
                        if (task->isOverdue()) {
                            cout << " (OVERDUE)";
                        }
                        cout << endl;
                    }
                }
                break;
            }
            case 4: {
                int taskId, userId;
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                cout << "Enter User ID to assign: ";
                cin >> userId;
                
                if (system.assignTask(taskId, userId)) {
                    cout << "Task assigned successfully." << endl;
                } else {
                    cout << "Failed to assign task. Task or user may not exist." << endl;
                }
                break;
            }
            case 5: {
                int taskId, actualHours;
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                cout << "Enter Actual Hours (0 if unknown): ";
                cin >> actualHours;
                
                if (system.completeTask(taskId, actualHours)) {
                    cout << "Task completed successfully." << endl;
                } else {
                    cout << "Failed to complete task. Task ID may not exist." << endl;
                }
                break;
            }
            case 6: {
                string title;
                cout << "Enter title to search: ";
                getline(cin, title);
                
                vector<Task*> tasks = system.searchTasksByTitle(title);
                if (tasks.empty()) {
                    cout << "No tasks found matching title." << endl;
                } else {
                    cout << "Tasks matching '" << title << "' (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Status: " << task->getStatusString() << endl;
                    }
                }
                break;
            }
            case 7: {
                int projectId;
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                vector<Task*> tasks = system.getTasksByProject(projectId);
                if (tasks.empty()) {
                    cout << "No tasks found for this project." << endl;
                } else {
                    cout << "Tasks for Project " << projectId << " (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Status: " << task->getStatusString();
                        
                        if (task->isOverdue()) {
                            cout << " (OVERDUE)";
                        }
                        cout << endl;
                    }
                }
                break;
            }
            case 8: {
                int userId;
                cout << "Enter User ID: ";
                cin >> userId;
                
                vector<Task*> tasks = system.getTasksByAssignee(userId);
                if (tasks.empty()) {
                    cout << "No tasks assigned to this user." << endl;
                } else {
                    cout << "Tasks assigned to User " << userId << " (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Status: " << task->getStatusString();
                        
                        if (task->isOverdue()) {
                            cout << " (OVERDUE)";
                        }
                        cout << endl;
                    }
                }
                break;
            }
            case 9: {
                int statusChoice;
                cout << "Select Status:" << endl;
                cout << "1. To Do" << endl;
                cout << "2. In Progress" << endl;
                cout << "3. Review" << endl;
                cout << "4. Done" << endl;
                cout << "Enter choice: ";
                cin >> statusChoice;
                
                TaskStatus status = static_cast<TaskStatus>(statusChoice);
                
                vector<Task*> tasks = system.getTasksByStatus(status);
                if (tasks.empty()) {
                    cout << "No tasks found with this status." << endl;
                } else {
                    cout << "Tasks with status '" << Task().getStatusStringStatic(status) << "' (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Priority: " << task->getPriorityString();
                        
                        if (task->isOverdue()) {
                            cout << " (OVERDUE)";
                        }
                        cout << endl;
                    }
                }
                break;
            }
            case 10: {
                vector<Task*> tasks = system.getOverdueTasks();
                if (tasks.empty()) {
                    cout << "No overdue tasks found." << endl;
                } else {
                    cout << "Overdue Tasks (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Assignee: " << task->getAssigneeId()
                             << ", Due Date: " << task->getDueDate().year 
                             << "-" << task->getDueDate().month 
                             << "-" << task->getDueDate().day << endl;
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

void commentMenu(TaskManagementSystem& system) {
    int choice;
    do {
        cout << "Comment Management" << endl;
        cout << "1. Add Comment to Task" << endl;
        cout << "2. Remove Comment" << endl;
        cout << "3. List Comments for Task" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int taskId, userId;
                string content;
                
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                cout << "Enter User ID: ";
                cin >> userId;
                
                cout << "Enter Comment Content: ";
                getline(cin, content);
                
                if (system.addCommentToTask(taskId, userId, content)) {
                    cout << "Comment added successfully." << endl;
                } else {
                    cout << "Failed to add comment. Task or user may not exist." << endl;
                }
                break;
            }
            case 2: {
                int commentId;
                cout << "Enter Comment ID to remove: ";
                cin >> commentId;
                
                if (system.removeComment(commentId)) {
                    cout << "Comment removed successfully." << endl;
                } else {
                    cout << "Failed to remove comment. Comment ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                int taskId;
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                vector<Comment*> comments = system.getCommentsByTask(taskId);
                if (comments.empty()) {
                    cout << "No comments found for this task." << endl;
                } else {
                    cout << "Comments for Task " << taskId << " (" << comments.size() << "):" << endl;
                    for (Comment* comment : comments) {
                        cout << "ID: " << comment->getCommentId()
                             << ", User ID: " << comment->getUserId()
                             << ", Content: " << comment->getContent()
                             << ", Date: " << comment->getCreatedDate().year 
                             << "-" << comment->getCreatedDate().month 
                             << "-" << comment->getCreatedDate().day;
                        
                        if (comment->getIsEdited()) {
                            cout << " (Edited)";
                        }
                        cout << endl;
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

void reportMenu(TaskManagementSystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Generate User Report" << endl;
        cout << "2. Generate Project Report" << endl;
        cout << "3. Generate Task Report" << endl;
        cout << "4. Generate System Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int userId;
                cout << "Enter User ID: ";
                cin >> userId;
                
                system.generateUserReport(userId);
                break;
            }
            case 2: {
                int projectId;
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                system.generateProjectReport(projectId);
                break;
            }
            case 3: {
                int taskId;
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                system.generateTaskReport(taskId);
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

void fileMenu(TaskManagementSystem& system) {
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
                    filename = "task_data.txt";
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
                    filename = "task_data.txt";
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
<｜fim▁hole｜>