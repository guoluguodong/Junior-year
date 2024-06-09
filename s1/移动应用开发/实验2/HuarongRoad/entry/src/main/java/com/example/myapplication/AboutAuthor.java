package com.example.myapplication;

import com.example.myapplication.slice.AboutAuthorSlice;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;

public class AboutAuthor extends Ability {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(AboutAuthorSlice.class.getName());
    }
}
