package org.file.utils.mystategy;

import org.file.utils.CenteredCommandLineTable;
import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Date;

public class ListFolderStrategy implements FOperationInterface {
    @Override
    public void execute(String[] args) {

        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        File curFolder = new File(curPath);
        String filter=args[0];
        String sortBy=args[1];
        ArrayList<String []> table = new ArrayList<>();
        if (curFolder.exists() && curFolder.isDirectory()) { // 检查文件夹是否存在且为文件夹类型
            File[] filesAndDirs = curFolder.listFiles(); // 获取文件夹中的文件和文件夹数组
            if (filesAndDirs != null) {
                System.out.println("文件夹内容：");
                Arrays.sort(filesAndDirs, getComparator(sortBy));
                for (File file : filesAndDirs) {
                    if (file.getName().contains(filter)) {
                        table.add(formatFileDetails(file));
                    }
                }
                String [][] tableString = new String[table.size()][4];
                for (int i=0;i<table.size();++i) {
                    System.arraycopy(table.get(i), 0, tableString[i], 0, 4);
                }
                CenteredCommandLineTable centeredCommandLineTable = new CenteredCommandLineTable(tableString);
                centeredCommandLineTable.draw();
            } else {
                System.out.println("文件夹为空：" + curFolder.getAbsolutePath());
            }
        } else {
            System.out.println("要列出内容的文件夹不存在或不是文件夹：" + curFolder.getAbsolutePath());
        }
    }
    private String[] formatFileDetails(File file) {
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String type = file.isDirectory() ? "Directory" : "File";
        String size = file.isDirectory() ? "-" : String.valueOf(file.length())+"字节";
        String date = dateFormat.format(new Date(file.lastModified()));
        return new String[] {file.getName(), type, size, date};
    }
    private Comparator<File> getComparator(String sortBy) {
        Comparator<File> comparator = Comparator.comparing(File::getName);
        switch (sortBy) {
            case "name":
                comparator = Comparator.comparing(File::getName);
                break;
            case "size":
                comparator = Comparator.comparingLong(File::length);
                break;
            case "date":
                comparator = Comparator.comparingLong(File::lastModified);
                break;
        }
        return comparator;
    }
}
