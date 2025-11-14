/**
 * 学生管理系统类
 * 用于管理学生信息、课程、成绩和出勤
 */
class StudentManagementSystem {
    private students: Map<string, Student>;         // 学号到学生信息的映射
    private courses: Map<string, Course>;          // 课程代码到课程信息的映射
    private enrollments: Map<string, Enrollment>;  // 学号到课程注册信息的映射
    private grades: Map<string, Grade[]>;          // 学号到成绩列表的映射
    private attendance: Map<string, Attendance[]>; // 学号到出勤记录的映射
    private teachers: Map<string, Teacher>;       // 教师ID到教师信息的映射
    
    /**
     * 构造函数
     */
    constructor() {
        this.students = new Map();
        this.courses = new Map();
        this.enrollments = new Map();
        this.grades = new Map();
        this.attendance = new Map();
        this.teachers = new Map();
    }
    
    /**
     * 添加新学生
     * @param studentId - 学生的唯一标识符
     * @param firstName - 学生的名字
     * @param lastName - 学生的姓氏
     * @param email - 学生的电子邮件
     * @param phoneNumber - 学生的电话号码
     * @param dateOfBirth - 学生的出生日期
     * @param address - 学生的地址
     * @returns 添加成功返回true，否则返回false
     */
    addStudent(studentId: string, firstName: string, lastName: string, email: string, 
                phoneNumber: string, dateOfBirth: Date, address: string): boolean {
        if (this.students.has(studentId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const student of this.students.values()) {
            if (student.email === email) {
                return false;
            }
        }
        
        const student = new Student(
            studentId, 
            firstName, 
            lastName, 
            email, 
            phoneNumber, 
            dateOfBirth, 
            address
        );
        
        this.students.set(studentId, student);
        this.grades.set(studentId, []);
        this.attendance.set(studentId, []);
        
        return true;
    }
    
    /**
     * 更新学生信息
     * @param studentId - 要更新的学生的ID
     * @param firstName - 新的名字（可选）
     * @param lastName - 新的姓氏（可选）
     * @param email - 新的电子邮件（可选）
     * @param phoneNumber - 新的电话号码（可选）
     * @param address - 新的地址（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateStudent(studentId: string, firstName?: string, lastName?: string, 
                 email?: string, phoneNumber?: string, address?: string): boolean {
        if (!this.students.has(studentId)) {
            return false;
        }
        
        const student = this.students.get(studentId)!;
        
        // 如果提供了电子邮件，检查是否已被其他学生使用
        if (email && email !== student.email) {
            for (const [id, s] of this.students) {
                if (id !== studentId && s.email === email) {
                    return false;
                }
            }
            student.email = email;
        }
        
        if (firstName !== undefined) {
            student.firstName = firstName;
        }
        
        if (lastName !== undefined) {
            student.lastName = lastName;
        }
        
        if (phoneNumber !== undefined) {
            student.phoneNumber = phoneNumber;
        }
        
        if (address !== undefined) {
            student.address = address;
        }
        
        return true;
    }
    
    /**
     * 获取学生信息
     * @param studentId - 学生的ID
     * @returns 找到的学生对象，若不存在则返回null
     */
    getStudent(studentId: string): Student | null {
        if (!this.students.has(studentId)) {
            return null;
        }
        
        return this.students.get(studentId)!;
    }
    
    /**
     * 删除学生
     * @param studentId - 要删除的学生的ID
     * @returns 删除成功返回true，否则返回false
     */
    removeStudent(studentId: string): boolean {
        if (!this.students.has(studentId)) {
            return false;
        }
        
        // 删除学生的所有注册记录
        for (const [key, enrollment] of this.enrollments) {
            if (enrollment.studentId === studentId) {
                this.enrollments.delete(key);
            }
        }
        
        // 删除学生的所有成绩记录
        this.grades.delete(studentId);
        
        // 删除学生的所有出勤记录
        this.attendance.delete(studentId);
        
        // 删除学生
        this.students.delete(studentId);
        
        return true;
    }
    
    /**
     * 添加新课程
     * @param courseCode - 课程的唯一代码
     * @param title - 课程标题
     * @param description - 课程描述
     * @param credits - 课程学分
     * @param department - 所属院系
     * @param schedule - 课程安排
     * @param teacherId - 授课教师ID（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addCourse(courseCode: string, title: string, description: string, 
               credits: number, department: string, schedule: Schedule, 
               teacherId?: string): boolean {
        if (this.courses.has(courseCode)) {
            return false;
        }
        
        const course = new Course(
            courseCode, 
            title, 
            description, 
            credits, 
            department, 
            schedule
        );
        
        if (teacherId && this.teachers.has(teacherId)) {
            course.teacherId = teacherId;
        }
        
        this.courses.set(courseCode, course);
        
        return true;
    }
    
    /**
     * 更新课程信息
     * @param courseCode - 要更新的课程代码
     * @param title - 新的标题（可选）
     * @param description - 新的描述（可选）
     * @param credits - 新的学分（可选）
     * @param department - 新的院系（可选）
     * @param schedule - 新的课程安排（可选）
     * @param teacherId - 新的教师ID（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateCourse(courseCode: string, title?: string, description?: string, 
                  credits?: number, department?: string, schedule?: Schedule, 
                  teacherId?: string): boolean {
        if (!this.courses.has(courseCode)) {
            return false;
        }
        
        const course = this.courses.get(courseCode)!;
        
        if (title !== undefined) {
            course.title = title;
        }
        
        if (description !== undefined) {
            course.description = description;
        }
        
        if (credits !== undefined) {
            course.credits = credits;
        }
        
        if (department !== undefined) {
            course.department = department;
        }
        
        if (schedule !== undefined) {
            course.schedule = schedule;
        }
        
        if (teacherId !== undefined) {
            if (teacherId === null) {
                course.teacherId = undefined;
            } else if (this.teachers.has(teacherId)) {
                course.teacherId = teacherId;
            } else {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * 获取课程信息
     * @param courseCode - 课程代码
     * @returns 找到的课程对象，若不存在则返回null
     */
    getCourse(courseCode: string): Course | null {
        if (!this.courses.has(courseCode)) {
            return null;
        }
        
        return this.courses.get(courseCode)!;
    }
    
    /**
     * 删除课程
     * @param courseCode - 要删除的课程代码
     * @returns 删除成功返回true，否则返回false
     */
    removeCourse(courseCode: string): boolean {
        if (!this.courses.has(courseCode)) {
            return false;
        }
        
        // 删除所有与该课程相关的注册记录
        for (const [key, enrollment] of this.enrollments) {
            if (enrollment.courseCode === courseCode) {
                this.enrollments.delete(key);
            }
        }
        
        // 删除所有与该课程相关的成绩记录
        for (const [studentId, grades] of this.grades) {
            const filteredGrades = grades.filter(grade => grade.courseCode !== courseCode);
            this.grades.set(studentId, filteredGrades);
        }
        
        // 删除所有与该课程相关的出勤记录
        for (const [studentId, attendanceRecords] of this.attendance) {
            const filteredAttendance = attendanceRecords.filter(record => record.courseCode !== courseCode);
            this.attendance.set(studentId, filteredAttendance);
        }
        
        // 删除课程
        this.courses.delete(courseCode);
        
        return true;
    }
    
    /**
     * 注册学生到课程
     * @param studentId - 学生ID
     * @param courseCode - 课程代码
     * @param semester - 学期
     * @param year - 学年
     * @returns 注册成功返回true，否则返回false
     */
    enrollStudent(studentId: string, courseCode: string, semester: string, year: number): boolean {
        if (!this.students.has(studentId) || !this.courses.has(courseCode)) {
            return false;
        }
        
        // 检查学生是否已注册该课程
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId && enrollment.courseCode === courseCode) {
                return false;
            }
        }
        
        const enrollment = new Enrollment(studentId, courseCode, semester, year);
        const enrollmentId = `${studentId}_${courseCode}_${semester}_${year}`;
        
        this.enrollments.set(enrollmentId, enrollment);
        
        return true;
    }
    
    /**
     * 取消学生课程注册
     * @param studentId - 学生ID
     * @param courseCode - 课程代码
     * @param semester - 学期
     * @param year - 学年
     * @returns 取消成功返回true，否则返回false
     */
    unenrollStudent(studentId: string, courseCode: string, semester: string, year: number): boolean {
        const enrollmentId = `${studentId}_${courseCode}_${semester}_${year}`;
        
        if (!this.enrollments.has(enrollmentId)) {
            return false;
        }
        
        // 删除该课程的所有成绩记录
        if (this.grades.has(studentId)) {
            const grades = this.grades.get(studentId)!;
            const filteredGrades = grades.filter(grade => grade.courseCode !== courseCode);
            this.grades.set(studentId, filteredGrades);
        }
        
        // 删除该课程的所有出勤记录
        if (this.attendance.has(studentId)) {
            const attendanceRecords = this.attendance.get(studentId)!;
            const filteredAttendance = attendanceRecords.filter(record => record.courseCode !== courseCode);
            this.attendance.set(studentId, filteredAttendance);
        }
        
        // 删除注册记录
        this.enrollments.delete(enrollmentId);
        
        return true;
    }
    
    /**
     * 获取学生注册的所有课程
     * @param studentId - 学生ID
     * @param semester - 学期筛选（可选）
     * @param year - 学年筛选（可选）
     * @returns 课程代码列表
     */
    getStudentCourses(studentId: string, semester?: string, year?: number): string[] {
        if (!this.students.has(studentId)) {
            return [];
        }
        
        const courses: string[] = [];
        
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId) {
                if (semester && enrollment.semester !== semester) {
                    continue;
                }
                
                if (year && enrollment.year !== year) {
                    continue;
                }
                
                courses.push(enrollment.courseCode);
            }
        }
        
        return courses;
    }
    
    /**
     * 获取课程中的所有学生
     * @param courseCode - 课程代码
     * @param semester - 学期筛选（可选）
     * @param year - 学年筛选（可选）
     * @returns 学生ID列表
     */
    getCourseStudents(courseCode: string, semester?: string, year?: number): string[] {
        if (!this.courses.has(courseCode)) {
            return [];
        }
        
        const students: string[] = [];
        
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.courseCode === courseCode) {
                if (semester && enrollment.semester !== semester) {
                    continue;
                }
                
                if (year && enrollment.year !== year) {
                    continue;
                }
                
                students.push(enrollment.studentId);
            }
        }
        
        return students;
    }
    
    /**
     * 添加学生成绩
     * @param studentId - 学生ID
     * @param courseCode - 课程代码
     * @param assessmentType - 评估类型（例如：考试、作业、项目）
     * @param score - 分数
     * @param maxScore - 最高分数
     * @param date - 评估日期
     * @param comments - 评论（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addGrade(studentId: string, courseCode: string, assessmentType: string, 
              score: number, maxScore: number, date: Date, comments?: string): boolean {
        if (!this.students.has(studentId) || !this.courses.has(courseCode)) {
            return false;
        }
        
        // 检查学生是否注册了该课程
        let isEnrolled = false;
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId && enrollment.courseCode === courseCode) {
                isEnrolled = true;
                break;
            }
        }
        
        if (!isEnrolled) {
            return false;
        }
        
        if (score < 0 || score > maxScore) {
            return false;
        }
        
        const grade = new Grade(
            courseCode, 
            assessmentType, 
            score, 
            maxScore, 
            date, 
            comments
        );
        
        if (!this.grades.has(studentId)) {
            this.grades.set(studentId, []);
        }
        
        this.grades.get(studentId)!.push(grade);
        
        return true;
    }
    
    /**
     * 获取学生成绩
     * @param studentId - 学生ID
     * @param courseCode - 课程代码筛选（可选）
     * @param assessmentType - 评估类型筛选（可选）
     * @returns 成绩列表
     */
    getStudentGrades(studentId: string, courseCode?: string, assessmentType?: string): Grade[] {
        if (!this.students.has(studentId) || !this.grades.has(studentId)) {
            return [];
        }
        
        let grades = [...this.grades.get(studentId)!];
        
        if (courseCode) {
            grades = grades.filter(grade => grade.courseCode === courseCode);
        }
        
        if (assessmentType) {
            grades = grades.filter(grade => grade.assessmentType === assessmentType);
        }
        
        return grades;
    }
    
    /**
     * 计算学生课程平均成绩
     * @param studentId - 学生ID
     * @param courseCode - 课程代码
     * @returns 平均成绩，若无成绩则返回null
     */
    calculateCourseAverage(studentId: string, courseCode: string): number | null {
        const grades = this.getStudentGrades(studentId, courseCode);
        
        if (grades.length === 0) {
            return null;
        }
        
        let totalScore = 0;
        let totalMaxScore = 0;
        
        for (const grade of grades) {
            totalScore += grade.score;
            totalMaxScore += grade.maxScore;
        }
        
        return totalScore / totalMaxScore * 100;
    }
    
    /**
     * 计算学生总体平均成绩
     * @param studentId - 学生ID
     * @returns 总体平均成绩，若无成绩则返回null
     */
    calculateOverallAverage(studentId: string): number | null {
        const grades = this.getStudentGrades(studentId);
        
        if (grades.length === 0) {
            return null;
        }
        
        let totalScore = 0;
        let totalMaxScore = 0;
        
        for (const grade of grades) {
            totalScore += grade.score;
            totalMaxScore += grade.maxScore;
        }
        
        return totalScore / totalMaxScore * 100;
    }
    
    /**
     * 记录学生出勤
     * @param studentId - 学生ID
     * @param courseCode - 课程代码
     * @param date - 出勤日期
     * @param status - 出勤状态（出席、缺席、迟到）
     * @param remarks - 备注（可选）
     * @returns 添加成功返回true，否则返回false
     */
    recordAttendance(studentId: string, courseCode: string, date: Date, 
                     status: AttendanceStatus, remarks?: string): boolean {
        if (!this.students.has(studentId) || !this.courses.has(courseCode)) {
            return false;
        }
        
        // 检查学生是否注册了该课程
        let isEnrolled = false;
        for (const enrollment of this.enrollments.values()) {
            if (enrollment.studentId === studentId && enrollment.courseCode === courseCode) {
                isEnrolled = true;
                break;
            }
        }
        
        if (!isEnrolled) {
            return false;
        }
        
        // 检查是否已存在该日期的出勤记录
        if (this.attendance.has(studentId)) {
            const attendanceRecords = this.attendance.get(studentId)!;
            for (const record of attendanceRecords) {
                if (record.courseCode === courseCode && 
                    record.date.toDateString() === date.toDateString()) {
                    record.status = status;
                    if (remarks !== undefined) {
                        record.remarks = remarks;
                    }
                    record.updatedAt = new Date();
                    return true;
                }
            }
        }
        
        const attendance = new Attendance(courseCode, date, status, remarks);
        
        if (!this.attendance.has(studentId)) {
            this.attendance.set(studentId, []);
        }
        
        this.attendance.get(studentId)!.push(attendance);
        
        return true;
    }
    
    /**
     * 获取学生出勤记录
     * @param studentId - 学生ID
     * @param courseCode - 课程代码筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @returns 出勤记录列表
     */
    getStudentAttendance(studentId: string, courseCode?: string, 
                         startDate?: Date, endDate?: Date): Attendance[] {
        if (!this.students.has(studentId) || !this.attendance.has(studentId)) {
            return [];
        }
        
        let attendanceRecords = [...this.attendance.get(studentId)!];
        
        if (courseCode) {
            attendanceRecords = attendanceRecords.filter(record => record.courseCode === courseCode);
        }
        
        if (startDate) {
            attendanceRecords = attendanceRecords.filter(record => record.date >= startDate);
        }
        
        if (endDate) {
            attendanceRecords = attendanceRecords.filter(record => record.date <= endDate);
        }
        
        return attendanceRecords;
    }
    
    /**
     * 计算学生课程出勤率
     * @param studentId - 学生ID
     * @param courseCode - 课程代码
     * @returns 出勤率，若无记录则返回null
     */
    calculateAttendanceRate(studentId: string, courseCode: string): number | null {
        const attendanceRecords = this.getStudentAttendance(studentId, courseCode);
        
        if (attendanceRecords.length === 0) {
            return null;
        }
        
        const presentCount = attendanceRecords.filter(
            record => record.status === AttendanceStatus.Present
        ).length;
        
        return presentCount / attendanceRecords.length * 100;
    }
    
    /**
     * 添加教师
     * @param teacherId - 教师ID
     * @param firstName - 教师名字
     * @param lastName - 教师姓氏
     * @param email - 教师电子邮件
     * @param phoneNumber - 教师电话号码
     * @param department - 所在院系
     * @param title - 职称
     * @returns 添加成功返回true，否则返回false
     */
    addTeacher(teacherId: string, firstName: string, lastName: string, email: string, 
                phoneNumber: string, department: string, title: string): boolean {
        if (this.teachers.has(teacherId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const teacher of this.teachers.values()) {
            if (teacher.email === email) {
                return false;
            }
        }
        
        const teacher = new Teacher(
            teacherId, 
            firstName, 
            lastName, 
            email, 
            phoneNumber, 
            department, 
            title
        );
        
        this.teachers.set(teacherId, teacher);
        
        return true;
    }
    
    /**
     * 获取教师信息
     * @param teacherId - 教师ID
     * @returns 教师对象，若不存在则返回null
     */
    getTeacher(teacherId: string): Teacher | null {
        if (!this.teachers.has(teacherId)) {
            return null;
        }
        
        return this.teachers.get(teacherId)!;
    }
    
    /**
     * 更新教师信息
     * @param teacherId - 教师ID
     * @param firstName - 新的名字（可选）
     * @param lastName - 新的姓氏（可选）
     * @param email - 新的电子邮件（可选）
     * @param phoneNumber - 新的电话号码（可选）
     * @param department - 新的院系（可选）
     * @param title - 新的职称（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateTeacher(teacherId: string, firstName?: string, lastName?: string, 
                   email?: string, phoneNumber?: string, department?: string, 
                   title?: string): boolean {
        if (!this.teachers.has(teacherId)) {
            return false;
        }
        
        const teacher = this.teachers.get(teacherId)!;
        
        // 如果提供了电子邮件，检查是否已被其他教师使用
        if (email && email !== teacher.email) {
            for (const [id, t] of this.teachers) {
                if (id !== teacherId && t.email === email) {
                    return false;
                }
            }
            teacher.email = email;
        }
        
        if (firstName !== undefined) {
            teacher.firstName = firstName;
        }
        
        if (lastName !== undefined) {
            teacher.lastName = lastName;
        }
        
        if (phoneNumber !== undefined) {
            teacher.phoneNumber = phoneNumber;
        }
        
        if (department !== undefined) {
            teacher.department = department;
        }
        
        if (title !== undefined) {
            teacher.title = title;
        }
        
        return true;
    }
    
    /**
     * 删除教师
     * @param teacherId - 要删除的教师ID
     * @returns 删除成功返回true，否则返回false
     */
    removeTeacher(teacherId: string): boolean {
        if (!this.teachers.has(teacherId)) {
            return false;
        }
        
        // 从所有课程中移除该教师
        for (const course of this.courses.values()) {
            if (course.teacherId === teacherId) {
                course.teacherId = undefined;
            }
        }
        
        // 删除教师
        this.teachers.delete(teacherId);
        
        return true;
    }
    
    /**
     * 获取教师教授的课程
     * @param teacherId - 教师ID
     * @returns 课程代码列表
     */
    getTeacherCourses(teacherId: string): string[] {
        if (!this.teachers.has(teacherId)) {
            return [];
        }
        
        const courses: string[] = [];
        
        for (const [courseCode, course] of this.courses) {
            if (course.teacherId === teacherId) {
                courses.push(courseCode);
            }
        }
        
        return courses;
    }
}

/**
 * 学生类
 */
class Student {
    studentId: string;
    firstName: string;
    lastName: string;
    email: string;
    phoneNumber: string;
    dateOfBirth: Date;
    address: string;
    enrolledAt: Date;
    
    constructor(studentId: string, firstName: string, lastName: string, email: string, 
                phoneNumber: string, dateOfBirth: Date, address: string) {
        this.studentId = studentId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phoneNumber = phoneNumber;
        this.dateOfBirth = dateOfBirth;
        this.address = address;
        this.enrolledAt = new Date();
    }
    
    /**
     * 获取全名
     */
    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }
}

/**
 * 课程类
 */
class Course {
    courseCode: string;
    title: string;
    description: string;
    credits: number;
    department: string;
    schedule: Schedule;
    teacherId?: string;
    createdAt: Date;
    
    constructor(courseCode: string, title: string, description: string, 
                credits: number, department: string, schedule: Schedule) {
        this.courseCode = courseCode;
        this.title = title;
        this.description = description;
        this.credits = credits;
        this.department = department;
        this.schedule = schedule;
        this.createdAt = new Date();
    }
}

/**
 * 课程安排接口
 */
interface Schedule {
    dayOfWeek: string;  // 星期几
    startTime: string;  // 开始时间
    endTime: string;    // 结束时间
    room: string;       // 教室
}

/**
 * 注册类
 */
class Enrollment {
    studentId: string;
    courseCode: string;
    semester: string;
    year: number;
    enrolledAt: Date;
    
    constructor(studentId: string, courseCode: string, semester: string, year: number) {
        this.studentId = studentId;
        this.courseCode = courseCode;
        this.semester = semester;
        this.year = year;
        this.enrolledAt = new Date();
    }
}

/**
 * 成绩类
 */
class Grade {
    courseCode: string;
    assessmentType: string;
    score: number;
    maxScore: number;
    date: Date;
    comments?: string;
    createdAt: Date;
    
    constructor(courseCode: string, assessmentType: string, score: number, 
                maxScore: number, date: Date, comments?: string) {
        this.courseCode = courseCode;
        this.assessmentType = assessmentType;
        this.score = score;
        this.maxScore = maxScore;
        this.date = date;
        this.comments = comments;
        this.createdAt = new Date();
    }
    
    /**
     * 获取百分比分数
     */
    getPercentage(): number {
        return this.score / this.maxScore * 100;
    }
}

/**
 * 出勤状态枚举
 */
enum AttendanceStatus {
    Present = "present",
    Absent = "absent",
    Late = "late"
}

/**
 * 出勤记录类
 */
class Attendance {
    courseCode: string;
    date: Date;
    status: AttendanceStatus;
    remarks?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(courseCode: string, date: Date, status: AttendanceStatus, remarks?: string) {
        this.courseCode = courseCode;
        this.date = date;
        this.status = status;
        this.remarks = remarks;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 教师类
 */
class Teacher {
    teacherId: string;
    firstName: string;
    lastName: string;
    email: string;
    phoneNumber: string;
    department: string;
    title: string;
    hiredAt: Date;
    
    constructor(teacherId: string, firstName: string, lastName: string, email: string, 
                phoneNumber: string, department: string, title: string) {
        this.teacherId = teacherId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phoneNumber = phoneNumber;
        this.department = department;
        this.title = title;
        this.hiredAt = new Date();
    }
    
    /**
     * 获取全名
     */
    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }
}