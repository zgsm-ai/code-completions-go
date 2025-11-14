#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define MAX_STUDENTS 500
#define MAX_COURSES 100
#define MAX_ENROLLMENTS 2000
#define MAX_PROFESSORS 200
#define MAX_DEPARTMENTS 20
#define MAX_ASSIGNMENTS 1000
#define MAX_GRADES 5000
#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 200
#define FILENAME_STUDENTS "students.dat"
#define FILENAME_COURSES "courses.dat"
#define FILENAME_ENROLLMENTS "enrollments.dat"
#define FILENAME_PROFESSORS "professors.dat"
#define FILENAME_DEPARTMENTS "departments.dat"
#define FILENAME_ASSIGNMENTS "assignments.dat"
#define FILENAME_GRADES "grades.dat"

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
    Date enrollment_date;
    int department_id;
    int year_level; // 1-4 for undergraduate, 5-6 for graduate
    int credits_completed;
    double gpa;
    int active;
    char major[50];
    char minor[50];
} Student;

typedef struct {
    int id;
    char course_code[20];
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    int department_id;
    int credits;
    int max_capacity;
    int current_enrollment;
    int active;
    char semester[20]; // "Fall", "Spring", "Summer", "Winter"
    int year;
    char schedule[100]; // e.g., "Mon, Wed, Fri 10:00-11:00"
    char location[50];
    int professor_id;
} Course;

typedef struct {
    int id;
    int student_id;
    int course_id;
    Date enrollment_date;
    char status[20]; // "Active", "Completed", "Dropped", "Withdrawn"
    int final_grade; // 0-100 or -1 if not graded
    double letter_grade_points; // 4.0 scale
    int active;
} Enrollment;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char office[30];
    int department_id;
    char title[30]; // "Professor", "Associate Professor", "Assistant Professor", "Lecturer"
    int hire_year;
    double salary;
    int active;
} Professor;

typedef struct {
    int id;
    char name[50];
    char description[MAX_DESCRIPTION_LENGTH];
    int active;
} Department;

typedef struct {
    int id;
    int course_id;
    char title[100];
    char description[MAX_DESCRIPTION_LENGTH];
    Date assigned_date;
    Date due_date;
    int max_points;
    int active;
} Assignment;

typedef struct {
    int id;
    int enrollment_id;
    int assignment_id;
    int points_earned;
    int max_points;
    Date submission_date;
    char feedback[200];
    int active;
} Grade;

typedef struct {
    Student students[MAX_STUDENTS];
    int student_count;
    Course courses[MAX_COURSES];
    int course_count;
    Enrollment enrollments[MAX_ENROLLMENTS];
    int enrollment_count;
    Professor professors[MAX_PROFESSORS];
    int professor_count;
    Department departments[MAX_DEPARTMENTS];
    int department_count;
    Assignment assignments[MAX_ASSIGNMENTS];
    int assignment_count;
    Grade grades[MAX_GRADES];
    int grade_count;
} UniversityDatabase;

// Function prototypes
void initialize_database(UniversityDatabase *db);
int add_student(UniversityDatabase *db, const char *first_name, const char *last_name,
                const char *email, const char *phone, const char *address,
                Date birth_date, int department_id, int year_level, const char *major, const char *minor);
int add_course(UniversityDatabase *db, const char *course_code, const char *name,
              const char *description, int department_id, int credits, int max_capacity,
              const char *semester, int year, const char *schedule, const char *location, int professor_id);
int enroll_student(UniversityDatabase *db, int student_id, int course_id);
int add_professor(UniversityDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *office,
                 int department_id, const char *title, int hire_year, double salary);
int add_department(UniversityDatabase *db, const char *name, const char *description);
int add_assignment(UniversityDatabase *db, int course_id, const char *title,
                 const char *description, Date due_date, int max_points);
int add_grade(UniversityDatabase *db, int enrollment_id, int assignment_id,
             int points_earned, const char *feedback);
void drop_course(UniversityDatabase *db, int student_id, int course_id);
void withdraw_student(UniversityDatabase *db, int student_id);
void update_student_gpa(UniversityDatabase *db, int student_id);
void calculate_course_average(UniversityDatabase *db, int course_id);
void display_student(const Student *student, const UniversityDatabase *db);
void display_course(const Course *course, const UniversityDatabase *db);
void display_professor(const Professor *professor, const UniversityDatabase *db);
void display_department(const Department *department);
void display_assignment(const Assignment *assignment, const UniversityDatabase *db);
void display_grade(const Grade *grade, const UniversityDatabase *db);
void display_enrollment(const Enrollment *enrollment, const UniversityDatabase *db);
void display_all_students(const UniversityDatabase *db);
void display_all_courses(const UniversityDatabase *db);
void display_all_professors(const UniversityDatabase *db);
void display_all_departments(const UniversityDatabase *db);
void display_all_assignments(const UniversityDatabase *db);
void display_all_grades(const UniversityDatabase *db);
void search_students_by_name(const UniversityDatabase *db, const char *name);
void search_courses_by_department(const UniversityDatabase *db, int department_id);
void search_courses_by_semester(const UniversityDatabase *db, const char *semester, int year);
void generate_student_report(const UniversityDatabase *db, int student_id);
void generate_course_report(const UniversityDatabase *db, int course_id);
void generate_professor_report(const UniversityDatabase *db, int professor_id);
void generate_department_report(const UniversityDatabase *db, int department_id);
void generate_academic_performance_report(const UniversityDatabase *db);
void generate_enrollment_statistics(const UniversityDatabase *db);
int is_valid_email(const char *email);
int is_valid_phone(const char *phone);
int is_valid_date(int day, int month, int year);
int calculate_letter_grade_points(int grade);
const char* get_letter_grade(int grade);
int find_student_by_id(const UniversityDatabase *db, int student_id);
int find_course_by_id(const UniversityDatabase *db, int course_id);
int find_professor_by_id(const UniversityDatabase *db, int professor_id);
int find_department_by_id(const UniversityDatabase *db, int department_id);
int find_enrollment_by_student_course(const UniversityDatabase *db, int student_id, int course_id);
int find_assignment_by_id(const UniversityDatabase *db, int assignment_id);
int find_grade_by_enrollment_assignment(const UniversityDatabase *db, int enrollment_id, int assignment_id);
int save_database(const UniversityDatabase *db);
int load_database(UniversityDatabase *db);
void generate_sample_data(UniversityDatabase *db);

int main() {
    UniversityDatabase db;
    initialize_database(&db);
    
    printf("University Management System\n");
    printf("1. Add Student\n");
    printf("2. Add Course\n");
    printf("3. Enroll Student in Course\n");
    printf("4. Add Professor\n");
    printf("5. Add Department\n");
    printf("6. Add Assignment\n");
    printf("7. Add Grade\n");
    printf("8. Drop Course\n");
    printf("9. Withdraw Student\n");
    printf("10. Update Student GPA\n");
    printf("11. Calculate Course Average\n");
    printf("12. Display All Students\n");
    printf("13. Display All Courses\n");
    printf("14. Display All Professors\n");
    printf("15. Display All Departments\n");
    printf("16. Display All Assignments\n");
    printf("17. Display All Grades\n");
    printf("18. Search Students by Name\n");
    printf("19. Search Courses by Department\n");
    printf("20. Search Courses by Semester\n");
    printf("21. Generate Student Report\n");
    printf("22. Generate Course Report\n");
    printf("23. Generate Professor Report\n");
    printf("24. Generate Department Report\n");
    printf("25. Generate Academic Performance Report\n");
    printf("26. Generate Enrollment Statistics\n");
    printf("27. Save Database\n");
    printf("28. Load Database\n");
    printf("29. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100], major[50], minor[50];
                int day, month, year, department_id, year_level;
                
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
                printf("Enter year level (1-6): ");
                scanf("%d", &year_level);
                printf("Enter major: ");
                scanf(" %[^\n]", major);
                printf("Enter minor (optional): ");
                scanf(" %[^\n]", minor);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                if (year_level < 1 || year_level > 6) {
                    printf("Invalid year level. Must be between 1 and 6.\n");
                    break;
                }
                
                Date birth_date = {day, month, year};
                
                int student_id = add_student(&db, first_name, last_name, email, phone,
                                           address, birth_date, department_id,
                                           year_level, major, minor);
                if (student_id != -1) {
                    printf("Student added with ID: %d\n", student_id);
                } else {
                    printf("Failed to add student. Database is full.\n");
                }
                break;
            }
            case 2: {
                char course_code[20], name[MAX_NAME_LENGTH], description[MAX_DESCRIPTION_LENGTH];
                char semester[20], schedule[100], location[50];
                int department_id, credits, max_capacity, year, professor_id;
                
                printf("Enter course code: ");
                scanf("%s", course_code);
                printf("Enter course name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                printf("Enter credits: ");
                scanf("%d", &credits);
                printf("Enter maximum capacity: ");
                scanf("%d", &max_capacity);
                printf("Enter semester (Fall, Spring, Summer, Winter): ");
                scanf(" %[^\n]", semester);
                printf("Enter year: ");
                scanf("%d", &year);
                printf("Enter schedule: ");
                scanf(" %[^\n]", schedule);
                printf("Enter location: ");
                scanf(" %[^\n]", location);
                printf("Enter professor ID: ");
                scanf("%d", &professor_id);
                
                if (credits < 1 || credits > 6) {
                    printf("Invalid credits. Must be between 1 and 6.\n");
                    break;
                }
                
                if (max_capacity < 1 || max_capacity > 500) {
                    printf("Invalid capacity. Must be between 1 and 500.\n");
                    break;
                }
                
                int course_id = add_course(&db, course_code, name, description,
                                        department_id, credits, max_capacity,
                                        semester, year, schedule, location, professor_id);
                if (course_id != -1) {
                    printf("Course added with ID: %d\n", course_id);
                } else {
                    printf("Failed to add course. Database is full.\n");
                }
                break;
            }
            case 3: {
                int student_id, course_id;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                
                int enrollment_id = enroll_student(&db, student_id, course_id);
                if (enrollment_id != -1) {
                    printf("Student enrolled successfully. Enrollment ID: %d\n", enrollment_id);
                } else {
                    printf("Failed to enroll student. Check student and course IDs, or course capacity.\n");
                }
                break;
            }
            case 4: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], office[30], title[30];
                int department_id, hire_year;
                double salary;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter office: ");
                scanf(" %[^\n]", office);
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                printf("Enter title (Professor, Associate Professor, Assistant Professor, Lecturer): ");
                scanf(" %[^\n]", title);
                printf("Enter hire year: ");
                scanf("%d", &hire_year);
                printf("Enter salary: ");
                scanf("%lf", &salary);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                if (hire_year < 1950 || hire_year > 2030) {
                    printf("Invalid hire year.\n");
                    break;
                }
                
                int professor_id = add_professor(&db, first_name, last_name, email, phone,
                                              office, department_id, title, hire_year, salary);
                if (professor_id != -1) {
                    printf("Professor added with ID: %d\n", professor_id);
                } else {
                    printf("Failed to add professor. Database is full.\n");
                }
                break;
            }
            case 5: {
                char name[50], description[MAX_DESCRIPTION_LENGTH];
                
                printf("Enter department name: ");
                scanf(" %[^\n]", name);
                printf("Enter department description: ");
                scanf(" %[^\n]", description);
                
                int department_id = add_department(&db, name, description);
                if (department_id != -1) {
                    printf("Department added with ID: %d\n", department_id);
                } else {
                    printf("Failed to add department. Database is full.\n");
                }
                break;
            }
            case 6: {
                int course_id, max_points;
                int day, month, year;
                char title[100], description[MAX_DESCRIPTION_LENGTH];
                Date due_date;
                
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                printf("Enter assignment title: ");
                scanf(" %[^\n]", title);
                printf("Enter assignment description: ");
                scanf(" %[^\n]", description);
                printf("Enter due date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter maximum points: ");
                scanf("%d", &max_points);
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                due_date.day = day;
                due_date.month = month;
                due_date.year = year;
                
                int assignment_id = add_assignment(&db, course_id, title, description, due_date, max_points);
                if (assignment_id != -1) {
                    printf("Assignment added with ID: %d\n", assignment_id);
                } else {
                    printf("Failed to add assignment. Database is full or course not found.\n");
                }
                break;
            }
            case 7: {
                int enrollment_id, assignment_id, points_earned;
                char feedback[200];
                
                printf("Enter enrollment ID: ");
                scanf("%d", &enrollment_id);
                printf("Enter assignment ID: ");
                scanf("%d", &assignment_id);
                printf("Enter points earned: ");
                scanf("%d", &points_earned);
                printf("Enter feedback: ");
                scanf(" %[^\n]", feedback);
                
                int grade_id = add_grade(&db, enrollment_id, assignment_id, points_earned, feedback);
                if (grade_id != -1) {
                    printf("Grade added with ID: %d\n", grade_id);
                } else {
                    printf("Failed to add grade. Check enrollment and assignment IDs.\n");
                }
                break;
            }
            case 8: {
                int student_id, course_id;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                
                drop_course(&db, student_id, course_id);
                break;
            }
            case 9: {
                int student_id;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                
                withdraw_student(&db, student_id);
                break;
            }
            case 10: {
                int student_id;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                
                update_student_gpa(&db, student_id);
                break;
            }
            case 11: {
                int course_id;
                
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                
                calculate_course_average(&db, course_id);
                break;
            }
            case 12: {
                display_all_students(&db);
                break;
            }
            case 13: {
                display_all_courses(&db);
                break;
            }
            case 14: {
                display_all_professors(&db);
                break;
            }
            case 15: {
                display_all_departments(&db);
                break;
            }
            case 16: {
                display_all_assignments(&db);
                break;
            }
            case 17: {
                display_all_grades(&db);
                break;
            }
            case 18: {
                char name[MAX_NAME_LENGTH];
                printf("Enter student name to search: ");
                scanf(" %[^\n]", name);
                search_students_by_name(&db, name);
                break;
            }
            case 19: {
                int department_id;
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                search_courses_by_department(&db, department_id);
                break;
            }
            case 20: {
                char semester[20];
                int year;
                printf("Enter semester (Fall, Spring, Summer, Winter): ");
                scanf(" %[^\n]", semester);
                printf("Enter year: ");
                scanf("%d", &year);
                search_courses_by_semester(&db, semester, year);
                break;
            }
            case 21: {
                int student_id;
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                generate_student_report(&db, student_id);
                break;
            }
            case 22: {
                int course_id;
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                generate_course_report(&db, course_id);
                break;
            }
            case 23: {
                int professor_id;
                printf("Enter professor ID: ");
                scanf("%d", &professor_id);
                generate_professor_report(&db, professor_id);
                break;
            }
            case 24: {
                int department_id;
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                generate_department_report(&db, department_id);
                break;
            }
            case 25: {
                generate_academic_performance_report(&db);
                break;
            }
            case 26: {
                generate_enrollment_statistics(&db);
                break;
            }
            case 27: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 28: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 29: {
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

void initialize_database(UniversityDatabase *db) {
    <｜fim▁hole｜>db->student_count = 0;
    db->course_count = 0;
    db->enrollment_count = 0;
    db->professor_count = 0;
    db->department_count = 0;
    db->assignment_count = 0;
    db->grade_count = 0;
}

int add_student(UniversityDatabase *db, const char *first_name, const char *last_name,
                const char *email, const char *phone, const char *address,
                Date birth_date, int department_id, int year_level, const char *major, const char *minor) {
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
    
    // Set enrollment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    student->enrollment_date.day = current_time->tm_mday;
    student->enrollment_date.month = current_time->tm_mon + 1;
    student->enrollment_date.year = current_time->tm_year + 1900;
    
    student->department_id = department_id;
    student->year_level = year_level;
    student->credits_completed = 0;
    student->gpa = 0.0;
    student->active = 1;
    strncpy(student->major, major, 49);
    student->major[49] = '\0';
    strncpy(student->minor, minor, 49);
    student->minor[49] = '\0';
    
    db->student_count++;
    return student->id;
}

int add_course(UniversityDatabase *db, const char *course_code, const char *name,
              const char *description, int department_id, int credits, int max_capacity,
              const char *semester, int year, const char *schedule, const char *location, int professor_id) {
    if (db->course_count >= MAX_COURSES) {
        return -1;
    }
    
    Course *course = &db->courses[db->course_count];
    course->id = db->course_count + 1;
    strncpy(course->course_code, course_code, 19);
    course->course_code[19] = '\0';
    strncpy(course->name, name, MAX_NAME_LENGTH - 1);
    course->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(course->description, description, MAX_DESCRIPTION_LENGTH - 1);
    course->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    course->department_id = department_id;
    course->credits = credits;
    course->max_capacity = max_capacity;
    course->current_enrollment = 0;
    course->active = 1;
    strncpy(course->semester, semester, 19);
    course->semester[19] = '\0';
    course->year = year;
    strncpy(course->schedule, schedule, 99);
    course->schedule[99] = '\0';
    strncpy(course->location, location, 49);
    course->location[49] = '\0';
    course->professor_id = professor_id;
    
    db->course_count++;
    return course->id;
}

int enroll_student(UniversityDatabase *db, int student_id, int course_id) {
    // Validate student and course exist
    if (find_student_by_id(db, student_id) == -1 || find_course_by_id(db, course_id) == -1) {
        return -1;
    }
    
    // Check if student is already enrolled
    if (find_enrollment_by_student_course(db, student_id, course_id) != -1) {
        return -1;
    }
    
    // Check course capacity
    int course_index = find_course_by_id(db, course_id);
    if (course_index != -1 && db->courses[course_index].current_enrollment >= db->courses[course_index].max_capacity) {
        return -1;
    }
    
    if (db->enrollment_count >= MAX_ENROLLMENTS) {
        return -1;
    }
    
    Enrollment *enrollment = &db->enrollments[db->enrollment_count];
    enrollment->id = db->enrollment_count + 1;
    enrollment->student_id = student_id;
    enrollment->course_id = course_id;
    
    // Set enrollment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    enrollment->enrollment_date.day = current_time->tm_mday;
    enrollment->enrollment_date.month = current_time->tm_mon + 1;
    enrollment->enrollment_date.year = current_time->tm_year + 1900;
    
    strcpy(enrollment->status, "Active");
    enrollment->final_grade = -1; // Not graded yet
    enrollment->letter_grade_points = 0.0;
    enrollment->active = 1;
    
    // Update course enrollment count
    if (course_index != -1) {
        db->courses[course_index].current_enrollment++;
    }
    
    db->enrollment_count++;
    return enrollment->id;
}

int add_professor(UniversityDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *office,
                 int department_id, const char *title, int hire_year, double salary) {
    if (db->professor_count >= MAX_PROFESSORS) {
        return -1;
    }
    
    Professor *professor = &db->professors[db->professor_count];
    professor->id = db->professor_count + 1;
    strncpy(professor->first_name, first_name, MAX_NAME_LENGTH - 1);
    professor->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(professor->last_name, last_name, MAX_NAME_LENGTH - 1);
    professor->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(professor->email, email, 49);
    professor->email[49] = '\0';
    strncpy(professor->phone, phone, 14);
    professor->phone[14] = '\0';
    strncpy(professor->office, office, 29);
    professor->office[29] = '\0';
    professor->department_id = department_id;
    strncpy(professor->title, title, 29);
    professor->title[29] = '\0';
    professor->hire_year = hire_year;
    professor->salary = salary;
    professor->active = 1;
    
    db->professor_count++;
    return professor->id;
}

int add_department(UniversityDatabase *db, const char *name, const char *description) {
    if (db->department_count >= MAX_DEPARTMENTS) {
        return -1;
    }
    
    Department *department = &db->departments[db->department_count];
    department->id = db->department_count + 1;
    strncpy(department->name, name, 49);
    department->name[49] = '\0';
    strncpy(department->description, description, MAX_DESCRIPTION_LENGTH - 1);
    department->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    department->active = 1;
    
    db->department_count++;
    return department->id;
}

int add_assignment(UniversityDatabase *db, int course_id, const char *title,
                 const char *description, Date due_date, int max_points) {
    // Validate course exists
    if (find_course_by_id(db, course_id) == -1) {
        return -1;
    }
    
    if (db->assignment_count >= MAX_ASSIGNMENTS) {
        return -1;
    }
    
    Assignment *assignment = &db->assignments[db->assignment_count];
    assignment->id = db->assignment_count + 1;
    assignment->course_id = course_id;
    strncpy(assignment->title, title, 99);
    assignment->title[99] = '\0';
    strncpy(assignment->description, description, MAX_DESCRIPTION_LENGTH - 1);
    assignment->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    
    // Set assigned date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    assignment->assigned_date.day = current_time->tm_mday;
    assignment->assigned_date.month = current_time->tm_mon + 1;
    assignment->assigned_date.year = current_time->tm_year + 1900;
    
    assignment->due_date = due_date;
    assignment->max_points = max_points;
    assignment->active = 1;
    
    db->assignment_count++;
    return assignment->id;
}

int add_grade(UniversityDatabase *db, int enrollment_id, int assignment_id,
             int points_earned, const char *feedback) {
    // Validate enrollment and assignment exist
    int enrollment_index = -1;
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].id == enrollment_id && db->enrollments[i].active) {
            enrollment_index = i;
            break;
        }
    }
    
    if (enrollment_index == -1) {
        return -1;
    }
    
    int assignment_index = -1;
    for (int i = 0; i < db->assignment_count; i++) {
        if (db->assignments[i].id == assignment_id && db->assignments[i].active) {
            assignment_index = i;
            break;
        }
    }
    
    if (assignment_index == -1) {
        return -1;
    }
    
    // Check if grade already exists for this enrollment and assignment
    if (find_grade_by_enrollment_assignment(db, enrollment_id, assignment_id) != -1) {
        return -1;
    }
    
    if (db->grade_count >= MAX_GRADES) {
        return -1;
    }
    
    Grade *grade = &db->grades[db->grade_count];
    grade->id = db->grade_count + 1;
    grade->enrollment_id = enrollment_id;
    grade->assignment_id = assignment_id;
    grade->points_earned = points_earned;
    grade->max_points = db->assignments[assignment_index].max_points;
    
    // Set submission date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    grade->submission_date.day = current_time->tm_mday;
    grade->submission_date.month = current_time->tm_mon + 1;
    grade->submission_date.year = current_time->tm_year + 1900;
    
    strncpy(grade->feedback, feedback, 199);
    grade->feedback[199] = '\0';
    grade->active = 1;
    
    db->grade_count++;
    return grade->id;
}

void drop_course(UniversityDatabase *db, int student_id, int course_id) {
    int enrollment_index = find_enrollment_by_student_course(db, student_id, course_id);
    if (enrollment_index == -1) {
        printf("Enrollment not found.\n");
        return;
    }
    
    Enrollment *enrollment = &db->enrollments[enrollment_index];
    
    // Update enrollment status
    strcpy(enrollment->status, "Dropped");
    
    // Update course enrollment count
    int course_index = find_course_by_id(db, course_id);
    if (course_index != -1) {
        db->courses[course_index].current_enrollment--;
    }
    
    printf("Student %d has dropped course %d.\n", student_id, course_id);
}

void withdraw_student(UniversityDatabase *db, int student_id) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    Student *student = &db->students[student_index];
    
    // Update all enrollments for this student
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id && db->enrollments[i].active) {
            strcpy(db->enrollments[i].status, "Withdrawn");
            
            // Update course enrollment count
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1) {
                db->courses[course_index].current_enrollment--;
            }
        }
    }
    
    // Deactivate student
    student->active = 0;
    
    printf("Student %d has withdrawn from the university.\n", student_id);
}

void update_student_gpa(UniversityDatabase *db, int student_id) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    // Calculate GPA based on completed courses
    double total_points = 0.0;
    int total_credits = 0;
    int completed_courses = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id && 
            strcmp(db->enrollments[i].status, "Completed") == 0 &&
            db->enrollments[i].final_grade >= 0) {
            
            // Get course credits
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1) {
                total_points += db->enrollments[i].letter_grade_points * db->courses[course_index].credits;
                total_credits += db->courses[course_index].credits;
                completed_courses++;
            }
        }
    }
    
    Student *student = &db->students[student_index];
    
    if (total_credits > 0) {
        student->gpa = total_points / total_credits;
        student->credits_completed = total_credits;
    } else {
        student->gpa = 0.0;
        student->credits_completed = 0;
    }
    
    printf("GPA updated for student %d.\n", student_id);
    printf("Completed courses: %d\n", completed_courses);
    printf("Total credits: %d\n", total_credits);
    printf("New GPA: %.2f\n", student->gpa);
}

void calculate_course_average(UniversityDatabase *db, int course_id) {
    int course_index = find_course_by_id(db, course_id);
    if (course_index == -1) {
        printf("Course not found.\n");
        return;
    }
    
    // Calculate average grade for this course
    double total_grades = 0.0;
    int graded_students = 0;
    int passed_students = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].course_id == course_id && 
            db->enrollments[i].final_grade >= 0) {
            total_grades += db->enrollments[i].final_grade;
            graded_students++;
            
            if (db->enrollments[i].final_grade >= 60) {
                passed_students++;
            }
        }
    }
    
    Course *course = &db->courses[course_index];
    
    printf("Course Statistics for %s (%s):\n", course->name, course->course_code);
    printf("Total enrolled: %d\n", course->current_enrollment);
    printf("Graded students: %d\n", graded_students);
    
    if (graded_students > 0) {
        double average = total_grades / graded_students;
        double pass_rate = (double)passed_students / graded_students * 100;
        
        printf("Average grade: %.1f\n", average);
        printf("Pass rate: %.1f%%\n", pass_rate);
        
        // Grade distribution
        int a_count = 0, b_count = 0, c_count = 0, d_count = 0, f_count = 0;
        
        for (int i = 0; i < db->enrollment_count; i++) {
            if (db->enrollments[i].course_id == course_id && 
                db->enrollments[i].final_grade >= 0) {
                if (db->enrollments[i].final_grade >= 90) a_count++;
                else if (db->enrollments[i].final_grade >= 80) b_count++;
                else if (db->enrollments[i].final_grade >= 70) c_count++;
                else if (db->enrollments[i].final_grade >= 60) d_count++;
                else f_count++;
            }
        }
        
        printf("Grade distribution:\n");
        printf("A (90-100): %d (%.1f%%)\n", a_count, (double)a_count / graded_students * 100);
        printf("B (80-89): %d (%.1f%%)\n", b_count, (double)b_count / graded_students * 100);
        printf("C (70-79): %d (%.1f%%)\n", c_count, (double)c_count / graded_students * 100);
        printf("D (60-69): %d (%.1f%%)\n", d_count, (double)d_count / graded_students * 100);
        printf("F (0-59): %d (%.1f%%)\n", f_count, (double)f_count / graded_students * 100);
    }
}

void display_student(const Student *student, const UniversityDatabase *db) {
    printf("\nStudent ID: %d\n", student->id);
    printf("Name: %s %s\n", student->first_name, student->last_name);
    printf("Email: %s\n", student->email);
    printf("Phone: %s\n", student->phone);
    printf("Address: %s\n", student->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           student->birth_date.day, student->birth_date.month, student->birth_date.year);
    printf("Enrollment Date: %02d/%02d/%d\n", 
           student->enrollment_date.day, student->enrollment_date.month, student->enrollment_date.year);
    
    // Display department name
    if (student->department_id > 0) {
        int dept_index = student->department_id - 1;
        if (dept_index >= 0 && dept_index < db->department_count) {
            printf("Department: %s\n", db->departments[dept_index].name);
        }
    }
    
    printf("Year Level: %d\n", student->year_level);
    printf("Major: %s\n", student->major);
    if (strlen(student->minor) > 0) {
        printf("Minor: %s\n", student->minor);
    }
    printf("Credits Completed: %d\n", student->credits_completed);
    printf("GPA: %.2f\n", student->gpa);
    printf("Status: %s\n", student->active ? "Active" : "Inactive");
}

void display_course(const Course *course, const UniversityDatabase *db) {
    printf("\nCourse ID: %d\n", course->id);
    printf("Course Code: %s\n", course->course_code);
    printf("Name: %s\n", course->name);
    printf("Description: %s\n", course->description);
    
    // Display department name
    if (course->department_id > 0) {
        int dept_index = course->department_id - 1;
        if (dept_index >= 0 && dept_index < db->department_count) {
            printf("Department: %s\n", db->departments[dept_index].name);
        }
    }
    
    printf("Credits: %d\n", course->credits);
    printf("Capacity: %d/%d\n", course->current_enrollment, course->max_capacity);
    printf("Semester: %s %d\n", course->semester, course->year);
    printf("Schedule: %s\n", course->schedule);
    printf("Location: %s\n", course->location);
    
    // Display professor name
    if (course->professor_id > 0) {
        int prof_index = -1;
        for (int i = 0; i < db->professor_count; i++) {
            if (db->professors[i].id == course->professor_id && db->professors[i].active) {
                prof_index = i;
                break;
            }
        }
        
        if (prof_index != -1) {
            printf("Professor: %s %s\n", 
                   db->professors[prof_index].first_name, 
                   db->professors[prof_index].last_name);
        }
    }
    
    printf("Status: %s\n", course->active ? "Active" : "Inactive");
}

void display_professor(const Professor *professor, const UniversityDatabase *db) {
    printf("\nProfessor ID: %d\n", professor->id);
    printf("Name: %s %s\n", professor->first_name, professor->last_name);
    printf("Email: %s\n", professor->email);
    printf("Phone: %s\n", professor->phone);
    printf("Office: %s\n", professor->office);
    
    // Display department name
    if (professor->department_id > 0) {
        int dept_index = professor->department_id - 1;
        if (dept_index >= 0 && dept_index < db->department_count) {
            printf("Department: %s\n", db->departments[dept_index].name);
        }
    }
    
    printf("Title: %s\n", professor->title);
    printf("Hire Year: %d\n", professor->hire_year);
    printf("Salary: $%.2f\n", professor->salary);
    printf("Status: %s\n", professor->active ? "Active" : "Inactive");
}

void display_department(const Department *department) {
    printf("\nDepartment ID: %d\n", department->id);
    printf("Name: %s\n", department->name);
    printf("Description: %s\n", department->description);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
}

void display_assignment(const Assignment *assignment, const UniversityDatabase *db) {
    printf("\nAssignment ID: %d\n", assignment->id);
    
    // Display course information
    int course_index = -1;
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].id == assignment->course_id && db->courses[i].active) {
            course_index = i;
            break;
        }
    }
    
    if (course_index != -1) {
        printf("Course: %s (%s)\n", 
               db->courses[course_index].name, 
               db->courses[course_index].course_code);
    }
    
    printf("Title: %s\n", assignment->title);
    printf("Description: %s\n", assignment->description);
    printf("Assigned Date: %02d/%02d/%d\n", 
           assignment->assigned_date.day, assignment->assigned_date.month, assignment->assigned_date.year);
    printf("Due Date: %02d/%02d/%d\n", 
           assignment->due_date.day, assignment->due_date.month, assignment->due_date.year);
    printf("Maximum Points: %d\n", assignment->max_points);
    printf("Status: %s\n", assignment->active ? "Active" : "Inactive");
}

void display_grade(const Grade *grade, const UniversityDatabase *db) {
    printf("\nGrade ID: %d\n", grade->id);
    
    // Display student and course information
    int enrollment_index = -1;
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].id == grade->enrollment_id && db->enrollments[i].active) {
            enrollment_index = i;
            break;
        }
    }
    
    if (enrollment_index != -1) {
        int student_index = find_student_by_id(db, db->enrollments[enrollment_index].student_id);
        int course_index = find_course_by_id(db, db->enrollments[enrollment_index].course_id);
        
        if (student_index != -1) {
            printf("Student: %s %s\n", 
                   db->students[student_index].first_name, 
                   db->students[student_index].last_name);
        }
        
        if (course_index != -1) {
            printf("Course: %s (%s)\n", 
                   db->courses[course_index].name, 
                   db->courses[course_index].course_code);
        }
    }
    
    // Display assignment information
    int assignment_index = find_assignment_by_id(db, grade->assignment_id);
    if (assignment_index != -1) {
        printf("Assignment: %s\n", db->assignments[assignment_index].title);
    }
    
    printf("Score: %d/%d (%.1f%%)\n", 
           grade->points_earned, 
           grade->max_points,
           grade->max_points > 0 ? (double)grade->points_earned / grade->max_points * 100 : 0.0);
    
    printf("Letter Grade: %s\n", get_letter_grade(
        grade->max_points > 0 ? (grade->points_earned * 100) / grade->max_points : 0));
    
    printf("Submission Date: %02d/%02d/%d\n", 
           grade->submission_date.day, grade->submission_date.month, grade->submission_date.year);
    printf("Feedback: %s\n", grade->feedback);
    printf("Status: %s\n", grade->active ? "Active" : "Inactive");
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
               db->courses[course_index].name, 
               db->courses[course_index].course_code,
               enrollment->course_id);
    }
    
    printf("Enrollment Date: %02d/%02d/%d\n", 
           enrollment->enrollment_date.day, enrollment->enrollment_date.month, enrollment->enrollment_date.year);
    printf("Status: %s\n", enrollment->status);
    
    if (enrollment->final_grade >= 0) {
        printf("Final Grade: %d (%s)\n", 
               enrollment->final_grade, 
               get_letter_grade(enrollment->final_grade));
        printf("Grade Points: %.1f\n", enrollment->letter_grade_points);
    } else {
        printf("Final Grade: Not graded yet\n");
    }
    
    printf("Status: %s\n", enrollment->active ? "Active" : "Inactive");
}

void display_all_students(const UniversityDatabase *db) {
    printf("\n=== All Students ===\n");
    printf("Total Students: %d\n\n", db->student_count);
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            display_student(&db->students[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_courses(const UniversityDatabase *db) {
    printf("\n=== All Courses ===\n");
    printf("Total Courses: %d\n\n", db->course_count);
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active) {
            display_course(&db->courses[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_professors(const UniversityDatabase *db) {
    printf("\n=== All Professors ===\n");
    printf("Total Professors: %d\n\n", db->professor_count);
    
    for (int i = 0; i < db->professor_count; i++) {
        if (db->professors[i].active) {
            display_professor(&db->professors[i], db);
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

void display_all_assignments(const UniversityDatabase *db) {
    printf("\n=== All Assignments ===\n");
    printf("Total Assignments: %d\n\n", db->assignment_count);
    
    for (int i = 0; i < db->assignment_count; i++) {
        if (db->assignments[i].active) {
            display_assignment(&db->assignments[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_grades(const UniversityDatabase *db) {
    printf("\n=== All Grades ===\n");
    printf("Total Grades: %d\n\n", db->grade_count);
    
    for (int i = 0; i < db->grade_count; i++) {
        if (db->grades[i].active) {
            display_grade(&db->grades[i], db);
            printf("------------------------\n");
        }
    }
}

void search_students_by_name(const UniversityDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active && 
            (strstr(db->students[i].first_name, name) != NULL || 
             strstr(db->students[i].last_name, name) != NULL)) {
            display_student(&db->students[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No students found with name containing '%s'.\n", name);
    }
}

void search_courses_by_department(const UniversityDatabase *db, int department_id) {
    printf("\nCourses in Department ID: %d\n", department_id);
    int found = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active && db->courses[i].department_id == department_id) {
            display_course(&db->courses[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No courses found in this department.\n");
    }
}

void search_courses_by_semester(const UniversityDatabase *db, const char *semester, int year) {
    printf("\nCourses in %s %d:\n", semester, year);
    int found = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active && 
            strcmp(db->courses[i].semester, semester) == 0 && 
            db->courses[i].year == year) {
            display_course(&db->courses[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No courses found for this semester.\n");
    }
}

void generate_student_report(const UniversityDatabase *db, int student_id) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    Student *student = &db->students[student_index];
    
    printf("\n=== Student Report: %s %s ===\n", student->first_name, student->last_name);
    printf("Student ID: %d\n", student->id);
    printf("Email: %s\n", student->email);
    printf("Phone: %s\n", student->phone);
    printf("Address: %s\n", student->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           student->birth_date.day, student->birth_date.month, student->birth_date.year);
    printf("Enrollment Date: %02d/%02d/%d\n", 
           student->enrollment_date.day, student->enrollment_date.month, student->enrollment_date.year);
    
    // Display department name
    if (student->department_id > 0) {
        int dept_index = student->department_id - 1;
        if (dept_index >= 0 && dept_index < db->department_count) {
            printf("Department: %s\n", db->departments[dept_index].name);
        }
    }
    
    printf("Year Level: %d\n", student->year_level);
    printf("Major: %s\n", student->major);
    if (strlen(student->minor) > 0) {
        printf("Minor: %s\n", student->minor);
    }
    printf("Credits Completed: %d\n", student->credits_completed);
    printf("GPA: %.2f\n", student->gpa);
    printf("Status: %s\n", student->active ? "Active" : "Inactive");
    
    // Course enrollments
    printf("\nCourse Enrollments:\n");
    printf("%-10s %-15s %-10s %-15s %-10s\n", 
           "Course", "Name", "Credits", "Status", "Grade");
    printf("------------------------------------------------\n");
    
    int current_enrollments = 0;
    int completed_courses = 0;
    int total_credits = 0;
    double total_grade_points = 0.0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id) {
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1) {
                printf("%-10s %-15s %-10d %-15s ", 
                       db->courses[course_index].course_code,
                       db->courses[course_index].name,
                       db->courses[course_index].credits,
                       db->enrollments[i].status);
                
                if (db->enrollments[i].final_grade >= 0) {
                    printf("%-10d\n", db->enrollments[i].final_grade);
                    total_credits += db->courses[course_index].credits;
                    total_grade_points += db->enrollments[i].letter_grade_points * db->courses[course_index].credits;
                    completed_courses++;
                } else {
                    printf("N/A\n");
                }
                
                if (strcmp(db->enrollments[i].status, "Active") == 0) {
                    current_enrollments++;
                }
            }
        }
    }
    
    printf("------------------------------------------------\n");
    printf("Current Enrollments: %d\n", current_enrollments);
    printf("Completed Courses: %d\n", completed_courses);
    printf("Total Credits: %d\n", total_credits);
    
    if (total_credits > 0) {
        printf("Calculated GPA: %.2f\n", total_grade_points / total_credits);
    }
    
    // Assignment grades
    printf("\nRecent Assignments:\n");
    printf("%-15s %-30s %-15s %-10s\n", 
           "Course", "Assignment", "Submission Date", "Score");
    printf("---------------------------------------------------------------\n");
    
    int assignment_count = 0;
    
    for (int i = 0; i < db->grade_count; i++) {
        int enrollment_index = -1;
        for (int j = 0; j < db->enrollment_count; j++) {
            if (db->enrollments[j].id == db->grades[i].enrollment_id && 
                db->enrollments[j].student_id == student_id) {
                enrollment_index = j;
                break;
            }
        }
        
        if (enrollment_index != -1) {
            int course_index = find_course_by_id(db, db->enrollments[enrollment_index].course_id);
            int assignment_index = find_assignment_by_id(db, db->grades[i].assignment_id);
            
            if (course_index != -1 && assignment_index != -1) {
                printf("%-15s %-30s %02d/%02d/%d %-10d/%d\n", 
                       db->courses[course_index].course_code,
                       db->assignments[assignment_index].title,
                       db->grades[i].submission_date.day, 
                       db->grades[i].submission_date.month, 
                       db->grades[i].submission_date.year,
                       db->grades[i].points_earned, 
                       db->grades[i].max_points);
                assignment_count++;
            }
        }
    }
    
    if (assignment_count == 0) {
        printf("No assignments found.\n");
    }
}

void generate_course_report(const UniversityDatabase *db, int course_id) {
    int course_index = find_course_by_id(db, course_id);
    if (course_index == -1) {
        printf("Course not found.\n");
        return;
    }
    
    Course *course = &db->courses[course_index];
    
    printf("\n=== Course Report: %s ===\n", course->name);
    printf("Course ID: %d\n", course->id);
    printf("Course Code: %s\n", course->course_code);
    printf("Description: %s\n", course->description);
    
    // Display department name
    if (course->department_id > 0) {
        int dept_index = course->department_id - 1;
        if (dept_index >= 0 && dept_index < db->department_count) {
            printf("Department: %s\n", db->departments[dept_index].name);
        }
    }
    
    printf("Credits: %d\n", course->credits);
    printf("Capacity: %d/%d\n", course->current_enrollment, course->max_capacity);
    printf("Semester: %s %d\n", course->semester, course->year);
    printf("Schedule: %s\n", course->schedule);
    printf("Location: %s\n", course->location);
    
    // Display professor name
    if (course->professor_id > 0) {
        int prof_index = -1;
        for (int i = 0; i < db->professor_count; i++) {
            if (db->professors[i].id == course->professor_id && db->professors[i].active) {
                prof_index = i;
                break;
            }
        }
        
        if (prof_index != -1) {
            printf("Professor: %s %s\n", 
                   db->professors[prof_index].first_name, 
                   db->professors[prof_index].last_name);
        }
    }
    
    printf("Status: %s\n", course->active ? "Active" : "Inactive");
    
    // Student enrollments
    printf("\nStudent Enrollments:\n");
    printf("%-10s %-20s %-15s %-10s\n", 
           "Student ID", "Name", "Status", "Grade");
    printf("--------------------------------------------------\n");
    
    int active_students = 0;
    int completed_students = 0;
    int dropped_students = 0;
    double total_grades = 0.0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].course_id == course_id) {
            int student_index = find_student_by_id(db, db->enrollments[i].student_id);
            if (student_index != -1) {
                printf("%-10d %-20s %-15s ", 
                       db->enrollments[i].student_id,
                       db->students[student_index].first_name,
                       db->students[student_index].last_name,
                       db->enrollments[i].status);
                
                if (db->enrollments[i].final_grade >= 0) {
                    printf("%-10d\n", db->enrollments[i].final_grade);
                    total_grades += db->enrollments[i].final_grade;
                    completed_students++;
                } else {
                    printf("N/A\n");
                }
                
                if (strcmp(db->enrollments[i].status, "Active") == 0) {
                    active_students++;
                } else if (strcmp(db->enrollments[i].status, "Dropped") == 0) {
                    dropped_students++;
                }
            }
        }
    }
    
    printf("--------------------------------------------------\n");
    printf("Active Students: %d\n", active_students);
    printf("Completed Students: %d\n", completed_students);
    printf("Dropped Students: %d\n", dropped_students);
    
    if (completed_students > 0) {
        printf("Average Grade: %.1f\n", total_grades / completed_students);
        
        // Grade distribution
        int a_count = 0, b_count = 0, c_count = 0, d_count = 0, f_count = 0;
        
        for (int i = 0; i < db->enrollment_count; i++) {
            if (db->enrollments[i].course_id == course_id && 
                db->enrollments[i].final_grade >= 0) {
                if (db->enrollments[i].final_grade >= 90) a_count++;
                else if (db->enrollments[i].final_grade >= 80) b_count++;
                else if (db->enrollments[i].final_grade >= 70) c_count++;
                else if (db->enrollments[i].final_grade >= 60) d_count++;
                else f_count++;
            }
        }
        
        printf("Grade Distribution:\n");
        printf("A (90-100): %d (%.1f%%)\n", a_count, (double)a_count / completed_students * 100);
        printf("B (80-89): %d (%.1f%%)\n", b_count, (double)b_count / completed_students * 100);
        printf("C (70-79): %d (%.1f%%)\n", c_count, (double)c_count / completed_students * 100);
        printf("D (60-69): %d (%.1f%%)\n", d_count, (double)d_count / completed_students * 100);
        printf("F (0-59): %d (%.1f%%)\n", f_count, (double)f_count / completed_students * 100);
    }
    
    // Course assignments
    printf("\nCourse Assignments:\n");
    printf("%-10s %-30s %-15s %-10s\n", 
           "ID", "Title", "Due Date", "Max Points");
    printf("--------------------------------------------------\n");
    
    int assignment_count = 0;
    
    for (int i = 0; i < db->assignment_count; i++) {
        if (db->assignments[i].course_id == course_id) {
            printf("%-10d %-30s %02d/%02d/%d %-10d\n", 
                   db->assignments[i].id,
                   db->assignments[i].title,
                   db->assignments[i].due_date.day, 
                   db->assignments[i].due_date.month, 
                   db->assignments[i].due_date.year,
                   db->assignments[i].max_points);
            assignment_count++;
        }
    }
    
    if (assignment_count == 0) {
        printf("No assignments found for this course.\n");
    }
}

void generate_professor_report(const UniversityDatabase *db, int professor_id) {
    int professor_index = find_professor_by_id(db, professor_id);
    if (professor_index == -1) {
        printf("Professor not found.\n");
        return;
    }
    
    Professor *professor = &db->professors[professor_index];
    
    printf("\n=== Professor Report: %s %s ===\n", professor->first_name, professor->last_name);
    printf("Professor ID: %d\n", professor->id);
    printf("Email: %s\n", professor->email);
    printf("Phone: %s\n", professor->phone);
    printf("Office: %s\n", professor->office);
    
    // Display department name
    if (professor->department_id > 0) {
        int dept_index = professor->department_id - 1;
        if (dept_index >= 0 && dept_index < db->department_count) {
            printf("Department: %s\n", db->departments[dept_index].name);
        }
    }
    
    printf("Title: %s\n", professor->title);
    printf("Hire Year: %d\n", professor->hire_year);
    printf("Years of Service: %d\n", 2023 - professor->hire_year);
    printf("Salary: $%.2f\n", professor->salary);
    printf("Status: %s\n", professor->active ? "Active" : "Inactive");
    
    // Courses taught
    printf("\nCourses Taught:\n");
    printf("%-10s %-20s %-15s %-10s %-10s\n", 
           "Course ID", "Name", "Semester", "Enrollment", "Average");
    printf("-----------------------------------------------------------\n");
    
    int courses_taught = 0;
    int total_students = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].professor_id == professor_id && db->courses[i].active) {
            printf("%-10d %-20s %-15s %-10d ", 
                   db->courses[i].id,
                   db->courses[i].name,
                   db->courses[i].semester,
                   db->courses[i].current_enrollment);
            
            // Calculate average grade for this course
            double total_grades = 0.0;
            int graded_students = 0;
            
            for (int j = 0; j < db->enrollment_count; j++) {
                if (db->enrollments[j].course_id == db->courses[i].id && 
                    db->enrollments[j].final_grade >= 0) {
                    total_grades += db->enrollments[j].final_grade;
                    graded_students++;
                }
            }
            
            if (graded_students > 0) {
                printf("%-10.1f\n", total_grades / graded_students);
            } else {
                printf("%-10s\n", "N/A");
            }
            
            courses_taught++;
            total_students += db->courses[i].current_enrollment;
        }
    }
    
    printf("-----------------------------------------------------------\n");
    printf("Total Courses Taught: %d\n", courses_taught);
    printf("Total Students Taught: %d\n", total_students);
    
    if (courses_taught > 0) {
        printf("Average Students per Course: %.1f\n", (float)total_students / courses_taught);
    }
}

void generate_department_report(const UniversityDatabase *db, int department_id) {
    int department_index = department_id - 1;
    if (department_index < 0 || department_index >= db->department_count || 
        !db->departments[department_index].active) {
        printf("Department not found.\n");
        return;
    }
    
    Department *department = &db->departments[department_index];
    
    printf("\n=== Department Report: %s ===\n", department->name);
    printf("Department ID: %d\n", department->id);
    printf("Description: %s\n", department->description);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
    
    // Professors in department
    printf("\nProfessors:\n");
    printf("%-10s %-20s %-15s %-10s\n", 
           "ID", "Name", "Title", "Hire Year");
    printf("------------------------------------------------\n");
    
    int professor_count = 0;
    double total_salary = 0.0;
    
    for (int i = 0; i < db->professor_count; i++) {
        if (db->professors[i].active && db->professors[i].department_id == department_id) {
            printf("%-10d %-20s %-15s %-10d\n", 
                   db->professors[i].id,
                   db->professors[i].first_name,
                   db->professors[i].last_name,
                   db->professors[i].title,
                   db->professors[i].hire_year);
            
            professor_count++;
            total_salary += db->professors[i].salary;
        }
    }
    
    printf("------------------------------------------------\n");
    printf("Total Professors: %d\n", professor_count);
    
    if (professor_count > 0) {
        printf("Total Salary Budget: $%.2f\n", total_salary);
        printf("Average Salary: $%.2f\n", total_salary / professor_count);
    }
    
    // Students in department
    printf("\nStudents:\n");
    printf("Year Level:   %d\n", 1);  // Year 1
    printf("Year Level:   %d\n", 2);  // Year 2
    printf("Year Level:   %d\n", 3);  // Year 3
    printf("Year Level:   %d\n", 4);  // Year 4
    printf("Year Level:   %d\n", 5);  // Graduate
    printf("Year Level:   %d\n", 6);  // Graduate
    
    int student_count = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active && db->students[i].department_id == department_id) {
            student_count++;
        }
    }
    
    printf("\nTotal Students: %d\n", student_count);
    
    // Courses offered
    printf("\nCourses Offered:\n");
    printf("%-10s %-20s %-15s %-10s\n", 
           "ID", "Name", "Semester", "Enrollment");
    printf("------------------------------------------------\n");
    
    int course_count = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active && db->courses[i].department_id == department_id) {
            printf("%-10d %-20s %-15s %-10d\n", 
                   db->courses[i].id,
                   db->courses[i].name,
                   db->courses[i].semester,
                   db->courses[i].current_enrollment);
            
            course_count++;
        }
    }
    
    printf("------------------------------------------------\n");
    printf("Total Courses: %d\n", course_count);
}

void generate_academic_performance_report(const UniversityDatabase *db) {
    printf("\n=== Academic Performance Report ===\n");
    
    // Overall student statistics
    printf("\nStudent Statistics:\n");
    
    int active_students = 0;
    int undergraduate_students = 0;
    int graduate_students = 0;
    double total_gpa = 0.0;
    int students_with_gpa = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            active_students++;
            
            if (db->students[i].year_level <= 4) {
                undergraduate_students++;
            } else {
                graduate_students++;
            }
            
            if (db->students[i].gpa > 0) {
                total_gpa += db->students[i].gpa;
                students_with_gpa++;
            }
        }
    }
    
    printf("Total Active Students: %d\n", active_students);
    printf("Undergraduate Students: %d\n", undergraduate_students);
    printf("Graduate Students: %d\n", graduate_students);
    
    if (students_with_gpa > 0) {
        printf("Average GPA: %.2f\n", total_gpa / students_with_gpa);
    }
    
    // GPA distribution
    printf("\nGPA Distribution:\n");
    int gpa_4_0 = 0, gpa_3_0_3_9 = 0, gpa_2_0_2_9 = 0, gpa_1_0_1_9 = 0, gpa_below_1_0 = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active && db->students[i].gpa > 0) {
            if (db->students[i].gpa >= 3.7) gpa_4_0++;
            else if (db->students[i].gpa >= 3.0) gpa_3_0_3_9++;
            else if (db->students[i].gpa >= 2.0) gpa_2_0_2_9++;
            else if (db->students[i].gpa >= 1.0) gpa_1_0_1_9++;
            else gpa_below_1_0++;
        }
    }
    
    printf("3.7-4.0: %d (%.1f%%)\n", gpa_4_0, students_with_gpa > 0 ? (double)gpa_4_0 / students_with_gpa * 100 : 0.0);
    printf("3.0-3.6: %d (%.1f%%)\n", gpa_3_0_3_9, students_with_gpa > 0 ? (double)gpa_3_0_3_9 / students_with_gpa * 100 : 0.0);
    printf("2.0-2.9: %d (%.1f%%)\n", gpa_2_0_2_9, students_with_gpa > 0 ? (double)gpa_2_0_2_9 / students_with_gpa * 100 : 0.0);
    printf("1.0-1.9: %d (%.1f%%)\n", gpa_1_0_1_9, students_with_gpa > 0 ? (double)gpa_1_0_1_9 / students_with_gpa * 100 : 0.0);
    printf("Below 1.0: %d (%.1f%%)\n", gpa_below_1_0, students_with_gpa > 0 ? (double)gpa_below_1_0 / students_with_gpa * 100 : 0.0);
    
    // Course statistics
    printf("\nCourse Statistics:\n");
    
    int active_courses = 0;
    int total_enrollment = 0;
    int total_capacity = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active) {
            active_courses++;
            total_enrollment += db->courses[i].current_enrollment;
            total_capacity += db->courses[i].max_capacity;
        }
    }
    
    printf("Total Active Courses: %d\n", active_courses);
    printf("Total Enrollment: %d\n", total_enrollment);
    printf("Total Capacity: %d\n", total_capacity);
    
    if (total_capacity > 0) {
        printf("Average Enrollment Rate: %.1f%%\n", (double)total_enrollment / total_capacity * 100);
    }
    
    // Department statistics
    printf("\nDepartment Statistics:\n");
    
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].active) {
            int dept_students = 0;
            int dept_courses = 0;
            int dept_professors = 0;
            
            for (int j = 0; j < db->student_count; j++) {
                if (db->students[j].active && db->students[j].department_id == db->departments[i].id) {
                    dept_students++;
                }
            }
            
            for (int j = 0; j < db->course_count; j++) {
                if (db->courses[j].active && db->courses[j].department_id == db->departments[i].id) {
                    dept_courses++;
                }
            }
            
            for (int j = 0; j < db->professor_count; j++) {
                if (db->professors[j].active && db->professors[j].department_id == db->departments[i].id) {
                    dept_professors++;
                }
            }
            
            printf("%s: %d students, %d courses, %d professors\n", 
                   db->departments[i].name, dept_students, dept_courses, dept_professors);
        }
    }
}

void generate_enrollment_statistics(const UniversityDatabase *db) {
    printf("\n=== Enrollment Statistics ===\n");
    
    // Enrollment by semester
    printf("\nEnrollment by Semester:\n");
    printf("%-15s %-10s\n", "Semester", "Enrollments");
    printf("----------------------------\n");
    
    int fall_enrollments = 0, spring_enrollments = 0, summer_enrollments = 0, winter_enrollments = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].active) {
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1) {
                if (strcmp(db->courses[course_index].semester, "Fall") == 0) {
                    fall_enrollments++;
                } else if (strcmp(db->courses[course_index].semester, "Spring") == 0) {
                    spring_enrollments++;
                } else if (strcmp(db->courses[course_index].semester, "Summer") == 0) {
                    summer_enrollments++;
                } else if (strcmp(db->courses[course_index].semester, "Winter") == 0) {
                    winter_enrollments++;
                }
            }
        }
    }
    
    printf("%-15s %-10d\n", "Fall", fall_enrollments);
    printf("%-15s %-10d\n", "Spring", spring_enrollments);
    printf("%-15s %-10d\n", "Summer", summer_enrollments);
    printf("%-15s %-10d\n", "Winter", winter_enrollments);
    
    // Enrollment by year level
    printf("\nEnrollment by Year Level:\n");
    printf("%-15s %-10s\n", "Year Level", "Enrollments");
    printf("----------------------------\n");
    
    int year1_enrollments = 0, year2_enrollments = 0, year3_enrollments = 0;
    int year4_enrollments = 0, year5_enrollments = 0, year6_enrollments = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].active) {
            int student_index = find_student_by_id(db, db->enrollments[i].student_id);
            if (student_index != -1) {
                switch (db->students[student_index].year_level) {
                    case 1: year1_enrollments++; break;
                    case 2: year2_enrollments++; break;
                    case 3: year3_enrollments++; break;
                    case 4: year4_enrollments++; break;
                    case 5: year5_enrollments++; break;
                    case 6: year6_enrollments++; break;
                }
            }
        }
    }
    
    printf("%-15s %-10d\n", "Year 1", year1_enrollments);
    printf("%-15s %-10d\n", "Year 2", year2_enrollments);
    printf("%-15s %-10d\n", "Year 3", year3_enrollments);
    printf("%-15s %-10d\n", "Year 4", year4_enrollments);
    printf("%-15s %-10d\n", "Year 5", year5_enrollments);
    printf("%-15s %-10d\n", "Year 6", year6_enrollments);
    
    // Enrollment status
    printf("\nEnrollment Status:\n");
    int active_enrollments = 0, completed_enrollments = 0, dropped_enrollments = 0, withdrawn_enrollments = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].active) {
            if (strcmp(db->enrollments[i].status, "Active") == 0) {
                active_enrollments++;
            } else if (strcmp(db->enrollments[i].status, "Completed") == 0) {
                completed_enrollments++;
            } else if (strcmp(db->enrollments[i].status, "Dropped") == 0) {
                dropped_enrollments++;
            } else if (strcmp(db->enrollments[i].status, "Withdrawn") == 0) {
                withdrawn_enrollments++;
            }
        }
    }
    
    printf("Active: %d\n", active_enrollments);
    printf("Completed: %d\n", completed_enrollments);
    printf("Dropped: %d\n", dropped_enrollments);
    printf("Withdrawn: %d\n", withdrawn_enrollments);
    
    // Course popularity
    printf("\nCourse Popularity:\n");
    printf("%-10s %-20s %-10s %-10s\n", 
           "Course ID", "Name", "Capacity", "Enrollment");
    printf("------------------------------------------------\n");
    
    // Sort courses by enrollment (simple bubble sort for demonstration)
    int sorted_indices[MAX_COURSES];
    int sorted_count = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active) {
            sorted_indices[sorted_count++] = i;
        }
    }
    
    // Simple sort by enrollment (descending)
    for (int i = 0; i < sorted_count - 1; i++) {
        for (int j = i + 1; j < sorted_count; j++) {
            if (db->courses[sorted_indices[j]].current_enrollment > 
                db->courses[sorted_indices[i]].current_enrollment) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < sorted_count && i < 10; i++) { // Top 10 courses
        int idx = sorted_indices[i];
        printf("%-10d %-20s %-10d %-10d\n", 
               db->courses[idx].id,
               db->courses[idx].name,
               db->courses[idx].max_capacity,
               db->courses[idx].current_enrollment);
    }
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

int is_valid_phone(const char *phone) {
    if (phone == NULL) return 0;
    
    // Simple validation: check if phone starts with valid prefix and has enough digits
    return (strncmp(phone, "555-", 4) == 0 || 
            strncmp(phone, "(", 1) == 0 || 
            strncmp(phone, "+", 1) == 0);
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

int calculate_letter_grade_points(int grade) {
    if (grade >= 90) return 4;
    if (grade >= 85) return 37;
    if (grade >= 80) return 33;
    if (grade >= 75) return 30;
    if (grade >= 70) return 27;
    if (grade >= 65) return 23;
    if (grade >= 60) return 20;
    if (grade >= 55) return 17;
    if (grade >= 50) return 13;
    if (grade >= 45) return 10;
    if (grade >= 40) return 7;
    return 0;
}

const char* get_letter_grade(int grade) {
    if (grade >= 90) return "A";
    if (grade >= 85) return "A-";
    if (grade >= 80) return "B+";
    if (grade >= 75) return "B";
    if (grade >= 70) return "B-";
    if (grade >= 65) return "C+";
    if (grade >= 60) return "C";
    if (grade >= 55) return "C-";
    if (grade >= 50) return "D+";
    if (grade >= 45) return "D";
    if (grade >= 40) return "D-";
    return "F";
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

int find_professor_by_id(const UniversityDatabase *db, int professor_id) {
    for (int i = 0; i < db->professor_count; i++) {
        if (db->professors[i].id == professor_id && db->professors[i].active) {
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

int find_enrollment_by_student_course(const UniversityDatabase *db, int student_id, int course_id) {
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id && 
            db->enrollments[i].course_id == course_id && 
            db->enrollments[i].active) {
            return i;
        }
    }
    return -1;
}

int find_assignment_by_id(const UniversityDatabase *db, int assignment_id) {
    for (int i = 0; i < db->assignment_count; i++) {
        if (db->assignments[i].id == assignment_id && db->assignments[i].active) {
            return i;
        }
    }
    return -1;
}

int find_grade_by_enrollment_assignment(const UniversityDatabase *db, int enrollment_id, int assignment_id) {
    for (int i = 0; i < db->grade_count; i++) {
        if (db->grades[i].enrollment_id == enrollment_id && 
            db->grades[i].assignment_id == assignment_id && 
            db->grades[i].active) {
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
    
    FILE *professors_file = fopen(FILENAME_PROFESSORS, "wb");
    if (professors_file != NULL) {
        fwrite(&db->professor_count, sizeof(int), 1, professors_file);
        fwrite(db->professors, sizeof(Professor), db->professor_count, professors_file);
        fclose(professors_file);
    } else {
        return 0;
    }
    
    FILE *departments_file = fopen(FILENAME_DEPARTMENTS, "wb");
    if (departments_file != NULL) {
        fwrite(&db->department_count, sizeof(int), 1, departments_file);
        fwrite(db->departments, sizeof(Department), db->department_count, departments_file);
        fclose(departments_file);
    } else {
        return 0;
    }
    
    FILE *assignments_file = fopen(FILENAME_ASSIGNMENTS, "wb");
    if (assignments_file != NULL) {
        fwrite(&db->assignment_count, sizeof(int), 1, assignments_file);
        fwrite(db->assignments, sizeof(Assignment), db->assignment_count, assignments_file);
        fclose(assignments_file);
    } else {
        return 0;
    }
    
    FILE *grades_file = fopen(FILENAME_GRADES, "wb");
    if (grades_file != NULL) {
        fwrite(&db->grade_count, sizeof(int), 1, grades_file);
        fwrite(db->grades, sizeof(Grade), db->grade_count, grades_file);
        fclose(grades_file);
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
    
    FILE *professors_file = fopen(FILENAME_PROFESSORS, "rb");
    if (professors_file != NULL) {
        fread(&db->professor_count, sizeof(int), 1, professors_file);
        fread(db->professors, sizeof(Professor), db->professor_count, professors_file);
        fclose(professors_file);
    } else {
        return 0;
    }
    
    FILE *departments_file = fopen(FILENAME_DEPARTMENTS, "rb");
    if (departments_file != NULL) {
        fread(&db->department_count, sizeof(int), 1, departments_file);
        fread(db->departments, sizeof(Department), db->department_count, departments_file);
        fclose(departments_file);
    } else {
        return 0;
    }
    
    FILE *assignments_file = fopen(FILENAME_ASSIGNMENTS, "rb");
    if (assignments_file != NULL) {
        fread(&db->assignment_count, sizeof(int), 1, assignments_file);
        fread(db->assignments, sizeof(Assignment), db->assignment_count, assignments_file);
        fclose(assignments_file);
    } else {
        return 0;
    }
    
    FILE *grades_file = fopen(FILENAME_GRADES, "rb");
    if (grades_file != NULL) {
        fread(&db->grade_count, sizeof(int), 1, grades_file);
        fread(db->grades, sizeof(Grade), db->grade_count, grades_file);
        fclose(grades_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(UniversityDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* departments[] = {"Computer Science", "Mathematics", "Physics", "Chemistry", "Biology",
                           "English", "History", "Economics", "Psychology", "Sociology"};
    
    char* course_prefixes[] = {"CS", "MATH", "PHYS", "CHEM", "BIO", "ENG", "HIST", "ECON", "PSYCH", "SOC"};
    
    char* course_names[] = {"Introduction to Programming", "Data Structures", "Algorithms",
                            "Database Systems", "Software Engineering", "Computer Networks",
                            "Artificial Intelligence", "Machine Learning", "Web Development",
                            "Operating Systems", "Computer Architecture", "Cybersecurity"};
    
    char* majors[] = {"Computer Science", "Mathematics", "Physics", "Chemistry", "Biology",
                      "English", "History", "Economics", "Psychology", "Sociology"};
    
    char* professor_titles[] = {"Professor", "Associate Professor", "Assistant Professor", "Lecturer"};
    
    char* semesters[] = {"Fall", "Spring", "Summer", "Winter"};
    
    char* assignment_titles[] = {"Homework 1", "Homework 2", "Midterm Exam", "Final Exam",
                                "Project 1", "Project 2", "Quiz 1", "Quiz 2", "Lab Assignment", "Research Paper"};
    
    srand(time(NULL));
    
    // Add departments
    for (int i = 0; i < 10 && db->department_count < MAX_DEPARTMENTS; i++) {
        char description[MAX_DESCRIPTION_LENGTH];
        sprintf(description, "Department of %s offering comprehensive academic programs and research opportunities", departments[i]);
        add_department(db, departments[i], description);
    }
    
    // Add professors
    for (int i = 0; i < 50 && db->professor_count < MAX_PROFESSORS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], office[30];
        sprintf(email, "%s.%d@university.edu", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(office, "Room %d-%d", 100 + (rand() % 5), 100 + (rand() % 50));
        
        int department_id = 1 + (rand() % 10);
        const char* title = professor_titles[rand() % 4];
        int hire_year = 1990 + (rand() % 30);
        double salary = 50000.0 + (rand() % 100000) + (rand() % 100) / 100.0;
        
        add_professor(db, first_name, last_name, email, phone, office,
                     department_id, title, hire_year, salary);
    }
    
    // Add courses
    for (int i = 0; i < 80 && db->course_count < MAX_COURSES; i++) {
        int department_id = 1 + (rand() % 10);
        int prefix_index = department_id - 1;
        
        char course_code[20];
        sprintf(course_code, "%s%d%02d", course_prefixes[prefix_index], 100 + (rand() % 400), 1 + (rand() % 3));
        
        int course_name_index = rand() % 12;
        char description[MAX_DESCRIPTION_LENGTH];
        sprintf(description, "Comprehensive study of %s with theoretical foundations and practical applications", course_names[course_name_index]);
        
        int credits = 1 + (rand() % 4);
        int max_capacity = 20 + (rand() % 80);
        const char* semester = semesters[rand() % 4];
        int year = 2020 + (rand() % 4);
        
        char schedule[100];
        sprintf(schedule, "%s, %s, %s %02d:00-%02d:00",
                rand() % 2 == 0 ? "Mon" : "Tue",
                rand() % 2 == 0 ? "Wed" : "Thu",
                rand() % 2 == 0 ? "Fri" : "Mon",
                8 + (rand() % 8),
                9 + (rand() % 8));
        
        char location[50];
        sprintf(location, "Building %c Room %d", 'A' + (rand() % 5), 100 + (rand() % 300));
        
        int professor_id = 1 + (rand() % 50);
        
        int course_id = add_course(db, course_code, course_names[course_name_index], description,
                                 department_id, credits, max_capacity, semester, year,
                                 schedule, location, professor_id);
        
        // Add some assignments for this course
        if (course_id != -1 && db->assignment_count < MAX_ASSIGNMENTS) {
            int assignment_count = 3 + (rand() % 5);
            for (int j = 0; j < assignment_count && db->assignment_count < MAX_ASSIGNMENTS; j++) {
                char title[100];
                sprintf(title, "%s - %s", course_names[course_name_index], assignment_titles[rand() % 10]);
                
                char description[MAX_DESCRIPTION_LENGTH];
                sprintf(description, "Complete all tasks related to %s", title);
                
                // Random due date (within 3 months from now)
                time_t now = time(NULL);
                time_t due_time = now + (rand() % 90) * 24 * 60 * 60;
                struct tm *due_tm = localtime(&due_time);
                Date due_date;
                due_date.day = due_tm->tm_mday;
                due_date.month = due_tm->tm_mon + 1;
                due_date.year = due_tm->tm_year + 1900;
                
                int max_points = 10 + (rand() % 90);
                
                add_assignment(db, course_id, title, description, due_date, max_points);
            }
        }
    }
    
    // Add students
    for (int i = 0; i < 200 && db->student_count < MAX_STUDENTS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[100];
        sprintf(email, "%s.%d@student.university.edu", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Campus Ave, City, State", 100 + rand() % 900);
        
        // Random birth date (18-30 years old)
        int birth_year = 1993 + (rand() % 12);
        int birth_month = 1 + (rand() % 12);
        int birth_day = 1 + (rand() % 28);
        Date birth_date = {birth_day, birth_month, birth_year};
        
        int department_id = 1 + (rand() % 10);
        int year_level = 1 + (rand() % 6);
        const char* major = majors[rand() % 10];
        const char* minor = rand() % 3 == 0 ? majors[rand() % 10] : "";
        
        int student_id = add_student(db, first_name, last_name, email, phone, address,
                                   birth_date, department_id, year_level, major, minor);
        
        // Enroll student in some courses
        if (student_id != -1) {
            int course_count = 3 + (rand() % 4);
            for (int j = 0; j < course_count; j++) {
                int course_id = 1 + (rand() % 80);
                enroll_student(db, student_id, course_id);
            }
        }
    }
    
    // Add grades for some enrollments
    for (int i = 0; i < 500 && db->grade_count < MAX_GRADES; i++) {
        int enrollment_id = 1 + (rand() % db->enrollment_count);
        int assignment_id = 1 + (rand() % db->assignment_count);
        
        // Get max points for this assignment
        int max_points = 100;
        for (int j = 0; j < db->assignment_count; j++) {
            if (db->assignments[j].id == assignment_id) {
                max_points = db->assignments[j].max_points;
                break;
            }
        }
        
        // Random score between 40% and 100%
        int points_earned = max_points * (0.4 + ((double)rand() / RAND_MAX * 0.6));
        
        char feedback[200];
        if (points_earned >= max_points * 0.9) {
            sprintf(feedback, "Excellent work! Keep up the great performance.");
        } else if (points_earned >= max_points * 0.7) {
            sprintf(feedback, "Good work. There are some areas for improvement.");
        } else if (points_earned >= max_points * 0.5) {
            sprintf(feedback, "Satisfactory work. Please review the material and try again.");
        } else {
            sprintf(feedback, "Needs significant improvement. Please see me during office hours.");
        }
        
        add_grade(db, enrollment_id, assignment_id, points_earned, feedback);
    }
    
    // Set some final grades and calculate GPAs
    for (int i = 0; i < db->enrollment_count; i++) {
        if (rand() % 3 == 0) { // 33% chance of having a final grade
            // Calculate average from assignment grades
            int total_points = 0;
            int max_points_total = 0;
            int grade_count = 0;
            
            for (int j = 0; j < db->grade_count; j++) {
                if (db->grades[j].enrollment_id == db->enrollments[i].id) {
                    total_points += db->grades[j].points_earned;
                    max_points_total += db->grades[j].max_points;
                    grade_count++;
                }
            }
            
            if (grade_count > 0) {
                int final_grade = max_points_total > 0 ? (total_points * 100) / max_points_total : 0;
                db->enrollments[i].final_grade = final_grade;
                db->enrollments[i].letter_grade_points = calculate_letter_grade_points(final_grade);
                
                // Randomly set status
                int status_rand = rand() % 4;
                switch (status_rand) {
                    case 0: strcpy(db->enrollments[i].status, "Active"); break;
                    case 1: strcpy(db->enrollments[i].status, "Completed"); break;
                    case 2: strcpy(db->enrollments[i].status, "Dropped"); break;
                    case 3: strcpy(db->enrollments[i].status, "Withdrawn"); break;
                }
            }
        }
    }
    
    // Update student GPAs
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            update_student_gpa(db, db->students[i].id);
        }
    }
}