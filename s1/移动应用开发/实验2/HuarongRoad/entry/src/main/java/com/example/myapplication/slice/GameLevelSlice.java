package com.example.myapplication.slice;

import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.content.Intent;
import ohos.aafwk.content.Operation;
import ohos.agp.components.Button;
import ohos.agp.components.Component;
import ohos.agp.window.dialog.CommonDialog;
import ohos.agp.window.dialog.IDialog;

public class GameLevelSlice extends AbilitySlice {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setUIContent(ResourceTable.Layout_ability_game_level);
        Button FirstLevel = (Button) findComponentById(ResourceTable.Id_first);
        FirstLevel.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.HuarongPath")
                        .build();
                intent.setParam("id",ResourceTable.Media_1);
                intent.setOperation(operation);
                startAbility(intent);
            }
        });
        Button SecondLevel = (Button) findComponentById(ResourceTable.Id_second);
        SecondLevel.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.HuarongPath")
                        .build();
                intent.setParam("id",ResourceTable.Media_2);
                intent.setOperation(operation);
                startAbility(intent);

            }
        });
        Button ThirdLevel = (Button) findComponentById(ResourceTable.Id_third);
        ThirdLevel.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.HuarongPath")
                        .build();
                intent.setParam("id",ResourceTable.Media_3);
                intent.setOperation(operation);
                startAbility(intent);

            }
        });
        Button backButton = (Button) findComponentById(ResourceTable.Id_goback);
        backButton.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                terminateAbility();
            }
        });
    }

    @Override
    public void onActive() {
        super.onActive();
    }

    @Override
    public void onForeground(Intent intent) {
        super.onForeground(intent);
    }
}
