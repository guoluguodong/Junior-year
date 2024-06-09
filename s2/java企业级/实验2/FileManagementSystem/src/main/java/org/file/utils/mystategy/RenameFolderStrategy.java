package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;

public class RenameFolderStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
        // 参数 旧文件夹名称和新文件夹名称
        String oldFolderName = args[0];
        String newFolderName = args[1]; // Assuming the second argument is the new folder name
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();

        String oldFolderPath = curPath + File.separator + oldFolderName;
        String newFolderPath = curPath + File.separator + newFolderName;

        File oldFolder = new File(oldFolderPath);
        File newFolder = new File(newFolderPath);

        if (oldFolder.exists() && oldFolder.isDirectory()) { // 检查旧文件夹是否存在且为文件夹类型
            boolean renamed = oldFolder.renameTo(newFolder); // 使用File类的renameTo方法重命名文件夹
            if (renamed) {
                System.out.println("文件夹重命名成功：" + newFolder.getAbsolutePath());
            } else {
                System.out.println("文件夹重命名失败：" + oldFolder.getAbsolutePath());
            }
        } else {
            System.out.println("要重命名的文件夹不存在或不是文件夹：" + oldFolder.getAbsolutePath());
        }
    }
}
