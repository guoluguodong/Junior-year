package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;
import java.io.IOException;

public class DeleteFileStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
//      参数 待删除文件的名称
        String fileName = args[0];
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        String filePath = curPath + File.separator + fileName;
        File fileToDelete = new File(filePath);

        if (fileToDelete.exists()) {
            if (fileToDelete.delete()) {
                System.out.println("文件已成功删除：" + filePath);
            } else {
                System.out.println("无法删除文件：" + filePath);
            }
        } else {
            System.out.println("文件不存在：" + filePath);
        }
    }
}
