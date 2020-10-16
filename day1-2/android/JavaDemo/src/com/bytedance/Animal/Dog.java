package com.bytedance.Animal;

public class Dog extends Animal{
    public Dog(String name) {
        super(name);
    }

    @Override
    void sing() {
        System.out.println("小狗在唱歌");
    }

    void eat() {
        System.out.println("小狗爱吃🦴🦴🦴");
    }
}
