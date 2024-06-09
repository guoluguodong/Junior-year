package org.ScoreMangementSystem.Database;

import org.ScoreMangementSystem.Database.MyClass;
import org.ScoreMangementSystem.Database.Student;
import org.ScoreMangementSystem.utils.MethodOrder;

import java.time.LocalDate;

public class StudentRelationshipClass {
    private Student student;
    private MyClass myclass;

    private StudentGrade studentGrade;

    public StudentRelationshipClass(Student student, MyClass myclass) {
        this.student = student;
        this.myclass = myclass;
        this.student.addClass(this);
        this.myclass.addStudent(this);
        this.studentGrade = new StudentGrade(student.myGetStudentId(),student.myGetPersonName(),myclass.myGetCourseName(),0,0,0,0);
    }

    public Student myGetStudentByClass() {
        return this.student;
    }

    public MyClass myGetClassByStudent() {
        return this.myclass;
    }

    public StudentGrade getStudentGrade() {
        return this.studentGrade;
    }
//    学生成绩作为内部类
    public class StudentGrade {
        private int classRank;
        private int studentId;

        private String studentName;

        private String courseName;
        private double attendanceGrade=0;
        private double midTermGrade;
        private double labGrade;
        private double finalGrade;
        private double comprehensiveGrade;


        private LocalDate attendanceGradRecordedDate;
        private LocalDate midTermGradeRecordedDate;
        private LocalDate labGradeRecordedDate;
        private LocalDate finalGradeRecordedDate;
        private LocalDate comprehensiveGradeRecordedDate;
        public StudentGrade(int studentId, String studentName, String courseName,double attendanceGrade,double midTermGrade,double labGrade,double finalGrade){
            this.studentId = studentId;
            this.studentName =studentName;
            this.courseName = courseName;
            this.attendanceGrade = attendanceGrade; attendanceGradRecordedDate=LocalDate.now();
            this.midTermGrade = midTermGrade; midTermGradeRecordedDate=LocalDate.now();
            this.labGrade = labGrade; labGradeRecordedDate=LocalDate.now();
            this.finalGrade = finalGrade; finalGradeRecordedDate=LocalDate.now();
            this.comprehensiveGrade = this.calculateCompositeGrade();
            this.comprehensiveGradeRecordedDate = LocalDate.now();
        }
        public void recordGrades(int category, double grade) {
            switch (category){
                case 1: this.attendanceGrade = grade; attendanceGradRecordedDate=LocalDate.now();break;
                case 2: this.midTermGrade = grade; midTermGradeRecordedDate=LocalDate.now();break;
                case 3: this.labGrade = grade; labGradeRecordedDate=LocalDate.now();break;
                case 4: this.finalGrade = grade; finalGradeRecordedDate=LocalDate.now();break;
            }
            this.comprehensiveGrade = this.calculateCompositeGrade();
            this.comprehensiveGradeRecordedDate = LocalDate.now();
        }
        public void setRank(int rank) {
        this.classRank = rank;
    }
        public double calculateCompositeGrade() {
            return (attendanceGrade + midTermGrade + labGrade + finalGrade) / 4.0;
        }
        @MethodOrder(0)
        public int myGetClassRank(){
                return this.classRank;
        }
        // 获取学生ID
        @MethodOrder(1)
        public int myGetStudentId() {
            return this.studentId;
        }

        @MethodOrder(2)
        public String myGetStudentName() {
            return this.studentName;
        }
        @MethodOrder(3)
        public String myGetClassName() {
            return this.courseName;
        }
        // 获取出勤成绩
        @MethodOrder(4)
        public double myGetAttendanceGrade() {
            return this.attendanceGrade;
        }
        // 获取期中成绩
        @MethodOrder(5)
        public double myGetMidTermGrade() {
            return this.midTermGrade;
        }

        // 获取实验成绩
        @MethodOrder(6)
        public double myGetLabGrade() {
            return this.labGrade;
        }

        // 获取期末成绩
        @MethodOrder(7)
        public double myGetFinalGrade() {
            return this.finalGrade;
        }
        @MethodOrder(8)
        public double myGetComprehensiveGrade() {
            return this.comprehensiveGrade;
        }

        // 获取出勤成绩记录日期
        public LocalDate getAttendanceGradRecordedDate() {
            return this.attendanceGradRecordedDate;
        }

        // 获取期中成绩记录日期
        public LocalDate getMidTermGradeRecordedDate() {
            return this.midTermGradeRecordedDate;
        }

        // 获取实验成绩记录日期
        public LocalDate getLabGradeRecordedDate() {
            return this.labGradeRecordedDate;
        }

        // 获取期末成绩记录日期
        public LocalDate getFinalGradRecordedDate() {
            return this.finalGradeRecordedDate;
        }
        @MethodOrder(9)
        public LocalDate myGetComprehensiveGradeRecordedDate() {
            return this.comprehensiveGradeRecordedDate;
        }
    }

}

//package org.example;
//
//import java.time.LocalDate;
//import java.util.HashMap;
//import java.util.Map;
//public class StudentRelationshipClass {
//    private Student student;
//    private MyClass myclass;
////    第三步，获得平时成绩，期中成绩，实验成绩，期末成绩，最后计算综合成绩，要记录成绩取得的时间。
//    private Map<String, Map<Integer, LocalDate>> score = new HashMap<>();
//    public StudentRelationshipClass(Student student, MyClass myclass) {
//        this.student = student;
//        this.myclass = myclass;
//        this.student.addClass(this);
//        this.myclass.addStudent(this);
//        this.score.put("期中成绩", new HashMap<>());
//        this.score.put("实验成绩", new HashMap<>());
//        this.score.put("期末成绩", new HashMap<>());
//        this.score.put("综合成绩", new HashMap<>());
//        this.score.get("期中成绩").put(0, LocalDate.now());
//        this.score.get("实验成绩").put(0, LocalDate.now());
//        this.score.get("期末成绩").put(0, LocalDate.now());
//        this.score.get("综合成绩").put(0, LocalDate.now());
//    }
//
//    public Student myGetStudentByClass() {
//        return this.student;
//    }
//
//    public MyClass myGetClassByStudent() {
//        return this.myclass;
//    }
//
//    public void updateScore(String category, int newScore, LocalDate newDate) {
//        if(this.score.containsKey(category)){
//            this.score.get(category).put(newScore, newDate);
//        }
//    }
//    public Map<String, Map<Integer, LocalDate>> getScore() {
//        return this.score;
//    }
//}
