package org.file.utils;

import java.io.File;

public class PathManager {
    private static PathManager instance;
    private String currentPath;

    private PathManager() {
        this.currentPath = String.valueOf(new File(System.getProperty("user.dir")));
    }

    public static PathManager getInstance() {
        if (instance == null) {
            instance = new PathManager();
        }
        return instance;
    }

    public String getCurrentPath() {
        return currentPath;
    }

    public void setCurrentPath(String newPath) {
        this.currentPath = newPath;
    }

    public static void main(String[] args) {
        PathManager pathManager = PathManager.getInstance();
        System.out.println("当前路径：" + pathManager.getCurrentPath());
        // Change the current path
        pathManager.setCurrentPath("/var/www");
        System.out.println("当前路径：" + pathManager.getCurrentPath());
    }
}
