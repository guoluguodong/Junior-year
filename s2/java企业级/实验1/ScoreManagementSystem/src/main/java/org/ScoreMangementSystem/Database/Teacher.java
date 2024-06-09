package org.ScoreMangementSystem.Database;

import org.ScoreMangementSystem.Database.ClassRelationshipTeacher;
import org.ScoreMangementSystem.Database.Course;
import org.ScoreMangementSystem.Database.MyClass;
import org.ScoreMangementSystem.Database.Person;

import java.util.ArrayList;
import java.util.List;

public class Teacher extends Person {
    private int teacherId;
    private List<Course> courses;
    public Teacher(int id, String name, int age, String gender, String college, int teacherId) {
        super(id, name, age, gender,college);
        this.teacherId = teacherId;
    }
    public int myGetTeacherId() {
        return this.teacherId;
    }
//    存储教师和教学班的关联
    private ArrayList<ClassRelationshipTeacher> classRelationshipTeacherList = new ArrayList<>();
    public void addClass(ClassRelationshipTeacher classRelationshipTeacher) {
        this.classRelationshipTeacherList.add(classRelationshipTeacher);
    }
    public List<MyClass> queryClass() {
        List<MyClass> myClassList = new ArrayList<>();
        for(ClassRelationshipTeacher classRelationshipTeacher :this.classRelationshipTeacherList) {
            myClassList.add(classRelationshipTeacher.myGetClassByTeacher());
        }
        return myClassList;
    }
    public String toString() {
        return "Teacher{" +
                "id=" + super.myGetId() +
                ", name='" + super.myGetPersonName() + '\'' +
                ", age=" + super.myGetAge() +
                ", gender='" + super.myGetGender() + '\'' +
                ", college='" + super.myGetCollege() + '\'' +
                ", teacherId=" + teacherId +
                '}';
    }
//    public static void main(String[] args) {
//        Teacher teacher = new Teacher(101, "lee", 25, "male", "Computer Science", 2000);
//        System.out.println(teacher.getName());
//    }
}

