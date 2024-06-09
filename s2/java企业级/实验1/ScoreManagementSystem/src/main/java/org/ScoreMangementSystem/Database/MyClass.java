package org.ScoreMangementSystem.Database;

import org.ScoreMangementSystem.utils.MethodOrder;

import java.util.ArrayList;

public class MyClass {
    private int teacherId;
    private String courseName;
    private int totalStudents = 0;
    private String classCode;
    private String semester;

    public MyClass(int teacherId, String courseName, int totalStudents, String classCode, String semester) {
//        this.teacherId = teacherId;
        this.courseName = courseName;
        this.totalStudents = totalStudents;
        this.classCode = classCode;
        this.semester = semester;
    }
    @MethodOrder(3)
    public int myGetTeacherId() {
        return teacherId;
    }
    @MethodOrder(1)
    public String myGetCourseName() {
        return courseName;
    }
    @MethodOrder(5)
    public int myGetTotalStudents() {
        return totalStudents;
    }
    @MethodOrder(2)
    public String myGetClassCode() {
        return classCode;
    }
    @MethodOrder(4)
    public String myGetSemester() {
        return semester;
    }
    //    存储学生课程关系
    private ArrayList<StudentRelationshipClass> studentRelationshipClassList = new ArrayList<>();

    public ArrayList<StudentRelationshipClass> getStudentRelationshipClassList() {
        return studentRelationshipClassList;
    }
    public void addStudent(StudentRelationshipClass studentRelationshipClass) {
        this.studentRelationshipClassList.add(studentRelationshipClass);
        this.totalStudents = this.studentRelationshipClassList.size();
    }

    public ArrayList<Student> queryStudent() {
        ArrayList<Student> students = new ArrayList<>();
        for (StudentRelationshipClass studentRelationshipClass : this.studentRelationshipClassList) {
            students.add(studentRelationshipClass.myGetStudentByClass());
        }
        return students;
    }

//      根据班级查询学生成绩
    public ArrayList<StudentRelationshipClass.StudentGrade> queryScoreByClass() {
        ArrayList<StudentRelationshipClass.StudentGrade> classStudentGradeList = new ArrayList<>();
        for(StudentRelationshipClass studentRelationshipClass: this.studentRelationshipClassList){
            classStudentGradeList.add(studentRelationshipClass.getStudentGrade());
        }
        return classStudentGradeList;
    }
    //    存储学生课程关系
    private ClassRelationshipCourse ClassRelationshipCourseList;
    public void setCourse(ClassRelationshipCourse ClassRelationshipCourse) {
        this.ClassRelationshipCourseList = ClassRelationshipCourse;
        this.courseName = this.ClassRelationshipCourseList.myGetCourseByClass().myGetCourseName();
    }
    public Course queryCourse() {
        return this.ClassRelationshipCourseList.myGetCourseByClass();
    }
    //    存储教师课程关系
    private ClassRelationshipTeacher ClassRelationshipTeacherList;
    public void setTeacher(ClassRelationshipTeacher classRelationshipTeacher) {
        this.ClassRelationshipTeacherList = classRelationshipTeacher;
        this.teacherId = this.ClassRelationshipTeacherList.myGetTeacherByClass().myGetId();
    }
    public Teacher queryTearcher() {
        return this.ClassRelationshipTeacherList.myGetTeacherByClass();
    }
    public String toString() {
        return "Class{" +
                "teacherId=" + teacherId +
                ", courseName='" + courseName + '\'' +
                ", totalStudents=" + totalStudents +
                ", classCode='" + classCode + '\'' +
                ", semester='" + semester + '\'' +
                '}';
    }

    //    public static void main(String[] args) {
//        Class myClass = new Class(101, "Java Programming", 30, "CS101", "Spring 2024");
//        System.out.println(myClass);
//    }
    public static void main(String[] args) {
        Student student1 = new Student(101, "lee", 25, "male", "Computer Science College", 2000, 1, "Computer Science");
        Student student2 = new Student(102, "wang", 24, "male", "Computer Science College", 2000, 1, "Computer Science");
        MyClass myClass1 = new MyClass(101, "Java Programming", 0, "CS101", "Spring 2024");
        MyClass myClass2 = new MyClass(102, "python Programming", 0, "CS101", "Spring 2024");


        StudentRelationshipClass studentRelationshipClass1 = new StudentRelationshipClass(student1, myClass1);
        StudentRelationshipClass studentRelationshipClass2 = new StudentRelationshipClass(student2, myClass1);
        StudentRelationshipClass studentRelationshipClass3 = new StudentRelationshipClass(student1, myClass2);
        System.out.println(myClass1);
        System.out.println(myClass1.queryStudent());

        Course course1 = new Course("101", "Java Programming");
        ClassRelationshipCourse classRelationshipCourse1 = new ClassRelationshipCourse(myClass1,course1);
        ClassRelationshipCourse classRelationshipCourse2 = new ClassRelationshipCourse(myClass2,course1);
        System.out.println(course1);
        System.out.println(course1.queryClass());
        System.out.println(course1.queryClass().get(0).queryCourse());
    }
}

