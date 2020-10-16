package com.bytedance;

import java.util.ArrayList;

public class ListDemo {
    public static void main(String[] args) {
        // 声明并初始化一个列表
        ArrayList<String> list = new ArrayList<>();

        // 向列表中添加两个元素
        list.add("Hello");
        list.add("World");

        // 三种方式遍历
        // for i
        for (int i = 0; i < list.size(); i++){
            System.out.println(list.get(i));
        }
        // for each
        for (String it : list){
            System.out.println(it);
        }
        // since java8
        list.forEach(System.out::println);
    }
}
