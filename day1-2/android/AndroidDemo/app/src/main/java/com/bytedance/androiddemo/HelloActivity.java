package com.bytedance.androiddemo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.widget.TextView;

public class HelloActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hello);//

        TextView tv = findViewById(R.id.tv_hello);// id
        tv.setText("你好世界！"); //
        tv.setTextColor(Color.RED);

        // todo more
    }
}