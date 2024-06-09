#include <iostream>
#include <bits/stdc++.h>

using namespace std;

bool compareByFrequency(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;  // 按照出现次数降序排序
}
int main() {
    string text = "UZ QSO VUOHXMOPV GPOZPEVSG ZWSZ OPFPESX UDBMETSX AIZ VUEPHZ HMDZSHZO WSFP APPD TSVP QUZW YMXUZUHSX EPYEPOPDZSZUFPO MB ZWP FUPZ HMDJ UD TMOHMQ";
    unordered_map<string, int> tripleChars;

    // 将文本转换为小写字母
    transform(text.begin(), text.end(), text.begin(), ::tolower);

    // // 遍历文本，统计双字符出现的频率
    for (size_t i = 0; i < text.size() - 2; ++i) {
        string tripleChar = text.substr(i, 3);
        if(tripleChar[0]!=' '&&tripleChar[1]!=' '&& tripleChar[2]!=' '){
        if (tripleChars.find(tripleChar) != tripleChars.end()) {
            tripleChars[tripleChar]++;
        } else {
            tripleChars[tripleChar] = 1;
        }
        }
    }
    vector<pair<string, int>> sortedChars{tripleChars.begin(), tripleChars.end()};
    sort(sortedChars.begin(), sortedChars.end(), compareByFrequency);
    // // 输出双字符及其频率
    vector<string> ans(3,"");
    for (const auto& pair : sortedChars) {
        cout << pair.first << ": " << pair.second << endl;
        ans[pair.second-1]+= " " + pair.first;
    }
    for (const auto& ansss : ans) {
        cout << ansss <<endl;
    }
    return 0;
}
