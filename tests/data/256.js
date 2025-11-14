/**
 * 学生管理系统类
 * 用于管理学生信息、课程、成绩和出勤记录
 */
class StudentManagementSystem {
    /**
     * 构造函数
     */
    constructor() {
        this.students = new Map();  // 学生ID到学生信息的映射
        this.courses = new Map();    // 课程ID到课程信息的映射
        this.teachers = new Map();   // 教师ID到教师信息的映射
        this.enrollments = new Map(); // 选课记录
        this.attendanceRecords = new Map(); // 出勤记录
        this.grades = [];           // 成绩记录
    }
    
    /**
     * 添加新学生
     * @param {string} studentId - 学生唯一标识符
     * @param {string} name - 学生姓名
     * @param {Date} dateOfBirth - 出生日期
     * @param {string} address - 地址
     * @param {string} phoneNumber - 电话号码
     * @param {string} email - 电子邮件
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addStudent(studentId, name, dateOfBirth, address, phoneNumber, email) {
        if (this.students.has(studentId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const student of this.students.values()) {
            if (student.email === email) {
                return false;
            }
        }
        
        // 检查用户名是否已被使用
        for (const student of this.students.values()) {
            if (student.name === name) {
                return false;
            }
        }
        
        const student = {
            studentId,
            name,
            dateOfBirth,
            address,
            phoneNumber,
            email,
            enrollments: [],
            createdAt: new Date()
        };
        
        this.students.set(studentId, student);
        return true;
    }
    
    /**
     * 添加新教师
     * @param {string} teacherId - 教师唯一标识符
     * @param {string} name - 教师姓名
     * @param {string} department - 所属部门
     * @param {string} email - 电子邮件
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addTeacher(teacherId, name, department, email) {
        if (this.teachers.has(teacherId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const teacher of this.teachers.values()) {
            if (teacher.email === email) {
                return false;
            }
        }
        
        const teacher = {
            teacherId,
            name,
            department,
            email,
            courses: [],
            createdAt: new Date()
        };
        
        this.teachers.set(teacherId, teacher);
        return true;
    }
    
    /**
     * 添加新课程
     * @param {string} courseId - 课程唯一标识符
     * @param {string} name - 课程名称
     * @param {string} description - 课程描述
     * @param {number} credits - 学分
     * @param {string} teacherId - 教师ID
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addCourse(courseId, name, description, credits, teacherId) {
        if (this.courses.has(courseId)) {
            return false;
        }
        
        if (!this.teachers.has(teacherId)) {
            return false;
        }
        
        const course = {
            courseId,
            name,
            description,
            credits,
            teacherId,
            createdAt: new Date()
        };
        
        this.courses.set(courseId, course);
        
        // 更新教师的课程列表
        this.teachers.get(teacherId).courses.push(courseId);
        
        return true;
    }
    
    /**
     * 学生选课
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID
     * @param {Date} enrollmentDate - 选课日期
     * @returns {string|null} 选课成功返回选课记录ID，否则返回null
     */
    enrollStudent(studentId, courseId, enrollmentDate) {
        if (!this.students.has(studentId) || !this.courses.has(courseId)) {
            return null;
        }
        
        // 检查学生是否已经选过这门课
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId && enrollment.courseId === courseId) {
                return null;
            }
        }
        
        const enrollmentId = this.generateId('enrollment');
        const enrollment = {
            enrollmentId,
            studentId,
            courseId,
            enrollmentDate,
            createdAt: new Date()
        };
        
        this.enrollments.set(enrollmentId, enrollment);
        
        // 更新学生的选课列表
        this.students.get(studentId).enrollments.push(enrollmentId);
        
        return enrollmentId;
    }
    
    /**
     * 记录学生出勤
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID
     * @param {Date} date - 出勤日期
     * @param {string} status - 出勤状态（present, absent, late）
     * @returns {string|null} 记录成功返回出勤记录ID，否则返回null
     */
    recordAttendance(studentId, courseId, date, status) {
        if (!this.students.has(studentId) || !this.courses.has(courseId)) {
            return null;
        }
        
        // 检查学生是否选了这门课
        let isEnrolled = false;
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId && enrollment.courseId === courseId) {
                isEnrolled = true;
                break;
            }
        }
        
        if (!isEnrolled) {
            return null;
        }
        
        // 检查当天是否已有出勤记录
        const attendanceKey = `${studentId}_${courseId}_${date.toISOString().split('T')[0]}`;
        if (this.attendanceRecords.has(attendanceKey)) {
            return null;
        }
        
        const attendanceId = this.generateId('attendance');
        const attendance = {
            attendanceId,
            studentId,
            courseId,
            date,
            status,
            createdAt: new Date()
        };
        
        this.attendanceRecords.set(attendanceKey, attendance);
        
        return attendanceId;
    }
    
    /**
     * 记录学生成绩
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID
     * @param {string} assessmentType - 评估类型（exam, quiz, assignment, project）
     * @param {number} score - 分数
     * @param {number} maxScore - 最高分
     * @param {Date} date - 记录日期
     * @returns {string|null} 记录成功返回成绩ID，否则返回null
     */
    recordGrade(studentId, courseId, assessmentType, score, maxScore, date) {
        if (!this.students.has(studentId) || !this.courses.has(courseId)) {
            return null;
        }
        
        // 检查学生是否选了这门课
        let isEnrolled = false;
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId && enrollment.courseId === courseId) {
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
        
        const gradeId = this.generateId('grade');
        const grade = {
            gradeId,
            studentId,
            courseId,
            assessmentType,
            score,
            maxScore,
            date,
            createdAt: new Date()
        };
        
        this.grades.push(grade);
        
        return gradeId;
    }
    
    /**
     * 获取学生信息
     * @param {string} studentId - 学生ID
     * @returns {Object|null} 学生信息，不存在则返回null
     */
    getStudent(studentId) {
        return this.students.get(studentId) || null;
    }
    
    /**
     * 获取课程信息
     * @param {string} courseId - 课程ID
     * @returns {Object|null} 课程信息，不存在则返回null
     */
    getCourse(courseId) {
        return this.courses.get(courseId) || null;
    }
    
    /**
     * 获取教师信息
     * @param {string} teacherId - 教师ID
     * @returns {Object|null} 教师信息，不存在则返回null
     */
    getTeacher(teacherId) {
        return this.teachers.get(teacherId) || null;
    }
    
    /**
     * 获取学生的所有选课记录
     * @param {string} studentId - 学生ID
     * @returns {Array} 选课记录列表
     */
    getStudentEnrollments(studentId) {
        const result = [];
        
        if (!this.students.has(studentId)) {
            return result;
        }
        
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId) {
                result.push(enrollment);
            }
        }
        
        return result;
    }
    
    /**
     * 获取课程的所有选课学生
     * @param {string} courseId - 课程ID
     * @returns {Array} 学生列表
     */
    getCourseStudents(courseId) {
        const result = [];
        
        if (!this.courses.has(courseId)) {
            return result;
        }
        
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.courseId === courseId) {
                const student = this.students.get(enrollment.studentId);
                if (student) {
                    result.push(student);
                }
            }
        }
        
        return result;
    }
    
    /**
     * 获取学生的出勤记录
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID（可选）
     * @returns {Array} 出勤记录列表
     */
    getStudentAttendance(studentId, courseId) {
        const result = [];
        
        if (!this.students.has(studentId)) {
            return result;
        }
        
        for (const attendance of this.attendanceRecords.values()) {
            if (attendance.studentId === studentId) {
                if (!courseId || attendance.courseId === courseId) {
                    result.push(attendance);
                }
            }
        }
        
        return result;
    }
    
    /**
     * 获取学生的成绩记录
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID（可选）
     * @returns {Array} 成绩记录列表
     */
    getStudentGrades(studentId, courseId) {
        const result = [];
        
        if (!this.students.has(studentId)) {
            return result;
        }
        
        for (const grade of this.grades) {
            if (grade.studentId === studentId) {
                if (!courseId || grade.courseId === courseId) {
                    result.push(grade);
                }
            }
        }
        
        return result;
    }
    
    /**
     * 计算学生的平均成绩
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID（可选）
     * @returns {number} 平均成绩，如果没有成绩记录返回0
     */
    calculateAverageGrade(studentId, courseId) {
        const studentGrades = this.getStudentGrades(studentId, courseId);
        
        if (studentGrades.length === 0) {
            return 0;
        }
        
        let total = 0;
        for (const grade of studentGrades) {
            total += (grade.score / grade.maxScore) * 100;
        }
        
        return total / studentGrades.length;
    }
    
    /**
     * 计算学生的出勤率
     * @param {string} studentId - 学生ID
     * @param {string} courseId - 课程ID（可选）
     * @returns {number} 出勤率百分比，没有出勤记录返回0
     */
    calculateAttendanceRate(studentId, courseId) {
        const attendanceList = this.getStudentAttendance(studentId, courseId);
        
        if (attendanceList.length === 0) {
            return 0;
        }
        
        let presentCount = 0;
        for (const attendance of attendanceList) {
            if (attendance.status === 'present' || attendance.status === 'late') {
                presentCount++;
            }
        }
        
        return (presentCount / attendanceList.length) * 100;
    }
    
    /**
     * 生成学生成绩单
     * @param {string} studentId - 学生ID
     * @returns {string} 成绩单字符串
     */
    generateStudentReport(studentId) {
        const student = this.students.get(studentId);
        if (!student) {
            return 'Student not found';
        }
        
        let report = 'Student Report\n';
        report += '=============\n';
        report += `ID: ${student.studentId}\n`;
        report += `Name: ${student.name}\n`;
        report += `Email: ${student.email}\n\n`;
        
        // 获取所有选课
        const enrollments = this.getStudentEnrollments(studentId);
        if (enrollments.length === 0) {
            report += 'No course enrollments found.\n';
            return report;
        }
        
        report += 'Course Grades\n';
        report += '-------------\n';
        
        for (const enrollment of enrollments) {
            const course = this.courses.get(enrollment.courseId);
            if (!course) {
                continue;
            }
            
            report += `Course: ${course.name}\n`;
            report += `Teacher: ${this.teachers.get(course.teacherId).name}\n`;
            
            // 获取该课程的成绩
            const courseGrades = this.getStudentGrades(studentId, course.courseId);
            if (courseGrades.length === 0) {
                report += 'No grades recorded for this course.\n\n';
                continue;
            }
            
            let courseTotal = 0;
            for (const grade of courseGrades) {
                const percentage = (grade.score / grade.maxScore) * 100;
                report += `  ${grade.assessmentType}: ${percentage.toFixed(1)}%\n`;
                courseTotal += percentage;
            }
            
            const courseAverage = courseTotal / courseGrades.length;
            report += `  Course Average: ${courseAverage.toFixed(1)}%\n`;
            
            // 获取出勤率
            const attendanceRate = this.calculateAttendanceRate(studentId, course.courseId);
            report += `  Attendance Rate: ${attendanceRate.toFixed(1)}%\n\n`;
        }
        
        // 计算总体平均成绩
        const overallAverage = this.calculateAverageGrade(studentId, null);
        report += `Overall Average: ${overallAverage.toFixed(1)}%\n`;
        
        return report;
    }
    
    /**
     * 生成课程报告
     * @param {string} courseId - 课程ID
     * @returns {string} 课程报告字符串
     */
    generateCourseReport(courseId) {
        const course = this.courses.get(courseId);
        if (!course) {
            return 'Course not found';
        }
        
        const teacher = this.teachers.get(course.teacherId);
        if (!teacher) {
            return 'Teacher not found';
        }
        
        let report = 'Course Report\n';
        report += '=============\n';
        report += `Course ID: ${course.courseId}\n`;
        report += `Name: ${course.name}\n`;
        report += `Description: ${course.description}\n`;
        report += `Credits: ${course.credits}\n`;
        report += `Teacher: ${teacher.name}\n\n`;
        
        // 获取所有选课学生
        const students = this.getCourseStudents(courseId);
        if (students.length === 0) {
            report += 'No students enrolled in this course.\n';
            return report;
        }
        
        report += `Enrolled Students: ${students.length}\n`;
        report += '---------------------\n';
        
        for (const student of students) {
            report += `ID: ${student.studentId}, `;
            report += `Name: ${student.name}, `;
            
            // 计算平均成绩
            const averageGrade = this.calculateAverageGrade(student.studentId, courseId);
            report += `Average Grade: ${averageGrade.toFixed(1)}%, `;
            
            // 计算出勤率
            const attendanceRate = this.calculateAttendanceRate(student.studentId, courseId);
            report += `Attendance: ${attendanceRate.toFixed(1)}%\n`;
        }
        
        return report;
    }
    
    /**
     * 搜索学生
     * @param {string} keyword - 搜索关键词
     * @returns {Array} 匹配的学生列表
     */
    searchStudents(keyword) {
        const result = [];
        keyword = keyword.toLowerCase();
        
        for (const student of this.students.values()) {
            if (student.name.toLowerCase().includes(keyword) || 
                student.studentId.toLowerCase().includes(keyword) ||
                student.email.toLowerCase().includes(keyword)) {
                result.push(student);
            }
        }
        
        return result;
    }
    
    /**
     * 生成唯一ID
     * @param {string} prefix - ID前缀
     * @returns {string} 唯一ID字符串
     */
    generateId(prefix) {
        return `${prefix}_${Math.random().toString(36).substring(2, 10)}`;
    }
}