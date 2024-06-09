package org.ScoreMangementSystem.Database;
import org.ScoreMangementSystem.utils.MethodOrder;

import java.util.ArrayList;
import java.util.List;
public class Student extends Person {

    private int studentId;
    private int grade;
    private String major;

    private double averageScore;
    private int rank;
    private List<StudentRelationshipClass> studentRelationshipClassList = new ArrayList<>();

    public Student(int id, String name, int age, String gender, String college, int studentId,int grade, String major) {
        super(id, name, age, gender,college);
        this.grade = grade;
        this.major = major;
        this.studentId = studentId;
    }
    @MethodOrder(8)
    public int myGetStudentId() {
        return this.studentId;
    }
    @MethodOrder(7)
    public int myGetGrade() {
        return this.grade;
    }
    @MethodOrder(6)
    public String myGetMajor() {
        return this.major;
    }
    @MethodOrder(9)
    public double myGetAverageScore() {
//        先更新
        this.averageScore = 0;
        ArrayList<StudentRelationshipClass.StudentGrade> studentGradeList = new ArrayList<>();
        for(StudentRelationshipClass studentRelationshipClass: this.studentRelationshipClassList){
            this.averageScore = this.averageScore +  studentRelationshipClass.getStudentGrade().myGetComprehensiveGrade();
        }
        return this.averageScore/this.studentRelationshipClassList.size();
    }
    public void addClass(StudentRelationshipClass studentRelationshipClass) {
        this.studentRelationshipClassList.add(studentRelationshipClass);
    }
    public void setRank(int rank) {
        this.rank = rank;
    }
    @MethodOrder(10)
    public int myGetRank() {
        return this.rank;
    }
    public List<MyClass> queryClass() {
        List<MyClass> myClassListlass = new ArrayList<>();
        for(StudentRelationshipClass studentRelationshipClass :this.studentRelationshipClassList) {
            myClassListlass.add(studentRelationshipClass.myGetClassByStudent());
        }
        return myClassListlass;
    }
    public ArrayList<StudentRelationshipClass.StudentGrade> queryScoreByStudentId() {
        ArrayList<StudentRelationshipClass.StudentGrade> studentGradeList = new ArrayList<>();
        for(StudentRelationshipClass studentRelationshipClass: this.studentRelationshipClassList){
            studentGradeList.add(studentRelationshipClass.getStudentGrade());
        }
        return studentGradeList;
    }
    @Override
    public String toString() {
        return "Student{" +
                "id=" + super.myGetId() +
                ", name='" + super.myGetPersonName() + '\'' +
                ", age=" + super.myGetAge() +
                ", gender='" + super.myGetGender() + '\'' +
                ", college='" + super.myGetCollege() + '\'' +
                ", studentId=" + studentId +
                ", grade=" + grade +
                ", major='" + major + '\'' +
                '}';
    }
    public static void main(String[] args) {
        Student student1 = new Student(101, "lee", 25, "male", "Computer Science College", 2000, 1,"Computer Science");
        MyClass myClass1 = new MyClass(101, "Java Programming", 30, "CS101", "Spring 2024");
        MyClass myClass2 = new MyClass(102, "python Programming", 30, "CS101", "Spring 2024");
        StudentRelationshipClass studentRelationshipClass1 = new StudentRelationshipClass(student1, myClass1);
        StudentRelationshipClass studentRelationshipClass2 = new StudentRelationshipClass(student1, myClass2);
        System.out.println(student1);
        System.out.println(student1.queryClass());
    }
}
