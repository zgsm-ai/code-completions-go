"""
Student Management System
This Python program implements a comprehensive student management system with courses, grades, and attendance.
"""

import random
import datetime
from typing import List, Dict, Optional, Any
from dataclasses import dataclass, field
from enum import Enum


class Grade(Enum):
    """Grade levels"""
    FRESHMAN = "Freshman"
    SOPHOMORE = "Sophomore"
    JUNIOR = "Junior"
    SENIOR = "Senior"


class Major(Enum):
    """Academic majors"""
    COMPUTER_SCIENCE = "Computer Science"
    MATHEMATICS = "Mathematics"
    PHYSICS = "Physics"
    CHEMISTRY = "Chemistry"
    BIOLOGY = "Biology"
    ENGLISH = "English"
    HISTORY = "History"
    ECONOMICS = "Economics"


@dataclass
class Address:
    """Student address information"""
    street: str
    city: str
    state: str
    zip_code: str
    country: str = "USA"


@dataclass
class Course:
    """Course information"""
    course_id: str
    name: str
    description: str
    credits: int
    department: str
    semester: str
    year: int
    max_students: int = 30
    
    def __str__(self):
        return f"{self.course_id}: {self.name} ({self.credits} credits)"


@dataclass
class AttendanceRecord:
    """Student attendance record"""
    date: datetime.date
    present: bool
    late: bool = False
    
    def __str__(self):
        status = "Present"
        if not self.present:
            status = "Absent"
        elif self.late:
            status = "Late"
        return f"{self.date}: {status}"


@dataclass
class Grade:
    """Course grade information"""
    course_id: str
    course_name: str
    score: float
    letter_grade: str = ""
    
    def calculate_letter_grade(self):
        """Calculate letter grade based on score"""
        if self.score >= 93:
            return "A"
        elif self.score >= 90:
            return "A-"
        elif self.score >= 87:
            return "B+"
        elif self.score >= 83:
            return "B"
        elif self.score >= 80:
            return "B-"
        elif self.score >= 77:
            return "C+"
        elif self.score >= 73:
            return "C"
        elif self.score >= 70:
            return "C-"
        elif self.score >= 60:
            return "D"
        else:
            return "F"
    
    def __post_init__(self):
        """Calculate letter grade after initialization"""
        self.letter_grade = self.calculate_letter_grade()


@dataclass
class Student:
    """Student information"""
    student_id: str
    first_name: str
    last_name: str
    email: str
    phone: str
    date_of_birth: datetime.date
    grade_level: Grade
    major: Major
    address: Address
    enrollment_date: datetime.date = field(default_factory=datetime.date.today)
    gpa: float = 0.0
    credits_completed: int = 0
    courses: List[str] = field(default_factory=list)
    grades: Dict[str, Grade] = field(default_factory=dict)
    attendance: List[AttendanceRecord] = field(default_factory=list)
    
    def get_full_name(self) -> str:
        """Get student's full name"""
        return f"{self.first_name} {self.last_name}"
    
    def get_age(self) -> int:
        """Calculate student's age"""
        today = datetime.date.today()
        return today.year - self.date_of_birth.year - (
            (today.month, today.day) < (self.date_of_birth.month, self.date_of_birth.day)
        )
    
    def enroll_in_course(self, course_id: str) -> bool:
        """Enroll student in a course"""
        if course_id not in self.courses:
            self.courses.append(course_id)
            return True
        return False
    
    def drop_course(self, course_id: str) -> bool:
        """Drop a course"""
        if course_id in self.courses:
            self.courses.remove(course_id)
            # Remove grade if exists
            if course_id in self.grades:
                del self.grades[course_id]
            return True
        return False
    
    def add_grade(self, course_id: str, course_name: str, score: float) -> bool:
        """Add a grade for a course"""
        if course_id in self.courses:
            grade = Grade(course_id, course_name, score)
            self.grades[course_id] = grade
            self.calculate_gpa()
            return True
        return False
    
    def calculate_gpa(self) -> float:
        """Calculate student's GPA based on grades"""
        if not self.grades:
            self.gpa = 0.0
            return 0.0
        
        total_points = 0.0
        total_credits = 0
        
        # Grade point values
        grade_points = {
            "A": 4.0, "A-": 3.7,
            "B+": 3.3, "B": 3.0, "B-": 2.7,
            "C+": 2.3, "C": 2.0, "C-": 1.7,
            "D": 1.0, "F": 0.0
        }
        
        for course_id, grade in self.grades.items():
            # In a real system, we'd get credits from course data
            # For simplicity, assuming 3 credits per course
            credits = 3
            letter_grade = grade.letter_grade
            points = grade_points.get(letter_grade, 0.0)
            
            total_points += points * credits
            total_credits += credits
        
        if total_credits > 0:
            self.gpa = total_points / total_credits
        else:
            self.gpa = 0.0
            
        return self.gpa
    
    def record_attendance(self, date: datetime.date, present: bool, late: bool = False) -> None:
        """Record attendance for a specific date"""
        record = AttendanceRecord(date, present, late)
        self.attendance.append(record)
    
    def get_attendance_rate(self) -> float:
        """Calculate attendance rate as percentage"""
        if not self.attendance:
            return 0.0
        
        present_days = sum(1 for record in self.attendance if record.present)
        return (present_days / len(self.attendance)) * 100
    
    def __str__(self) -> str:
        return f"{self.student_id}: {self.get_full_name()} ({self.major.value})"


class StudentManagementSystem:
    """Main student management system"""
    
    def __init__(self):
        self.students: Dict[str, Student] = {}
        self.courses: Dict[str, Course] = {}
    
    def generate_student_id(self) -> str:
        """Generate a unique student ID"""
        while True:
            student_id = f"STU{random.randint(10000, 99999)}"
            if student_id not in self.students:
                return student_id
    
    def generate_course_id(self) -> str:
        """Generate a unique course ID"""
        while True:
            course_id = f"CRS{random.randint(100, 999)}"
            if course_id not in self.courses:
                return course_id
    
    def add_student(self, first_name: str, last_name: str, email: str, phone: str,
                 date_of_birth: datetime.date, grade_level: Grade, major: Major,
                 address: Address) -> Student:
        """Add a new student to the system"""
        student_id = self.generate_student_id()
        student = Student(
            student_id=student_id,
            first_name=first_name,
            last_name=last_name,
            email=email,
            phone=phone,
            date_of_birth=date_of_birth,
            grade_level=grade_level,
            major=major,
            address=address
        )
        
        self.students[student_id] = student
        return student
    
    def get_student(self, student_id: str) -> Optional[Student]:
        """Get a student by ID"""
        return self.students.get(student_id)
    
    def update_student(self, student_id: str, **kwargs) -> bool:
        """Update student information"""
        student = self.get_student(student_id)
        if not student:
            return False
        
        for key, value in kwargs.items():
            if hasattr(student, key):
                setattr(student, key, value)
        
        return True
    
    def delete_student(self, student_id: str) -> bool:
        """Delete a student from the system"""
        if student_id in self.students:
            del self.students[student_id]
            return True
        return False
    
    def add_course(self, name: str, description: str, credits: int, 
                  department: str, semester: str, year: int,
                  max_students: int = 30) -> Course:
        """Add a new course to the system"""
        course_id = self.generate_course_id()
        course = Course(
            course_id=course_id,
            name=name,
            description=description,
            credits=credits,
            department=department,
            semester=semester,
            year=year,
            max_students=max_students
        )
        
        self.courses[course_id] = course
        return course
    
    def get_course(self, course_id: str) -> Optional[Course]:
        """Get a course by ID"""
        return self.courses.get(course_id)
    
    def enroll_student_in_course(self, student_id: str, course_id: str) -> bool:
        """Enroll a student in a course"""
        student = self.get_student(student_id)
        course = self.get_course(course_id)
        
        if not student or not course:
            return False
        
        # Check if course is full
        enrolled_students = self.get_students_in_course(course_id)
        if len(enrolled_students) >= course.max_students:
            return False
        
        return student.enroll_in_course(course_id)
    
    def drop_student_from_course(self, student_id: str, course_id: str) -> bool:
        """Drop a student from a course"""
        student = self.get_student(student_id)
        if not student:
            return False
        
        return student.drop_course(course_id)
    
    def add_student_grade(self, student_id: str, course_id: str, score: float) -> bool:
        """Add a grade for a student in a course"""
        student = self.get_student(student_id)
        course = self.get_course(course_id)
        
        if not student or not course:
            return False
        
        return student.add_grade(course_id, course.name, score)
    
    def record_student_attendance(self, student_id: str, date: datetime.date, 
                             present: bool, late: bool = False) -> bool:
        """Record attendance for a student"""
        student = self.get_student(student_id)
        if not student:
            return False
        
        student.record_attendance(date, present, late)
        return True
    
    def get_students_in_course(self, course_id: str) -> List[Student]:
        """Get all students enrolled in a course"""
        return [student for student in self.students.values() 
                if course_id in student.courses]
    
    def get_students_by_major(self, major: Major) -> List[Student]:
        """Get all students with a specific major"""
        return [student for student in self.students.values() 
                if student.major == major]
    
    def get_students_by_grade_level(self, grade_level: Grade) -> List[Student]:
        """Get all students at a specific grade level"""
        return [student for student in self.students.values() 
                if student.grade_level == grade_level]
    
    def search_students(self, query: str) -> List[Student]:
        """Search for students by name or email"""
        query = query.lower()
        results = []
        
        for student in self.students.values():
            if (query in student.first_name.lower() or 
                query in student.last_name.lower() or 
                query in student.email.lower()):
                results.append(student)
        
        return results
    
    def calculate_class_gpa(self, course_id: str) -> float:
        """Calculate the average GPA for all students in a course"""
        course_students = self.get_students_in_course(course_id)
        if not course_students:
            return 0.0
        
        total_gpa = sum(student.gpa for student in course_students 
                      if course_id in student.grades)
        
        if course_students:
            return total_gpa / len(course_students)
        return 0.0
    
    def get_top_students(self, count: int = 10) -> List[Student]:
        """Get the top students by GPA"""
        return sorted(self.students.values(), 
                   key=lambda student: student.gpa, reverse=True)[:count]
    
    def get_at_risk_students(self, gpa_threshold: float = 2.0) -> List[Student]:
        """Get students who may be at risk (low GPA)"""
        return [student for student in self.students.values() 
                if student.gpa < gpa_threshold]
    
    def generate_student_report(self, student_id: str) -> Optional[Dict[str, Any]]:
        """Generate a comprehensive report for a student"""
        student = self.get_student(student_id)
        if not student:
            return None
        
        return {
            "student_info": {
                "id": student.student_id,
                "name": student.get_full_name(),
                "email": student.email,
                "age": student.get_age(),
                "grade_level": student.grade_level.value,
                "major": student.major.value,
                "gpa": round(student.gpa, 2),
                "credits": student.credits_completed,
                "enrollment_date": student.enrollment_date.strftime("%Y-%m-%d")
            },
            "courses": [
                {
                    "course_id": course_id,
                    "name": self.courses[course_id].name if course_id in self.courses else "Unknown"
                }
                for course_id in student.courses
            ],
            "grades": [
                {
                    "course_id": grade.course_id,
                    "course_name": grade.course_name,
                    "score": grade.score,
                    "letter_grade": grade.letter_grade
                }
                for grade in student.grades.values()
            ],
            "attendance_rate": round(student.get_attendance_rate(), 1)
        }
    
    def generate_course_report(self, course_id: str) -> Optional[Dict[str, Any]]:
        """Generate a comprehensive report for a course"""
        course = self.get_course(course_id)
        if not course:
            return None
        
        students = self.get_students_in_course(course_id)
        grades = [
            student.grades[course_id].score
            for student in students
            if course_id in student.grades
        ]
        
        return {
            "course_info": {
                "id": course.course_id,
                "name": course.name,
                "description": course.description,
                "credits": course.credits,
                "department": course.department,
                "semester": course.semester,
                "year": course.year,
                "max_students": course.max_students,
                "enrolled": len(students)
            },
            "statistics": {
                "average_score": sum(grades) / len(grades) if grades else 0,
                "high_score": max(grades) if grades else 0,
                "low_score": min(grades) if grades else 0,
                "class_gpa": round(self.calculate_class_gpa(course_id), 2)
            }
        }
    
    def generate_system_statistics(self) -> Dict[str, Any]:
        """Generate overall system statistics"""
        total_students = len(self.students)
        total_courses = len(self.courses)
        
        # Calculate grade distribution
        all_grades = []
        for student in self.students.values():
            all_grades.extend([grade.score for grade in student.grades.values()])
        
        # Calculate enrollment by major
        major_counts = {}
        for student in self.students.values():
            major = student.major.value
            major_counts[major] = major_counts.get(major, 0) + 1
        
        # Calculate enrollment by grade level
        grade_counts = {}
        for student in self.students.values():
            grade = student.grade_level.value
            grade_counts[grade] = grade_counts.get(grade, 0) + 1
        
        return {
            "overview": {
                "total_students": total_students,
                "total_courses": total_courses,
                "total_enrollments": sum(len(student.courses) for student in self.students.values())
            },
            "grade_distribution": {
                "average_score": sum(all_grades) / len(all_grades) if all_grades else 0,
                "high_score": max(all_grades) if all_grades else 0,
                "low_score": min(all_grades) if all_grades else 0
            },
            "major_distribution": major_counts,
            "grade_level_distribution": grade_counts
        }


def generate_sample_data() -> StudentManagementSystem:
    """Generate sample data for the system"""
    system = StudentManagementSystem()
    
    # Add courses
    comp_sci_101 = system.add_course(
        "Introduction to Computer Science",
        "Basic concepts of programming and computer science",
        3, "Computer Science", "Fall", 2025, 30
    )
    
    math_201 = system.add_course(
        "Calculus II",
        "Advanced calculus topics including integration techniques",
        4, "Mathematics", "Fall", 2025, 25
    )
    
    phys_101 = system.add_course(
        "Introduction to Physics",
        "Basic principles of mechanics and thermodynamics",
        4, "Physics", "Fall", 2025, 20
    )
    
    econ_101 = system.add_course(
        "Introduction to Economics",
        "Fundamental concepts of microeconomics and macroeconomics",
        3, "Economics", "Fall", 2025, 35
    )
    
    # Add students
    john_doe = system.add_student(
        "John", "Doe", "john.doe@example.com", "555-1234",
        datetime.date(2003, 5, 15), Grade.FRESHMAN, Major.COMPUTER_SCIENCE,
        Address("123 Main St", "Boston", "MA", "02108")
    )
    
    jane_smith = system.add_student(
        "Jane", "Smith", "jane.smith@example.com", "555-5678",
        datetime.date(2002, 8, 22), Grade.SOPHOMORE, Major.MATHEMATICS,
        Address("456 Oak Ave", "Cambridge", "MA", "02139")
    )
    
    mike_johnson = system.add_student(
        "Mike", "Johnson", "mike.johnson@example.com", "555-9012",
        datetime.date(2001, 3, 10), Grade.JUNIOR, Major.PHYSICS,
        Address("789 Pine Rd", "Newton", "MA", "02158")
    )
    
    sarah_williams = system.add_student(
        "Sarah", "Williams", "sarah.williams@example.com", "555-3456",
        datetime.date(2000, 11, 5), Grade.SENIOR, Major.ECONOMICS,
        Address("321 Elm St", "Quincy", "MA", "02169")
    )
    
    # Enroll students in courses
    system.enroll_student_in_course(john_doe.student_id, comp_sci_101.course_id)
    system.enroll_student_in_course(jane_smith.student_id, comp_sci_101.course_id)
    system.enroll_student_in_course(mike_johnson.student_id, comp_sci_101.course_id)
    
    system.enroll_student_in_course(john_doe.student_id, math_201.course_id)
    system.enroll_student_in_course(jane_smith.student_id, math_201.course_id)
    
    system.enroll_student_in_course(mike_johnson.student_id, phys_101.course_id)
    system.enroll_student_in_course(sarah_williams.student_id, phys_101.course_id)
    
    system.enroll_student_in_course(sarah_williams.student_id, econ_101.course_id)
    
    # Add grades
    system.add_student_grade(john_doe.student_id, comp_sci_101.course_id, 92.5)
    system.add_student_grade(jane_smith.student_id, comp_sci_101.course_id, 88.0)
    system.add_student_grade(mike_johnson.student_id, comp_sci_101.course_id, 95.0)
    
    system.add_student_grade(john_doe.student_id, math_201.course_id, 85.0)
    system.add_student_grade(jane_smith.student_id, math_201.course_id, 93.5)
    
    system.add_student_grade(mike_johnson.student_id, phys_101.course_id, 90.0)
    system.add_student_grade(sarah_williams.student_id, phys_101.course_id, 87.5)
    
    system.add_student_grade(sarah_williams.student_id, econ_101.course_id, 91.0)
    
    # Record some attendance
    today = datetime.date.today()
    for i in range(10):
        date = today - datetime.timedelta(days=i)
        
        # John's attendance for Comp Sci
        system.record_student_attendance(john_doe.student_id, date, True, i == 2)
        
        # Jane's attendance for Comp Sci
        system.record_student_attendance(jane_smith.student_id, date, True, i == 1)
        
        # Mike's attendance for Comp Sci
        system.record_student_attendance(mike_johnson.student_id, date, i < 8, i == 5)
    
    return system


def display_student_report(system: StudentManagementSystem, student_id: str):
    """Display a formatted student report"""
    report = system.generate_student_report(student_id)
    if not report:
        print(f"No student found with ID: {student_id}")
        return
    
    student_info = report["student_info"]
    print(f"Student Report: {student_info['name']} ({student_info['id']})")
    print(f"Email: {student_info['email']}")
    print(f"Age: {student_info['age']}")
    print(f"Grade Level: {student_info['grade_level']}")
    print(f"Major: {student_info['major']}")
    print(f"GPA: {student_info['gpa']}")
    print(f"Credits: {student_info['credits']}")
    print(f"Enrollment Date: {student_info['enrollment_date']}")
    print(f"Attendance Rate: {student_info['attendance_rate']}%")
    
    print("\nCourses:")
    for course in report["courses"]:
        print(f"- {course['name']} ({course['course_id']})")
    
    print("\nGrades:")
    for grade in report["grades"]:
        print(f"- {grade['course_name']}: {grade['score']} ({grade['letter_grade']})")


def display_course_report(system: StudentManagementSystem, course_id: str):
    """Display a formatted course report"""
    report = system.generate_course_report(course_id)
    if not report:
        print(f"No course found with ID: {course_id}")
        return
    
    course_info = report["course_info"]
    print(f"Course Report: {course_info['name']} ({course_info['id']})")
    print(f"Department: {course_info['department']}")
    print(f"Semester: {course_info['semester']} {course_info['year']}")
    print(f"Credits: {course_info['credits']}")
    print(f"Description: {course_info['description']}")
    print(f"Enrollment: {course_info['enrolled']}/{course_info['max_students']}")
    
    stats = report["statistics"]
    print(f"\nStatistics:")
    print(f"Average Score: {stats['average_score']:.2f}")
    print(f"Highest Score: {stats['high_score']:.2f}")
    print(f"Lowest Score: {stats['low_score']:.2f}")
    print(f"Class GPA: {stats['class_gpa']}")


def display_system_statistics(system: StudentManagementSystem):
    """Display system-wide statistics"""
    stats = system.generate_system_statistics()
    
    print("=== System Statistics ===")
    overview = stats["overview"]
    print(f"Total Students: {overview['total_students']}")
    print(f"Total Courses: {overview['total_courses']}")
    print(f"Total Enrollments: {overview['total_enrollments']}")
    
    grade_dist = stats["grade_distribution"]
    print(f"\nGrade Distribution:")
    print(f"Average Score: {grade_dist['average_score']:.2f}")
    print(f"Highest Score: {grade_dist['high_score']:.2f}")
    print(f"Lowest Score: {grade_dist['low_score']:.2f}")
    
    print("\nMajor Distribution:")
    for major, count in stats["major_distribution"].items():
        print(f"- {major}: {count}")
    
    print("\nGrade Level Distribution:")
    for grade, count in stats["grade_level_distribution"].items():
        print(f"- {grade}: {count}")


def main():
    """Main function to demonstrate the system"""
    # Create system and generate sample data
    system = generate_sample_data()
    
    # Display system statistics
    display_system_statistics(system)
    
    # Display individual student reports
    print("\n=== Student Reports ===")
    for student_id in ["STU10001", "STU10002"]:
        display_student_report(system, student_id)
        print()
    
    # Display course reports
    print("=== Course Reports ===")
    for course_id in ["CRS100", "CRS101"]:
        display_course_report(system, course_id)
        print()
    
    # Search for students
    print("=== Search Results ===")
    search_results = system.search_students("john")
    print(f"Students matching 'john':")
    for student in search_results:
        print(f"- {student.get_full_name()} ({student.student_id})")
    
    # Top students
    print("\n=== Top Students ===")
    top_students = system.get_top_students(3)
    for i, student in enumerate(top_students, 1):
        print(f"{i}. {student.get_full_name()} - GPA: {student.gpa:.2f}")
    
    # At-risk students
    print("\n=== At-Risk Students ===")
    at_risk = system.get_at_risk_students()
    if at_risk:
        for student in at_risk:
            print(f"- {student.get_full_name()} - GPA: {student.gpa:.2f}")
    else:
        print("No at-risk students found.")
    
    # Display course enrollment
    print("\n=== Course Enrollment ===")
    for course_id in ["CRS100", "CRS101", "CRS102"]:
        students = system.get_students_in_course(course_id)
        course = system.get_course(course_id)
        if course:
            print(f"{course.name} ({course_id}):")
            for student in students:
                print(f"  - {student.get_full_name()} ({student.student_id})")


if __name__ == "__main__":
    main()