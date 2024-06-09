package org.file.utils.mystategy;
import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Base64;

public class DecryptFileStrategy implements FOperationInterface {

    private String encryptionAlgorithm;
    /**
     * 解密文件密文件，密钥记录
     */
    @Override
    public void execute(String[] args) {
//        fileName    待解密文件名称
//        newName     解密后文件名称
//        keyFileName 密钥存储的文件名
        String fileName = args[0];
        String newName = args[1];
        String keyFileName = args[2];
        this.encryptionAlgorithm = args[3];
        try {
            PathManager pathManager = PathManager.getInstance();
            String curPath = pathManager.getCurrentPath();
            String FilePath = curPath + File.separator + fileName;
            String newFilePath = curPath + File.separator + newName;
            String keyPath = curPath + File.separator + keyFileName;
            // 读取DES密钥
            SecretKey secretKey = readKey(keyPath);
            decryptFile(FilePath,newFilePath,secretKey);
            System.out.println("解密成功");
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("解密失败");
        }
    }

    private SecretKey readKey(String filePath) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String encodedKeyString = reader.readLine();
            byte[] encodedKey = Base64.getDecoder().decode(encodedKeyString);
            return new SecretKeySpec(encodedKey, 0, encodedKey.length, this.encryptionAlgorithm);
        }
    }
    private void decryptFile(String inputFilePath, String outputFilePath, SecretKey secretKey) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IOException {
        Cipher cipher = Cipher.getInstance(this.encryptionAlgorithm);
        cipher.init(Cipher.DECRYPT_MODE, secretKey);

        try (FileInputStream inputStream = new FileInputStream(inputFilePath);
             FileOutputStream outputStream = new FileOutputStream(outputFilePath);
             CipherInputStream cipherInputStream = new CipherInputStream(inputStream, cipher)) {

            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = cipherInputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
            }
        }
    }

}
