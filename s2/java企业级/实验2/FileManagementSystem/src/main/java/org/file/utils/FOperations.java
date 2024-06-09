package org.file.utils;

import org.file.utils.myinterface.FOperationInterface;
import org.file.utils.mystategy.*;

import java.util.HashMap;
import java.util.Map;

public class FOperations {
    public void setStrategy(FOperationInterface strategy,String[] args) {
        strategy.execute(args);
    }
    public static Map<String, Integer> map = new HashMap<>();

    public static void main(String[] args) {
        map.put("AES", 128);
        map.put("DES", 56);
        PathManager pathManager = PathManager.getInstance();
        System.out.println("当前路径：" + pathManager.getCurrentPath());

        pathManager.setCurrentPath("C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem");
        System.out.println("当前路径：" + pathManager.getCurrentPath());

        FOperations fOperations = new FOperations();
        fOperations.setStrategy(new AddFolderStrategy(),new String[]{"file"});
//        folderOperations.setStrategy(new DeleteFolderStrategy(),new String[]{"file"});
//        fOperations.setStrategy(new RenameFolderStrategy(),new String[]{"file","newfile"});
//        fOperations.setStrategy(new ListFolderStrategy(),new String[]{"",""});
//        fOperations.setStrategy(new AddFileStrategy(),new String[]{"a.txt"});
//        fOperations.setStrategy(new AddFileStrategy(),new String[]{"b.txt"});
//        fOperations.setStrategy(new DeleteFileStrategy(),new String[]{"a.txt"});
//        fOperations.setStrategy(new RenameFileStrategy(),new String[]{"b.txt","c.txt"});
//        fOperations.setStrategy(new CopyFileStrategy(),new String[]{"C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\file\\20215279李宽宇1.doc","C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\newfile\\2024版.doc"});
//        fOperations.setStrategy(new CopyFolderStrategy(),new String[]{"C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\file","C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\newfile"});
//        CopyFolderThread thread = new CopyFolderThread(new String[]{"C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\file","C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\newfile"});
//        // 启动线程
//        thread.start();
//        System.out.println("请输入：" );
//        fOperations.setStrategy(new EncryptFileStrategy(),new String[]{"a.txt","sec.txt","key.txt","AES", String.valueOf(map.get("AES"))});
//        fOperations.setStrategy(new DecryptFileStrategy(),new String[]{"sec.txt","a_de.txt","key.txt","AES"});
//          fOperations.setStrategy(new ReadTxtStrategy(),new String[]{"a.txt"});
//        fOperations.setStrategy(new CompressFStrategy(),new String[]{"C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\file\\file\\20215279李宽宇1.doc","C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\20215279李宽宇1.zip"});
//        fOperations.setStrategy(new DecompressFStrategy(),new String[]{"C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\20215279李宽宇1.zip","C:\\Users\\a3840\\Desktop\\ScoreManagementSystem\\FileManagementSystem\\file2"});

    }

}


