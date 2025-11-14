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

#define MAX_EMPLOYEES 500
#define MAX_DEPARTMENTS 20
#define MAX_PROJECTS 50
#define MAX_TASKS 1000
#define MAX_TIME_ENTRIES 5000

struct date {
    int year;
    int month;
    int day;
};

class Employee;
class Department;
class Project;
class Task;
class TimeEntry;

enum EmployeeStatus {
    ACTIVE = 1,
    ON_LEAVE = 2,
    TERMINATED = 3
};

class Employee {
private:
    int employeeId;
    string firstName;
    string lastName;
    string email;
    string phone;
    string position;
    date hireDate;
    double salary;
    EmployeeStatus status;
    int departmentId;
    vector<int> projectIds;
    vector<int> taskIds;
    
public:
    Employee(int employeeId = 0, const string& firstName = "", const string& lastName = "",
             const string& email = "", const string& phone = "", const string& position = "",
             date hireDate = {0, 0, 0}, double salary = 0.0, 
             EmployeeStatus status = ACTIVE, int departmentId = 0)
        : employeeId(employeeId), firstName(firstName), lastName(lastName), email(email),
          phone(phone), position(position), hireDate(hireDate), salary(salary),
          status(status), departmentId(departmentId) {}
    
    int getEmployeeId() const { return employeeId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getPosition() const { return position; }
    date getHireDate() const { return hireDate; }
    double getSalary() const { return salary; }
    EmployeeStatus getStatus() const { return status; }
    int getDepartmentId() const { return departmentId; }
    vector<int> getProjectIds() const { return projectIds; }
    vector<int> getTaskIds() const { return taskIds; }
    
    void setEmployeeId(int id) { employeeId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setPosition(const string& position) { this->position = position; }
    void setHireDate(date date) { hireDate = date; }
    void setSalary(double salary) { this->salary = salary; }
    void setStatus(EmployeeStatus status) { this->status = status; }
    void setDepartmentId(int departmentId) { this->departmentId = departmentId; }
    
    void addProject(int projectId) {
        if (find(projectIds.begin(), projectIds.end(), projectId) == projectIds.end()) {
            projectIds.push_back(projectId);
        }
    }
    
    void removeProject(int projectId) {
        auto it = find(projectIds.begin(), projectIds.end(), projectId);
        if (it != projectIds.end()) {
            projectIds.erase(it);
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
    
    string getStatusString() const {
        switch (status) {
            case ACTIVE: return "Active";
            case ON_LEAVE: return "On Leave";
            case TERMINATED: return "Terminated";
            default: return "Unknown";
        }
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
        cout << "Employee ID: " << employeeId << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Position: " << position << endl;
        
        // Format hire date
        tm tm = {};
        tm.tm_year = hireDate.year - 1900;
        tm.tm_mon = hireDate.month - 1;
        tm.tm_mday = hireDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Hire Date: " << dateStr << endl;
        
        cout << "Salary: $" << fixed << setprecision(2) << salary << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Department ID: " << departmentId << endl;
        cout << "Years of Service: " << getYearsOfService() << endl;
        cout << "Projects: " << projectIds.size() << endl;
        cout << "Tasks: " << taskIds.size() << endl;
    }
};

class Department {
private:
    int departmentId;
    string name;
    string description;
    int managerId;
    vector<int> employeeIds;
    
public:
    Department(int departmentId = 0, const string& name = "", const string& description = "", 
               int managerId = 0)
        : departmentId(departmentId), name(name), description(description), managerId(managerId) {}
    
    int getDepartmentId() const { return departmentId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    int getManagerId() const { return managerId; }
    vector<int> getEmployeeIds() const { return employeeIds; }
    
    void setDepartmentId(int id) { departmentId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setManagerId(int managerId) { this->managerId = managerId; }
    
    void addEmployee(int employeeId) {
        if (find(employeeIds.begin(), employeeIds.end(), employeeId) == employeeIds.end()) {
            employeeIds.push_back(employeeId);
        }
    }
    
    void removeEmployee(int employeeId) {
        auto it = find(employeeIds.begin(), employeeIds.end(), employeeId);
        if (it != employeeIds.end()) {
            employeeIds.erase(it);
        }
    }
    
    void display() const {
        cout << "Department ID: " << departmentId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Manager ID: " << managerId << endl;
        cout << "Employees: " << employeeIds.size() << endl;
    }
};

enum ProjectStatus {
    PLANNING = 1,
    IN_PROGRESS = 2,
    ON_HOLD = 3,
    COMPLETED = 4,
    CANCELLED = 5
};

class Project {
private:
    int projectId;
    string name;
    string description;
    date startDate;
    date endDate;
    ProjectStatus status;
    int budget;
    vector<int> employeeIds;
    vector<int> taskIds;
    
public:
    Project(int projectId = 0, const string& name = "", const string& description = "",
            date startDate = {0, 0, 0}, date endDate = {0, 0, 0},
            ProjectStatus status = PLANNING, int budget = 0)
        : projectId(projectId), name(name), description(description), 
          startDate(startDate), endDate(endDate), status(status), budget(budget) {}
    
    int getProjectId() const { return projectId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    date getStartDate() const { return startDate; }
    date getEndDate() const { return endDate; }
    ProjectStatus getStatus() const { return status; }
    int getBudget() const { return budget; }
    vector<int> getEmployeeIds() const { return employeeIds; }
    vector<int> getTaskIds() const { return taskIds; }
    
    void setProjectId(int id) { projectId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setStartDate(date date) { startDate = date; }
    void setEndDate(date date) { endDate = date; }
    void setStatus(ProjectStatus status) { this->status = status; }
    void setBudget(int budget) { this->budget = budget; }
    
    void addEmployee(int employeeId) {
        if (find(employeeIds.begin(), employeeIds.end(), employeeId) == employeeIds.end()) {
            employeeIds.push_back(employeeId);
        }
    }
    
    void removeEmployee(int employeeId) {
        auto it = find(employeeIds.begin(), employeeIds.end(), employeeId);
        if (it != employeeIds.end()) {
            employeeIds.erase(it);
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
    
    string getStatusString() const {
        switch (status) {
            case PLANNING: return "Planning";
            case IN_PROGRESS: return "In Progress";
            case ON_HOLD: return "On Hold";
            case COMPLETED: return "Completed";
            case CANCELLED: return "Cancelled";
            default: return "Unknown";
        }
    }
    
    int getDurationInDays() const {
        // Simple calculation - not accounting for leap years or month lengths
        int days = (endDate.year - startDate.year) * 365;
        days += (endDate.month - startDate.month) * 30;
        days += endDate.day - startDate.day;
        
        return max(0, days);
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
        
        cout << "Status: " << getStatusString() << endl;
        cout << "Budget: $" << fixed << setprecision(2) << budget << endl;
        cout << "Duration: " << getDurationInDays() << " days" << endl;
        cout << "Team Members: " << employeeIds.size() << endl;
        cout << "Tasks: " << taskIds.size() << endl;
    }
};

enum TaskStatus {
    TODO = 1,
    IN_PROGRESS = 2,
    IN_REVIEW = 3,
    COMPLETED = 4
};

enum TaskPriority {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4
};

class Task {
private:
    int taskId;
    string title;
    string description;
    TaskStatus status;
    TaskPriority priority;
    date createdDate;
    date dueDate;
    date completedDate;
    int projectId;
    int assigneeId;
    int estimatedHours;
    int actualHours;
    
public:
    Task(int taskId = 0, const string& title = "", const string& description = "",
         TaskStatus status = TODO, TaskPriority priority = MEDIUM,
         date createdDate = {0, 0, 0}, date dueDate = {0, 0, 0},
         int projectId = 0, int assigneeId = 0)
        : taskId(taskId), title(title), description(description), status(status), 
          priority(priority), createdDate(createdDate), dueDate(dueDate),
          projectId(projectId), assigneeId(assigneeId) {
        completedDate = {0, 0, 0};
        estimatedHours = 0;
        actualHours = 0;
    }
    
    int getTaskId() const { return taskId; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    TaskStatus getStatus() const { return status; }
    TaskPriority getPriority() const { return priority; }
    date getCreatedDate() const { return createdDate; }
    date getDueDate() const { return dueDate; }
    date getCompletedDate() const { return completedDate; }
    int getProjectId() const { return projectId; }
    int getAssigneeId() const { return assigneeId; }
    int getEstimatedHours() const { return estimatedHours; }
    int getActualHours() const { return actualHours; }
    
    void setTaskId(int id) { taskId = id; }
    void setTitle(const string& title) { this->title = title; }
    void setDescription(const string& description) { this->description = description; }
    void setStatus(TaskStatus status) { this->status = status; }
    void setPriority(TaskPriority priority) { this->priority = priority; }
    void setCreatedDate(date date) { createdDate = date; }
    void setDueDate(date date) { dueDate = date; }
    void setCompletedDate(date date) { completedDate = date; }
    void setProjectId(int projectId) { this->projectId = projectId; }
    void setAssigneeId(int assigneeId) { this->assigneeId = assigneeId; }
    void setEstimatedHours(int hours) { estimatedHours = hours; }
    void setActualHours(int hours) { actualHours = hours; }
    
    string getStatusString() const {
        switch (status) {
            case TODO: return "To Do";
            case IN_PROGRESS: return "In Progress";
            case IN_REVIEW: return "In Review";
            case COMPLETED: return "Completed";
            default: return "Unknown";
        }
    }
    
    string getPriorityString() const {
        switch (priority) {
            case LOW: return "Low";
            case MEDIUM: return "Medium";
            case HIGH: return "High";
            case CRITICAL: return "Critical";
            default: return "Unknown";
        }
    }
    
    bool isOverdue() const {
        if (status == COMPLETED) return false;
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
    
    double getCompletionPercentage() const {
        if (estimatedHours == 0) return 0.0;
        return min(100.0, (double)actualHours / estimatedHours * 100.0);
    }
    
    void display() const {
        cout << "Task ID: " << taskId << endl;
        cout << "Title: " << title << endl;
        cout << "Description: " << description << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Priority: " << getPriorityString() << endl;
        
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
        
        cout << "Project ID: " << projectId << endl;
        cout << "Assignee ID: " << assigneeId << endl;
        
        if (estimatedHours > 0) {
            cout << "Estimated Hours: " << estimatedHours << endl;
            if (actualHours > 0) {
                cout << "Actual Hours: " << actualHours << endl;
                cout << "Completion: " << fixed << setprecision(1) << getCompletionPercentage() << "%" << endl;
            }
        }
    }
};

class TimeEntry {
private:
    int timeEntryId;
    int employeeId;
    int taskId;
    date date;
    int hoursWorked;
    string description;
    
public:
    TimeEntry(int timeEntryId = 0, int employeeId = 0, int taskId = 0,
              date date = {0, 0, 0}, int hoursWorked = 0, const string& description = "")
        : timeEntryId(timeEntryId), employeeId(employeeId), taskId(taskId),
          date(date), hoursWorked(hoursWorked), description(description) {}
    
    int getTimeEntryId() const { return timeEntryId; }
    int getEmployeeId() const { return employeeId; }
    int getTaskId() const { return taskId; }
    date getDate() const { return date; }
    int getHoursWorked() const { return hoursWorked; }
    string getDescription() const { return description; }
    
    void setTimeEntryId(int id) { timeEntryId = id; }
    void setEmployeeId(int employeeId) { this->employeeId = employeeId; }
    void setTaskId(int taskId) { this->taskId = taskId; }
    void setDate(date date) { this->date = date; }
    void setHoursWorked(int hours) { hoursWorked = hours; }
    void setDescription(const string& description) { this->description = description; }
    
    void display() const {
        cout << "Time Entry ID: " << timeEntryId << endl;
        cout << "Employee ID: " << employeeId << endl;
        cout << "Task ID: " << taskId << endl;
        
        // Format date
        tm tm = {};
        tm.tm_year = date.year - 1900;
        tm.tm_mon = date.month - 1;
        tm.tm_mday = date.day;
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Date: " << dateStr << endl;
        
        cout << "Hours Worked: " << hoursWorked << endl;
        cout << "Description: " << description << endl;
    }
};

class EmployeeManagementSystem {
private:
    vector<Employee> employees;
    vector<Department> departments;
    vector<Project> projects;
    vector<Task> tasks;
    vector<TimeEntry> timeEntries;
    
    int nextEmployeeId;
    int nextDepartmentId;
    int nextProjectId;
    int nextTaskId;
    int nextTimeEntryId;
    
public:
    EmployeeManagementSystem() {
        nextEmployeeId = 1;
        nextDepartmentId = 1;
        nextProjectId = 1;
        nextTaskId = 1;
        nextTimeEntryId = 1;
    }
    
    // Employee management
    Employee* findEmployee(int employeeId) {
        for (auto& employee : employees) {
            if (employee.getEmployeeId() == employeeId) {
                return &employee;
            }
        }
        return nullptr;
    }
    
    bool addEmployee(const Employee& employee) {
        if (employees.size() >= MAX_EMPLOYEES) {
            return false;
        }
        
        employees.push_back(employee);
        return true;
    }
    
    bool removeEmployee(int employeeId) {
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getEmployeeId() == employeeId) {
                employees.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Employee*> getAllEmployees() {
        vector<Employee*> result;
        for (auto& employee : employees) {
            result.push_back(&employee);
        }
        return result;
    }
    
    vector<Employee*> searchEmployeesByName(const string& name) {
        vector<Employee*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& employee : employees) {
            string fullName = employee.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&employee);
            }
        }
        return result;
    }
    
    vector<Employee*> getEmployeesByDepartment(int departmentId) {
        vector<Employee*> result;
        for (auto& employee : employees) {
            if (employee.getDepartmentId() == departmentId) {
                result.push_back(&employee);
            }
        }
        return result;
    }
    
    vector<Employee*> getEmployeesByStatus(EmployeeStatus status) {
        vector<Employee*> result;
        for (auto& employee : employees) {
            if (employee.getStatus() == status) {
                result.push_back(&employee);
            }
        }
        return result;
    }
    
    int getNextEmployeeId() { return nextEmployeeId++; }
    
    // Department management
    Department* findDepartment(int departmentId) {
        for (auto& department : departments) {
            if (department.getDepartmentId() == departmentId) {
                return &department;
            }
        }
        return nullptr;
    }
    
    bool addDepartment(const Department& department) {
        if (departments.size() >= MAX_DEPARTMENTS) {
            return false;
        }
        
        departments.push_back(department);
        return true;
    }
    
    bool removeDepartment(int departmentId) {
        for (auto it = departments.begin(); it != departments.end(); ++it) {
            if (it->getDepartmentId() == departmentId) {
                departments.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Department*> getAllDepartments() {
        vector<Department*> result;
        for (auto& department : departments) {
            result.push_back(&department);
        }
        return result;
    }
    
    int getNextDepartmentId() { return nextDepartmentId++; }
    
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
    
    vector<Project*> getProjectsByStatus(ProjectStatus status) {
        vector<Project*> result;
        for (auto& project : projects) {
            if (project.getStatus() == status) {
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
    
    vector<Task*> getTasksByAssignee(int employeeId) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.getAssigneeId() == employeeId) {
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
    
    vector<Task*> getOverdueTasks() {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isOverdue()) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    int getNextTaskId() { return nextTaskId++; }
    
    // Time entry management
    TimeEntry* findTimeEntry(int timeEntryId) {
        for (auto& timeEntry : timeEntries) {
            if (timeEntry.getTimeEntryId() == timeEntryId) {
                return &timeEntry;
            }
        }
        return nullptr;
    }
    
    bool addTimeEntry(const TimeEntry& timeEntry) {
        if (timeEntries.size() >= MAX_TIME_ENTRIES) {
            return false;
        }
        
        timeEntries.push_back(timeEntry);
        return true;
    }
    
    bool removeTimeEntry(int timeEntryId) {
        for (auto it = timeEntries.begin(); it != timeEntries.end(); ++it) {
            if (it->getTimeEntryId() == timeEntryId) {
                timeEntries.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<TimeEntry*> getAllTimeEntries() {
        vector<TimeEntry*> result;
        for (auto& timeEntry : timeEntries) {
            result.push_back(&timeEntry);
        }
        return result;
    }
    
    vector<TimeEntry*> getTimeEntriesByEmployee(int employeeId) {
        vector<TimeEntry*> result;
        for (auto& timeEntry : timeEntries) {
            if (timeEntry.getEmployeeId() == employeeId) {
                result.push_back(&timeEntry);
            }
        }
        return result;
    }
    
    vector<TimeEntry*> getTimeEntriesByTask(int taskId) {
        vector<TimeEntry*> result;
        for (auto& timeEntry : timeEntries) {
            if (timeEntry.getTaskId() == taskId) {
                result.push_back(&timeEntry);
            }
        }
        return result;
    }
    
    int getNextTimeEntryId() { return nextTimeEntryId++; }
    
    // Business operations
    bool assignEmployeeToProject(int employeeId, int projectId) {
        Employee* employee = findEmployee(employeeId);
        Project* project = findProject(projectId);
        
        if (!employee || !project) {
            return false;
        }
        
        employee->addProject(projectId);
        project->addEmployee(employeeId);
        
        return true;
    }
    
    bool assignEmployeeToTask(int employeeId, int taskId) {
        Employee* employee = findEmployee(employeeId);
        Task* task = findTask(taskId);
        
        if (!employee || !task) {
            return false;
        }
        
        employee->addTask(taskId);
        task->setAssigneeId(employeeId);
        
        return true;
    }
    
    bool completeTask(int taskId, int actualHours) {
        Task* task = findTask(taskId);
        if (!task) {
            return false;
        }
        
        task->setStatus(COMPLETED);
        task->setActualHours(actualHours);
        
        // Set completion date to today
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date completedDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        task->setCompletedDate(completedDate);
        
        return true;
    }
    
    bool logTime(int employeeId, int taskId, const date& date, int hours, const string& description) {
        Employee* employee = findEmployee(employeeId);
        Task* task = findTask(taskId);
        
        if (!employee || !task) {
            return false;
        }
        
        TimeEntry timeEntry(getNextTimeEntryId(), employeeId, taskId, date, hours, description);
        
        if (!addTimeEntry(timeEntry)) {
            return false;
        }
        
        // Update task actual hours
        task->setActualHours(task->getActualHours() + hours);
        
        return true;
    }
    
    // Reporting
    void generateEmployeeReport(int employeeId) {
        Employee* employee = findEmployee(employeeId);
        if (!employee) {
            cout << "Employee not found." << endl;
            return;
        }
        
        cout << "Employee Report" << endl;
        cout << "===============" << endl;
        employee->display();
        cout << endl;
        
        // Display department information
        if (employee->getDepartmentId() > 0) {
            Department* department = findDepartment(employee->getDepartmentId());
            if (department) {
                cout << "Department:" << endl;
                cout << "  Name: " << department->getName() << endl;
                cout << "  Description: " << department->getDescription() << endl;
            }
        }
        
        // Display projects
        vector<int> projectIds = employee->getProjectIds();
        if (!projectIds.empty()) {
            cout << endl << "Projects (" << projectIds.size() << "):" << endl;
            for (int projectId : projectIds) {
                Project* project = findProject(projectId);
                if (project) {
                    cout << "  ID: " << project->getProjectId()
                         << ", Name: " << project->getName()
                         << ", Status: " << project->getStatusString() << endl;
                }
            }
        }
        
        // Display tasks
        vector<int> taskIds = employee->getTaskIds();
        if (!taskIds.empty()) {
            cout << endl << "Tasks (" << taskIds.size() << "):" << endl;
            
            int completedCount = 0;
            int overdueCount = 0;
            
            for (int taskId : taskIds) {
                Task* task = findTask(taskId);
                if (task) {
                    cout << "  ID: " << task->getTaskId()
                         << ", Title: " << task->getTitle()
                         << ", Status: " << task->getStatusString();
                    
                    if (task->isOverdue()) {
                        cout << " (OVERDUE)";
                        overdueCount++;
                    }
                    
                    if (task->getStatus() == COMPLETED) {
                        completedCount++;
                    }
                    
                    cout << endl;
                }
            }
            
            cout << endl << "Task Summary:" << endl;
            cout << "  Completed: " << completedCount << endl;
            cout << "  Overdue: " << overdueCount << endl;
            cout << "  In Progress: " << (taskIds.size() - completedCount - overdueCount) << endl;
        }
        
        // Display time entries
        vector<TimeEntry*> timeEntryList = getTimeEntriesByEmployee(employeeId);
        if (!timeEntryList.empty()) {
            cout << endl << "Time Entries (" << timeEntryList.size() << "):" << endl;
            
            int totalHours = 0;
            for (TimeEntry* timeEntry : timeEntryList) {
                cout << "  ID: " << timeEntry->getTimeEntryId()
                     << ", Date: " << timeEntry->getDate().year 
                     << "-" << timeEntry->getDate().month 
                     << "-" << timeEntry->getDate().day
                     << ", Hours: " << timeEntry->getHoursWorked()
                     << ", Task ID: " << timeEntry->getTaskId() << endl;
                
                totalHours += timeEntry->getHoursWorked();
            }
            
            cout << endl << "Total Hours Logged: " << totalHours << endl;
        }
    }
    
    void generateProjectReport(int projectId) {
        Project* project = findProject(projectId);
        if (!project) {
            cout << "Project not found." << endl;
            return;
        }
        
        cout << "Project Report" << endl;
        cout << "==============" << endl;
        project->display();
        cout << endl;
        
        // Display team members
        vector<int> employeeIds = project->getEmployeeIds();
        if (!employeeIds.empty()) {
            cout << "Team Members (" << employeeIds.size() << "):" << endl;
            for (int employeeId : employeeIds) {
                Employee* employee = findEmployee(employeeId);
                if (employee) {
                    cout << "  ID: " << employee->getEmployeeId()
                         << ", Name: " << employee->getFullName()
                         << ", Position: " << employee->getPosition() << endl;
                }
            }
        }
        
        // Display tasks
        vector<Task*> projectTasks = getTasksByProject(projectId);
        if (!projectTasks.empty()) {
            cout << endl << "Tasks (" << projectTasks.size() << "):" << endl;
            
            map<TaskStatus, int> statusCounts;
            int overdueCount = 0;
            int totalEstimatedHours = 0;
            int totalActualHours = 0;
            
            for (Task* task : projectTasks) {
                statusCounts[task->getStatus()]++;
                
                if (task->isOverdue()) {
                    overdueCount++;
                }
                
                totalEstimatedHours += task->getEstimatedHours();
                totalActualHours += task->getActualHours();
            }
            
            // Display status summary
            cout << endl << "Task Status Summary:" << endl;
            for (const auto& pair : statusCounts) {
                Task dummyTask;
                cout << "  " << dummyTask.getStatusStringStatic(pair.first) 
                     << ": " << pair.second << endl;
            }
            
            cout << "  Overdue: " << overdueCount << endl;
            
            // Calculate completion rate
            if (projectTasks.size() > 0) {
                double completionRate = (double)statusCounts[COMPLETED] / projectTasks.size() * 100.0;
                cout << "  Completion Rate: " << fixed << setprecision(1) << completionRate << "%" << endl;
            }
            
            cout << endl << "Time Summary:" << endl;
            cout << "  Total Estimated Hours: " << totalEstimatedHours << endl;
            cout << "  Total Actual Hours: " << totalActualHours << endl;
            
            if (totalEstimatedHours > 0) {
                double efficiency = (double)totalActualHours / totalEstimatedHours * 100.0;
                cout << "  Time Efficiency: " << fixed << setprecision(1) << efficiency << "%" << endl;
            }
        }
    }
    
    void generateDepartmentReport(int departmentId) {
        Department* department = findDepartment(departmentId);
        if (!department) {
            cout << "Department not found." << endl;
            return;
        }
        
        cout << "Department Report" << endl;
        cout << "=================" << endl;
        department->display();
        cout << endl;
        
        // Display manager information
        Employee* manager = findEmployee(department->getManagerId());
        if (manager) {
            cout << "Manager:" << endl;
            cout << "  Name: " << manager->getFullName() << endl;
            cout << "  Email: " << manager->getEmail() << endl;
            cout << "  Phone: " << manager->getPhone() << endl;
        }
        
        // Display employees
        vector<Employee*> departmentEmployees = getEmployeesByDepartment(departmentId);
        if (!departmentEmployees.empty()) {
            cout << endl << "Employees (" << departmentEmployees.size() << "):" << endl;
            
            double totalSalary = 0.0;
            int activeCount = 0;
            int onLeaveCount = 0;
            int terminatedCount = 0;
            
            for (Employee* employee : departmentEmployees) {
                cout << "  ID: " << employee->getEmployeeId()
                     << ", Name: " << employee->getFullName()
                     << ", Position: " << employee->getPosition()
                     << ", Status: " << employee->getStatusString() << endl;
                
                totalSalary += employee->getSalary();
                
                switch (employee->getStatus()) {
                    case ACTIVE: activeCount++; break;
                    case ON_LEAVE: onLeaveCount++; break;
                    case TERMINATED: terminatedCount++; break;
                }
            }
            
            cout << endl << "Department Statistics:" << endl;
            cout << "  Total Salary: $" << fixed << setprecision(2) << totalSalary << endl;
            cout << "  Average Salary: $" << fixed << setprecision(2) 
                 << (departmentEmployees.size() > 0 ? totalSalary / departmentEmployees.size() : 0.0) << endl;
            cout << "  Active: " << activeCount << endl;
            cout << "  On Leave: " << onLeaveCount << endl;
            cout << "  Terminated: " << terminatedCount << endl;
        }
    }
    
    void generateSystemReport() {
        cout << "System Report" << endl;
        cout << "=============" << endl;
        
        // Employee statistics
        cout << "Employees: " << employees.size() << endl;
        map<EmployeeStatus, int> statusCounts;
        double totalSalary = 0.0;
        
        for (const auto& employee : employees) {
            statusCounts[employee.getStatus()]++;
            totalSalary += employee.getSalary();
        }
        
        cout << "Employee Status Distribution:" << endl;
        for (const auto& pair : statusCounts) {
            Employee dummyEmployee;
            cout << "  " << dummyEmployee.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        cout << "Total Payroll: $" << fixed << setprecision(2) << totalSalary << endl;
        
        // Department statistics
        cout << endl << "Departments: " << departments.size() << endl;
        
        // Project statistics
        cout << "Projects: " << projects.size() << endl;
        map<ProjectStatus, int> projectStatusCounts;
        
        for (const auto& project : projects) {
            projectStatusCounts[project.getStatus()]++;
        }
        
        cout << "Project Status Distribution:" << endl;
        for (const auto& pair : projectStatusCounts) {
            Project dummyProject;
            cout << "  " << dummyProject.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        // Task statistics
        cout << endl << "Tasks: " << tasks.size() << endl;
        map<TaskStatus, int> taskStatusCounts;
        map<TaskPriority, int> taskPriorityCounts;
        int overdueCount = 0;
        
        for (const auto& task : tasks) {
            taskStatusCounts[task.getStatus()]++;
            taskPriorityCounts[task.getPriority()]++;
            
            if (task.isOverdue()) {
                overdueCount++;
            }
        }
        
        cout << "Task Status Distribution:" << endl;
        for (const auto& pair : taskStatusCounts) {
            Task dummyTask;
            cout << "  " << dummyTask.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        cout << "Task Priority Distribution:" << endl;
        for (const auto& pair : taskPriorityCounts) {
            Task dummyTask;
            cout << "  " << dummyTask.getPriorityStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        cout << "Overdue Tasks: " << overdueCount << endl;
        
        // Time entry statistics
        cout << endl << "Time Entries: " << timeEntries.size() << endl;
        
        int totalHoursLogged = 0;
        for (const auto& timeEntry : timeEntries) {
            totalHoursLogged += timeEntry.getHoursWorked();
        }
        
        cout << "Total Hours Logged: " << totalHoursLogged << endl;
        
        if (timeEntries.size() > 0) {
            cout << "Average Hours per Entry: " << fixed << setprecision(1) 
                 << (double)totalHoursLogged / timeEntries.size() << endl;
        }
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save next IDs
        outFile << "NEXT_IDS" << endl;
        outFile << nextEmployeeId << "|" << nextDepartmentId << "|" << nextProjectId << "|" 
                << nextTaskId << "|" << nextTimeEntryId << endl;
        
        // Save employees
        outFile << "EMPLOYEES" << endl;
        for (const auto& employee : employees) {
            outFile << employee.getEmployeeId() << "|" 
                    << employee.getFirstName() << "|" 
                    << employee.getLastName() << "|" 
                    << employee.getEmail() << "|" 
                    << employee.getPhone() << "|" 
                    << employee.getPosition() << "|" 
                    << employee.getHireDate().year << "|" 
                    << employee.getHireDate().month << "|" 
                    << employee.getHireDate().day << "|" 
                    << fixed << setprecision(2) << employee.getSalary() << "|" 
                    << static_cast<int>(employee.getStatus()) << "|" 
                    << employee.getDepartmentId() << "|";
            
            // Save project IDs
            vector<int> projectIds = employee.getProjectIds();
            outFile << projectIds.size();
            for (int projectId : projectIds) {
                outFile << "," << projectId;
            }
            outFile << "|";
            
            // Save task IDs
            vector<int> taskIds = employee.getTaskIds();
            outFile << taskIds.size();
            for (int taskId : taskIds) {
                outFile << "," << taskId;
            }
            outFile << endl;
        }
        
        // Save departments
        outFile << "DEPARTMENTS" << endl;
        for (const auto& department : departments) {
            outFile << department.getDepartmentId() << "|" 
                    << department.getName() << "|" 
                    << department.getDescription() << "|" 
                    << department.getManagerId() << "|";
            
            // Save employee IDs
            vector<int> employeeIds = department.getEmployeeIds();
            outFile << employeeIds.size();
            for (int employeeId : employeeIds) {
                outFile << "," << employeeId;
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
                    << static_cast<int>(project.getStatus()) << "|" 
                    << project.getBudget() << "|";
            
            // Save employee IDs
            vector<int> employeeIds = project.getEmployeeIds();
            outFile << employeeIds.size();
            for (int employeeId : employeeIds) {
                outFile << "," << employeeId;
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
                    << static_cast<int>(task.getStatus()) << "|" 
                    << static_cast<int>(task.getPriority()) << "|" 
                    << task.getCreatedDate().year << "|" 
                    << task.getCreatedDate().month << "|" 
                    << task.getCreatedDate().day << "|" 
                    << task.getDueDate().year << "|" 
                    << task.getDueDate().month << "|" 
                    << task.getDueDate().day << "|" 
                    << task.getCompletedDate().year << "|" 
                    << task.getCompletedDate().month << "|" 
                    << task.getCompletedDate().day << "|" 
                    << task.getProjectId() << "|" 
                    << task.getAssigneeId() << "|" 
                    << task.getEstimatedHours() << "|" 
                    << task.getActualHours() << endl;
        }
        
        // Save time entries
        outFile << "TIME_ENTRIES" << endl;
        for (const auto& timeEntry : timeEntries) {
            outFile << timeEntry.getTimeEntryId() << "|" 
                    << timeEntry.getEmployeeId() << "|" 
                    << timeEntry.getTaskId() << "|" 
                    << timeEntry.getDate().year << "|" 
                    << timeEntry.getDate().month << "|" 
                    << timeEntry.getDate().day << "|" 
                    << timeEntry.getHoursWorked() << "|" 
                    << timeEntry.getDescription() << endl;
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
            } else if (line == "EMPLOYEES") {
                section = "EMPLOYEES";
                continue;
            } else if (line == "DEPARTMENTS") {
                section = "DEPARTMENTS";
                continue;
            } else if (line == "PROJECTS") {
                section = "PROJECTS";
                continue;
            } else if (line == "TASKS") {
                section = "TASKS";
                continue;
            } else if (line == "TIME_ENTRIES") {
                section = "TIME_ENTRIES";
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
                nextEmployeeId = stoi(tokens[0]);
                nextDepartmentId = stoi(tokens[1]);
                nextProjectId = stoi(tokens[2]);
                nextTaskId = stoi(tokens[3]);
                nextTimeEntryId = stoi(tokens[4]);
            } else if (section == "EMPLOYEES" && tokens.size() >= 14) {
                int employeeId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string email = tokens[3];
                string phone = tokens[4];
                string position = tokens[5];
                
                date hireDate;
                hireDate.year = stoi(tokens[6]);
                hireDate.month = stoi(tokens[7]);
                hireDate.day = stoi(tokens[8]);
                
                double salary = stod(tokens[9]);
                EmployeeStatus status = static_cast<EmployeeStatus>(stoi(tokens[10]));
                int departmentId = stoi(tokens[11]);
                
                Employee employee(employeeId, firstName, lastName, email, phone, position, 
                                hireDate, salary, status, departmentId);
                
                // Parse project IDs
                int projectCount = stoi(tokens[12]);
                if (projectCount > 0) {
                    stringstream projectSs(tokens[13].substr(1)); // Skip first character
                    string projectToken;
                    while (getline(projectSs, projectToken, ',')) {
                        employee.addProject(stoi(projectToken));
                    }
                }
                
                // Parse task IDs
                if (tokens.size() > 14) {
                    int taskCount = stoi(tokens[14]);
                    if (taskCount > 0) {
                        stringstream taskSs(tokens[15].substr(1)); // Skip first character
                        string taskToken;
                        while (getline(taskSs, taskToken, ',')) {
                            employee.addTask(stoi(taskToken));
                        }
                    }
                }
                
                employees.push_back(employee);
            } else if (section == "DEPARTMENTS" && tokens.size() >= 5) {
                int departmentId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                int managerId = stoi(tokens[3]);
                
                Department department(departmentId, name, description, managerId);
                
                // Parse employee IDs
                int employeeCount = stoi(tokens[4]);
                if (employeeCount > 0) {
                    stringstream employeeSs(tokens[5].substr(1)); // Skip first character
                    string employeeToken;
                    while (getline(employeeSs, employeeToken, ',')) {
                        department.addEmployee(stoi(employeeToken));
                    }
                }
                
                departments.push_back(department);
            } else if (section == "PROJECTS" && tokens.size() >= 15) {
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
                
                ProjectStatus status = static_cast<ProjectStatus>(stoi(tokens[9]));
                int budget = stoi(tokens[10]);
                
                Project project(projectId, name, description, startDate, endDate, status, budget);
                
                // Parse employee IDs
                int employeeCount = stoi(tokens[11]);
                if (employeeCount > 0) {
                    stringstream employeeSs(tokens[12].substr(1)); // Skip first character
                    string employeeToken;
                    while (getline(employeeSs, employeeToken, ',')) {
                        project.addEmployee(stoi(employeeToken));
                    }
                }
                
                // Parse task IDs
                int taskCount = stoi(tokens[13]);
                if (taskCount > 0) {
                    stringstream taskSs(tokens[14].substr(1)); // Skip first character
                    string taskToken;
                    while (getline(taskSs, taskToken, ',')) {
                        project.addTask(stoi(taskToken));
                    }
                }
                
                projects.push_back(project);
            } else if (section == "TASKS" && tokens.size() >= 18) {
                int taskId = stoi(tokens[0]);
                string title = tokens[1];
                string description = tokens[2];
                TaskStatus status = static_cast<TaskStatus>(stoi(tokens[3]));
                TaskPriority priority = static_cast<TaskPriority>(stoi(tokens[4]));
                
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
                
                int projectId = stoi(tokens[14]);
                int assigneeId = stoi(tokens[15]);
                int estimatedHours = stoi(tokens[16]);
                int actualHours = stoi(tokens[17]);
                
                Task task(taskId, title, description, status, priority, 
                         createdDate, dueDate, projectId, assigneeId);
                task.setCompletedDate(completedDate);
                task.setEstimatedHours(estimatedHours);
                task.setActualHours(actualHours);
                
                tasks.push_back(task);
            } else if (section == "TIME_ENTRIES" && tokens.size() >= 9) {
                int timeEntryId = stoi(tokens[0]);
                int employeeId = stoi(tokens[1]);
                int taskId = stoi(tokens[2]);
                
                date date;
                date.year = stoi(tokens[3]);
                date.month = stoi(tokens[4]);
                date.day = stoi(tokens[5]);
                
                int hoursWorked = stoi(tokens[6]);
                string description = tokens[7];
                
                TimeEntry timeEntry(timeEntryId, employeeId, taskId, date, hoursWorked, description);
                
                timeEntries.push_back(timeEntry);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        employees.clear();
        departments.clear();
        projects.clear();
        tasks.clear();
        timeEntries.clear();
        
        nextEmployeeId = 1;
        nextDepartmentId = 1;
        nextProjectId = 1;
        nextTaskId = 1;
        nextTimeEntryId = 1;
        
        // Create sample departments
        vector<Department> sampleDepartments = {
            Department(getNextDepartmentId(), "Engineering", "Software and hardware development", 1),
            Department(getNextDepartmentId(), "Marketing", "Marketing and sales activities", 2),
            Department(getNextDepartmentId(), "Human Resources", "HR and administrative functions", 3),
            Department(getNextDepartmentId(), "Finance", "Financial planning and accounting", 4)
        };
        
        for (const auto& department : sampleDepartments) {
            addDepartment(department);
        }
        
        // Create sample employees
        vector<Employee> sampleEmployees = {
            Employee(getNextEmployeeId(), "John", "Smith", "john.smith@example.com", "555-1234", "Software Engineer", {2020, 1, 15}, 85000.0, ACTIVE, 1),
            Employee(getNextEmployeeId(), "Jane", "Johnson", "jane.johnson@example.com", "555-5678", "Engineering Manager", {2018, 3, 10}, 120000.0, ACTIVE, 1),
            Employee(getNextEmployeeId(), "Michael", "Brown", "michael.brown@example.com", "555-9012", "Marketing Specialist", {2019, 6, 20}, 65000.0, ACTIVE, 2),
            Employee(getNextEmployeeId(), "Sarah", "Davis", "sarah.davis@example.com", "555-3456", "HR Manager", {2017, 2, 5}, 95000.0, ACTIVE, 3),
            Employee(getNextEmployeeId(), "David", "Wilson", "david.wilson@example.com", "555-7890", "Financial Analyst", {2021, 4, 12}, 70000.0, ACTIVE, 4),
            Employee(getNextEmployeeId(), "Emily", "Taylor", "emily.taylor@example.com", "555-2345", "Junior Developer", {2022, 5, 8}, 60000.0, ACTIVE, 1),
            Employee(getNextEmployeeId(), "Robert", "Anderson", "robert.anderson@example.com", "555-6789", "Sales Representative", {2021, 7, 15}, 55000.0, ACTIVE, 2),
            Employee(getNextEmployeeId(), "Lisa", "Miller", "lisa.miller@example.com", "555-1357", "Accountant", {2019, 9, 22}, 75000.0, ACTIVE, 4)
        };
        
        for (const auto& employee : sampleEmployees) {
            addEmployee(employee);
            // Add employee to department
            findDepartment(employee.getDepartmentId())->addEmployee(employee.getEmployeeId());
        }
        
        // Create sample projects
        vector<Project> sampleProjects = {
            Project(getNextProjectId(), "Website Redesign", "Redesign company website with modern UI", {2023, 1, 1}, {2023, 6, 30}, IN_PROGRESS, 50000),
            Project(getNextProjectId(), "Mobile App Development", "Develop new mobile application for customers", {2023, 2, 15}, {2023, 8, 31}, IN_PROGRESS, 80000),
            Project(getNextProjectId(), "Q1 Marketing Campaign", "Spring marketing campaign for product launch", {2023, 3, 1}, {2023, 5, 31}, PLANNING, 30000),
            Project(getNextProjectId(), "Financial System Upgrade", "Upgrade accounting and financial systems", {2023, 4, 1}, {2023, 9, 30}, PLANNING, 60000)
        };
        
        for (const auto& project : sampleProjects) {
            addProject(project);
        }
        
        // Assign employees to projects
        assignEmployeeToProject(1, 1); // John to Website Redesign
        assignEmployeeToProject(6, 1); // Emily to Website Redesign
        assignEmployeeToProject(1, 2); // John to Mobile App
        assignEmployeeToProject(6, 2); // Emily to Mobile App
        assignEmployeeToProject(3, 3); // Michael to Q1 Marketing Campaign
        assignEmployeeToProject(7, 3); // Robert to Q1 Marketing Campaign
        assignEmployeeToProject(4, 4); // Sarah to Financial System Upgrade
        assignEmployeeToProject(8, 4); // Lisa to Financial System Upgrade
        
        // Create sample tasks
        vector<Task> sampleTasks = {
            Task(getNextTaskId(), "Design Homepage Mockup", "Create initial design mockup for new homepage", TODO, HIGH, {2023, 1, 5}, {2023, 1, 15}, 1, 1),
            Task(getNextTaskId(), "Implement Homepage", "Develop frontend for new homepage", TODO, MEDIUM, {2023, 1, 10}, {2023, 2, 5}, 1, 6),
            Task(getNextTaskId(), "Design Mobile App UI", "Create user interface designs for mobile app", TODO, HIGH, {2023, 2, 20}, {2023, 3, 10}, 2, 1),
            Task(getNextTaskId(), "Develop App Backend", "Implement backend services for mobile app", TODO, HIGH, {2023, 2, 25}, {2023, 4, 15}, 2, 6),
            Task(getNextTaskId(), "Create Marketing Materials", "Design brochures and digital ads", TODO, MEDIUM, {2023, 3, 5}, {2023, 3, 20}, 3, 3),
            Task(getNextTaskId(), "Plan Social Media Campaign", "Plan social media content calendar", TODO, LOW, {2023, 3, 10}, {2023, 3, 25}, 3, 7),
            Task(getNextTaskId(), "Analyze Current Systems", "Analyze existing financial systems", TODO, MEDIUM, {2023, 4, 1}, {2023, 4, 20}, 4, 8),
            Task(getNextTaskId(), "Select New Accounting Software", "Research and select new accounting software", TODO, HIGH, {2023, 4, 5}, {2023, 5, 15}, 4, 8)
        };
        
        for (const auto& task : sampleTasks) {
            addTask(task);
            // Add task to project
            findProject(task.getProjectId())->addTask(task.getTaskId());
            
            // Assign task to employee
            assignEmployeeToTask(task.getAssigneeId(), task.getTaskId());
        }
        
        // Create sample time entries
        vector<TimeEntry> sampleTimeEntries = {
            TimeEntry(getNextTimeEntryId(), 1, 1, {2023, 1, 6}, 8, "Worked on homepage design"),
            TimeEntry(getNextTimeEntryId(), 6, 2, {2023, 1, 11}, 6, "Implemented homepage components"),
            TimeEntry(getNextTimeEntryId(), 1, 2, {2023, 1, 12}, 4, "Planning mobile app architecture"),
            TimeEntry(getNextTimeEntryId(), 6, 2, {2023, 1, 13}, 7, "Developing app backend services"),
            TimeEntry(getNextTimeEntryId(), 3, 5, {2023, 3, 6}, 5, "Designing marketing materials"),
            TimeEntry(getNextTimeEntryId(), 7, 6, {2023, 3, 11}, 3, "Social media campaign planning"),
            TimeEntry(getNextTimeEntryId(), 8, 7, {2023, 4, 2}, 8, "Analyzing current financial systems"),
            TimeEntry(getNextTimeEntryId(), 8, 8, {2023, 4, 6}, 6, "Researching accounting software options")
        };
        
        for (const auto& timeEntry : sampleTimeEntries) {
            addTimeEntry(timeEntry);
        }
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Departments: " << departments.size() << endl;
        cout << "Employees: " << employees.size() << endl;
        cout << "Projects: " << projects.size() << endl;
        cout << "Tasks: " << tasks.size() << endl;
        cout << "Time Entries: " << timeEntries.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(EmployeeManagementSystem& system, int choice);
void employeeMenu(EmployeeManagementSystem& system);
void departmentMenu(EmployeeManagementSystem& system);
void projectMenu(EmployeeManagementSystem& system);
void taskMenu(EmployeeManagementSystem& system);
void timeEntryMenu(EmployeeManagementSystem& system);
void reportMenu(EmployeeManagementSystem& system);
void fileMenu(EmployeeManagementSystem& system);

int main() {
    EmployeeManagementSystem system;
    
    cout << "Employee Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("employee_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("employee_data.txt");
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
    system.saveToFile("employee_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Employee Management System" << endl;
    cout << "1. Employee Management" << endl;
    cout << "2. Department Management" << endl;
    cout << "3. Project Management" << endl;
    cout << "4. Task Management" << endl;
    cout << "5. Time Entry Management" << endl;
    cout << "6. Reports" << endl;
    cout << "7. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(EmployeeManagementSystem& system, int choice) {
    switch (choice) {
        case 1:
            employeeMenu(system);
            break;
        case 2:
            departmentMenu(system);
            break;
        case 3:
            projectMenu(system);
            break;
        case 4:
            taskMenu(system);
            break;
        case 5:
            timeEntryMenu(system);
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

void employeeMenu(EmployeeManagementSystem& system) {
    int choice;
    do {
        cout << "Employee Management" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Remove Employee" << endl;
        cout << "3. List All Employees" << endl;
        cout << "4. Search Employees by Name" << endl;
        cout << "5. List Employees by Department" << endl;
        cout << "6. List Employees by Status" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, email, phone, position;
                int departmentId;
                double salary;
                int statusChoice;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Position: ";
                getline(cin, position);
                
                cout << "Enter Department ID: ";
                cin >> departmentId;
                
                cout << "Enter Salary: ";
                cin >> salary;
                
                cout << "Enter Status (1-Active, 2-On Leave, 3-Terminated): ";
                cin >> statusChoice;
                
                // Get current date for hire date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date hireDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                EmployeeStatus status = static_cast<EmployeeStatus>(statusChoice);
                
                Employee employee(system.getNextEmployeeId(), firstName, lastName, email, 
                                phone, position, hireDate, salary, status, departmentId);
                
                if (system.addEmployee(employee)) {
                    // Add employee to department
                    Department* department = system.findDepartment(departmentId);
                    if (department) {
                        department->addEmployee(employee.getEmployeeId());
                    }
                    
                    cout << "Employee added successfully." << endl;
                } else {
                    cout << "Failed to add employee. Maximum number of employees reached." << endl;
                }
                break;
            }
            case 2: {
                int employeeId;
                cout << "Enter Employee ID to remove: ";
                cin >> employeeId;
                
                if (system.removeEmployee(employeeId)) {
                    cout << "Employee removed successfully." << endl;
                } else {
                    cout << "Failed to remove employee. Employee ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Employee*> employees = system.getAllEmployees();
                if (employees.empty()) {
                    cout << "No employees found." << endl;
                } else {
                    cout << "Employees (" << employees.size() << "):" << endl;
                    for (Employee* employee : employees) {
                        cout << "ID: " << employee->getEmployeeId()
                             << ", Name: " << employee->getFullName()
                             << ", Position: " << employee->getPosition()
                             << ", Status: " << employee->getStatusString() << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                
                vector<Employee*> employees = system.searchEmployeesByName(name);
                if (employees.empty()) {
                    cout << "No employees found matching name." << endl;
                } else {
                    cout << "Employees matching '" << name << "' (" << employees.size() << "):" << endl;
                    for (Employee* employee : employees) {
                        cout << "ID: " << employee->getEmployeeId()
                             << ", Name: " << employee->getFullName()
                             << ", Department: " << employee->getDepartmentId() << endl;
                    }
                }
                break;
            }
            case 5: {
                int departmentId;
                cout << "Enter Department ID: ";
                cin >> departmentId;
                
                vector<Employee*> employees = system.getEmployeesByDepartment(departmentId);
                if (employees.empty()) {
                    cout << "No employees found in this department." << endl;
                } else {
                    cout << "Employees in Department " << departmentId << " (" << employees.size() << "):" << endl;
                    for (Employee* employee : employees) {
                        cout << "ID: " << employee->getEmployeeId()
                             << ", Name: " << employee->getFullName()
                             << ", Position: " << employee->getPosition() << endl;
                    }
                }
                break;
            }
            case 6: {
                int statusChoice;
                cout << "Enter Status (1-Active, 2-On Leave, 3-Terminated): ";
                cin >> statusChoice;
                
                EmployeeStatus status = static_cast<EmployeeStatus>(statusChoice);
                vector<Employee*> employees = system.getEmployeesByStatus(status);
                
                string statusStr;
                switch (status) {
                    case ACTIVE: statusStr = "Active"; break;
                    case ON_LEAVE: statusStr = "On Leave"; break;
                    case TERMINATED: statusStr = "Terminated"; break;
                }
                
                if (employees.empty()) {
                    cout << "No " << statusStr << " employees found." << endl;
                } else {
                    cout << statusStr << " Employees (" << employees.size() << "):" << endl;
                    for (Employee* employee : employees) {
                        cout << "ID: " << employee->getEmployeeId()
                             << ", Name: " << employee->getFullName()
                             << ", Department: " << employee->getDepartmentId() << endl;
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

void departmentMenu(EmployeeManagementSystem& system) {
    int choice;
    do {
        cout << "Department Management" << endl;
        cout << "1. Add Department" << endl;
        cout << "2. Remove Department" << endl;
        cout << "3. List All Departments" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description;
                int managerId;
                
                cout << "Enter Department Name: ";
                getline(cin, name);
                
                cout << "Enter Department Description: ";
                getline(cin, description);
                
                cout << "Enter Manager ID: ";
                cin >> managerId;
                
                Department department(system.getNextDepartmentId(), name, description, managerId);
                
                if (system.addDepartment(department)) {
                    cout << "Department added successfully." << endl;
                } else {
                    cout << "Failed to add department. Maximum number of departments reached." << endl;
                }
                break;
            }
            case 2: {
                int departmentId;
                cout << "Enter Department ID to remove: ";
                cin >> departmentId;
                
                if (system.removeDepartment(departmentId)) {
                    cout << "Department removed successfully." << endl;
                } else {
                    cout << "Failed to remove department. Department ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Department*> departments = system.getAllDepartments();
                if (departments.empty()) {
                    cout << "No departments found." << endl;
                } else {
                    cout << "Departments (" << departments.size() << "):" << endl;
                    for (Department* department : departments) {
                        cout << "ID: " << department->getDepartmentId()
                             << ", Name: " << department->getName()
                             << ", Manager: " << department->getManagerId() << endl;
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

void projectMenu(EmployeeManagementSystem& system) {
    int choice;
    do {
        cout << "Project Management" << endl;
        cout << "1. Add Project" << endl;
        cout << "2. Remove Project" << endl;
        cout << "3. List All Projects" << endl;
        cout << "4. List Projects by Status" << endl;
        cout << "5. Assign Employee to Project" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description;
                int statusChoice, budget;
                date startDate, endDate;
                
                cout << "Enter Project Name: ";
                getline(cin, name);
                
                cout << "Enter Project Description: ";
                getline(cin, description);
                
                cout << "Enter Start Date (YYYY MM DD): ";
                cin >> startDate.year >> startDate.month >> startDate.day;
                
                cout << "Enter End Date (YYYY MM DD): ";
                cin >> endDate.year >> endDate.month >> endDate.day;
                
                cout << "Enter Status (1-Planning, 2-In Progress, 3-On Hold, 4-Completed, 5-Cancelled): ";
                cin >> statusChoice;
                
                cout << "Enter Budget: ";
                cin >> budget;
                
                ProjectStatus status = static_cast<ProjectStatus>(statusChoice);
                
                Project project(system.getNextProjectId(), name, description, startDate, endDate, status, budget);
                
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
                             << ", Status: " << project->getStatusString() << endl;
                    }
                }
                break;
            }
            case 4: {
                int statusChoice;
                cout << "Enter Status (1-Planning, 2-In Progress, 3-On Hold, 4-Completed, 5-Cancelled): ";
                cin >> statusChoice;
                
                ProjectStatus status = static_cast<ProjectStatus>(statusChoice);
                vector<Project*> projects = system.getProjectsByStatus(status);
                
                string statusStr;
                switch (status) {
                    case PLANNING: statusStr = "Planning"; break;
                    case IN_PROGRESS: statusStr = "In Progress"; break;
                    case ON_HOLD: statusStr = "On Hold"; break;
                    case COMPLETED: statusStr = "Completed"; break;
                    case CANCELLED: statusStr = "Cancelled"; break;
                }
                
                if (projects.empty()) {
                    cout << "No " << statusStr << " projects found." << endl;
                } else {
                    cout << statusStr << " Projects (" << projects.size() << "):" << endl;
                    for (Project* project : projects) {
                        cout << "ID: " << project->getProjectId()
                             << ", Name: " << project->getName()
                             << ", Budget: $" << fixed << setprecision(2) << project->getBudget() << endl;
                    }
                }
                break;
            }
            case 5: {
                int employeeId, projectId;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                if (system.assignEmployeeToProject(employeeId, projectId)) {
                    cout << "Employee assigned to project successfully." << endl;
                } else {
                    cout << "Failed to assign employee to project. Employee or project may not exist." << endl;
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

void taskMenu(EmployeeManagementSystem& system) {
    int choice;
    do {
        cout << "Task Management" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Remove Task" << endl;
        cout << "3. List All Tasks" << endl;
        cout << "4. List Tasks by Project" << endl;
        cout << "5. List Tasks by Assignee" << endl;
        cout << "6. List Tasks by Status" << endl;
        cout << "7. List Overdue Tasks" << endl;
        cout << "8. Assign Employee to Task" << endl;
        cout << "9. Complete Task" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string title, description;
                int statusChoice, priorityChoice, projectId, assigneeId, estimatedHours;
                date createdDate, dueDate;
                
                cout << "Enter Task Title: ";
                getline(cin, title);
                
                cout << "Enter Task Description: ";
                getline(cin, description);
                
                cout << "Enter Status (1-To Do, 2-In Progress, 3-In Review, 4-Completed): ";
                cin >> statusChoice;
                
                cout << "Enter Priority (1-Low, 2-Medium, 3-High, 4-Critical): ";
                cin >> priorityChoice;
                
                cout << "Enter Project ID: ";
                cin >> projectId;
                
                cout << "Enter Assignee ID (0 for unassigned): ";
                cin >> assigneeId;
                
                cout << "Enter Estimated Hours: ";
                cin >> estimatedHours;
                
                cout << "Enter Due Date (YYYY MM DD, 0 0 0 for no due date): ";
                cin >> dueDate.year >> dueDate.month >> dueDate.day;
                
                // Get current date for created date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                createdDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                TaskStatus status = static_cast<TaskStatus>(statusChoice);
                TaskPriority priority = static_cast<TaskPriority>(priorityChoice);
                
                Task task(system.getNextTaskId(), title, description, status, priority, 
                         createdDate, dueDate, projectId, assigneeId);
                task.setEstimatedHours(estimatedHours);
                
                if (system.addTask(task)) {
                    // Add task to project
                    Project* project = system.findProject(projectId);
                    if (project) {
                        project->addTask(task.getTaskId());
                    }
                    
                    cout << "Task added successfully." << endl;
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
            case 5: {
                int employeeId;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                
                vector<Task*> tasks = system.getTasksByAssignee(employeeId);
                if (tasks.empty()) {
                    cout << "No tasks assigned to this employee." << endl;
                } else {
                    cout << "Tasks assigned to Employee " << employeeId << " (" << tasks.size() << "):" << endl;
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
            case 6: {
                int statusChoice;
                cout << "Enter Status (1-To Do, 2-In Progress, 3-In Review, 4-Completed): ";
                cin >> statusChoice;
                
                TaskStatus status = static_cast<TaskStatus>(statusChoice);
                vector<Task*> tasks = system.getTasksByStatus(status);
                
                string statusStr;
                switch (status) {
                    case TODO: statusStr = "To Do"; break;
                    case IN_PROGRESS: statusStr = "In Progress"; break;
                    case IN_REVIEW: statusStr = "In Review"; break;
                    case COMPLETED: statusStr = "Completed"; break;
                }
                
                if (tasks.empty()) {
                    cout << "No " << statusStr << " tasks found." << endl;
                } else {
                    cout << statusStr << " Tasks (" << tasks.size() << "):" << endl;
                    for (Task* task : tasks) {
                        cout << "ID: " << task->getTaskId()
                             << ", Title: " << task->getTitle()
                             << ", Priority: " << task->getPriorityString() << endl;
                    }
                }
                break;
            }
            case 7: {
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
            case 8: {
                int employeeId, taskId;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                if (system.assignEmployeeToTask(employeeId, taskId)) {
                    cout << "Employee assigned to task successfully." << endl;
                } else {
                    cout << "Failed to assign employee to task. Employee or task may not exist." << endl;
                }
                break;
            }
            case 9: {
                int taskId, actualHours;
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                cout << "Enter Actual Hours: ";
                cin >> actualHours;
                
                if (system.completeTask(taskId, actualHours)) {
                    cout << "Task completed successfully." << endl;
                } else {
                    cout << "Failed to complete task. Task ID may not exist." << endl;
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

void timeEntryMenu(EmployeeManagementSystem& system) {
    int choice;
    do {
        cout << "Time Entry Management" << endl;
        cout << "1. Add Time Entry" << endl;
        cout << "2. Remove Time Entry" << endl;
        cout << "3. List All Time Entries" << endl;
        cout << "4. List Time Entries by Employee" << endl;
        cout << "5. List Time Entries by Task" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int employeeId, taskId, hoursWorked;
                date date;
                string description;
                
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                cout << "Enter Date (YYYY MM DD): ";
                cin >> date.year >> date.month >> date.day;
                
                cout << "Enter Hours Worked: ";
                cin >> hoursWorked;
                
                cout << "Enter Description: ";
                getline(cin, description);
                
                if (system.logTime(employeeId, taskId, date, hoursWorked, description)) {
                    cout << "Time entry added successfully." << endl;
                } else {
                    cout << "Failed to add time entry. Employee or task may not exist." << endl;
                }
                break;
            }
            case 2: {
                int timeEntryId;
                cout << "Enter Time Entry ID to remove: ";
                cin >> timeEntryId;
                
                if (system.removeTimeEntry(timeEntryId)) {
                    cout << "Time entry removed successfully." << endl;
                } else {
                    cout << "Failed to remove time entry. Time Entry ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<TimeEntry*> timeEntries = system.getAllTimeEntries();
                if (timeEntries.empty()) {
                    cout << "No time entries found." << endl;
                } else {
                    cout << "Time Entries (" << timeEntries.size() << "):" << endl;
                    for (TimeEntry* timeEntry : timeEntries) {
                        cout << "ID: " << timeEntry->getTimeEntryId()
                             << ", Employee: " << timeEntry->getEmployeeId()
                             << ", Task: " << timeEntry->getTaskId()
                             << ", Date: " << timeEntry->getDate().year 
                             << "-" << timeEntry->getDate().month 
                             << "-" << timeEntry->getDate().day
                             << ", Hours: " << timeEntry->getHoursWorked() << endl;
                    }
                }
                break;
            }
            case 4: {
                int employeeId;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                
                vector<TimeEntry*> timeEntries = system.getTimeEntriesByEmployee(employeeId);
                if (timeEntries.empty()) {
                    cout << "No time entries found for this employee." << endl;
                } else {
                    cout << "Time Entries for Employee " << employeeId << " (" << timeEntries.size() << "):" << endl;
                    for (TimeEntry* timeEntry : timeEntries) {
                        cout << "ID: " << timeEntry->getTimeEntryId()
                             << ", Task: " << timeEntry->getTaskId()
                             << ", Date: " << timeEntry->getDate().year 
                             << "-" << timeEntry->getDate().month 
                             << "-" << timeEntry->getDate().day
                             << ", Hours: " << timeEntry->getHoursWorked() << endl;
                    }
                }
                break;
            }
            case 5: {
                int taskId;
                cout << "Enter Task ID: ";
                cin >> taskId;
                
                vector<TimeEntry*> timeEntries = system.getTimeEntriesByTask(taskId);
                if (timeEntries.empty()) {
                    cout << "No time entries found for this task." << endl;
                } else {
                    cout << "Time Entries for Task " << taskId << " (" << timeEntries.size() << "):" << endl;
                    for (TimeEntry* timeEntry : timeEntries) {
                        cout << "ID: " << timeEntry->getTimeEntryId()
                             << ", Employee: " << timeEntry->getEmployeeId()
                             << ", Date: " << timeEntry->getDate().year 
                             << "-" << timeEntry->getDate().month 
                             << "-" << timeEntry->getDate().day
                             << ", Hours: " << timeEntry->getHoursWorked() << endl;
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

void reportMenu(EmployeeManagementSystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Generate Employee Report" << endl;
        cout << "2. Generate Project Report" << endl;
        cout << "3. Generate Department Report" << endl;
        cout << "4. Generate System Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int employeeId;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                
                system.generateEmployeeReport(employeeId);
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
                int departmentId;
                cout << "Enter Department ID: ";
                cin >> departmentId;
                
                system.generateDepartmentReport(departmentId);
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

void fileMenu(EmployeeManagementSystem& system) {
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
                    filename = "employee_data.txt";
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
                    filename = "employee_data.txt";
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