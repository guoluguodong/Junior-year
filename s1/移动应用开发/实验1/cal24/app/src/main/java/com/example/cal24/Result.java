package com.example.cal24;


import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class Result extends AppCompatActivity {
    @SuppressLint("ResourceAsColor")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);
        Intent intent = getIntent();
        String msg = intent.getStringExtra("curFourCard");
        String cleanInput = msg.replaceAll("\\[|\\]", "");
        // 使用逗号 "," 分割字符串
        String[] parts = cleanInput.split(",");
        ArrayList<Integer> curFourCard = new ArrayList<Integer>();
        int[] arr =new int[4];
        for (String part : parts) {
            int intValue = Integer.parseInt(part.trim()); // 去除空格并解析为整数
            curFourCard.add(intValue);
        }

        ImageModel.init();
        LinearLayout fourCard = findViewById(R.id.fourCard);
        final List<ImageView> fourCards = MainActivity.getAllImageViewsInLayout(fourCard);
        for (int j = 0; j < 4; j++) {
            int i = (curFourCard.get(j)-1)%13;
            arr[j] = i+1;
            if(1<=curFourCard.get(j) && curFourCard.get(j)<=13){
                fourCards.get(j).setImageResource(ImageModel.hearts.get(i));
            }
            else if(14<=curFourCard.get(j) && curFourCard.get(j)<=26){
                fourCards.get(j).setImageResource(ImageModel.diamonds.get(i));
            }
            else if(27<=curFourCard.get(j) && curFourCard.get(j)<=39){
                fourCards.get(j).setImageResource(ImageModel.clubs.get(i));
            }
            else if(40<=curFourCard.get(j) && curFourCard.get(j)<=52){
                fourCards.get(j).setImageResource(ImageModel.spades.get(i));
            }
        }
        ArrayList<String> allRes = CalModel.calModel(arr);
        LinearLayout resultLayout = findViewById(R.id.result);
        TextView moban =findViewById(R.id.moban);
        FrameLayout.LayoutParams layoutParams1 = new FrameLayout.LayoutParams(moban.getLayoutParams());
        if(allRes.size()>0){
            moban.setText("一共有"+ allRes.size()+"种不同的结果");
        }
        for(int i=0;i<allRes.size();i++){
            TextView lineTextView = new TextView(Result.this);
            lineTextView.setText(allRes.get(i));
            lineTextView.setLayoutParams(layoutParams1);
            lineTextView.setTextSize(30);
            lineTextView.setTextColor(R.color.purple_200);
            lineTextView.setGravity(Gravity.CENTER);
            resultLayout.addView(lineTextView);
        }


    }
}