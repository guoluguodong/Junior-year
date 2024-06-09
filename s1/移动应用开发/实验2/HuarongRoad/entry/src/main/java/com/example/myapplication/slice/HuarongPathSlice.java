package com.example.myapplication.slice;

import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.ability.DataAbilityHelper;
import ohos.aafwk.content.Intent;
import ohos.aafwk.content.Operation;
import ohos.agp.animation.Animator;
import ohos.agp.animation.AnimatorProperty;
import ohos.agp.animation.AnimatorScatter;
import ohos.agp.animation.AnimatorValue;
import ohos.agp.colors.RgbColor;
import ohos.agp.components.*;
import ohos.agp.components.element.PixelMapElement;
import ohos.agp.components.element.ShapeElement;
import ohos.agp.components.element.StateElement;
import ohos.agp.render.Canvas;
import ohos.agp.render.Paint;
import ohos.agp.render.Texture;
import ohos.agp.utils.Color;
import ohos.agp.utils.LayoutAlignment;
import ohos.agp.window.dialog.CommonDialog;
import ohos.agp.window.dialog.IDialog;
import ohos.app.Context;
import ohos.data.DatabaseHelper;
import ohos.data.preferences.Preferences;
import ohos.data.rdb.ValuesBucket;
import ohos.global.resource.NotExistException;
import ohos.global.resource.Resource;
import ohos.media.image.ImagePacker;
import ohos.media.image.ImageSource;
import ohos.media.image.PixelMap;
import ohos.media.image.common.*;
import ohos.media.photokit.metadata.AVStorage;
import ohos.utils.net.Uri;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Stack;
//import org.joda.time.DateTime;
import static ohos.media.image.ImageSource.createIncrementalSource;

public class HuarongPathSlice extends AbilitySlice {
    ArrayList<PixelMapElement> pic = new ArrayList<>();
    int m;
    int n;
    int step = 0;
    // state 记录华容道状态，state[i][j]=k,代表当前第i行，第j列块为第k块(1<=k<=m*n)，k=m*n代表为空块
    int[][] state;
    // originState记录pic中的按钮顺序
    ArrayList<Integer> originState;
    TickTimer timer;
    List<SampleItem> list = getData();
    DirectionalLayout firstDl = new DirectionalLayout(getContext());
    // 设置布局大小
    DirectionalLayout firstleftDl = new DirectionalLayout(getContext());
    DirectionalLayout secondtotalDl = new DirectionalLayout(getContext());

    SampleItemProvider sampleItemProvider = new SampleItemProvider(list, this);
    Text textM = new Text(getContext());
    Text textN = new Text(getContext());
    DirectionalLayout.LayoutConfig layoutConfig;
    boolean showNum = true;
    int imgRequestCode = ResourceTable.Media_1;
    ImageSource imageSource;
    PixelMap pixelMap;
    int animationMode = 1;
    int animationRorate = 1;

    int score = 0;
    Stack<Integer> historyStep = new Stack<>();
    PixelMap getPixelMap(int imageId) {
        ImageSource imageSource = null;
        InputStream inputStream = null;
        try {
            inputStream = getContext().getResourceManager().getResource(imageId);
            imageSource = ImageSource.create(inputStream, null);
            inputStream.close();
        } catch (NotExistException | IOException e) {
            throw new RuntimeException(e);
        }
        return imageSource.createPixelmap(null);
    }

    ImageSource getImageSource(int imageId) {
        ImageSource imageSource = null;
        InputStream inputStream = null;
        try {
            inputStream = getContext().getResourceManager().getResource(imageId);
            imageSource = ImageSource.create(inputStream, null);
            inputStream.close();
        } catch (NotExistException | IOException e) {
            throw new RuntimeException(e);
        }
        return imageSource;
    }

    // 交换按钮的背景和文字
    boolean swapButton(int ButtonNum) {
        boolean valid = false;
        int I = 0, Ii = 0;
        for (int j = 0; j < m; j++) {
            for (int jj = 0; jj < n; jj++) {
                if (state[j][jj] == ButtonNum) {
                    I = j;
                    Ii = jj;
                    break;
                }
            }
        }
        TableLayout Dl1 = (TableLayout) secondtotalDl.getComponentAt(0);
        if (I - 1 >= 0 && state[I - 1][Ii] == (this.m) * (this.n)) {
            // 在pic中找到按钮值为state[I - 1][Ii] - 1、state[I][Ii] - 1
            int a = 0, b = 0;
            for (int k = 0; k < originState.size(); k++) {
                if (originState.get(k) + 1 == state[I - 1][Ii]) {
                    a = k;
                }
                if (originState.get(k) + 1 == state[I][Ii]) {
                    b = k;
                }
            }
            Button DL1 = (Button) Dl1.getComponentAt(a);
            Button DL2 = (Button) Dl1.getComponentAt(b);
            float temp = DL2.getContentPositionY();
            state[I - 1][Ii] = state[I][Ii];
            state[I][Ii] = (this.m) * (this.n);
            valid = true;
            if (animationMode == 0) {
                DL2.setContentPositionY(DL1.getContentPositionY());
                DL1.setContentPositionY(temp);
            } else if (animationMode == 1) {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromY(DL2.getContentPositionY()).moveToY(DL1.getContentPositionY()).setDuration(1000).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                DL1.setContentPositionY(temp);
            } else {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromY(DL2.getContentPositionY()).moveToY(DL1.getContentPositionY()).setDuration(1000).rotate(720 * animationRorate).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                animationRorate = -animationRorate;
                DL1.setContentPositionY(temp);
            }
        } else if (I + 1 < m && state[I + 1][Ii] == (this.m) * (this.n)) {
            int a = 0, b = 0;
            for (int k = 0; k < originState.size(); k++) {
                if (originState.get(k) + 1 == state[I + 1][Ii]) {
                    a = k;
                }
                if (originState.get(k) + 1 == state[I][Ii]) {
                    b = k;
                }
            }
            Button DL1 = (Button) Dl1.getComponentAt(a);
            Button DL2 = (Button) Dl1.getComponentAt(b);
            state[I + 1][Ii] = state[I][Ii];
            state[I][Ii] = (this.m) * (this.n);
            valid = true;
            float temp = DL2.getContentPositionY();
            if (animationMode == 0) {
                DL2.setContentPositionY(DL1.getContentPositionY());
                DL1.setContentPositionY(temp);
            } else if (animationMode == 1) {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromY(DL2.getContentPositionY()).moveToY(DL1.getContentPositionY()).setDuration(1000).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                DL1.setContentPositionY(temp);
            } else {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromY(DL2.getContentPositionY()).moveToY(DL1.getContentPositionY()).setDuration(1000).rotate(720 * animationRorate).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                animationRorate = -animationRorate;
                DL1.setContentPositionY(temp);
            }
        } else if (Ii - 1 >= 0 && state[I][Ii - 1] == (this.m) * (this.n)) {
            int a = 0, b = 0;
            for (int k = 0; k < originState.size(); k++) {
                if (originState.get(k) + 1 == state[I][Ii - 1]) {
                    a = k;
                }
                if (originState.get(k) + 1 == state[I][Ii]) {
                    b = k;
                }
            }
            Button DL1 = (Button) Dl1.getComponentAt(a);
            Button DL2 = (Button) Dl1.getComponentAt(b);
            state[I][Ii - 1] = state[I][Ii];
            state[I][Ii] = (this.m) * (this.n);
            valid = true;
            float temp = DL2.getContentPositionX();
            if (animationMode == 0) {
                DL2.setContentPositionX(DL1.getContentPositionX());
                DL1.setContentPositionX(temp);
            } else if (animationMode == 1) {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromX(DL2.getContentPositionX()).moveToX(DL1.getContentPositionX()).setDuration(1000).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                DL1.setContentPositionX(temp);
            } else {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromX(DL2.getContentPositionX()).moveToX(DL1.getContentPositionX()).setDuration(1000).rotate(720 * animationRorate).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                animationRorate = -animationRorate;
                DL1.setContentPositionX(temp);
            }
        } else if (Ii + 1 < n && state[I][Ii + 1] == (this.m) * (this.n)) {
            int a = 0, b = 0;
            for (int k = 0; k < originState.size(); k++) {
                if (originState.get(k) + 1 == state[I][Ii + 1]) {
                    a = k;
                }
                if (originState.get(k) + 1 == state[I][Ii]) {
                    b = k;
                }
            }
            Button DL1 = (Button) Dl1.getComponentAt(a);
            Button DL2 = (Button) Dl1.getComponentAt(b);
            state[I][Ii + 1] = state[I][Ii];
            state[I][Ii] = (this.m) * (this.n);
            valid = true;
            float temp = DL2.getContentPositionX();
            if (animationMode == 0) {
                DL2.setContentPositionX(DL1.getContentPositionX());
                DL1.setContentPositionX(temp);
            } else if (animationMode == 1) {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromX(DL2.getContentPositionX()).moveToX(DL1.getContentPositionX()).setDuration(1000).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                DL1.setContentPositionX(temp);
            } else {
                AnimatorProperty animatorProperty = new AnimatorProperty();
                animatorProperty.setTarget(DL2);
                animatorProperty.moveFromX(DL2.getContentPositionX()).moveToX(DL1.getContentPositionX()).setDuration(1000).rotate(720 * animationRorate).setDelay(100);
                //点击图片开始/停止动画
                animatorProperty.start();
                animationRorate = -animationRorate;
                DL1.setContentPositionX(temp);
            }
        }
        return valid;
    }

    private ArrayList<SampleItem> getData() {
        ArrayList<SampleItem> list = new ArrayList<>();
        for (int i = 0; i <= 4; i++) {
            list.add(new SampleItem(String.valueOf(i + 2)));
        }
        return list;
    }

    private StateElement trackElementInit(ShapeElement on, ShapeElement off) {
        StateElement trackElement = new StateElement();
        trackElement.addState(new int[]{ComponentState.COMPONENT_STATE_CHECKED}, on);
        trackElement.addState(new int[]{ComponentState.COMPONENT_STATE_EMPTY}, off);
        return trackElement;
    }

    private StateElement thumbElementInit(ShapeElement on, ShapeElement off) {
        StateElement thumbElement = new StateElement();
        thumbElement.addState(new int[]{ComponentState.COMPONENT_STATE_CHECKED}, on);
        thumbElement.addState(new int[]{ComponentState.COMPONENT_STATE_EMPTY}, off);
        return thumbElement;
    }

    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        imgRequestCode = intent.getIntParam("id",ResourceTable.Media_1);
        if(imgRequestCode==ResourceTable.Media_1){
            this.m = 3;
            this.n = 3;
        }
        else if(imgRequestCode==ResourceTable.Media_2){
            this.m = 4;
            this.n = 4;
        }
        else if(imgRequestCode==ResourceTable.Media_3){
            this.m = 5;
            this.n = 5;
        }
        else {m = 3;
        n = 3;}
        state = new int[this.m][this.n];
        pixelMap = getPixelMap(imgRequestCode);
        imageSource = getImageSource(imgRequestCode);

        // 声明布局
        DirectionalLayout directionalLayout = new DirectionalLayout(getContext());
        // 设置布局大小
        directionalLayout.setWidth(ComponentContainer.LayoutConfig.MATCH_PARENT);
        directionalLayout.setHeight(ComponentContainer.LayoutConfig.MATCH_PARENT);
        // 设置布局属性
        directionalLayout.setOrientation(Component.VERTICAL);
        directionalLayout.setPadding(32, 32, 32, 20);
        // 添加背景图片
        PixelMap backgroundPM = getPixelMap(ResourceTable.Media_mountains_sunset_art_146043_800x1200);
        PixelMapElement background = new PixelMapElement(backgroundPM);
        directionalLayout.setBackground(background);
        // 第一行
        // 声明布局
        firstDl.setWidth(ComponentContainer.LayoutConfig.MATCH_PARENT);
        firstDl.setHeight(900);
        // 设置布局属性
        firstDl.setOrientation(Component.HORIZONTAL);
        firstDl.setPadding(32, 32, 32, 32);
        // 为组件添加对应布局的布局属性
        layoutConfig = new DirectionalLayout.LayoutConfig(ComponentContainer.LayoutConfig.MATCH_CONTENT, ComponentContainer.LayoutConfig.MATCH_CONTENT);
        layoutConfig.alignment = LayoutAlignment.HORIZONTAL_CENTER;
        // 提示图片
        Image image = new Image(getContext());
        image.setScaleMode(Image.ScaleMode.ZOOM_CENTER);
        image.setPixelMap(pixelMap);
        image.setLayoutConfig(layoutConfig);
        // 将Text添加到布局中
        firstDl.addComponent(initFirstLeftDL());
        firstDl.addComponent(image);
        // 第二行
        // second totalDl
        secondtotalDl.setWidth(ComponentContainer.LayoutConfig.MATCH_PARENT);
        secondtotalDl.setHeight(ComponentContainer.LayoutConfig.MATCH_PARENT);
        secondtotalDl.setOrientation(Component.HORIZONTAL);
        // 第二行设置难度、重新开始的按钮
        DirectionalLayout secondRightDl = new DirectionalLayout(getContext());
        // 设置布局大小
        secondRightDl.setWidth(400);
        secondRightDl.setHeight(ComponentContainer.LayoutConfig.MATCH_PARENT);
        // 设置布局属性
        secondRightDl.setOrientation(Component.VERTICAL);
//        类似的添加一个Button
        Button buttonRestart = new Button(getContext());
        layoutConfig.setMargins(0, 50, 0, 0);
        buttonRestart.setLayoutConfig(layoutConfig);
        buttonRestart.setText("重新开始");
        buttonRestart.setTextSize(50);
        buttonRestart.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                Intent intent = new Intent();
                Operation operation = new Intent.OperationBuilder().withDeviceId("")
                        .withBundleName("com.example.myapplication")
                        .withAbilityName("com.example.myapplication.HuarongPath").build();
                intent.setOperation(operation);
                intent.setParam("id",imgRequestCode);
                terminateAbility();
                startAbility(intent);
            }
        });
        Button buttonChoosePhoto = new Button(getContext());
        layoutConfig.setMargins(0, 50, 0, 0);
        buttonChoosePhoto.setLayoutConfig(layoutConfig);
        buttonChoosePhoto.setText("选择图片");
        buttonChoosePhoto.setTextSize(50);
        buttonChoosePhoto.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                String[] permissions = {"ohos.permission.READ_USER_STORAGE", "ohos.permission.WRITE_USER_STORAGE"};
                requestPermissionsFromUser(permissions, 0);
                imgRequestCode = 0;
                selectPhoto();
            }
        });
        Button buttonChooseHard = new Button(getContext());
        layoutConfig.setMargins(0, 50, 0, 0);
        buttonChooseHard.setLayoutConfig(layoutConfig);
        buttonChooseHard.setText("选择难度");
        buttonChooseHard.setTextSize(50);
        buttonChooseHard.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                CommonDialog dialog = new CommonDialog(getContext());
                ListContainer listContainerM = new ListContainer(getContext());
                listContainerM.setHeight(300);
                listContainerM.setWidth(500);
                listContainerM.setItemProvider(sampleItemProvider);
                listContainerM.setBindStateChangedListener(new Component.BindStateChangedListener() {
                    @Override
                    public void onComponentBoundToWindow(Component component) {
                        // ListContainer初始化时数据统一在provider中创建，不直接调用这个接口；
                        // 建议在onComponentBoundToWindow监听或者其他事件监听中调用。
                        sampleItemProvider.notifyDataChanged();
                    }

                    @Override
                    public void onComponentUnboundFromWindow(Component component) {
                    }
                });
                int[] tempMN = {m, n};
                listContainerM.setItemClickedListener((listContainer, component1, i, l) -> {
                    SampleItem item = (SampleItem) listContainerM.getItemProvider().getItem(i);
                    textM.setText(item.getName() + "行");
                    tempMN[0] = Integer.parseInt(item.getName());
                });
                ListContainer listContainerN = new ListContainer(getContext());
                listContainerN.setHeight(300);
                listContainerN.setWidth(500);
                listContainerN.setItemProvider(sampleItemProvider);
                listContainerN.setBindStateChangedListener(new Component.BindStateChangedListener() {
                    @Override
                    public void onComponentBoundToWindow(Component component) {
                        // ListContainer初始化时数据统一在provider中创建，不直接调用这个接口；
                        // 建议在onComponentBoundToWindow监听或者其他事件监听中调用。
                        sampleItemProvider.notifyDataChanged();
                    }

                    @Override
                    public void onComponentUnboundFromWindow(Component component) {
                    }
                });
                listContainerN.setItemClickedListener((listContainer, component1, i, l) -> {
                    SampleItem item = (SampleItem) listContainerN.getItemProvider().getItem(i);
                    textN.setText(item.getName() + "列");
                    tempMN[1] = Integer.parseInt(item.getName());
                });
                textM.setText("行");
                textM.setTextSize(50);
                textM.setLayoutConfig(layoutConfig);
                textN.setText("列");
                textN.setTextSize(50);
                textN.setLayoutConfig(layoutConfig);
                DirectionalLayout directionalLayout1 = new DirectionalLayout(getContext());
                directionalLayout1.setWidth(1000);
                directionalLayout1.setHeight(400);
                directionalLayout1.setOrientation(Component.HORIZONTAL);
                DirectionalLayout directionalLayout11 = new DirectionalLayout(getContext());
                directionalLayout11.setWidth(500);
                directionalLayout11.setHeight(400);
                directionalLayout11.setOrientation(Component.VERTICAL);
                DirectionalLayout directionalLayout12 = new DirectionalLayout(getContext());
                directionalLayout12.setWidth(500);
                directionalLayout12.setHeight(400);
                directionalLayout12.setOrientation(Component.VERTICAL);
                directionalLayout11.addComponent(listContainerM);
                directionalLayout11.addComponent(textM);
                directionalLayout12.addComponent(listContainerN);
                directionalLayout12.addComponent(textN);
                directionalLayout1.addComponent(directionalLayout11);
                directionalLayout1.addComponent(directionalLayout12);
                dialog.setContentCustomComponent(directionalLayout1);
                dialog.setSize(1000, 500);
                dialog.setButton(IDialog.BUTTON1, "确认", (iDialog, i) -> {
                    iDialog.destroy();
                    m = tempMN[0];
                    n = tempMN[1];
                    // 刷新组件
                    pic = new ArrayList<>();
                    step = 0;
                    // state 记录华容道状态，state[i][j]=k,代表当前第i行，第j列块为第k块(1<=k<=m*n)，k=m*n代表为空块
                    state = new int[m][n];
                    historyStep.clear();
                    textM = new Text(getContext());
                    textN = new Text(getContext());
                    secondtotalDl.removeComponentAt(0);
                    secondtotalDl.addComponent(initPiv(m, n), 0);
                    firstDl.removeComponentAt(0);
                    firstDl.addComponent(initFirstLeftDL(), 0);
                });
                dialog.setButton(IDialog.BUTTON2, "取消", (iDialog, i) -> {
                    iDialog.destroy();
                });
                dialog.show();
            }
        });
        DirectionalLayout showNumDl = new DirectionalLayout(getContext());
        showNumDl.setWidth(ComponentContainer.LayoutConfig.MATCH_PARENT);
        showNumDl.setHeight(ComponentContainer.LayoutConfig.MATCH_CONTENT);
        showNumDl.setOrientation(Component.HORIZONTAL);
        showNumDl.setMarginsTopAndBottom(50, 0);
        showNumDl.setAlignment(LayoutAlignment.CENTER);
        Text showNumText = new Text(getContext());
        showNumText.setText("显示数字");
        showNumText.setTextSize(50);
        Switch btnSwitch = new Switch(getContext());
        // 设置滑块样式
        ShapeElement elementThumbOn = new ShapeElement();
        elementThumbOn.setShape(ShapeElement.OVAL);
        elementThumbOn.setRgbColor(RgbColor.fromArgbInt(0xFF1E90FF));
        elementThumbOn.setCornerRadius(50);
        // 关闭状态下滑块的样式
        ShapeElement elementThumbOff = new ShapeElement();
        elementThumbOff.setShape(ShapeElement.OVAL);
        elementThumbOff.setRgbColor(RgbColor.fromArgbInt(0xFFFFFFFF));
        elementThumbOff.setCornerRadius(50);
        // 开启状态下轨迹样式
        ShapeElement elementTrackOn = new ShapeElement();
        elementTrackOn.setShape(ShapeElement.RECTANGLE);
        elementTrackOn.setRgbColor(RgbColor.fromArgbInt(0xFF87CEFA));
        elementTrackOn.setCornerRadius(50);
// 关闭状态下轨迹样式
        ShapeElement elementTrackOff = new ShapeElement();
        elementTrackOff.setShape(ShapeElement.RECTANGLE);
        elementTrackOff.setRgbColor(RgbColor.fromArgbInt(0xFF808080));
        elementTrackOff.setCornerRadius(50);
        btnSwitch.setChecked(true);
        btnSwitch.setTrackElement(trackElementInit(elementTrackOn, elementTrackOff));
        btnSwitch.setThumbElement(thumbElementInit(elementThumbOn, elementThumbOff));
        btnSwitch.setCheckedStateChangedListener(new AbsButton.CheckedStateChangedListener() {
            // 回调处理Switch状态改变事件
            @Override
            public void onCheckedChanged(AbsButton button, boolean isChecked) {
                showNum = isChecked;
                // 刷新组件
                pic = new ArrayList<>();
                step = 0;
                // state 记录华容道状态，state[i][j]=k,代表当前第i行，第j列块为第k块(1<=k<=m*n)，k=m*n代表为空块
                state = new int[m][n];
                historyStep.clear();
                textM = new Text(getContext());
                textN = new Text(getContext());
                secondtotalDl.removeComponentAt(0);
                secondtotalDl.addComponent(initPiv(m, n), 0);
                firstDl.removeComponentAt(0);
                firstDl.addComponent(initFirstLeftDL(), 0);
            }
        });
        showNumDl.addComponent(showNumText);
        showNumDl.addComponent(btnSwitch);
        Button buttonAm = new Button(getContext());
        layoutConfig.setMargins(0, 50, 0, 0);
        buttonAm.setLayoutConfig(layoutConfig);
        buttonAm.setText("动画设置");
        buttonAm.setTextSize(50);
        buttonAm.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                CommonDialog dialog = new CommonDialog(getContext());
                // 此处添加按钮被点击需要执行的操作
                Picker picker = new Picker(getContext());
                picker.setWidth(600);
                picker.setHeight(300);
                layoutConfig.setMargins(0, 50, 0, 0);
                picker.setLayoutConfig(layoutConfig);
                picker.setNormalTextSize(50);
                picker.setSelectedTextSize(70);
                ShapeElement shape = new ShapeElement();
                shape.setShape(ShapeElement.RECTANGLE);
                shape.setRgbColor(RgbColor.fromArgbInt(0xFF40E0D0));
                picker.setDisplayedLinesElements(shape, shape);
                String[] anm = {"无动画", "平移动画", "旋转动画"};
                picker.setDisplayedData(anm);
                int temp = animationMode;
                picker.setValueChangedListener((picker1, oldVal, newVal) -> {
                    // oldVal:上一次选择的值； newVal：最新选择的值
                    animationMode = newVal;
                });
                dialog.setTitleText(null);
                dialog.setMovable(false);
                dialog.setContentCustomComponent(picker);
                dialog.setSize(600, 600);
                dialog.setButton(IDialog.BUTTON1, "确认", (iDialog, i) -> {
                    iDialog.destroy();
                });
                dialog.setButton(IDialog.BUTTON2, "取消", (iDialog, i) -> {
                    animationMode = temp;
                    iDialog.destroy();
                });
                dialog.show();
            }
        });
        Button buttonBackOneStep = new Button(getContext());
        layoutConfig.setMargins(0, 50, 0, 0);
        buttonBackOneStep.setLayoutConfig(layoutConfig);
        buttonBackOneStep.setText("返回上一步");
        buttonBackOneStep.setTextSize(50);
        buttonBackOneStep.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                if(historyStep.empty()){
                    CommonDialog dialog = new CommonDialog(getContext());
                    dialog.setTitleText(null);
                    dialog.setMovable(true);
                    dialog.setSize(1000, 200);
                    dialog.setContentText("已经不能再后退了！！！");
                    dialog.setButton(IDialog.BUTTON1, "确认", (iDialog, i) -> {
                        iDialog.destroy();
                    });
                    dialog.show();
                }
                else{
                    swapButton(historyStep.pop().intValue());
                }
            }
        });
        Button buttonFinish = new Button(getContext());
        layoutConfig.setMargins(0, 50, 0, 0);
        buttonFinish.setLayoutConfig(layoutConfig);
        buttonFinish.setText("提前结束");
        buttonFinish.setTextSize(50);
        buttonFinish.setClickedListener(new Component.ClickedListener() {
            @Override
            // 在组件中增加对点击事件的检测
            public void onClick(Component component) {
                // 此处添加按钮被点击需要执行的操作
                CommonDialog dialog = new CommonDialog(getContext());
                dialog.setTitleText(null);
                dialog.setMovable(true);
                dialog.setSize(1000, 200);
                dialog.setContentText("真的不玩了吗？今天的还可以再挑战下哦");
                dialog.setButton(IDialog.BUTTON1, "确认退出", (iDialog, i) -> {
                    iDialog.destroy();
                    terminateAbility();
                });
                dialog.setButton(IDialog.BUTTON2, "取消", (iDialog, i) -> {
                    iDialog.destroy();
                });
                dialog.show();
            }
        });
        secondRightDl.addComponent(buttonRestart);
        secondRightDl.addComponent(buttonChoosePhoto);
        secondRightDl.addComponent(buttonChooseHard);
        secondRightDl.addComponent(showNumDl);
        secondRightDl.addComponent(buttonAm);
        secondRightDl.addComponent(buttonBackOneStep);
        secondRightDl.addComponent(buttonFinish);
        secondtotalDl.addComponent(initPiv(m, n));
        secondtotalDl.addComponent(secondRightDl);
        directionalLayout.addComponent(firstDl);
        directionalLayout.addComponent(secondtotalDl);
//
//        // 将布局作为根布局添加到视图树中
        super.setUIContent(directionalLayout);
    }

    private DirectionalLayout initFirstLeftDL() {
        firstleftDl = new DirectionalLayout(getContext());
        firstleftDl.setHeight(ComponentContainer.LayoutConfig.MATCH_PARENT);
        // 设置布局属性
        firstleftDl.setOrientation(Component.VERTICAL);
        firstleftDl.setPadding(32, 32, 32, 32);
        // 为组件添加对应布局的布局属性
        layoutConfig.alignment = LayoutAlignment.CENTER;
        Text text = new Text(getContext());
        text.setText("当前步数:" + String.valueOf(step));
        text.setTextSize(50);
        text.setId(100);
        text.setLayoutConfig(layoutConfig);
        // 计时
        DirectionalLayout time = new DirectionalLayout(getContext());
        // 设置布局大小
        time.setWidth(500);
        time.setHeight(100);
        // 设置布局属性
        time.setOrientation(Component.HORIZONTAL);
        time.setPadding(32, 32, 32, 20);
        timer = new TickTimer(this);
        timer.setCountDown(false);
        timer.setTextSize(50);
        timer.start();
        Text text1 = new Text(getContext());
        text1.setText("耗时");
        text1.setTextSize(50);
        text1.setId(100);
        time.setLayoutConfig(layoutConfig);
        time.addComponent(text1);
        time.addComponent(timer);
        Text text2 = new Text(getContext());
        text2.setText("当前难度：" + String.valueOf(m) + "X" + String.valueOf(n));
        text2.setTextSize(50);
        text2.setId(100);
        text2.setLayoutConfig(layoutConfig);
        firstleftDl.addComponent(text);
        firstleftDl.addComponent(time);
        firstleftDl.addComponent(text2);
        return firstleftDl;
    }

    private TableLayout initPiv(int m, int n) {
        TableLayout secondDl = new TableLayout(getContext());
        secondDl.setRowCount(m);
        secondDl.setColumnCount(n);
        PixelMap picPM = pixelMap;
        // 设置布局大小
        secondDl.setWidth(700);
        secondDl.setHeight(ComponentContainer.LayoutConfig.MATCH_CONTENT);
        // 设置布局属性
        secondDl.setOrientation(Component.HORIZONTAL);
        secondDl.setPadding(32, 32, 0, 32);
        // 初始化pic
        int unitWidth, unitHeight;
        ImageInfo info = picPM.getImageInfo();
        unitWidth = (int) info.size.width / n;
        unitHeight = (int) info.size.height / m;
        int minsz = Math.min(unitHeight, unitWidth);
        for (int i = 0; i < this.m; i++) {
            for (int ii = 0; ii < this.n; ii++) {
                // 普通解码叠加缩放、裁剪、旋转
                ImageSource.DecodingOptions decodingOpts = new ImageSource.DecodingOptions();
                decodingOpts.desiredRegion = new Rect(ii * unitWidth, i * unitHeight, unitWidth, unitHeight);
                PixelMap unitPM = imageSource.createPixelmap(decodingOpts);
                if (showNum) {
                    Canvas canvas = new Canvas(new Texture(unitPM));
                    Paint paint = new Paint();
                    paint.setTextSize(minsz / 2);
                    paint.setColor(Color.RED);
                    canvas.drawText(paint, "" + (i * n + ii + 1), unitWidth / 2-minsz/5, unitHeight / 2);
                }
                // 在指定区域写入像素
                PixelMapElement unitPML = new PixelMapElement(unitPM);
                state[i][ii] = i * n + ii + 1;
                pic.add(unitPML);
            }
        }
        // 洗牌，然后修改secondDl
        shuffle();
        originState = new ArrayList<>();
        for (int i = 0; i < this.m; i++) {
            for (int ii = 0; ii < this.n; ii++) {
                Button temp = new Button(getContext());
                temp.setWidth(unitWidth);
                temp.setHeight(unitHeight);
//                temp.setText(String.valueOf(i * n + ii + 1));
                temp.setTextSize(100);
                temp.setId(100);
                // 为按钮设置点击事件回调
                int finalI = i;
                int finalIi = ii;
                originState.add(state[i][ii] - 1);
                temp.setClickedListener(new Component.ClickedListener() {
                    @Override
                    public void onClick(Component component) {
                        if (swapButton(originState.get(finalI * n + finalIi) + 1)) {
                            step++;
                            Text text = (Text) firstleftDl.getComponentAt(0);
                            text.setText("当前步数:" + String.valueOf(step));
                            Integer a = Integer.valueOf(originState.get(finalI * n + finalIi) + 1);
                            historyStep.push(a);
                            // 检查结果是否成功
                            boolean flag = true;
                            for (int k = 0; k < n; k++) {
                                for (int kk = 0; kk < n; kk++) {
                                    if (state[k][kk] != k * n + kk + 1) {
                                        flag = false;
                                        break;
                                    }
                                }
                            }
                            if (flag) {
                                CommonDialog dialog = new CommonDialog(getContext());
                                dialog.setTitleText(null);
                                dialog.setMovable(true);
                                dialog.setSize(1000, 1200);
                                DirectionalLayout dl = new DirectionalLayout(getContext());
                                dl.setWidth(1000);
                                dl.setHeight(900);
                                Image image = new Image(getContext());
                                image.setScaleMode(Image.ScaleMode.ZOOM_CENTER);
                                image.setPixelMap(ResourceTable.Media_flower);
                                image.setLayoutConfig(layoutConfig);

                                Text textSuccess = new Text(getContext());
                                textSuccess.setHeight(100);
                                textSuccess.setText("成功");
                                text.setTextSize(100);
                                Text textScore = new Text(getContext());
                                textSuccess.setHeight(100);
                                // 得分根据步数和用时计算
                                String time = timer.getText();
                                int second = Integer.parseInt(time.substring(0,2))+Integer.parseInt(time.substring(3,5));
                                int Score = 1000*((500-step)+(600-second));
                                if (Score<0){
                                    Score = 0;
                                }
                                textScore.setText("得分"+String.valueOf(Score));
                                text.setTextSize(100);

                                dl.addComponent(image);
                                dl.addComponent(textSuccess);
                                dl.addComponent(textScore);
                                dialog.setContentCustomComponent(dl);
                                dialog.setButton(IDialog.BUTTON1, "确认", (iDialog, i) -> {
                                    iDialog.destroy();
                                });
                                dialog.setButton(IDialog.BUTTON2, "取消", (iDialog, i) -> {
                                    iDialog.destroy();
                                });
                                Context context = getApplicationContext();
                                DatabaseHelper databaseHelper = new DatabaseHelper(context); // context入参类型为ohos.app.Context。
                                String fileName = "test_pref"; // fileName表示文件名，其取值不能为空，也不能包含路径，默认存储目录可以通过context.getPreferencesDir()获取。
                                Preferences preferences = databaseHelper.getPreferences(fileName);
//                                DateTime now = DateTime.now();
//zx
//
                                int value = preferences.getInt("latest", 0);
                                preferences.putString("StringKey"+String.valueOf(value), "得分"+String.valueOf(Score)+",步数"+String.valueOf(step)+",用时"+time);
                                preferences.putInt("latest",value+1);
                                preferences.flushSync();  // 同步方法
//                                BufferedWriter out = null;
//                                try {
//                                    out = new BufferedWriter(new FileWriter("runoob.log"));
//                                    out.write("菜鸟教程");
//                                    out.close();
//                                    System.out.println("文件创建成功！");
//
//                                } catch (IOException e) {
//                                    throw new RuntimeException(e);
//                                }
//
//                                try {
//                                    BufferedReader in = new BufferedReader(new FileReader("runoob.log"));
//                                    String str;
//                                    while ((str = in.readLine()) != null) {
//                                        System.out.println(str);
//                                    }
//                                    System.out.println(str);
//                                } catch (IOException ignored) {
//                                }
                                dialog.show();
                            }
                        }
                    }
                });
                if (state[i][ii] != m * n) {
                    temp.setBackground(pic.get(state[i][ii] - 1));
                }
                temp.setMarginsLeftAndRight(3,3);
                temp.setMarginsTopAndBottom(5,5);
                secondDl.addComponent(temp);
            }
        }
        return secondDl;
    }

    private void selectPhoto() {
        //调起系统的选择来源数据视图
        Intent intent = new Intent();
        Operation opt = new Intent.OperationBuilder().withAction("android.intent.action.GET_CONTENT").build();
        intent.setOperation(opt);
        intent.addFlags(Intent.FLAG_NOT_OHOS_COMPONENT);
        intent.setType("image/*");
        startAbilityForResult(intent, imgRequestCode);
    }

    /*选择图片回调*/
    @Override
    protected void onAbilityResult(int requestCode, int resultCode, Intent resultData) {
        if (requestCode == imgRequestCode && resultData != null) {
            //选择的Img对应的Uri
            String chooseImgUri = resultData.getUriString();
            //定义数据能力帮助对象
            DataAbilityHelper helper = DataAbilityHelper.creator(getContext());
            //定义图片来源对象
            //获取选择的Img对应的Id
            String chooseImgId = null;
            //如果是选择文件则getUriString结果为dataability:///com.android.providers.media.documents/document/image%3A437，其中%3A437是":"的URL编码结果，后面的数字就是image对应的Id
            //如果选择的是图库则getUriString结果为dataability:///media/external/images/media/262，最后就是image对应的Id
            //这里需要判断是选择了文件还是图库
            if (chooseImgUri.lastIndexOf("%3A") != -1) {
                chooseImgId = chooseImgUri.substring(chooseImgUri.lastIndexOf("%3A") + 3);
            } else {
                chooseImgId = chooseImgUri.substring(chooseImgUri.lastIndexOf('/') + 1);
            }
            //获取图片对应的uri，由于获取到的前缀是content，我们替换成对应的dataability前缀
            Uri uri = Uri.appendEncodedPathToUri(AVStorage.Images.Media.EXTERNAL_DATA_ABILITY_URI, chooseImgId);
            try {
                //读取图片
                FileDescriptor fd = helper.openFile(uri, "r");
                imageSource = ImageSource.create(fd, null);
                // 普通解码叠加缩放、裁剪、旋转
                ImageSource.DecodingOptions decodingOpts = new ImageSource.DecodingOptions();
                pixelMap = imageSource.createPixelmap(decodingOpts);
                ImageInfo info = pixelMap.getImageInfo();
                // 如果应该缩放以宽为依据，false,
                boolean longerEdge = info.size.height * 675 > info.size.width * 910;
                // if(maxsz>300)
                if (longerEdge) {
                    decodingOpts.desiredSize = new Size(info.size.width * 910 / info.size.height, 910);
                } else {
                    decodingOpts.desiredSize = new Size(675, info.size.height * 675 / info.size.width);
                }
                //创建位图
                pixelMap = imageSource.createPixelmap(decodingOpts);
                imageSource.release();
                // 保存临时图片，根据路径来读取imageSource
                imageSource = saveImageToLibrary("temp.JPEG", pixelMap);
                Image temp = (Image) firstDl.getComponentAt(1);
                temp.setPixelMap(pixelMap);
                pic = new ArrayList<>();
                step = 0;
                // state 记录华容道状态，state[i][j]=k,代表当前第i行，第j列块为第k块(1<=k<=m*n)，k=m*n代表为空块
                state = new int[m][n];
                historyStep.clear();
                textM = new Text(getContext());
                textN = new Text(getContext());
                secondtotalDl.removeComponentAt(0);
                secondtotalDl.addComponent(initPiv(m, n), 0);
                firstDl.removeComponentAt(0);
                firstDl.addComponent(initFirstLeftDL(), 0);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private ImageSource saveImageToLibrary(String fileName, PixelMap pixelMap) {
        try {
            ValuesBucket valuesBucket = new ValuesBucket();
            //文件名
            valuesBucket.putString(AVStorage.Images.Media.DISPLAY_NAME, fileName);
            //相对路径
            valuesBucket.putString("relative_path", "DCIM/");
            //文件格式，类型要一定要注意要是JPEG，PNG类型不支持
            valuesBucket.putString(AVStorage.Images.Media.MIME_TYPE, "image/JPEG");
            //应用独占：is_pending设置为1时表示只有该应用能访问此图片，其他应用无法发现该图片，当图片处理操作完成后再吧is_pending设置为0，解除独占，让其他应用可见
            valuesBucket.putInteger("is_pending", 1);
            //鸿蒙的helper.insert方法和安卓的contentResolver.insert方法有所不同，安卓方法直接返回一个uri，我们就可以拿来直接操作，而鸿蒙方法返回官方描述是Returns the index of the inserted data record（返回插入的数据记录的索引），这个index我的理解就是id，因此，我们需要自己在后面拼出文件的uri再进行操作
            DataAbilityHelper helper = DataAbilityHelper.creator(this);
            int index = helper.insert(AVStorage.Images.Media.EXTERNAL_DATA_ABILITY_URI, valuesBucket);
            Uri uri = Uri.appendEncodedPathToUri(AVStorage.Images.Media.EXTERNAL_DATA_ABILITY_URI, String.valueOf(index));
            //获取到uri后，安卓通过contentResolver.openOutputStream(uri)就能获取到输出流来写文件，而鸿蒙没有提供这样的方法，我们就只能通过uri获取FileDescriptor，再通过FileDescriptor生成输出流打包编码成新的图片文件，这里helper.openFile方法一定要有“w”写模式，不然会报FileNotFound的错误。
            FileDescriptor fd = helper.openFile(uri, "w");
            ImagePacker imagePacker = ImagePacker.create();
            ImagePacker.PackingOptions packingOptions = new ImagePacker.PackingOptions();
            OutputStream outputStream = new FileOutputStream(fd);
            packingOptions.format = "image/jpeg";
            packingOptions.quality = 90;
            boolean result = imagePacker.initializePacking(outputStream, packingOptions);
            if (result) {
                result = imagePacker.addImage(pixelMap);
                if (result) {
                    long dataSize = imagePacker.finalizePacking();
                }
            }
            outputStream.flush();
            outputStream.close();
            valuesBucket.clear();
            //解除独占
            valuesBucket.putInteger("is_pending", 0);
            helper.update(uri, valuesBucket, null);
            FileDescriptor fd1 = helper.openFile(uri, "r");
            return ImageSource.create(fd1, null);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    //
    void shuffle() {
        for (int i = 0; i < 500; i++) {
            int I = 0, Ii = 0;
            for (int j = 0; j < m; j++) {
                for (int jj = 0; jj < n; jj++) {
                    if (state[j][jj] == m * n) {
                        I = j;
                        Ii = jj;
                        break;
                    }
                }
            }
            // 生成随机数，调整I,Ii
            Random r = new Random();
            int random1 = r.nextInt(4); // 生成[0,3]区间的整数
            if (random1 == 0) {
                if (I - 1 >= 0) {
                    state[I][Ii] = state[I - 1][Ii];
                    state[I - 1][Ii] = (m) * (n);
                }
            } else if (random1 == 1) {
                if (I + 1 < m) {
                    state[I][Ii] = state[I + 1][Ii];
                    state[I + 1][Ii] = (m) * (n);
                }
            } else if (random1 == 2) {
                if (Ii - 1 >= 0) {
                    state[I][Ii] = state[I][Ii - 1];
                    state[I][Ii - 1] = (m) * (n);
                }
            } else {
                if (Ii + 1 < n) {
                    state[I][Ii] = state[I][Ii + 1];
                    state[I][Ii + 1] = (m) * (n);
                }
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
// secondtotalDl.getComponentAt(0).getComponentAt(0).getComponentAt(1).getContentPositionX()
