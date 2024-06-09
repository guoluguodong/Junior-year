#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int rounds = 16;             // 轮数
// 随机生成密钥
vector<unsigned int> generateRandomKeys() {
    vector<unsigned int> keys;
    srand(time(0)); 
    for (int i = 0; i < rounds; ++i) {
        unsigned int key = rand();
        keys.push_back(key);
    }
    return keys;
}
// 固定密钥
// vector<unsigned int> generateRandomKeys() {
//     vector<unsigned int> keys;
//     srand(time(0)); 
//     for (int i = 0; i < rounds; ++i) {
//         keys.push_back(12345678);
//     }
//     return keys;
// }
vector<unsigned int> keys = generateRandomKeys();

vector<char> FxorStrings(vector<char> R, vector<char> L,size_t round)
{
    vector<char> result;
    for (size_t i = 0; i < L.size(); ++i)
    {
        // unsigned int 32 位每次取8位
        // Example Feistel round operation: XOR with key
        char temp = R[i] ^ (keys[round] >> (i % 4 * 8)); 
        result.push_back(L[i] ^ temp);
    }
    return result;
}

pair<vector<char>, vector<char>> feistelOneRound(vector<char> L, vector<char> R,size_t round)
{
    pair<vector<char>, vector<char>> result;
    result.second = R;
    result.first = FxorStrings(R, L,round);
    return result;
}
pair<vector<char>, vector<char>> feistel(vector<char> L, vector<char> R,bool decode=false)
{
    if(decode){
        reverse(keys.begin(), keys.end());
    }
    pair<vector<char>, vector<char>> result;
    for (size_t i = 0; i < rounds; ++i)
    {
        result = feistelOneRound(L, R,i);
        L = result.second;
        R = result.first;
    }
    return result;
}
int main()
{
    string s = "CQUINFORMATIONSECURITYEXP ";
    vector<char> LE0, RE0, LEn, REn, RDn, LDn;
    for (size_t i = 0; i < s.size() / 2; ++i)
    {
        LE0.push_back(s[i]);
        RE0.push_back(s[i + s.size() / 2]);
    }
    cout << "加密前" << endl;
    for (size_t i = 0; i < LE0.size(); ++i)
    {
        cout << static_cast<int>(LE0[i]) << " ";
    }
    for (size_t i = 0; i < RE0.size(); ++i)
    {
        cout << static_cast<int>(RE0[i]) << " ";
    }
    cout << endl
         << "加密" << endl;
    pair<vector<char>, vector<char>> resultE = feistel(LE0, RE0);
    REn = resultE.first;
    LEn = resultE.second;
    for (size_t i = 0; i < REn.size(); ++i)
    {
        cout << static_cast<int>(REn[i]) << " ";
    }
    for (size_t i = 0; i < LEn.size(); ++i)
    {
        cout << static_cast<int>(LEn[i]) << " ";
    }
    cout << endl
         << "解密：" << endl;
    pair<vector<char>, vector<char>> resultD = feistel(REn, LEn, true);
    RDn = resultD.first;
    LDn = resultD.second;
    for (size_t i = 0; i < RDn.size(); ++i)
    {
        cout << static_cast<int>(RDn[i]) << " ";
    }
    for (size_t i = 0; i < LDn.size(); ++i)
    {
        cout << static_cast<int>(LDn[i]) << " ";
    }
    cout << endl
         << "此时的keys" << endl;
   
    for (size_t i = 0; i < keys.size(); ++i)
    {
        cout << keys[i] << " ";
    }
    return 0;
}
