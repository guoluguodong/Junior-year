package org.file.utils.mystategy;

import org.file.utils.myinterface.FOperationInterface;

import java.io.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;
import java.nio.file.Files;

public class DecompressFStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
        String compressedFilePath = args[0];
        String targetFilePath = args[1];
        decompressFolder(compressedFilePath, targetFilePath);
    }


    public static void decompressFolder(String compressedFilePath, String targetFilePath) {
        File targetFolder = new File(targetFilePath);
        if (!targetFolder.exists()) {
            targetFolder.mkdirs();
        }

        try (ZipInputStream zipInputStream = new ZipInputStream(new FileInputStream(compressedFilePath))) {
            ZipEntry entry;
            while ((entry = zipInputStream.getNextEntry()) != null) {
                String entryName = entry.getName();
                // 使用\\分割字符串，并存储到字符数组中
                String[] pathComponents = entryName.split("/");
                String curPath = targetFilePath;
                File entryFile;
                for (int i = 0; i < pathComponents.length - 1; i++) {
                    curPath += File.separator + pathComponents[i];
                    entryFile = new File(curPath);
                    if (!entryFile.exists()) {
                        entryFile.mkdirs();
                    }
                }
                try (FileOutputStream outputStream = new FileOutputStream(targetFilePath + File.separator + entryName)) {
                    byte[] buffer = new byte[1024];
                    int length;
                    while ((length = zipInputStream.read(buffer)) > 0) {
                        outputStream.write(buffer, 0, length);
                    }
                }
                zipInputStream.closeEntry();
            }
            System.out.println("文件解压缩成功");
        } catch (IOException e) {
            System.out.println("在解压文件中失败： " + e.getMessage());
        }
    }


}