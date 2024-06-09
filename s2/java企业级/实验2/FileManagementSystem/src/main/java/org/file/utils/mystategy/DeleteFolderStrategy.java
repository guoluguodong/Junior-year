package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;

public class DeleteFolderStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
//      参数 删除文件夹的名称
        String fileName = args[0];
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        String newFilePath = curPath + File.separator + fileName;
        File fileToDelete = new File(newFilePath);


        if (fileToDelete.exists() && fileToDelete.isDirectory()) { // 检查文件夹是否存在且为文件夹类型
            boolean deleted = deleteFolder(fileToDelete); // 调用删除文件夹的方法
            if (deleted) {
                System.out.println("文件夹删除成功：" + fileToDelete.getAbsolutePath());
            } else {
                System.out.println("文件夹删除失败：" + fileToDelete.getAbsolutePath());
            }
        } else {
            System.out.println("要删除的文件夹不存在或不是文件夹：" + fileToDelete.getAbsolutePath());
        }
    }

    /**
     * 递归删除文件夹及其内容
     *
     * @param folder 要删除的文件夹
     * @return true if deletion is successful, false otherwise
     */
    private boolean deleteFolder(File folder) {
        if (folder.isDirectory()) {
            File[] files = folder.listFiles();
            if (files != null) {
                for (File file : files) {
                    deleteFolder(file); // 递归删除文件夹中的内容
                }
            }
        }
        return folder.delete(); // 删除空文件夹或文件
    }
}
