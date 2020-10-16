package com.bytedance.Animal;

public class Test {

    static void func(){
        System.out.println("我是静态方法");
    }

    void func2(){
        System.out.println("我是成员方法");
    }

    public static void main(String[] args) {
        Test.func(); // 直接调用静态方法
        new Test().func2(); // 先创建 实例 再调用成员方法
    }
}
