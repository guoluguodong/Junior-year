package org.file.utils.mystategy;

import org.file.utils.myinterface.FOperationInterface;

import java.io.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class CompressFStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {
        String sourceFilePath = args[0];
        String compressedFilePath = args[1];
        File sourceFile = new File(sourceFilePath);
        if (sourceFile.isFile()) {
            compressFile(sourceFilePath, compressedFilePath);
        } else if (sourceFile.isDirectory()) {
            compressFolder(sourceFilePath, compressedFilePath);
        } else {
            System.out.println("Invalid path.");
        }

    }



    public static void compressFile(String sourceFile, String compressedFile) {
        try (FileInputStream fis = new FileInputStream(sourceFile);
             FileOutputStream fos = new FileOutputStream(compressedFile);
             ZipOutputStream zipOut = new ZipOutputStream(fos)) {

            ZipEntry zipEntry = new ZipEntry(new File(sourceFile).getName());
            zipOut.putNextEntry(zipEntry);

            byte[] bytes = new byte[1024];
            int length;
            while ((length = fis.read(bytes)) >= 0) {
                zipOut.write(bytes, 0, length);
            }

            System.out.println("文件成功压缩");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void compressFolder(String sourceFolder, String compressedFile) {
        try {
            FileOutputStream fos = new FileOutputStream(compressedFile);
            ZipOutputStream zipOut = new ZipOutputStream(fos);
            File folderToZip = new File(sourceFolder);

            zipFolder(folderToZip, folderToZip.getName(), zipOut);

            zipOut.close();
            fos.close();

            System.out.println("文件夹成功压缩");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void zipFolder(File folder, String parentFolder, ZipOutputStream zipOut) throws IOException {
        for (File file : folder.listFiles()) {
            if (file.isDirectory()) {
                zipFolder(file, parentFolder + "/" + file.getName(), zipOut);
                continue;
            }

            FileInputStream fis = new FileInputStream(file);
            ZipEntry zipEntry = new ZipEntry(parentFolder + "/" + file.getName());
            zipOut.putNextEntry(zipEntry);

            byte[] bytes = new byte[1024];
            int length;
            while ((length = fis.read(bytes)) >= 0) {
                zipOut.write(bytes, 0, length);
            }

            fis.close();
        }
    }
}