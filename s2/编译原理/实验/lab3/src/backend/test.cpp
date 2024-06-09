#include <assert.h>
#include <unordered_map>
#include <map>
#include <functional> // 用于std::function
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <sstream>
#include<stdint.h>
using namespace std;
string binaryToUnsignedIntString(string binary)
{
    bitset<32> bits(binary);
    unsigned int unsignedInt = bits.to_ulong();
    return to_string(unsignedInt);
}
// 将浮点数转换为IEEE 754格式的二进制字符串
string floatToLong(float num)
{
    stringstream ss;
    union
    {
        float input;
        int output;
    } data;
    data.input = num;
    bitset<sizeof(float) * 8> bits(data.output);
    ss << bits;
    return binaryToUnsignedIntString(ss.str());
}
std::string bit_extend(float a)
{
    uint32_t bits;
    // 使用联合体来查看浮点数的位模式
    union
    {
        float f;
        uint32_t u;
    } float_union;
    float_union.f = a;
    bits = float_union.u;
    char octalStr[9];
    snprintf(octalStr, sizeof(octalStr), "%08X", float_union.u);
    char finalStr[13]; // "0x" + 8位16进制字符串 + 终止符
    snprintf(finalStr, sizeof(finalStr), "0x%s", octalStr);
    return finalStr;
}
int main()
{
    float a = 3.1415;
    cout << a << endl;
    cout << floatToLong(a) << endl;
    cout << bit_extend(a) << endl;
    // cout<<binaryToUnsignedIntString(floatToLong(a))<<endl;
}