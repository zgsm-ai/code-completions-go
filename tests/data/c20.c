#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_STUDENTS 1000
#define MAX_COURSES 500
#define MAX_ENROLLMENTS 2000
#define MAX_DEPARTMENTS 50
#define MAX_NAME_LENGTH 50
#define FILENAME_STUDENTS "students.dat"
#define FILENAME_COURSES "courses.dat"
#define FILENAME_ENROLLMENTS "enrollments.dat"
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
    Date birth_date;
    int student_id;
    float gpa;
    int credits_completed;
    int total_credits;
    Date enrollment_date;
    int active;
    int graduation_year;
} Student;

typedef struct {
    int id;
    char course_code[20];
    char course_name[MAX_NAME_LENGTH];
    char instructor_name[MAX_NAME_LENGTH];
    int credits;
    int max_students;
    char department[50];
    char description[200];
    int active;
    int semester;
} Course;

typedef struct {
    int id;
    int student_id;
    int course_id;
    Date enrollment_date;
    char grade;
    float grade_points;
    int credits_earned;
    int completed;
    int semester;
} Enrollment;

typedef struct {
    int id;
    char name[50];
    char chairperson[MAX_NAME_LENGTH];
    char description[200];
    int budget;
    int active;
} Department;

typedef struct {
    Student students[MAX_STUDENTS];
    int student_count;
    Course courses[MAX_COURSES];
    int course_count;
    Enrollment enrollments[MAX_ENROLLMENTS];
    int enrollment_count;
    Department departments[MAX_DEPARTMENTS];
    int department_count;
} UniversityDatabase;

// Function prototypes
void initialize_database(UniversityDatabase *db);
int add_student(UniversityDatabase *db, const char *first_name, const char *last_name,
                  const char *email, const char *phone, const char *address,
                  Date birth_date, int student_id, float gpa, int credits_completed,
                  int total_credits, int graduation_year);
int add_course(UniversityDatabase *db, const char *course_code, const char *course_name,
                 const char *instructor_name, int credits, int max_students,
                 const char *department, const char *description, int semester);
int add_department(UniversityDatabase *db, const char *name, const char *chairperson,
                       const char *description, int budget);
int enroll_student(UniversityDatabase *db, int student_id, int course_id, int semester, const char *grade);
void update_student_gpa(UniversityDatabase *db, int student_id);
void display_student(const Student *student);
void display_course(const Course *course);
void display_department(const Department *department);
void display_enrollment(const Enrollment *enrollment, const UniversityDatabase *db);
void display_all_students(const UniversityDatabase *db);
void display_all_courses(const UniversityDatabase *db);
void display_all_departments(const UniversityDatabase *db);
void display_all_enrollments(const UniversityDatabase *db);
void search_students_by_name(const UniversityDatabase *db, const char *name);
void search_courses_by_department(const UniversityDatabase *db, const char *department);
void search_enrollments_by_semester(const UniversityDatabase *db, int semester);
void generate_transcript(const UniversityDatabase *db, int student_id);
void generate_course_report(const UniversityDatabase *db, int course_id);
void generate_department_report(const UniversityDatabase *db, int department_id);
void generate_gpa_statistics(const UniversityDatabase *db);
int find_student_by_id(const UniversityDatabase *db, int student_id);
int find_course_by_id(const UniversityDatabase *db, int course_id);
int find_department_by_id(const UniversityDatabase *db, int department_id);
int find_enrollment_by_id(const UniversityDatabase *db, int enrollment_id);
int save_database(const UniversityDatabase *db);
int load_database(UniversityDatabase *db);
void generate_sample_data(UniversityDatabase *db);
void calculate_student_rankings(UniversityDatabase *db);
void display_academic_calendar(const UniversityDatabase *db, int semester);
int is_valid_date(int day, int month, int year);
const char* grade_to_points(const char *grade);

int main() {
    UniversityDatabase db;
    initialize_database(&db);
    
    printf("University Management System\n");
    printf("1. Add Student\n");
    printf("2. Add Course\n");
    printf("3. Add Department\n");
    printf("4. Enroll Student\n");
    printf("5. Update Student GPA\n");
    printf("6. Display All Students\n");
    printf("7. Display All Courses\n");
    printf("8. Display All Departments\n");
    printf("9. Display All Enrollments\n");
    printf("10. Search Students by Name\n");
    printf("11. Search Courses by Department\n");
    printf("12. Search Enrollments by Semester\n");
    printf("13. Generate Transcript\n");
    printf("14. Generate Course Report\n");
    printf("15. Generate Department Report\n");
    printf("16. Generate GPA Statistics\n");
    printf("17. Save Database\n");
    printf("18. Load Database\n");
    printf("19. Generate Sample Data\n");
    printf("20. Calculate Student Rankings\n");
    printf("21. Display Academic Calendar\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100];
                int day, month, year, student_id, graduation_year;
                float gpa;
                int credits_completed, total_credits;
                
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
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter GPA: ");
                scanf("%f", &gpa);
                printf("Enter credits completed: ");
                scanf("%d", &credits_completed);
                printf("Enter total credits: ");
                scanf("%d", &total_credits);
                printf("Enter graduation year (0 if not graduated): ");
                scanf("%d", &graduation_year);
                
                Date birth_date = {day, month, year};
                
                int student_id_return = add_student(&db, first_name, last_name, email, phone, address,
                                                 birth_date, student_id, gpa, credits_completed,
                                                 total_credits, graduation_year);
                if (student_id_return != -1) {
                    printf("Student added with ID: %d\n", student_id_return);
                } else {
                    printf("Failed to add student. Database is full.\n");
                }
                break;
            }
            case 2: {
                char course_code[20], course_name[MAX_NAME_LENGTH];
                char instructor_name[MAX_NAME_LENGTH], department[50], description[200];
                int credits, max_students, semester, active;
                
                printf("Enter course code: ");
                scanf("%s", course_code);
                printf("Enter course name: ");
                scanf(" %[^\n]", course_name);
                printf("Enter instructor name: ");
                scanf(" %[^\n]", instructor_name);
                printf("Enter department: ");
                scanf(" %[^\n]", department);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter credits: ");
                scanf("%d", &credits);
                printf("Enter max students: ");
                scanf("%d", &max_students);
                printf("Enter semester (1-Spring, 2-Summer, 3-Fall): ");
                scanf("%d", &semester);
                printf("Is course active (0-No, 1-Yes): ");
                scanf("%d", &active);
                
                int course_id = add_course(&db, course_code, course_name, instructor_name,
                                           credits, max_students, department, 
                                           description, semester, active);
                if (course_id != -1) {
                    printf("Course added with ID: %d\n", course_id);
                } else {
                    printf("Failed to add course. Database is full.\n");
                }
                break;
            }
            case 3: {
                char name[50], chairperson[MAX_NAME_LENGTH], description[200];
                int budget, active;
                
                printf("Enter department name: ");
                scanf(" %[^\n]", name);
                printf("Enter chairperson: ");
                scanf(" %[^\n]", chairperson);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter budget: ");
                scanf("%d", &budget);
                printf("Is department active (0-No, 1-Yes): ");
                scanf("%d", &active);
                
                int department_id = add_department(db, name, chairperson, description, budget, active);
                if (department_id != -1) {
                    printf("Department added with ID: %d\n", department_id);
                } else {
                    printf("Failed to add department. Database is full.\n");
                }
                break;
            }
            case 4: {
                int student_id, course_id, semester;
                char grade;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                printf("Enter semester (1-Spring, 2-Summer, 3-Fall): ");
                scanf("%d", &semester);
                printf("Enter grade (A-F): ");
                scanf(" %c", &grade);
                
                if (enroll_student(&db, student_id, course_id, semester, grade)) {
                    printf("Student enrolled successfully.\n");
                } else {
                    printf("Failed to enroll student. Check IDs and course capacity.\n");
                }
                break;
            }
            case 5: {
                int student_id;
                float gpa;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter new GPA: ");
                scanf("%f", &gpa);
                
                update_student_gpa(&db, student_id, gpa);
                break;
            }
            case 6: {
                display_all_students(&db);
                break;
            }
            case 7: {
                display_all_courses(&db);
                break;
            }
            case 8: {
                display_all_departments(&db);
                break;
            }
            case 9: {
                display_all_enrollments(&db);
                break;
            }
            case 10: {
                char name[MAX_NAME_LENGTH];
                printf("Enter student name to search: ");
                scanf(" %[^\n]", name);
                search_students_by_name(&db, name);
                break;
            }
            case 11: {
                char department[50];
                printf("Enter department to search: ");
                scanf(" %[^\n]", department);
                search_courses_by_department(&db, department);
                break;
            }
            case 12: {
                int semester;
                printf("Enter semester (1-Spring, 2-Summer, 3-Fall): ");
                scanf("%d", &semester);
                search_enrollments_by_semester(&db, semester);
                break;
            }
            case 13: {
                int student_id;
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                generate_transcript(&db, student_id);
                break;
            }
            case 14: {
                int course_id;
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                generate_course_report(&db, course_id);
                break;
            }
            case 15: {
                int department_id;
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                generate_department_report(&db, department_id);
                break;
            }
            case 16: {
                generate_gpa_statistics(&db);
                break;
            }
            case 17: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 18: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 19: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 20: {
                calculate_student_rankings(&db);
                break;
            }
            case 21: {
                int semester;
                printf("Enter semester (1-Spring, 2-Summer, 3-Fall): ");
                scanf("%d", &semester);
                display_academic_calendar(&db, semester);
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

void initialize_database(UniversityDatabase *db) {
    <｜fim▁hole｜>db->student_count = 0;
    db->course_count = 0;
    db->enrollment_count = 0;
    db->department_count = 0;
}

int add_student(UniversityDatabase *db, const char *first_name, const char *last_name,
                  const char *email, const char *phone, const char *address,
                  Date birth_date, int student_id, float gpa, int credits_completed,
                  int total_credits, int graduation_year) {
    if (db->student_count >= MAX_STUDENTS) {
        return -1;
    }
    
    Student *student = &db->students[db->student_count];
    student->id = db->student_count + 1;
    strncpy(student->first_name, first_name, MAX_NAME_LENGTH - 1);
    student->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(student->last_name, last_name, MAX_NAME_LENGTH - 1);
    student->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(student->email, email, 49);
    student->email[49] = '\0';
    strncpy(student->phone, phone, 14);
    student->phone[14] = '\0';
    strncpy(student->address, address, 99);
    student->address[99] = '\0';
    student->birth_date = birth_date;
    student->student_id = student_id;
    student->gpa = gpa;
    student->credits_completed = credits_completed;
    student->total_credits = total_credits;
    
    // Set enrollment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    student->enrollment_date.day = current_time->tm_mday;
    student->enrollment_date.month = current_time->tm_mon + 1;
    student->enrollment_date.year = current_time->tm_year + 1900;
    
    student->graduation_year = graduation_year;
    student->active = 1;
    
    db->student_count++;
    return student->id;
}

int add_course(UniversityDatabase *db, const char *course_code, const char *course_name,
                 const char *instructor_name, int credits, int max_students,
                 const char *department, const char *description, int semester, int active) {
    if (db->course_count >= MAX_COURSES) {
        return -1;
    }
    
    Course *course = &db->courses[db->course_count];
    course->id = db->course_count + 1;
    strncpy(course->course_code, course_code, 19);
    course->course_code[19] = '\0';
    strncpy(course->course_name, course_name, MAX_NAME_LENGTH - 1);
    course->course_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(course->instructor_name, instructor_name, MAX_NAME_LENGTH - 1);
    course->instructor_name[MAX_NAME_LENGTH - 1] = '\0';
    course->credits = credits;
    course->max_students = max_students;
    strncpy(course->department, department, 49);
    course->department[49] = '\0';
    strncpy(course->description, description, 199);
    course->description[199] = '\0';
    course->active = active;
    course->semester = semester;
    
    db->course_count++;
    return course->id;
}

int add_department(UniversityDatabase *db, const char *name, const char *chairperson,
                       const char *description, int budget, int active) {
    if (db->department_count >= MAX_DEPARTMENTS) {
        return -1;
    }
    
    Department *department = &db->departments[db->department_count];
    department->id = db->department_count + 1;
    strncpy(department->name, name, 49);
    department->name[49] = '\0';
    strncpy(department->chairperson, chairperson, MAX_NAME_LENGTH - 1);
    department->chairperson[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(department->description, description, 199);
    department->description[199] = '\0';
    department->budget = budget;
    department->active = active;
    
    db->department_count++;
    return department->id;
}

int enroll_student(UniversityDatabase *db, int student_id, int course_id, int semester, const char *grade) {
    if (db->enrollment_count >= MAX_ENROLLMENTS) {
        return 0;
    }
    
    // Validate student and course exist
    int student_index = find_student_by_id(db, student_id);
    int course_index = find_course_by_id(db, course_id);
    
    if (student_index == -1 || course_index == -1) {
        return 0;
    }
    
    // Check if course is active and has capacity
    if (!db->courses[course_index].active) {
        printf("Course is not active.\n");
        return 0;
    }
    
    int enrolled_count = 0;
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].course_id == course_id && db->enrollments[i].completed) {
            enrolled_count++;
        }
    }
    
    if (enrolled_count >= db->courses[course_index].max_students) {
        printf("Course is at maximum capacity.\n");
        return 0;
    }
    
    // Validate grade
    if (strlen(grade) != 1 || (grade[0] < 'A' || grade[0] > 'F')) {
        printf("Invalid grade. Must be A-F.\n");
        return 0;
    }
    
    // Create enrollment
    Enrollment *enrollment = &db->enrollments[db->enrollment_count];
    enrollment->id = db->enrollment_count + 1;
    enrollment->student_id = student_id;
    enrollment->course_id = course_id;
    enrollment->semester = semester;
    
    // Set enrollment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    enrollment->enrollment_date.day = current_time->tm_mday;
    enrollment->enrollment_date.month = current_time->tm_mon + 1;
    enrollment->enrollment_date.year = current_time->tm_year + 1900;
    
    enrollment->grade[0] = grade[0];
    enrollment->grade[1] = '\0';
    enrollment->grade_points = grade_to_points(grade);
    enrollment->credits_earned = db->courses[course_index].credits;
    enrollment->completed = 0; // Not completed until graded
    
    db->enrollment_count++;
    return 1;
}

void update_student_gpa(UniversityDatabase *db, int student_id, float gpa) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    Student *student = &db->students[student_index];
    student->gpa = gpa;
    
    printf("GPA updated for student %s %s: %.2f\n", 
           student->first_name, student->last_name, student->gpa);
}

void display_student(const Student *student) {
    printf("\nStudent ID: %d\n", student->id);
    printf("Name: %s %s\n", student->first_name, student->last_name);
    printf("Student ID: %d\n", student->student_id);
    printf("Email: %s\n", student->email);
    printf("Phone: %s\n", student->phone);
    printf("Address: %s\n", student->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           student->birth_date.day, student->birth_date.month, student->birth_date.year);
    printf("GPA: %.2f\n", student->gpa);
    printf("Credits Completed: %d/%d\n", student->credits_completed, student->total_credits);
    printf("Enrollment Date: %02d/%02d/%d\n", 
           student->enrollment_date.day, student->enrollment_date.month, student->enrollment_date.year);
    printf("Graduation Year: %d\n", student->graduation_year);
    printf("Status: %s\n", student->active ? "Active" : "Inactive");
}

void display_course(const Course *course) {
    printf("\nCourse ID: %d\n", course->id);
    printf("Code: %s\n", course->course_code);
    printf("Name: %s\n", course->course_name);
    printf("Instructor: %s\n", course->instructor_name);
    printf("Credits: %d\n", course->credits);
    printf("Department: %s\n", course->department);
    printf("Max Students: %d\n", course->max_students);
    printf("Description: %s\n", course->description);
    printf("Semester: %d\n", course->semester);
    printf("Status: %s\n", course->active ? "Active" : "Inactive");
}

void display_department(const Department *department) {
    printf("\nDepartment ID: %d\n", department->id);
    printf("Name: %s\n", department->name);
    printf("Chairperson: %s\n", department->chairperson);
    printf("Description: %s\n", department->description);
    printf("Budget: $%d\n", department->budget);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
}

void display_enrollment(const Enrollment *enrollment, const UniversityDatabase *db) {
    printf("\nEnrollment ID: %d\n", enrollment->id);
    
    // Display student information
    int student_index = find_student_by_id(db, enrollment->student_id);
    if (student_index != -1) {
        printf("Student: %s %s (ID: %d)\n", 
               db->students[student_index].first_name,
               db->students[student_index].last_name,
               enrollment->student_id);
    }
    
    // Display course information
    int course_index = find_course_by_id(db, enrollment->course_id);
    if (course_index != -1) {
        printf("Course: %s (%s) (ID: %d)\n", 
               db->courses[course_index].course_code,
               db->courses[course_index].course_name,
               enrollment->course_id);
    }
    
    printf("Enrollment Date: %02d/%02d/%d\n", 
           enrollment->enrollment_date.day, enrollment->enrollment_date.month, enrollment->enrollment_date.year);
    printf("Semester: %d\n", enrollment->semester);
    printf("Grade: %s\n", enrollment->grade);
    printf("Grade Points: %.1f\n", enrollment->grade_points);
    printf("Credits Earned: %d\n", enrollment->credits_earned);
    printf("Status: %s\n", enrollment->completed ? "Completed" : "In Progress");
}

void display_all_students(const UniversityDatabase *db) {
    printf("\n=== All Students ===\n");
    printf("Total Students: %d\n\n", db->student_count);
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            display_student(&db->students[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_courses(const UniversityDatabase *db) {
    printf("\n=== All Courses ===\n");
    printf("Total Courses: %d\n\n", db->course_count);
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active) {
            display_course(&db->courses[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_departments(const UniversityDatabase *db) {
    printf("\n=== All Departments ===\n");
    printf("Total Departments: %d\n\n", db->department_count);
    
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].active) {
            display_department(&db->departments[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_enrollments(const UniversityDatabase *db) {
    printf("\n=== All Enrollments ===\n");
    printf("Total Enrollments: %d\n\n", db->enrollment_count);
    
    for (int i = 0; i < db->enrollment_count; i++) {
        display_enrollment(&db->enrollments[i], db);
        printf("========================\n");
    }
}

void search_students_by_name(const UniversityDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active && 
            (strstr(db->students[i].first_name, name) != NULL || 
             strstr(db->students[i].last_name, name) != NULL)) {
            display_student(&db->students[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No students found with name containing '%s'.\n", name);
    }
}

void search_courses_by_department(const UniversityDatabase *db, const char *department) {
    printf("\nCourses in Department: %s\n", department);
    int found = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active && strstr(db->courses[i].department, department) != NULL) {
            display_course(&db->courses[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No courses found in this department.\n");
    }
}

void search_enrollments_by_semester(const UniversityDatabase *db, int semester) {
    printf("\nEnrollments for Semester: %d\n", semester);
    int found = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].semester == semester) {
            display_enrollment(&db->enrollments[i], db);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No enrollments found for this semester.\n");
    }
}

void generate_transcript(const UniversityDatabase *db, int student_id) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    Student *student = &db->students[student_index];
    
    printf("\n=== Transcript for %s %s ===\n", student->first_name, student->last_name);
    printf("Student ID: %d\n", student->student_id);
    printf("GPA: %.2f\n", student->gpa);
    printf("Credits Completed: %d/%d\n", student->credits_completed, student->total_credits);
    printf("Enrollment Date: %02d/%02d/%d\n", 
           student->enrollment_date.day, student->enrollment_date.month, student->enrollment_date.year);
    printf("Graduation Year: %d\n", student->graduation_year);
    printf("\nCourse History:\n");
    printf("%-10s %-15s %-8s %-8s %-8s\n", 
           "Semester", "Course Code", "Grade", "Credits", "Status");
    printf("-------------------------------------------------\n");
    
    int total_credits = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id) {
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1) {
                printf("%-10d %-15s %-8s %-8s %-8d %-8s\n", 
                       db->enrollments[i].semester,
                       db->courses[course_index].course_code,
                       db->enrollments[i].grade,
                       db->enrollments[i].credits_earned,
                       db->enrollments[i].completed ? "Completed" : "In Progress");
                
                if (db->enrollments[i].completed) {
                    total_credits += db->enrollments[i].credits_earned;
                }
            }
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Total Credits Earned: %d\n", total_credits);
    printf("Overall GPA: %.2f\n", student->gpa);
}

void generate_course_report(const UniversityDatabase *db, int course_id) {
    int course_index = find_course_by_id(db, course_id);
    if (course_index == -1) {
        printf("Course not found.\n");
        return;
    }
    
    Course *course = &db->courses[course_index];
    
    printf("\n=== Course Report: %s ===\n", course->course_name);
    printf("Course Code: %s\n", course->course_code);
    printf("Instructor: %s\n", course->instructor_name);
    printf("Credits: %d\n", course->credits);
    printf("Max Students: %d\n", course->max_students);
    printf("Department: %s\n", course->department);
    printf("Semester: %d\n", course->semester);
    printf("Description: %s\n", course->description);
    printf("Status: %s\n", course->active ? "Active" : "Inactive");
    printf("\nEnrollment Statistics:\n");
    
    int total_enrolled = 0;
    int completed = 0;
    int in_progress = 0;
    int a_count = 0, b_count = 0, c_count = 0, d_count = 0, f_count = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].course_id == course_id) {
            total_enrolled++;
            
            if (db->enrollments[i].completed) {
                completed++;
            } else {
                in_progress++;
            }
            
            switch (db->enrollments[i].grade[0]) {
                case 'A': case 'a': a_count++; break;
                case 'B': case 'b': b_count++; break;
                case 'C': case 'c': c_count++; break;
                case 'D': case 'd': d_count++; break;
                case 'F': case 'f': f_count++; break;
            }
        }
    }
    
    printf("Total Enrolled: %d\n", total_enrolled);
    printf("Completed: %d (%.1f%%)\n", completed, total_enrolled > 0 ? (float)completed / total_enrolled * 100 : 0.0f);
    printf("In Progress: %d (%.1f%%)\n", in_progress, total_enrolled > 0 ? (float)in_progress / total_enrolled * 100 : 0.0f);
    
    printf("\nGrade Distribution:\n");
    printf("A: %d (%.1f%%)\n", a_count, total_enrolled > 0 ? (float)a_count / total_enrolled * 100 : 0.0f);
    printf("B: %d (%.1f%%)\n", b_count, total_enrolled > 0 ? (float)b_count / total_enrolled * 100 : 0.0f);
    printf("C: %d (%.1f%%)\n", c_count, total_enrolled > 0 ? (float)c_count / total_enrolled * 100 : 0.0f);
    printf("D: %d (%.1f%%)\n", d_count, total_enrolled > 0 ? (float)d_count / total_enrolled * 100 : 0.0f);
    printf("F: %d (%.1f%%)\n", f_count, total_enrolled > 0 ? (float)f_count / total_enrolled * 100 : 0.0f);
    
    printf("Average Grade: %.1f\n", 
           (a_count * 4.0f + b_count * 3.0f + c_count * 2.0f + d_count * 1.0f + f_count * 0.0f) / 
           (a_count + b_count + c_count + d_count + f_count) > 0 ? 1.0f : 0.0f);
}

void generate_department_report(const UniversityDatabase *db, int department_id) {
    int department_index = find_department_by_id(db, department_id);
    if (department_index == -1) {
        printf("Department not found.\n");
        return;
    }
    
    Department *department = &db->departments[department_index];
    
    printf("\n=== Department Report: %s ===\n", department->name);
    printf("Chairperson: %s\n", department->chairperson);
    printf("Budget: $%d\n", department->budget);
    printf("Description: %s\n", department->description);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
    printf("\nCourse Statistics:\n");
    
    int total_courses = 0;
    int active_courses = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (strcmp(db->courses[i].department, department->name) == 0) {
            total_courses++;
            
            if (db->courses[i].active) {
                active_courses++;
            }
        }
    }
    
    printf("Total Courses: %d\n", total_courses);
    printf("Active Courses: %d (%.1f%%)\n", 
           total_courses > 0 ? (float)active_courses / total_courses * 100 : 0.0f);
    
    printf("\nEnrollment Statistics:\n");
    int total_enrollments = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].completed) {
            total_enrollments++;
        }
    }
    
    int total_credits = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].completed) {
            total_credits += db->enrollments[i].credits_earned;
        }
    }
    
    printf("Total Enrollments Completed: %d\n", total_enrollments);
    printf("Total Credits Earned: %d\n", total_credits);
    printf("Average Credits per Course: %.1f\n", 
           total_courses > 0 ? (float)total_credits / total_courses : 0.0f);
}

void generate_gpa_statistics(const UniversityDatabase *db) {
    printf("\n=== GPA Statistics ===\n");
    
    float sum_gpa = 0.0f;
    int student_count = 0;
    int students_4_0 = 0, students_3_0 = 0, students_2_5 = 0, students_1_5 = 0;
    students_0_0 = 0;
    
    // Calculate statistics
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            sum_gpa += db->students[i].gpa;
            student_count++;
            
            if (db->students[i].gpa >= 4.0f) students_4_0++;
            else if (db->students[i].gpa >= 3.0f) students_3_0++;
            else if (db->students[i].gpa >= 2.0f) students_2_5++;
            else if (db->students[i].gpa >= 1.0f) students_1_5++;
            else students_0_0++;
        }
    }
    
    float avg_gpa = student_count > 0 ? sum_gpa / student_count : 0.0f;
    
    printf("Total Students: %d\n", student_count);
    printf("Average GPA: %.2f\n", avg_gpa);
    printf("GPA Distribution:\n");
    printf("4.0-4.0: %d (%.1f%%)\n", students_4_0,
           student_count > 0 ? (float)students_4_0 / student_count * 100 : 0.0f);
    printf("3.0-3.9: %d (%.1f%%)\n", students_3_0,
           student_count > 0 ? (float)students_3_0 / student_count * 100 : 0.0f);
    printf("2.0-2.9: %d (%.1f%%)\n", students_2_5,
           student_count > 0 ? (float)students_2_5 / student_count * 100 : 0.0f);
    printf("1.0-1.9: %d (%.1f%%)\n", students_1_5,
           student_count > 0 ? (float)students_1_5 / student_count * 100 : 0.0f);
    printf("0.0-0.9: %d (%.1f%%)\n", students_0_0,
           student_count > 0 ? (float)students_0_0 / student_count * 100 : 0.0f);
}

void calculate_student_rankings(UniversityDatabase *db) {
    printf("\n=== Student Rankings ===\n");
    
    // Create temporary array to sort students by GPA
    int sorted_indices[MAX_STUDENTS];
    for (int i = 0; i < db->student_count; i++) {
        sorted_indices[i] = i;
    }
    
    // Sort students by GPA (descending)
    for (int i = 0; i < db->student_count - 1; i++) {
        for (int j = 0; j < db->student_count - i - 1; j++) {
            if (db->students[sorted_indices[j]].gpa < db->students[sorted_indices[j + 1]].gpa) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    printf("%-5s %-20s %-10s %-10s %-10s\n", 
           "Rank", "Name", "GPA", "Credits", "Grad Year");
    printf("--------------------------------------------------\n");
    
    int display_count = db->student_count < 20 ? db->student_count : 20;
    
    for (int i = 0; i < display_count; i++) {
        int student_index = sorted_indices[db->student_count - i - 1];
        Student *student = &db->students[student_index];
        
        printf("%-5d %-20s %-20s %.2f %-10d/%-d %d\n", 
               i + 1, student->first_name, student->last_name, student->gpa,
               student->credits_completed, student->total_credits, student->graduation_year);
    }
}

void display_academic_calendar(const UniversityDatabase *db, int semester) {
    printf("\n=== Academic Calendar: Semester %d ===\n", semester);
    
    const char* semester_names[] = {"Spring", "Summer", "Fall"};
    
    printf("Courses for %s Semester:\n", semester_names[semester - 1]);
    
    int course_count = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active && db->courses[i].semester == semester) {
            display_course(&db->courses[i]);
            printf("------------------------\n");
            course_count++;
        }
    }
    
    printf("Total Courses: %d\n", course_count);
    
    printf("\nEnrollments for %s Semester:\n", semester_names[semester - 1]);
    
    int enrollment_count = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].semester == semester) {
            display_enrollment(&db->enrollments[i], db);
            printf("========================\n");
            enrollment_count++;
        }
    }
    
    printf("Total Enrollments: %d\n", enrollment_count);
}

int find_student_by_id(const UniversityDatabase *db, int student_id) {
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].id == student_id && db->students[i].active) {
            return i;
        }
    }
    return -1;
}

int find_course_by_id(const UniversityDatabase *db, int course_id) {
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].id == course_id && db->courses[i].active) {
            return i;
        }
    }
    return -1;
}

int find_department_by_id(const UniversityDatabase *db, int department_id) {
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].id == department_id && db->departments[i].active) {
            return i;
        }
    }
    return -1;
}

int find_enrollment_by_id(const UniversityDatabase *db, int enrollment_id) {
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].id == enrollment_id) {
            return i;
        }
    }
    return -1;
}

int save_database(const UniversityDatabase *db) {
    FILE *students_file = fopen(FILENAME_STUDENTS, "wb");
    if (students_file != NULL) {
        fwrite(&db->student_count, sizeof(int), 1, students_file);
        fwrite(db->students, sizeof(Student), db->student_count, students_file);
        fclose(students_file);
    } else {
        return 0;
    }
    
    FILE *courses_file = fopen(FILENAME_COURSES, "wb");
    if (courses_file != NULL) {
        fwrite(&db->course_count, sizeof(int), 1, courses_file);
        fwrite(db->courses, sizeof(Course), db->course_count, courses_file);
        fclose(courses_file);
    } else {
        return 0;
    }
    
    FILE *enrollments_file = fopen(FILENAME_ENROLLMENTS, "wb");
    if (enrollments_file != NULL) {
        fwrite(&db->enrollment_count, sizeof(int), 1, enrollments_file);
        fwrite(db->enrollments, sizeof(Enrollment), db->enrollment_count, enrollments_file);
        fclose(enrollments_file);
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

int load_database(UniversityDatabase *db) {
    FILE *students_file = fopen(FILENAME_STUDENTS, "rb");
    if (students_file != NULL) {
        fread(&db->student_count, sizeof(int), 1, students_file);
        fread(db->students, sizeof(Student), db->student_count, students_file);
        fclose(students_file);
    } else {
        return 0;
    }
    
    FILE *courses_file = fopen(FILENAME_COURSES, "rb");
    if (courses_file != NULL) {
        fread(&db->course_count, sizeof(int), 1, courses_file);
        fread(db->courses, sizeof(Course), db->course_count, courses_file);
        fclose(courses_file);
    } else {
        return 0;
    }
    
    FILE *enrollments_file = fopen(FILENAME_ENROLLMENTS, "rb");
    if (enrollments_file != NULL) {
        fread(&db->enrollment_count, sizeof(int), 1, enrollments_file);
        fread(db->enrollments, sizeof(Enrollment), db->enrollment_count, enrollments_file);
        fclose(enrollments_file);
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

void generate_sample_data(UniversityDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* department_names[] = {"Computer Science", "Mathematics", "Physics", "Chemistry",
                             "Biology", "Psychology", "History", "Literature",
                             "Economics", "Business", "Art", "Music"};
    
    char* instructor_names[] = {"Dr. Smith", "Prof. Johnson", "Dr. Williams", "Prof. Brown",
                               "Dr. Jones", "Prof. Garcia", "Dr. Miller", "Prof. Davis",
                               "Dr. Rodriguez", "Prof. Martinez", "Dr. Wilson", "Prof. Anderson"};
    
    char* chairpersons[] = {"Dr. Smith", "Prof. Johnson", "Dr. Williams", "Prof. Brown",
                           "Dr. Jones", "Prof. Garcia", "Dr. Miller", "Prof. Davis",
                           "Dr. Rodriguez", "Prof. Martinez", "Dr. Wilson", "Prof. Anderson"};
    
    srand(time(NULL));
    
    // Add departments
    for (int i = 0; i < 10 && db->department_count < MAX_DEPARTMENTS; i++) {
        char description[200];
        sprintf(description, "Department of %s offering cutting-edge research and education", department_names[i]);
        int budget = 1000000 + (rand() % 5000000);
        
        add_department(db, department_names[i], chairpersons[i], description, budget, 1);
    }
    
    // Add courses
    for (int i = 0; i < 50 && db->course_count < MAX_COURSES; i++) {
        char course_code[20], description[200];
        sprintf(course_code, "CS%d%03d", 100 + i);
        sprintf(description, "Advanced %s course covering fundamental and advanced topics", department_names[rand() % 10]);
        
        int credits = 3 + (rand() % 4); // 3-6 credits
        int max_students = 30 + (rand() % 50); // 30-80 students
        int semester = 1 + (rand() % 3); // 1-3 semesters
        int active = 1;
        
        add_course(db, course_code, course_names[rand() % 10], instructor_names[rand() % 10],
                 credits, max_students, department_names[rand() % 10],
                 description, semester, active);
    }
    
    // Add students
    for (int i = 0; i < 200 && db->student_count < MAX_STUDENTS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 20]);
        strcpy(last_name, last_names[rand() % 12]);
        
        char email[50], phone[15], address[100];
        sprintf(email, "%s.%s@university.edu", first_name, last_name);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Campus Drive, City, State", 100 + rand() % 900);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1990 + (rand() % 30);
        Date birth_date = {day, month, year};
        
        int student_id = 10000 + i;
        float gpa = 2.0f + (rand() % 200) / 100.0f; // 2.0-4.0 GPA
        int credits_completed = 30 + (rand() % 120); // 30-150 credits
        int total_credits = credits_completed + (rand() % 30); // Some have not yet taken courses
        int graduation_year = (rand() % 15 == 0) ? 0 : 2020 + (rand() % 10); // Some have graduated
        
        add_student(db, first_name, last_name, email, phone, address,
                 birth_date, student_id, gpa, credits_completed, total_credits, graduation_year);
    }
    
    // Create enrollments
    for (int i = 0; i < 300 && db->enrollment_count < MAX_ENROLLMENTS; i++) {
        int student_id = 10000 + (rand() % 200);
        int course_id = 100 + (rand() % 50);
        int semester = 1 + (rand() % 3);
        char grade[] = {'A', 'B', 'C', 'D', 'F'};
        
        const char* grade_point_values[] = {"4.0", "3.0", "2.0", "1.0", "0.0"};
        float grade_points = atof(grade_point_values[rand() % 5]);
        
        // Create enrollment
        int enrollment_id = db->enrollment_count + 1;
        
        Enrollment *enrollment = &db->enrollments[db->enrollment_count];
        enrollment->id = enrollment_id;
        enrollment->student_id = student_id;
        enrollment->course_id = course_id;
        enrollment->semester = semester;
        
        // Set enrollment date to a recent date
        int days_ago = rand() % 365;
        time_t now = time(NULL);
        struct tm *past_time = localtime(&now);
        time_t past_time = now - (days_ago * 24 * 60 * 60);
        struct tm * enrollment_time = localtime(&past_time);
        
        enrollment->enrollment_date.day = enrollment_time->tm_mday;
        enrollment->enrollment_date.month = enrollment_time->tm_mon + 1;
        enrollment->enrollment_date.year = enrollment_time->tm_year + 1900;
        
        enrollment->grade[0] = grade[grade_points >= 4.0f ? 0 : (grade_points >= 3.0f ? 1 : (grade_points >= 2.0f ? 2 : (grade_points >= 1.0f ? 3 : 4))];
        enrollment->grade[1] = '\0';
        enrollment->grade_points = grade_points;
        enrollment->credits_earned = db->courses[course_id - 1].credits;
        enrollment->completed = rand() % 10 < 2; // 20% chance of being completed
        
        db->enrollment_count++;
    }
}

const char* grade_to_points(const char *grade) {
    switch (grade[0]) {
        case 'A': case 'a': return "4.0";
        case 'B': case 'b': return "3.0";
        case 'C': case 'c': return "2.0";
        case 'D': case 'd': return "1.0";
        case 'F': case 'f': return "0.0";
        default: return "0.0";
    }
}