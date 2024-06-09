package org.ScoreMangementSystem.Database;

public class ClassRelationshipCourse {
    private MyClass myclass;
    private Course course;

    public ClassRelationshipCourse(MyClass myclass, Course course) {
        this.course = course;
        this.myclass = myclass;
        this.course.addClass(this);
        this.myclass.setCourse(this);
    }
    public MyClass myGetClassByCourse() {
        return this.myclass;
    }
    public Course myGetCourseByClass() {
        return this.course;
    }
}
