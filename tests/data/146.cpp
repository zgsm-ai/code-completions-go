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

#define MAX_STUDENTS 1000
#define MAX_COURSES 100
#define MAX_ENROLLMENTS 2000

struct date {
    int year;
    int month;
    int day;
};

class Course;
class Enrollment;

enum Grade {
    A = 4.0, B_PLUS = 3.5, B = 3.0, C_PLUS = 2.5, C = 2.0, D = 1.0, F = 0.0
};

class Student {
private:
    int studentId;
    string firstName;
    string lastName;
    string email;
    string phone;
    string address;
    date enrollmentDate;
    string department;
    int yearOfStudy;
    double gpa;
    int totalCredits;
    
public:
    Student(int studentId = 0, const string& firstName = "", const string& lastName = "", 
            const string& email = "", const string& phone = "", const string& address = "",
            date enrollmentDate = {0, 0, 0}, const string& department = "", int yearOfStudy = 1)
        : studentId(studentId), firstName(firstName), lastName(lastName), email(email), 
          phone(phone), address(address), enrollmentDate(enrollmentDate), department(department),
          yearOfStudy(yearOfStudy) {
        gpa = 0.0;
        totalCredits = 0;
    }
    
    int getStudentId() const { return studentId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }
    date getEnrollmentDate() const { return enrollmentDate; }
    string getDepartment() const { return department; }
    int getYearOfStudy() const { return yearOfStudy; }
    double getGpa() const { return gpa; }
    int getTotalCredits() const { return totalCredits; }
    
    void setStudentId(int id) { studentId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setAddress(const string& address) { this->address = address; }
    void setEnrollmentDate(date date) { enrollmentDate = date; }
    void setDepartment(const string& department) { this->department = department; }
    void setYearOfStudy(int year) { yearOfStudy = year; }
    void setGpa(double gpa) { this->gpa = gpa; }
    void setTotalCredits(int credits) { totalCredits = credits; }
    
    void updateGpa(const vector<Enrollment*>& enrollments) {
        if (enrollments.empty()) {
            gpa = 0.0;
            totalCredits = 0;
            return;
        }
        
        double totalGradePoints = 0.0;
        int sumCredits = 0;
        
        for (Enrollment* enrollment : enrollments) {
            if (enrollment->getStudentId() == studentId && enrollment->hasGrade()) {
                totalGradePoints += enrollment->getGradePoints() * enrollment->getCredits();
                sumCredits += enrollment->getCredits();
            }
        }
        
        totalCredits = sumCredits;
        gpa = (sumCredits > 0) ? totalGradePoints / sumCredits : 0.0;
    }
    
    void display() const {
        cout << "Student ID: " << studentId << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Address: " << address << endl;
        
        // Format date
        tm tm = {};
        tm.tm_year = enrollmentDate.year - 1900;
        tm.tm_mon = enrollmentDate.month - 1;
        tm.tm_mday = enrollmentDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Enrollment Date: " << dateStr << endl;
        
        cout << "Department: " << department << endl;
        cout << "Year of Study: " << yearOfStudy << endl;
        cout << "GPA: " << fixed << setprecision(2) << gpa << endl;
        cout << "Total Credits: " << totalCredits << endl;
    }
};

class Course {
private:
    int courseId;
    string courseCode;
    string title;
    string description;
    string instructor;
    string department;
    int credits;
    int capacity;
    int currentEnrollment;
    string semester;
    int year;
    string schedule;
    string location;
    
public:
    Course(int courseId = 0, const string& courseCode = "", const string& title = "", 
           const string& description = "", const string& instructor = "", 
           const string& department = "", int credits = 3, int capacity = 30)
        : courseId(courseId), courseCode(courseCode), title(title), description(description),
          instructor(instructor), department(department), credits(credits), capacity(capacity) {
        currentEnrollment = 0;
        semester = "Fall";
        year = 2023;
        schedule = "Mon/Wed 10:00-11:30";
        location = "TBD";
    }
    
    int getCourseId() const { return courseId; }
    string getCourseCode() const { return courseCode; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getInstructor() const { return instructor; }
    string getDepartment() const { return department; }
    int getCredits() const { return credits; }
    int getCapacity() const { return capacity; }
    int getCurrentEnrollment() const { return currentEnrollment; }
    string getSemester() const { return semester; }
    int getYear() const { return year; }
    string getSchedule() const { return schedule; }
    string getLocation() const { return location; }
    
    void setCourseId(int id) { courseId = id; }
    void setCourseCode(const string& code) { courseCode = code; }
    void setTitle(const string& title) { this->title = title; }
    void setDescription(const string& description) { this->description = description; }
    void setInstructor(const string& instructor) { this->instructor = instructor; }
    void setDepartment(const string& department) { this->department = department; }
    void setCredits(int credits) { this->credits = credits; }
    void setCapacity(int capacity) { this->capacity = capacity; }
    void setCurrentEnrollment(int enrollment) { currentEnrollment = enrollment; }
    void setSemester(const string& semester) { this->semester = semester; }
    void setYear(int year) { this->year = year; }
    void setSchedule(const string& schedule) { this->schedule = schedule; }
    void setLocation(const string& location) { this->location = location; }
    
    bool isFull() const {
        return currentEnrollment >= capacity;
    }
    
    void incrementEnrollment() {
        if (currentEnrollment < capacity) {
            currentEnrollment++;
        }
    }
    
    void decrementEnrollment() {
        if (currentEnrollment > 0) {
            currentEnrollment--;
        }
    }
    
    void display() const {
        cout << "Course ID: " << courseId << endl;
        cout << "Course Code: " << courseCode << endl;
        cout << "Title: " << title << endl;
        cout << "Description: " << description << endl;
        cout << "Instructor: " << instructor << endl;
        cout << "Department: " << department << endl;
        cout << "Credits: " << credits << endl;
        cout << "Capacity: " << capacity << endl;
        cout << "Current Enrollment: " << currentEnrollment << endl;
        cout << "Semester: " << semester << " " << year << endl;
        cout << "Schedule: " << schedule << endl;
        cout << "Location: " << location << endl;
    }
};

class Enrollment {
private:
    int enrollmentId;
    int studentId;
    int courseId;
    Grade grade;
    bool hasGradeFlag;
    date enrollmentDate;
    bool isActive;
    bool isCompleted;
    int credits;
    
public:
    Enrollment(int enrollmentId = 0, int studentId = 0, int courseId = 0)
        : enrollmentId(enrollmentId), studentId(studentId), courseId(courseId) {
        grade = F;
        hasGradeFlag = false;
        
        // Set enrollment date to today
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        enrollmentDate.day = timeinfo->tm_mday;
        enrollmentDate.month = timeinfo->tm_mon + 1;
        enrollmentDate.year = timeinfo->tm_year + 1900;
        
        isActive = true;
        isCompleted = false;
        credits = 0;
    }
    
    int getEnrollmentId() const { return enrollmentId; }
    int getStudentId() const { return studentId; }
    int getCourseId() const { return courseId; }
    Grade getGrade() const { return grade; }
    bool hasGrade() const { return hasGradeFlag; }
    date getEnrollmentDate() const { return enrollmentDate; }
    bool getIsActive() const { return isActive; }
    bool getIsCompleted() const { return isCompleted; }
    int getCredits() const { return credits; }
    
    double getGradePoints() const {
        if (!hasGradeFlag) return 0.0;
        return static_cast<double>(grade);
    }
    
    void setEnrollmentId(int id) { enrollmentId = id; }
    void setStudentId(int id) { studentId = id; }
    void setCourseId(int id) { courseId = id; }
    void setGrade(Grade grade) { 
        this->grade = grade; 
        hasGradeFlag = true;
    }
    void setEnrollmentDate(date date) { enrollmentDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    void setIsCompleted(bool isCompleted) { this->isCompleted = isCompleted; }
    void setCredits(int credits) { this->credits = credits; }
    
    void completeCourse(Grade grade) {
        setGrade(grade);
        isCompleted = true;
        isActive = false;
    }
    
    void withdraw() {
        isActive = false;
        isCompleted = false;
    }
    
    string getGradeLetter() const {
        switch (grade) {
            case A: return "A";
            case B_PLUS: return "B+";
            case B: return "B";
            case C_PLUS: return "C+";
            case C: return "C";
            case D: return "D";
            case F: return hasGradeFlag ? "F" : "N/A";
            default: return "N/A";
        }
    }
    
    void display() const {
        cout << "Enrollment ID: " << enrollmentId << endl;
        cout << "Student ID: " << studentId << endl;
        cout << "Course ID: " << courseId << endl;
        cout << "Grade: " << getGradeLetter() << endl;
        
        // Format date
        tm tm = {};
        tm.tm_year = enrollmentDate.year - 1900;
        tm.tm_mon = enrollmentDate.month - 1;
        tm.tm_mday = enrollmentDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Enrollment Date: " << dateStr << endl;
        
        cout << "Status: ";
        if (isCompleted) {
            cout << "Completed";
        } else if (isActive) {
            cout << "Active";
        } else {
            cout << "Withdrawn";
        }
        cout << endl;
        
        cout << "Credits: " << credits << endl;
    }
};

class EnrollmentSystem {
private:
    vector<Student> students;
    vector<Course> courses;
    vector<Enrollment> enrollments;
    int nextStudentId;
    int nextCourseId;
    int nextEnrollmentId;
    
public:
    EnrollmentSystem() {
        nextStudentId = 1;
        nextCourseId = 1;
        nextEnrollmentId = 1;
    }
    
    // Student management
    Student* findStudent(int studentId) {
        for (auto& student : students) {
            if (student.getStudentId() == studentId) {
                return &student;
            }
        }
        return nullptr;
    }
    
    bool addStudent(const Student& student) {
        if (students.size() >= MAX_STUDENTS) {
            return false;
        }
        
        students.push_back(student);
        return true;
    }
    
    bool removeStudent(int studentId) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getStudentId() == studentId) {
                students.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Student*> getAllStudents() {
        vector<Student*> result;
        for (auto& student : students) {
            result.push_back(&student);
        }
        return result;
    }
    
    vector<Student*> searchStudentsByName(const string& name) {
        vector<Student*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& student : students) {
            string fullName = student.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&student);
            }
        }
        return result;
    }
    
    vector<Student*> searchStudentsByDepartment(const string& department) {
        vector<Student*> result;
        string lowerDept = department;
        transform(lowerDept.begin(), lowerDept.end(), lowerDept.begin(), ::tolower);
        
        for (auto& student : students) {
            string studentDept = student.getDepartment();
            transform(studentDept.begin(), studentDept.end(), studentDept.begin(), ::tolower);
            
            if (studentDept.find(lowerDept) != string::npos) {
                result.push_back(&student);
            }
        }
        return result;
    }
    
    int getNextStudentId() { return nextStudentId++; }
    
    // Course management
    Course* findCourse(int courseId) {
        for (auto& course : courses) {
            if (course.getCourseId() == courseId) {
                return &course;
            }
        }
        return nullptr;
    }
    
    bool addCourse(const Course& course) {
        if (courses.size() >= MAX_COURSES) {
            return false;
        }
        
        courses.push_back(course);
        return true;
    }
    
    bool removeCourse(int courseId) {
        for (auto it = courses.begin(); it != courses.end(); ++it) {
            if (it->getCourseId() == courseId) {
                courses.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Course*> getAllCourses() {
        vector<Course*> result;
        for (auto& course : courses) {
            result.push_back(&course);
        }
        return result;
    }
    
    vector<Course*> searchCoursesByTitle(const string& title) {
        vector<Course*> result;
        string lowerTitle = title;
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        
        for (auto& course : courses) {
            string courseTitle = course.getTitle();
            transform(courseTitle.begin(), courseTitle.end(), courseTitle.begin(), ::tolower);
            
            if (courseTitle.find(lowerTitle) != string::npos) {
                result.push_back(&course);
            }
        }
        return result;
    }
    
    vector<Course*> searchCoursesByInstructor(const string& instructor) {
        vector<Course*> result;
        string lowerInstructor = instructor;
        transform(lowerInstructor.begin(), lowerInstructor.end(), lowerInstructor.begin(), ::tolower);
        
        for (auto& course : courses) {
            string courseInstructor = course.getInstructor();
            transform(courseInstructor.begin(), courseInstructor.end(), courseInstructor.begin(), ::tolower);
            
            if (courseInstructor.find(lowerInstructor) != string::npos) {
                result.push_back(&course);
            }
        }
        return result;
    }
    
    vector<Course*> getAvailableCourses() {
        vector<Course*> result;
        for (auto& course : courses) {
            if (!course.isFull()) {
                result.push_back(&course);
            }
        }
        return result;
    }
    
    int getNextCourseId() { return nextCourseId++; }
    
    // Enrollment management
    Enrollment* findEnrollment(int enrollmentId) {
        for (auto& enrollment : enrollments) {
            if (enrollment.getEnrollmentId() == enrollmentId) {
                return &enrollment;
            }
        }
        return nullptr;
    }
    
    bool addEnrollment(const Enrollment& enrollment) {
        if (enrollments.size() >= MAX_ENROLLMENTS) {
            return false;
        }
        
        enrollments.push_back(enrollment);
        return true;
    }
    
    bool removeEnrollment(int enrollmentId) {
        for (auto it = enrollments.begin(); it != enrollments.end(); ++it) {
            if (it->getEnrollmentId() == enrollmentId) {
                enrollments.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Enrollment*> getAllEnrollments() {
        vector<Enrollment*> result;
        for (auto& enrollment : enrollments) {
            result.push_back(&enrollment);
        }
        return result;
    }
    
    vector<Enrollment*> getStudentEnrollments(int studentId) {
        vector<Enrollment*> result;
        for (auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId) {
                result.push_back(&enrollment);
            }
        }
        return result;
    }
    
    vector<Enrollment*> getCourseEnrollments(int courseId) {
        vector<Enrollment*> result;
        for (auto& enrollment : enrollments) {
            if (enrollment.getCourseId() == courseId) {
                result.push_back(&enrollment);
            }
        }
        return result;
    }
    
    Enrollment* findEnrollment(int studentId, int courseId) {
        for (auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId && enrollment.getCourseId() == courseId) {
                return &enrollment;
            }
        }
        return nullptr;
    }
    
    int getNextEnrollmentId() { return nextEnrollmentId++; }
    
    // Enrollment operations
    bool enrollStudent(int studentId, int courseId) {
        // Check if student exists
        Student* student = findStudent(studentId);
        if (!student) {
            return false;
        }
        
        // Check if course exists
        Course* course = findCourse(courseId);
        if (!course) {
            return false;
        }
        
        // Check if course is full
        if (course->isFull()) {
            return false;
        }
        
        // Check if student is already enrolled
        if (findEnrollment(studentId, courseId) != nullptr) {
            return false;
        }
        
        // Create enrollment
        Enrollment enrollment(getNextEnrollmentId(), studentId, courseId);
        enrollment.setCredits(course->getCredits());
        
        if (!addEnrollment(enrollment)) {
            return false;
        }
        
        // Update course enrollment count
        course->incrementEnrollment();
        
        return true;
    }
    
    bool withdrawStudent(int studentId, int courseId) {
        // Find enrollment
        Enrollment* enrollment = findEnrollment(studentId, courseId);
        if (!enrollment || !enrollment->getIsActive()) {
            return false;
        }
        
        // Check if course exists
        Course* course = findCourse(courseId);
        if (!course) {
            return false;
        }
        
        // Withdraw student
        enrollment->withdraw();
        
        // Update course enrollment count
        course->decrementEnrollment();
        
        return true;
    }
    
    bool completeCourse(int studentId, int courseId, Grade grade) {
        // Find enrollment
        Enrollment* enrollment = findEnrollment(studentId, courseId);
        if (!enrollment || !enrollment->getIsActive()) {
            return false;
        }
        
        // Complete course with grade
        enrollment->completeCourse(grade);
        
        // Update student GPA
        Student* student = findStudent(studentId);
        if (student) {
            vector<Enrollment*> studentEnrollments = getStudentEnrollments(studentId);
            student->updateGpa(studentEnrollments);
        }
        
        return true;
    }
    
    // Reporting
    void generateStudentReport(int studentId) {
        Student* student = findStudent(studentId);
        if (!student) {
            cout << "Student not found." << endl;
            return;
        }
        
        cout << "Student Report" << endl;
        cout << "==============" << endl;
        student->display();
        cout << endl;
        
        cout << "Course Enrollments" << endl;
        cout << "------------------" << endl;
        
        vector<Enrollment*> studentEnrollments = getStudentEnrollments(studentId);
        if (studentEnrollments.empty()) {
            cout << "No enrollments found." << endl;
            return;
        }
        
        for (Enrollment* enrollment : studentEnrollments) {
            Course* course = findCourse(enrollment->getCourseId());
            if (course) {
                cout << "Course: " << course->getTitle() << " (" << course->getCourseCode() << ")" << endl;
                cout << "Credits: " << course->getCredits() << endl;
                cout << "Grade: " << enrollment->getGradeLetter() << endl;
                
                cout << "Status: ";
                if (enrollment->getIsCompleted()) {
                    cout << "Completed";
                } else if (enrollment->getIsActive()) {
                    cout << "Active";
                } else {
                    cout << "Withdrawn";
                }
                cout << endl;
                cout << endl;
            }
        }
    }
    
    void generateCourseReport(int courseId) {
        Course* course = findCourse(courseId);
        if (!course) {
            cout << "Course not found." << endl;
            return;
        }
        
        cout << "Course Report" << endl;
        cout << "=============" << endl;
        course->display();
        cout << endl;
        
        cout << "Student Enrollments" << endl;
        cout << "-------------------" << endl;
        
        vector<Enrollment*> courseEnrollments = getCourseEnrollments(courseId);
        if (courseEnrollments.empty()) {
            cout << "No enrollments found." << endl;
            return;
        }
        
        for (Enrollment* enrollment : courseEnrollments) {
            Student* student = findStudent(enrollment->getStudentId());
            if (student) {
                cout << "Student: " << student->getFullName() << " (" << student->getStudentId() << ")" << endl;
                cout << "Grade: " << enrollment->getGradeLetter() << endl;
                
                cout << "Status: ";
                if (enrollment->getIsCompleted()) {
                    cout << "Completed";
                } else if (enrollment->getIsActive()) {
                    cout << "Active";
                } else {
                    cout << "Withdrawn";
                }
                cout << endl;
                cout << endl;
            }
        }
        
        // Calculate statistics
        int completedCount = 0;
        int activeCount = 0;
        int withdrawnCount = 0;
        double totalGradePoints = 0.0;
        int gradedCount = 0;
        
        for (Enrollment* enrollment : courseEnrollments) {
            if (enrollment->getIsCompleted()) {
                completedCount++;
                if (enrollment->hasGrade()) {
                    totalGradePoints += enrollment->getGradePoints();
                    gradedCount++;
                }
            } else if (enrollment->getIsActive()) {
                activeCount++;
            } else {
                withdrawnCount++;
            }
        }
        
        cout << "Course Statistics" << endl;
        cout << "-----------------" << endl;
        cout << "Total Enrollments: " << courseEnrollments.size() << endl;
        cout << "Active: " << activeCount << endl;
        cout << "Completed: " << completedCount << endl;
        cout << "Withdrawn: " << withdrawnCount << endl;
        
        if (gradedCount > 0) {
            double averageGrade = totalGradePoints / gradedCount;
            cout << "Average Grade: " << fixed << setprecision(2) << averageGrade << endl;
        } else {
            cout << "Average Grade: N/A (no graded enrollments)" << endl;
        }
    }
    
    void generateDepartmentReport(const string& department) {
        cout << "Department Report: " << department << endl;
        cout << "============================" << endl;
        
        // Count students in department
        int studentCount = 0;
        double totalGpa = 0.0;
        
        for (const auto& student : students) {
            if (student.getDepartment() == department) {
                studentCount++;
                totalGpa += student.getGpa();
            }
        }
        
        cout << "Number of Students: " << studentCount << endl;
        
        if (studentCount > 0) {
            double averageGpa = totalGpa / studentCount;
            cout << "Average GPA: " << fixed << setprecision(2) << averageGpa << endl;
        } else {
            cout << "Average GPA: N/A" << endl;
        }
        
        cout << endl;
        
        // Count courses in department
        int courseCount = 0;
        int totalEnrollments = 0;
        int totalCapacity = 0;
        
        for (const auto& course : courses) {
            if (course.getDepartment() == department) {
                courseCount++;
                totalEnrollments += course.getCurrentEnrollment();
                totalCapacity += course.getCapacity();
            }
        }
        
        cout << "Number of Courses: " << courseCount << endl;
        cout << "Total Course Capacity: " << totalCapacity << endl;
        cout << "Total Current Enrollments: " << totalEnrollments << endl;
        
        if (totalCapacity > 0) {
            double utilizationRate = (double)totalEnrollments / totalCapacity * 100.0;
            cout << "Course Utilization Rate: " << fixed << setprecision(1) << utilizationRate << "%" << endl;
        } else {
            cout << "Course Utilization Rate: N/A" << endl;
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
        outFile << nextStudentId << "|" << nextCourseId << "|" << nextEnrollmentId << endl;
        
        // Save students
        outFile << "STUDENTS" << endl;
        for (const auto& student : students) {
            outFile << student.getStudentId() << "|" 
                    << student.getFirstName() << "|" 
                    << student.getLastName() << "|" 
                    << student.getEmail() << "|" 
                    << student.getPhone() << "|" 
                    << student.getAddress() << "|" 
                    << student.getEnrollmentDate().year << "|" 
                    << student.getEnrollmentDate().month << "|" 
                    << student.getEnrollmentDate().day << "|" 
                    << student.getDepartment() << "|" 
                    << student.getYearOfStudy() << "|" 
                    << fixed << setprecision(2) << student.getGpa() << "|" 
                    << student.getTotalCredits() << endl;
        }
        
        // Save courses
        outFile << "COURSES" << endl;
        for (const auto& course : courses) {
            outFile << course.getCourseId() << "|" 
                    << course.getCourseCode() << "|" 
                    << course.getTitle() << "|" 
                    << course.getDescription() << "|" 
                    << course.getInstructor() << "|" 
                    << course.getDepartment() << "|" 
                    << course.getCredits() << "|" 
                    << course.getCapacity() << "|" 
                    << course.getCurrentEnrollment() << "|" 
                    << course.getSemester() << "|" 
                    << course.getYear() << "|" 
                    << course.getSchedule() << "|" 
                    << course.getLocation() << endl;
        }
        
        // Save enrollments
        outFile << "ENROLLMENTS" << endl;
        for (const auto& enrollment : enrollments) {
            outFile << enrollment.getEnrollmentId() << "|" 
                    << enrollment.getStudentId() << "|" 
                    << enrollment.getCourseId() << "|" 
                    << static_cast<int>(enrollment.getGrade()) << "|" 
                    << (enrollment.hasGrade() ? "1" : "0") << "|" 
                    << enrollment.getEnrollmentDate().year << "|" 
                    << enrollment.getEnrollmentDate().month << "|" 
                    << enrollment.getEnrollmentDate().day << "|" 
                    << (enrollment.getIsActive() ? "1" : "0") << "|" 
                    << (enrollment.getIsCompleted() ? "1" : "0") << "|" 
                    << enrollment.getCredits() << endl;
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
            } else if (line == "STUDENTS") {
                section = "STUDENTS";
                continue;
            } else if (line == "COURSES") {
                section = "COURSES";
                continue;
            } else if (line == "ENROLLMENTS") {
                section = "ENROLLMENTS";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "NEXT_IDS" && tokens.size() >= 3) {
                nextStudentId = stoi(tokens[0]);
                nextCourseId = stoi(tokens[1]);
                nextEnrollmentId = stoi(tokens[2]);
            } else if (section == "STUDENTS" && tokens.size() >= 13) {
                int studentId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string email = tokens[3];
                string phone = tokens[4];
                string address = tokens[5];
                
                date enrollmentDate;
                enrollmentDate.year = stoi(tokens[6]);
                enrollmentDate.month = stoi(tokens[7]);
                enrollmentDate.day = stoi(tokens[8]);
                
                string department = tokens[9];
                int yearOfStudy = stoi(tokens[10]);
                double gpa = stod(tokens[11]);
                int totalCredits = stoi(tokens[12]);
                
                Student student(studentId, firstName, lastName, email, phone, address, 
                               enrollmentDate, department, yearOfStudy);
                student.setGpa(gpa);
                student.setTotalCredits(totalCredits);
                
                students.push_back(student);
            } else if (section == "COURSES" && tokens.size() >= 14) {
                int courseId = stoi(tokens[0]);
                string courseCode = tokens[1];
                string title = tokens[2];
                string description = tokens[3];
                string instructor = tokens[4];
                string department = tokens[5];
                int credits = stoi(tokens[6]);
                int capacity = stoi(tokens[7]);
                int currentEnrollment = stoi(tokens[8]);
                string semester = tokens[9];
                int year = stoi(tokens[10]);
                string schedule = tokens[11];
                string location = tokens[12];
                
                Course course(courseId, courseCode, title, description, instructor, department, credits, capacity);
                course.setCurrentEnrollment(currentEnrollment);
                course.setSemester(semester);
                course.setYear(year);
                course.setSchedule(schedule);
                course.setLocation(location);
                
                courses.push_back(course);
            } else if (section == "ENROLLMENTS" && tokens.size() >= 11) {
                int enrollmentId = stoi(tokens[0]);
                int studentId = stoi(tokens[1]);
                int courseId = stoi(tokens[2]);
                Grade grade = static_cast<Grade>(stoi(tokens[3]));
                bool hasGradeFlag = (tokens[4] == "1");
                
                date enrollmentDate;
                enrollmentDate.year = stoi(tokens[5]);
                enrollmentDate.month = stoi(tokens[6]);
                enrollmentDate.day = stoi(tokens[7]);
                
                bool isActive = (tokens[8] == "1");
                bool isCompleted = (tokens[9] == "1");
                int credits = stoi(tokens[10]);
                
                Enrollment enrollment(enrollmentId, studentId, courseId);
                
                if (hasGradeFlag) {
                    enrollment.setGrade(grade);
                }
                
                enrollment.setEnrollmentDate(enrollmentDate);
                enrollment.setIsActive(isActive);
                enrollment.setIsCompleted(isCompleted);
                enrollment.setCredits(credits);
                
                enrollments.push_back(enrollment);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        students.clear();
        courses.clear();
        enrollments.clear();
        nextStudentId = 1;
        nextCourseId = 1;
        nextEnrollmentId = 1;
        
        // Create sample students
        vector<Student> sampleStudents = {
            Student(getNextStudentId(), "John", "Smith", "john.smith@example.com", "555-1234", "123 Main St", {2020, 9, 1}, "Computer Science", 3),
            Student(getNextStudentId(), "Jane", "Johnson", "jane.johnson@example.com", "555-5678", "456 Oak Ave", {2020, 9, 1}, "Computer Science", 3),
            Student(getNextStudentId(), "Michael", "Brown", "michael.brown@example.com", "555-9012", "789 Pine Rd", {2019, 9, 1}, "Mathematics", 4),
            Student(getNextStudentId(), "Sarah", "Davis", "sarah.davis@example.com", "555-3456", "321 Elm St", {2020, 9, 1}, "Physics", 3),
            Student(getNextStudentId(), "David", "Wilson", "david.wilson@example.com", "555-7890", "654 Maple Dr", {2021, 9, 1}, "Computer Science", 2),
            Student(getNextStudentId(), "Emily", "Taylor", "emily.taylor@example.com", "555-2345", "987 Cedar Ln", {2020, 9, 1}, "Mathematics", 3),
            Student(getNextStudentId(), "Robert", "Anderson", "robert.anderson@example.com", "555-6789", "147 Birch Way", {2021, 9, 1}, "Physics", 2),
            Student(getNextStudentId(), "Lisa", "Miller", "lisa.miller@example.com", "555-1357", "258 Spruce St", {2019, 9, 1}, "Computer Science", 4)
        };
        
        for (const auto& student : sampleStudents) {
            addStudent(student);
        }
        
        // Create sample courses
        vector<Course> sampleCourses = {
            Course(getNextCourseId(), "CS101", "Introduction to Computer Science", "Basic concepts of programming", "Dr. Smith", "Computer Science", 3, 30),
            Course(getNextCourseId(), "CS201", "Data Structures", "Fundamental data structures", "Dr. Johnson", "Computer Science", 4, 25),
            Course(getNextCourseId(), "CS301", "Algorithms", "Design and analysis of algorithms", "Dr. Brown", "Computer Science", 4, 20),
            Course(getNextCourseId(), "MATH101", "Calculus I", "Differential and integral calculus", "Dr. Davis", "Mathematics", 4, 30),
            Course(getNextCourseId(), "MATH201", "Linear Algebra", "Matrix theory and linear systems", "Dr. Wilson", "Mathematics", 3, 25),
            Course(getNextCourseId(), "PHYS101", "Physics I", "Mechanics and thermodynamics", "Dr. Anderson", "Physics", 4, 25),
            Course(getNextCourseId(), "PHYS201", "Physics II", "Electromagnetism and optics", "Dr. Taylor", "Physics", 4, 20),
            Course(getNextCourseId(), "CS401", "Database Systems", "Database design and implementation", "Dr. Miller", "Computer Science", 3, 15)
        };
        
        for (const auto& course : sampleCourses) {
            addCourse(course);
        }
        
        // Create sample enrollments
        // Student 1 (John Smith)
        enrollStudent(1, 1);  // CS101
        enrollStudent(1, 2);  // CS201
        enrollStudent(1, 4);  // MATH101
        
        // Student 2 (Jane Johnson)
        enrollStudent(2, 1);  // CS101
        enrollStudent(2, 3);  // CS301
        enrollStudent(2, 5);  // MATH201
        
        // Student 3 (Michael Brown)
        enrollStudent(3, 4);  // MATH101
        enrollStudent(3, 5);  // MATH201
        
        // Student 4 (Sarah Davis)
        enrollStudent(4, 6);  // PHYS101
        enrollStudent(4, 7);  // PHYS201
        
        // Student 5 (David Wilson)
        enrollStudent(5, 1);  // CS101
        enrollStudent(5, 8);  // CS401
        
        // Set some grades for completed courses
        completeCourse(1, 1, Grade::A);      // John Smith - CS101
        completeCourse(1, 4, Grade::B);      // John Smith - MATH101
        completeCourse(2, 1, Grade::B_PLUS); // Jane Johnson - CS101
        completeCourse(3, 4, Grade::A);      // Michael Brown - MATH101
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Students: " << students.size() << endl;
        cout << "Courses: " << courses.size() << endl;
        cout << "Enrollments: " << enrollments.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(EnrollmentSystem& system, int choice);
void studentMenu(EnrollmentSystem& system);
void courseMenu(EnrollmentSystem& system);
void enrollmentMenu(EnrollmentSystem& system);
void reportMenu(EnrollmentSystem& system);
void fileMenu(EnrollmentSystem& system);

int main() {
    EnrollmentSystem system;
    
    cout << "University Enrollment System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("enrollment_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("enrollment_data.txt");
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
    system.saveToFile("enrollment_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "University Enrollment System" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Course Management" << endl;
    cout << "3. Enrollment Management" << endl;
    cout << "4. Reports" << endl;
    cout << "5. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(EnrollmentSystem& system, int choice) {
    switch (choice) {
        case 1:
            studentMenu(system);
            break;
        case 2:
            courseMenu(system);
            break;
        case 3:
            enrollmentMenu(system);
            break;
        case 4:
            reportMenu(system);
            break;
        case 5:
            fileMenu(system);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void studentMenu(EnrollmentSystem& system) {
    int choice;
    do {
        cout << "Student Management" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Remove Student" << endl;
        cout << "3. List All Students" << endl;
        cout << "4. Search Students by Name" << endl;
        cout << "5. Search Students by Department" << endl;
        cout << "6. Generate Student Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, email, phone, address, department;
                int yearOfStudy;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Address: ";
                getline(cin, address);
                
                cout << "Enter Department: ";
                getline(cin, department);
                
                cout << "Enter Year of Study: ";
                cin >> yearOfStudy;
                
                // Get current date for enrollment
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date enrollmentDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                Student student(system.getNextStudentId(), firstName, lastName, email, phone, address, 
                               enrollmentDate, department, yearOfStudy);
                
                if (system.addStudent(student)) {
                    cout << "Student added successfully." << endl;
                } else {
                    cout << "Failed to add student. Maximum number of students reached." << endl;
                }
                break;
            }
            case 2: {
                int studentId;
                cout << "Enter Student ID to remove: ";
                cin >> studentId;
                
                if (system.removeStudent(studentId)) {
                    cout << "Student removed successfully." << endl;
                } else {
                    cout << "Failed to remove student. Student ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Student*> students = system.getAllStudents();
                if (students.empty()) {
                    cout << "No students found." << endl;
                } else {
                    cout << "Students (" << students.size() << "):" << endl;
                    for (Student* student : students) {
                        cout << "ID: " << student->getStudentId() 
                             << ", Name: " << student->getFullName()
                             << ", Department: " << student->getDepartment() << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                
                vector<Student*> students = system.searchStudentsByName(name);
                if (students.empty()) {
                    cout << "No students found matching the name." << endl;
                } else {
                    cout << "Students matching '" << name << "' (" << students.size() << "):" << endl;
                    for (Student* student : students) {
                        cout << "ID: " << student->getStudentId() 
                             << ", Name: " << student->getFullName()
                             << ", Department: " << student->getDepartment() << endl;
                    }
                }
                break;
            }
            case 5: {
                string department;
                cout << "Enter department to search: ";
                getline(cin, department);
                
                vector<Student*> students = system.searchStudentsByDepartment(department);
                if (students.empty()) {
                    cout << "No students found in the department." << endl;
                } else {
                    cout << "Students in '" << department << "' (" << students.size() << "):" << endl;
                    for (Student* student : students) {
                        cout << "ID: " << student->getStudentId() 
                             << ", Name: " << student->getFullName()
                             << ", Year: " << student->getYearOfStudy() << endl;
                    }
                }
                break;
            }
            case 6: {
                int studentId;
                cout << "Enter Student ID: ";
                cin >> studentId;
                
                system.generateStudentReport(studentId);
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

void courseMenu(EnrollmentSystem& system) {
    int choice;
    do {
        cout << "Course Management" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. Remove Course" << endl;
        cout << "3. List All Courses" << endl;
        cout << "4. Search Courses by Title" << endl;
        cout << "5. Search Courses by Instructor" << endl;
        cout << "6. List Available Courses" << endl;
        cout << "7. Generate Course Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string courseCode, title, description, instructor, department;
                int credits, capacity;
                
                cout << "Enter Course Code: ";
                getline(cin, courseCode);
                
                cout << "Enter Course Title: ";
                getline(cin, title);
                
                cout << "Enter Course Description: ";
                getline(cin, description);
                
                cout << "Enter Instructor Name: ";
                getline(cin, instructor);
                
                cout << "Enter Department: ";
                getline(cin, department);
                
                cout << "Enter Credits: ";
                cin >> credits;
                
                cout << "Enter Capacity: ";
                cin >> capacity;
                
                Course course(system.getNextCourseId(), courseCode, title, description, instructor, 
                             department, credits, capacity);
                
                if (system.addCourse(course)) {
                    cout << "Course added successfully." << endl;
                } else {
                    cout << "Failed to add course. Maximum number of courses reached." << endl;
                }
                break;
            }
            case 2: {
                int courseId;
                cout << "Enter Course ID to remove: ";
                cin >> courseId;
                
                if (system.removeCourse(courseId)) {
                    cout << "Course removed successfully." << endl;
                } else {
                    cout << "Failed to remove course. Course ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Course*> courses = system.getAllCourses();
                if (courses.empty()) {
                    cout << "No courses found." << endl;
                } else {
                    cout << "Courses (" << courses.size() << "):" << endl;
                    for (Course* course : courses) {
                        cout << "ID: " << course->getCourseId()
                             << ", Code: " << course->getCourseCode()
                             << ", Title: " << course->getTitle()
                             << ", Enrollment: " << course->getCurrentEnrollment() 
                             << "/" << course->getCapacity() << endl;
                    }
                }
                break;
            }
            case 4: {
                string title;
                cout << "Enter title to search: ";
                getline(cin, title);
                
                vector<Course*> courses = system.searchCoursesByTitle(title);
                if (courses.empty()) {
                    cout << "No courses found matching the title." << endl;
                } else {
                    cout << "Courses matching '" << title << "' (" << courses.size() << "):" << endl;
                    for (Course* course : courses) {
                        cout << "ID: " << course->getCourseId()
                             << ", Code: " << course->getCourseCode()
                             << ", Title: " << course->getTitle() << endl;
                    }
                }
                break;
            }
            case 5: {
                string instructor;
                cout << "Enter instructor name to search: ";
                getline(cin, instructor);
                
                vector<Course*> courses = system.searchCoursesByInstructor(instructor);
                if (courses.empty()) {
                    cout << "No courses found for the instructor." << endl;
                } else {
                    cout << "Courses by '" << instructor << "' (" << courses.size() << "):" << endl;
                    for (Course* course : courses) {
                        cout << "ID: " << course->getCourseId()
                             << ", Code: " << course->getCourseCode()
                             << ", Title: " << course->getTitle() << endl;
                    }
                }
                break;
            }
            case 6: {
                vector<Course*> courses = system.getAvailableCourses();
                if (courses.empty()) {
                    cout << "No available courses found." << endl;
                } else {
                    cout << "Available Courses (" << courses.size() << "):" << endl;
                    for (Course* course : courses) {
                        cout << "ID: " << course->getCourseId()
                             << ", Code: " << course->getCourseCode()
                             << ", Title: " << course->getTitle()
                             << ", Available: " << (course->getCapacity() - course->getCurrentEnrollment()) << endl;
                    }
                }
                break;
            }
            case 7: {
                int courseId;
                cout << "Enter Course ID: ";
                cin >> courseId;
                
                system.generateCourseReport(courseId);
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

void enrollmentMenu(EnrollmentSystem& system) {
    int choice;
    do {
        cout << "Enrollment Management" << endl;
        cout << "1. Enroll Student in Course" << endl;
        cout << "2. Withdraw Student from Course" << endl;
        cout << "3. Complete Course with Grade" << endl;
        cout << "4. List All Enrollments" << endl;
        cout << "5. List Student Enrollments" << endl;
        cout << "6. List Course Enrollments" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int studentId, courseId;
                cout << "Enter Student ID: ";
                cin >> studentId;
                cout << "Enter Course ID: ";
                cin >> courseId;
                
                if (system.enrollStudent(studentId, courseId)) {
                    cout << "Student enrolled successfully." << endl;
                } else {
                    cout << "Failed to enroll student. Student or course may not exist, course may be full, or student may already be enrolled." << endl;
                }
                break;
            }
            case 2: {
                int studentId, courseId;
                cout << "Enter Student ID: ";
                cin >> studentId;
                cout << "Enter Course ID: ";
                cin >> courseId;
                
                if (system.withdrawStudent(studentId, courseId)) {
                    cout << "Student withdrawn successfully." << endl;
                } else {
                    cout << "Failed to withdraw student. Enrollment may not exist or may not be active." << endl;
                }
                break;
            }
            case 3: {
                int studentId, courseId;
                int gradeChoice;
                
                cout << "Enter Student ID: ";
                cin >> studentId;
                cout << "Enter Course ID: ";
                cin >> courseId;
                
                cout << "Select Grade:" << endl;
                cout << "1. A (4.0)" << endl;
                cout << "2. B+ (3.5)" << endl;
                cout << "3. B (3.0)" << endl;
                cout << "4. C+ (2.5)" << endl;
                cout << "5. C (2.0)" << endl;
                cout << "6. D (1.0)" << endl;
                cout << "7. F (0.0)" << endl;
                cout << "Enter choice: ";
                cin >> gradeChoice;
                
                Grade grade;
                switch (gradeChoice) {
                    case 1: grade = Grade::A; break;
                    case 2: grade = Grade::B_PLUS; break;
                    case 3: grade = Grade::B; break;
                    case 4: grade = Grade::C_PLUS; break;
                    case 5: grade = Grade::C; break;
                    case 6: grade = Grade::D; break;
                    case 7: grade = Grade::F; break;
                    default: grade = Grade::F; break;
                }
                
                if (system.completeCourse(studentId, courseId, grade)) {
                    cout << "Course completed successfully." << endl;
                } else {
                    cout << "Failed to complete course. Enrollment may not exist or may not be active." << endl;
                }
                break;
            }
            case 4: {
                vector<Enrollment*> enrollments = system.getAllEnrollments();
                if (enrollments.empty()) {
                    cout << "No enrollments found." << endl;
                } else {
                    cout << "Enrollments (" << enrollments.size() << "):" << endl;
                    for (Enrollment* enrollment : enrollments) {
                        cout << "ID: " << enrollment->getEnrollmentId()
                             << ", Student: " << enrollment->getStudentId()
                             << ", Course: " << enrollment->getCourseId()
                             << ", Grade: " << enrollment->getGradeLetter() << endl;
                    }
                }
                break;
            }
            case 5: {
                int studentId;
                cout << "Enter Student ID: ";
                cin >> studentId;
                
                vector<Enrollment*> enrollments = system.getStudentEnrollments(studentId);
                if (enrollments.empty()) {
                    cout << "No enrollments found for the student." << endl;
                } else {
                    cout << "Enrollments for Student " << studentId << " (" << enrollments.size() << "):" << endl;
                    for (Enrollment* enrollment : enrollments) {
                        cout << "ID: " << enrollment->getEnrollmentId()
                             << ", Course: " << enrollment->getCourseId()
                             << ", Grade: " << enrollment->getGradeLetter();
                        
                        if (enrollment->getIsCompleted()) {
                            cout << " (Completed)";
                        } else if (enrollment->getIsActive()) {
                            cout << " (Active)";
                        } else {
                            cout << " (Withdrawn)";
                        }
                        cout << endl;
                    }
                }
                break;
            }
            case 6: {
                int courseId;
                cout << "Enter Course ID: ";
                cin >> courseId;
                
                vector<Enrollment*> enrollments = system.getCourseEnrollments(courseId);
                if (enrollments.empty()) {
                    cout << "No enrollments found for the course." << endl;
                } else {
                    cout << "Enrollments for Course " << courseId << " (" << enrollments.size() << "):" << endl;
                    for (Enrollment* enrollment : enrollments) {
                        cout << "ID: " << enrollment->getEnrollmentId()
                             << ", Student: " << enrollment->getStudentId()
                             << ", Grade: " << enrollment->getGradeLetter();
                        
                        if (enrollment->getIsCompleted()) {
                            cout << " (Completed)";
                        } else if (enrollment->getIsActive()) {
                            cout << " (Active)";
                        } else {
                            cout << " (Withdrawn)";
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

void reportMenu(EnrollmentSystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Generate Student Report" << endl;
        cout << "2. Generate Course Report" << endl;
        cout << "3. Generate Department Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int studentId;
                cout << "Enter Student ID: ";
                cin >> studentId;
                
                system.generateStudentReport(studentId);
                break;
            }
            case 2: {
                int courseId;
                cout << "Enter Course ID: ";
                cin >> courseId;
                
                system.generateCourseReport(courseId);
                break;
            }
            case 3: {
                string department;
                cout << "Enter Department: ";
                getline(cin, department);
                
                system.generateDepartmentReport(department);
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

void fileMenu(EnrollmentSystem& system) {
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
                    filename = "enrollment_data.txt";
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
                    filename = "enrollment_data.txt";
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