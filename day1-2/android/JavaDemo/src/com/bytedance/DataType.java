package com.bytedance;

import com.bytedance.Animal.Dog;

public class DataType {
    public static void main(String[] args) {
        boolean bool = Boolean.TRUE; // default = false
        byte b = Byte.MAX_VALUE; // default = 0
        char c = Character.MAX_VALUE; // '\uFFFF'
        short s = Short.MAX_VALUE; // 0x7fff
        int i = Integer.MAX_VALUE; //0x7fff ffff
        long l = Long.MAX_VALUE; // 0x7ffff ffff ffff ffff
        float f = Float.MAX_VALUE; //0x1.fffffeP+127f
        double d = Double.MAX_VALUE; //0x1.fffffffffffffP+1023


        Dog dog1 = new Dog("旺财");
        Dog dog2 = dog1;
        System.out.println(dog1 == dog2);

        dog2.name = "汪星人";
        System.out.println(dog2.name); //
    }
}
