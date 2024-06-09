package com.example.cal24;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class GenerateRandomNum {
    public static  int[] generateFourRandomNum() {
        int[] ans={0,0,0,0};
        Random r = new Random();
        for (int i = 0; i < 4; i++) {
            ans[i] = r.nextInt(12)+1; // 生成[1,13]区间的整数
        }
        return ans;
    }
}
