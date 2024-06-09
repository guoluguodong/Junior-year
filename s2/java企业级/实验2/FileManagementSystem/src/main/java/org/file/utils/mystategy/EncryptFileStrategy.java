package org.file.utils.mystategy;

import org.file.utils.PathManager;
import org.file.utils.myinterface.FOperationInterface;

import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Base64;

public class EncryptFileStrategy implements FOperationInterface {
    private String fileName;
    private String newName;
    private String keyFileName ;
    private String encryptionAlgorithm;
    private Integer keyLength;
    /**
     * 加密文件，密钥记录
     */
    @Override
    public void execute(String[] args) {
//        FilePath 待加密文件名称
//        newName 加密后文件名称
//        keyPath 密钥存储的文件名
//        encryptionAlgorithm 密钥存储的文件名
        this.fileName = args[0];
        this.newName = args[1];
        this.keyFileName = args[2];
        this.encryptionAlgorithm = args[3];
        this.keyLength = Integer.valueOf(args[4]);
        try {

            PathManager pathManager = PathManager.getInstance();
            String curPath = pathManager.getCurrentPath();
            String FilePath = curPath + File.separator + fileName;
            String newFilePath = curPath + File.separator + newName;
            String keyPath = curPath + File.separator + keyFileName;
            File keyfile = new File(keyPath);
            SecretKey secretKey;
            try {
                if (keyfile.createNewFile()) {
                    secretKey = generateDESKey();
                    // 将密钥写入文本文件
                    writeKeyToFile(secretKey, keyPath);
                    encryptFile(FilePath, newFilePath, secretKey);
                } else {
                    secretKey = readKey(keyPath);
                    encryptFile(FilePath, newFilePath, secretKey);
                }
            } catch (IOException e) {
                System.out.println("创建文件时出现异常：" + e.getMessage());
            }

            System.out.println("加密成功");
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("加密失败");
        }
    }

    private SecretKey generateDESKey() throws NoSuchAlgorithmException {
        KeyGenerator keyGenerator = KeyGenerator.getInstance(this.encryptionAlgorithm);
        keyGenerator.init(this.keyLength); // 使用128位密钥
        return keyGenerator.generateKey();
    }

    private SecretKey readKey(String filePath) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String encodedKeyString = reader.readLine();
            byte[] encodedKey = Base64.getDecoder().decode(encodedKeyString);
            return new SecretKeySpec(encodedKey, 0, encodedKey.length, this.encryptionAlgorithm);
        }
    }

    private void encryptFile(String inputFilePath, String outputFilePath, SecretKey secretKey) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IOException, NoSuchPaddingException {
        Cipher cipher = Cipher.getInstance(this.encryptionAlgorithm);
        cipher.init(Cipher.ENCRYPT_MODE, secretKey);
        try (FileInputStream inputStream = new FileInputStream(inputFilePath);
             FileOutputStream outputStream = new FileOutputStream(outputFilePath);
             CipherOutputStream cipherOutputStream = new CipherOutputStream(outputStream, cipher)) {

            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                cipherOutputStream.write(buffer, 0, bytesRead);
            }
        }
    }

    // 将密钥写入文本文件
    private void writeKeyToFile(SecretKey secretKey, String filePath) throws IOException {
        byte[] encodedKey = secretKey.getEncoded();
        String encodedKeyString = Base64.getEncoder().encodeToString(encodedKey);
        try (FileWriter fileWriter = new FileWriter(filePath);
             BufferedWriter bufferedWriter = new BufferedWriter(fileWriter))
        {
            bufferedWriter.write(encodedKeyString);
        }
    }
}
