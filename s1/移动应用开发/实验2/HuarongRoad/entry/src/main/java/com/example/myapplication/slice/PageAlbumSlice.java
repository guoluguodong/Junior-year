package com.example.myapplication.slice;

import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.ability.DataAbilityHelper;
import ohos.aafwk.content.Intent;
import ohos.aafwk.content.Operation;
import ohos.agp.components.Button;
import ohos.agp.components.Component;
import ohos.agp.components.Image;
import ohos.data.rdb.ValuesBucket;
import ohos.global.resource.NotExistException;
import ohos.media.image.ImagePacker;
import ohos.media.image.ImageSource;
import ohos.media.image.PixelMap;
import ohos.media.photokit.metadata.AVStorage;
import ohos.utils.net.Uri;

import java.io.*;

public class PageAlbumSlice extends AbilitySlice {
    int imgRequestCode;
    Image photo;
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setUIContent(ResourceTable.Layout_ability_album);
        String[] permissions = {"ohos.permission.READ_USER_STORAGE", "ohos.permission.WRITE_USER_STORAGE"};
        requestPermissionsFromUser(permissions, 0);
        Button saveButton = (Button) findComponentById(ResourceTable.Id_save_photo);
        photo = (Image) findComponentById(ResourceTable.Id_show_photo);
        saveButton.setClickedListener(new Component.ClickedListener() {
            @Override
            public void onClick(Component component) {
                // 此处添加点击按钮后的事件处理逻辑
                saveImageToLibrary("yll.JPEG",getPixelMap(ResourceTable.Media_1));
            }
        });
        Button selectButton = (Button) findComponentById(ResourceTable.Id_select_photo);
        selectButton.setClickedListener(new Component.ClickedListener() {
            @Override
            public void onClick(Component component) {
                // 此处添加点击按钮后的事件处理逻辑
                selectPhoto();
            }
        });
    }
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
    //保存图片到相册 fileName文件名  PixelMap 图片数据
    private void saveImageToLibrary(String fileName, PixelMap pixelMap) {
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
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //选择图片
    private void selectPhoto() {
        //调起系统的选择来源数据视图
        Intent intent = new Intent();
        Operation opt=new Intent.OperationBuilder().withAction("android.intent.action.GET_CONTENT").build();
        intent.setOperation(opt);
        intent.addFlags(Intent.FLAG_NOT_OHOS_COMPONENT);
        intent.setType("image/*");
        startAbilityForResult(intent, imgRequestCode);
    }
    /*选择图片回调*/
    @Override
    protected void onAbilityResult(int requestCode, int resultCode, Intent resultData) {
        if(requestCode==imgRequestCode && resultData!=null)
        {
            //选择的Img对应的Uri
            String chooseImgUri=resultData.getUriString();
            //定义数据能力帮助对象
            DataAbilityHelper helper=DataAbilityHelper.creator(getContext());
            //定义图片来源对象
            ImageSource imageSource = null;
            //获取选择的Img对应的Id
            String chooseImgId=null;
            //如果是选择文件则getUriString结果为dataability:///com.android.providers.media.documents/document/image%3A437，其中%3A437是":"的URL编码结果，后面的数字就是image对应的Id
            //如果选择的是图库则getUriString结果为dataability:///media/external/images/media/262，最后就是image对应的Id
            //这里需要判断是选择了文件还是图库
            if(chooseImgUri.lastIndexOf("%3A")!=-1){
                chooseImgId = chooseImgUri.substring(chooseImgUri.lastIndexOf("%3A")+3);
            }
            else {
                chooseImgId = chooseImgUri.substring(chooseImgUri.lastIndexOf('/')+1);
            }
            //获取图片对应的uri，由于获取到的前缀是content，我们替换成对应的dataability前缀
            Uri uri=Uri.appendEncodedPathToUri(AVStorage.Images.Media.EXTERNAL_DATA_ABILITY_URI,chooseImgId);
            try {
                //读取图片
                FileDescriptor fd = helper.openFile(uri, "r");
                imageSource = ImageSource.create(fd, null);
                //创建位图
                PixelMap pixelMap = imageSource.createPixelmap(null);
                //设置图片控件对应的位图
                photo.setPixelMap(pixelMap);
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (imageSource != null) {
                    imageSource.release();
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
