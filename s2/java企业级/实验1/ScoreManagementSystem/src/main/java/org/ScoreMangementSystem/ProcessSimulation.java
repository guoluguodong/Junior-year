package org.ScoreMangementSystem;

import org.ScoreMangementSystem.Database.*;

import java.lang.reflect.InvocationTargetException;
import java.util.*;

public class ProcessSimulation {
    public ArrayList<Teacher> teachers;
    public ArrayList<Course> courses;
    public ArrayList<MyClass> myClasses;

    public Map<String,MyClass> classCodeMap = new HashMap<>();
    public Map<Integer, Student> studentIdMap = new HashMap<>();
    public ArrayList<Student> students;
    public void init(int teacherNum, int studentNum) throws InvocationTargetException, IllegalAccessException {
        this.teachers = generateTeachers(teacherNum, 0); // 生成5个老师
        System.out.println("已生成" + teachers.size() + "位教师");
//        CenteredCommandLineTable<Teacher> table2 = new CenteredCommandLineTable<>(teachers, Teacher.class);
//        table2.draw();
        this.courses = generateCourses(); // 生成5个课程
        System.out.println("已生成" + courses.size() + "门课程");
//        CenteredCommandLineTable<Course> table3 = new CenteredCommandLineTable<>(courses, Course.class);
//        table3.draw();
        this.myClasses = generateClass(2, courses, teachers); // 生成5*2个班级，关联课程和班级,关联教师和班级
        System.out.println("已生成 " + myClasses.size() + " 个班级");
//        CenteredCommandLineTable<MyClass> table4 = new CenteredCommandLineTable<>(myClasses, MyClass.class);
//        table4.draw();
//        System.out.println(myClasses.get(0).queryCourse());
//        System.out.println(myClasses.get(0).queryTearcher());
        this.students = generateStudents(studentNum, teachers.size(), myClasses); // 生成100个学生对象，并帮他们选课
        System.out.println("已生成 " + students.size() + " 位学生，并随机为他们选 3 门课程");
//        CenteredCommandLineTable<Student> table5 = new CenteredCommandLineTable<>(students, Student.class);
//        table5.draw();
//        System.out.println(students.get(0).queryClass());
//        CenteredCommandLineTable<MyClass> table6 = new CenteredCommandLineTable<>(myClasses, MyClass.class);
//        table6.draw();
//        ArrayList<Student> classStudent = myClasses.get(0).queryStudent();
//        CenteredCommandLineTable<Student> table7 = new CenteredCommandLineTable<>(classStudent, Student.class);
//        table7.draw();
//        遍历班级，遍历所有学生选课关系，初始化成绩
        generateScores(myClasses);
        System.out.println("已生成每位学生的每一门课程生成成绩");
//        this.classStudentGradeList = myClasses.get(0).queryScoreByClass();
//        CenteredCommandLineTable<StudentRelationshipClass.StudentGrade> table8 = new CenteredCommandLineTable<>(classStudentGradeList, StudentRelationshipClass.StudentGrade.class);
//        table8.draw();
//        找到班级，找到关系，修改成绩
//        打印成绩表格
    }

    public  ArrayList<Student> generateStudents(int count, int startId, ArrayList<MyClass> myClasses) {
        ArrayList<Student> students = new ArrayList<>();
        Random random = new Random();

        String[] names = {"Alice", "Bob", "Charlie", "David", "Emma", "Frank", "Grace", "Henry", "Ivy", "Jack"};
        String[] genders = {"Male", "Female"};
        String[] colleges = {"Harvard", "MIT", "Stanford", "Yale", "Princeton", "Caltech", "Oxford", "Cambridge"};
        String[] majors = {"Computer Science", "Engineering", "Mathematics", "Biology", "Physics", "Economics"};
        ArrayList<ArrayList<Integer>> selectClass = new ArrayList<>(
                Arrays.asList(
                        new ArrayList<>(Arrays.asList(0, 2, 4)),
                        new ArrayList<>(Arrays.asList(0, 2, 6)),
                        new ArrayList<>(Arrays.asList(0, 2, 8)),
                        new ArrayList<>(Arrays.asList(0, 4, 6)),
                        new ArrayList<>(Arrays.asList(0, 4, 8)),
                        new ArrayList<>(Arrays.asList(0, 6, 8)),
                        new ArrayList<>(Arrays.asList(2, 4, 6)),
                        new ArrayList<>(Arrays.asList(2, 4, 8)),
                        new ArrayList<>(Arrays.asList(2, 6, 8)),
                        new ArrayList<>(Arrays.asList(4, 6, 8)),
                        new ArrayList<>(Arrays.asList(1, 3, 5)),
                        new ArrayList<>(Arrays.asList(1, 3, 7)),
                        new ArrayList<>(Arrays.asList(1, 3, 9)),
                        new ArrayList<>(Arrays.asList(1, 4, 7)),
                        new ArrayList<>(Arrays.asList(1, 4, 9)),
                        new ArrayList<>(Arrays.asList(1, 7, 9)),
                        new ArrayList<>(Arrays.asList(3, 5, 7)),
                        new ArrayList<>(Arrays.asList(3, 5, 9)),
                        new ArrayList<>(Arrays.asList(3, 7, 9)),
                        new ArrayList<>(Arrays.asList(5, 7, 9))
                )
        );

        for (int i = 0; i < count; i++) {
            int id = i + 1 + startId; // 生成学生的ID，从1开始递增
            String name = names[random.nextInt(names.length)];
            int age = random.nextInt(5) + 18; // 年龄范围在18到27之间
            String gender = genders[random.nextInt(genders.length)];
            String college = colleges[random.nextInt(colleges.length)];
            int studentId = id + 1000; // 学生ID范围在1000到1999之间
            int grade = random.nextInt(5) + 1; // 年级范围在1到5之间
            String major = majors[random.nextInt(majors.length)];

            Student student = new Student(id, name, age, gender, college, studentId, grade, major);
            int ranCourseIndex = random.nextInt(selectClass.size());
            new StudentRelationshipClass(student, myClasses.get(selectClass.get(ranCourseIndex).get(0)));
            new StudentRelationshipClass(student, myClasses.get(selectClass.get(ranCourseIndex).get(1)));
            new StudentRelationshipClass(student, myClasses.get(selectClass.get(ranCourseIndex).get(2)));
            students.add(student);
            studentIdMap.put(studentId,student);
        }

        return students;
    }

    public  ArrayList<Teacher> generateTeachers(int count, int startId) {
        ArrayList<Teacher> teachers = new ArrayList<>();
        Random random = new Random();

        String[] names = {"Alice", "Bob", "Charlie", "David", "Emma", "Frank", "Grace", "Henry", "Ivy", "Jack"};
        String[] genders = {"Male", "Female"};
        String[] colleges = {"Harvard", "MIT", "Stanford", "Yale", "Princeton", "Caltech", "Oxford", "Cambridge"};

        for (int i = 0; i < count; i++) {
            int id = i + startId + 1; // 生成教师的ID，从1开始递增
            String name = names[random.nextInt(names.length)];
            int age = random.nextInt(15) + 25; // 年龄范围在25到39之间
            String gender = genders[random.nextInt(genders.length)];
            String college = colleges[random.nextInt(colleges.length)];
            int teacherId = id + 2000; // 教师ID范围在2000到2999之间

            Teacher teacher = new Teacher(id, name, age, gender, college, teacherId);
            teachers.add(teacher);
        }

        return teachers;
    }

    public  ArrayList<Course> generateCourses() {
        ArrayList<Course> courses = new ArrayList<>();
        Random random = new Random();
        String[] courseNames = {"Mathematics", "Computer Science", "Physics", "Biology", "Chemistry"};
        String[] classCodes = {"MAT", "CS", "PHY", "BIO", "CHE"};
        for (int courseIndex = 0; courseIndex < courseNames.length; courseIndex++) {
            String courseName = courseNames[courseIndex];
            String courseId = classCodes[courseIndex] + "00" + courseIndex + 1;
            Course course = new Course(courseId, courseName);
            courses.add(course);
        }
        return courses;
    }

    public ArrayList<MyClass> generateClass(int count, ArrayList<Course> courses, ArrayList<Teacher> teachers) {
        ArrayList<MyClass> myClasses = new ArrayList<>();
        Random random = new Random();
        String[] courseNames = {"Mathematics", "Computer Science", "Physics", "Biology", "Chemistry"};
        String[] semesters = {"Spring", "Summer", "Fall"};
        String[] classCodes = {"993221-", "193200-", "243213-", "286215-", "373255-"};
        for (int courseIndex = 0; courseIndex < courseNames.length; courseIndex++) {
            for (int i = 1; i <= count; i++) {
                String courseName = courseNames[courseIndex];
                String classCode = classCodes[courseIndex] + "00" + i;
                String semester = semesters[random.nextInt(semesters.length)];
                MyClass myClass = new MyClass(0, courseName, 0, classCode, semester);
                new ClassRelationshipCourse(myClass, courses.get(courseIndex));
                new ClassRelationshipTeacher(myClass, teachers.get(random.nextInt(teachers.size())));
                myClasses.add(myClass);
                this.classCodeMap.put(classCode,myClass);
            }
        }
        return myClasses;
    }

    public  void generateScores(ArrayList<MyClass> myClasses) {
        Random random = new Random();
        for(MyClass myClass: myClasses){
            for(StudentRelationshipClass studentRelationshipClass: myClass.getStudentRelationshipClassList()){
                studentRelationshipClass.getStudentGrade().recordGrades(1,80 + random.nextInt(20));
                studentRelationshipClass.getStudentGrade().recordGrades(2,90 + random.nextInt(10));
                studentRelationshipClass.getStudentGrade().recordGrades(3,60 + random.nextInt(40));
                studentRelationshipClass.getStudentGrade().recordGrades(4,30 + random.nextInt(70));
            }
        }
    }
}
