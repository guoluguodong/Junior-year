package org.file.utils;

import org.file.utils.myinterface.CopyFolderThreadCallback;
import org.file.utils.mystategy.CopyFolderStrategyBackground;
import java.util.concurrent.CompletableFuture;
// 创建一个继承自Thread类的自定义线程类
public class CopyFolderThread extends Thread {
    private String sourceFolderPath;
    private String destinationFolderPath;
    private CopyFolderThreadCallback callback;
    public CopyFolderThread(String[] args,CopyFolderThreadCallback callback) {
        if (args.length < 2) {
            System.err.println("Usage: CopyFolderThread <sourceFilePath> <destinationFilePath>");
            return;
        }
        this.sourceFolderPath = args[0];
        this.destinationFolderPath = args[1];
        this.callback = callback;
    }

    public void run() {
        FOperations fOperations = new FOperations();
        fOperations.setStrategy(new CopyFolderStrategyBackground(), new String[]{this.sourceFolderPath, this.destinationFolderPath});
        CompletableFuture.runAsync(() -> {
            if (callback != null) {
                callback.onComplete(this.sourceFolderPath + " 文件夹拷贝成功：" + this.destinationFolderPath);
            }
        });
    }

}

