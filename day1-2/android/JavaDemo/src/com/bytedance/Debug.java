package com.bytedance;

public class Debug {
    public static void main(String[] args) {
        int i = 0;
        for (;i < 5; i++){
            if (i == 2){
                func();
            }
        }
    }

    public static void func(){
        System.out.println("func called");
    }
}
