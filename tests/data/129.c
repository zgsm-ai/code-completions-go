#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_STUDENTS 1000
#define MAX_COURSES 500
#define MAX_ENROLLMENTS 2000
#define MAX_INSTRUCTORS 100
#define MAX_NAME_LENGTH 50
#define FILENAME_STUDENTS "students.dat"
#define FILENAME_COURSES "courses.dat"
#define FILENAME_ENROLLMENTS "enrollments.dat"
#define FILENAME_INSTRUCTORS "instructors.dat"

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
    char major[50];
    double gpa;
    int credits_completed;
    int active;
    char student_id[20];
} Student;

typedef struct {
    int id;
    char course_code[20];
    char course_name[MAX_NAME_LENGTH];
    char description[200];
    int credits;
    char department[50];
    int max_students;
    int active;
    int semester;
    int year;
    char schedule[50]; // e.g., "Mon/Wed/Fri 10:00-11:00"
} Course;

typedef struct {
    int id;
    int student_id;
    int course_id;
    Date enrollment_date;
    char grade[5]; // A, B, C, D, F, I, W
    double grade_points;
    int active;
    int semester;
    int year;
} Enrollment;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char office[50];
    char department[50];
    char title[MAX_NAME_LENGTH];
    double salary;
    int active;
    char specialization[50];
} Instructor;

typedef struct {
    Student students[MAX_STUDENTS];
    int student_count;
    Course courses[MAX_COURSES];
    int course_count;
    Enrollment enrollments[MAX_ENROLLMENTS];
    int enrollment_count;
    Instructor instructors[MAX_INSTRUCTORS];
    int instructor_count;
} UniversityDatabase;

// Function prototypes
void initialize_database(UniversityDatabase *db);
int add_student(UniversityDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 Date birth_date, const char *major, const char *student_id);
int add_course(UniversityDatabase *db, const char *course_code, const char *course_name,
               const char *description, int credits, const char *department,
               int max_students, int semester, int year, const char *schedule);
int add_instructor(UniversityDatabase *db, const char *first_name, const char *last_name,
                   const char *email, const char *phone, const char *office,
                   const char *department, const char *title, double salary,
                   const char *specialization);
int enroll_student(UniversityDatabase *db, int student_id, int course_id, int semester, int year);
void assign_instructor_to_course(UniversityDatabase *db, int course_id, int instructor_id);
void assign_grade(UniversityDatabase *db, int enrollment_id, const char *grade);
void display_student(const Student *student);
void display_course(const Course *course);
void display_instructor(const Instructor *instructor);
void display_enrollment(const Enrollment *enrollment, const UniversityDatabase *db);
void display_all_students(const UniversityDatabase *db);
void display_all_courses(const UniversityDatabase *db);
void display_all_instructors(const UniversityDatabase *db);
void display_all_enrollments(const UniversityDatabase *db);
void search_students_by_name(const UniversityDatabase *db, const char *name);
void search_courses_by_department(const UniversityDatabase *db, const char *department);
void search_instructors_by_department(const UniversityDatabase *db, const char *department);
void generate_student_transcript(const UniversityDatabase *db, int student_id);
void generate_course_roster(const UniversityDatabase *db, int course_id);
void generate_instructor_schedule(const UniversityDatabase *db, int instructor_id);
void generate_department_report(const UniversityDatabase *db, const char *department);
void calculate_gpa_statistics(const UniversityDatabase *db);
int find_student_by_id(const UniversityDatabase *db, int student_id);
int find_course_by_id(const UniversityDatabase *db, int course_id);
int find_instructor_by_id(const UniversityDatabase *db, int instructor_id);
int find_enrollment_by_id(const UniversityDatabase *db, int enrollment_id);
int save_database(const UniversityDatabase *db);
int load_database(UniversityDatabase *db);
void generate_sample_data(UniversityDatabase *db);
double grade_to_points(const char *grade);
void update_student_gpa(UniversityDatabase *db, int student_id);
int is_valid_email(const char *email);
int is_valid_phone(const char *phone);

int main() {
    UniversityDatabase db;
    initialize_database(&db);
    
    printf("University Management System\n");
    printf("1. Add Student\n");
    printf("2. Add Course\n");
    printf("3. Add Instructor\n");
    printf("4. Enroll Student\n");
    printf("5. Assign Instructor to Course\n");
    printf("6. Assign Grade\n");
    printf("7. Display All Students\n");
    printf("8. Display All Courses\n");
    printf("9. Display All Instructors\n");
    printf("10. Display All Enrollments\n");
    printf("11. Search Students by Name\n");
    printf("12. Search Courses by Department\n");
    printf("13. Search Instructors by Department\n");
    printf("14. Generate Student Transcript\n");
    printf("15. Generate Course Roster\n");
    printf("16. Generate Instructor Schedule\n");
    printf("17. Generate Department Report\n");
    printf("18. Calculate GPA Statistics\n");
    printf("19. Update Student GPA\n");
    printf("20. Save Database\n");
    printf("21. Load Database\n");
    printf("22. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100], major[50], student_id[20];
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
                printf("Enter major: ");
                scanf(" %[^\n]", major);
                printf("Enter student ID: ");
                scanf(" %[^\n]", student_id);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                Date birth_date = {day, month, year};
                
                int student_id_return = add_student(&db, first_name, last_name, email, phone, address,
                                                 birth_date, major, student_id);
                if (student_id_return != -1) {
                    printf("Student added with ID: %d\n", student_id_return);
                } else {
                    printf("Failed to add student. Database is full.\n");
                }
                break;
            }
            case 2: {
                char course_code[20], course_name[MAX_NAME_LENGTH], description[200];
                char department[50], schedule[50];
                int credits, max_students, semester, year;
                
                printf("Enter course code: ");
                scanf("%s", course_code);
                printf("Enter course name: ");
                scanf(" %[^\n]", course_name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter credits: ");
                scanf("%d", &credits);
                printf("Enter department: ");
                scanf(" %[^\n]", department);
                printf("Enter max students: ");
                scanf("%d", &max_students);
                printf("Enter semester (1-Spring, 2-Summer, 3-Fall): ");
                scanf("%d", &semester);
                printf("Enter year: ");
                scanf("%d", &year);
                printf("Enter schedule (e.g., Mon/Wed/Fri 10:00-11:00): ");
                scanf(" %[^\n]", schedule);
                
                int course_id = add_course(&db, course_code, course_name, description, credits,
                                        department, max_students, semester, year, schedule);
                if (course_id != -1) {
                    printf("Course added with ID: %d\n", course_id);
                } else {
                    printf("Failed to add course. Database is full.\n");
                }
                break;
            }
            case 3: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], office[50], department[50];
                char title[MAX_NAME_LENGTH], specialization[50];
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
                printf("Enter department: ");
                scanf(" %[^\n]", department);
                printf("Enter title: ");
                scanf(" %[^\n]", title);
                printf("Enter salary: ");
                scanf("%lf", &salary);
                printf("Enter specialization: ");
                scanf(" %[^\n]", specialization);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                int instructor_id = add_instructor(&db, first_name, last_name, email, phone, office,
                                               department, title, salary, specialization);
                if (instructor_id != -1) {
                    printf("Instructor added with ID: %d\n", instructor_id);
                } else {
                    printf("Failed to add instructor. Database is full.\n");
                }
                break;
            }
            case 4: {
                int student_id, course_id, semester, year;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                printf("Enter semester (1-Spring, 2-Summer, 3-Fall): ");
                scanf("%d", &semester);
                printf("Enter year: ");
                scanf("%d", &year);
                
                if (enroll_student(&db, student_id, course_id, semester, year)) {
                    printf("Student enrolled successfully.\n");
                } else {
                    printf("Failed to enroll student. Check IDs and course capacity.\n");
                }
                break;
            }
            case 5: {
                int course_id, instructor_id;
                
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                printf("Enter instructor ID: ");
                scanf("%d", &instructor_id);
                
                assign_instructor_to_course(&db, course_id, instructor_id);
                break;
            }
            case 6: {
                int enrollment_id;
                char grade[5];
                
                printf("Enter enrollment ID: ");
                scanf("%d", &enrollment_id);
                printf("Enter grade (A, B, C, D, F, I, W): ");
                scanf("%s", grade);
                
                if (strlen(grade) != 1 || (grade[0] != 'A' && grade[0] != 'B' && 
                    grade[0] != 'C' && grade[0] != 'D' && grade[0] != 'F' && 
                    grade[0] != 'I' && grade[0] != 'W')) {
                    printf("Invalid grade. Must be one of: A, B, C, D, F, I, W\n");
                    break;
                }
                
                assign_grade(&db, enrollment_id, grade);
                break;
            }
            case 7: {
                display_all_students(&db);
                break;
            }
            case 8: {
                display_all_courses(&db);
                break;
            }
            case 9: {
                display_all_instructors(&db);
                break;
            }
            case 10: {
                display_all_enrollments(&db);
                break;
            }
            case 11: {
                char name[MAX_NAME_LENGTH];
                printf("Enter student name to search: ");
                scanf(" %[^\n]", name);
                search_students_by_name(&db, name);
                break;
            }
            case 12: {
                char department[50];
                printf("Enter department to search: ");
                scanf(" %[^\n]", department);
                search_courses_by_department(&db, department);
                break;
            }
            case 13: {
                char department[50];
                printf("Enter department to search: ");
                scanf(" %[^\n]", department);
                search_instructors_by_department(&db, department);
                break;
            }
            case 14: {
                int student_id;
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                generate_student_transcript(&db, student_id);
                break;
            }
            case 15: {
                int course_id;
                printf("Enter course ID: ");
                scanf("%d", &course_id);
                generate_course_roster(&db, course_id);
                break;
            }
            case 16: {
                int instructor_id;
                printf("Enter instructor ID: ");
                scanf("%d", &instructor_id);
                generate_instructor_schedule(&db, instructor_id);
                break;
            }
            case 17: {
                char department[50];
                printf("Enter department: ");
                scanf(" %[^\n]", department);
                generate_department_report(&db, department);
                break;
            }
            case 18: {
                calculate_gpa_statistics(&db);
                break;
            }
            case 19: {
                int student_id;
                double gpa;
                
                printf("Enter student ID: ");
                scanf("%d", &student_id);
                printf("Enter new GPA: ");
                scanf("%lf", &gpa);
                
                update_student_gpa(&db, student_id, gpa);
                break;
            }
            case 20: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 21: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 22: {
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
    db->instructor_count = 0;
}

int add_student(UniversityDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 Date birth_date, const char *major, const char *student_id) {
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
    strncpy(student->major, major, 49);
    student->major[49] = '\0';
    strncpy(student->student_id, student_id, 19);
    student->student_id[19] = '\0';
    student->gpa = 0.0;
    student->credits_completed = 0;
    student->active = 1;
    
    // Set enrollment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    student->enrollment_date.day = current_time->tm_mday;
    student->enrollment_date.month = current_time->tm_mon + 1;
    student->enrollment_date.year = current_time->tm_year + 1900;
    
    db->student_count++;
    return student->id;
}

int add_course(UniversityDatabase *db, const char *course_code, const char *course_name,
               const char *description, int credits, const char *department,
               int max_students, int semester, int year, const char *schedule) {
    if (db->course_count >= MAX_COURSES) {
        return -1;
    }
    
    Course *course = &db->courses[db->course_count];
    course->id = db->course_count + 1;
    strncpy(course->course_code, course_code, 19);
    course->course_code[19] = '\0';
    strncpy(course->course_name, course_name, MAX_NAME_LENGTH - 1);
    course->course_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(course->description, description, 199);
    course->description[199] = '\0';
    course->credits = credits;
    strncpy(course->department, department, 49);
    course->department[49] = '\0';
    course->max_students = max_students;
    course->active = 1;
    course->semester = semester;
    course->year = year;
    strncpy(course->schedule, schedule, 49);
    course->schedule[49] = '\0';
    
    db->course_count++;
    return course->id;
}

int add_instructor(UniversityDatabase *db, const char *first_name, const char *last_name,
                   const char *email, const char *phone, const char *office,
                   const char *department, const char *title, double salary,
                   const char *specialization) {
    if (db->instructor_count >= MAX_INSTRUCTORS) {
        return -1;
    }
    
    Instructor *instructor = &db->instructors[db->instructor_count];
    instructor->id = db->instructor_count + 1;
    strncpy(instructor->first_name, first_name, MAX_NAME_LENGTH - 1);
    instructor->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(instructor->last_name, last_name, MAX_NAME_LENGTH - 1);
    instructor->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(instructor->email, email, 49);
    instructor->email[49] = '\0';
    strncpy(instructor->phone, phone, 14);
    instructor->phone[14] = '\0';
    strncpy(instructor->office, office, 49);
    instructor->office[49] = '\0';
    strncpy(instructor->department, department, 49);
    instructor->department[49] = '\0';
    strncpy(instructor->title, title, MAX_NAME_LENGTH - 1);
    instructor->title[MAX_NAME_LENGTH - 1] = '\0';
    instructor->salary = salary;
    instructor->active = 1;
    strncpy(instructor->specialization, specialization, 49);
    instructor->specialization[49] = '\0';
    
    db->instructor_count++;
    return instructor->id;
}

int enroll_student(UniversityDatabase *db, int student_id, int course_id, int semester, int year) {
    if (db->enrollment_count >= MAX_ENROLLMENTS) {
        return 0;
    }
    
    // Validate student and course exist
    if (find_student_by_id(db, student_id) == -1 || find_course_by_id(db, course_id) == -1) {
        return 0;
    }
    
    // Check if student is already enrolled in this course
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id && 
            db->enrollments[i].course_id == course_id &&
            db->enrollments[i].semester == semester &&
            db->enrollments[i].year == year &&
            db->enrollments[i].active) {
            return 0;
        }
    }
    
    // Check course capacity
    int course_index = find_course_by_id(db, course_id);
    if (course_index != -1) {
        int enrolled_count = 0;
        for (int i = 0; i < db->enrollment_count; i++) {
            if (db->enrollments[i].course_id == course_id && 
                db->enrollments[i].semester == semester &&
                db->enrollments[i].year == year &&
                db->enrollments[i].active) {
                enrolled_count++;
            }
        }
        
        if (enrolled_count >= db->courses[course_index].max_students) {
            return 0;
        }
    }
    
    // Create enrollment
    Enrollment *enrollment = &db->enrollments[db->enrollment_count];
    enrollment->id = db->enrollment_count + 1;
    enrollment->student_id = student_id;
    enrollment->course_id = course_id;
    enrollment->semester = semester;
    enrollment->year = year;
    enrollment->active = 1;
    strcpy(enrollment->grade, "I"); // Incomplete initially
    
    // Set enrollment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    enrollment->enrollment_date.day = current_time->tm_mday;
    enrollment->enrollment_date.month = current_time->tm_mon + 1;
    enrollment->enrollment_date.year = current_time->tm_year + 1900;
    
    db->enrollment_count++;
    return 1;
}

void assign_instructor_to_course(UniversityDatabase *db, int course_id, int instructor_id) {
    int course_index = find_course_by_id(db, course_id);
    int instructor_index = find_instructor_by_id(db, instructor_id);
    
    if (course_index == -1) {
        printf("Course not found.\n");
        return;
    }
    
    if (instructor_index == -1) {
        printf("Instructor not found.\n");
        return;
    }
    
    printf("Instructor assigned to course.\n");
    printf("Course: %s (%s)\n", 
           db->courses[course_index].course_name, db->courses[course_index].course_code);
    printf("Instructor: %s %s\n", 
           db->instructors[instructor_index].first_name,
           db->instructors[instructor_index].last_name);
    printf("Department: %s\n", db->courses[course_index].department);
    printf("Schedule: %s\n", db->courses[course_index].schedule);
}

void assign_grade(UniversityDatabase *db, int enrollment_id, const char *grade) {
    int enrollment_index = find_enrollment_by_id(db, enrollment_id);
    if (enrollment_index == -1) {
        printf("Enrollment not found.\n");
        return;
    }
    
    Enrollment *enrollment = &db->enrollments[enrollment_index];
    strcpy(enrollment->grade, grade);
    enrollment->grade_points = grade_to_points(grade);
    
    // Update student GPA
    update_student_gpa(db, enrollment->student_id);
    
    printf("Grade assigned successfully.\n");
    printf("Enrollment ID: %d\n", enrollment_id);
    printf("Grade: %s\n", grade);
    printf("Grade Points: %.1f\n", enrollment->grade_points);
}

void display_student(const Student *student) {
    printf("\nStudent ID: %d\n", student->id);
    printf("Name: %s %s\n", student->first_name, student->last_name);
    printf("Student ID: %s\n", student->student_id);
    printf("Email: %s\n", student->email);
    printf("Phone: %s\n", student->phone);
    printf("Address: %s\n", student->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           student->birth_date.day, student->birth_date.month, student->birth_date.year);
    printf("Enrollment Date: %02d/%02d/%d\n", 
           student->enrollment_date.day, student->enrollment_date.month, student->enrollment_date.year);
    printf("Major: %s\n", student->major);
    printf("GPA: %.2f\n", student->gpa);
    printf("Credits Completed: %d\n", student->credits_completed);
    printf("Status: %s\n", student->active ? "Active" : "Inactive");
}

void display_course(const Course *course) {
    printf("\nCourse ID: %d\n", course->id);
    printf("Course Code: %s\n", course->course_code);
    printf("Course Name: %s\n", course->course_name);
    printf("Description: %s\n", course->description);
    printf("Credits: %d\n", course->credits);
    printf("Department: %s\n", course->department);
    printf("Max Students: %d\n", course->max_students);
    printf("Semester: %d\n", course->semester);
    printf("Year: %d\n", course->year);
    printf("Schedule: %s\n", course->schedule);
    printf("Status: %s\n", course->active ? "Active" : "Inactive");
}

void display_instructor(const Instructor *instructor) {
    printf("\nInstructor ID: %d\n", instructor->id);
    printf("Name: %s %s\n", instructor->first_name, instructor->last_name);
    printf("Email: %s\n", instructor->email);
    printf("Phone: %s\n", instructor->phone);
    printf("Office: %s\n", instructor->office);
    printf("Department: %s\n", instructor->department);
    printf("Title: %s\n", instructor->title);
    printf("Salary: $%.2f\n", instructor->salary);
    printf("Specialization: %s\n", instructor->specialization);
    printf("Status: %s\n", instructor->active ? "Active" : "Inactive");
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
               db->courses[course_index].course_name,
               db->courses[course_index].course_code,
               enrollment->course_id);
    }
    
    printf("Enrollment Date: %02d/%02d/%d\n", 
           enrollment->enrollment_date.day, enrollment->enrollment_date.month, enrollment->enrollment_date.year);
    printf("Semester: %d\n", enrollment->semester);
    printf("Year: %d\n", enrollment->year);
    printf("Grade: %s\n", enrollment->grade);
    printf("Grade Points: %.1f\n", enrollment->grade_points);
    printf("Status: %s\n", enrollment->active ? "Active" : "Inactive");
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

void display_all_instructors(const UniversityDatabase *db) {
    printf("\n=== All Instructors ===\n");
    printf("Total Instructors: %d\n\n", db->instructor_count);
    
    for (int i = 0; i < db->instructor_count; i++) {
        if (db->instructors[i].active) {
            display_instructor(&db->instructors[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_enrollments(const UniversityDatabase *db) {
    printf("\n=== All Enrollments ===\n");
    printf("Total Enrollments: %d\n\n", db->enrollment_count);
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].active) {
            display_enrollment(&db->enrollments[i], db);
            printf("========================\n");
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

void search_instructors_by_department(const UniversityDatabase *db, const char *department) {
    printf("\nInstructors in Department: %s\n", department);
    int found = 0;
    
    for (int i = 0; i < db->instructor_count; i++) {
        if (db->instructors[i].active && strstr(db->instructors[i].department, department) != NULL) {
            display_instructor(&db->instructors[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No instructors found in this department.\n");
    }
}

void generate_student_transcript(const UniversityDatabase *db, int student_id) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    Student *student = &db->students[student_index];
    
    printf("\n=== Transcript for %s %s ===\n", student->first_name, student->last_name);
    printf("Student ID: %s\n", student->student_id);
    printf("Email: %s\n", student->email);
    printf("Major: %s\n", student->major);
    printf("GPA: %.2f\n", student->gpa);
    printf("Credits Completed: %d\n", student->credits_completed);
    printf("Enrollment Date: %02d/%02d/%d\n", 
           student->enrollment_date.day, student->enrollment_date.month, student->enrollment_date.year);
    
    printf("\nCourse History:\n");
    printf("%-15s %-10s %-8s %-8s %-8s %-8s\n", 
           "Semester", "Course Code", "Grade", "Credits", "Status");
    printf("-------------------------------------------------\n");
    
    int total_credits = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id && db->enrollments[i].active) {
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1) {
                printf("%d-%d %-15s %-10s %-8s %-8d %-8s\n", 
                       db->enrollments[i].year, db->enrollments[i].semester,
                       db->courses[course_index].course_code,
                       db->enrollments[i].grade,
                       db->courses[course_index].credits,
                       db->enrollments[i].grade);
                
                if (strcmp(db->enrollments[i].grade, "W") != 0) { // Not withdrawn
                    total_credits += db->courses[course_index].credits;
                }
            }
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Total Credits: %d\n", total_credits);
    printf("Overall GPA: %.2f\n", student->gpa);
}

void generate_course_roster(const UniversityDatabase *db, int course_id) {
    int course_index = find_course_by_id(db, course_id);
    if (course_index == -1) {
        printf("Course not found.\n");
        return;
    }
    
    Course *course = &db->courses[course_index];
    
    printf("\n=== Course Roster: %s ===\n", course->course_name);
    printf("Course Code: %s\n", course->course_code);
    printf("Credits: %d\n", course->credits);
    printf("Department: %s\n", course->department);
    printf("Semester: %d\n", course->semester);
    printf("Year: %d\n", course->year);
    printf("Schedule: %s\n", course->schedule);
    
    printf("\nEnrolled Students:\n");
    printf("%-5s %-20s %-15s %-8s %-8s\n", 
           "ID", "Name", "Student ID", "Grade", "Status");
    printf("-------------------------------------------------\n");
    
    int enrolled_count = 0;
    double total_grade_points = 0.0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].course_id == course_id && db->enrollments[i].active) {
            int student_index = find_student_by_id(db, db->enrollments[i].student_id);
            if (student_index != -1) {
                printf("%-5d %-20s %-15s %-8s %-8s\n", 
                       db->enrollments[i].student_id,
                       db->students[student_index].first_name,
                       db->students[student_index].student_id,
                       db->enrollments[i].grade,
                       db->enrollments[i].grade);
                
                enrolled_count++;
                
                if (strcmp(db->enrollments[i].grade, "W") != 0) { // Not withdrawn
                    total_grade_points += db->enrollments[i].grade_points;
                }
            }
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Total Enrolled: %d\n", enrolled_count);
    
    if (enrolled_count > 0) {
        double average_grade = total_grade_points / enrolled_count;
        printf("Average Grade: %.2f\n", average_grade);
    }
}

void generate_instructor_schedule(const UniversityDatabase *db, int instructor_id) {
    int instructor_index = find_instructor_by_id(db, instructor_id);
    if (instructor_index == -1) {
        printf("Instructor not found.\n");
        return;
    }
    
    Instructor *instructor = &db->instructors[instructor_index];
    
    printf("\n=== Instructor Schedule: %s %s ===\n", instructor->first_name, instructor->last_name);
    printf("Department: %s\n", instructor->department);
    printf("Title: %s\n", instructor->title);
    printf("Office: %s\n", instructor->office);
    printf("Email: %s\n", instructor->email);
    printf("Phone: %s\n", instructor->phone);
    printf("Specialization: %s\n", instructor->specialization);
    
    printf("\nCourse Schedule:\n");
    printf("%-15s %-10s %-15s %-20s %-15s\n", 
           "Course Code", "Semester", "Course Name", "Schedule", "Department");
    printf("-----------------------------------------------------------------\n");
    
    for (int i = 0; i < db->course_count; i++) {
        // Simple check: assume instructor teaches course if course is in their department
        if (db->courses[i].active && 
            strcmp(db->courses[i].department, instructor->department) == 0) {
            printf("%-15s %-10d %-15s %-20s %-15s\n", 
                   db->courses[i].course_code,
                   db->courses[i].semester,
                   db->courses[i].course_name,
                   db->courses[i].schedule,
                   db->courses[i].department);
        }
    }
}

void generate_department_report(const UniversityDatabase *db, const char *department) {
    printf("\n=== Department Report: %s ===\n", department);
    
    // Count instructors in department
    int instructor_count = 0;
    double total_salary = 0.0;
    
    for (int i = 0; i < db->instructor_count; i++) {
        if (db->instructors[i].active && 
            strstr(db->instructors[i].department, department) != NULL) {
            instructor_count++;
            total_salary += db->instructors[i].salary;
        }
    }
    
    // Count courses in department
    int course_count = 0;
    int total_credits = 0;
    
    for (int i = 0; i < db->course_count; i++) {
        if (db->courses[i].active && 
            strstr(db->courses[i].department, department) != NULL) {
            course_count++;
            total_credits += db->courses[i].credits;
        }
    }
    
    // Count students in department
    int student_count = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active && 
            strstr(db->students[i].major, department) != NULL) {
            student_count++;
        }
    }
    
    printf("\nDepartment Summary:\n");
    printf("Instructors: %d\n", instructor_count);
    printf("Average Salary: $%.2f\n", instructor_count > 0 ? total_salary / instructor_count : 0.0);
    printf("Courses: %d\n", course_count);
    printf("Total Credits Offered: %d\n", total_credits);
    printf("Students: %d\n", student_count);
    
    // Calculate average student GPA in department
    double total_gpa = 0.0;
    int gpa_count = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active && 
            strstr(db->students[i].major, department) != NULL) {
            total_gpa += db->students[i].gpa;
            gpa_count++;
        }
    }
    
    printf("Average Student GPA: %.2f\n", gpa_count > 0 ? total_gpa / gpa_count : 0.0);
}

void calculate_gpa_statistics(const UniversityDatabase *db) {
    printf("\n=== GPA Statistics ===\n");
    
    double sum_gpa = 0.0;
    int student_count = 0;
    
    int gpa_ranges[5] = {0}; // 0-1.0, 1.01-2.0, 2.01-3.0, 3.01-4.0, 4.0
    
    // Calculate GPA statistics
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            sum_gpa += db->students[i].gpa;
            student_count++;
            
            // Count in appropriate range
            if (db->students[i].gpa <= 1.0) {
                gpa_ranges[0]++;
            } else if (db->students[i].gpa <= 2.0) {
                gpa_ranges[1]++;
            } else if (db->students[i].gpa <= 3.0) {
                gpa_ranges[2]++;
            } else if (db->students[i].gpa < 4.0) {
                gpa_ranges[3]++;
            } else {
                gpa_ranges[4]++;
            }
        }
    }
    
    double avg_gpa = student_count > 0 ? sum_gpa / student_count : 0.0;
    
    printf("Total Students: %d\n", student_count);
    printf("Average GPA: %.2f\n", avg_gpa);
    
    printf("\nGPA Distribution:\n");
    printf("0.0-1.0: %d (%.1f%%)\n", gpa_ranges[0],
           student_count > 0 ? (float)gpa_ranges[0] / student_count * 100 : 0.0);
    printf("1.01-2.0: %d (%.1f%%)\n", gpa_ranges[1],
           student_count > 0 ? (float)gpa_ranges[1] / student_count * 100 : 0.0);
    printf("2.01-3.0: %d (%.1f%%)\n", gpa_ranges[2],
           student_count > 0 ? (float)gpa_ranges[2] / student_count * 100 : 0.0);
    printf("3.01-4.0: %d (%.1f%%)\n", gpa_ranges[3],
           student_count > 0 ? (float)gpa_ranges[3] / student_count * 100 : 0.0);
    printf("4.0: %d (%.1f%%)\n", gpa_ranges[4],
           student_count > 0 ? (float)gpa_ranges[4] / student_count * 100 : 0.0);
    
    // Department breakdown
    printf("\nAverage GPA by Major:\n");
    printf("%-20s %-10s %-10s\n", "Major", "Students", "Avg GPA");
    printf("----------------------------\n");
    
    // Use a simple approach to count majors
    typedef struct {
        char major[50];
        int count;
        double total_gpa;
    } MajorStats;
    
    MajorStats majors[20];
    int major_count = 0;
    
    for (int i = 0; i < db->student_count; i++) {
        if (db->students[i].active) {
            int found = 0;
            for (int j = 0; j < major_count; j++) {
                if (strcmp(majors[j].major, db->students[i].major) == 0) {
                    majors[j].count++;
                    majors[j].total_gpa += db->students[i].gpa;
                    found = 1;
                    break;
                }
            }
            
            if (!found && major_count < 20) {
                strcpy(majors[major_count].major, db->students[i].major);
                majors[major_count].count = 1;
                majors[major_count].total_gpa = db->students[i].gpa;
                major_count++;
            }
        }
    }
    
    for (int i = 0; i < major_count; i++) {
        printf("%-20s %-10d %-10.2f\n", 
               majors[i].major, majors[i].count, 
               majors[i].total_gpa / majors[i].count);
    }
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

int find_instructor_by_id(const UniversityDatabase *db, int instructor_id) {
    for (int i = 0; i < db->instructor_count; i++) {
        if (db->instructors[i].id == instructor_id && db->instructors[i].active) {
            return i;
        }
    }
    return -1;
}

int find_enrollment_by_id(const UniversityDatabase *db, int enrollment_id) {
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].id == enrollment_id && db->enrollments[i].active) {
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
    
    FILE *instructors_file = fopen(FILENAME_INSTRUCTORS, "wb");
    if (instructors_file != NULL) {
        fwrite(&db->instructor_count, sizeof(int), 1, instructors_file);
        fwrite(db->instructors, sizeof(Instructor), db->instructor_count, instructors_file);
        fclose(instructors_file);
    } else {
        return 0;
    }
    
    FILE *enrollments_file = fopen(FILENAME_ENROLLMENTS, "wb");
    if (enrollments_file != NULL) {
        fwrite(&db->enrollment_count, sizeof(int), 1, enrollments_file);
        fwrite(db->enrollments, sizeof(Enrollment), db->enrollment_count, enrollments_file);
        fclose(enrollments_file);
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
    
    FILE *instructors_file = fopen(FILENAME_INSTRUCTORS, "rb");
    if (instructors_file != NULL) {
        fread(&db->instructor_count, sizeof(int), 1, instructors_file);
        fread(db->instructors, sizeof(Instructor), db->instructor_count, instructors_file);
        fclose(instructors_file);
    } else {
        return 0;
    }
    
    FILE *enrollments_file = fopen(FILENAME_ENROLLMENTS, "rb");
    if (enrollments_file != NULL) {
        fread(&db->enrollment_count, sizeof(int), 1, enrollments_file);
        fread(db->enrollments, sizeof(Enrollment), db->enrollment_count, enrollments_file);
        fclose(enrollments_file);
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
    
    char* departments[] = {"Computer Science", "Mathematics", "Physics", "Chemistry",
                         "Biology", "Psychology", "History", "Literature",
                         "Economics", "Business", "Art", "Music"};
    
    char* majors[] = {"Computer Science", "Mathematics", "Physics", "Chemistry",
                      "Biology", "Psychology", "History", "Literature",
                      "Economics", "Business", "Art", "Music"};
    
    char* titles[] = {"Professor", "Associate Professor", "Assistant Professor",
                     "Lecturer", "Instructor", "Adjunct Professor"};
    
    char* specializations[] = {"Artificial Intelligence", "Data Science", "Cybersecurity",
                            "Software Engineering", "Theoretical Computer Science",
                            "Algebra", "Analysis", "Applied Mathematics",
                            "Quantum Physics", "Astrophysics", "Thermodynamics"};
    
    srand(time(NULL));
    
    // Add instructors
    for (int i = 0; i < 30 && db->instructor_count < MAX_INSTRUCTORS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], office[50], title[MAX_NAME_LENGTH], specialization[50];
        sprintf(email, "%s.%s@university.edu", first_name, last_name);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(office, "Building %d, Room %d", 1 + (rand() % 10), 100 + (rand() % 500));
        
        strcpy(title, titles[rand() % 6]);
        strcpy(specialization, specializations[rand() % 10]);
        
        double salary = 50000.0 + (rand() % 100000);
        
        add_instructor(db, first_name, last_name, email, phone, office,
                     departments[rand() % 12], title, salary, specialization);
    }
    
    // Add courses
    for (int i = 0; i < 100 && db->course_count < MAX_COURSES; i++) {
        char course_code[20], course_name[MAX_NAME_LENGTH], description[200], schedule[50];
        sprintf(course_code, "CS%d%03d", 100 + i);
        
        sprintf(course_name, "Introduction to %s", majors[rand() % 12]);
        sprintf(description, "Fundamental course covering key concepts in %s", majors[rand() % 12]);
        
        int credits = 1 + (rand() % 6); // 1-6 credits
        int max_students = 20 + (rand() % 100); // 20-120 students
        int semester = 1 + (rand() % 3); // 1-3 semesters
        int year = 2020 + (rand() % 4); // 2020-2023
        
        sprintf(schedule, "%s %s:%s-%s",
               (rand() % 2) ? "Mon/Wed/Fri" : "Tue/Thu",
               "08:00", "10:00");
        
        add_course(db, course_code, course_name, description, credits,
                 departments[rand() % 12], max_students, semester, year, schedule);
    }
    
    // Add students
    for (int i = 0; i < 300 && db->student_count < MAX_STUDENTS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[100], student_id[20];
        sprintf(email, "%s.%d@university.edu", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Campus Drive, University City, State", 100 + rand() % 900);
        sprintf(student_id, "%d%05d", 2023, 10000 + i);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1990 + (rand() % 25);
        Date birth_date = {day, month, year};
        
        add_student(db, first_name, last_name, email, phone, address,
                  birth_date, majors[rand() % 12], student_id);
    }
    
    // Add enrollments
    for (int i = 0; i < 1000 && db->enrollment_count < MAX_ENROLLMENTS; i++) {
        int student_id = 1 + (rand() % 300);
        int course_id = 1 + (rand() % 100);
        int semester = 1 + (rand() % 3);
        int year = 2020 + (rand() % 4);
        
        if (enroll_student(db, student_id, course_id, semester, year)) {
            // Assign random grade to some enrollments
            if (rand() % 4 < 3) { // 75% chance of having a grade
                int enrollment_index = db->enrollment_count - 1;
                const char* grades[] = {"A", "B", "C", "D", "F", "I", "W"};
                assign_grade(db, db->enrollments[enrollment_index].id, grades[rand() % 6]);
            }
        }
    }
}

double grade_to_points(const char *grade) {
    if (strcmp(grade, "A") == 0) return 4.0;
    if (strcmp(grade, "B") == 0) return 3.0;
    if (strcmp(grade, "C") == 0) return 2.0;
    if (strcmp(grade, "D") == 0) return 1.0;
    if (strcmp(grade, "F") == 0) return 0.0;
    if (strcmp(grade, "I") == 0) return 0.0; // Incomplete
    if (strcmp(grade, "W") == 0) return 0.0; // Withdrawn
    return 0.0;
}

void update_student_gpa(UniversityDatabase *db, int student_id) {
    int student_index = find_student_by_id(db, student_id);
    if (student_index == -1) {
        printf("Student not found.\n");
        return;
    }
    
    Student *student = &db->students[student_index];
    
    double total_points = 0.0;
    int total_credits = 0;
    
    for (int i = 0; i < db->enrollment_count; i++) {
        if (db->enrollments[i].student_id == student_id && db->enrollments[i].active) {
            int course_index = find_course_by_id(db, db->enrollments[i].course_id);
            if (course_index != -1 && strcmp(db->enrollments[i].grade, "W") != 0) { // Not withdrawn
                total_points += db->enrollments[i].grade_points * db->courses[course_index].credits;
                total_credits += db->courses[course_index].credits;
            }
        }
    }
    
    student->gpa = total_credits > 0 ? total_points / total_credits : 0.0;
    student->credits_completed = total_credits;
    
    printf("Student GPA updated.\n");
    printf("Student: %s %s\n", student->first_name, student->last_name);
    printf("New GPA: %.2f\n", student->gpa);
    printf("Total Credits: %d\n", student->credits_completed);
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