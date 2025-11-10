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

using namespace std;

#define MAX_EMPLOYEES 500
#define MAX_PROJECTS 100
#define MAX_TASKS 1000
#define MAX_DEPARTMENTS 20

class Date {
private:
    int day, month, year;
    
public:
    Date(int d = 1, int m = 1, int y = 2023) : day(d), month(m), year(y) {}
    
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    void setDay(int d) { day = d; }
    void setMonth(int m) { month = m; }
    void setYear(int y) { year = y; }
    
    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
    
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
};

class Employee {
protected:
    int employeeId;
    string firstName;
    string lastName;
    string email;
    string phoneNumber;
    Date hireDate;
    string jobTitle;
    double salary;
    int departmentId;
    bool active;
    
public:
    Employee(int id = 0, const string& fname = "", const string& lname = "", 
              const string& email = "", const string& phone = "",
              const Date& hire = Date(), const string& title = "", 
              double sal = 0.0, int deptId = 0)
        : employeeId(id), firstName(fname), lastName(lname), email(email), 
          phoneNumber(phone), hireDate(hire), jobTitle(title), salary(sal), 
          departmentId(deptId) {
        active = true;
    }
    
    virtual ~Employee() {}
    
    int getEmployeeId() const { return employeeId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getEmail() const { return email; }
    string getPhoneNumber() const { return phoneNumber; }
    Date getHireDate() const { return hireDate; }
    string getJobTitle() const { return jobTitle; }
    double getSalary() const { return salary; }
    int getDepartmentId() const { return departmentId; }
    bool isActive() const { return active; }
    
    void setEmployeeId(int id) { employeeId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPhoneNumber(const string& phone) { phoneNumber = phone; }
    void setHireDate(const Date& date) { hireDate = date; }
    void setJobTitle(const string& title) { jobTitle = title; }
    void setSalary(double sal) { salary = sal; }
    void setDepartmentId(int id) { departmentId = id; }
    void setActive(bool status) { active = status; }
    
    void giveRaise(double percentage) {
        salary *= (1.0 + percentage / 100.0);
    }
    
    virtual void display() const {
        cout << "Employee ID: " << employeeId << endl;
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Hire Date: " << hireDate.toString() << endl;
        cout << "Job Title: " << jobTitle << endl;
        cout << "Salary: $" << fixed << setprecision(2) << salary << endl;
        cout << "Department ID: " << departmentId << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class Department {
private:
    int departmentId;
    string departmentName;
    string location;
    int managerId;
    double budget;
    bool active;
    
public:
    Department(int id = 0, const string& name = "", const string& loc = "", 
               int mgrId = 0, double budg = 0.0)
        : departmentId(id), departmentName(name), location(loc), 
          managerId(mgrId), budget(budg) {
        active = true;
    }
    
    int getDepartmentId() const { return departmentId; }
    string getDepartmentName() const { return departmentName; }
    string getLocation() const { return location; }
    int getManagerId() const { return managerId; }
    double getBudget() const { return budget; }
    bool isActive() const { return active; }
    
    void setDepartmentId(int id) { departmentId = id; }
    void setDepartmentName(const string& name) { departmentName = name; }
    void setLocation(const string& loc) { location = loc; }
    void setManagerId(int id) { managerId = id; }
    void setBudget(double budg) { budget = budg; }
    void setActive(bool status) { active = status; }
    
    void increaseBudget(double amount) {
        budget += amount;
    }
    
    void decreaseBudget(double amount) {
        if (budget >= amount) {
            budget -= amount;
        }
    }
    
    void display() const {
        cout << "Department ID: " << departmentId << endl;
        cout << "Department Name: " << departmentName << endl;
        cout << "Location: " << location << endl;
        cout << "Manager ID: " << managerId << endl;
        cout << "Budget: $" << fixed << setprecision(2) << budget << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class Project {
private:
    int projectId;
    string projectName;
    string description;
    Date startDate;
    Date endDate;
    double budget;
    int managerId;
    string status; // "Planning", "In Progress", "Completed", "On Hold", "Cancelled"
    bool active;
    
public:
    Project(int id = 0, const string& name = "", const string& desc = "",
           const Date& start = Date(), const Date& end = Date(), 
           double budg = 0.0, int mgrId = 0, const string& stat = "Planning")
        : projectId(id), projectName(name), description(desc), startDate(start), 
          endDate(end), budget(budg), managerId(mgrId), status(stat) {
        active = true;
    }
    
    int getProjectId() const { return projectId; }
    string getProjectName() const { return projectName; }
    string getDescription() const { return description; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    double getBudget() const { return budget; }
    int getManagerId() const { return managerId; }
    string getStatus() const { return status; }
    bool isActive() const { return active; }
    
    void setProjectId(int id) { projectId = id; }
    void setProjectName(const string& name) { projectName = name; }
    void setDescription(const string& desc) { description = desc; }
    void setStartDate(const Date& date) { startDate = date; }
    void setEndDate(const Date& date) { endDate = date; }
    void setBudget(double budg) { budget = budg; }
    void setManagerId(int id) { managerId = id; }
    void setStatus(const string& stat) { status = stat; }
    void setActive(bool status) { active = status; }
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
    
    void extendEndDate(int days) {
        // Simple implementation - just add days to the end date
        // In a real implementation, you'd need proper date arithmetic
        endDate.setDay(endDate.getDay() + days);
    }
    
    void display() const {
        cout << "Project ID: " << projectId << endl;
        cout << "Project Name: " << projectName << endl;
        cout << "Description: " << description << endl;
        cout << "Start Date: " << startDate.toString() << endl;
        cout << "End Date: " << endDate.toString() << endl;
        cout << "Budget: $" << fixed << setprecision(2) << budget << endl;
        cout << "Manager ID: " << managerId << endl;
        cout << "Status: " << status << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class Task {
private:
    int taskId;
    string taskName;
    string description;
    int projectId;
    int assignedToId;
    Date dueDate;
    string status; // "Not Started", "In Progress", "Completed", "Overdue"
    int priority; // 1-5, where 1 is lowest and 5 is highest
    int estimatedHours;
    int actualHours;
    bool active;
    
public:
    Task(int id = 0, const string& name = "", const string& desc = "",
         int projId = 0, int assignedId = 0, const Date& due = Date(),
         const string& stat = "Not Started", int prio = 3, int estHours = 0)
        : taskId(id), taskName(name), description(desc), projectId(projId), 
          assignedToId(assignedId), dueDate(due), status(stat), priority(prio), 
          estimatedHours(estHours) {
        actualHours = 0;
        active = true;
    }
    
    int getTaskId() const { return taskId; }
    string getTaskName() const { return taskName; }
    string getDescription() const { return description; }
    int getProjectId() const { return projectId; }
    int getAssignedToId() const { return assignedToId; }
    Date getDueDate() const { return dueDate; }
    string getStatus() const { return status; }
    int getPriority() const { return priority; }
    int getEstimatedHours() const { return estimatedHours; }
    int getActualHours() const { return actualHours; }
    bool isActive() const { return active; }
    
    void setTaskId(int id) { taskId = id; }
    void setTaskName(const string& name) { taskName = name; }
    void setDescription(const string& desc) { description = desc; }
    void setProjectId(int id) { projectId = id; }
    void setAssignedToId(int id) { assignedToId = id; }
    void setDueDate(const Date& date) { dueDate = date; }
    void setStatus(const string& stat) { status = stat; }
    void setPriority(int prio) { priority = prio; }
    void setEstimatedHours(int hours) { estimatedHours = hours; }
    void setActualHours(int hours) { actualHours = hours; }
    void setActive(bool status) { active = status; }
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
    
    void logHours(int hours) {
        actualHours += hours;
    }
    
    bool isOverdue() const {
        // Check if current date is past due date and task is not completed
        time_t now = time(nullptr);
        tm* timeinfo = localtime(&now);
        Date currentDate(timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
        
        return currentDate > dueDate && status != "Completed";
    }
    
    void display() const {
        cout << "Task ID: " << taskId << endl;
        cout << "Task Name: " << taskName << endl;
        cout << "Description: " << description << endl;
        cout << "Project ID: " << projectId << endl;
        cout << "Assigned To ID: " << assignedToId << endl;
        cout << "Due Date: " << dueDate.toString() << endl;
        cout << "Status: " << status;
        if (isOverdue()) {
            cout << " (OVERDUE)";
        }
        cout << endl;
        cout << "Priority: " << priority << "/5" << endl;
        cout << "Estimated Hours: " << estimatedHours << endl;
        cout << "Actual Hours: " << actualHours << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class ProjectManagementSystem {
private:
    vector<Employee> employees;
    vector<Department> departments;
    vector<Project> projects;
    vector<Task> tasks;
    map<int, int> employeeIdMap;
    map<int, int> departmentIdMap;
    map<int, int> projectIdMap;
    
public:
    ProjectManagementSystem() {}
    
    // Employee management
    bool addEmployee(const Employee& employee) {
        if (employees.size() >= MAX_EMPLOYEES) {
            return false;
        }
        
        employees.push_back(employee);
        int index = employees.size() - 1;
        employeeIdMap[employee.getEmployeeId()] = index;
        
        return true;
    }
    
    bool removeEmployee(int employeeId) {
        auto it = employeeIdMap.find(employeeId);
        if (it == employeeIdMap.end()) {
            return false;
        }
        
        int index = it->second;
        employees[index].setActive(false);
        
        // Update tasks assigned to this employee
        for (auto& task : tasks) {
            if (task.getAssignedToId() == employeeId && task.isActive()) {
                task.updateStatus("Unassigned");
                task.setAssignedToId(0);
            }
        }
        
        return true;
    }
    
    Employee* findEmployee(int employeeId) {
        auto it = employeeIdMap.find(employeeId);
        if (it == employeeIdMap.end() || !employees[it->second].isActive()) {
            return nullptr;
        }
        
        return &employees[it->second];
    }
    
    vector<Employee*> getAllEmployees() {
        vector<Employee*> result;
        for (auto& employee : employees) {
            if (employee.isActive()) {
                result.push_back(&employee);
            }
        }
        return result;
    }
    
    vector<Employee*> getEmployeesByDepartment(int departmentId) {
        vector<Employee*> result;
        for (auto& employee : employees) {
            if (employee.isActive() && employee.getDepartmentId() == departmentId) {
                result.push_back(&employee);
            }
        }
        return result;
    }
    
    // Department management
    bool addDepartment(const Department& department) {
        if (departments.size() >= MAX_DEPARTMENTS) {
            return false;
        }
        
        departments.push_back(department);
        int index = departments.size() - 1;
        departmentIdMap[department.getDepartmentId()] = index;
        
        return true;
    }
    
    bool removeDepartment(int departmentId) {
        auto it = departmentIdMap.find(departmentId);
        if (it == departmentIdMap.end()) {
            return false;
        }
        
        int index = it->second;
        departments[index].setActive(false);
        
        // Update employees in this department
        for (auto& employee : employees) {
            if (employee.isActive() && employee.getDepartmentId() == departmentId) {
                employee.setDepartmentId(0);
            }
        }
        
        return true;
    }
    
    Department* findDepartment(int departmentId) {
        auto it = departmentIdMap.find(departmentId);
        if (it == departmentIdMap.end() || !departments[it->second].isActive()) {
            return nullptr;
        }
        
        return &departments[it->second];
    }
    
    vector<Department*> getAllDepartments() {
        vector<Department*> result;
        for (auto& department : departments) {
            if (department.isActive()) {
                result.push_back(&department);
            }
        }
        return result;
    }
    
    // Project management
    bool addProject(const Project& project) {
        if (projects.size() >= MAX_PROJECTS) {
            return false;
        }
        
        projects.push_back(project);
        int index = projects.size() - 1;
        projectIdMap[project.getProjectId()] = index;
        
        return true;
    }
    
    bool removeProject(int projectId) {
        auto it = projectIdMap.find(projectId);
        if (it == projectIdMap.end()) {
            return false;
        }
        
        int index = it->second;
        projects[index].setActive(false);
        
        // Update tasks in this project
        for (auto& task : tasks) {
            if (task.getProjectId() == projectId && task.isActive()) {
                task.setActive(false);
            }
        }
        
        return true;
    }
    
    Project* findProject(int projectId) {
        auto it = projectIdMap.find(projectId);
        if (it == projectIdMap.end() || !projects[it->second].isActive()) {
            return nullptr;
        }
        
        return &projects[it->second];
    }
    
    vector<Project*> getAllProjects() {
        vector<Project*> result;
        for (auto& project : projects) {
            if (project.isActive()) {
                result.push_back(&project);
            }
        }
        return result;
    }
    
    vector<Project*> getProjectsByManager(int managerId) {
        vector<Project*> result;
        for (auto& project : projects) {
            if (project.isActive() && project.getManagerId() == managerId) {
                result.push_back(&project);
            }
        }
        return result;
    }
    
    // Task management
    bool addTask(const Task& task) {
        if (tasks.size() >= MAX_TASKS) {
            return false;
        }
        
        tasks.push_back(task);
        return true;
    }
    
    bool removeTask(int taskId) {
        for (auto& task : tasks) {
            if (task.getTaskId() == taskId && task.isActive()) {
                task.setActive(false);
                return true;
            }
        }
        
        return false;
    }
    
    Task* findTask(int taskId) {
        for (auto& task : tasks) {
            if (task.getTaskId() == taskId && task.isActive()) {
                return &task;
            }
        }
        
        return nullptr;
    }
    
    vector<Task*> getAllTasks() {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isActive()) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByProject(int projectId) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isActive() && task.getProjectId() == projectId) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByEmployee(int employeeId) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isActive() && task.getAssignedToId() == employeeId) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getOverdueTasks() {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isActive() && task.isOverdue()) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    vector<Task*> getTasksByStatus(const string& status) {
        vector<Task*> result;
        for (auto& task : tasks) {
            if (task.isActive() && task.getStatus() == status) {
                result.push_back(&task);
            }
        }
        return result;
    }
    
    // Reports
    void displayAllEmployees() const {
        cout << "=== All Employees ===" << endl;
        for (const auto& employee : employees) {
            if (employee.isActive()) {
                employee.display();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayAllDepartments() const {
        cout << "=== All Departments ===" << endl;
        for (const auto& department : departments) {
            if (department.isActive()) {
                department.display();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayAllProjects() const {
        cout << "=== All Projects ===" << endl;
        for (const auto& project : projects) {
            if (project.isActive()) {
                project.display();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayAllTasks() const {
        cout << "=== All Tasks ===" << endl;
        for (const auto& task : tasks) {
            if (task.isActive()) {
                task.display();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayDepartmentReport(int departmentId) const {
        const Department* department = nullptr;
        for (const auto& d : departments) {
            if (d.getDepartmentId() == departmentId && d.isActive()) {
                department = &d;
                break;
            }
        }
        
        if (!department) {
            cout << "Department not found." << endl;
            return;
        }
        
        cout << "=== Department Report ===" << endl;
        department->display();
        cout << endl;
        
        cout << "Employees in this department:" << endl;
        cout << "ID\tName\t\t\tJob Title\t\tSalary" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        int employeeCount = 0;
        double totalSalary = 0.0;
        
        for (const auto& employee : employees) {
            if (employee.isActive() && employee.getDepartmentId() == departmentId) {
                cout << employee.getEmployeeId() << "\t" 
                     << employee.getFirstName() << " " << employee.getLastName() << "\t\t" 
                     << employee.getJobTitle() << "\t\t$" 
                     << fixed << setprecision(2) << employee.getSalary() << endl;
                
                employeeCount++;
                totalSalary += employee.getSalary();
            }
        }
        
        cout << "------------------------------------------------------------" << endl;
        cout << "Total Employees: " << employeeCount << endl;
        cout << "Total Salary: $" << fixed << setprecision(2) << totalSalary << endl;
        
        if (employeeCount > 0) {
            cout << "Average Salary: $" << fixed << setprecision(2) << totalSalary / employeeCount << endl;
        }
        
        // Check budget
        cout << "Department Budget: $" << fixed << setprecision(2) << department->getBudget() << endl;
        if (totalSalary > department->getBudget()) {
            cout << "WARNING: Total salary exceeds department budget by $" 
                 << fixed << setprecision(2) << (totalSalary - department->getBudget()) << endl;
        } else {
            cout << "Budget remaining: $" << fixed << setprecision(2) 
                 << (department->getBudget() - totalSalary) << endl;
        }
    }
    
    void displayProjectReport(int projectId) const {
        const Project* project = nullptr;
        for (const auto& p : projects) {
            if (p.getProjectId() == projectId && p.isActive()) {
                project = &p;
                break;
            }
        }
        
        if (!project) {
            cout << "Project not found." << endl;
            return;
        }
        
        cout << "=== Project Report ===" << endl;
        project->display();
        cout << endl;
        
        cout << "Tasks in this project:" << endl;
        cout << "ID\tName\t\t\tStatus\t\tPriority\tDue Date\tHours" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        
        int taskCount = 0;
        int completedTasks = 0;
        int totalEstimatedHours = 0;
        int totalActualHours = 0;
        
        for (const auto& task : tasks) {
            if (task.isActive() && task.getProjectId() == projectId) {
                cout << task.getTaskId() << "\t" 
                     << task.getTaskName() << "\t\t" 
                     << task.getStatus() << "\t\t" 
                     << task.getPriority() << "\t\t" 
                     << task.getDueDate().toString() << "\t" 
                     << task.getActualHours() << "/" << task.getEstimatedHours() << endl;
                
                taskCount++;
                totalEstimatedHours += task.getEstimatedHours();
                totalActualHours += task.getActualHours();
                
                if (task.getStatus() == "Completed") {
                    completedTasks++;
                }
            }
        }
        
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "Total Tasks: " << taskCount << endl;
        cout << "Completed Tasks: " << completedTasks << endl;
        
        if (taskCount > 0) {
            cout << "Completion Rate: " << fixed << setprecision(1) 
                 << (double)completedTasks / taskCount * 100 << "%" << endl;
        }
        
        cout << "Total Estimated Hours: " << totalEstimatedHours << endl;
        cout << "Total Actual Hours: " << totalActualHours << endl;
        
        if (totalEstimatedHours > 0) {
            cout << "Hours Variance: " << (totalActualHours - totalEstimatedHours) << endl;
        }
    }
    
    void displayEmployeeReport(int employeeId) const {
        const Employee* employee = nullptr;
        for (const auto& e : employees) {
            if (e.getEmployeeId() == employeeId && e.isActive()) {
                employee = &e;
                break;
            }
        }
        
        if (!employee) {
            cout << "Employee not found." << endl;
            return;
        }
        
        cout << "=== Employee Report ===" << endl;
        employee->display();
        cout << endl;
        
        // Department information
        for (const auto& department : departments) {
            if (department.isActive() && department.getDepartmentId() == employee->getDepartmentId()) {
                cout << "Department: " << department.getDepartmentName() << endl;
                break;
            }
        }
        
        cout << endl;
        
        // Tasks assigned to this employee
        cout << "Tasks assigned to this employee:" << endl;
        cout << "ID\tName\t\t\tStatus\t\tPriority\tDue Date\tHours" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        
        int taskCount = 0;
        int completedTasks = 0;
        int overdueTasks = 0;
        int totalEstimatedHours = 0;
        int totalActualHours = 0;
        
        for (const auto& task : tasks) {
            if (task.isActive() && task.getAssignedToId() == employeeId) {
                cout << task.getTaskId() << "\t" 
                     << task.getTaskName() << "\t\t" 
                     << task.getStatus() << "\t\t" 
                     << task.getPriority() << "\t\t" 
                     << task.getDueDate().toString() << "\t" 
                     << task.getActualHours() << "/" << task.getEstimatedHours() << endl;
                
                taskCount++;
                totalEstimatedHours += task.getEstimatedHours();
                totalActualHours += task.getActualHours();
                
                if (task.getStatus() == "Completed") {
                    completedTasks++;
                }
                
                if (task.isOverdue()) {
                    overdueTasks++;
                }
            }
        }
        
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "Total Tasks: " << taskCount << endl;
        cout << "Completed Tasks: " << completedTasks << endl;
        cout << "Overdue Tasks: " << overdueTasks << endl;
        
        if (taskCount > 0) {
            cout << "Completion Rate: " << fixed << setprecision(1) 
                 << (double)completedTasks / taskCount * 100 << "%" << endl;
        }
        
        cout << "Total Estimated Hours: " << totalEstimatedHours << endl;
        cout << "Total Actual Hours: " << totalActualHours << endl;
        
        if (totalEstimatedHours > 0) {
            cout << "Hours Variance: " << (totalActualHours - totalEstimatedHours) << endl;
            cout << "Efficiency: " << fixed << setprecision(1) 
                 << (double)totalEstimatedHours / totalActualHours * 100 << "%" << endl;
        }
        
        // Projects managed by this employee
        cout << endl << "Projects managed by this employee:" << endl;
        cout << "ID\tName\t\t\tStatus\t\tStart Date\tEnd Date" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        int projectCount = 0;
        
        for (const auto& project : projects) {
            if (project.isActive() && project.getManagerId() == employeeId) {
                cout << project.getProjectId() << "\t" 
                     << project.getProjectName() << "\t\t" 
                     << project.getStatus() << "\t\t" 
                     << project.getStartDate().toString() << "\t" 
                     << project.getEndDate().toString() << endl;
                
                projectCount++;
            }
        }
        
        cout << "------------------------------------------------------------" << endl;
        cout << "Total Projects Managed: " << projectCount << endl;
    }
    
    void displaySystemStatistics() const {
        cout << "=== System Statistics ===" << endl;
        
        // Employee statistics
        int totalEmployees = 0;
        int activeEmployees = 0;
        double totalSalary = 0.0;
        map<int, int> deptEmployeeCount;
        
        for (const auto& employee : employees) {
            totalEmployees++;
            if (employee.isActive()) {
                activeEmployees++;
                totalSalary += employee.getSalary();
                deptEmployeeCount[employee.getDepartmentId()]++;
            }
        }
        
        cout << "Employee Statistics:" << endl;
        cout << "Total Employees: " << totalEmployees << endl;
        cout << "Active Employees: " << activeEmployees << endl;
        if (activeEmployees > 0) {
            cout << "Average Salary: $" << fixed << setprecision(2) << totalSalary / activeEmployees << endl;
        }
        cout << "Employees by Department:" << endl;
        
        for (const auto& pair : deptEmployeeCount) {
            int deptId = pair.first;
            int count = pair.second;
            
            for (const auto& department : departments) {
                if (department.isActive() && department.getDepartmentId() == deptId) {
                    cout << "  " << department.getDepartmentName() << ": " << count << endl;
                    break;
                }
            }
        }
        cout << endl;
        
        // Department statistics
        int totalDepartments = 0;
        int activeDepartments = 0;
        double totalBudget = 0.0;
        
        for (const auto& department : departments) {
            totalDepartments++;
            if (department.isActive()) {
                activeDepartments++;
                totalBudget += department.getBudget();
            }
        }
        
        cout << "Department Statistics:" << endl;
        cout << "Total Departments: " << totalDepartments << endl;
        cout << "Active Departments: " << activeDepartments << endl;
        cout << "Total Budget: $" << fixed << setprecision(2) << totalBudget << endl;
        if (activeDepartments > 0) {
            cout << "Average Budget: $" << fixed << setprecision(2) << totalBudget / activeDepartments << endl;
        }
        cout << endl;
        
        // Project statistics
        int totalProjects = 0;
        int activeProjects = 0;
        map<string, int> projectStatusCount;
        
        for (const auto& project : projects) {
            totalProjects++;
            if (project.isActive()) {
                activeProjects++;
                projectStatusCount[project.getStatus()]++;
            }
        }
        
        cout << "Project Statistics:" << endl;
        cout << "Total Projects: " << totalProjects << endl;
        cout << "Active Projects: " << activeProjects << endl;
        cout << "Projects by Status:" << endl;
        
        for (const auto& pair : projectStatusCount) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
        cout << endl;
        
        // Task statistics
        int totalTasks = 0;
        int activeTasks = 0;
        int overdueTasks = 0;
        map<string, int> taskStatusCount;
        
        for (const auto& task : tasks) {
            totalTasks++;
            if (task.isActive()) {
                activeTasks++;
                taskStatusCount[task.getStatus()]++;
                
                if (task.isOverdue()) {
                    overdueTasks++;
                }
            }
        }
        
        cout << "Task Statistics:" << endl;
        cout << "Total Tasks: " << totalTasks << endl;
        cout << "Active Tasks: " << activeTasks << endl;
        cout << "Overdue Tasks: " << overdueTasks << endl;
        cout << "Tasks by Status:" << endl;
        
        for (const auto& pair : taskStatusCount) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save employees
        outFile << "EMPLOYEES" << endl;
        for (const auto& employee : employees) {
            outFile << employee.getEmployeeId() << "|" 
                    << employee.getFirstName() << "|" 
                    << employee.getLastName() << "|" 
                    << employee.getEmail() << "|" 
                    << employee.getPhoneNumber() << "|" 
                    << employee.getHireDate().toString() << "|" 
                    << employee.getJobTitle() << "|" 
                    << employee.getSalary() << "|" 
                    << employee.getDepartmentId() << "|" 
                    << (employee.isActive() ? "1" : "0") << endl;
        }
        
        // Save departments
        outFile << "DEPARTMENTS" << endl;
        for (const auto& department : departments) {
            outFile << department.getDepartmentId() << "|" 
                    << department.getDepartmentName() << "|" 
                    << department.getLocation() << "|" 
                    << department.getManagerId() << "|" 
                    << department.getBudget() << "|" 
                    << (department.isActive() ? "1" : "0") << endl;
        }
        
        // Save projects
        outFile << "PROJECTS" << endl;
        for (const auto& project : projects) {
            outFile << project.getProjectId() << "|" 
                    << project.getProjectName() << "|" 
                    << project.getDescription() << "|" 
                    << project.getStartDate().toString() << "|" 
                    << project.getEndDate().toString() << "|" 
                    << project.getBudget() << "|" 
                    << project.getManagerId() << "|" 
                    << project.getStatus() << "|" 
                    << (project.isActive() ? "1" : "0") << endl;
        }
        
        // Save tasks
        outFile << "TASKS" << endl;
        for (const auto& task : tasks) {
            outFile << task.getTaskId() << "|" 
                    << task.getTaskName() << "|" 
                    << task.getDescription() << "|" 
                    << task.getProjectId() << "|" 
                    << task.getAssignedToId() << "|" 
                    << task.getDueDate().toString() << "|" 
                    << task.getStatus() << "|" 
                    << task.getPriority() << "|" 
                    << task.getEstimatedHours() << "|" 
                    << task.getActualHours() << "|" 
                    << (task.isActive() ? "1" : "0") << endl;
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
            if (line == "EMPLOYEES") {
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
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "EMPLOYEES" && tokens.size() >= 10) {
                int employeeId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string email = tokens[3];
                string phone = tokens[4];
                
                // Parse hire date
                vector<string> dateTokens;
                stringstream dateSs(tokens[5]);
                string dateToken;
                while (getline(dateSs, dateToken, '/')) {
                    dateTokens.push_back(dateToken);
                }
                
                Date hireDate(stoi(dateTokens[0]), stoi(dateTokens[1]), stoi(dateTokens[2]));
                
                string jobTitle = tokens[6];
                double salary = stod(tokens[7]);
                int departmentId = stoi(tokens[8]);
                bool active = tokens[9] == "1";
                
                Employee employee(employeeId, firstName, lastName, email, phone, 
                              hireDate, jobTitle, salary, departmentId);
                employee.setActive(active);
                
                addEmployee(employee);
            } else if (section == "DEPARTMENTS" && tokens.size() >= 6) {
                int departmentId = stoi(tokens[0]);
                string departmentName = tokens[1];
                string location = tokens[2];
                int managerId = stoi(tokens[3]);
                double budget = stod(tokens[4]);
                bool active = tokens[5] == "1";
                
                Department department(departmentId, departmentName, location, managerId, budget);
                department.setActive(active);
                
                addDepartment(department);
            } else if (section == "PROJECTS" && tokens.size() >= 9) {
                int projectId = stoi(tokens[0]);
                string projectName = tokens[1];
                string description = tokens[2];
                
                // Parse dates
                vector<string> startDateTokens;
                stringstream startSs(tokens[3]);
                string startToken;
                while (getline(startSs, startToken, '/')) {
                    startDateTokens.push_back(startToken);
                }
                
                vector<string> endDateTokens;
                stringstream endSs(tokens[4]);
                string endToken;
                while (getline(endSs, endToken, '/')) {
                    endDateTokens.push_back(endToken);
                }
                
                Date startDate(stoi(startDateTokens[0]), stoi(startDateTokens[1]), stoi(startDateTokens[2]));
                Date endDate(stoi(endDateTokens[0]), stoi(endDateTokens[1]), stoi(endDateTokens[2]));
                
                double budget = stod(tokens[5]);
                int managerId = stoi(tokens[6]);
                string status = tokens[7];
                bool active = tokens[8] == "1";
                
                Project project(projectId, projectName, description, startDate, endDate, budget, managerId, status);
                project.setActive(active);
                
                addProject(project);
            } else if (section == "TASKS" && tokens.size() >= 12) {
                int taskId = stoi(tokens[0]);
                string taskName = tokens[1];
                string description = tokens[2];
                int projectId = stoi(tokens[3]);
                int assignedToId = stoi(tokens[4]);
                
                // Parse due date
                vector<string> dateTokens;
                stringstream dateSs(tokens[5]);
                string dateToken;
                while (getline(dateSs, dateToken, '/')) {
                    dateTokens.push_back(dateToken);
                }
                
                Date dueDate(stoi(dateTokens[0]), stoi(dateTokens[1]), stoi(dateTokens[2]));
                
                string status = tokens[6];
                int priority = stoi(tokens[7]);
                int estimatedHours = stoi(tokens[8]);
                int actualHours = stoi(tokens[9]);
                bool active = tokens[10] == "1";
                
                Task task(taskId, taskName, description, projectId, assignedToId, dueDate, status, priority, estimatedHours);
                task.setActualHours(actualHours);
                task.setActive(active);
                
                addTask(task);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Utility functions
    void generateSampleData() {
        // Sample departments
        vector<string> departmentNames = {"Engineering", "Marketing", "Sales", "Human Resources", "Finance",
                                       "Research", "Operations", "Customer Service", "Legal", "IT"};
        
        vector<string> locations = {"Building A", "Building B", "Building C", "Building D", "Building E"};
        
        // Create sample departments
        for (int i = 0; i < 10; i++) {
            int departmentId = 100 + i;
            string departmentName = departmentNames[i];
            string location = locations[i % locations.size()];
            int managerId = 1000 + i * 10; // Will be set later when employees are created
            double budget = 500000.0 + (rand() % 1000000);
            
            Department department(departmentId, departmentName, location, managerId, budget);
            addDepartment(department);
        }
        
        // Sample employee data
        vector<string> firstNames = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                                   "David", "Jessica", "William", "Ashley", "James", "Amanda",
                                   "Daniel", "Jennifer", "Matthew", "Melissa"};
        
        vector<string> lastNames = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                                  "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
        
        vector<string> jobTitles = {"Software Engineer", "Project Manager", "Product Manager", "Sales Manager",
                                  "Marketing Manager", "HR Manager", "Finance Manager", "Research Scientist",
                                  "Operations Manager", "Customer Service Manager", "Legal Counsel", "IT Manager"};
        
        // Create sample employees
        for (int i = 0; i < 50; i++) {
            int employeeId = 1000 + i;
            string firstName = firstNames[rand() % firstNames.size()];
            string lastName = lastNames[rand() % lastNames.size()];
            string email = firstName + "." + lastName + to_string(employeeId) + "@company.com";
            string phone = "555-" + to_string(100 + rand() % 900);
            
            // Random hire date (1-10 years ago)
            time_t now = time(nullptr);
            time_t hireTime = now - ((rand() % 10 + 1) * 365 * 24 * 60 * 60);
            tm* hireTimeinfo = localtime(&hireTime);
            Date hireDate(hireTimeinfo->tm_mday, hireTimeinfo->tm_mon + 1, hireTimeinfo->tm_year + 1900);
            
            string jobTitle = jobTitles[rand() % jobTitles.size()];
            double salary = 40000.0 + (rand() % 100000) + (rand() % 100) / 100.0;
            int departmentId = 100 + (rand() % 10);
            
            Employee employee(employeeId, firstName, lastName, email, phone, hireDate, jobTitle, salary, departmentId);
            addEmployee(employee);
        }
        
        // Update department managers
        for (int i = 0; i < 10; i++) {
            int departmentId = 100 + i;
            int managerId = 1000 + i * 10;
            
            Department* department = findDepartment(departmentId);
            Employee* manager = findEmployee(managerId);
            
            if (department && manager) {
                department->setManagerId(managerId);
            }
        }
        
        // Sample projects
        vector<string> projectNames = {"Website Redesign", "Mobile App Development", "Database Migration",
                                     "Security Audit", "Marketing Campaign", "Sales System Upgrade",
                                     "HR Portal", "Financial Reporting", "Research Study", "Process Automation"};
        
        vector<string> projectStatuses = {"Planning", "In Progress", "Completed", "On Hold"};
        
        // Create sample projects
        for (int i = 0; i < 20; i++) {
            int projectId = 500 + i;
            string projectName = projectNames[rand() % projectNames.size()];
            string description = "Detailed description of " + projectName + " project with objectives and deliverables";
            
            // Random start date (within last 6 months)
            time_t now = time(nullptr);
            time_t startTime = now - ((rand() % 180) * 24 * 60 * 60);
            tm* startTimeinfo = localtime(&startTime);
            Date startDate(startTimeinfo->tm_mday, startTimeinfo->tm_mon + 1, startTimeinfo->tm_year + 1900);
            
            // Random end date (within next year)
            time_t endTime = now + ((rand() % 365) * 24 * 60 * 60);
            tm* endTimeinfo = localtime(&endTime);
            Date endDate(endTimeinfo->tm_mday, endTimeinfo->tm_mon + 1, endTimeinfo->tm_year + 1900);
            
            double budget = 10000.0 + (rand() % 100000);
            int managerId = 1000 + (rand() % 50);
            string status = projectStatuses[rand() % projectStatuses.size()];
            
            Project project(projectId, projectName, description, startDate, endDate, budget, managerId, status);
            addProject(project);
        }
        
        // Sample tasks
        vector<string> taskNames = {"Requirements Analysis", "Design Documentation", "Implementation",
                                   "Testing", "Deployment", "User Training", "Bug Fixing",
                                   "Code Review", "Performance Optimization", "Security Review"};
        
        vector<string> taskStatuses = {"Not Started", "In Progress", "Completed"};
        
        // Create sample tasks
        for (int i = 0; i < 100; i++) {
            int taskId = 2000 + i;
            string taskName = taskNames[rand() % taskNames.size()];
            string description = "Detailed description of " + taskName + " task with specific requirements";
            int projectId = 500 + (rand() % 20);
            int assignedToId = (rand() % 2 == 0) ? 1000 + (rand() % 50) : 0; // 50% chance of being assigned
            
            // Random due date (within next 3 months)
            time_t now = time(nullptr);
            time_t dueTime = now + ((rand() % 90) * 24 * 60 * 60);
            tm* dueTimeinfo = localtime(&dueTime);
            Date dueDate(dueTimeinfo->tm_mday, dueTimeinfo->tm_mon + 1, dueTimeinfo->tm_year + 1900);
            
            string status = taskStatuses[rand() % taskStatuses.size()];
            int priority = 1 + (rand() % 5);
            int estimatedHours = 4 + (rand() % 40);
            
            Task task(taskId, taskName, description, projectId, assignedToId, dueDate, status, priority, estimatedHours);
            
            // Set actual hours if task is completed
            if (status == "Completed") {
                int actualHours = estimatedHours * (0.8 + ((double)rand() / RAND_MAX * 0.4));
                task.setActualHours(actualHours);
            }
            
            addTask(task);
        }
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(ProjectManagementSystem& system, int choice);
void addEmployeeMenu(ProjectManagementSystem& system);
void addDepartmentMenu(ProjectManagementSystem& system);
void addProjectMenu(ProjectManagementSystem& system);
void addTaskMenu(ProjectManagementSystem& system);
void assignTaskMenu(ProjectManagementSystem& system);
void updateTaskStatusMenu(ProjectManagementSystem& system);
void viewEmployeeReportMenu(ProjectManagementSystem& system);
void viewProjectReportMenu(ProjectManagementSystem& system);
void viewDepartmentReportMenu(ProjectManagementSystem& system);
void viewSystemStatisticsMenu(ProjectManagementSystem& system);

int main() {
    ProjectManagementSystem system;
    
    cout << "Project Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("project_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("project_data.txt");
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
    system.saveToFile("project_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Project Management System" << endl;
    cout << "1. Add Employee" << endl;
    cout << "2. Add Department" << endl;
    cout << "3. Add Project" << endl;
    cout << "4. Add Task" << endl;
    cout << "5. Assign Task to Employee" << endl;
    cout << "6. Update Task Status" << endl;
    cout << "7. View All Employees" << endl;
    cout << "8. View All Departments" << endl;
    cout << "9. View All Projects" << endl;
    cout << "10. View All Tasks" << endl;
    cout << "11. View Employee Report" << endl;
    cout << "12. View Project Report" << endl;
    cout << "13. View Department Report" << endl;
    cout << "14. View System Statistics" << endl;
    cout << "15. Save Data to File" << endl;
    cout << "16. Load Data from File" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(ProjectManagementSystem& system, int choice) {
    switch (choice) {
        case 1:
            addEmployeeMenu(system);
            break;
        case 2:
            addDepartmentMenu(system);
            break;
        case 3:
            addProjectMenu(system);
            break;
        case 4:
            addTaskMenu(system);
            break;
        case 5:
            assignTaskMenu(system);
            break;
        case 6:
            updateTaskStatusMenu(system);
            break;
        case 7:
            system.displayAllEmployees();
            break;
        case 8:
            system.displayAllDepartments();
            break;
        case 9:
            system.displayAllProjects();
            break;
        case 10:
            system.displayAllTasks();
            break;
        case 11:
            viewEmployeeReportMenu(system);
            break;
        case 12:
            viewProjectReportMenu(system);
            break;
        case 13:
            viewDepartmentReportMenu(system);
            break;
        case 14:
            viewSystemStatisticsMenu(system);
            break;
        case 15:
            if (system.saveToFile("project_data.txt")) {
                cout << "Data saved successfully." << endl;
            } else {
                cout << "Failed to save data." << endl;
            }
            break;
        case 16:
            if (system.loadFromFile("project_data.txt")) {
                cout << "Data loaded successfully." << endl;
            } else {
                cout << "Failed to load data." << endl;
            }
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void addEmployeeMenu(ProjectManagementSystem& system) {
    int employeeId, departmentId;
    string firstName, lastName, email, phone, jobTitle;
    double salary;
    int day, month, year;
    
    cout << "Enter Employee ID: ";
    cin >> employeeId;
    cin.ignore();
    
    cout << "Enter First Name: ";
    getline(cin, firstName);
    
    cout << "Enter Last Name: ";
    getline(cin, lastName);
    
    cout << "Enter Email: ";
    getline(cin, email);
    
    cout << "Enter Phone: ";
    getline(cin, phone);
    
    cout << "Enter Hire Date (DD MM YYYY): ";
    cin >> day >> month >> year;
    
    cin.ignore();
    cout << "Enter Job Title: ";
    getline(cin, jobTitle);
    
    cout << "Enter Salary: ";
    cin >> salary;
    
    cout << "Enter Department ID: ";
    cin >> departmentId;
    
    Date hireDate(day, month, year);
    Employee employee(employeeId, firstName, lastName, email, phone, hireDate, jobTitle, salary, departmentId);
    
    if (system.addEmployee(employee)) {
        cout << "Employee added successfully." << endl;
    } else {
        cout << "Failed to add employee. The employee ID may already exist or database is full." << endl;
    }
}

void addDepartmentMenu(ProjectManagementSystem& system) {
    int departmentId, managerId;
    string departmentName, location;
    double budget;
    
    cout << "Enter Department ID: ";
    cin >> departmentId;
    cin.ignore();
    
    cout << "Enter Department Name: ";
    getline(cin, departmentName);
    
    cout << "Enter Location: ";
    getline(cin, location);
    
    cout << "Enter Manager ID (0 for none): ";
    cin >> managerId;
    
    cout << "Enter Budget: ";
    cin >> budget;
    
    Department department(departmentId, departmentName, location, managerId, budget);
    
    if (system.addDepartment(department)) {
        cout << "Department added successfully." << endl;
    } else {
        cout << "Failed to add department. The department ID may already exist or database is full." << endl;
    }
}

void addProjectMenu(ProjectManagementSystem& system) {
    int projectId, managerId;
    string projectName, description, status;
    double budget;
    int startDay, startMonth, startYear;
    int endDay, endMonth, endYear;
    
    cout << "Enter Project ID: ";
    cin >> projectId;
    cin.ignore();
    
    cout << "Enter Project Name: ";
    getline(cin, projectName);
    
    cout << "Enter Description: ";
    getline(cin, description);
    
    cout << "Enter Start Date (DD MM YYYY): ";
    cin >> startDay >> startMonth >> startYear;
    
    cout << "Enter End Date (DD MM YYYY): ";
    cin >> endDay >> endMonth >> endYear;
    
    cout << "Enter Budget: ";
    cin >> budget;
    
    cout << "Enter Manager ID: ";
    cin >> managerId;
    
    cin.ignore();
    cout << "Enter Status: ";
    getline(cin, status);
    
    Date startDate(startDay, startMonth, startYear);
    Date endDate(endDay, endMonth, endYear);
    Project project(projectId, projectName, description, startDate, endDate, budget, managerId, status);
    
    if (system.addProject(project)) {
        cout << "Project added successfully." << endl;
    } else {
        cout << "Failed to add project. The project ID may already exist or database is full." << endl;
    }
}

void addTaskMenu(ProjectManagementSystem& system) {
    int taskId, projectId, assignedToId, priority, estimatedHours;
    string taskName, description, status;
    int day, month, year;
    
    cout << "Enter Task ID: ";
    cin >> taskId;
    cin.ignore();
    
    cout << "Enter Task Name: ";
    getline(cin, taskName);
    
    cout << "Enter Description: ";
    getline(cin, description);
    
    cout << "Enter Project ID: ";
    cin >> projectId;
    
    cout << "Enter Assigned To ID (0 for none): ";
    cin >> assignedToId;
    
    cout << "Enter Due Date (DD MM YYYY): ";
    cin >> day >> month >> year;
    
    cin.ignore();
    cout << "Enter Status: ";
    getline(cin, status);
    
    cout << "Enter Priority (1-5): ";
    cin >> priority;
    
    cout << "Enter Estimated Hours: ";
    cin >> estimatedHours;
    
    Date dueDate(day, month, year);
    Task task(taskId, taskName, description, projectId, assignedToId, dueDate, status, priority, estimatedHours);
    
    if (system.addTask(task)) {
        cout << "Task added successfully." << endl;
    } else {
        cout << "Failed to add task. The task ID may already exist or database is full." << endl;
    }
}

void assignTaskMenu(ProjectManagementSystem& system) {
    int taskId, employeeId;
    
    cout << "Enter Task ID: ";
    cin >> taskId;
    
    cout << "Enter Employee ID (0 to unassign): ";
    cin >> employeeId;
    
    Task* task = system.findTask(taskId);
    if (task) {
        task->setAssignedToId(employeeId);
        if (employeeId > 0) {
            task->updateStatus("In Progress");
        } else {
            task->updateStatus("Not Started");
        }
        cout << "Task assigned successfully." << endl;
    } else {
        cout << "Task not found." << endl;
    }
}

void updateTaskStatusMenu(ProjectManagementSystem& system) {
    int taskId;
    string status;
    
    cout << "Enter Task ID: ";
    cin >> taskId;
    cin.ignore();
    
    cout << "Enter New Status: ";
    getline(cin, status);
    
    Task* task = system.findTask(taskId);
    if (task) {
        task->updateStatus(status);
        cout << "Task status updated successfully." << endl;
    } else {
        cout << "Task not found." << endl;
    }
}

void viewEmployeeReportMenu(ProjectManagementSystem& system) {
    int employeeId;
    
    cout << "Enter Employee ID: ";
    cin >> employeeId;
    
    system.displayEmployeeReport(employeeId);
}

void viewProjectReportMenu(ProjectManagementSystem& system) {
    int projectId;
    
    cout << "Enter Project ID: ";
    cin >> projectId;
    
    system.displayProjectReport(projectId);
}

void viewDepartmentReportMenu(ProjectManagementSystem& system) {
    int departmentId;
    
    cout << "Enter Department ID: ";
    cin >> departmentId;
    
    system.displayDepartmentReport(departmentId);
}

void viewSystemStatisticsMenu(ProjectManagementSystem& system) {
    system.displaySystemStatistics();
}