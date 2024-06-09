package com.example.myapplication;

import com.example.myapplication.slice.HuarongPathSlice;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;

public class HuarongPath extends Ability {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(HuarongPathSlice.class.getName());
    }
}
