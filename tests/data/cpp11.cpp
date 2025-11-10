#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <memory>
#include <functional>
#include <stdexcept>

using namespace std;

#define MAX_STUDENTS 1000
#define MAX_COURSES 100
#define MAX_ENROLLMENTS 2000

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

class Person {
protected:
    string firstName;
    string lastName;
    string email;
    string phoneNumber;
    
public:
    Person(const string& fname = "", const string& lname = "", 
           const string& email = "", const string& phone = "")
        : firstName(fname), lastName(lname), email(email), phoneNumber(phone) {}
    
    virtual ~Person() {}
    
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getEmail() const { return email; }
    string getPhoneNumber() const { return phoneNumber; }
    
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPhoneNumber(const string& phone) { phoneNumber = phone; }
    
    virtual void display() const {
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phoneNumber << endl;
    }
};

class Student : public Person {
private:
    int studentId;
    Date birthDate;
    Date enrollmentDate;
    string major;
    double gpa;
    int creditsCompleted;
    bool active;
    
public:
    Student(int id = 0, const string& fname = "", const string& lname = "", 
            const string& email = "", const string& phone = "",
            const Date& birth = Date(), const string& major = "Undeclared")
        : Person(fname, lname, email, phone), studentId(id), birthDate(birth), major(major) {
        // Set enrollment date to today
        time_t now = time(nullptr);
        tm* timeinfo = localtime(&now);
        enrollmentDate = Date(timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
        
        gpa = 0.0;
        creditsCompleted = 0;
        active = true;
    }
    
    int getStudentId() const { return studentId; }
    Date getBirthDate() const { return birthDate; }
    Date getEnrollmentDate() const { return enrollmentDate; }
    string getMajor() const { return major; }
    double getGPA() const { return gpa; }
    int getCreditsCompleted() const { return creditsCompleted; }
    bool isActive() const { return active; }
    
    void setStudentId(int id) { studentId = id; }
    void setBirthDate(const Date& date) { birthDate = date; }
    void setMajor(const string& m) { major = m; }
    void setGPA(double g) { gpa = g; }
    void setCreditsCompleted(int credits) { creditsCompleted = credits; }
    void setActive(bool status) { active = status; }
    
    void updateGPA(double newGPA) {
        gpa = newGPA;
    }
    
    void addCredits(int credits) {
        creditsCompleted += credits;
    }
    
    void display() const override {
        Person::display();
        cout << "Student ID: " << studentId << endl;
        cout << "Birth Date: " << birthDate.toString() << endl;
        cout << "Enrollment Date: " << enrollmentDate.toString() << endl;
        cout << "Major: " << major << endl;
        cout << "GPA: " << fixed << setprecision(2) << gpa << endl;
        cout << "Credits Completed: " << creditsCompleted << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class Course {
private:
    int courseId;
    string courseCode;
    string courseName;
    string description;
    int credits;
    int maxCapacity;
    int currentEnrollment;
    bool active;
    
public:
    Course(int id = 0, const string& code = "", const string& name = "", 
           const string& desc = "", int credits = 3, int capacity = 50)
        : courseId(id), courseCode(code), courseName(name), description(desc), 
          credits(credits), maxCapacity(capacity) {
        currentEnrollment = 0;
        active = true;
    }
    
    int getCourseId() const { return courseId; }
    string getCourseCode() const { return courseCode; }
    string getCourseName() const { return courseName; }
    string getDescription() const { return description; }
    int getCredits() const { return credits; }
    int getMaxCapacity() const { return maxCapacity; }
    int getCurrentEnrollment() const { return currentEnrollment; }
    bool isActive() const { return active; }
    
    void setCourseId(int id) { courseId = id; }
    void setCourseCode(const string& code) { courseCode = code; }
    void setCourseName(const string& name) { courseName = name; }
    void setDescription(const string& desc) { description = desc; }
    void setCredits(int c) { credits = c; }
    void setMaxCapacity(int capacity) { maxCapacity = capacity; }
    void setActive(bool status) { active = status; }
    
    bool enrollStudent() {
        if (currentEnrollment < maxCapacity) {
            currentEnrollment++;
            return true;
        }
        return false;
    }
    
    void dropStudent() {
        if (currentEnrollment > 0) {
            currentEnrollment--;
        }
    }
    
    void display() const {
        cout << "Course ID: " << courseId << endl;
        cout << "Course Code: " << courseCode << endl;
        cout << "Course Name: " << courseName << endl;
        cout << "Description: " << description << endl;
        cout << "Credits: " << credits << endl;
        cout << "Enrollment: " << currentEnrollment << "/" << maxCapacity << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class Enrollment {
private:
    int enrollmentId;
    int studentId;
    int courseId;
    Date enrollmentDate;
    string status; // "Active", "Completed", "Dropped", "Withdrawn"
    double finalGrade;
    
public:
    Enrollment(int id = 0, int sid = 0, int cid = 0, const string& status = "Active")
        : enrollmentId(id), studentId(sid), courseId(cid), status(status) {
        // Set enrollment date to today
        time_t now = time(nullptr);
        tm* timeinfo = localtime(&now);
        enrollmentDate = Date(timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
        
        finalGrade = -1.0; // Not graded yet
    }
    
    int getEnrollmentId() const { return enrollmentId; }
    int getStudentId() const { return studentId; }
    int getCourseId() const { return courseId; }
    Date getEnrollmentDate() const { return enrollmentDate; }
    string getStatus() const { return status; }
    double getFinalGrade() const { return finalGrade; }
    
    void setEnrollmentId(int id) { enrollmentId = id; }
    void setStudentId(int id) { studentId = id; }
    void setCourseId(int id) { courseId = id; }
    void setEnrollmentDate(const Date& date) { enrollmentDate = date; }
    void setStatus(const string& s) { status = s; }
    void setFinalGrade(double grade) { finalGrade = grade; }
    
    string getLetterGrade() const {
        if (finalGrade < 0) return "N/A";
        if (finalGrade >= 90) return "A";
        if (finalGrade >= 85) return "A-";
        if (finalGrade >= 80) return "B+";
        if (finalGrade >= 75) return "B";
        if (finalGrade >= 70) return "B-";
        if (finalGrade >= 65) return "C+";
        if (finalGrade >= 60) return "C";
        if (finalGrade >= 55) return "C-";
        if (finalGrade >= 50) return "D+";
        if (finalGrade >= 45) return "D";
        if (finalGrade >= 40) return "D-";
        return "F";
    }
    
    void display() const {
        cout << "Enrollment ID: " << enrollmentId << endl;
        cout << "Student ID: " << studentId << endl;
        cout << "Course ID: " << courseId << endl;
        cout << "Enrollment Date: " << enrollmentDate.toString() << endl;
        cout << "Status: " << status << endl;
        cout << "Final Grade: ";
        if (finalGrade < 0) {
            cout << "Not graded yet" << endl;
        } else {
            cout << fixed << setprecision(1) << finalGrade << " (" << getLetterGrade() << ")" << endl;
        }
    }
};

class University {
private:
    vector<Student> students;
    vector<Course> courses;
    vector<Enrollment> enrollments;
    map<string, int> studentEmailMap;
    map<int, int> studentIdMap;
    map<int, int> courseIdMap;
    
public:
    University() {}
    
    // Student management
    bool addStudent(const Student& student) {
        if (students.size() >= MAX_STUDENTS) {
            return false;
        }
        
        // Check if email already exists
        if (studentEmailMap.find(student.getEmail()) != studentEmailMap.end()) {
            return false;
        }
        
        students.push_back(student);
        int index = students.size() - 1;
        studentEmailMap[student.getEmail()] = index;
        studentIdMap[student.getStudentId()] = index;
        
        return true;
    }
    
    bool removeStudent(int studentId) {
        auto it = studentIdMap.find(studentId);
        if (it == studentIdMap.end()) {
            return false;
        }
        
        int index = it->second;
        students[index].setActive(false);
        
        // Remove all enrollments for this student
        for (auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId && enrollment.getStatus() == "Active") {
                enrollment.setStatus("Withdrawn");
            }
        }
        
        return true;
    }
    
    Student* findStudent(int studentId) {
        auto it = studentIdMap.find(studentId);
        if (it == studentIdMap.end() || !students[it->second].isActive()) {
            return nullptr;
        }
        
        return &students[it->second];
    }
    
    Student* findStudentByEmail(const string& email) {
        auto it = studentEmailMap.find(email);
        if (it == studentEmailMap.end() || !students[it->second].isActive()) {
            return nullptr;
        }
        
        return &students[it->second];
    }
    
    vector<Student*> getAllStudents() {
        vector<Student*> result;
        for (auto& student : students) {
            if (student.isActive()) {
                result.push_back(&student);
            }
        }
        return result;
    }
    
    // Course management
    bool addCourse(const Course& course) {
        if (courses.size() >= MAX_COURSES) {
            return false;
        }
        
        courses.push_back(course);
        int index = courses.size() - 1;
        courseIdMap[course.getCourseId()] = index;
        
        return true;
    }
    
    bool removeCourse(int courseId) {
        auto it = courseIdMap.find(courseId);
        if (it == courseIdMap.end()) {
            return false;
        }
        
        int index = it->second;
        courses[index].setActive(false);
        
        // Remove all enrollments for this course
        for (auto& enrollment : enrollments) {
            if (enrollment.getCourseId() == courseId && enrollment.getStatus() == "Active") {
                enrollment.setStatus("Cancelled");
            }
        }
        
        return true;
    }
    
    Course* findCourse(int courseId) {
        auto it = courseIdMap.find(courseId);
        if (it == courseIdMap.end() || !courses[it->second].isActive()) {
            return nullptr;
        }
        
        return &courses[it->second];
    }
    
    vector<Course*> getAllCourses() {
        vector<Course*> result;
        for (auto& course : courses) {
            if (course.isActive()) {
                result.push_back(&course);
            }
        }
        return result;
    }
    
    // Enrollment management
    bool enrollStudent(int studentId, int courseId) {
        Student* student = findStudent(studentId);
        Course* course = findCourse(courseId);
        
        if (!student || !course) {
            return false;
        }
        
        // Check if already enrolled
        for (const auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId && 
                enrollment.getCourseId() == courseId &&
                (enrollment.getStatus() == "Active" || enrollment.getStatus() == "Completed")) {
                return false;
            }
        }
        
        // Check course capacity
        if (!course->enrollStudent()) {
            return false;
        }
        
        // Create new enrollment
        int enrollmentId = enrollments.size() + 1;
        enrollments.push_back(Enrollment(enrollmentId, studentId, courseId, "Active"));
        
        return true;
    }
    
    bool dropStudent(int studentId, int courseId) {
        for (auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId && 
                enrollment.getCourseId() == courseId &&
                enrollment.getStatus() == "Active") {
                
                enrollment.setStatus("Dropped");
                
                // Update course enrollment
                Course* course = findCourse(courseId);
                if (course) {
                    course->dropStudent();
                }
                
                return true;
            }
        }
        
        return false;
    }
    
    bool assignGrade(int studentId, int courseId, double grade) {
        for (auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId && 
                enrollment.getCourseId() == courseId &&
                enrollment.getStatus() == "Active") {
                
                enrollment.setFinalGrade(grade);
                enrollment.setStatus("Completed");
                
                // Update student GPA and credits
                Student* student = findStudent(studentId);
                Course* course = findCourse(courseId);
                
                if (student && course) {
                    student->addCredits(course->getCredits());
                    
                    // Simple GPA calculation
                    double newGPA = (student->getGPA() * student->getCreditsCompleted() + grade * course->getCredits()) /
                                  (student->getCreditsCompleted() + course->getCredits());
                    student->updateGPA(newGPA);
                }
                
                return true;
            }
        }
        
        return false;
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
    
    // Reports
    void displayAllStudents() const {
        cout << "=== All Students ===" << endl;
        for (const auto& student : students) {
            if (student.isActive()) {
                student.display();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayAllCourses() const {
        cout << "=== All Courses ===" << endl;
        for (const auto& course : courses) {
            if (course.isActive()) {
                course.display();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayStudentTranscript(int studentId) const {
        const Student* student = nullptr;
        for (const auto& s : students) {
            if (s.getStudentId() == studentId && s.isActive()) {
                student = &s;
                break;
            }
        }
        
        if (!student) {
            cout << "Student not found." << endl;
            return;
        }
        
        cout << "=== Student Transcript ===" << endl;
        student->display();
        cout << endl;
        
        cout << "Course Enrollments:" << endl;
        cout << "ID\tCourse Code\tCourse Name\t\tCredits\tGrade" << endl;
        cout << "--------------------------------------------------------" << endl;
        
        double totalGradePoints = 0.0;
        int totalCredits = 0;
        
        for (const auto& enrollment : enrollments) {
            if (enrollment.getStudentId() == studentId) {
                const Course* course = nullptr;
                for (const auto& c : courses) {
                    if (c.getCourseId() == enrollment.getCourseId() && c.isActive()) {
                        course = &c;
                        break;
                    }
                }
                
                if (course) {
                    cout << course->getCourseId() << "\t" 
                         << course->getCourseCode() << "\t" 
                         << course->getCourseName() << "\t\t" 
                         << course->getCredits() << "\t";
                    
                    if (enrollment.getFinalGrade() >= 0) {
                        cout << enrollment.getFinalGrade() << " (" << enrollment.getLetterGrade() << ")";
                        
                        // Calculate grade points
                        double gradePoints = 0.0;
                        double grade = enrollment.getFinalGrade();
                        if (grade >= 90) gradePoints = 4.0;
                        else if (grade >= 85) gradePoints = 3.7;
                        else if (grade >= 80) gradePoints = 3.3;
                        else if (grade >= 75) gradePoints = 3.0;
                        else if (grade >= 70) gradePoints = 2.7;
                        else if (grade >= 65) gradePoints = 2.3;
                        else if (grade >= 60) gradePoints = 2.0;
                        else if (grade >= 55) gradePoints = 1.7;
                        else if (grade >= 50) gradePoints = 1.3;
                        else if (grade >= 45) gradePoints = 1.0;
                        else gradePoints = 0.0;
                        
                        totalGradePoints += gradePoints * course->getCredits();
                        totalCredits += course->getCredits();
                    } else {
                        cout << "In Progress";
                    }
                    
                    cout << endl;
                }
            }
        }
        
        cout << "--------------------------------------------------------" << endl;
        cout << "Total Credits: " << totalCredits << endl;
        if (totalCredits > 0) {
            cout << "Calculated GPA: " << fixed << setprecision(2) << totalGradePoints / totalCredits << endl;
        } else {
            cout << "Calculated GPA: 0.00" << endl;
        }
    }
    
    void displayCourseRoster(int courseId) const {
        const Course* course = nullptr;
        for (const auto& c : courses) {
            if (c.getCourseId() == courseId && c.isActive()) {
                course = &c;
                break;
            }
        }
        
        if (!course) {
            cout << "Course not found." << endl;
            return;
        }
        
        cout << "=== Course Roster ===" << endl;
        course->display();
        cout << endl;
        
        cout << "Enrolled Students:" << endl;
        cout << "ID\tName\t\t\tEmail\t\t\tGrade" << endl;
        cout << "-----------------------------------------------------------------" << endl;
        
        for (const auto& enrollment : enrollments) {
            if (enrollment.getCourseId() == courseId) {
                const Student* student = nullptr;
                for (const auto& s : students) {
                    if (s.getStudentId() == enrollment.getStudentId() && s.isActive()) {
                        student = &s;
                        break;
                    }
                }
                
                if (student) {
                    cout << student->getStudentId() << "\t" 
                         << student->getFirstName() << " " << student->getLastName() << "\t\t" 
                         << student->getEmail() << "\t";
                    
                    if (enrollment.getFinalGrade() >= 0) {
                        cout << enrollment.getFinalGrade() << " (" << enrollment.getLetterGrade() << ")";
                    } else {
                        cout << "In Progress";
                    }
                    
                    cout << endl;
                }
            }
        }
        
        cout << "-----------------------------------------------------------------" << endl;
        cout << "Total Enrolled: " << course->getCurrentEnrollment() << endl;
    }
    
    void displayAcademicStatistics() const {
        cout << "=== Academic Statistics ===" << endl;
        
        // Student statistics
        int totalStudents = 0;
        int activeStudents = 0;
        double totalGPA = 0.0;
        int studentsWithGPA = 0;
        
        for (const auto& student : students) {
            totalStudents++;
            if (student.isActive()) {
                activeStudents++;
                if (student.getGPA() > 0) {
                    totalGPA += student.getGPA();
                    studentsWithGPA++;
                }
            }
        }
        
        cout << "Student Statistics:" << endl;
        cout << "Total Students: " << totalStudents << endl;
        cout << "Active Students: " << activeStudents << endl;
        if (studentsWithGPA > 0) {
            cout << "Average GPA: " << fixed << setprecision(2) << totalGPA / studentsWithGPA << endl;
        } else {
            cout << "Average GPA: 0.00" << endl;
        }
        cout << endl;
        
        // Course statistics
        int totalCourses = 0;
        int activeCourses = 0;
        int totalEnrollments = 0;
        
        for (const auto& course : courses) {
            totalCourses++;
            if (course.isActive()) {
                activeCourses++;
                totalEnrollments += course.getCurrentEnrollment();
            }
        }
        
        cout << "Course Statistics:" << endl;
        cout << "Total Courses: " << totalCourses << endl;
        cout << "Active Courses: " << activeCourses << endl;
        cout << "Total Enrollments: " << totalEnrollments << endl;
        if (activeCourses > 0) {
            cout << "Average Enrollment per Course: " << fixed << setprecision(1) 
                 << (double)totalEnrollments / activeCourses << endl;
        }
        cout << endl;
        
        // Enrollment statistics
        int activeEnrollments = 0;
        int completedEnrollments = 0;
        int droppedEnrollments = 0;
        double totalFinalGrades = 0.0;
        int gradedEnrollments = 0;
        
        for (const auto& enrollment : enrollments) {
            if (enrollment.getStatus() == "Active") {
                activeEnrollments++;
            } else if (enrollment.getStatus() == "Completed") {
                completedEnrollments++;
                if (enrollment.getFinalGrade() >= 0) {
                    totalFinalGrades += enrollment.getFinalGrade();
                    gradedEnrollments++;
                }
            } else if (enrollment.getStatus() == "Dropped") {
                droppedEnrollments++;
            }
        }
        
        cout << "Enrollment Statistics:" << endl;
        cout << "Active Enrollments: " << activeEnrollments << endl;
        cout << "Completed Enrollments: " << completedEnrollments << endl;
        cout << "Dropped Enrollments: " << droppedEnrollments << endl;
        
        if (gradedEnrollments > 0) {
            cout << "Average Final Grade: " << fixed << setprecision(1) 
                 << totalFinalGrades / gradedEnrollments << endl;
        }
        
        // Grade distribution
        cout << endl << "Grade Distribution:" << endl;
        int aCount = 0, bCount = 0, cCount = 0, dCount = 0, fCount = 0;
        
        for (const auto& enrollment : enrollments) {
            if (enrollment.getStatus() == "Completed" && enrollment.getFinalGrade() >= 0) {
                double grade = enrollment.getFinalGrade();
                if (grade >= 80) aCount++;
                else if (grade >= 70) bCount++;
                else if (grade >= 60) cCount++;
                else if (grade >= 50) dCount++;
                else fCount++;
            }
        }
        
        if (gradedEnrollments > 0) {
            cout << "A (80-100): " << aCount << " (" << fixed << setprecision(1) 
                 << (double)aCount / gradedEnrollments * 100 << "%)" << endl;
            cout << "B (70-79): " << bCount << " (" << fixed << setprecision(1) 
                 << (double)bCount / gradedEnrollments * 100 << "%)" << endl;
            cout << "C (60-69): " << cCount << " (" << fixed << setprecision(1) 
                 << (double)cCount / gradedEnrollments * 100 << "%)" << endl;
            cout << "D (50-59): " << dCount << " (" << fixed << setprecision(1) 
                 << (double)dCount / gradedEnrollments * 100 << "%)" << endl;
            cout << "F (0-49): " << fCount << " (" << fixed << setprecision(1) 
                 << (double)fCount / gradedEnrollments * 100 << "%)" << endl;
        }
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save students
        outFile << "STUDENTS" << endl;
        for (const auto& student : students) {
            outFile << student.getStudentId() << "|" 
                    << student.getFirstName() << "|" 
                    << student.getLastName() << "|" 
                    << student.getEmail() << "|" 
                    << student.getPhoneNumber() << "|" 
                    << student.getBirthDate().toString() << "|" 
                    << student.getEnrollmentDate().toString() << "|" 
                    << student.getMajor() << "|" 
                    << student.getGPA() << "|" 
                    << student.getCreditsCompleted() << "|" 
                    << (student.isActive() ? "1" : "0") << endl;
        }
        
        // Save courses
        outFile << "COURSES" << endl;
        for (const auto& course : courses) {
            outFile << course.getCourseId() << "|" 
                    << course.getCourseCode() << "|" 
                    << course.getCourseName() << "|" 
                    << course.getDescription() << "|" 
                    << course.getCredits() << "|" 
                    << course.getMaxCapacity() << "|" 
                    << course.getCurrentEnrollment() << "|" 
                    << (course.isActive() ? "1" : "0") << endl;
        }
        
        // Save enrollments
        outFile << "ENROLLMENTS" << endl;
        for (const auto& enrollment : enrollments) {
            outFile << enrollment.getEnrollmentId() << "|" 
                    << enrollment.getStudentId() << "|" 
                    << enrollment.getCourseId() << "|" 
                    << enrollment.getEnrollmentDate().toString() << "|" 
                    << enrollment.getStatus() << "|" 
                    << enrollment.getFinalGrade() << endl;
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
            if (line == "STUDENTS") {
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
            
            if (section == "STUDENTS" && tokens.size() >= 11) {
                int studentId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string email = tokens[3];
                string phone = tokens[4];
                
                // Parse dates
                vector<string> birthDateTokens;
                stringstream birthSs(tokens[5]);
                string birthToken;
                while (getline(birthSs, birthToken, '/')) {
                    birthDateTokens.push_back(birthToken);
                }
                
                vector<string> enrollDateTokens;
                stringstream enrollSs(tokens[6]);
                string enrollToken;
                while (getline(enrollSs, enrollToken, '/')) {
                    enrollDateTokens.push_back(enrollToken);
                }
                
                Date birthDate(stoi(birthDateTokens[0]), stoi(birthDateTokens[1]), stoi(birthDateTokens[2]));
                Date enrollDate(stoi(enrollDateTokens[0]), stoi(enrollDateTokens[1]), stoi(enrollDateTokens[2]));
                
                string major = tokens[7];
                double gpa = stod(tokens[8]);
                int credits = stoi(tokens[9]);
                bool active = tokens[10] == "1";
                
                Student student(studentId, firstName, lastName, email, phone, birthDate, major);
                student.setEnrollmentDate(enrollDate);
                student.setGPA(gpa);
                student.setCreditsCompleted(credits);
                student.setActive(active);
                
                addStudent(student);
            } else if (section == "COURSES" && tokens.size() >= 8) {
                int courseId = stoi(tokens[0]);
                string courseCode = tokens[1];
                string courseName = tokens[2];
                string description = tokens[3];
                int credits = stoi(tokens[4]);
                int maxCapacity = stoi(tokens[5]);
                int currentEnrollment = stoi(tokens[6]);
                bool active = tokens[7] == "1";
                
                Course course(courseId, courseCode, courseName, description, credits, maxCapacity);
                course.setCurrentEnrollment(currentEnrollment);
                course.setActive(active);
                
                addCourse(course);
            } else if (section == "ENROLLMENTS" && tokens.size() >= 6) {
                int enrollmentId = stoi(tokens[0]);
                int studentId = stoi(tokens[1]);
                int courseId = stoi(tokens[2]);
                
                // Parse date
                vector<string> dateTokens;
                stringstream dateSs(tokens[3]);
                string dateToken;
                while (getline(dateSs, dateToken, '/')) {
                    dateTokens.push_back(dateToken);
                }
                
                Date enrollDate(stoi(dateTokens[0]), stoi(dateTokens[1]), stoi(dateTokens[2]));
                
                string status = tokens[4];
                double finalGrade = stod(tokens[5]);
                
                Enrollment enrollment(enrollmentId, studentId, courseId, status);
                enrollment.setEnrollmentDate(enrollDate);
                enrollment.setFinalGrade(finalGrade);
                
                enrollments.push_back(enrollment);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Utility functions
    void generateSampleData() {
        // Sample departments
        vector<string> majors = {"Computer Science", "Mathematics", "Physics", "Chemistry", "Biology",
                               "English", "History", "Economics", "Psychology", "Sociology"};
        
        vector<string> firstNames = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                                   "David", "Jessica", "William", "Ashley", "James", "Amanda",
                                   "Daniel", "Jennifer", "Matthew", "Melissa"};
        
        vector<string> lastNames = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                                  "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
        
        vector<string> courseCodes = {"CS", "MATH", "PHYS", "CHEM", "BIO", "ENG", "HIST", "ECON", "PSYCH", "SOC"};
        
        vector<string> courseNames = {"Introduction to Programming", "Data Structures", "Algorithms",
                                     "Database Systems", "Software Engineering", "Computer Networks",
                                     "Artificial Intelligence", "Machine Learning", "Web Development",
                                     "Operating Systems"};
        
        // Create sample students
        for (int i = 0; i < 50; i++) {
            int studentId = 1000 + i;
            string firstName = firstNames[rand() % firstNames.size()];
            string lastName = lastNames[rand() % lastNames.size()];
            string email = firstName + "." + lastName + to_string(studentId) + "@university.edu";
            string phone = "555-" + to_string(100 + rand() % 900);
            
            // Random birth date (18-25 years old)
            int birthYear = 1998 + rand() % 8;
            int birthMonth = 1 + rand() % 12;
            int birthDay = 1 + rand() % 28;
            Date birthDate(birthDay, birthMonth, birthYear);
            
            string major = majors[rand() % majors.size()];
            
            Student student(studentId, firstName, lastName, email, phone, birthDate, major);
            addStudent(student);
        }
        
        // Create sample courses
        for (int i = 0; i < 20; i++) {
            int courseId = 100 + i;
            string courseCode = courseCodes[rand() % courseCodes.size()] + to_string(100 + i);
            string courseName = courseNames[rand() % courseNames.size()];
            string description = "Comprehensive study of " + courseName + " with theoretical foundations and practical applications.";
            int credits = 1 + rand() % 4;
            int capacity = 20 + rand() % 80;
            
            Course course(courseId, courseCode, courseName, description, credits, capacity);
            addCourse(course);
        }
        
        // Create sample enrollments
        for (int i = 0; i < 100; i++) {
            int studentId = 1000 + rand() % 50;
            int courseId = 100 + rand() % 20;
            
            if (enrollStudent(studentId, courseId)) {
                // Randomly assign grades to some enrollments
                if (rand() % 3 == 0) {
                    double grade = 40.0 + (rand() % 60) + (rand() % 100) / 100.0;
                    assignGrade(studentId, courseId, grade);
                }
            }
        }
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(University& university, int choice);
void addStudentMenu(University& university);
void addCourseMenu(University& university);
void enrollStudentMenu(University& university);
void dropStudentMenu(University& university);
void assignGradeMenu(University& university);
void viewStudentTranscriptMenu(University& university);
void viewCourseRosterMenu(University& university);

int main() {
    University university;
    
    cout << "University Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!university.loadFromFile("university_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        university.generateSampleData();
        university.saveToFile("university_data.txt");
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
        
        handleMenuChoice(university, choice);
        
        cout << endl;
        system("pause");
        system("cls");
    } while (choice != 0);
    
    // Save data before exiting
    university.saveToFile("university_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "University Management System" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Add Course" << endl;
    cout << "3. Enroll Student in Course" << endl;
    cout << "4. Drop Student from Course" << endl;
    cout << "5. Assign Grade" << endl;
    cout << "6. View All Students" << endl;
    cout << "7. View All Courses" << endl;
    cout << "8. View Student Transcript" << endl;
    cout << "9. View Course Roster" << endl;
    cout << "10. View Academic Statistics" << endl;
    cout << "11. Save Data to File" << endl;
    cout << "12. Load Data from File" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(University& university, int choice) {
    switch (choice) {
        case 1:
            addStudentMenu(university);
            break;
        case 2:
            addCourseMenu(university);
            break;
        case 3:
            enrollStudentMenu(university);
            break;
        case 4:
            dropStudentMenu(university);
            break;
        case 5:
            assignGradeMenu(university);
            break;
        case 6:
            university.displayAllStudents();
            break;
        case 7:
            university.displayAllCourses();
            break;
        case 8:
            viewStudentTranscriptMenu(university);
            break;
        case 9:
            viewCourseRosterMenu(university);
            break;
        case 10:
            university.displayAcademicStatistics();
            break;
        case 11:
            if (university.saveToFile("university_data.txt")) {
                cout << "Data saved successfully." << endl;
            } else {
                cout << "Failed to save data." << endl;
            }
            break;
        case 12:
            if (university.loadFromFile("university_data.txt")) {
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

void addStudentMenu(University& university) {
    int studentId;
    string firstName, lastName, email, phone, major;
    int day, month, year;
    
    cout << "Enter Student ID: ";
    cin >> studentId;
    cin.ignore();
    
    cout << "Enter First Name: ";
    getline(cin, firstName);
    
    cout << "Enter Last Name: ";
    getline(cin, lastName);
    
    cout << "Enter Email: ";
    getline(cin, email);
    
    cout << "Enter Phone: ";
    getline(cin, phone);
    
    cout << "Enter Birth Date (DD MM YYYY): ";
    cin >> day >> month >> year;
    
    cin.ignore();
    cout << "Enter Major: ";
    getline(cin, major);
    
    Date birthDate(day, month, year);
    Student student(studentId, firstName, lastName, email, phone, birthDate, major);
    
    if (university.addStudent(student)) {
        cout << "Student added successfully." << endl;
    } else {
        cout << "Failed to add student. The student ID or email may already exist." << endl;
    }
}

void addCourseMenu(University& university) {
    int courseId, credits, capacity;
    string courseCode, courseName, description;
    
    cout << "Enter Course ID: ";
    cin >> courseId;
    cin.ignore();
    
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    
    cout << "Enter Course Name: ";
    getline(cin, courseName);
    
    cout << "Enter Course Description: ";
    getline(cin, description);
    
    cout << "Enter Credits: ";
    cin >> credits;
    
    cout << "Enter Maximum Capacity: ";
    cin >> capacity;
    
    Course course(courseId, courseCode, courseName, description, credits, capacity);
    
    if (university.addCourse(course)) {
        cout << "Course added successfully." << endl;
    } else {
        cout << "Failed to add course. The course ID may already exist." << endl;
    }
}

void enrollStudentMenu(University& university) {
    int studentId, courseId;
    
    cout << "Enter Student ID: ";
    cin >> studentId;
    
    cout << "Enter Course ID: ";
    cin >> courseId;
    
    if (university.enrollStudent(studentId, courseId)) {
        cout << "Student enrolled successfully." << endl;
    } else {
        cout << "Failed to enroll student. Check if the student and course exist, or if the course is full." << endl;
    }
}

void dropStudentMenu(University& university) {
    int studentId, courseId;
    
    cout << "Enter Student ID: ";
    cin >> studentId;
    
    cout << "Enter Course ID: ";
    cin >> courseId;
    
    if (university.dropStudent(studentId, courseId)) {
        cout << "Student dropped successfully." << endl;
    } else {
        cout << "Failed to drop student. Check if the enrollment exists." << endl;
    }
}

void assignGradeMenu(University& university) {
    int studentId, courseId;
    double grade;
    
    cout << "Enter Student ID: ";
    cin >> studentId;
    
    cout << "Enter Course ID: ";
    cin >> courseId;
    
    cout << "Enter Grade (0-100): ";
    cin >> grade;
    
    if (university.assignGrade(studentId, courseId, grade)) {
        cout << "Grade assigned successfully." << endl;
    } else {
        cout << "Failed to assign grade. Check if the enrollment exists." << endl;
    }
}

void viewStudentTranscriptMenu(University& university) {
    int studentId;
    
    cout << "Enter Student ID: ";
    cin >> studentId;
    
    university.displayStudentTranscript(studentId);
}

void viewCourseRosterMenu(University& university) {
    int courseId;
    
    cout << "Enter Course ID: ";
    cin >> courseId;
    
    university.displayCourseRoster(courseId);
}