package org.ScoreMangementSystem.Database;

public class ClassRelationshipTeacher {
    private MyClass myclass;
    private Teacher teacher;

    public ClassRelationshipTeacher(MyClass myclass, Teacher teacher) {
        this.teacher = teacher;
        this.myclass = myclass;
        this.teacher.addClass(this);
        this.myclass.setTeacher(this);
    }
    public MyClass myGetClassByTeacher() {
        return this.myclass;
    }
    public Teacher myGetTeacherByClass() {
        return this.teacher;
    }
}
