package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;
import java.io.IOException;

public class RenameFileStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
        if (args.length < 2) {
            System.out.println("请输入要重命名的文件名称和新名称。");
            return;
        }
        // 参数 原文件名和新文件名
        String oldFileName = args[0];
        String newFileName = args[1];
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        String oldFilePath = curPath + File.separator + oldFileName;
        String newFilePath = curPath + File.separator + newFileName;
        File oldFile = new File(oldFilePath);
        File newFile = new File(newFilePath);

        if (oldFile.exists()) {
            if (oldFile.renameTo(newFile)) {
                System.out.println("文件重命名成功：" + newFilePath);
            } else {
                System.out.println("无法重命名文件：" + oldFilePath);
            }
        } else {
            System.out.println("文件不存在：" + oldFilePath);
        }
    }
}
