package org.file;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        CommandLineMenu commandLineMenu = new CommandLineMenu();
        commandLineMenu.showCommandLineMenu();
    }
}