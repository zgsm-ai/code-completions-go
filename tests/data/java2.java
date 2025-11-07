/**
 * Student Information Management System
 * 
 * This class implements a comprehensive student information management system
 * that can handle student records, courses, grades, and attendance.
 * 
 * Features:
 * - Student profile management
 * - Course enrollment and management
 * - Grade tracking and calculation
 * - Attendance tracking
 * - Academic performance analysis
 * - Class schedule management
 * - Teacher information
 */

import java.util.*;
import java.text.*;
import java.io.*;

/**
 * Address class represents a physical address
 */
class Address {
    private String street;
    private String city;
    private String state;
    private String zipCode;
    private String country;
    
    /**
     * Constructor for Address
     * @param street Street address
     * @param city City
     * @param state State/province
     * @param zipCode ZIP/postal code
     * @param country Country
     */
    public Address(String street, String city, String state, String zipCode, String country) {
        this.street = street;
        this.city = city;
        this.state = state;
        this.zipCode = zipCode;
        this.country = country;
    }
    
    /**
     * Get street address
     * @return Street address
     */
    public String getStreet() {
        return street;
    }
    
    /**
     * Get city
     * @return City
     */
    public String getCity() {
        return city;
    }
    
    /**
     * Get state/province
     * @return State/province
     */
    public String getState() {
        return state;
    }
    
    /**
     * Get ZIP/postal code
     * @return ZIP/postal code
     */
    public String getZipCode() {
        return zipCode;
    }
    
    /**
     * Get country
     * @return Country
     */
    public String getCountry() {
        return country;
    }
    
    /**
     * Get full address as string
     * @return Full address string
     */
    public String getFullAddress() {
        return street + ", " + city + ", " + state + " " + zipCode + ", " + country;
    }
    
    /**
     * Get address summary
     * @return Address summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Street: ").append(street).append("\n");
        sb.append("City: ").append(city).append("\n");
        sb.append("State: ").append(state).append("\n");
        sb.append("ZIP Code: ").append(zipCode).append("\n");
        sb.append("Country: ").append(country).append("\n");
        return sb.toString();
    }
}

/**
 * Course class represents a course
 */
class Course {
    private String courseId;
    private String courseName;
    private String courseCode;
    private int credits;
    private String department;
    private String instructor;
    private String semester;
    private int year;
    private String schedule;
    private List<Student> enrolledStudents;
    
    /**
     * Constructor for Course
     * @param courseId Course ID
     * @param courseName Course name
     * @param courseCode Course code
     * @param credits Course credits
     * @param department Department
     * @param instructor Instructor name
     * @param semester Semester
     * @param year Year
     * @param schedule Schedule information
     */
    public Course(String courseId, String courseName, String courseCode, int credits,
               String department, String instructor, String semester, int year, String schedule) {
        this.courseId = courseId;
        this.courseName = courseName;
        this.courseCode = courseCode;
        this.credits = credits;
        this.department = department;
        this.instructor = instructor;
        this.semester = semester;
        this.year = year;
        this.schedule = schedule;
        this.enrolledStudents = new ArrayList<>();
    }
    
    /**
     * Get course ID
     * @return Course ID
     */
    public String getCourseId() {
        return courseId;
    }
    
    /**
     * Get course name
     * @return Course name
     */
    public String getCourseName() {
        return courseName;
    }
    
    /**
     * Get course code
     * @return Course code
     */
    public String getCourseCode() {
        return courseCode;
    }
    
    /**
     * Get course credits
     * @return Course credits
     */
    public int getCredits() {
        return credits;
    }
    
    /**
     * Get department
     * @return Department
     */
    public String getDepartment() {
        return department;
    }
    
    /**
     * Get instructor
     * @return Instructor name
     */
    public String getInstructor() {
        return instructor;
    }
    
    /**
     * Get semester
     * @return Semester
     */
    public String getSemester() {
        return semester;
    }
    
    /**
     * Get year
     * @return Year
     */
    public int getYear() {
        return year;
    }
    
    /**
     * Get schedule
     * @return Schedule information
     */
    public String getSchedule() {
        return schedule;
    }
    
    /**
     * Get enrolled students
     * @return List of enrolled students
     */
    public List<Student> getEnrolledStudents() {
        return enrolledStudents;
    }
    
    /**
     * Enroll a student
     * @param student Student to enroll
     * @return True if enrolled successfully, false otherwise
     */
    public boolean enrollStudent(Student student) {
        if (enrolledStudents.contains(student)) {
            return false;
        }
        
        enrolledStudents.add(student);
        student.enrollInCourse(this);
        return true;
    }
    
    /**
     * Drop a student
     * @param studentId Student ID to drop
     * @return True if dropped successfully, false otherwise
     */
    public boolean dropStudent(String studentId) {
        for (int i = 0; i < enrolledStudents.size(); i++) {
            if (enrolledStudents.get(i).getStudentId().equals(studentId)) {
                Student student = enrolledStudents.get(i);
                student.dropFromCourse(this.courseId);
                enrolledStudents.remove(i);
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * Get course summary
     * @return Course summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Course ID: ").append(courseId).append("\n");
        sb.append("Course Name: ").append(courseName).append("\n");
        sb.append("Course Code: ").append(courseCode).append("\n");
        sb.append("Credits: ").append(credits).append("\n");
        sb.append("Department: ").append(department).append("\n");
        sb.append("Instructor: ").append(instructor).append("\n");
        sb.append("Semester: ").append(semester).append(" ").append(year).append("\n");
        sb.append("Schedule: ").append(schedule).append("\n");
        sb.append("Enrolled Students: ").append(enrolledStudents.size()).append("\n");
        
        return sb.toString();
    }
}

/**
 * Grade class represents a student grade
 */
class Grade {
    private String courseId;
    private String studentId;
    private String gradeType;
    private double score;
    private double maxScore;
    private Date gradeDate;
    
    /**
     * Constructor for Grade
     * @param courseId Course ID
     * @param studentId Student ID
     * @param gradeType Type of grade (assignment, exam, etc.)
     * @param score Score received
     * @param maxScore Maximum possible score
     * @param gradeDate Date grade was assigned
     */
    public Grade(String courseId, String studentId, String gradeType,
               double score, double maxScore, Date gradeDate) {
        this.courseId = courseId;
        this.studentId = studentId;
        this.gradeType = gradeType;
        this.score = score;
        this.maxScore = maxScore;
        this.gradeDate = gradeDate;
    }
    
    /**
     * Get course ID
     * @return Course ID
     */
    public String getCourseId() {
        return courseId;
    }
    
    /**
     * Get student ID
     * @return Student ID
     */
    public String getStudentId() {
        return studentId;
    }
    
    /**
     * Get grade type
     * @return Grade type
     */
    public String getGradeType() {
        return gradeType;
    }
    
    /**
     * Get score
     * @return Score
     */
    public double getScore() {
        return score;
    }
    
    /**
     * Get maximum score
     * @return Maximum score
     */
    public double getMaxScore() {
        return maxScore;
    }
    
    /**
     * Get grade percentage
     * @return Percentage score
     */
    public double getPercentage() {
        if (maxScore == 0) {
            return 0.0;
        }
        return (score / maxScore) * 100.0;
    }
    
    /**
     * Get letter grade
     * @return Letter grade
     */
    public String getLetterGrade() {
        double percentage = getPercentage();
        
        if (percentage >= 97) {
            return "A+";
        } else if (percentage >= 93) {
            return "A";
        } else if (percentage >= 90) {
            return "A-";
        } else if (percentage >= 87) {
            return "B+";
        } else if (percentage >= 83) {
            return "B";
        } else if (percentage >= 80) {
            return "B-";
        } else if (percentage >= 77) {
            return "C+";
        } else if (percentage >= 73) {
            return "C";
        } else if (percentage >= 70) {
            return "C-";
        } else if (percentage >= 67) {
            return "D+";
        } else if (percentage >= 63) {
            return "D";
        } else if (percentage >= 60) {
            return "D-";
        } else {
            return "F";
        }
    }
    
    /**
     * Get grade date
     * @return Grade date
     */
    public Date getGradeDate() {
        return gradeDate;
    }
    
    /**
     * Get grade summary
     * @return Grade summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Course ID: ").append(courseId).append("\n");
        sb.append("Student ID: ").append(studentId).append("\n");
        sb.append("Grade Type: ").append(gradeType).append("\n");
        sb.append("Score: ").append(score).append(" / ").append(maxScore).append("\n");
        sb.append("Percentage: ").append(String.format("%.2f%%", getPercentage())).append("\n");
        sb.append("Letter Grade: ").append(getLetterGrade()).append("\n");
        sb.append("Grade Date: ").append(gradeDate).append("\n");
        
        return sb.toString();
    }
}

/**
 * Attendance class represents attendance record
 */
class Attendance {
    private String courseId;
    private String studentId;
    private Date attendanceDate;
    private boolean present;
    private String status;
    
    /**
     * Constructor for Attendance
     * @param courseId Course ID
     * @param studentId Student ID
     * @param attendanceDate Attendance date
     * @param present Whether student was present
     * @param status Attendance status (Present, Absent, Late, Excused)
     */
    public Attendance(String courseId, String studentId, Date attendanceDate, boolean present, String status) {
        this.courseId = courseId;
        this.studentId = studentId;
        this.attendanceDate = attendanceDate;
        this.present = present;
        this.status = status;
    }
    
    /**
     * Get course ID
     * @return Course ID
     */
    public String getCourseId() {
        return courseId;
    }
    
    /**
     * Get student ID
     * @return Student ID
     */
    public String getStudentId() {
        return studentId;
    }
    
    /**
     * Get attendance date
     * @return Attendance date
     */
    public Date getAttendanceDate() {
        return attendanceDate;
    }
    
    /**
     * Check if student was present
     * @return True if present, false otherwise
     */
    public boolean isPresent() {
        return present;
    }
    
    /**
     * Get attendance status
     * @return Attendance status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get attendance summary
     * @return Attendance summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Course ID: ").append(courseId).append("\n");
        sb.append("Student ID: ").append(studentId).append("\n");
        sb.append("Date: ").append(attendanceDate).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Present: ").append(present ? "Yes" : "No").append("\n");
        
        return sb.toString();
    }
}

/**
 * Student class represents a student
 */
class Student {
    private String studentId;
    private String firstName;
    private String lastName;
    private Date birthDate;
    private String gender;
    private Address address;
    private String phone;
    private String email;
    private String major;
    private String classLevel;  // Freshman, Sophomore, Junior, Senior
    private Date admissionDate;
    private double gpa;
    private List<String> enrolledCourseIds;
    private Map<String, List<Grade>> courseGrades;
    private Map<String, List<Attendance>> courseAttendance;
    
    /**
     * Constructor for Student
     * @param studentId Student ID
     * @param firstName First name
     * @param lastName Last name
     * @param birthDate Birth date
     * @param gender Gender
     * @param address Address
     * @param phone Phone number
     * @param email Email address
     * @param major Major
     * @param classLevel Class level
     * @param admissionDate Admission date
     */
    public Student(String studentId, String firstName, String lastName, Date birthDate,
                String gender, Address address, String phone, String email,
                String major, String classLevel, Date admissionDate) {
        this.studentId = studentId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.birthDate = birthDate;
        this.gender = gender;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.major = major;
        this.classLevel = classLevel;
        this.admissionDate = admissionDate;
        this.gpa = 0.0;
        this.enrolledCourseIds = new ArrayList<>();
        this.courseGrades = new HashMap<>();
        this.courseAttendance = new HashMap<>();
    }
    
    /**
     * Get student ID
     * @return Student ID
     */
    public String getStudentId() {
        return studentId;
    }
    
    /**
     * Get first name
     * @return First name
     */
    public String getFirstName() {
        return firstName;
    }
    
    /**
     * Get last name
     * @return Last name
     */
    public String getLastName() {
        return lastName;
    }
    
    /**
     * Get full name
     * @return Full name
     */
    public String getFullName() {
        return firstName + " " + lastName;
    }
    
    /**
     * Get birth date
     * @return Birth date
     */
    public Date getBirthDate() {
        return birthDate;
    }
    
    /**
     * Get gender
     * @return Gender
     */
    public String getGender() {
        return gender;
    }
    
    /**
     * Get address
     * @return Address
     */
    public Address getAddress() {
        return address;
    }
    
    /**
     * Get phone number
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get email address
     * @return Email address
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get major
     * @return Major
     */
    public String getMajor() {
        return major;
    }
    
    /**
     * Get class level
     * @return Class level
     */
    public String getClassLevel() {
        return classLevel;
    }
    
    /**
     * Get admission date
     * @return Admission date
     */
    public Date getAdmissionDate() {
        return admissionDate;
    }
    
    /**
     * Get GPA
     * @return GPA
     */
    public double getGpa() {
        return gpa;
    }
    
    /**
     * Get enrolled course IDs
     * @return List of enrolled course IDs
     */
    public List<String> getEnrolledCourseIds() {
        return enrolledCourseIds;
    }
    
    /**
     * Get course grades
     * @return Map of course IDs to grade lists
     */
    public Map<String, List<Grade>> getCourseGrades() {
        return courseGrades;
    }
    
    /**
     * Get course attendance
     * @return Map of course IDs to attendance lists
     */
    public Map<String, List<Attendance>> getCourseAttendance() {
        return courseAttendance;
    }
    
    /**
     * Enroll in a course
     * @param courseId Course ID
     * @return True if enrolled successfully, false otherwise
     */
    public boolean enrollInCourse(String courseId) {
        if (enrolledCourseIds.contains(courseId)) {
            return false;
        }
        
        enrolledCourseIds.add(courseId);
        return true;
    }
    
    /**
     * Drop from a course
     * @param courseId Course ID
     * @return True if dropped successfully, false otherwise
     */
    public boolean dropFromCourse(String courseId) {
        if (enrolledCourseIds.contains(courseId)) {
            enrolledCourseIds.remove(courseId);
            courseGrades.remove(courseId);
            courseAttendance.remove(courseId);
            return true;
        }
        
        return false;
    }
    
    /**
     * Add a grade for a course
     * @param grade Grade to add
     */
    public void addGrade(Grade grade) {
        String courseId = grade.getCourseId();
        
        if (!courseGrades.containsKey(courseId)) {
            courseGrades.put(courseId, new ArrayList<>());
        }
        
        courseGrades.get(courseId).add(grade);
        
        // Update GPA
        updateGpa();
    }
    
    /**
     * Get grades for a course
     * @param courseId Course ID
     * @return List of grades
     */
    public List<Grade> getGradesForCourse(String courseId) {
        if (courseGrades.containsKey(courseId)) {
            return courseGrades.get(courseId);
        }
        
        return new ArrayList<>();
    }
    
    /**
     * Get final grade for a course
     * @param courseId Course ID
     * @return Final grade
     */
    public String getFinalGradeForCourse(String courseId) {
        List<Grade> grades = getGradesForCourse(courseId);
        if (grades.isEmpty()) {
            return "N/A";
        }
        
        // Calculate average percentage
        double totalPercentage = 0.0;
        for (Grade grade : grades) {
            totalPercentage += grade.getPercentage();
        }
        double averagePercentage = totalPercentage / grades.size();
        
        // Convert to letter grade
        if (averagePercentage >= 97) {
            return "A+";
        } else if (averagePercentage >= 93) {
            return "A";
        } else if (averagePercentage >= 90) {
            return "A-";
        } else if (averagePercentage >= 87) {
            return "B+";
        } else if (averagePercentage >= 83) {
            return "B";
        } else if (averagePercentage >= 80) {
            return "B-";
        } else if (averagePercentage >= 77) {
            return "C+";
        } else if (averagePercentage >= 73) {
            return "C";
        } else if (averagePercentage >= 70) {
            return "C-";
        } else if (averagePercentage >= 67) {
            return "D+";
        } else if (averagePercentage >= 63) {
            return "D";
        } else if (averagePercentage >= 60) {
            return "D-";
        } else {
            return "F";
        }
    }
    
    /**
     * Add attendance for a course
     * @param attendance Attendance record
     */
    public void addAttendance(Attendance attendance) {
        String courseId = attendance.getCourseId();
        
        if (!courseAttendance.containsKey(courseId)) {
            courseAttendance.put(courseId, new ArrayList<>());
        }
        
        courseAttendance.get(courseId).add(attendance);
    }
    
    /**
     * Get attendance for a course
     * @param courseId Course ID
     * @return List of attendance records
     */
    public List<Attendance> getAttendanceForCourse(String courseId) {
        if (courseAttendance.containsKey(courseId)) {
            return courseAttendance.get(courseId);
        }
        
        return new ArrayList<>();
    }
    
    /**
     * Get attendance percentage for a course
     * @param courseId Course ID
     * @return Attendance percentage
     */
    public double getAttendancePercentageForCourse(String courseId) {
        List<Attendance> attendanceRecords = getAttendanceForCourse(courseId);
        if (attendanceRecords.isEmpty()) {
            return 0.0;
        }
        
        int presentCount = 0;
        for (Attendance record : attendanceRecords) {
            if (record.isPresent()) {
                presentCount++;
            }
        }
        
        return ((double) presentCount / attendanceRecords.size()) * 100.0;
    }
    
    /**
     * Update GPA based on all grades
     */
    private void updateGpa() {
        double totalPoints = 0.0;
        int totalCredits = 0;
        
        // Get all courses with grades
        Set<String> coursesWithGrades = courseGrades.keySet();
        
        for (String courseId : coursesWithGrades) {
            List<Grade> grades = courseGrades.get(courseId);
            if (!grades.isEmpty()) {
                continue;
            }
            
            // Get final letter grade
            String letterGrade = getFinalGradeForCourse(courseId);
            
            // Convert to GPA points
            double gradePoints = 0.0;
            if (letterGrade.startsWith("A")) {
                gradePoints = 4.0;
                if (letterGrade.equals("A+")) {
                    gradePoints = 4.0;
                } else if (letterGrade.equals("A")) {
                    gradePoints = 4.0;
                } else if (letterGrade.equals("A-")) {
                    gradePoints = 3.7;
                }
            } else if (letterGrade.startsWith("B")) {
                if (letterGrade.equals("B+")) {
                    gradePoints = 3.3;
                } else if (letterGrade.equals("B")) {
                    gradePoints = 3.0;
                } else if (letterGrade.equals("B-")) {
                    gradePoints = 2.7;
                }
            } else if (letterGrade.startsWith("C")) {
                if (letterGrade.equals("C+")) {
                    gradePoints = 2.3;
                } else if (letterGrade.equals("C")) {
                    gradePoints = 2.0;
                } else if (letterGrade.equals("C-")) {
                    gradePoints = 1.7;
                }
            } else if (letterGrade.startsWith("D")) {
                if (letterGrade.equals("D+")) {
                    gradePoints = 1.3;
                } else if (letterGrade.equals("D")) {
                    gradePoints = 1.0;
                } else if (letterGrade.equals("D-")) {
                    gradePoints = 0.7;
                }
            }
            // F grades are 0.0 points
            
            // Find course credits (simplified, would need course info)
            int credits = 3; // Default to 3 credits
            
            totalPoints += gradePoints * credits;
            totalCredits += credits;
        }
        
        // Calculate GPA
        if (totalCredits > 0) {
            gpa = totalPoints / totalCredits;
        } else {
            gpa = 0.0;
        }
    }
    
    /**
     * Get student summary
     * @return Student summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Student ID: ").append(studentId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Birth Date: ").append(birthDate).append("\n");
        sb.append("Gender: ").append(gender).append("\n");
        sb.append("Major: ").append(major).append("\n");
        sb.append("Class Level: ").append(classLevel).append("\n");
        sb.append("Admission Date: ").append(admissionDate).append("\n");
        sb.append("GPA: ").append(String.format("%.2f", gpa)).append("\n");
        sb.append("Enrolled Courses: ").append(enrolledCourseIds.size()).append("\n");
        
        if (address != null) {
            sb.append("Address:\n").append(address.getSummary()).append("\n");
        }
        
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Email: ").append(email).append("\n");
        
        return sb.toString();
    }
}

/**
 * StudentInformationSystem class manages the entire student information system
 */
public class StudentInformationSystem {
    private String institutionName;
    private List<Student> students;
    private Map<String, Student> studentMap;
    private List<Course> courses;
    private Map<String, Course> courseMap;
    
    /**
     * Constructor for StudentInformationSystem
     * @param institutionName Name of the institution
     */
    public StudentInformationSystem(String institutionName) {
        this.institutionName = institutionName;
        this.students = new ArrayList<>();
        this.courses = new ArrayList<>();
        this.studentMap = new HashMap<>();
        this.courseMap = new HashMap<>();
    }
    
    /**
     * Get institution name
     * @return Institution name
     */
    public String getInstitutionName() {
        return institutionName;
    }
    
    /**
     * Get students
     * @return List of students
     */
    public List<Student> getStudents() {
        return students;
    }
    
    /**
     * Get courses
     * @return List of courses
     */
    public List<Course> getCourses() {
        return courses;
    }
    
    /**
     * Add a student
     * @param student Student to add
     * @return True if added successfully, false otherwise
     */
    public boolean addStudent(Student student) {
        if (studentMap.containsKey(student.getStudentId())) {
            return false;
        }
        
        students.add(student);
        studentMap.put(student.getStudentId(), student);
        return true;
    }
    
    /**
     * Get a student by ID
     * @param studentId Student ID
     * @return Student if found, null otherwise
     */
    public Student getStudent(String studentId) {
        return studentMap.get(studentId);
    }
    
    /**
     * Add a course
     * @param course Course to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCourse(Course course) {
        if (courseMap.containsKey(course.getCourseId())) {
            return false;
        }
        
        courses.add(course);
        courseMap.put(course.getCourseId(), course);
        return true;
    }
    
    /**
     * Get a course by ID
     * @param courseId Course ID
     * @return Course if found, null otherwise
     */
    public Course getCourse(String courseId) {
        return courseMap.get(courseId);
    }
    
    /**
     * Enroll a student in a course
     * @param studentId Student ID
     * @param courseId Course ID
     * @return True if enrolled successfully, false otherwise
     */
    public boolean enrollStudentInCourse(String studentId, String courseId) {
        Student student = getStudent(studentId);
        Course course = getCourse(courseId);
        
        if (student == null || course == null) {
            return false;
        }
        
        if (course.enrollStudent(student)) {
            return student.enrollInCourse(courseId);
        }
        
        return false;
    }
    
    /**
     * Drop a student from a course
     * @param studentId Student ID
     * @param courseId Course ID
     * @return True if dropped successfully, false otherwise
     */
    public boolean dropStudentFromCourse(String studentId, String courseId) {
        Student student = getStudent(studentId);
        Course course = getCourse(courseId);
        
        if (student == null || course == null) {
            return false;
        }
        
        if (course.dropStudent(studentId)) {
            return student.dropFromCourse(courseId);
        }
        
        return false;
    }
    
    /**
     * Add a grade
     * @param grade Grade to add
     * @return True if added successfully, false otherwise
     */
    public boolean addGrade(Grade grade) {
        Student student = getStudent(grade.getStudentId());
        Course course = getCourse(grade.getCourseId());
        
        if (student == null || course == null) {
            return false;
        }
        
        // Add grade to student
        student.addGrade(grade);
        
        return true;
    }
    
    /**
     * Add attendance record
     * @param attendance Attendance record
     * @return True if added successfully, false otherwise
     */
    public boolean addAttendance(Attendance attendance) {
        Student student = getStudent(attendance.getStudentId());
        Course course = getCourse(attendance.getCourseId());
        
        if (student == null || course == null) {
            return false;
        }
        
        // Add attendance to student
        student.addAttendance(attendance);
        
        return true;
    }
    
    /**
     * Get students in a course
     * @param courseId Course ID
     * @return List of students in the course
     */
    public List<Student> getStudentsInCourse(String courseId) {
        Course course = getCourse(courseId);
        if (course == null) {
            return new ArrayList<>();
        }
        
        return course.getEnrolledStudents();
    }
    
    /**
     * Get courses for a student
     * @param studentId Student ID
     * @return List of course IDs for the student
     */
    public List<String> getCoursesForStudent(String studentId) {
        Student student = getStudent(studentId);
        if (student == null) {
            return new ArrayList<>();
        }
        
        return student.getEnrolledCourseIds();
    }
    
    /**
     * Calculate GPA for a student
     * @param studentId Student ID
     * @return GPA of the student
     */
    public double calculateGpaForStudent(String studentId) {
        Student student = getStudent(studentId);
        if (student == null) {
            return 0.0;
        }
        
        return student.getGpa();
    }
    
    /**
     * Calculate average GPA for all students
     * @return Average GPA
     */
    public double calculateAverageGpa() {
        double totalGpa = 0.0;
        int studentCount = 0;
        
        for (Student student : students) {
            totalGpa += student.getGpa();
            studentCount++;
        }
        
        if (studentCount == 0) {
            return 0.0;
        }
        
        return totalGpa / studentCount;
    }
    
    /**
     * Calculate average GPA by major
     * @return Map of majors to average GPA
     */
    public Map<String, Double> calculateAverageGpaByMajor() {
        Map<String, List<Student>> studentsByMajor = new HashMap<>();
        Map<String, Double> averageGpaByMajor = new HashMap<>();
        
        // Group students by major
        for (Student student : students) {
            String major = student.getMajor();
            
            if (!studentsByMajor.containsKey(major)) {
                studentsByMajor.put(major, new ArrayList<>());
            }
            
            studentsByMajor.get(major).add(student);
        }
        
        // Calculate average GPA for each major
        for (Map.Entry<String, List<Student>> entry : studentsByMajor.entrySet()) {
            String major = entry.getKey();
            List<Student> studentsInMajor = entry.getValue();
            
            double totalGpa = 0.0;
            for (Student student : studentsInMajor) {
                totalGpa += student.getGpa();
            }
            
            double averageGpa = totalGpa / studentsInMajor.size();
            averageGpaByMajor.put(major, averageGpa);
        }
        
        return averageGpaByMajor;
    }
    
    /**
     * Calculate attendance percentage for a student in a course
     * @param studentId Student ID
     * @param courseId Course ID
     * @return Attendance percentage
     */
    public double calculateAttendanceForStudentInCourse(String studentId, String courseId) {
        Student student = getStudent(studentId);
        if (student == null) {
            return 0.0;
        }
        
        return student.getAttendancePercentageForCourse(courseId);
    }
    
    /**
     * Generate academic report
     * @return Academic report string
     */
    public String generateAcademicReport() {
        StringBuilder sb = new StringBuilder();
        sb.append("=========================================\n");
        sb.append("         ACADEMIC REPORT\n");
        sb.append("=========================================\n");
        sb.append("Institution: ").append(institutionName).append("\n");
        sb.append("Report Date: ").append(new Date()).append("\n\n");
        
        // Summary statistics
        sb.append("SUMMARY STATISTICS\n");
        sb.append("------------------\n");
        sb.append("Total Students: ").append(students.size()).append("\n");
        sb.append("Total Courses: ").append(courses.size()).append("\n");
        sb.append("Average GPA: ").append(String.format("%.2f", calculateAverageGpa())).append("\n\n");
        
        // GPA by major
        sb.append("AVERAGE GPA BY MAJOR\n");
        sb.append("---------------------\n");
        Map<String, Double> averageGpaByMajor = calculateAverageGpaByMajor();
        for (Map.Entry<String, Double> entry : averageGpaByMajor.entrySet()) {
            sb.append(entry.getKey()).append(": ").append(String.format("%.2f", entry.getValue())).append("\n");
        }
        
        sb.append("\n");
        
        // Top performing students
        sb.append("TOP PERFORMING STUDENTS\n");
        sb.append("-----------------------\n");
        List<Student> sortedStudents = new ArrayList<>(students);
        Collections.sort(sortedStudents, (a, b) -> Double.compare(b.getGpa(), a.getGpa()));
        
        int count = Math.min(5, sortedStudents.size());  // Top 5 or fewer
        for (int i = 0; i < count; i++) {
            Student student = sortedStudents.get(i);
            sb.append(i + 1).append(". ").append(student.getFullName())
              .append(" (ID: ").append(student.getStudentId()).append("): ")
              .append(String.format("%.2f", student.getGpa())).append("\n");
        }
        
        return sb.toString();
    }
    
    /**
     * Save student data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("STUDENT INFORMATION SYSTEM DATA EXPORT");
            writer.println("=====================================");
            writer.println("Institution: " + institutionName);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export students
            writer.println("STUDENTS");
            writer.println("--------");
            for (Student student : students) {
                writer.println("ID: " + student.getStudentId());
                writer.println("Name: " + student.getFullName());
                writer.println("Birth Date: " + student.getBirthDate());
                writer.println("Gender: " + student.getGender());
                writer.println("Major: " + student.getMajor());
                writer.println("Class Level: " + student.getClassLevel());
                writer.println("Admission Date: " + student.getAdmissionDate());
                writer.println("GPA: " + String.format("%.2f", student.getGpa()));
                writer.println("Enrolled Courses: " + student.getEnrolledCourseIds().size());
                writer.println("Address: " + (student.getAddress() != null ? student.getAddress().getFullAddress() : "N/A"));
                writer.println("Phone: " + student.getPhone());
                writer.println("Email: " + student.getEmail());
                writer.println();
            }
            
            // Export courses
            writer.println("COURSES");
            writer.println("-------");
            for (Course course : courses) {
                writer.println("ID: " + course.getCourseId());
                writer.println("Name: " + course.getCourseName());
                writer.println("Code: " + course.getCourseCode());
                writer.println("Credits: " + course.getCredits());
                writer.println("Department: " + course.getDepartment());
                writer.println("Instructor: " + course.getInstructor());
                writer.println("Semester: " + course.getSemester() + " " + course.getYear());
                writer.println("Schedule: " + course.getSchedule());
                writer.println("Enrolled Students: " + course.getEnrolledStudents().size());
                writer.println();
            }
            
            writer.close();
            return true;
        } catch (IOException e) {
            System.err.println("Error saving to file: " + e.getMessage());
            return false;
        }
    }
    
    /**
     * Main method for demonstration
     */
    public static void main(String[] args) {
        // Create a new student information system
        StudentInformationSystem system = new StudentInformationSystem("State University");
        
        // Create some courses
        Course course1 = new Course("CS101", "Introduction to Computer Science", "CS101", 3,
                "Computer Science", "Dr. Smith", "Fall", 2023,
                "Mon/Wed/Fri 9:00-10:00 AM");
        
        Course course2 = new Course("CS102", "Data Structures", "CS102", 3,
                "Computer Science", "Dr. Johnson", "Fall", 2023,
                "Mon/Wed/Fri 10:00-11:00 AM");
        
        Course course3 = new Course("CS103", "Algorithms", "CS103", 3,
                "Computer Science", "Dr. Williams", "Fall", 2023,
                "Mon/Wed/Fri 1:00-2:00 PM");
        
        Course course4 = new Course("MATH101", "Calculus I", "MATH101", 4,
                "Mathematics", "Prof. Davis", "Fall", 2023,
                "Tue/Thu 9:00-10:30 AM");
        
        // Add courses to the system
        system.addCourse(course1);
        system.addCourse(course2);
        system.addCourse(course3);
        system.addCourse(course4);
        
        // Create some students
        Address address1 = new Address("123 Main St", "College Town", "CA", "12345", "USA");
        Address address2 = new Address("456 Oak Ave", "University City", "NY", "67890", "USA");
        Address address3 = new Address("789 Pine Rd", "Campus Town", "TX", "98765", "USA");
        Address address4 = new Address("321 Elm Blvd", "Academic City", "MA", "54321", "USA");
        Address address5 = new Address("654 Maple Dr", "Scholar Town", "IL", "13579", "USA");
        
        Date birthDate1 = new Date(2002, 5, 15);
        Date birthDate2 = new Date(2001, 8, 22);
        Date birthDate3 = new Date(2003, 2, 10);
        Date birthDate4 = new Date(2002, 11, 5);
        Date birthDate5 = new Date(2001, 4, 30);
        
        Date admissionDate1 = new Date(2020, 8, 20);
        Date admissionDate2 = new Date(2019, 9, 15);
        Date admissionDate3 = new Date(2021, 1, 10);
        Date admissionDate4 = new Date(2020, 6, 5);
        Date admissionDate5 = new Date(2019, 3, 25);
        
        Student student1 = new Student("S001", "John", "Doe", birthDate1, "Male",
                address1, "(555) 123-4567", "john.doe@university.edu", 
                "Computer Science", "Sophomore", admissionDate1);
        
        Student student2 = new Student("S002", "Jane", "Smith", birthDate2, "Female",
                address2, "(555) 987-6543", "jane.smith@university.edu", 
                "Mathematics", "Junior", admissionDate2);
        
        Student student3 = new Student("S003", "Michael", "Johnson", birthDate3, "Male",
                address3, "(555) 246-8135", "michael.johnson@university.edu", 
                "Computer Science", "Freshman", admissionDate3);
        
        Student student4 = new Student("S004", "Emily", "Davis", birthDate4, "Female",
                address4, "(555) 135-7924", "emily.davis@university.edu", 
                "Computer Science", "Senior", admissionDate4);
        
        Student student5 = new Student("S005", "David", "Wilson", birthDate5, "Male",
                address5, "(555) 975-3142", "david.wilson@university.edu", 
                "Mathematics", "Junior", admissionDate5);
        
        // Add students to the system
        system.addStudent(student1);
        system.addStudent(student2);
        system.addStudent(student3);
        system.addStudent(student4);
        system.addStudent(student5);
        
        // Enroll students in courses
        system.enrollStudentInCourse("S001", "CS101");
        system.enrollStudentInCourse("S001", "CS102");
        system.enrollStudentInCourse("S001", "MATH101");
        
        system.enrollStudentInCourse("S002", "CS101");
        system.enrollStudentInCourse("S002", "CS103");
        system.enrollStudentInCourse("S002", "MATH101");
        
        system.enrollStudentInCourse("S003", "CS101");
        system.enrollStudentInCourse("S003", "CS102");
        system.enrollStudentInCourse("S003", "CS103");
        
        system.enrollStudentInCourse("S004", "CS102");
        system.enrollStudentInCourse("S004", "CS103");
        system.enrollStudentInCourse("S004", "MATH101");
        
        system.enrollStudentInCourse("S005", "CS101");
        system.enrollStudentInCourse("S005", "MATH101");
        
        // Add some grades
        Date today = new Date();
        Grade grade1 = new Grade("CS101", "S001", "Midterm", 85.0, 100.0, today);
        Grade grade2 = new Grade("CS102", "S001", "Final Exam", 92.0, 100.0, today);
        Grade grade3 = new Grade("MATH101", "S001", "Homework 1", 90.0, 100.0, today);
        
        system.addGrade(grade1);
        system.addGrade(grade2);
        system.addGrade(grade3);
        
        Grade grade4 = new Grade("CS101", "S002", "Midterm", 88.0, 100.0, today);
        Grade grade5 = new Grade("CS103", "S002", "Final Exam", 95.0, 100.0, today);
        Grade grade6 = new Grade("MATH101", "S002", "Project", 93.0, 100.0, today);
        
        system.addGrade(grade4);
        system.addGrade(grade5);
        system.addGrade(grade6);
        
        Grade grade7 = new Grade("CS101", "S003", "Midterm", 78.0, 100.0, today);
        Grade grade8 = new Grade("CS102", "S003", "Final Exam", 82.0, 100.0, today);
        Grade grade9 = new Grade("CS103", "S003", "Midterm", 75.0, 100.0, today);
        
        system.addGrade(grade7);
        system.addGrade(grade8);
        system.addGrade(grade9);
        
        // Add some attendance records
        Attendance attendance1 = new Attendance("CS101", "S001", new Date(2023, 9, 5), true, "Present");
        Attendance attendance2 = new Attendance("CS102", "S001", new Date(2023, 9, 7), true, "Present");
        Attendance attendance3 = new Attendance("CS101", "S002", new Date(2023, 9, 5), true, "Present");
        Attendance attendance4 = new Attendance("CS103", "S002", new Date(2023, 9, 7), false, "Absent");
        Attendance attendance5 = new Attendance("MATH101", "S001", new Date(2023, 9, 6), true, "Late");
        
        system.addAttendance(attendance1);
        system.addAttendance(attendance2);
        system.addAttendance(attendance3);
        system.addAttendance(attendance4);
        system.addAttendance(attendance5);
        
        // Print system information
        System.out.println("Student Information Management System");
        System.out.println("===============================");
        System.out.println("Institution: " + system.getInstitutionName());
        System.out.println("Total Students: " + system.getStudents().size());
        System.out.println("Total Courses: " + system.getCourses().size());
        System.out.println("Average GPA: " + String.format("%.2f", system.calculateAverageGpa()));
        System.out.println();
        
        // Print student summaries
        System.out.println("Student Summaries");
        System.out.println("-----------------");
        for (Student student : system.getStudents()) {
            System.out.println(student.getSummary());
            System.out.println();
        }
        
        // Print course summaries
        System.out.println("Course Summaries");
        System.out.println("-----------------");
        for (Course course : system.getCourses()) {
            System.out.println(course.getSummary());
            System.out.println();
        }
        
        // Print GPA by major
        System.out.println("Average GPA by Major");
        System.out.println("-------------------");
        Map<String, Double> averageGpaByMajor = system.calculateAverageGpaByMajor();
        for (Map.Entry<String, Double> entry : averageGpaByMajor.entrySet()) {
            System.out.println(entry.getKey() + ": " + String.format("%.2f", entry.getValue()));
        }
        
        // Print academic report
        System.out.println("\n" + system.generateAcademicReport());
        
        // Save data to file
        system.saveToFile("student_system_data_export.txt");
        
        System.out.println("\n\nStudent Information Management System demonstration completed successfully!");
    }
}
<｜fim▁hole｜>