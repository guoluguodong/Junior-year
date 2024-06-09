package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class ReadTxtStrategy  implements FOperationInterface {
    @Override
    public void execute(String[] args) {
//      参数 读取文件的名称
        String fileName = args[0];
        PathManager pathManager = PathManager.getInstance();
        String curPath = pathManager.getCurrentPath();
        String filePath = curPath + File.separator + fileName;
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            // 逐行读取文本文件内容并打印到控制台
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
