package org.ScoreMangementSystem.Database;

import java.util.ArrayList;
import java.util.List;

public class Course {
    private String courseId;
    private String courseName;

    public Course(String courseId, String courseName) {
        this.courseId = courseId;
        this.courseName = courseName;
    }

    public String myGetCourseId() {
        return courseId;
    }
    //    存储班级课程关系
    private List<ClassRelationshipCourse> classRelationshipCourse = new ArrayList<>();
    public void addClass(ClassRelationshipCourse ClassRelationshipCourse) {
        this.classRelationshipCourse.add(ClassRelationshipCourse);
    }
    public List<MyClass> queryClass() {
        List<MyClass> myclasses = new ArrayList<>();
        for (ClassRelationshipCourse ClassRelationshipCourse : this.classRelationshipCourse) {
            myclasses.add(ClassRelationshipCourse.myGetClassByCourse());
        }
        return myclasses;
    }

    public String myGetCourseName() {
        return courseName;
    }
    public String toString() {
        return "Course{" +
                "courseId=" + courseId +
                ", courseName='" + courseName + '\'' +
                '}';
    }



//    public static void main(String[] args) {
//        Course course = new Course(101, "Java Programming");
//        System.out.println(course);
//    }
}

