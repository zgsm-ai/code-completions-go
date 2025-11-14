#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_EMPLOYEES 500
#define MAX_DEPARTMENTS 20
#define MAX_NAME_LENGTH 50
#define MAX_POSITION_LENGTH 50
#define FILENAME "employees.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char position[MAX_POSITION_LENGTH];
    int department_id;
    Date hire_date;
    float salary;
    float bonus;
    float performance_score;
    int years_of_service;
    char email[50];
    char phone[15];
    char address[100];
    int manager_id;
    int active;
} Employee;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char description[100];
    int manager_id;
    float budget;
    int employee_count;
} Department;

typedef struct {
    Employee employees[MAX_EMPLOYEES];
    int employee_count;
    Department departments[MAX_DEPARTMENTS];
    int department_count;
} HRDatabase;

// Function prototypes
void initialize_database(HRDatabase *db);
int add_employee(HRDatabase *db, const char *name, const char *position, int department_id,
                 Date hire_date, float salary, const char *email, const char *phone, 
                 const char *address, int manager_id);
int add_department(HRDatabase *db, const char *name, const char *description, 
                   int manager_id, float budget);
void update_employee_performance(HRDatabase *db, int employee_id, float score);
void calculate_bonus(HRDatabase *db, int employee_id);
void calculate_years_of_service(HRDatabase *db);
void display_employee(const Employee *employee, const HRDatabase *db);
void display_department(const Department *department, const HRDatabase *db);
void display_all_employees(const HRDatabase *db);
void display_all_departments(const HRDatabase *db);
int find_employee_by_id(const HRDatabase *db, int employee_id);
int find_department_by_id(const HRDatabase *db, int department_id);
void search_employees_by_name(const HRDatabase *db, const char *name);
void search_employees_by_department(const HRDatabase *db, int department_id);
void display_department_hierarchy(const HRDatabase *db, int department_id);
void calculate_payroll(HRDatabase *db);
void promote_employee(HRDatabase *db, int employee_id, const char *new_position, float salary_increase);
void transfer_employee(HRDatabase *db, int employee_id, int new_department_id);
void terminate_employee(HRDatabase *db, int employee_id);
int save_database(const HRDatabase *db, const char *filename);
int load_database(HRDatabase *db, const char *filename);
void generate_sample_data(HRDatabase *db);
void display_top_performers(const HRDatabase *db);
void display_salary_statistics(const HRDatabase *db);
void generate_payroll_report(const HRDatabase *db, int month, int year);
int is_valid_date(int day, int month, int year);

const char* position_levels[] = {
    "Intern", "Junior Developer", "Developer", "Senior Developer",
    "Tech Lead", "Manager", "Senior Manager", "Director", "VP", "CEO"
};

int main() {
    HRDatabase db;
    initialize_database(&db);
    
    printf("Human Resources Management System\n");
    printf("1. Add Employee\n");
    printf("2. Add Department\n");
    printf("3. Update Employee Performance\n");
    printf("4. Display All Employees\n");
    printf("5. Display All Departments\n");
    printf("6. Search Employee by Name\n");
    printf("7. Search Employees by Department\n");
    printf("8. Display Department Hierarchy\n");
    printf("9. Calculate Payroll\n");
    printf("10. Promote Employee\n");
    printf("11. Transfer Employee\n");
    printf("12. Terminate Employee\n");
    printf("13. Save Database\n");
    printf("14. Load Database\n");
    printf("15. Generate Sample Data\n");
    printf("16. Display Top Performers\n");
    printf("17. Display Salary Statistics\n");
    printf("18. Generate Payroll Report\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH], position[MAX_POSITION_LENGTH];
                char email[50], phone[15], address[100];
                int department_id, manager_id, day, month, year;
                float salary;
                
                printf("Enter employee name: ");
                scanf(" %[^\n]", name);
                printf("Enter position: ");
                scanf(" %[^\n]", position);
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                printf("Enter hire date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter salary: ");
                scanf("%f", &salary);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter manager ID (0 if no manager): ");
                scanf("%d", &manager_id);
                
                Date hire_date = {day, month, year};
                int employee_id = add_employee(&db, name, position, department_id, 
                                             hire_date, salary, email, phone, address, manager_id);
                if (employee_id != -1) {
                    printf("Employee added with ID: %d\n", employee_id);
                } else {
                    printf("Failed to add employee. Database is full.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_NAME_LENGTH], description[100];
                int manager_id;
                float budget;
                
                printf("Enter department name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter manager ID (0 if no manager): ");
                scanf("%d", &manager_id);
                printf("Enter budget: ");
                scanf("%f", &budget);
                
                int department_id = add_department(&db, name, description, manager_id, budget);
                if (department_id != -1) {
                    printf("Department added with ID: %d\n", department_id);
                } else {
                    printf("Failed to add department. Database is full.\n");
                }
                break;
            }
            case 3: {
                int employee_id;
                float score;
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                printf("Enter performance score (0-10): ");
                scanf("%f", &score);
                update_employee_performance(&db, employee_id, score);
                break;
            }
            case 4: {
                display_all_employees(&db);
                break;
            }
            case 5: {
                display_all_departments(&db);
                break;
            }
            case 6: {
                char name[MAX_NAME_LENGTH];
                printf("Enter employee name to search: ");
                scanf(" %[^\n]", name);
                search_employees_by_name(&db, name);
                break;
            }
            case 7: {
                int department_id;
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                search_employees_by_department(&db, department_id);
                break;
            }
            case 8: {
                int department_id;
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                display_department_hierarchy(&db, department_id);
                break;
            }
            case 9: {
                calculate_payroll(&db);
                break;
            }
            case 10: {
                int employee_id;
                char new_position[MAX_POSITION_LENGTH];
                float salary_increase;
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                printf("Enter new position: ");
                scanf(" %[^\n]", new_position);
                printf("Enter salary increase percentage: ");
                scanf("%f", &salary_increase);
                promote_employee(&db, employee_id, new_position, salary_increase);
                break;
            }
            case 11: {
                int employee_id, new_department_id;
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                printf("Enter new department ID: ");
                scanf("%d", &new_department_id);
                transfer_employee(&db, employee_id, new_department_id);
                break;
            }
            case 12: {
                int employee_id;
                printf("Enter employee ID: ");
                scanf("%d", &employee_id);
                terminate_employee(&db, employee_id);
                break;
            }
            case 13: {
                if (save_database(&db, FILENAME)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 14: {
                if (load_database(&db, FILENAME)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 15: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 16: {
                display_top_performers(&db);
                break;
            }
            case 17: {
                display_salary_statistics(&db);
                break;
            }
            case 18: {
                int month, year;
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_payroll_report(&db, month, year);
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

void initialize_database(HRDatabase *db) {
    <｜fim▁hole｜>db->employee_count = 0;
    db->department_count = 0;
}

int add_employee(HRDatabase *db, const char *name, const char *position, int department_id,
                 Date hire_date, float salary, const char *email, const char *phone, 
                 const char *address, int manager_id) {
    if (db->employee_count >= MAX_EMPLOYEES) {
        return -1;
    }
    
    // Validate department exists
    int dept_index = find_department_by_id(db, department_id);
    if (dept_index == -1 && department_id != 0) {
        printf("Department not found.\n");
        return -1;
    }
    
    // Validate manager exists (if specified)
    if (manager_id != 0 && find_employee_by_id(db, manager_id) == -1) {
        printf("Manager not found.\n");
        return -1;
    }
    
    Employee *employee = &db->employees[db->employee_count];
    employee->id = db->employee_count + 1;
    strncpy(employee->name, name, MAX_NAME_LENGTH - 1);
    employee->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(employee->position, position, MAX_POSITION_LENGTH - 1);
    employee->position[MAX_POSITION_LENGTH - 1] = '\0';
    employee->department_id = department_id;
    employee->hire_date = hire_date;
    employee->salary = salary;
    employee->bonus = 0.0f;
    employee->performance_score = 5.0f; // Default score
    employee->years_of_service = 0;
    strncpy(employee->email, email, 49);
    employee->email[49] = '\0';
    strncpy(employee->phone, phone, 14);
    employee->phone[14] = '\0';
    strncpy(employee->address, address, 99);
    employee->address[99] = '\0';
    employee->manager_id = manager_id;
    employee->active = 1;
    
    // Calculate years of service
    calculate_years_of_service(db);
    
    // Update department employee count
    if (department_id != 0) {
        db->departments[dept_index].employee_count++;
    }
    
    db->employee_count++;
    return employee->id;
}

int add_department(HRDatabase *db, const char *name, const char *description, 
                   int manager_id, float budget) {
    if (db->department_count >= MAX_DEPARTMENTS) {
        return -1;
    }
    
    // Validate manager exists (if specified)
    if (manager_id != 0 && find_employee_by_id(db, manager_id) == -1) {
        printf("Manager not found.\n");
        return -1;
    }
    
    Department *department = &db->departments[db->department_count];
    department->id = db->department_count + 1;
    strncpy(department->name, name, MAX_NAME_LENGTH - 1);
    department->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(department->description, description, 99);
    department->description[99] = '\0';
    department->manager_id = manager_id;
    department->budget = budget;
    department->employee_count = 0;
    
    db->department_count++;
    return department->id;
}

void update_employee_performance(HRDatabase *db, int employee_id) {
    int index = find_employee_by_id(db, employee_id);
    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    if (db->employees[index].performance_score < 0 || db->employees[index].performance_score > 10) {
        printf("Invalid performance score. Must be between 0 and 10.\n");
        return;
    }
    
    printf("Performance score updated for employee %s.\n", db->employees[index].name);
    
    // Calculate bonus based on performance
    calculate_bonus(db, employee_id);
}

void calculate_bonus(HRDatabase *db, int employee_id) {
    int index = find_employee_by_id(db, employee_id);
    if (index == -1) {
        return;
    }
    
    Employee *employee = &db->employees[index];
    
    // Bonus calculation based on performance score
    if (employee->performance_score >= 9.0) {
        employee->bonus = employee->salary * 0.20f; // 20% bonus
    } else if (employee->performance_score >= 8.0) {
        employee->bonus = employee->salary * 0.15f; // 15% bonus
    } else if (employee->performance_score >= 7.0) {
        employee->bonus = employee->salary * 0.10f; // 10% bonus
    } else if (employee->performance_score >= 6.0) {
        employee->bonus = employee->salary * 0.05f; // 5% bonus
    } else {
        employee->bonus = 0.0f; // No bonus
    }
    
    // Additional bonus based on years of service
    if (employee->years_of_service >= 10) {
        employee->bonus += employee->salary * 0.05f; // Additional 5%
    } else if (employee->years_of_service >= 5) {
        employee->bonus += employee->salary * 0.025f; // Additional 2.5%
    }
    
    printf("Bonus calculated: $%.2f\n", employee->bonus);
}

void calculate_years_of_service(HRDatabase *db) {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;
    
    for (int i = 0; i < db->employee_count; i++) {
        Employee *employee = &db->employees[i];
        
        int years = current_year - employee->hire_date.year;
        int months = current_month - employee->hire_date.month;
        int days = current_day - employee->hire_date.day;
        
        if (days < 0) {
            months--;
            days += 30; // Approximate
        }
        
        if (months < 0) {
            years--;
            months += 12;
        }
        
        employee->years_of_service = years;
    }
}

void display_employee(const Employee *employee, const HRDatabase *db) {
    printf("\nEmployee ID: %d\n", employee->id);
    printf("Name: %s\n", employee->name);
    printf("Position: %s\n", employee->position);
    printf("Department: ");
    
    int dept_index = find_department_by_id(db, employee->department_id);
    if (dept_index != -1) {
        printf("%s\n", db->departments[dept_index].name);
    } else {
        printf("N/A\n");
    }
    
    printf("Hire Date: %02d/%02d/%d\n", 
           employee->hire_date.day, employee->hire_date.month, employee->hire_date.year);
    printf("Years of Service: %d\n", employee->years_of_service);
    printf("Salary: $%.2f\n", employee->salary);
    printf("Bonus: $%.2f\n", employee->bonus);
    printf("Performance Score: %.1f/10\n", employee->performance_score);
    printf("Email: %s\n", employee->email);
    printf("Phone: %s\n", employee->phone);
    printf("Address: %s\n", employee->address);
    
    if (employee->manager_id != 0) {
        int manager_index = find_employee_by_id(db, employee->manager_id);
        if (manager_index != -1) {
            printf("Manager: %s (ID: %d)\n", 
                   db->employees[manager_index].name, employee->manager_id);
        }
    } else {
        printf("Manager: N/A\n");
    }
    
    printf("Status: %s\n", employee->active ? "Active" : "Inactive");
}

void display_department(const Department *department, const HRDatabase *db) {
    printf("\nDepartment ID: %d\n", department->id);
    printf("Name: %s\n", department->name);
    printf("Description: %s\n", department->description);
    printf("Budget: $%.2f\n", department->budget);
    printf("Employee Count: %d\n", department->employee_count);
    
    if (department->manager_id != 0) {
        int manager_index = find_employee_by_id(db, department->manager_id);
        if (manager_index != -1) {
            printf("Manager: %s (ID: %d)\n", 
                   db->employees[manager_index].name, department->manager_id);
        }
    } else {
        printf("Manager: N/A\n");
    }
}

void display_all_employees(const HRDatabase *db) {
    printf("\n=== All Employees ===\n");
    printf("Total Employees: %d\n\n", db->employee_count);
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            display_employee(&db->employees[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_departments(const HRDatabase *db) {
    printf("\n=== All Departments ===\n");
    printf("Total Departments: %d\n\n", db->department_count);
    
    for (int i = 0; i < db->department_count; i++) {
        display_department(&db->departments[i], db);
        printf("------------------------\n");
    }
}

int find_employee_by_id(const HRDatabase *db, int employee_id) {
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].id == employee_id && db->employees[i].active) {
            return i;
        }
    }
    return -1;
}

int find_department_by_id(const HRDatabase *db, int department_id) {
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].id == department_id) {
            return i;
        }
    }
    return -1;
}

void search_employees_by_name(const HRDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active && strstr(db->employees[i].name, name) != NULL) {
            display_employee(&db->employees[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No employees found with name containing '%s'.\n", name);
    }
}

void search_employees_by_department(const HRDatabase *db, int department_id) {
    printf("\nEmployees in Department ID %d:\n", department_id);
    int found = 0;
    
    int dept_index = find_department_by_id(db, department_id);
    if (dept_index == -1) {
        printf("Department not found.\n");
        return;
    }
    
    printf("Department: %s\n", db->departments[dept_index].name);
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active && db->employees[i].department_id == department_id) {
            display_employee(&db->employees[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No employees found in this department.\n");
    }
}

void display_department_hierarchy(const HRDatabase *db, int department_id) {
    int dept_index = find_department_by_id(db, department_id);
    if (dept_index == -1) {
        printf("Department not found.\n");
        return;
    }
    
    Department *department = &db->departments[dept_index];
    printf("\n=== Hierarchy for %s ===\n", department->name);
    
    // Find the manager
    if (department->manager_id != 0) {
        int manager_index = find_employee_by_id(db, department->manager_id);
        if (manager_index != -1) {
            printf("Manager: %s\n", db->employees[manager_index].name);
            printf("  Position: %s\n", db->employees[manager_index].position);
            printf("  Email: %s\n", db->employees[manager_index].email);
        }
    } else {
        printf("Manager: N/A\n");
    }
    
    // Find all employees in this department
    printf("\nDepartment Employees:\n");
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active && db->employees[i].department_id == department_id) {
            printf("- %s (%s)\n", db->employees[i].name, db->employees[i].position);
        }
    }
}

void calculate_payroll(HRDatabase *db) {
    calculate_years_of_service(db);
    
    float total_payroll = 0.0f;
    float total_bonus = 0.0f;
    
    printf("\n=== Payroll Summary ===\n");
    printf("%-5s %-20s %-15s %-10s %-10s %-10s\n", 
           "ID", "Name", "Position", "Salary", "Bonus", "Total");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            Employee *employee = &db->employees[i];
            calculate_bonus(db, employee->id);
            float total = employee->salary + employee->bonus;
            
            printf("%-5d %-20s %-15s $%-9.2f $%-9.2f $%-9.2f\n", 
                   employee->id, employee->name, employee->position, 
                   employee->salary, employee->bonus, total);
            
            total_payroll += employee->salary;
            total_bonus += employee->bonus;
        }
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total Salary: $%.2f\n", total_payroll);
    printf("Total Bonus: $%.2f\n", total_bonus);
    printf("Total Payroll: $%.2f\n", total_payroll + total_bonus);
}

void promote_employee(HRDatabase *db, int employee_id, const char *new_position, float salary_increase) {
    int index = find_employee_by_id(db, employee_id);
    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    Employee *employee = &db->employees[index];
    
    // Update position and salary
    strncpy(employee->position, new_position, MAX_POSITION_LENGTH - 1);
    employee->position[MAX_POSITION_LENGTH - 1] = '\0';
    
    float increase_amount = employee->salary * (salary_increase / 100.0f);
    employee->salary += increase_amount;
    
    printf("Employee %s promoted to %s with salary increase of $%.2f\n", 
           employee->name, new_position, increase_amount);
    printf("New salary: $%.2f\n", employee->salary);
    
    // If this is a management position, check if they should be a department manager
    if (strstr(new_position, "Manager") != NULL || strstr(new_position, "Director") != NULL) {
        int dept_index = find_department_by_id(db, employee->department_id);
        if (dept_index != -1 && db->departments[dept_index].manager_id == 0) {
            printf("This employee is now a manager. Would you like to assign them as department manager? (y/n): ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                db->departments[dept_index].manager_id = employee_id;
                printf("Employee assigned as manager of %s department.\n", 
                       db->departments[dept_index].name);
            }
        }
    }
}

void transfer_employee(HRDatabase *db, int employee_id, int new_department_id) {
    int emp_index = find_employee_by_id(db, employee_id);
    int old_dept_index = find_department_by_id(db, db->employees[emp_index].department_id);
    int new_dept_index = find_department_by_id(db, new_department_id);
    
    if (emp_index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    if (new_dept_index == -1) {
        printf("Department not found.\n");
        return;
    }
    
    if (old_dept_index != -1) {
        db->departments[old_dept_index].employee_count--;
    }
    
    db->employees[emp_index].department_id = new_department_id;
    db->departments[new_dept_index].employee_count++;
    
    printf("Employee %s transferred to department %s.\n", 
           db->employees[emp_index].name, db->departments[new_dept_index].name);
}

void terminate_employee(HRDatabase *db, int employee_id) {
    int index = find_employee_by_id(db, employee_id);
    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    int dept_index = find_department_by_id(db, db->employees[index].department_id);
    if (dept_index != -1) {
        db->departments[dept_index].employee_count--;
    }
    
    // Check if this employee is a manager of any department
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].manager_id == employee_id) {
            db->departments[i].manager_id = 0;
            printf("Employee was manager of %s department. Manager position cleared.\n", 
                   db->departments[i].name);
        }
    }
    
    // Check if this employee is a manager for any other employees
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].manager_id == employee_id) {
            db->employees[i].manager_id = 0;
        }
    }
    
    db->employees[index].active = 0;
    printf("Employee %s terminated.\n", db->employees[index].name);
}

int save_database(const HRDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    fwrite(&db->employee_count, sizeof(int), 1, file);
    fwrite(db->employees, sizeof(Employee), db->employee_count, file);
    fwrite(&db->department_count, sizeof(int), 1, file);
    fwrite(db->departments, sizeof(Department), db->department_count, file);
    
    fclose(file);
    return 1;
}

int load_database(HRDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    fread(&db->employee_count, sizeof(int), 1, file);
    fread(db->employees, sizeof(Employee), db->employee_count, file);
    fread(&db->department_count, sizeof(int), 1, file);
    fread(db->departments, sizeof(Department), db->department_count, file);
    
    fclose(file);
    return 1;
}

void generate_sample_data(HRDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* dept_names[] = {"Engineering", "Sales", "Marketing", "Human Resources", 
                          "Finance", "Operations", "Legal", "IT Support"};
    
    char* dept_desc[] = {"Software development and technical solutions", 
                        "Sales and customer relations",
                        "Marketing and advertising", 
                        "Employee management and recruitment",
                        "Financial planning and accounting", 
                        "Business operations and logistics",
                        "Legal affairs and compliance", 
                        "Technical support and infrastructure"};
    
    srand(time(NULL));
    
    // Add departments
    for (int i = 0; i < 8 && db->department_count < MAX_DEPARTMENTS; i++) {
        add_department(db, dept_names[i], dept_desc[i], 0, 100000.0f + (rand() % 500000));
    }
    
    // Add employees
    for (int i = 0; i < 50 && db->employee_count < MAX_EMPLOYEES; i++) {
        char name[60], email[60], phone[15], address[100];
        sprintf(name, "%s %s", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(email, "%s.%s@company.com", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Business Ave, City, State", 100 + rand() % 900);
        
        int department_id = 1 + (rand() % db->department_count);
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 2015 + (rand() % 8);
        Date hire_date = {day, month, year};
        
        float salary = 30000.0f + (rand() % 100000);
        
        int manager_id = 0;
        if (i > 0 && (rand() % 3) != 0) { // 2/3 chance of having a manager
            manager_id = 1 + (rand() % i);
        }
        
        const char *position = position_levels[rand() % 10];
        
        add_employee(db, name, position, department_id, hire_date, salary, 
                    email, phone, address, manager_id);
        
        // Set random performance score
        if (i > 0) {
            db->employees[i].performance_score = 3.0f + (rand() % 71) / 10.0f; // 3.0 to 10.0
        }
    }
    
    // Assign some employees as department managers
    for (int i = 0; i < db->department_count && i < 10; i++) {
        int employee_index = 1 + (rand() % (db->employee_count - 1));
        db->departments[i].manager_id = db->employees[employee_index].id;
        
        // Update the employee's position to a management one
        sprintf(db->employees[employee_index].position, "Manager");
    }
}

void display_top_performers(const HRDatabase *db) {
    printf("\n=== Top Performers ===\n");
    
    // Create a temporary array to sort employees by performance score
    Employee temp_employees[MAX_EMPLOYEES];
    int count = 0;
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            temp_employees[count] = db->employees[i];
            count++;
        }
    }
    
    // Sort employees by performance score (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (temp_employees[j].performance_score < temp_employees[j + 1].performance_score) {
                Employee temp = temp_employees[j];
                temp_employees[j] = temp_employees[j + 1];
                temp_employees[j + 1] = temp;
            }
        }
    }
    
    int display_count = count < 10 ? count : 10;
    printf("%-5s %-20s %-15s %-10s %-15s\n", 
           "ID", "Name", "Position", "Score", "Bonus");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < display_count; i++) {
        calculate_bonus((HRDatabase*)db, temp_employees[i].id);
        
        printf("%-5d %-20s %-15s %-10.1f $%-14.2f\n", 
               temp_employees[i].id, temp_employees[i].name, 
               temp_employees[i].position, temp_employees[i].performance_score,
               temp_employees[i].bonus);
    }
}

void display_salary_statistics(const HRDatabase *db) {
    if (db->employee_count == 0) {
        printf("No employees in database.\n");
        return;
    }
    
    float total_salary = 0.0f;
    float min_salary = FLT_MAX;
    float max_salary = 0.0f;
    int active_count = 0;
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            float salary = db->employees[i].salary;
            total_salary += salary;
            if (salary < min_salary) min_salary = salary;
            if (salary > max_salary) max_salary = salary;
            active_count++;
        }
    }
    
    float average_salary = total_salary / active_count;
    
    printf("\n=== Salary Statistics ===\n");
    printf("Active Employees: %d\n", active_count);
    printf("Total Salary Expense: $%.2f\n", total_salary);
    printf("Average Salary: $%.2f\n", average_salary);
    printf("Minimum Salary: $%.2f\n", min_salary);
    printf("Maximum Salary: $%.2f\n", max_salary);
    
    // Calculate median
    float salaries[MAX_EMPLOYEES];
    int count = 0;
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            salaries[count] = db->employees[i].salary;
            count++;
        }
    }
    
    // Sort salaries
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (salaries[j] > salaries[j + 1]) {
                float temp = salaries[j];
                salaries[j] = salaries[j + 1];
                salaries[j + 1] = temp;
            }
        }
    }
    
    float median;
    if (count % 2 == 0) {
        median = (salaries[count/2 - 1] + salaries[count/2]) / 2.0f;
    } else {
        median = salaries[count/2];
    }
    
    printf("Median Salary: $%.2f\n", median);
}

void generate_payroll_report(const HRDatabase *db, int month, int year) {
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        printf("Invalid month or year.\n");
        return;
    }
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Payroll Report for %s %d ===\n", month_names[month - 1], year);
    
    float total_salary = 0.0f;
    float total_bonus = 0.0f;
    float total_tax = 0.0f;
    
    printf("%-5s %-20s %-15s %-10s %-10s %-10s %-10s\n", 
           "ID", "Name", "Position", "Salary", "Bonus", "Tax", "Net Pay");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < db->employee_count; i++) {
        if (db->employees[i].active) {
            Employee *employee = &db->employees[i];
            calculate_bonus((HRDatabase*)db, employee->id);
            
            float gross_pay = employee->salary + employee->bonus;
            float tax = gross_pay * 0.25f; // 25% tax
            float net_pay = gross_pay - tax;
            
            printf("%-5d %-20s %-15s $%-9.2f $%-9.2f $%-9.2f $%-9.2f\n", 
                   employee->id, employee->name, employee->position, 
                   employee->salary, employee->bonus, tax, net_pay);
            
            total_salary += employee->salary;
            total_bonus += employee->bonus;
            total_tax += tax;
        }
    }
    
    printf("--------------------------------------------------------------------\n");
    printf("Total Salary: $%.2f\n", total_salary);
    printf("Total Bonus: $%.2f\n", total_bonus);
    printf("Total Tax: $%.2f\n", total_tax);
    printf("Total Net Pay: $%.2f\n", total_salary + total_bonus - total_tax);
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Check for leap year
    if (month == 2 && ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0))) {
        days_in_month[1] = 29;
    }
    
    if (day < 1 || day > days_in_month[month - 1]) return 0;
    
    return 1;
}