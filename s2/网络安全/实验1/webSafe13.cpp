#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// 解密函数
string decrypt(const string &cipher, const map<char, char> &substitution) {
    string plaintext;
    for (char c : cipher) {
        if (substitution.count(c) > 0) {
            plaintext += substitution.at(c);
        } else {
            plaintext += c; // 如果字符不在代换规则中，则保留原字符
        }
    }
    return plaintext;
}

int main() {
    string text = "UZ QSO VUOHXMOPV GPOZPEVSG ZWSZ OPFPESX UDBMETSX AIZ VUEPHZ HMDZSHZO WSFP APPD TSVP QUZW YMXUZUHSX EPYEPOPDZSZUFPO MB ZWP FUPZ HMDJ UD TMOHMQ";

    // 将文本转换为小写字母
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    string cipher=text;
    
   map<char, char> substitution{
    {'a', 'b'},
    {'b', 'f'},
    {'c', 'c'},
    {'d', 'n'},
    {'e', 'r'},
    {'f', 'v'},
    {'g', 'y'},
    {'h', 'c'},
    {'i', 'u'},
    {'j', 'g'},
    {'k', 'k'},
    {'l', 'l'},
    {'m', 'o'},
    {'n', 'n'},
    {'o', 's'},
    {'p', 'e'},
    {'q', 'w'},
    {'r', 'r'},
    {'s', 'a'},
    {'t', 'm'},
    {'u', 'i'},
    {'v', 'd'},
    {'w', 'h'},
    {'x', 'l'},
    {'y', 'p'},
    {'z', 't'}
};

    string plaintext = decrypt(cipher, substitution);
    cout<<  "待解密明文: "<<text<<endl;
    cout << "解密后密文: " << plaintext << endl;

    return 0;
}
