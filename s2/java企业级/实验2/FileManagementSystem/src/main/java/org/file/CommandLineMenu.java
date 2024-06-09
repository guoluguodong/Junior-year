package org.file;

import org.file.utils.CopyFolderThread;
import org.file.utils.FOperations;
import org.file.utils.PathManager;
import org.file.utils.myinterface.CopyFolderThreadCallback;
import org.file.utils.mystategy.*;

import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class CommandLineMenu {
    public static Map<String, String> map = new HashMap<>();

    void showCommandLineMenu() {
        map.put("AES", "128");
        map.put("DES", "56");
        Scanner scanner = new Scanner(System.in);
        int choice;
        System.out.println("============== 基于命令行的文件管理器 ==============");
        PathManager pathManager = PathManager.getInstance();
        System.out.println("当前路径: " + pathManager.getCurrentPath());
        FOperations fOperations = new FOperations();
        do {
            System.out.println("======== 主菜单 ========");
            System.out.println("1. 选项一: 修改当前工作文件夹");
            System.out.println("2. 选项二: 文件的增删改操作");
            System.out.println("3. 选项三: 文件夹的增删改操作");
            System.out.println("4. 选项四: 罗列文件");
            System.out.println("5. 选项五: 查看一个文本文件的内容");
            System.out.println("6. 选项六: 拷贝文件或者文件夹");
            System.out.println("7. 选项七: 文件加密解密");
            System.out.println("8. 选项八: 文件压缩与解压");
            System.out.println("9. 退出");
            System.out.print("请选择操作: ");

            // 读取用户输入的选项
            choice = scanner.nextInt();
            scanner.nextLine(); // 消耗掉输入的换行符

            // 根据用户的选择执行相应的操作
            switch (choice) {
                case 1:
                    System.out.println("======== 修改当前工作文件夹 ========");
                    System.out.println("当前路径: " + pathManager.getCurrentPath());
                    System.out.println("请输入target路径: ");
                    String line = scanner.nextLine();
                    pathManager.setCurrentPath(line);
                    System.out.println("当前路径: " + pathManager.getCurrentPath());
                    break;
                case 2:
                    System.out.println("======== 文件的增删改操作 ========");
                    System.out.println("1. 选项一: 添加文件");
                    System.out.println("2. 选项二: 删除文件");
                    System.out.println("3. 选项三: 重命名文件");
                    System.out.println("4. 选项四: 返回主菜单");
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            System.out.println("======== 添加文件 ========");
                            System.out.println("请输入文件名: ");
                            line = scanner.nextLine();
                            fOperations.setStrategy(new AddFileStrategy(), new String[]{line});
                            break;
                        case 2:
                            System.out.println("======== 删除文件 ========");
                            System.out.println("请输入文件名: ");
                            line = scanner.nextLine();
                            fOperations.setStrategy(new DeleteFileStrategy(), new String[]{line});
                            break;
                        case 3:
                            System.out.println("======== 重命名文件 ========");
                            System.out.println("请输入旧文件名: ");
                            line = scanner.nextLine();
                            System.out.println("请输入新文件名: ");
                            String line2 = scanner.nextLine();
                            fOperations.setStrategy(new RenameFileStrategy(), new String[]{line, line2});
                            break;
                        case 4:
                            break;
                    }
                    break;
                case 3:
                    System.out.println("======== 文件夹的增删改操作 ========");
                    System.out.println("1. 选项一: 新建文件夹");
                    System.out.println("2. 选项二: 删除文件夹");
                    System.out.println("3. 选项三: 重命名文件夹");
                    System.out.println("4. 选项四: 返回主菜单");
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            System.out.println("======== 新建文件夹 ========");
                            System.out.println("请输入文件夹名: ");
                            line = scanner.nextLine();
                            fOperations.setStrategy(new AddFolderStrategy(), new String[]{line});
                            break;
                        case 2:
                            System.out.println("======== 删除文件夹 ========");
                            System.out.println("请输入文件夹名: ");
                            line = scanner.nextLine();
                            fOperations.setStrategy(new DeleteFolderStrategy(), new String[]{line});
                            break;
                        case 3:
                            System.out.println("======== 重命名文件夹 ========");
                            System.out.println("请输入旧文件夹名: ");
                            line = scanner.nextLine();
                            System.out.println("请输入新文件夹名: ");
                            String line2 = scanner.nextLine();
                            fOperations.setStrategy(new RenameFolderStrategy(), new String[]{line, line2});
                            break;
                        case 4:
                            break;
                    }
                    break;
                case 4:
                    System.out.println("======== 罗列文件 ========");
                    System.out.println("输入查询条件（如文件名的一部分，输入回车跳过过滤）: ");
                    line = scanner.nextLine();
                    System.out.println("选择排序方式，1.名称   2.文件大小   3. 日期");
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    String[] sort = {"name", "size", "data"};
                    fOperations.setStrategy(new ListFolderStrategy(), new String[]{line, sort[choice-1]});
                    break;
                case 5:
                    System.out.println("======== 查看一个文本文件的内容 ========");
                    System.out.println("请输入文件夹名: ");
                    line = scanner.nextLine();
                    fOperations.setStrategy(new ReadTxtStrategy(), new String[]{line});
                    break;
                case 6:
                    System.out.println("======== 拷贝文件或者文件夹 ========");
                    System.out.println("1. 选项一: 拷贝文件");
                    System.out.println("2. 选项二: 拷贝文件夹");
                    System.out.println("3. 选项三: 返回主菜单");
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            System.out.println("======== 拷贝文件 ========");
                            System.out.println("请输入文件绝对路径 （形如C:\\Users\\a3840\\a.txt）");
                            line = scanner.nextLine();
                            System.out.println("请输入复制到的绝对路径  ");
                            String line2 = scanner.nextLine();
                            fOperations.setStrategy(new CopyFileStrategy(), new String[]{line, line2});
                            break;
                        case 2:
                            System.out.println("======== 拷贝文件夹 ========");
                            System.out.println("请输入文件夹绝对路径: （形如C:\\Users\\a3840\\file）");
                            line = scanner.nextLine();
                            System.out.println("请输入复制到的绝对路径: （形如C:\\Users\\a3840\\newFile）");
                            line2 = scanner.nextLine();
                            System.out.println("文件夹较大，输入1 前台拷贝，输入2后台拷贝");
                            choice = scanner.nextInt();
                            scanner.nextLine(); // 消耗掉输入的换行符
                            switch (choice) {
                                case 1:
                                    fOperations.setStrategy(new CopyFolderStrategy(), new String[]{line, line2});
                                    break;
                                case 2:
                                    CopyFolderThreadCallback callback = new CopyFolderThreadCallback() {
                                        @Override
                                        public void onComplete(String result) {
                                            System.out.println("Callback received: " + result);
                                        }
                                    };
                                    CopyFolderThread thread = new CopyFolderThread(new String[]{line, line2},callback);
                                    thread.start();
                                    break;
                            }
                            break;
                        case 3:
                            break;
                    }
                    break;
                case 7:
                    System.out.println("======== 文件或文件夹加密解密 ========");
                    System.out.println("输入1 加密，输入2 解密");
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            System.out.println("请输入待加密文件名称");
                            line = scanner.nextLine();
                            System.out.println("请输入加密后文件名称");
                            String line2 = scanner.nextLine();
                            System.out.println("请输入密钥文件名 ");
                            String line3 = scanner.nextLine();
                            System.out.println("请选择加密算法，  1.AES 2.DES");
                            choice = scanner.nextInt();
                            scanner.nextLine(); // 消耗掉输入的换行符
                            String[] encrypt = new String[]{"AES", "DES"};
                            fOperations.setStrategy(new EncryptFileStrategy(), new String[]{line, line2, line3, encrypt[choice-1], String.valueOf(map.get(encrypt[choice-1]))});
                            break;
                        case 2:
                            System.out.println("请输入待解密文件名称");
                            line = scanner.nextLine();
                            System.out.println("请输入解密后文件名称");
                            line2 = scanner.nextLine();
                            System.out.println("请输入密钥文件名 ");
                            line3 = scanner.nextLine();
                            System.out.println("请选择加密算法，  1.AES 2.DES");
                            choice = scanner.nextInt();
                            scanner.nextLine(); // 消耗掉输入的换行符
                            encrypt = new String[]{"AES", "DES"};
                            fOperations.setStrategy(new DecryptFileStrategy(), new String[]{line, line2, line3, encrypt[choice-1]});
                            break;
                    }
                    break;
                case 8:
                    System.out.println("======== 文件压缩与解压 ========");
                    System.out.println("输入1 压缩，输入2 解压缩");
                    choice = scanner.nextInt();
                    scanner.nextLine(); // 消耗掉输入的换行符
                    switch (choice) {
                        case 1:
                            System.out.println("请输入待压缩文件和文件夹绝对路径 （形如C:\\Users\\a3840\\file）");
                            line = scanner.nextLine();
                            System.out.println("请输入压缩后文件名（形如C:\\Users\\a3840\\file\\zip））");
                            String line2 = scanner.nextLine();
                            fOperations.setStrategy(new CompressFStrategy(), new String[]{line,line2});
                            break;
                        case 2:
                            System.out.println("请输入待解压文件绝对路径 （形如C:\\Users\\a3840\\file.zip）");
                            line = scanner.nextLine();
                            System.out.println("请输入解压到的文件夹绝对路径（形如C:\\Users\\a3840\\file2））");
                            line2 = scanner.nextLine();
                            fOperations.setStrategy(new DecompressFStrategy(), new String[]{line,line2});
                            break;
                    }
                    break;
            }
        } while (choice != 9);


    }
}
