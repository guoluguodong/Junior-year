package com.example.cal24;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class HomePage  extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_homepage);
        Button button = (Button) findViewById(R.id.buttonStartGame);
        TextView text = findViewById(R.id.tv_marquee);
        text.setSelected(true);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(HomePage.this,MainActivity.class);
                startActivity(intent);
            }
        });
        Button button2 = (Button) findViewById(R.id.buttonSeeHelp);
        button2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(HomePage.this,Statement.class);
                startActivity(intent);
            }
        });
        Button button3 = (Button) findViewById(R.id.seeaboutauthor);
        button3.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(HomePage.this,AboutAuthor.class);
                startActivity(intent);
            }
        });
        Button button4 = (Button) findViewById(R.id.buttonTrainModel);
        button4.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(HomePage.this,TrainModel.class);
                startActivity(intent);
            }
        });
    }
}
