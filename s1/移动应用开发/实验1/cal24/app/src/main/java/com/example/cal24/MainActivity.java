package com.example.cal24;


import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ImageModel.init();
        LinearLayout fourCard = findViewById(R.id.fourCard);
        final List<ImageView> fourCards = getAllImageViewsInLayout(fourCard);

        FrameLayout heartLayout = findViewById(R.id.heartlayout);
//        设置红桃的13张图片
        ImageView exampleImageView = findViewById(R.id.card);
        FrameLayout.LayoutParams exampleLayout = (FrameLayout.LayoutParams) exampleImageView.getLayoutParams();
        for (int i = 0; i < 13; i++) {
            ImageView cardsImageView = new ImageView(MainActivity.this);
            cardsImageView.setImageResource(ImageModel.hearts.get(i));
            FrameLayout.LayoutParams layoutParams1 = new FrameLayout.LayoutParams(exampleLayout.width, exampleLayout.height);
            layoutParams1.topMargin = exampleLayout.topMargin;
            layoutParams1.leftMargin = i * 60;
            cardsImageView.setLayoutParams(layoutParams1);
            heartLayout.addView(cardsImageView);
            cardsImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    FrameLayout.LayoutParams curLayout = (FrameLayout.LayoutParams) view.getLayoutParams();
//                    计算第i张图片
                    int i = (int) curLayout.leftMargin / 60;
                    if (ImageModel.heartIsClicked.get(i) == 0) {
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == 0) {
                                fourCards.get(j).setImageResource(ImageModel.hearts.get(i));
                                ImageModel.curFourCard.set(j, i + 1);
                                ImageModel.heartIsClicked.set(i, 1);
                                curLayout.topMargin = (curLayout.topMargin) / 2;
                                view.setLayoutParams(curLayout);
                                break;
                            }
                        }
                    } else {
                        ImageModel.heartIsClicked.set(i, 0);
                        curLayout.topMargin = (curLayout.topMargin) * 2;
                        view.setLayoutParams(curLayout);
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == i + 1) {
                                fourCards.get(j).setImageResource(R.drawable.back);
                                ImageModel.curFourCard.set(j, 0);
                            }
                        }

                    }

                }
            });
        }
        FrameLayout diamondLayout = findViewById(R.id.diamondLayout);
        for (int i = 0; i < 13; i++) {
            ImageView cardsImageView = new ImageView(MainActivity.this);
            cardsImageView.setImageResource(ImageModel.diamonds.get(i));
            FrameLayout.LayoutParams layoutParams1 = new FrameLayout.LayoutParams(exampleLayout.width, exampleLayout.height);
            layoutParams1.topMargin = exampleLayout.topMargin;
            layoutParams1.leftMargin = i * 60;
            cardsImageView.setLayoutParams(layoutParams1);
            diamondLayout.addView(cardsImageView);
            cardsImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    FrameLayout.LayoutParams curLayout = (FrameLayout.LayoutParams) view.getLayoutParams();
//                    计算第i张图片
                    int i = (int) curLayout.leftMargin / 60;
                    if (ImageModel.diamondIsClicked.get(i) == 0) {

                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == 0) {
                                fourCards.get(j).setImageResource(ImageModel.diamonds.get(i));
                                ImageModel.curFourCard.set(j, i + 1 + 13);
                                ImageModel.diamondIsClicked.set(i, 1);
                                curLayout.topMargin = (curLayout.topMargin) / 2;
                                view.setLayoutParams(curLayout);
                                break;
                            }
                        }
                    } else {
                        ImageModel.diamondIsClicked.set(i, 0);
                        curLayout.topMargin = (curLayout.topMargin) * 2;
                        view.setLayoutParams(curLayout);
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == i + 1 + 13) {
                                fourCards.get(j).setImageResource(R.drawable.back);
                                ImageModel.curFourCard.set(j, 0);
                            }
                        }
                    }

                }
            });
        }
        FrameLayout clubLayout = findViewById(R.id.clubLayout);
        for (int i = 0; i < 13; i++) {
            ImageView cardsImageView = new ImageView(MainActivity.this);
            cardsImageView.setImageResource(ImageModel.clubs.get(i));
            FrameLayout.LayoutParams layoutParams1 = new FrameLayout.LayoutParams(exampleLayout.width, exampleLayout.height);
            layoutParams1.topMargin = exampleLayout.topMargin;
            layoutParams1.leftMargin = i * 60;
            cardsImageView.setLayoutParams(layoutParams1);
            clubLayout.addView(cardsImageView);
            cardsImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    FrameLayout.LayoutParams curLayout = (FrameLayout.LayoutParams) view.getLayoutParams();
//                    计算第i张图片
                    int i = (int) curLayout.leftMargin / 60;
                    if (ImageModel.clubIsClicked.get(i) == 0) {
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == 0) {
                                fourCards.get(j).setImageResource(ImageModel.clubs.get(i));
                                ImageModel.curFourCard.set(j, i + 1 + 13*2);
                                ImageModel.clubIsClicked.set(i, 1);
                                curLayout.topMargin = (curLayout.topMargin) / 2;
                                view.setLayoutParams(curLayout);
                                break;
                            }
                        }
                    } else {
                        ImageModel.clubIsClicked.set(i, 0);
                        curLayout.topMargin = (curLayout.topMargin) * 2;
                        view.setLayoutParams(curLayout);
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == i + 1 + 13*2) {
                                fourCards.get(j).setImageResource(R.drawable.back);
                                ImageModel.curFourCard.set(j, 0);
                            }
                        }
                    }
                }
            });
        }
        FrameLayout spadeLayout = findViewById(R.id.spadeLayout);
        for (int i = 0; i < 13; i++) {
            ImageView cardsImageView = new ImageView(MainActivity.this);
            cardsImageView.setImageResource(ImageModel.spades.get(i));
            FrameLayout.LayoutParams layoutParams1 = new FrameLayout.LayoutParams(exampleLayout.width, exampleLayout.height);
            layoutParams1.topMargin = exampleLayout.topMargin;
            layoutParams1.leftMargin = i * 60;
            cardsImageView.setLayoutParams(layoutParams1);
            spadeLayout.addView(cardsImageView);
            cardsImageView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    FrameLayout.LayoutParams curLayout = (FrameLayout.LayoutParams) view.getLayoutParams();
//                    计算第i张图片
                    int i = (int) curLayout.leftMargin / 60;
                    if (ImageModel.spadeIsClicked.get(i) == 0) {
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == 0) {
                                fourCards.get(j).setImageResource(ImageModel.spades.get(i));
                                ImageModel.curFourCard.set(j, i + 1 + 13*3);
                                ImageModel.spadeIsClicked.set(i, 1);
                                curLayout.topMargin = (curLayout.topMargin) / 2;
                                view.setLayoutParams(curLayout);

                                break;
                            }
                        }
                    } else {
                        ImageModel.spadeIsClicked.set(i, 0);
                        curLayout.topMargin = (curLayout.topMargin) * 2;
                        view.setLayoutParams(curLayout);
                        for (int j = 0; j < 4; j++) {
                            if (ImageModel.curFourCard.get(j) == i + 1 + 13*3) {
                                fourCards.get(j).setImageResource(R.drawable.back);
                                ImageModel.curFourCard.set(j, 0);
                            }
                        }
                    }

                }
            });
        }
        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                boolean flag=true;
                for(int i=0;i<4;i++){
                    if (ImageModel.curFourCard.get(i) == 0) {
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    Intent intent = new Intent(MainActivity.this,Result.class);
                    intent.putExtra("curFourCard", ImageModel.curFourCard.toString());
                    ImageModel.init();
                    Intent intent1 = getIntent();
                    finish();
                    startActivity(intent1);
                    startActivity(intent);
                }
                else{
                    Toast.makeText(getApplicationContext(), "请选取4张扑克牌", Toast.LENGTH_SHORT).show();
                }
            }
        });
        Button buttonClear = (Button) findViewById(R.id.button2);
        buttonClear.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ImageModel.init();
                Intent intent = getIntent();
                finish();
                startActivity(intent);
            }
        });

    }
//    添加点击事件

    //   根据layout返回其中的子组件
    public static List<ImageView> getAllImageViewsInLayout(ViewGroup layout) {
        List<ImageView> imageViews = new ArrayList<>();
        for (int i = 0; i < layout.getChildCount(); i++) {
            View child = layout.getChildAt(i);
            if (child instanceof ImageView) {
                // 如果子视图是 ImageView，则将其添加到列表中
                imageViews.add((ImageView) child);
            } else if (child instanceof ViewGroup) {
                // 如果子视图是 ViewGroup，则递归查找其内部的 ImageView
                imageViews.addAll(getAllImageViewsInLayout((ViewGroup) child));
            }
        }
        return imageViews;
    }
}