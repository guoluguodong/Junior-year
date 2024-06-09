#include <iostream>
#include <bits/stdc++.h>

using namespace std;

bool compareByFrequency(const pair<char, int>& a, const pair<char, int>& b) {
    return a.second > b.second;  // 按照出现次数降序排序
}

int main() {
    string text="UZ QSO VUOHXMOPV GPOZPEVSG ZWSZ OPFPESX UDBMETSX AIZ VUEPHZ HMDZSHZO WSFP APPD TSVP QUZW YMXUZUHSX EPYEPOPDZSZUFPO MB ZWP FUPZ HMDJ UD TMOHMQ";
    unordered_map<char, int> charFrequency;

    for (char c : text) {
        if (isalpha(c)) {  // 只考虑字母字符
            charFrequency[tolower(c)]++;  // 不区分大小写，将字母转换为小写
        }
    }
    vector<pair<char, int>> sortedChars(charFrequency.begin(), charFrequency.end());
    sort(sortedChars.begin(), sortedChars.end(), compareByFrequency);
    ofstream outputFile("output.csv");
    if (outputFile.is_open()) {
        outputFile << "字符,出现频数,出现频率" << endl;
        double sum = 0;
        for (const auto& pair : sortedChars) {
            sum += pair.second;
        }
        for (const auto& pair : sortedChars) {
            outputFile << pair.first << "," << pair.second << "," << fixed << setprecision(4) << pair.second / sum << endl;
        }
        outputFile.close();
        cout << "数据已成功写入 output.csv 文件中。" << endl;
    } else {
        cout << "无法打开文件 output.csv" << endl;
    }

    return 0;
}
