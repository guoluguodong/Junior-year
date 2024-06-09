package org.file.utils.mystategy;

import org.file.utils.FOperations;
import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;
import org.file.utils.mystategy.CopyFileStrategy;

import java.io.File;

public class CopyFolderStrategyBackground implements FOperationInterface {
    @Override
    public void execute(String[] args) {
        String sourceFolderPath = args[0];
        String destinationFolderPath = args[1];
        PathManager pathManager = PathManager.getInstance();
        File sourceFolder = new File(sourceFolderPath);
        File destinationFolder = new File(destinationFolderPath);
        if (!destinationFolder.exists()) {
            destinationFolder.mkdirs();
        }
        copyFolder(sourceFolder, destinationFolder);
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
            return false;
        }

        if (!destinationFolder.exists()) {
            boolean created = destinationFolder.mkdirs();
            if (!created) {
                return false;
            }
        }

        File[] files = sourceFolder.listFiles();
        if (files == null) {
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
                fOperations.setStrategy(new CopyFileStrategyBackground(),new String[]{file.getAbsolutePath(),destination.getAbsolutePath()});
            }
        }

        return true; // All files and subfolders copied successfully
    }

}

