package com.example.myapplication;

import com.example.myapplication.slice.GameLevelSlice;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;

public class GameLevel extends Ability {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(GameLevelSlice.class.getName());
    }
}
