package com.example.myapplication;

import com.example.myapplication.slice.PageAlbumSlice;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;

public class PageAlbum extends Ability {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(PageAlbumSlice.class.getName());
    }
}
