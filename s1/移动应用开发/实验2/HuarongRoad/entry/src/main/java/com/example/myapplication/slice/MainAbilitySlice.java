package com.example.myapplication.slice;

import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.content.Intent;
import ohos.aafwk.content.Operation;
import ohos.agp.components.Button;
import ohos.agp.components.Component;

public class MainAbilitySlice extends AbilitySlice {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setUIContent(ResourceTable.Layout_ability_main);
        Button huarongpath = (Button) findComponentById(ResourceTable.Id_startgame);
        huarongpath.setClickedListener(new Component.ClickedListener() {
            @Override
            public void onClick(Component component) {
                // 此处添加点击按钮后的事件处理逻辑
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.GameLevel")
                        .build();
                intent.setOperation(operation);
                startAbility(intent);
            }
        });
        Button huaronghistory = (Button) findComponentById(ResourceTable.Id_historyscore);
        huaronghistory.setClickedListener(new Component.ClickedListener() {
            @Override
            public void onClick(Component component) {
                // 此处添加点击按钮后的事件处理逻辑
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.HistoryScore")
                        .build();
                intent.setOperation(operation);
                startAbility(intent);
            }
        });
        Button statementButton = (Button) findComponentById(ResourceTable.Id_statement);
        statementButton.setClickedListener(new Component.ClickedListener() {
            @Override
            public void onClick(Component component) {
                // 此处添加点击按钮后的事件处理逻辑
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.StatementHelp")
                        .build();
                intent.setOperation(operation);
                startAbility(intent);
            }
        });
        Button albumButton = (Button) findComponentById(ResourceTable.Id_about_author);
        albumButton.setClickedListener(new Component.ClickedListener() {
            @Override
            public void onClick(Component component) {
                // 此处添加点击按钮后的事件处理逻辑
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder()
                        .withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.AboutAuthor")
                        .build();
                intent.setOperation(operation);
                startAbility(intent);
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
