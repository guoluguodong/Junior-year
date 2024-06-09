package com.example.myapplication.slice;

import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.content.Intent;
import ohos.agp.components.DirectionalLayout;
import ohos.agp.components.TabList;
import ohos.agp.components.TableLayout;
import ohos.agp.components.Text;
import ohos.app.Context;
import ohos.data.DatabaseHelper;
import ohos.data.preferences.Preferences;

import java.util.ArrayList;
import java.util.Map;

public class HistoryScoreSlice extends AbilitySlice {
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setUIContent(ResourceTable.Layout_ability_history_score);

        Context context = getApplicationContext();
        DatabaseHelper databaseHelper = new DatabaseHelper(context); // context入参类型为ohos.app.Context。
        String fileName = "test_pref"; // fileName表示文件名，其取值不能为空，也不能包含路径，默认存储目录可以通过context.getPreferencesDir()获取。
        Preferences preferences = databaseHelper.getPreferences(fileName);
        DirectionalLayout dl = (DirectionalLayout) findComponentById(ResourceTable.Id_historydl);
        TabList tabList = (TabList) findComponentById(ResourceTable.Id_tab_list);
        TabList.Tab tab = tabList.new Tab(getContext());
        tab.setText("得分");
        tabList.addTab(tab);
        TabList.Tab tab1 = tabList.new Tab(getContext());
        tab1.setText("步数");
        tabList.addTab(tab1);
        TabList.Tab tab2 = tabList.new Tab(getContext());
        tab2.setText("用时");
        tabList.addTab(tab2);
        Map<String,?> ms = preferences.getAll();
        String values = ms.values().toString();

        if(values.length()>10){
            String[] records = values.substring(1,values.length()-3).split("得分");
            for (int i=1;i<records.length;i++){
                TabList tabListTemp = new TabList(getContext());
                tabListTemp.setLayoutConfig(tabList.getLayoutConfig());
                tabListTemp.setTabTextSize(60);
                tabListTemp.setTabLength(300);
                dl.addComponent(tabListTemp);
                String[] s = records[i].split("步数");
                String[] ss = s[1].split("用时");
                String score = s[0].split(",")[0];
                String step = ss[0].split(",")[0];
                String time = ss[1].split(",")[0];
                TabList.Tab tabtemp = tabList.new Tab(getContext());
                tabtemp.setText(score);
                tabListTemp.addTab(tabtemp);
                TabList.Tab tabtemp2 = tabList.new Tab(getContext());
                tabtemp2.setText(step);
                tabListTemp.addTab(tabtemp2);
                TabList.Tab tabtemp3 = tabList.new Tab(getContext());
                tabtemp3.setText(time);
                tabListTemp.addTab(tabtemp3);


            }
        }

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
