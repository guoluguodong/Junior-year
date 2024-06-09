package org.ScoreMangementSystem.utils;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.RUNTIME)
public @interface MethodOrder {
    int value(); // 指定方法的顺序
}
