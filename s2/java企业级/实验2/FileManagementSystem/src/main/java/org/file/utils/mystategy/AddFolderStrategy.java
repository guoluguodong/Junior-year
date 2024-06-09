package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;

public class AddFolderStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
//      参数 创建文件夹的名称
        String folderName = args[0];
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        String newFolderPath = curPath + File.separator + folderName;
        File newFolder = new File(newFolderPath);

        if (!newFolder.exists()) { // 检查文件夹是否已经存在
            boolean created = newFolder.mkdir(); // 创建文件夹
            if (created) {
                System.out.println("文件夹创建成功：" + newFolder.getAbsolutePath());
            } else {
                System.out.println("文件夹创建失败：" + newFolder.getAbsolutePath());
            }
        } else {
            System.out.println("文件夹已经存在：" + newFolder.getAbsolutePath());
        }
    }
}
