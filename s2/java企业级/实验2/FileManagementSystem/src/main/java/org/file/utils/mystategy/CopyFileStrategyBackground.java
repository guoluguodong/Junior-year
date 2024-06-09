package org.file.utils.mystategy;

import org.file.utils.myinterface.FOperationInterface;
import org.fusesource.jansi.Ansi;
import org.fusesource.jansi.AnsiConsole;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.time.Duration;
import java.time.Instant;

public class CopyFileStrategyBackground implements FOperationInterface {
    Instant startTime  = Instant.now(); // 记录拷贝开始时间
    @Override
    public void execute(String[] args) {
        if (args.length < 2) {
            System.err.println("Usage: CopyFileStrategy <sourceFilePath> <destinationFilePath>");
            return;
        }

        String sourceFilePath = args[0];
        String destinationFilePath = args[1];

        File sourceFile = new File(sourceFilePath);
        File destinationFile = new File(destinationFilePath);

        try {
            this.startTime = Instant.now(); // 记录拷贝开始时间
            copyFileWithProgress(sourceFile, destinationFile);

            Instant endTime = Instant.now(); // 拷贝完成时间
            Duration duration = Duration.between(startTime, endTime); // 计算拷贝时间
        } catch (IOException e) {
            System.err.println("拷贝文件出错: " + e.getMessage());
        }
    }
    private static final int PROGRESS_BAR_LENGTH = 50; // 进度条长度
    private static final char PROGRESS_CHAR = '█'; // 进度条填充字符
    private void copyFileWithProgress(File sourceFile, File destinationFile) throws IOException {
        try (FileInputStream inputStream = new FileInputStream(sourceFile);
             FileOutputStream outputStream = new FileOutputStream(destinationFile);
             FileChannel inChannel = inputStream.getChannel();
             FileChannel outChannel = outputStream.getChannel()) {

            long fileSize = sourceFile.length();
            long transferred = 0;
            final int bufferSize = 8;
            ByteBuffer buffer = ByteBuffer.allocate(bufferSize);

            while (inChannel.read(buffer) != -1) {
                buffer.flip();
                outChannel.write(buffer);
                buffer.clear();
                transferred += bufferSize;

                int progress = (int) ((transferred * PROGRESS_BAR_LENGTH) / fileSize);
            }

        }
    }
}
