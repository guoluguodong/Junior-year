package com.example.myapplication.slice;

import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.content.Intent;
import ohos.agp.components.Button;
import ohos.agp.components.Component;
import ohos.agp.window.dialog.CommonDialog;
import ohos.agp.window.dialog.IDialog;

public class StatementHelpSlice extends AbilitySlice {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setUIContent(ResourceTable.Layout_statement_help);
        Button backButton = (Button) findComponentById(ResourceTable.Id_goback);
        backButton.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                CommonDialog dialog = new CommonDialog(getContext());
                dialog.setTitleText(null);
                dialog.setMovable(true);
                dialog.setSize(1000, 200);
                dialog.setContentText("规则太容易啦");
                dialog.setButton(IDialog.BUTTON1, "确认退出", (iDialog, i) -> {
                    iDialog.destroy();
                    terminateAbility();
                });
                dialog.setButton(IDialog.BUTTON2, "取消", (iDialog, i) ->{iDialog.destroy();});
                dialog.show();
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
