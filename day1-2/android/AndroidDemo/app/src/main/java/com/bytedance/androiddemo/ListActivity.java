package com.bytedance.androiddemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;

public class ListActivity extends AppCompatActivity {

    ListView listView;
    ArrayList<String> list = new ArrayList<String>();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 0. 初始化数据
        list.add("A");
        list.add("B");
        list.add("C");

        // 1. 关联布局文件
        setContentView(R.layout.activity_demo);
        // 2. 初始化控件
        listView = findViewById(R.id.lv);
        // 3. 设置适配器
        listView.setAdapter(new ArrayAdapter<>(
                ListActivity.this,
                android.R.layout.simple_expandable_list_item_1,
                list));
    }
}