package org.file.utils.mystategy;

import org.file.utils.FOperations;
import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;
import org.file.utils.mystategy.CopyFileStrategy;

import java.io.File;

public class CopyFolderStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
        String sourceFolderPath = args[0];
        String destinationFolderPath = args[1];
        PathManager pathManager = PathManager.getInstance();
        File sourceFolder = new File(sourceFolderPath);
        File destinationFolder = new File(destinationFolderPath);
        if(!destinationFolder.exists()){
            destinationFolder.mkdirs();
        }

        if (sourceFolder.exists() && sourceFolder.isDirectory()&&destinationFolder.exists() && destinationFolder.isDirectory()) { // 检查文件夹是否存在且为文件夹类型
            boolean copied = copyFolder(sourceFolder,destinationFolder);
            if (copied) {
                System.out.println(sourceFolder.getAbsolutePath()+" 文件夹拷贝成功：" +destinationFolder.getAbsolutePath() );
            } else {
                System.out.println(sourceFolder.getAbsolutePath()+" 文件夹拷贝失败：" +destinationFolder.getAbsolutePath() );
            }
        } else {
            System.out.println("要拷贝的文件夹: "+sourceFolder.getAbsolutePath()+",或者目的文件夹: " +destinationFolder.getAbsolutePath()+"不存在或不是文件夹" );
        }
    }

    /**
     * 递归拷贝文件夹及其内容
     *
     * @param sourceFolder 要拷贝的文件夹，
     * @param  destinationFolder 目的地文件夹
     * @return true if deletion is successful, false otherwise
     */
    private boolean copyFolder(File sourceFolder, File destinationFolder) {
        if (!sourceFolder.exists() || !sourceFolder.isDirectory()) {
            System.out.println("Source folder does not exist or is not a directory.");
            return false;
        }

        if (!destinationFolder.exists()) {
            boolean created = destinationFolder.mkdirs();
            if (!created) {
                System.out.println("创建目标文件夹失败");
                return false;
            }
        }

        File[] files = sourceFolder.listFiles();
        if (files == null) {
            System.out.println("源文件夹不存在");
            return false;
        }

        for (File file : files) {
            File destination = new File(destinationFolder, file.getName());
            if (file.isDirectory()) {
                // Recursively copy subfolders and their contents
                boolean success = copyFolder(file, destination);
                if (!success) {
                    return false; // Stop copying if any subfolder fails
                }
            } else {
                // Copy file
                FOperations fOperations = new FOperations();
                fOperations.setStrategy(new CopyFileStrategy(),new String[]{file.getAbsolutePath(),destination.getAbsolutePath()});
            }
        }

        return true; // All files and subfolders copied successfully
    }

}

