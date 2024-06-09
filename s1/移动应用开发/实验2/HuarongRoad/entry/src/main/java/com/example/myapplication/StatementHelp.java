package com.example.myapplication;

import com.example.myapplication.slice.StatementHelpSlice;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;

public class StatementHelp extends Ability {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(StatementHelpSlice.class.getName());
    }
}
