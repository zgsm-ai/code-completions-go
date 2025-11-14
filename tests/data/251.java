import java.util.*;
import java.time.*;

/**
 * 学生管理系统类
 * 用于管理学生信息、课程、成绩和出勤记录
 */
public class StudentManagementSystem {
    private Map<String, Student> students;
    private Map<String, Course> courses;
    private Map<String, Teacher> teachers;
    private Map<String, Enrollment> enrollments;
    private Map<String, Attendance> attendanceRecords;
    private List<Grade> grades;
    
    /**
     * 学生管理系统构造函数
     */
    public StudentManagementSystem() {
        this.students = new HashMap<>();
        this.courses = new HashMap<>();
        this.teachers = new HashMap<>();
        this.enrollments = new HashMap<>();
        this.attendanceRecords = new HashMap<>();
        this.grades = new ArrayList<>();
    }
    
    /**
     * 添加新学生
     * @param studentId 学生唯一标识符
     * @param name 学生姓名
     * @param dateOfBirth 出生日期
     * @param address 地址
     * @param phoneNumber 电话号码
     * @param email 电子邮件
     * @return 添加成功返回true，否则返回false
     */
    public boolean addStudent(String studentId, String name, LocalDate dateOfBirth, 
                             String address, String phoneNumber, String email) {
        if (students.containsKey(studentId)) {
            return false;
        }
        
        Student student = new Student(studentId, name, dateOfBirth, address, phoneNumber, email);
        students.put(studentId, student);
        return true;
    }
    
    /**
     * 添加新教师
     * @param teacherId 教师唯一标识符
     * @param name 教师姓名
     * @param department 所属部门
     * @param email 电子邮件
     * @return 添加成功返回true，否则返回false
     */
    public boolean addTeacher(String teacherId, String name, String department, String email) {
        if (teachers.containsKey(teacherId)) {
            return false;
        }
        
        Teacher teacher = new Teacher(teacherId, name, department, email);
        teachers.put(teacherId, teacher);
        return true;
    }
    
    /**
     * 添加新课程
     * @param courseId 课程唯一标识符
     * @param name 课程名称
     * @param description 课程描述
     * @param credits 学分
     * @param teacherId 教师ID
     * @return 添加成功返回true，否则返回false
     */
    public boolean addCourse(String courseId, String name, String description, 
                            int credits, String teacherId) {
        if (courses.containsKey(courseId)) {
            return false;
        }
        
        if (!teachers.containsKey(teacherId)) {
            return false;
        }
        
        Course course = new Course(courseId, name, description, credits, teacherId);
        courses.put(courseId, course);
        
        // 更新教师的课程列表
        teachers.get(teacherId).addCourse(courseId);
        
        return true;
    }
    
    /**
     * 学生选课
     * @param studentId 学生ID
     * @param courseId 课程ID
     * @param enrollmentDate 选课日期
     * @return 选课成功返回选课记录ID，否则返回null
     */
    public String enrollStudent(String studentId, String courseId, LocalDate enrollmentDate) {
        if (!students.containsKey(studentId) || !courses.containsKey(courseId)) {
            return null;
        }
        
        // 检查学生是否已经选过这门课
        for (Enrollment enrollment : enrollments.values()) {
            if (enrollment.getStudentId().equals(studentId) && 
                enrollment.getCourseId().equals(courseId)) {
                return null;
            }
        }
        
        String enrollmentId = generateId("enrollment");
        Enrollment enrollment = new Enrollment(enrollmentId, studentId, courseId, enrollmentDate);
        enrollments.put(enrollmentId, enrollment);
        
        // 更新学生的选课列表
        students.get(studentId).addEnrollment(enrollmentId);
        
        return enrollmentId;
    }
    
    /**
     * 记录学生出勤
     * @param studentId 学生ID
     * @param courseId 课程ID
     * @param date 出勤日期
     * @param status 出勤状态（present, absent, late）
     * @return 记录成功返回出勤记录ID，否则返回null
     */
    public String recordAttendance(String studentId, String courseId, LocalDate date, String status) {
        if (!students.containsKey(studentId) || !courses.containsKey(courseId)) {
            return null;
        }
        
        // 检查学生是否选了这门课
        boolean isEnrolled = false;
        for (Enrollment enrollment : enrollments.values()) {
            if (enrollment.getStudentId().equals(studentId) && 
                enrollment.getCourseId().equals(courseId)) {
                isEnrolled = true;
                break;
            }
        }
        
        if (!isEnrolled) {
            return null;
        }
        
        // 检查当天是否已有出勤记录
        String attendanceKey = studentId + "_" + courseId + "_" + date.toString();
        if (attendanceRecords.containsKey(attendanceKey)) {
            return null;
        }
        
        String attendanceId = generateId("attendance");
        Attendance attendance = new Attendance(attendanceId, studentId, courseId, date, status);
        attendanceRecords.put(attendanceKey, attendance);
        
        return attendanceId;
    }
    
    /**
     * 记录学生成绩
     * @param studentId 学生ID
     * @param courseId 课程ID
     * @param assessmentType 评估类型（exam, quiz, assignment, project）
     * @param score 分数
     * @param maxScore 最高分
     * @param date 记录日期
     * @return 记录成功返回成绩ID，否则返回null
     */
    public String recordGrade(String studentId, String courseId, String assessmentType, 
                             double score, double maxScore, LocalDate date) {
        if (!students.containsKey(studentId) || !courses.containsKey(courseId)) {
            return null;
        }
        
        // 检查学生是否选了这门课
        boolean isEnrolled = false;
        for (Enrollment enrollment : enrollments.values()) {
            if (enrollment.getStudentId().equals(studentId) && 
                enrollment.getCourseId().equals(courseId)) {
                isEnrolled = true;
                break;
            }
        }
        
        if (!isEnrolled) {
            return null;
        }
        
        if (score < 0 || score > maxScore) {
            return null;
        }
        
        String gradeId = generateId("grade");
        Grade grade = new Grade(gradeId, studentId, courseId, assessmentType, score, maxScore, date);
        grades.add(grade);
        
        return gradeId;
    }
    
    /**
     * 获取学生信息
     * @param studentId 学生ID
     * @return 学生信息，不存在则返回null
     */
    public Student getStudent(String studentId) {
        return students.get(studentId);
    }
    
    /**
     * 获取课程信息
     * @param courseId 课程ID
     * @return 课程信息，不存在则返回null
     */
    public Course getCourse(String courseId) {
        return courses.get(courseId);
    }
    
    /**
     * 获取教师信息
     * @param teacherId 教师ID
     * @return 教师信息，不存在则返回null
     */
    public Teacher getTeacher(String teacherId) {
        return teachers.get(teacherId);
    }
    
    /**
     * 获取学生的所有选课记录
     * @param studentId 学生ID
     * @return 选课记录列表
     */
    public List<Enrollment> getStudentEnrollments(String studentId) {
        List<Enrollment> result = new ArrayList<>();
        
        if (!students.containsKey(studentId)) {
            return result;
        }
        
        for (Enrollment enrollment : enrollments.values()) {
            if (enrollment.getStudentId().equals(studentId)) {
                result.add(enrollment);
            }
        }
        
        return result;
    }
    
    /**
     * 获取课程的所有选课学生
     * @param courseId 课程ID
     * @return 学生列表
     */
    public List<Student> getCourseStudents(String courseId) {
        List<Student> result = new ArrayList<>();
        
        if (!courses.containsKey(courseId)) {
            return result;
        }
        
        for (Enrollment enrollment : enrollments.values()) {
            if (enrollment.getCourseId().equals(courseId)) {
                Student student = students.get(enrollment.getStudentId());
                if (student != null) {
                    result.add(student);
                }
            }
        }
        
        return result;
    }
    
    /**
     * 获取学生的出勤记录
     * @param studentId 学生ID
     * @param courseId 课程ID（可选）
     * @return 出勤记录列表
     */
    public List<Attendance> getStudentAttendance(String studentId, String courseId) {
        List<Attendance> result = new ArrayList<>();
        
        if (!students.containsKey(studentId)) {
            return result;
        }
        
        for (Attendance attendance : attendanceRecords.values()) {
            if (attendance.getStudentId().equals(studentId)) {
                if (courseId == null || attendance.getCourseId().equals(courseId)) {
                    result.add(attendance);
                }
            }
        }
        
        return result;
    }
    
    /**
     * 获取学生的成绩记录
     * @param studentId 学生ID
     * @param courseId 课程ID（可选）
     * @return 成绩记录列表
     */
    public List<Grade> getStudentGrades(String studentId, String courseId) {
        List<Grade> result = new ArrayList<>();
        
        if (!students.containsKey(studentId)) {
            return result;
        }
        
        for (Grade grade : grades) {
            if (grade.getStudentId().equals(studentId)) {
                if (courseId == null || grade.getCourseId().equals(courseId)) {
                    result.add(grade);
                }
            }
        }
        
        return result;
    }
    
    /**
     * 计算学生的平均成绩
     * @param studentId 学生ID
     * @param courseId 课程ID（可选）
     * @return 平均成绩，如果没有成绩记录返回0
     */
    public double calculateAverageGrade(String studentId, String courseId) {
        List<Grade> studentGrades = getStudentGrades(studentId, courseId);
        
        if (studentGrades.isEmpty()) {
            return 0;
        }
        
        double total = 0;
        for (Grade grade : studentGrades) {
            total += (grade.getScore() / grade.getMaxScore()) * 100;
        }
        
        return total / studentGrades.size();
    }
    
    /**
     * 计算学生的出勤率
     * @param studentId 学生ID
     * @param courseId 课程ID（可选）
     * @return 出勤率百分比，没有出勤记录返回0
     */
    public double calculateAttendanceRate(String studentId, String courseId) {
        List<Attendance> attendanceList = getStudentAttendance(studentId, courseId);
        
        if (attendanceList.isEmpty()) {
            return 0;
        }
        
        int presentCount = 0;
        for (Attendance attendance : attendanceList) {
            if ("present".equals(attendance.getStatus()) || "late".equals(attendance.getStatus())) {
                presentCount++;
            }
        }
        
        return (double) presentCount / attendanceList.size() * 100;
    }
    
    /**
     * 生成学生成绩单
     * @param studentId 学生ID
     * @return 成绩单字符串
     */
    public String generateStudentReport(String studentId) {
        Student student = students.get(studentId);
        if (student == null) {
            return "Student not found";
        }
        
        StringBuilder report = new StringBuilder();
        report.append("Student Report\n");
        report.append("=============\n");
        report.append("ID: ").append(student.getStudentId()).append("\n");
        report.append("Name: ").append(student.getName()).append("\n");
        report.append("Email: ").append(student.getEmail()).append("\n\n");
        
        // 获取所有选课
        List<Enrollment> enrollments = getStudentEnrollments(studentId);
        if (enrollments.isEmpty()) {
            report.append("No course enrollments found.\n");
            return report.toString();
        }
        
        report.append("Course Grades\n");
        report.append("-------------\n");
        
        for (Enrollment enrollment : enrollments) {
            Course course = courses.get(enrollment.getCourseId());
            if (course == null) {
                continue;
            }
            
            report.append("Course: ").append(course.getName()).append("\n");
            report.append("Teacher: ").append(teachers.get(course.getTeacherId()).getName()).append("\n");
            
            // 获取该课程的成绩
            List<Grade> courseGrades = getStudentGrades(studentId, course.getCourseId());
            if (courseGrades.isEmpty()) {
                report.append("No grades recorded for this course.\n\n");
                continue;
            }
            
            double courseTotal = 0;
            for (Grade grade : courseGrades) {
                double percentage = (grade.getScore() / grade.getMaxScore()) * 100;
                report.append("  ").append(grade.getAssessmentType()).append(": ")
                      .append(String.format("%.1f", percentage)).append("%\n");
                courseTotal += percentage;
            }
            
            double courseAverage = courseTotal / courseGrades.size();
            report.append("  Course Average: ").append(String.format("%.1f", courseAverage)).append("%\n");
            
            // 获取出勤率
            double attendanceRate = calculateAttendanceRate(studentId, course.getCourseId());
            report.append("  Attendance Rate: ").append(String.format("%.1f", attendanceRate)).append("%\n\n");
        }
        
        // 计算总体平均成绩
        double overallAverage = calculateAverageGrade(studentId, null);
        report.append("Overall Average: ").append(String.format("%.1f", overallAverage)).append("%\n");
        
        return report.toString();
    }
    
    /**
     * 生成课程报告
     * @param courseId 课程ID
     * @return 课程报告字符串
     */
    public String generateCourseReport(String courseId) {
        Course course = courses.get(courseId);
        if (course == null) {
            return "Course not found";
        }
        
        Teacher teacher = teachers.get(course.getTeacherId());
        if (teacher == null) {
            return "Teacher not found";
        }
        
        StringBuilder report = new StringBuilder();
        report.append("Course Report\n");
        report.append("=============\n");
        report.append("Course ID: ").append(course.getCourseId()).append("\n");
        report.append("Name: ").append(course.getName()).append("\n");
        report.append("Description: ").append(course.getDescription()).append("\n");
        report.append("Credits: ").append(course.getCredits()).append("\n");
        report.append("Teacher: ").append(teacher.getName()).append("\n\n");
        
        // 获取所有选课学生
        List<Student> students = getCourseStudents(courseId);
        if (students.isEmpty()) {
            report.append("No students enrolled in this course.\n");
            return report.toString();
        }
        
        report.append("Enrolled Students: ").append(students.size()).append("\n");
        report.append("---------------------\n");
        
        for (Student student : students) {
            report.append("ID: ").append(student.getStudentId()).append(", ");
            report.append("Name: ").append(student.getName()).append(", ");
            
            // 计算平均成绩
            double averageGrade = calculateAverageGrade(student.getStudentId(), courseId);
            report.append("Average Grade: ").append(String.format("%.1f", averageGrade)).append("%, ");
            
            // 计算出勤率
            double attendanceRate = calculateAttendanceRate(student.getStudentId(), courseId);
            report.append("Attendance: ").append(String.format("%.1f", attendanceRate)).append("%\n");
        }
        
        return report.toString();
    }
    
    /**
     * 搜索学生
     * @param keyword 搜索关键词
     * @return 匹配的学生列表
     */
    public List<Student> searchStudents(String keyword) {
        List<Student> result = new ArrayList<>();
        keyword = keyword.toLowerCase();
        
        for (Student student : students.values()) {
            if (student.getName().toLowerCase().contains(keyword) || 
                student.getStudentId().toLowerCase().contains(keyword) ||
                student.getEmail().toLowerCase().contains(keyword)) {
                result.add(student);
            }
        }
        
        return result;
    }
    
    /**
     * 生成唯一ID
     * @param prefix ID前缀
     * @return 唯一ID字符串
     */
    private String generateId(String prefix) {
        return prefix + "_" + UUID.randomUUID().toString().substring(0, 8);
    }
    
    /**
     * 获取当前时间戳
     * @return 当前时间戳字符串
     */
    private String getCurrentTimestamp() {
        return LocalDateTime.now().toString();
    }
}

/**
 * 学生类
 */
class Student {
    private String studentId;
    private String name;
    private LocalDate dateOfBirth;
    private String address;
    private String phoneNumber;
    private String email;
    private List<String> enrollmentIds;
    
    public Student(String studentId, String name, LocalDate dateOfBirth, String address, String phoneNumber, String email) {
        this.studentId = studentId;
        this.name = name;
        this.dateOfBirth = dateOfBirth;
        this.address = address;
        this.phoneNumber = phoneNumber;
        this.email = email;
        this.enrollmentIds = new ArrayList<>();
    }
    
    // Getters and setters
    public String getStudentId() {
        return studentId;
    }
    
    public String getName() {
        return name;
    }
    
    public LocalDate getDateOfBirth() {
        return dateOfBirth;
    }
    
    public String getAddress() {
        return address;
    }
    
    public String getPhoneNumber() {
        return phoneNumber;
    }
    
    public String getEmail() {
        return email;
    }
    
    public List<String> getEnrollmentIds() {
        return new ArrayList<>(enrollmentIds);
    }
    
    public void addEnrollment(String enrollmentId) {
        enrollmentIds.add(enrollmentId);
    }
}

/**
 * 教师类
 */
class Teacher {
    private String teacherId;
    private String name;
    private String department;
    private String email;
    private List<String> courseIds;
    
    public Teacher(String teacherId, String name, String department, String email) {
        this.teacherId = teacherId;
        this.name = name;
        this.department = department;
        this.email = email;
        this.courseIds = new ArrayList<>();
    }
    
    // Getters and setters
    public String getTeacherId() {
        return teacherId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getDepartment() {
        return department;
    }
    
    public String getEmail() {
        return email;
    }
    
    public List<String> getCourseIds() {
        return new ArrayList<>(courseIds);
    }
    
    public void addCourse(String courseId) {
        courseIds.add(courseId);
    }
}

/**
 * 课程类
 */
class Course {
    private String courseId;
    private String name;
    private String description;
    private int credits;
    private String teacherId;
    
    public Course(String courseId, String name, String description, int credits, String teacherId) {
        this.courseId = courseId;
        this.name = name;
        this.description = description;
        this.credits = credits;
        this.teacherId = teacherId;
    }
    
    // Getters and setters
    public String getCourseId() {
        return courseId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getDescription() {
        return description;
    }
    
    public int getCredits() {
        return credits;
    }
    
    public String getTeacherId() {
        return teacherId;
    }
}

/**
 * 选课记录类
 */
class Enrollment {
    private String enrollmentId;
    private String studentId;
    private String courseId;
    private LocalDate enrollmentDate;
    
    public Enrollment(String enrollmentId, String studentId, String courseId, LocalDate enrollmentDate) {
        this.enrollmentId = enrollmentId;
        this.studentId = studentId;
        this.courseId = courseId;
        this.enrollmentDate = enrollmentDate;
    }
    
    // Getters and setters
    public String getEnrollmentId() {
        return enrollmentId;
    }
    
    public String getStudentId() {
        return studentId;
    }
    
    public String getCourseId() {
        return courseId;
    }
    
    public LocalDate getEnrollmentDate() {
        return enrollmentDate;
    }
}

/**
 * 出勤记录类
 */
class Attendance {
    private String attendanceId;
    private String studentId;
    private String courseId;
    private LocalDate date;
    private String status;
    
    public Attendance(String attendanceId, String studentId, String courseId, LocalDate date, String status) {
        this.attendanceId = attendanceId;
        this.studentId = studentId;
        this.courseId = courseId;
        this.date = date;
        this.status = status;
    }
    
    // Getters and setters
    public String getAttendanceId() {
        return attendanceId;
    }
    
    public String getStudentId() {
        return studentId;
    }
    
    public String getCourseId() {
        return courseId;
    }
    
    public LocalDate getDate() {
        return date;
    }
    
    public String getStatus() {
        return status;
    }
}

/**
 * 成绩类
 */
class Grade {
    private String gradeId;
    private String studentId;
    private String courseId;
    private String assessmentType;
    private double score;
    private double maxScore;
    private LocalDate date;
    
    public Grade(String gradeId, String studentId, String courseId, String assessmentType, 
                double score, double maxScore, LocalDate date) {
        this.gradeId = gradeId;
        this.studentId = studentId;
        this.courseId = courseId;
        this.assessmentType = assessmentType;
        this.score = score;
        this.maxScore = maxScore;
        this.date = date;
    }
    
    // Getters and setters
    public String getGradeId() {
        return gradeId;
    }
    
    public String getStudentId() {
        return studentId;
    }
    
    public String getCourseId() {
        return courseId;
    }
    
    public String getAssessmentType() {
        return assessmentType;
    }
    
    public double getScore() {
        return score;
    }
    
    public double getMaxScore() {
        return maxScore;
    }
    
    public LocalDate getDate() {
        return date;
    }
}