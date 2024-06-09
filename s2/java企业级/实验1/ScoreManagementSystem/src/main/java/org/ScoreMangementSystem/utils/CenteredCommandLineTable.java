package org.ScoreMangementSystem.utils;

import org.ScoreMangementSystem.Database.MyClass;
import org.ScoreMangementSystem.Database.Student;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

public class CenteredCommandLineTable<T> implements Drawable {
    private int rows;
    private int columns;
    private String[][] data;
    private Class typeClass;

    public CenteredCommandLineTable(ArrayList<T> arrayList, Class typeClass) throws InvocationTargetException, IllegalAccessException {
        this.typeClass = typeClass;
//        System.out.println("泛型参数类型: " + typeClass.getName());
        Method[] methods = typeClass.getMethods();
//      使用了 Lambda 表达式来创建一个匿名的 Comparator 对象
        Arrays.sort(methods, Comparator.comparingInt(method -> {
            MethodOrder annotation = method.getAnnotation(MethodOrder.class);
            return annotation != null ? annotation.value() : Integer.MAX_VALUE; // 判空处理
        }));
        String subString = "myGet";
        int indexI = 1;
        int indexJ = 0;
        this.rows = arrayList.size()+1;
        this.columns = 0;
        for (Method method : methods) {
            if (method.getName().contains(subString)) {
                columns++;
            }
        }
        this.data = new String[rows][columns];
        for (Method method : methods) {
            if (method.getName().contains(subString)) {
                data[0][indexJ] = method.getName().replace(subString, "");
                indexJ++;
            }
        }

        for (T t : arrayList) {
            indexJ = 0;
            for (Method method : methods) {
                if (method.getName().contains(subString)) {
                    Object[] parameters = {};
                    data[indexI][indexJ] = method.invoke(t, parameters).toString();
                    indexJ++;
                }
            }
            indexI++;
        }
    }
    @Override
    public void draw() {
    // Calculate maximum width for each column
    int[] columnWidths = new int[columns];
    for (int j = 0; j < columns; j++) {
        int maxWidth = 0;
        for (int i = 0; i < rows; i++) {
            String cellValue = data[i][j] != null ? data[i][j] : "";
            maxWidth = Math.max(maxWidth, cellValue.length());
        }
        columnWidths[j] = maxWidth + 2; // Add padding for better readability
    }

    // Draw top border
    drawHorizontalLine(columnWidths);

    // Draw table content
    for (int i = 0; i < rows; i++) {
        System.out.print("|");
        for (int j = 0; j < columns; j++) {
            String cellValue = data[i][j] != null ? data[i][j] : "";
            System.out.format("%-" + columnWidths[j] + "s|", centerAlign(cellValue, columnWidths[j]));
        }
        System.out.println();
        drawHorizontalLine(columnWidths);
    }
}

    private void drawHorizontalLine(int[] columnWidths) {
        for (int width : columnWidths) {
            for (int i = 0; i < width; i++) {
                System.out.print("-");
            }
            System.out.print("+"); // Add '+' at the end of each column
        }
        System.out.println(); // Move to the next line after drawing the horizontal line
    }

    private String centerAlign(String text, int width) {
        return String.format("%-" + width + "s", String.format("%" + (width + text.length()) / 2 + "s", text));
    }

    public static void main(String[] args) throws InvocationTargetException, IllegalAccessException {
        Student student1 = new Student(101, "lee", 25, "male", "Computer Science College", 2000, 1, "Computer Science");
        Student student2 = new Student(102, "wang", 25, "male", "Computer Science College", 2000, 1, "Computer Science");

        ArrayList<Student> studentArrayList = new ArrayList<>();
        studentArrayList.add(student1);
        studentArrayList.add(student2);
        studentArrayList.add(student1);
        CenteredCommandLineTable<Student> table1 = new CenteredCommandLineTable<>(studentArrayList, Student.class);
        table1.draw();
        MyClass myClass1 = new MyClass(101, "Java Programming", 0, "CS101", "Spring 2024");
        MyClass myClass2 = new MyClass(102, "python Programming", 0, "CS101", "Spring 2024");
        ArrayList<MyClass> classArrayList = new ArrayList<>();
        classArrayList.add(myClass1);
        classArrayList.add(myClass2);
        classArrayList.add(myClass1);
        CenteredCommandLineTable<MyClass> table2 = new CenteredCommandLineTable<>(classArrayList, MyClass.class);
        table2.draw();
    }
}
