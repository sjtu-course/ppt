package com.bytedance.Animal;

// 猫咪继承了动物
public class Cat extends Animal{
    Cat(String name) {
        super(name);
    }

    @Override
    void sing() {
        System.out.println("猫咪在唱歌");
    }
}
