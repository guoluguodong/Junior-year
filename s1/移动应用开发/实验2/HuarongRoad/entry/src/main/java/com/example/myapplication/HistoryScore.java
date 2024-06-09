package com.example.myapplication;

import com.example.myapplication.slice.HistoryScoreSlice;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;

public class HistoryScore extends Ability {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(HistoryScoreSlice.class.getName());
    }
}
