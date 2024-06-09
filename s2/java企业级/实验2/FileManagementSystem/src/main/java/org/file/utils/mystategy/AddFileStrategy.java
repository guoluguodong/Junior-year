package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;
import java.io.IOException;

public class AddFileStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
//      参数 创建文件夹的名称
        String fileName = args[0];
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        String newFilePath = curPath + File.separator + fileName;
        File newFile = new File(newFilePath);
        try {
            if (newFile.createNewFile()) {
                System.out.println("文件已创建：" + newFile.getAbsolutePath());
            } else {
                System.out.println("文件已存在：" + newFile.getAbsolutePath());
            }
        } catch (IOException e) {
            System.out.println("创建文件时出现异常：" + e.getMessage());
        }
    }
}
