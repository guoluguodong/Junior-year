package org.ScoreMangementSystem;

import java.lang.reflect.InvocationTargetException;
import java.util.*;
import java.util.stream.Collectors;

import org.ScoreMangementSystem.Database.*;
import org.ScoreMangementSystem.utils.CenteredCommandLineTable;
import org.fusesource.jansi.Ansi;
import org.fusesource.jansi.AnsiConsole;
// 界面类
public class CommandLineMenu {
    public static void showCommandLineMenu() throws InvocationTargetException, IllegalAccessException {
        Scanner scanner = new Scanner(System.in);
        int choice;
        System.out.println("==== 基于命令行的学生成绩管理系统 ====");
        System.out.println("请输入教师数目:");
        int teacherNum = scanner.nextInt();
        scanner.nextLine(); // 消耗掉输入的换行符
        System.out.println("请输入学生数目:");
        int studentNum = scanner.nextInt();
        scanner.nextLine(); // 消耗掉输入的换行符
        System.out.println("====正在模拟生成学生成绩====");
        ProcessSimulation processSimulation = new ProcessSimulation();
        processSimulation.init(teacherNum, studentNum);
        do {
            System.out.println("======== 主菜单 ========");
            System.out.println("1. 选项一: 查询所有课程");
            System.out.println("2. 选项二: 查询所有教学班");
            System.out.println("3. 选项三：查询一个教学班并显示该班级所有学生、成绩、排名 ");
            System.out.println("4. 选项四：查询所有学生");
            System.out.println("5. 选项五：学号查询学生的所有科目的成绩、总成绩、排名");
            System.out.println("6. 选项六：查询所有教师");
            System.out.println("7. 选项七：给班级学生赋分");
            System.out.println("8. 退出");
            System.out.print("请选择操作：");

            // 读取用户输入的选项
            choice = scanner.nextInt();
            scanner.nextLine(); // 消耗掉输入的换行符

            // 根据用户的选择执行相应的操作
            switch (choice) {
                case 1:
                    System.out.println("你选择了选项一。");
                    CenteredCommandLineTable<Course> table3 = new CenteredCommandLineTable<>(processSimulation.courses, Course.class);
                    table3.draw();
                    break;
                case 2:
                    System.out.println("你选择了选项二。");

                    CenteredCommandLineTable<MyClass> table4 = new CenteredCommandLineTable<>(processSimulation.myClasses, MyClass.class);
                    table4.draw();
                    break;
                case 3:
                    System.out.println("你选择了选项三。");
                    System.out.println("请输入ClassCode, 如果记不清可以返回组菜单查询（输入0返回）");
                    String ClassCode = scanner.next();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    if (ClassCode.equals("0")) break;
                    if (!processSimulation.classCodeMap.containsKey(ClassCode)) {
                        System.out.println(ClassCode);
                        System.out.println(processSimulation.classCodeMap.toString());
                        break;
                    }
                    ArrayList<StudentRelationshipClass.StudentGrade> classStudentGradeList = processSimulation.classCodeMap.get(ClassCode).queryScoreByClass();
                    System.out.println("1. 选项一: 根据学号排序");
                    System.out.println("2. 选项二: 根据成绩排序");
                    System.out.println("3. 选项三: 班级成绩的分数段分布");
                    List<StudentRelationshipClass.StudentGrade> classStudentGradeListSorted;
                    ArrayList<StudentRelationshipClass.StudentGrade> sortedClassStudentGradeList;
                    // 读取用户输入的选项
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            classStudentGradeListSorted = classStudentGradeList.stream()
                                    .sorted(Comparator.comparingDouble(StudentRelationshipClass.StudentGrade::myGetComprehensiveGrade).reversed())
                                    .toList();
                            for(int i=1;i<=classStudentGradeListSorted.size();++i){
                                classStudentGradeListSorted.get(i-1).setRank(i);
                            }
                            classStudentGradeListSorted = classStudentGradeList.stream()
                                    .sorted(Comparator.comparingInt(StudentRelationshipClass.StudentGrade::myGetStudentId))
                                    .toList();

                            sortedClassStudentGradeList = new ArrayList<>(classStudentGradeListSorted);
                            CenteredCommandLineTable<StudentRelationshipClass.StudentGrade> table8 = new CenteredCommandLineTable<>(sortedClassStudentGradeList, StudentRelationshipClass.StudentGrade.class);
                            table8.draw();
                            break;
                        case 2:
                            classStudentGradeListSorted = classStudentGradeList.stream()
                                    .sorted(Comparator.comparingDouble(StudentRelationshipClass.StudentGrade::myGetComprehensiveGrade).reversed())
                                    .toList();
                            for(int i=1;i<=classStudentGradeListSorted.size();++i){
                                classStudentGradeListSorted.get(i-1).setRank(i);
                            }

                            sortedClassStudentGradeList = new ArrayList<>(classStudentGradeListSorted);
                            CenteredCommandLineTable<StudentRelationshipClass.StudentGrade> table9 = new CenteredCommandLineTable<>(sortedClassStudentGradeList, StudentRelationshipClass.StudentGrade.class);
                            table9.draw();
                            break;
                        case 3:
                            Map<String, Long> scoreCounts = classStudentGradeList.stream()
                                   .collect(Collectors.groupingBy(
                                           studentGrade -> {
                                                double score = studentGrade.myGetComprehensiveGrade();
                                               if (score >= 90) return "A-90分以上";
                                               else if (score >= 80) return "B-80分以上";
                                               else if (score >= 70) return "C-70分以上";
                                               else return "D-70分以下";
                                           },
                                           Collectors.counting()
                                   ));
                            scoreCounts.forEach((grade, count) -> {
                                Ansi.Color color = switch (grade) {
                                    case "A-90分以上" -> Ansi.Color.GREEN;
                                    case "B-80分以上" -> Ansi.Color.BLUE;
                                    case "C-70分以上" -> Ansi.Color.YELLOW;
                                    case "D-70分以下" -> Ansi.Color.RED;
                                    default -> Ansi.Color.DEFAULT;
                                };

                                Ansi ansi = Ansi.ansi();
                                ansi.fg(color).a(grade + ": ");

                                for (int i = 0; i < count; i++) {
                                    ansi.a('█'); // 输出方块字符
                                }

                                System.out.println(ansi.reset());
                            });

                            AnsiConsole.systemUninstall(); // 卸载 AnsiConsole
                            break;
                        default:
                            System.out.println("无效的选项，请重新选择。");
                            break;
                    }
                    break;
                case 4:
                    System.out.println("你选择了选项四");
                    ArrayList<Student> studentsList = processSimulation.students;
                    System.out.println("1. 选项一: 根据学号排序");
                    System.out.println("2. 选项二: 根据成绩排序");
                    System.out.println("3. 选项三: 总成绩的分数段分布");
                    List<Student> studentsListSorted;
                    ArrayList<Student> sortedStudentsList;
                    // 读取用户输入的选项
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            // 先获得排名
                            studentsListSorted = studentsList.stream()
                                    .sorted(Comparator.comparingDouble(Student::myGetAverageScore).reversed())
                                    .toList();
                            for(int i=1;i<=studentsListSorted.size();++i){
                                studentsListSorted.get(i-1).setRank(i);
                            }
                            studentsListSorted = studentsList.stream()
                                    .sorted(Comparator.comparingInt(Student::myGetStudentId))
                                    .toList();

                            sortedStudentsList = new ArrayList<>(studentsListSorted);
                            CenteredCommandLineTable<Student> table8 = new CenteredCommandLineTable<>(sortedStudentsList, Student.class);
                            table8.draw();
                            break;
                        case 2:

                            studentsListSorted = studentsList.stream()
                                    .sorted(Comparator.comparingDouble(Student::myGetAverageScore).reversed())
                                    .toList();
                            for(int i=1;i<=studentsListSorted.size();++i){
                                studentsListSorted.get(i-1).setRank(i);
                            }
                            sortedStudentsList = new ArrayList<>(studentsListSorted);
                            CenteredCommandLineTable<Student> table9 = new CenteredCommandLineTable<>(sortedStudentsList, Student.class);
                            table9.draw();
                            break;
                        case 3:
                            Map<String, Long> scoreCounts = studentsList.stream()
                                    .collect(Collectors.groupingBy(
                                            student -> {
                                                double score = student.myGetAverageScore();
                                                if (score >= 90) return "A-90分以上";
                                                else if (score >= 80) return "B-80分以上";
                                                else if (score >= 70) return "C-70分以上";
                                                else return "D-70分以下";
                                            },
                                            Collectors.counting()
                                    ));
                            scoreCounts.forEach((grade, count) -> {
                                Ansi.Color color = switch (grade) {
                                    case "A-90分以上" -> Ansi.Color.GREEN;
                                    case "B-80分以上" -> Ansi.Color.BLUE;
                                    case "C-70分以上" -> Ansi.Color.YELLOW;
                                    case "D-70分以下" -> Ansi.Color.RED;
                                    default -> Ansi.Color.DEFAULT;
                                };

                                Ansi ansi = Ansi.ansi();
                                ansi.fg(color).a(grade + ": "+count+" 人 ");

                                for (int i = 0; i < count; i++) {
                                    ansi.a('█'); // 输出方块字符
                                }

                                System.out.println(ansi.reset());
                            });
                            AnsiConsole.systemUninstall(); // 卸载 AnsiConsole
                            break;
                    }
                    break;
                case 5:
                    System.out.println("你选择了选项五");
                    System.out.println("请输入StudentId, 如果记不清可以返回组菜单查询（输入0返回）");
                    int studentid = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    if (studentid == 0) break;
                    if (!processSimulation.studentIdMap.containsKey(studentid)) {
                        System.out.println(studentid);
                        System.out.println(processSimulation.classCodeMap.toString());
                        break;
                    }
//                    ToDo
//                    需要补充从学生类返回ArrayList<Classes> = classStudentGradeList,对于每门课都要排序
                    ArrayList<StudentRelationshipClass.StudentGrade> studentGradeList = processSimulation.studentIdMap.get(studentid).queryScoreByStudentId();
                    for(MyClass myClass: processSimulation.studentIdMap.get(studentid).queryClass()){
                        classStudentGradeListSorted = processSimulation.classCodeMap.get(myClass.myGetClassCode()).queryScoreByClass().stream()
                                .sorted(Comparator.comparingDouble(StudentRelationshipClass.StudentGrade::myGetComprehensiveGrade).reversed())
                                .toList();
                        for(int i=1;i<=classStudentGradeListSorted.size();++i){
                            classStudentGradeListSorted.get(i-1).setRank(i);
                        }
                    }
                    List<StudentRelationshipClass.StudentGrade> studentGradeListSorted;
                    ArrayList<StudentRelationshipClass.StudentGrade> sortedStudentGradeList;
                    studentGradeListSorted = studentGradeList.stream()
                            .sorted(Comparator.comparingDouble(StudentRelationshipClass.StudentGrade::myGetComprehensiveGrade))
                            .toList();
                    sortedStudentGradeList = new ArrayList<>(studentGradeListSorted);
                    CenteredCommandLineTable<StudentRelationshipClass.StudentGrade> table8 = new CenteredCommandLineTable<>(sortedStudentGradeList, StudentRelationshipClass.StudentGrade.class);
                    table8.draw();
                    double aveg = processSimulation.studentIdMap.get(studentid).myGetAverageScore();
                    System.out.println("================================================================================================  总成绩: "+String.format("%.2f", aveg)+" ,   已修 "+sortedStudentGradeList.size()+" 门课");
                    break;
                case 6:
                    System.out.println("你选择了选项六。");
                    CenteredCommandLineTable<Teacher> table11 = new CenteredCommandLineTable<>(processSimulation.teachers, Teacher.class);
                    table11.draw();
                    break;
                case 7:
                    System.out.println("你选择了选项七。");
                    System.out.println("请输入ClassCode, 如果记不清可以返回组菜单查询（输入0返回）");
                    ClassCode = scanner.next();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    if (ClassCode.equals("0")) break;
                    if (!processSimulation.classCodeMap.containsKey(ClassCode)) {
                        System.out.println(ClassCode);
                        System.out.println(processSimulation.classCodeMap.toString());
                        break;
                    }
                    System.out.println("请输入StudentId, 如果记不清可以返回组菜单查询（输入0返回）");
                    studentid = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    if (studentid == 0) break;
                    if (!processSimulation.studentIdMap.containsKey(studentid)) {
                        System.out.println(studentid);
                        System.out.println(processSimulation.classCodeMap.toString());
                        break;
                    }
                    classStudentGradeListSorted = processSimulation.classCodeMap.get(ClassCode).queryScoreByClass().stream()
                            .sorted(Comparator.comparingDouble(StudentRelationshipClass.StudentGrade::myGetComprehensiveGrade).reversed())
                            .toList();
                    for(int i=1;i<=classStudentGradeListSorted.size();++i){
                        classStudentGradeListSorted.get(i-1).setRank(i);
                    }
                    System.out.println("当前学生成绩：");
                    ArrayList<StudentRelationshipClass.StudentGrade> selectedGrade = new ArrayList<>();
                    for(StudentRelationshipClass.StudentGrade studentGrade:processSimulation.classCodeMap.get(ClassCode).queryScoreByClass()){
                        if (studentGrade.myGetStudentId()==studentid){
                            selectedGrade.add(studentGrade);
                            break;
                        }
                    }
                    CenteredCommandLineTable<StudentRelationshipClass.StudentGrade> table12 = new CenteredCommandLineTable<>(selectedGrade, StudentRelationshipClass.StudentGrade.class);
                    table12.draw();
                    double score;
                    do {
                        System.out.println("输入1~4指定待修改的成绩");
                        System.out.println("1. 修改 AttendanceGrade");
                        System.out.println("2. 修改 MidTermGrade");
                        System.out.println("3. 修改 LabGrade");
                        System.out.println("4. 修改 FinalGrade");
                        System.out.println("5. 完成");
                        choice = scanner.nextInt();
                        scanner.nextLine(); // 消耗掉输入的换行符
                        if(choice==5){
                            break;
                        }
                        System.out.println("请输入学生成绩");
                        score = scanner.nextDouble();
                        scanner.nextLine(); // 消耗掉输入的换行符
                        selectedGrade.get(0).recordGrades(choice,score);
                        System.out.println("当前学生成绩：");
                        selectedGrade.clear();
                        classStudentGradeListSorted = processSimulation.classCodeMap.get(ClassCode).queryScoreByClass().stream()
                                .sorted(Comparator.comparingDouble(StudentRelationshipClass.StudentGrade::myGetComprehensiveGrade).reversed())
                                .toList();
                        for(int i=1;i<=classStudentGradeListSorted.size();++i){
                            classStudentGradeListSorted.get(i-1).setRank(i);
                        }
                        for(StudentRelationshipClass.StudentGrade studentGrade:processSimulation.classCodeMap.get(ClassCode).queryScoreByClass()){
                            if (studentGrade.myGetStudentId()==studentid){
                                selectedGrade.add(studentGrade);
                                break;
                            }
                        }
                        CenteredCommandLineTable<StudentRelationshipClass.StudentGrade> table13 = new CenteredCommandLineTable<>(selectedGrade, StudentRelationshipClass.StudentGrade.class);
                        table13.draw();
                    } while (true);
                    break;
                case 8:
                    System.out.println("感谢使用，再见！");
                    break;
                default:
                    System.out.println("无效的选项，请重新选择。");
                    break;
            }
        } while (choice != 8);

        scanner.close();
    }
}
