package com.example.cal24;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;


public class CalModel {
    public static ArrayList<String> calModel(int[] a) {
        List<DataStuct> orignData = new ArrayList<DataStuct>();
        for (int num : a) {
            DataStuct temp = new DataStuct((double) num, Integer.toString((int) num));
            orignData.add(temp);
        }
        ArrayList<DataStuct> res = new ArrayList<DataStuct>();
        ArrayList<String> ans =new ArrayList<String>();
        cal24(orignData,res);

        for (DataStuct oneres : res) {
            System.out.println(oneres.howTogenerate);
            ans.add(oneres.howTogenerate);
        }
        return FormulaDuplicateRemoval.formulaDuplicateRemoval(ans);
    }

    // 24点的递归
    static public void cal24(List<DataStuct> oneGroupNum, List<DataStuct> res) {
        if (oneGroupNum.size() == 1) {
            if (Math.abs(oneGroupNum.get(0).num - 24) < 0.000002 ) {
                res.add(oneGroupNum.get(0));
            }
            return;
        }
        List<List<DataStuct>> allGroups = Permute.permute(oneGroupNum);
        for (List<DataStuct> one : allGroups) {
            DataStuct first = one.get(0);
            one.remove(0);
            DataStuct second = one.get(0);
            one.remove(0);
            // 合并+-*/
            // +
            DataStuct mergesum = new DataStuct(first.num + second.num,
                    "(" + first.howTogenerate + "+" + second.howTogenerate + ')');
            one.add(0, mergesum);
            cal24(one, res);
            one.remove(0);
            // -
            DataStuct mergesub = new DataStuct(first.num - second.num,
                    "(" + first.howTogenerate + "-" + second.howTogenerate + ')');
            one.add(0, mergesub);
            cal24(one, res);
            one.remove(0);
            // 乘法
            DataStuct mergemul = new DataStuct(first.num * second.num,
                    "(" + first.howTogenerate + "x" + second.howTogenerate + ')');
            one.add(0, mergemul);
            cal24(one, res);
            one.remove(0);

            // 除法
            if (Math.abs(second.num) > 0.000002) {
                if (Math.abs(second.num) > 0.000002) {
                    DataStuct mergediv = new DataStuct(first.num / second.num,
                            "(" + first.howTogenerate + "/" + second.howTogenerate + ')');
                    one.add(0, mergediv);
                    cal24(one, res);
                    one.remove(0);
                }
            }
        }
    }
}

// 全排列
class Permute {
    public static List<List<DataStuct>> permute(List<DataStuct> oneGroupNum) {
        int n = oneGroupNum.size();
        Set<List<DataStuct>> uniquePermutations = new HashSet<List<DataStuct>>(); // 使用 Set 来存储唯一的排列
        backtrack(n, oneGroupNum, uniquePermutations, 0);

        // 将 Set 转换为 List
        List<List<DataStuct>> res = new ArrayList<List<DataStuct>>(uniquePermutations);

        return res;
    }

    public static void backtrack(int n, List<DataStuct> output, Set<List<DataStuct>> res, int first) {
        // 所有数都填完了
        if (first == n) {
            res.add(new ArrayList<DataStuct>(output));
        }
        for (int i = first; i < n; i++) {
            // 动态维护数组
            Collections.swap(output, first, i);
            // 继续递归填下一个数
            backtrack(n, output, res, first + 1);
            // 撤销操作
            Collections.swap(output, first, i);
        }
    }
}


class DataStuct {
    double num = 0;
    String howTogenerate;

    public DataStuct(Double _num, String str) {
        num = _num;
        howTogenerate = str;
    }
}



class FormulaDuplicateRemoval {
    public static ArrayList<String> formulaDuplicateRemoval(ArrayList<String> formulas) {

        // 去重复并满足结合律
        Set<String> uniqueFormulas = new HashSet<>();
        ArrayList<String> ans = new ArrayList<>();
        for (String formula : formulas) {
            String sortedFormula = sortFormula(formula);
            if(!uniqueFormulas.contains(sortedFormula)) {
                uniqueFormulas.add(sortedFormula);
                ans.add(formula);
            }
        }
        return ans;
    }

    // 对公式排序以满足结合律
    private static String sortFormula(String formula) {
        // 使用递归来处理括号
        while (formula.contains("(")) {
            int openIndex = formula.lastIndexOf("(");
            int closeIndex = formula.indexOf(")", openIndex);
            if (openIndex == -1 || closeIndex == -1) {
                break; // 如果没有匹配的括号，退出循环
            }
            // 提取括号内的子公式，对子公式进行排序后替换回去
            String innerFormula = formula.substring(openIndex + 1, closeIndex);
            String sortedInnerFormula = sortFormula(innerFormula);
            formula = formula.substring(0, openIndex) + sortedInnerFormula + formula.substring(closeIndex + 1);
        }
        // 重新构建排序后的公式
        return formula;
    }
}




