package com.bytedance;

public class FunctionType {
    int type = 0;
    public static void main(String[] args) {
        FunctionType.staticFunc(); // 静态方法直接调用
        new FunctionType().instanceFunc();// 实例方法挂载在对象上
    }

    public static void staticFunc(){
        System.out.println("我是静态方法, 不可以访问 this.type");
    }

    public void instanceFunc(){
        System.out.println("我是实例方法, 访问 " + this.type );
    }
}
