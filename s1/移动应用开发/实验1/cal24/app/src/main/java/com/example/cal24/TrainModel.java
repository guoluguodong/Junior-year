package com.example.cal24;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class TrainModel extends AppCompatActivity {
    @SuppressLint("ResourceAsColor")
    final int[] curFourNum = {0,0,0,0};
    final int[] curState = {0, 0, 0, 0};
    final ArrayList<Integer> fourcards = new ArrayList<>();
    final int[] selected = {0};
    final int[] Sign = {-1};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.train_model_layout);

//        直到生成能计算24点的4个数
        while (CalModel.calModel(curFourNum).size() == 0) {
            int[] temp = GenerateRandomNum.generateFourRandomNum();
            for (int i = 0; i < 4; i++) {
                curFourNum[i] =temp[i];
            }
        }
        for (int i = 0; i < 4; i++) {
            if(fourcards.size()<=4)
                fourcards.add(i,curFourNum[i]);
            else{
                fourcards.set(i,curFourNum[i]);
            }
        }

        ArrayList<Button> ButtonNum = new ArrayList<>();
        ButtonNum.add(findViewById(R.id.num1));
        ButtonNum.add(findViewById(R.id.num2));
        ButtonNum.add(findViewById(R.id.num3));
        ButtonNum.add(findViewById(R.id.num4));
        for (int i = 0; i < 4; i++) {
            ButtonNum.get(i).setText(String.valueOf(curFourNum[i]));
            int finalI = i;
            ButtonNum.get(i).setOnClickListener(new View.OnClickListener() {
                @SuppressLint("UseCompatLoadingForDrawables")
                @Override
                public void onClick(View view) {
                    if (curState[finalI] == 0 && selected[0] < 1) {
                        view.setBackground(getDrawable(R.drawable.btn_selected2));
                        curState[finalI] = 1;
                        selected[0] = selected[0] + 1;
                    } else if (curState[finalI] == 0 && selected[0] == 1 && Sign[0] !=-1) {
                        view.setBackground(getDrawable(R.drawable.btn_selected2));
                        int anotherNum = 0;
                        for (int j = 0; j < 4; j++) {
                            if (curState[j] == 1) {
                                anotherNum = j;
                                break;
                            }
                        }
                        curState[finalI] = 1;
                        selected[0] = selected[0] + 1;
//                        灰掉第一个被选中的数
                        ButtonNum.get(anotherNum).setText("");
                        ButtonNum.get(anotherNum).setBackground(getDrawable(R.drawable.btn_disappear));
                        ButtonNum.get(anotherNum).setClickable(false);
//                        第二个被选中的数显示计算结果
                        int ans=0;
                        if(Sign[0] == 0){
                            ans = curFourNum[anotherNum] + curFourNum[finalI];
                        }
                        else if(Sign[0] == 1){
                            ans = curFourNum[anotherNum] - curFourNum[finalI];
                        }
                        else if(Sign[0] == 2){
                            ans = curFourNum[anotherNum] * curFourNum[finalI];
                        }
                        else{
                            ans = (int) curFourNum[anotherNum] / curFourNum[finalI];
                        }

                        ButtonNum.get(finalI).setText(String.valueOf(ans));
                        curFourNum[anotherNum] = 0;
                        curState[anotherNum]=0;
                        curFourNum[finalI] = ans;
                        ButtonNum.get(finalI).setBackground(getDrawable(R.drawable.btn_selected1));
                        curState[finalI] = 0;
                        selected[0] = 0;
                        Sign[0] = -1;
                    } else if (curState[finalI] == 1 ) {
                        view.setBackground(getDrawable(R.drawable.btn_selected1));
                        curState[finalI] = 0;
                        selected[0] = selected[0] - 1;
                    }
                }
            });
        }
        ArrayList<Button> ButtonSign = new ArrayList<>();
        ButtonSign.add(findViewById(R.id.plus));
        ButtonSign.add(findViewById(R.id.sub));
        ButtonSign.add(findViewById(R.id.mul));
        ButtonSign.add(findViewById(R.id.div));
        for (int i = 0; i < 4; i++) {
            int finalI = i;
            ButtonSign.get(i).setOnClickListener(new View.OnClickListener() {
                @SuppressLint("UseCompatLoadingForDrawables")
                @Override
                public void onClick(View view) {
                    Sign[0] = finalI;

                }
            });
        }
        Button buttonRestart =  findViewById(R.id.reset);
        buttonRestart.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ImageModel.init();
                Intent intent = getIntent();
                finish();
                startActivity(intent);
            }
        });
        Button button = (Button) findViewById(R.id.seeResult);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(TrainModel.this, Result.class);
                intent.putExtra("curFourCard", fourcards.toString());
                ImageModel.init();
                Intent intent1 = getIntent();
                finish();
                startActivity(intent1);
                startActivity(intent);
            }
            });
    }



}
