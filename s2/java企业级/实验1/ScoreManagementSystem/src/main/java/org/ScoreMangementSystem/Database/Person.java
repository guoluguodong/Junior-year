package org.ScoreMangementSystem.Database;

import org.ScoreMangementSystem.utils.MethodOrder;

public class Person {
    private int id;
    private String name;
    private int age;

    private String gender;
    private String college;

    public Person(int id, String name, int age, String gender, String college) {
        this.id = id;
        this.name = name;
        this.age = age;
        this.gender = gender;
        this.college = college;
    }
    @MethodOrder(1)
    public int myGetId() {
        return this.id;
    }
    @MethodOrder(2)

    public String myGetPersonName() {
        return this.name;
    }
    @MethodOrder(3)
    public int myGetAge() {
        return this.age;
    }
    @MethodOrder(4)
    public String myGetGender() {
        return this.gender;
    }
    @MethodOrder(5)
    public String myGetCollege() {
        return this.college;
    }
    public String toString() {
        return "Person{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", age=" + age +
                ", gender='" + gender + '\'' +
                ", college='" + college + '\'' +
                '}';
    }
}