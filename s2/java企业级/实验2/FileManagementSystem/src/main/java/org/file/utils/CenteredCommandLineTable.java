package org.file.utils;

import org.file.utils.myinterface.Drawable;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

public class CenteredCommandLineTable implements Drawable {
    private int rows;
    private int columns;
    private String[][] data;

    public CenteredCommandLineTable(String[][] data)  {
        this.data = data;
        this.rows = data.length;
        this.columns = 4;
    }
    @Override
    public void draw() {
    // Calculate maximum width for each column
    int[] columnWidths = new int[columns];
    for (int j = 0; j < columns; j++) {
        int maxWidth = 0;
        for (int i = 0; i < rows; i++) {
            String cellValue = data[i][j] != null ? data[i][j] : "";
            maxWidth = Math.max(maxWidth, cellValue.length());
        }
        columnWidths[j] = maxWidth + 2; // Add padding for better readability
    }

    // Draw top border
    drawHorizontalLine(columnWidths);

    // Draw table content
    for (int i = 0; i < rows; i++) {
        System.out.print("|");
        for (int j = 0; j < columns; j++) {
            String cellValue = data[i][j] != null ? data[i][j] : "";
            System.out.format("%-" + columnWidths[j] + "s|", centerAlign(cellValue, columnWidths[j]));
        }
        System.out.println();
        drawHorizontalLine(columnWidths);
    }
}
    private void drawHorizontalLine(int[] columnWidths) {
        for (int width : columnWidths) {
            for (int i = 0; i < width; i++) {
                System.out.print("-");
            }
            System.out.print("+"); // Add '+' at the end of each column
        }
        System.out.println(); // Move to the next line after drawing the horizontal line
    }
    private String centerAlign(String text, int width) {
        return String.format("%-" + width + "s", String.format("%" + (width + text.length()) / 2 + "s", text));
    }
}
