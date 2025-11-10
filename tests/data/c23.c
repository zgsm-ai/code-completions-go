#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_EMPLOYEES 1000
#define MAX_PROJECTS 500
#define MAX_TASKS 2000
#define MAX_DEPARTMENTS 50
#define MAX_NAME_LENGTH 50
#define FILENAME_EMPLOYEES "employees.dat"
#define FILENAME_PROJECTS "projects.dat"
#define FILENAME_TASKS "tasks.dat"
#define FILENAME_DEPARTMENTS "departments.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char address[100];
    Date hire_date;
    int department_id;
    char position[MAX_NAME_LENGTH];
    double salary;
    int active;
    int skill_level;
    char skills[200];
    Date birth_date;
} Employee;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char description[200];
    Date start_date;
    Date end_date;
    int department_id;
    int manager_id;
    char status[20]; // "Planning", "In Progress", "Completed", "Cancelled"
    double budget;
    int active;
    int priority; // 1-5, 5 highest
} Project;

typedef struct {
    int id;
    int project_id;
    int employee_id;
    char title[MAX_NAME_LENGTH];
    char description[200];
    Date due_date;
    Date completion_date;
    int hours_worked;
    int hours_estimated;
    char status[20]; // "Not Started", "In Progress", "Completed", "Overdue"
    int priority; // 1-5, 5 highest
    int active;
} Task;

typedef struct {
    int id;
    char name[50];
    char description[200];
    char manager_name[MAX_NAME_LENGTH];
    int budget;
    int active;
} Department;

typedef struct {
    Employee employees[MAX_EMPLOYEES];
    int employee_count;
    Project projects[MAX_PROJECTS];
    int project_count;
    Task tasks[MAX_TASKS];
    int task_count;
    Department departments[MAX_DEPARTMENTS];
    int department_count;
} ProjectManagementDatabase;

// Function prototypes
void initialize_database(ProjectManagementDatabase *db);
int add_employee(ProjectManagementDatabase *db, const char *first_name, const char *last_name,
                const char *email, const char *phone, const char *address,
                Date birth_date, int department_id, const char *position, double salary, int skill_level, const char *skills);
int add_project(ProjectManagementDatabase *db, const char *name, const char *description,
               Date start_date, Date end_date, int department_id, int manager_id,
               const char *status, double budget, int priority);
int add_department(ProjectManagementDatabase *db, const char *name, const char *description,
                 const char *manager_name, int budget);
int add_task(ProjectManagementDatabase *db, int project_id, int employee_id, const char *title,
            const char *description, Date due_date, int hours_estimated, int priority);
void assign_task_to_employee(ProjectManagementDatabase *db, int task_id, int employee_id);
void update_task_status(ProjectManagementDatabase *db, int task_id, const char *status, int hours_worked);
void update_project_status(ProjectManagementDatabase *db, int project_id, const char *status);
void display_employee(const Employee *employee, const ProjectManagementDatabase *db);
void display_project(const Project *project, const ProjectManagementDatabase *db);
void display_task(const Task *task, const ProjectManagementDatabase *db);
void display_department(const Department *department);
void display_all_employees(const ProjectManagementDatabase *db);
void display_all_projects(const ProjectManagementDatabase *db);
void display_all_tasks(const ProjectManagementDatabase *db);
void display_all_departments(const ProjectManagementDatabase *db);
void search_employees_by_name(const ProjectManagementDatabase *db, const char *name);
void search_projects_by_status(const ProjectManagementDatabase *db, const char *status);
void search_tasks_by_employee(const ProjectManagementDatabase *db, int employee_id);
void generate_employee_report(const ProjectManagementDatabase *db, int employee_id);
void generate_project_report(const ProjectManagementDatabase *db, int project_id);
void generate_department_report(const ProjectManagementDatabase *db, int department_id);
void generate_workload_report(const ProjectManagementDatabase *db);
void generate_project_timeline(const ProjectManagementDatabase *db, int project_id);
void generate_task_overview(const ProjectManagementDatabase *db);
int find_employee_by_id(const ProjectManagementDatabase *db, int employee_id);
int find_project_by_id(const ProjectManagementDatabase *db, int project_id);
int find_task_by_id(const ProjectManagementDatabase *db, int task_id);
int find_department_by_id(const ProjectManagementDatabase *db, int department_id);
int save_database(const ProjectManagementDatabase *db);
int load_database(ProjectManagementDatabase *db);
void generate_sample_data(ProjectManagementDatabase *db);
void update_employee_salary(ProjectManagementDatabase *db, int employee_id, double new_salary);
void add_skill_to_employee(ProjectManagementDatabase *db, int employee_id, const char *skill);
int is_valid_email(const char *email);
int is_valid_date(int day, int month, int year);
const char* get_project_status_description(const char *status);
const char* get_task_status_description(const char *status);

int main() {
    ProjectManagementDatabase db;
    initialize_database(&db);
    
    printf("Project Management System\n");
    printf("1. Add Employee\n");
    printf("2. Add Project\n");
    printf("3. Add Department\n");
    printf("4. Add Task\n");
    printf("5. Assign Task to Employee\n");
    printf("6. Update Task Status\n");
    printf("7. Update Project Status\n");
    printf("8. Display All Employees\n");
    printf("9. Display All Projects\n");
    printf("10. Display All Tasks\n");
    printf("11. Display All Departments\n");
    printf("12. Search Employees by Name\n");
    printf("13. Search Projects by Status\n");
    printf("14. Search Tasks by Employee\n");
    printf("15. Generate Employee Report\n");
    printf("16. Generate Project Report\n");
    printf("17. Generate Department Report\n");
    printf("18. Generate Workload Report\n");
    printf("19. Generate Project Timeline\n");
    printf("20. Generate Task Overview\n");
    printf("21. Update Employee Salary\n");
    printf("22. Add Skill to Employee\n");
    printf("23. Save Database\n");
    printf("24. Load Database\n");
    printf("25. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100], position[MAX_NAME_LENGTH], skills[200];
                int department_id, skill_level;
                double salary;
                int day, month, year;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                printf("Enter position: ");
                scanf(" %[^\n]", position);
                printf("Enter salary: ");
                scanf("%lf", &salary);
                printf("Enter skill level (1-5): ");
                scanf("%d", &skill_level);
                printf("Enter skills (comma separated): ");
                scanf(" %[^\n]", skills);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                if (skill_level < 1 || skill_level > 5) {
                    printf("Skill level must be between 1 and 5.\n");
                    break;
                }
                
                Date birth_date = {day, month, year};
                
                int employee_id = add_employee(&db, first_name, last_name, email, phone, address,
                                           birth_date, department_id, position, salary, 
                                           skill_level, skills);
                if (employee_id != -1) {
                    printf("Employee added with ID: %d\n", employee_id);
                } else {
                    printf("Failed to add employee. Database is full.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_NAME_LENGTH], description[200], status[20];
                int department_id, manager_id, priority;
                double budget;
                int start_day, start_month, start_year;
                int end_day, end_month, end_year;
                
                printf("Enter project name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &start_day, &start_month, &start_year);
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &end_day, &end_month, &end_year);
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                printf("Enter manager ID: ");
                scanf("%d", &manager_id);
                printf("Enter status (Planning, In Progress, Completed, Cancelled): ");
                scanf(" %[^\n]", status);
                printf("Enter budget: ");
                scanf("%lf", &budget);
                printf("Enter priority (1-5, 5 highest): ");
                scanf("%d", &priority);
                
                if (!is_valid_date(start_day, start_month, start_year) || 
                    !is_valid_date(end_day, end_month, end_year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                if (priority < 1 || priority > 5) {
                    printf("Priority must be between 1 and 5.\n");
                    break;
                }
                
                if (strcmp(status, "Planning") != 0 && strcmp(status, "In Progress") != 0 &&
                    strcmp(status, "Completed") != 0 && strcmp(status, "Cancelled") != 0) {
                    printf("Invalid status. Must be one of: Planning, In Progress, Completed, Cancelled.\n");
                    break;
                }
                
                Date start_date = {start_day, start_month, start_year};
                Date end_date = {end_day, end_month, end_year};
                
                int project_id = add_project(&db, name, description, start_date, end_date,
                                           department_id, manager_id, status, budget, priority);
                if (project_id != -1) {
                    printf("Project added with ID: %d\n", project_id);
                } else {
                    printf("Failed to add project. Database is full.\n");
                }
                break;
            }
            case 3: {
                char name[50], description[200], manager_name[MAX_NAME_LENGTH];
                int budget;
                
                printf("Enter department name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter manager name: ");
                scanf(" %[^\n]", manager_name);
                printf("Enter budget: ");
                scanf("%d", &budget);
                
                int department_id = add_department(db, name, description, manager_name, budget);
                if (department_id != -1) {
                    printf("Department added with ID: %d\n", department_id);
                } else {
                    printf("Failed to add department. Database is full.\n");
                }
                break;
            }
            case 4: {
                int project_id, employee_id, hours_estimated, priority;
                char title[MAX_NAME_LENGTH], description[200];
                int due_day, due_month, due_year;
                
                printf("Enter project ID: ");
                scanf("%d", &project_id);
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                printf("Enter task title: ");
                scanf(" %[^\n]", title);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter due date (DD MM YYYY): ");
                scanf("%d %d %d", &due_day, &due_month, &due_year);
                printf("Enter estimated hours: ");
                scanf("%d", &hours_estimated);
                printf("Enter priority (1-5, 5 highest): ");
                scanf("%d", &priority);
                
                if (!is_valid_date(due_day, due_month, due_year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                if (priority < 1 || priority > 5) {
                    printf("Priority must be between 1 and 5.\n");
                    break;
                }
                
                Date due_date = {due_day, due_month, due_year};
                
                int task_id = add_task(&db, project_id, employee_id, title, description,
                                     due_date, hours_estimated, priority);
                if (task_id != -1) {
                    printf("Task added with ID: %d\n", task_id);
                } else {
                    printf("Failed to add task. Database is full or invalid IDs.\n");
                }
                break;
            }
            case 5: {
                int task_id, employee_id;
                
                printf("Enter task ID: ");
                scanf("%d", &task_id);
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                
                assign_task_to_employee(&db, task_id, employee_id);
                break;
            }
            case 6: {
                int task_id, hours_worked;
                char status[20];
                
                printf("Enter task ID: ");
                scanf("%d", &task_id);
                printf("Enter new status (Not Started, In Progress, Completed, Overdue): ");
                scanf(" %[^\n]", status);
                printf("Enter hours worked: ");
                scanf("%d", &hours_worked);
                
                if (strcmp(status, "Not Started") != 0 && strcmp(status, "In Progress") != 0 &&
                    strcmp(status, "Completed") != 0 && strcmp(status, "Overdue") != 0) {
                    printf("Invalid status. Must be one of: Not Started, In Progress, Completed, Overdue.\n");
                    break;
                }
                
                update_task_status(&db, task_id, status, hours_worked);
                break;
            }
            case 7: {
                int project_id;
                char status[20];
                
                printf("Enter project ID: ");
                scanf("%d", &project_id);
                printf("Enter new status (Planning, In Progress, Completed, Cancelled): ");
                scanf(" %[^\n]", status);
                
                if (strcmp(status, "Planning") != 0 && strcmp(status, "In Progress") != 0 &&
                    strcmp(status, "Completed") != 0 && strcmp(status, "Cancelled") != 0) {
                    printf("Invalid status. Must be one of: Planning, In Progress, Completed, Cancelled.\n");
                    break;
                }
                
                update_project_status(&db, project_id, status);
                break;
            }
            case 8: {
                display_all_employees(&db);
                break;
            }
            case 9: {
                display_all_projects(&db);
                break;
            }
            case 10: {
                display_all_tasks(&db);
                break;
            }
            case 11: {
                display_all_departments(&db);
                break;
            }
            case 12: {
                char name[MAX_NAME_LENGTH];
                printf("Enter employee name to search: ");
                scanf(" %[^\n]", name);
                search_employees_by_name(&db, name);
                break;
            }
            case 13: {
                char status[20];
                printf("Enter project status to search (Planning, In Progress, Completed, Cancelled): ");
                scanf(" %[^\n]", status);
                search_projects_by_status(&db, status);
                break;
            }
            case 14: {
                int employee_id;
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                search_tasks_by_employee(&db, employee_id);
                break;
            }
            case 15: {
                int employee_id;
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                generate_employee_report(&db, employee_id);
                break;
            }
            case 16: {
                int project_id;
                printf("Enter project ID: ");
                scanf("%d", &project_id);
                generate_project_report(&db, project_id);
                break;
            }
            case 17: {
                int department_id;
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                generate_department_report(&db, department_id);
                break;
            }
            case 18: {
                generate_workload_report(&db);
                break;
            }
            case 19: {
                int project_id;
                printf("Enter project ID: ");
                scanf("%d", &project_id);
                generate_project_timeline(&db, project_id);
                break;
            }
            case 20: {
                generate_task_overview(&db);
                break;
            }
            case 21: {
                int employee_id;
                double new_salary;
                
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                printf("Enter new salary: ");
                scanf("%lf", &new_salary);
                
                update_employee_salary(&db, employee_id, new_salary);
                break;
            }
            case 22: {
                int employee_id;
                char skill[50];
                
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                printf("Enter skill to add: ");
                scanf(" %[^\n]", skill);
                
                add_skill_to_employee(&db, employee_id, skill);
                break;
            }
            case 23: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 24: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 25: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 0: {
                printf("Exiting program.\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 0);
    
    return 0;
}

void initialize_database(ProjectManagementDatabase *db) {
    <｜fim▁hole｜>db->employee_count = 0;
    db->project_count = 0;
    db->task_count = 0;
    db->department_count = 0;
}

int add_employee(ProjectManagementDatabase *db, const char *first_name, const char *last_name,
                const char *email, const char *phone, const char *address,
                Date birth_date, int department_id, const char *position, double salary, int skill_level, const char *skills) {
    if (db->employee_count >= MAX_EMPLOYEES) {
        return -1;
    }
    
    // Validate department exists
    if (find_department_by_id(db, department_id) == -1) {
        return -1;
    }
    
    Employee *employee = &db->employees[db->employee_count];
    employee->id = db->employee_count + 1;
    strncpy(employee->first_name, first_name, MAX_NAME_LENGTH - 1);
    employee->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(employee->last_name, last_name, MAX_NAME_LENGTH - 1);
    employee->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(employee->email, email, 49);
    employee->email[49] = '\0';
    strncpy(employee->phone, phone, 14);
    employee->phone[14] = '\0';
    strncpy(employee->address, address, 99);
    employee->address[99] = '\0';
    employee->birth_date = birth_date;
    employee->department_id = department_id;
    strncpy(employee->position, position, MAX_NAME_LENGTH - 1);
    employee->position[MAX_NAME_LENGTH - 1] = '\0';
    employee->salary = salary;
    employee->active = 1;
    employee->skill_level = skill_level;
    strncpy(employee->skills, skills, 199);
    employee->skills[199] = '\0';
    
    // Set hire date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    employee->hire_date.day = current_time->tm_mday;
    employee->hire_date.month = current_time->tm_mon + 1;
    employee->hire_date.year = current_time->tm_year + 1900;
    
    db->employee_count++;
    return employee->id;
}

int add_project(ProjectManagementDatabase *db, const char *name, const char *description,
               Date start_date, Date end_date, int department_id, int manager_id,
               const char *status, double budget, int priority) {
    if (db->project_count >= MAX_PROJECTS) {
        return -1;
    }
    
    // Validate department and manager exist
    if (find_department_by_id(db, department_id) == -1 || find_employee_by_id(db, manager_id) == -1) {
        return -1;
    }
    
    Project *project = &db->projects[db->project_count];
    project->id = db->project_count + 1;
    strncpy(project->name, name, MAX_NAME_LENGTH - 1);
    project->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(project->description, description, 199);
    project->description[199] = '\0';
    project->start_date = start_date;
    project->end_date = end_date;
    project->department_id = department_id;
    project->manager_id = manager_id;
    strncpy(project->status, status, 19);
    project->status[19] = '\0';
    project->budget = budget;
    project->active = 1;
    project->priority = priority;
    
    db->project_count++;
    return project->id;
}

int add_department(ProjectManagementDatabase *db, const char *name, const char *description,
                 const char *manager_name, int budget) {
    if (db->department_count >= MAX_DEPARTMENTS) {
        return -1;
    }
    
    Department *department = &db->departments[db->department_count];
    department->id = db->department_count + 1;
    strncpy(department->name, name, 49);
    department->name[49] = '\0';
    strncpy(department->description, description, 199);
    department->description[199] = '\0';
    strncpy(department->manager_name, manager_name, MAX_NAME_LENGTH - 1);
    department->manager_name[MAX_NAME_LENGTH - 1] = '\0';
    department->budget = budget;
    department->active = 1;
    
    db->department_count++;
    return department->id;
}

int add_task(ProjectManagementDatabase *db, int project_id, int employee_id, const char *title,
            const char *description, Date due_date, int hours_estimated, int priority) {
    if (db->task_count >= MAX_TASKS) {
        return -1;
    }
    
    // Validate project and employee exist
    if (find_project_by_id(db, project_id) == -1 || find_employee_by_id(db, employee_id) == -1) {
        return -1;
    }
    
    Task *task = &db->tasks[db->task_count];
    task->id = db->task_count + 1;
    task->project_id = project_id;
    task->employee_id = employee_id;
    strncpy(task->title, title, MAX_NAME_LENGTH - 1);
    task->title[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(task->description, description, 199);
    task->description[199] = '\0';
    task->due_date = due_date;
    task->hours_worked = 0;
    task->hours_estimated = hours_estimated;
    strcpy(task->status, "Not Started");
    task->priority = priority;
    task->active = 1;
    
    db->task_count++;
    return task->id;
}

void assign_task_to_employee(ProjectManagementDatabase *db, int task_id, int employee_id) {
    int task_index = find_task_by_id(db, task_id);
    int employee_index = find_employee_by_id(db, employee_id);
    
    if (task_index == -1) {
        printf("Task not found.\n");
        return;
    }
    
    if (employee_index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    db->tasks[task_index].employee_id = employee_id;
    
    printf("Task #%d assigned to employee %s %s.\n", task_id,
           db->employees[employee_index].first_name,
           db->employees[employee_index].last_name);
}

void update_task_status(ProjectManagementDatabase *db, int task_id, const char *status, int hours_worked) {
    int task_index = find_task_by_id(db, task_id);
    
    if (task_index == -1) {
        printf("Task not found.\n");
        return;
    }
    
    Task *task = &db->tasks[task_index];
    
    strcpy(task->status, status);
    task->hours_worked += hours_worked;
    
    // Set completion date if task is completed
    if (strcmp(status, "Completed") == 0) {
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);
        task->completion_date.day = current_time->tm_mday;
        task->completion_date.month = current_time->tm_mon + 1;
        task->completion_date.year = current_time->tm_year + 1900;
    }
    
    printf("Task #%d status updated to '%s'.\n", task_id, status);
    printf("Total hours worked: %d\n", task->hours_worked);
    printf("Estimated hours: %d\n", task->hours_estimated);
    
    if (task->hours_worked > task->hours_estimated) {
        printf("Note: Task has exceeded estimated hours.\n");
    }
}

void update_project_status(ProjectManagementDatabase *db, int project_id, const char *status) {
    int project_index = find_project_by_id(db, project_id);
    
    if (project_index == -1) {
        printf("Project not found.\n");
        return;
    }
    
    Project *project = &db->projects[project_index];
    
    strcpy(project->status, status);
    
    printf("Project #%d status updated to '%s'.\n", project_id, status);
    printf("Description: %s\n", get_project_status_description(status));
}

void display_employee(const Employee *employee, const ProjectManagementDatabase *db) {
    printf("\nEmployee ID: %d\n", employee->id);
    printf("Name: %s %s\n", employee->first_name, employee->last_name);
    printf("Email: %s\n", employee->email);
    printf("Phone: %s\n", employee->phone);
    printf("Address: %s\n", employee->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           employee->birth_date.day, employee->birth_date.month, employee->birth_date.year);
    printf("Hire Date: %02d/%02d/%d\n", 
           employee->hire_date.day, employee->hire_date.month, employee->hire_date.year);
    
    // Display department name
    int department_index = find_department_by_id(db, employee->department_id);
    if (department_index != -1) {
        printf("Department: %s\n", db->departments[department_index].name);
    }
    
    printf("Position: %s\n", employee->position);
    printf("Salary: $%.2f\n", employee->salary);
    printf("Skill Level: %d\n", employee->skill_level);
    printf("Skills: %s\n", employee->skills);
    printf("Status: %s\n", employee->active ? "Active" : "Inactive");
}

void display_project(const Project *project, const ProjectManagementDatabase *db) {
    printf("\nProject ID: %d\n", project->id);
    printf("Name: %s\n", project->name);
    printf("Description: %s\n", project->description);
    printf("Start Date: %02d/%02d/%d\n", 
           project->start_date.day, project->start_date.month, project->start_date.year);
    printf("End Date: %02d/%02d/%d\n", 
           project->end_date.day, project->end_date.month, project->end_date.year);
    
    // Display department name
    int department_index = find_department_by_id(db, project->department_id);
    if (department_index != -1) {
        printf("Department: %s\n", db->departments[department_index].name);
    }
    
    // Display manager name
    int manager_index = find_employee_by_id(db, project->manager_id);
    if (manager_index != -1) {
        printf("Manager: %s %s\n", 
               db->employees[manager_index].first_name,
               db->employees[manager_index].last_name);
    }
    
    printf("Status: %s\n", project->status);
    printf("Description: %s\n", get_project_status_description(project->status));
    printf("Budget: $%.2f\n", project->budget);
    printf("Priority: %d\n", project->priority);
    printf("Status: %s\n", project->active ? "Active" : "Inactive");
}

void display_task(const Task *task, const ProjectManagementDatabase *db) {
    printf("\nTask ID: %d\n", task->id);
    printf("Title: %s\n", task->title);
    printf("Description: %s\n", task->description);
    printf("Due Date: %02d/%02d/%d\n", 
           task->due_date.day, task->due_date.month, task->due_date.year);
    
    if (strcmp(task->status, "Completed") == 0) {
        printf("Completion Date: %02d/%02d/%d\n", 
               task->completion_date.day, task->completion_date.month, task->completion_date.year);
    }
    
    // Display project name
    int project_index = find_project_by_id(db, task->project_id);
    if (project_index != -1) {
        printf("Project: %s\n", db->projects[project_index].name);
    }
    
    // Display employee name
    int employee_index = find_employee_by_id(db, task->employee_id);
    if (employee_index != -1) {
        printf("Assigned to: %s %s\n", 
               db->employees[employee_index].first_name,
               db->employees[employee_index].last_name);
    }
    
    printf("Hours Worked: %d\n", task->hours_worked);
    printf("Hours Estimated: %d\n", task->hours_estimated);
    printf("Status: %s\n", task->status);
    printf("Description: %s\n", get_task_status_description(task->status));
    printf("Priority: %d\n", task->priority);
    printf("Status: %s\n", task->active ? "Active" : "Inactive");
}

void display_department(const Department *department) {
    printf("\nDepartment ID: %d\n", department->id);
    printf("Name: %s\n", department->name);
    printf("Description: %s\n", department->description);
    printf("Manager: %s\n", department->manager_name);
    printf("Budget: $%d\n", department->budget);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
}

void display_all_employees(const ProjectManagementDatabase *db) {
    printf("\n=== All Employees ===\n");
    printf("Total Employees: %d\n\n", db->employee_count);
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            display_employee(&db->employees[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_projects(const ProjectManagementDatabase *db) {
    printf("\n=== All Projects ===\n");
    printf("Total Projects: %d\n\n", db->project_count);
    
    for (int i = 0; i < db->project_count; i++) {
        if (db->projects[i].active) {
            display_project(&db->projects[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_tasks(const ProjectManagementDatabase *db) {
    printf("\n=== All Tasks ===\n");
    printf("Total Tasks: %d\n\n", db->task_count);
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active) {
            display_task(&db->tasks[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_departments(const ProjectManagementDatabase *db) {
    printf("\n=== All Departments ===\n");
    printf("Total Departments: %d\n\n", db->department_count);
    
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].active) {
            display_department(&db->departments[i]);
            printf("------------------------\n");
        }
    }
}

void search_employees_by_name(const ProjectManagementDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active && 
            (strstr(db->employees[i].first_name, name) != NULL || 
             strstr(db->employees[i].last_name, name) != NULL)) {
            display_employee(&db->employees[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No employees found with name containing '%s'.\n", name);
    }
}

void search_projects_by_status(const ProjectManagementDatabase *db, const char *status) {
    printf("\nProjects with Status: %s\n", status);
    int found = 0;
    
    for (int i = 0; i < db->project_count; i++) {
        if (db->projects[i].active && strcmp(db->projects[i].status, status) == 0) {
            display_project(&db->projects[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No projects found with this status.\n");
    }
}

void search_tasks_by_employee(const ProjectManagementDatabase *db, int employee_id) {
    int employee_index = find_employee_by_id(db, employee_id);
    if (employee_index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    printf("\nTasks for Employee: %s %s (ID: %d)\n", 
           db->employees[employee_index].first_name,
           db->employees[employee_index].last_name,
           employee_id);
    
    int found = 0;
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].employee_id == employee_id) {
            display_task(&db->tasks[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No tasks found for this employee.\n");
    }
}

void generate_employee_report(const ProjectManagementDatabase *db, int employee_id) {
    int employee_index = find_employee_by_id(db, employee_id);
    if (employee_index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    Employee *employee = &db->employees[employee_index];
    
    printf("\n=== Employee Report: %s %s ===\n", employee->first_name, employee->last_name);
    printf("Employee ID: %d\n", employee->id);
    printf("Email: %s\n", employee->email);
    printf("Phone: %s\n", employee->phone);
    printf("Address: %s\n", employee->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           employee->birth_date.day, employee->birth_date.month, employee->birth_date.year);
    printf("Hire Date: %02d/%02d/%d\n", 
           employee->hire_date.day, employee->hire_date.month, employee->hire_date.year);
    
    // Display department name
    int department_index = find_department_by_id(db, employee->department_id);
    if (department_index != -1) {
        printf("Department: %s\n", db->departments[department_index].name);
    }
    
    printf("Position: %s\n", employee->position);
    printf("Salary: $%.2f\n", employee->salary);
    printf("Skill Level: %d\n", employee->skill_level);
    printf("Skills: %s\n", employee->skills);
    printf("Status: %s\n", employee->active ? "Active" : "Inactive");
    
    printf("\nTask Summary:\n");
    printf("%-5s %-15s %-10s %-10s %-10s %-10s\n", 
           "ID", "Title", "Status", "Hours", "Est.", "Progress");
    printf("---------------------------------------------------------\n");
    
    int total_tasks = 0;
    int completed_tasks = 0;
    int total_hours_worked = 0;
    int total_hours_estimated = 0;
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].employee_id == employee_id) {
            int progress = 0;
            if (db->tasks[i].hours_estimated > 0) {
                progress = (int)((float)db->tasks[i].hours_worked / db->tasks[i].hours_estimated * 100);
                if (progress > 100) progress = 100;
            }
            
            printf("%-5d %-15s %-10s %-10d %-10d %d%%\n", 
                   db->tasks[i].id,
                   db->tasks[i].title,
                   db->tasks[i].status,
                   db->tasks[i].hours_worked,
                   db->tasks[i].hours_estimated,
                   progress);
            
            total_tasks++;
            total_hours_worked += db->tasks[i].hours_worked;
            total_hours_estimated += db->tasks[i].hours_estimated;
            
            if (strcmp(db->tasks[i].status, "Completed") == 0) {
                completed_tasks++;
            }
        }
    }
    
    printf("---------------------------------------------------------\n");
    printf("Total Tasks: %d\n", total_tasks);
    printf("Completed Tasks: %d (%.1f%%)\n", 
           total_tasks > 0 ? (float)completed_tasks / total_tasks * 100 : 0.0f);
    printf("Total Hours Worked: %d\n", total_hours_worked);
    printf("Total Hours Estimated: %d\n", total_hours_estimated);
    printf("Overall Progress: %.1f%%\n", 
           total_hours_estimated > 0 ? (float)total_hours_worked / total_hours_estimated * 100 : 0.0f);
    
    // Project summary
    printf("\nProject Summary:\n");
    
    int project_ids[MAX_PROJECTS];
    int project_count = 0;
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].employee_id == employee_id) {
            int project_id = db->tasks[i].project_id;
            
            // Check if project is already in the list
            int found = 0;
            for (int j = 0; j < project_count; j++) {
                if (project_ids[j] == project_id) {
                    found = 1;
                    break;
                }
            }
            
            if (!found && project_count < MAX_PROJECTS) {
                project_ids[project_count++] = project_id;
            }
        }
    }
    
    for (int i = 0; i < project_count; i++) {
        int project_index = find_project_by_id(db, project_ids[i]);
        if (project_index != -1) {
            printf("- %s (Status: %s)\n", 
                   db->projects[project_index].name,
                   db->projects[project_index].status);
        }
    }
}

void generate_project_report(const ProjectManagementDatabase *db, int project_id) {
    int project_index = find_project_by_id(db, project_id);
    if (project_index == -1) {
        printf("Project not found.\n");
        return;
    }
    
    Project *project = &db->projects[project_index];
    
    printf("\n=== Project Report: %s ===\n", project->name);
    printf("Project ID: %d\n", project->id);
    printf("Description: %s\n", project->description);
    printf("Start Date: %02d/%02d/%d\n", 
           project->start_date.day, project->start_date.month, project->start_date.year);
    printf("End Date: %02d/%02d/%d\n", 
           project->end_date.day, project->end_date.month, project->end_date.year);
    
    // Display department name
    int department_index = find_department_by_id(db, project->department_id);
    if (department_index != -1) {
        printf("Department: %s\n", db->departments[department_index].name);
    }
    
    // Display manager name
    int manager_index = find_employee_by_id(db, project->manager_id);
    if (manager_index != -1) {
        printf("Manager: %s %s\n", 
               db->employees[manager_index].first_name,
               db->employees[manager_index].last_name);
    }
    
    printf("Status: %s\n", project->status);
    printf("Description: %s\n", get_project_status_description(project->status));
    printf("Budget: $%.2f\n", project->budget);
    printf("Priority: %d\n", project->priority);
    printf("Status: %s\n", project->active ? "Active" : "Inactive");
    
    printf("\nTask Summary:\n");
    printf("%-5s %-15s %-15s %-10s %-10s %-10s\n", 
           "ID", "Title", "Assigned To", "Status", "Hours", "Est.");
    printf("------------------------------------------------------------\n");
    
    int total_tasks = 0;
    int completed_tasks = 0;
    int total_hours_worked = 0;
    int total_hours_estimated = 0;
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].project_id == project_id) {
            // Get employee name
            int employee_index = find_employee_by_id(db, db->tasks[i].employee_id);
            char employee_name[MAX_NAME_LENGTH * 2];
            if (employee_index != -1) {
                sprintf(employee_name, "%s %s", 
                       db->employees[employee_index].first_name,
                       db->employees[employee_index].last_name);
            } else {
                strcpy(employee_name, "Unassigned");
            }
            
            printf("%-5d %-15s %-15s %-10s %-10d %-10d\n", 
                   db->tasks[i].id,
                   db->tasks[i].title,
                   employee_name,
                   db->tasks[i].status,
                   db->tasks[i].hours_worked,
                   db->tasks[i].hours_estimated);
            
            total_tasks++;
            total_hours_worked += db->tasks[i].hours_worked;
            total_hours_estimated += db->tasks[i].hours_estimated;
            
            if (strcmp(db->tasks[i].status, "Completed") == 0) {
                completed_tasks++;
            }
        }
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total Tasks: %d\n", total_tasks);
    printf("Completed Tasks: %d (%.1f%%)\n", 
           total_tasks > 0 ? (float)completed_tasks / total_tasks * 100 : 0.0f);
    printf("Total Hours Worked: %d\n", total_hours_worked);
    printf("Total Hours Estimated: %d\n", total_hours_estimated);
    printf("Overall Progress: %.1f%%\n", 
           total_hours_estimated > 0 ? (float)total_hours_worked / total_hours_estimated * 100 : 0.0f);
    
    // Team members
    printf("\nTeam Members:\n");
    
    int employee_ids[MAX_EMPLOYEES];
    int employee_count = 0;
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].project_id == project_id) {
            int employee_id = db->tasks[i].employee_id;
            
            // Check if employee is already in the list
            int found = 0;
            for (int j = 0; j < employee_count; j++) {
                if (employee_ids[j] == employee_id) {
                    found = 1;
                    break;
                }
            }
            
            if (!found && employee_count < MAX_EMPLOYEES) {
                employee_ids[employee_count++] = employee_id;
            }
        }
    }
    
    for (int i = 0; i < employee_count; i++) {
        int employee_index = find_employee_by_id(db, employee_ids[i]);
        if (employee_index != -1) {
            printf("- %s %s (%s)\n", 
                   db->employees[employee_index].first_name,
                   db->employees[employee_index].last_name,
                   db->employees[employee_index].position);
        }
    }
}

void generate_department_report(const ProjectManagementDatabase *db, int department_id) {
    int department_index = find_department_by_id(db, department_id);
    if (department_index == -1) {
        printf("Department not found.\n");
        return;
    }
    
    Department *department = &db->departments[department_index];
    
    printf("\n=== Department Report: %s ===\n", department->name);
    printf("Department ID: %d\n", department->id);
    printf("Description: %s\n", department->description);
    printf("Manager: %s\n", department->manager_name);
    printf("Budget: $%d\n", department->budget);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
    
    printf("\nEmployee Summary:\n");
    printf("%-5s %-20s %-15s %-10s %-10s\n", 
           "ID", "Name", "Position", "Salary", "Skill Lvl");
    printf("------------------------------------------------\n");
    
    int total_employees = 0;
    double total_salary = 0.0;
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active && db->employees[i].department_id == department_id) {
            printf("%-5d %-20s %-15s $%-9.2f %-10d\n", 
                   db->employees[i].id,
                   db->employees[i].first_name,
                   db->employees[i].position,
                   db->employees[i].salary,
                   db->employees[i].skill_level);
            
            total_employees++;
            total_salary += db->employees[i].salary;
        }
    }
    
    printf("------------------------------------------------\n");
    printf("Total Employees: %d\n", total_employees);
    printf("Total Salary: $%.2f\n", total_salary);
    printf("Average Salary: $%.2f\n", total_employees > 0 ? total_salary / total_employees : 0.0);
    
    printf("\nProject Summary:\n");
    
    int total_projects = 0;
    int completed_projects = 0;
    int in_progress_projects = 0;
    int planning_projects = 0;
    
    for (int i = 0; i < db->project_count; i++) {
        if (db->projects[i].active && db->projects[i].department_id == department_id) {
            total_projects++;
            
            if (strcmp(db->projects[i].status, "Completed") == 0) {
                completed_projects++;
            } else if (strcmp(db->projects[i].status, "In Progress") == 0) {
                in_progress_projects++;
            } else if (strcmp(db->projects[i].status, "Planning") == 0) {
                planning_projects++;
            }
        }
    }
    
    printf("Total Projects: %d\n", total_projects);
    printf("Completed Projects: %d (%.1f%%)\n", 
           total_projects > 0 ? (float)completed_projects / total_projects * 100 : 0.0f);
    printf("In Progress Projects: %d (%.1f%%)\n", 
           total_projects > 0 ? (float)in_progress_projects / total_projects * 100 : 0.0f);
    printf("Planning Projects: %d (%.1f%%)\n", 
           total_projects > 0 ? (float)planning_projects / total_projects * 100 : 0.0f);
    
    printf("\nBudget vs. Salary:\n");
    printf("Department Budget: $%d\n", department->budget);
    printf("Total Salary Expense: $%.2f\n", total_salary);
    printf("Budget Remaining: $%.2f\n", department->budget - total_salary);
    printf("Salary as Percentage of Budget: %.1f%%\n", 
           department->budget > 0 ? (float)total_salary / department->budget * 100 : 0.0f);
}

void generate_workload_report(const ProjectManagementDatabase *db) {
    printf("\n=== Employee Workload Report ===\n");
    printf("%-5s %-20s %-15s %-10s %-10s %-10s\n", 
           "ID", "Name", "Department", "Tasks", "Hours", "Est.");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            int task_count = 0;
            int total_hours_worked = 0;
            int total_hours_estimated = 0;
            
            for (int j = 0; j < db->task_count; j++) {
                if (db->tasks[j].active && db->tasks[j].employee_id == db->employees[i].id) {
                    task_count++;
                    total_hours_worked += db->tasks[j].hours_worked;
                    total_hours_estimated += db->tasks[j].hours_estimated;
                }
            }
            
            // Get department name
            int department_index = find_department_by_id(db, db->employees[i].department_id);
            char department_name[50];
            if (department_index != -1) {
                strcpy(department_name, db->departments[department_index].name);
            } else {
                strcpy(department_name, "Unknown");
            }
            
            printf("%-5d %-20s %-15s %-10d %-10d %-10d\n", 
                   db->employees[i].id,
                   db->employees[i].first_name,
                   department_name,
                   task_count,
                   total_hours_worked,
                   total_hours_estimated);
        }
    }
    
    printf("-----------------------------------------------------------\n");
}

void generate_project_timeline(const ProjectManagementDatabase *db, int project_id) {
    int project_index = find_project_by_id(db, project_id);
    if (project_index == -1) {
        printf("Project not found.\n");
        return;
    }
    
    Project *project = &db->projects[project_index];
    
    printf("\n=== Project Timeline: %s ===\n", project->name);
    printf("Project ID: %d\n", project->id);
    printf("Start Date: %02d/%02d/%d\n", 
           project->start_date.day, project->start_date.month, project->start_date.year);
    printf("End Date: %02d/%02d/%d\n", 
           project->end_date.day, project->end_date.month, project->end_date.year);
    
    printf("\nTask Timeline:\n");
    printf("%-5s %-15s %-12s %-12s %-10s %-10s\n", 
           "ID", "Title", "Due Date", "Comp. Date", "Status", "Priority");
    printf("-----------------------------------------------------------\n");
    
    // Create array to store tasks
    typedef struct {
        Task task;
        int original_index;
    } TaskWithIndex;
    
    TaskWithIndex project_tasks[MAX_TASKS];
    int task_count = 0;
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].project_id == project_id) {
            project_tasks[task_count].task = db->tasks[i];
            project_tasks[task_count].original_index = i;
            task_count++;
        }
    }
    
    // Sort tasks by due date
    for (int i = 0; i < task_count - 1; i++) {
        for (int j = 0; j < task_count - i - 1; j++) {
            int date1 = project_tasks[j].task.due_date.year * 10000 + 
                       project_tasks[j].task.due_date.month * 100 + 
                       project_tasks[j].task.due_date.day;
            
            int date2 = project_tasks[j + 1].task.due_date.year * 10000 + 
                       project_tasks[j + 1].task.due_date.month * 100 + 
                       project_tasks[j + 1].task.due_date.day;
            
            if (date1 > date2) {
                TaskWithIndex temp = project_tasks[j];
                project_tasks[j] = project_tasks[j + 1];
                project_tasks[j + 1] = temp;
            }
        }
    }
    
    // Display sorted tasks
    for (int i = 0; i < task_count; i++) {
        Task *task = &project_tasks[i].task;
        
        printf("%-5d %-15s %02d/%02d/%d ", 
               task->id,
               task->title,
               task->due_date.day, task->due_date.month, task->due_date.year);
        
        if (strcmp(task->status, "Completed") == 0) {
            printf("%02d/%02d/%d ", 
                   task->completion_date.day, task->completion_date.month, task->completion_date.year);
        } else {
            printf("%-12s ", "N/A");
        }
        
        printf("%-10s %-10d\n", task->status, task->priority);
    }
    
    printf("-----------------------------------------------------------\n");
    
    // Gantt chart visualization (simplified)
    printf("\nGantt Chart (Simplified):\n");
    
    // Calculate project duration in days
    time_t start_time, end_time;
    struct tm start_tm = {0, 0, 0, project->start_date.day, project->start_date.month - 1, project->start_date.year - 1900};
    struct tm end_tm = {0, 0, 0, project->end_date.day, project->end_date.month - 1, project->end_date.year - 1900};
    
    start_time = mktime(&start_tm);
    end_time = mktime(&end_tm);
    
    int project_duration = (int)((end_time - start_time) / (60 * 60 * 24));
    
    printf("Project Duration: %d days\n\n", project_duration);
    
    // Display tasks on timeline
    for (int i = 0; i < task_count && i < 10; i++) { // Limit to first 10 tasks for readability
        Task *task = &project_tasks[i].task;
        
        // Calculate task position in timeline (simplified)
        time_t task_due_time;
        struct tm task_tm = {0, 0, 0, task->due_date.day, task->due_date.month - 1, task->due_date.year - 1900};
        task_due_time = mktime(&task_tm);
        
        int days_from_start = (int)((task_due_time - start_time) / (60 * 60 * 24));
        
        // Calculate position (max 50 characters wide)
        int position = (int)((float)days_from_start / project_duration * 50);
        if (position < 0) position = 0;
        if (position > 50) position = 50;
        
        printf("%-15s ", task->title);
        
        for (int j = 0; j < position; j++) {
            printf("-");
        }
        
        if (strcmp(task->status, "Completed") == 0) {
            printf("*");
        } else if (strcmp(task->status, "Overdue") == 0) {
            printf("X");
        } else {
            printf("o");
        }
        
        printf("\n");
    }
}

void generate_task_overview(const ProjectManagementDatabase *db) {
    printf("\n=== Task Overview ===\n");
    
    int total_tasks = 0;
    int not_started_tasks = 0;
    int in_progress_tasks = 0;
    int completed_tasks = 0;
    int overdue_tasks = 0;
    
    int total_hours_worked = 0;
    int total_hours_estimated = 0;
    
    // Get current date
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    Date current_date = {
        current_time->tm_mday,
        current_time->tm_mon + 1,
        current_time->tm_year + 1900
    };
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active) {
            total_tasks++;
            total_hours_worked += db->tasks[i].hours_worked;
            total_hours_estimated += db->tasks[i].hours_estimated;
            
            if (strcmp(db->tasks[i].status, "Not Started") == 0) {
                not_started_tasks++;
            } else if (strcmp(db->tasks[i].status, "In Progress") == 0) {
                in_progress_tasks++;
            } else if (strcmp(db->tasks[i].status, "Completed") == 0) {
                completed_tasks++;
            }
            
            // Check if overdue
            int task_timestamp = db->tasks[i].due_date.year * 10000 + 
                               db->tasks[i].due_date.month * 100 + 
                               db->tasks[i].due_date.day;
            
            int current_timestamp = current_date.year * 10000 + 
                                current_date.month * 100 + 
                                current_date.day;
            
            if (task_timestamp < current_timestamp && 
                strcmp(db->tasks[i].status, "Completed") != 0) {
                overdue_tasks++;
            }
        }
    }
    
    printf("Total Tasks: %d\n", total_tasks);
    printf("Not Started: %d (%.1f%%)\n", not_started_tasks, 
           total_tasks > 0 ? (float)not_started_tasks / total_tasks * 100 : 0.0f);
    printf("In Progress: %d (%.1f%%)\n", in_progress_tasks, 
           total_tasks > 0 ? (float)in_progress_tasks / total_tasks * 100 : 0.0f);
    printf("Completed: %d (%.1f%%)\n", completed_tasks, 
           total_tasks > 0 ? (float)completed_tasks / total_tasks * 100 : 0.0f);
    printf("Overdue: %d (%.1f%%)\n", overdue_tasks, 
           total_tasks > 0 ? (float)overdue_tasks / total_tasks * 100 : 0.0f);
    
    printf("\nHours Summary:\n");
    printf("Total Hours Worked: %d\n", total_hours_worked);
    printf("Total Hours Estimated: %d\n", total_hours_estimated);
    printf("Overall Progress: %.1f%%\n", 
           total_hours_estimated > 0 ? (float)total_hours_worked / total_hours_estimated * 100 : 0.0f);
    
    // Priority breakdown
    printf("\nPriority Breakdown:\n");
    printf("%-10s %-10s %-10s %-10s %-10s\n", 
           "Priority 1", "Priority 2", "Priority 3", "Priority 4", "Priority 5");
    printf("---------------------------------------------------------\n");
    
    int priority_counts[6] = {0}; // 0-5, index 0 unused
    
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].active && db->tasks[i].priority >= 1 && db->tasks[i].priority <= 5) {
            priority_counts[db->tasks[i].priority]++;
        }
    }
    
    for (int i = 1; i <= 5; i++) {
        printf("%-10d ", priority_counts[i]);
    }
    
    printf("\n");
}

int find_employee_by_id(const ProjectManagementDatabase *db, int employee_id) {
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].id == employee_id && db->employees[i].active) {
            return i;
        }
    }
    return -1;
}

int find_project_by_id(const ProjectManagementDatabase *db, int project_id) {
    for (int i = 0; i < db->project_count; i++) {
        if (db->projects[i].id == project_id && db->projects[i].active) {
            return i;
        }
    }
    return -1;
}

int find_task_by_id(const ProjectManagementDatabase *db, int task_id) {
    for (int i = 0; i < db->task_count; i++) {
        if (db->tasks[i].id == task_id && db->tasks[i].active) {
            return i;
        }
    }
    return -1;
}

int find_department_by_id(const ProjectManagementDatabase *db, int department_id) {
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].id == department_id && db->departments[i].active) {
            return i;
        }
    }
    return -1;
}

int save_database(const ProjectManagementDatabase *db) {
    FILE *employees_file = fopen(FILENAME_EMPLOYEES, "wb");
    if (employees_file != NULL) {
        fwrite(&db->employee_count, sizeof(int), 1, employees_file);
        fwrite(db->employees, sizeof(Employee), db->employee_count, employees_file);
        fclose(employees_file);
    } else {
        return 0;
    }
    
    FILE *projects_file = fopen(FILENAME_PROJECTS, "wb");
    if (projects_file != NULL) {
        fwrite(&db->project_count, sizeof(int), 1, projects_file);
        fwrite(db->projects, sizeof(Project), db->project_count, projects_file);
        fclose(projects_file);
    } else {
        return 0;
    }
    
    FILE *tasks_file = fopen(FILENAME_TASKS, "wb");
    if (tasks_file != NULL) {
        fwrite(&db->task_count, sizeof(int), 1, tasks_file);
        fwrite(db->tasks, sizeof(Task), db->task_count, tasks_file);
        fclose(tasks_file);
    } else {
        return 0;
    }
    
    FILE *departments_file = fopen(FILENAME_DEPARTMENTS, "wb");
    if (departments_file != NULL) {
        fwrite(&db->department_count, sizeof(int), 1, departments_file);
        fwrite(db->departments, sizeof(Department), db->department_count, departments_file);
        fclose(departments_file);
        return 1;
    }
    
    return 0;
}

int load_database(ProjectManagementDatabase *db) {
    FILE *employees_file = fopen(FILENAME_EMPLOYEES, "rb");
    if (employees_file != NULL) {
        fread(&db->employee_count, sizeof(int), 1, employees_file);
        fread(db->employees, sizeof(Employee), db->employee_count, employees_file);
        fclose(employees_file);
    } else {
        return 0;
    }
    
    FILE *projects_file = fopen(FILENAME_PROJECTS, "rb");
    if (projects_file != NULL) {
        fread(&db->project_count, sizeof(int), 1, projects_file);
        fread(db->projects, sizeof(Project), db->project_count, projects_file);
        fclose(projects_file);
    } else {
        return 0;
    }
    
    FILE *tasks_file = fopen(FILENAME_TASKS, "rb");
    if (tasks_file != NULL) {
        fread(&db->task_count, sizeof(int), 1, tasks_file);
        fread(db->tasks, sizeof(Task), db->task_count, tasks_file);
        fclose(tasks_file);
    } else {
        return 0;
    }
    
    FILE *departments_file = fopen(FILENAME_DEPARTMENTS, "rb");
    if (departments_file != NULL) {
        fread(&db->department_count, sizeof(int), 1, departments_file);
        fread(db->departments, sizeof(Department), db->department_count, departments_file);
        fclose(departments_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(ProjectManagementDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* department_names[] = {"Engineering", "Marketing", "Sales", "HR", "Finance",
                               "Operations", "Customer Support", "Product", "Design", "IT"};
    
    char* manager_names[] = {"Dr. Smith", "Prof. Johnson", "Dr. Williams", "Prof. Brown",
                           "Dr. Jones", "Prof. Garcia", "Dr. Miller", "Prof. Davis",
                           "Dr. Rodriguez", "Prof. Martinez"};
    
    char* project_names[] = {"Website Redesign", "Mobile App Development", "Database Migration",
                            "Marketing Campaign", "Product Launch", "System Upgrade",
                            "Customer Portal", "Analytics Dashboard", "Security Audit",
                            "API Development"};
    
    char* task_titles[] = {"Design UI", "Implement Backend", "Write Documentation",
                          "Test Feature", "Review Code", "Create Mockups",
                          "Deploy System", "Analyze Data", "Fix Bug", "Optimize Performance"};
    
    char* skills[] = {"Programming", "Design", "Management", "Marketing", "Sales",
                     "Communication", "Analytics", "Leadership", "Problem Solving", "Teamwork"};
    
    srand(time(NULL));
    
    // Add departments
    for (int i = 0; i < 10 && db->department_count < MAX_DEPARTMENTS; i++) {
        int budget = 50000 + (rand() % 500000);
        add_department(db, department_names[i], "Department responsible for key business functions", 
                     manager_names[i], budget);
    }
    
    // Add employees
    for (int i = 0; i < 200 && db->employee_count < MAX_EMPLOYEES; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[100], position[MAX_NAME_LENGTH], employee_skills[200];
        sprintf(email, "%s.%d@company.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        sprintf(position, "%s Specialist", department_names[rand() % 10]);
        
        // Build skills string
        strcpy(employee_skills, skills[rand() % 10]);
        for (int j = 0; j < 3; j++) { // Add 3 more skills
            sprintf(employee_skills + strlen(employee_skills), ", %s", skills[rand() % 10]);
        }
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1970 + (rand() % 40);
        Date birth_date = {day, month, year};
        
        int department_id = 1 + (rand() % 10);
        double salary = 40000.0 + (rand() % 80000) + (rand() % 100) / 100.0;
        int skill_level = 1 + (rand() % 5);
        
        add_employee(db, first_name, last_name, email, phone, address, birth_date,
                   department_id, position, salary, skill_level, employee_skills);
    }
    
    // Add projects
    for (int i = 0; i < 50 && db->project_count < MAX_PROJECTS; i++) {
        char description[200];
        sprintf(description, "Critical project for %s with strategic importance", department_names[rand() % 10]);
        
        int start_day = 1 + (rand() % 28);
        int start_month = 1 + (rand() % 12);
        int start_year = 2023 + (rand() % 3);
        Date start_date = {start_day, start_month, start_year};
        
        int duration = 30 + (rand() % 180); // 1-6 months
        time_t start_time = 0;
        struct tm start_tm = {0, 0, 0, start_day, start_month - 1, start_year - 1900};
        start_time = mktime(&start_tm);
        time_t end_time = start_time + (duration * 24 * 60 * 60);
        struct tm *end_tm = localtime(&end_time);
        Date end_date = {end_tm->tm_mday, end_tm->tm_mon + 1, end_tm->tm_year + 1900};
        
        int department_id = 1 + (rand() % 10);
        int manager_id = 1 + (rand() % 200);
        
        const char* statuses[] = {"Planning", "In Progress", "Completed", "Cancelled"};
        char status[20];
        strcpy(status, statuses[rand() % 4]);
        
        double budget = 10000.0 + (rand() % 100000) + (rand() % 100) / 100.0;
        int priority = 1 + (rand() % 5);
        
        add_project(db, project_names[rand() % 10], description, start_date, end_date,
                  department_id, manager_id, status, budget, priority);
    }
    
    // Add tasks
    for (int i = 0; i < 500 && db->task_count < MAX_TASKS; i++) {
        char description[200];
        sprintf(description, "Detailed task for implementing %s with high priority", task_titles[rand() % 10]);
        
        int project_id = 1 + (rand() % 50);
        int employee_id = 1 + (rand() % 200);
        
        int due_day = 1 + (rand() % 28);
        int due_month = 1 + (rand() % 12);
        int due_year = 2023 + (rand() % 3);
        Date due_date = {due_day, due_month, due_year};
        
        int hours_estimated = 4 + (rand() % 40); // 4-44 hours
        int priority = 1 + (rand() % 5);
        
        add_task(db, project_id, employee_id, task_titles[rand() % 10], description,
                due_date, hours_estimated, priority);
    }
    
    // Update some tasks with random statuses and hours worked
    for (int i = 0; i < 200; i++) {
        int task_index = rand() % db->task_count;
        if (db->tasks[task_index].active) {
            const char* statuses[] = {"Not Started", "In Progress", "Completed", "Overdue"};
            int status_index = rand() % 4;
            
            int hours_worked = 0;
            if (status_index >= 1) {
                hours_worked = db->tasks[task_index].hours_estimated * (rand() % 100) / 100;
            }
            
            update_task_status(db, db->tasks[task_index].id, statuses[status_index], hours_worked);
        }
    }
}

void update_employee_salary(ProjectManagementDatabase *db, int employee_id, double new_salary) {
    int employee_index = find_employee_by_id(db, employee_id);
    if (employee_index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    Employee *employee = &db->employees[employee_index];
    
    double old_salary = employee->salary;
    employee->salary = new_salary;
    
    printf("Salary updated for employee %s %s\n", 
           employee->first_name, employee->last_name);
    printf("Previous Salary: $%.2f\n", old_salary);
    printf("New Salary: $%.2f\n", new_salary);
    
    if (new_salary > old_salary) {
        printf("Increase: $%.2f (%.1f%%)\n", 
               new_salary - old_salary, 
               (new_salary - old_salary) / old_salary * 100);
    } else if (new_salary < old_salary) {
        printf("Decrease: $%.2f (%.1f%%)\n", 
               old_salary - new_salary, 
               (old_salary - new_salary) / old_salary * 100);
    } else {
        printf("No change in salary.\n");
    }
}

void add_skill_to_employee(ProjectManagementDatabase *db, int employee_id, const char *skill) {
    int employee_index = find_employee_by_id(db, employee_id);
    if (employee_index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    Employee *employee = &db->employees[employee_index];
    
    // Check if skill already exists
    if (strstr(employee->skills, skill) != NULL) {
        printf("Employee already has this skill.\n");
        return;
    }
    
    // Add skill to skills string
    if (strlen(employee->skills) > 0) {
        strcat(employee->skills, ", ");
    }
    
    strncat(employee->skills, skill, 200 - strlen(employee->skills) - 1);
    
    printf("Skill added to employee %s %s\n", 
           employee->first_name, employee->last_name);
    printf("New Skills: %s\n", employee->skills);
}

int is_valid_email(const char *email) {
    if (email == NULL) return 0;
    
    int at_symbol = 0;
    int dot_after_at = 0;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_symbol = 1;
        } else if (at_symbol && email[i] == '.') {
            dot_after_at = 1;
        }
    }
    
    return at_symbol && dot_after_at;
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int max_days;
    switch (month) {
        case 2:
            // Leap year check
            max_days = ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            max_days = 30;
            break;
        default:
            max_days = 31;
    }
    
    return day >= 1 && day <= max_days;
}

const char* get_project_status_description(const char *status) {
    if (strcmp(status, "Planning") == 0) return "Project is in planning phase";
    if (strcmp(status, "In Progress") == 0) return "Project is currently being worked on";
    if (strcmp(status, "Completed") == 0) return "Project has been completed successfully";
    if (strcmp(status, "Cancelled") == 0) return "Project has been cancelled";
    return "Unknown status";
}

const char* get_task_status_description(const char *status) {
    if (strcmp(status, "Not Started") == 0) return "Task has not been started yet";
    if (strcmp(status, "In Progress") == 0) return "Task is currently being worked on";
    if (strcmp(status, "Completed") == 0) return "Task has been completed successfully";
    if (strcmp(status, "Overdue") == 0) return "Task is past its due date and not completed";
    return "Unknown status";
}