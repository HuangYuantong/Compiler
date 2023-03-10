#pragma once

#include "Data.h"

class Morphology // 生成data中的Token
{
public:
    // 词法分析构造函数，需要传入data引用以启动
    Morphology(Data &a);
    // 词法分析，将Token改为下一个单词，文件读完后的Token为(#,-1)
    int NEXTw();
    ifstream infile; // 文件流读入

private:
    Data &data;                     // 引用操作data
    bool Number_Generator(char ch); // 常数生成器单独封装

    bool IsFilter(char ch);     // 过滤符判断
    int IsSeparater(char ch);   // 分隔符判断
    int IsKeyword(string word); // 关键字判断
    int IsOperator(char ch);    // 运算符判断

    int IsIdentity(string word); // 标识符判断

    int IsConstant_int(int number);     // int整数判断
    int IsConstant_float(float number); // float浮点数数判断
    int IsConstant_char(char ch);       // char字符常量判断
    int IsConstant_string(string word); // string字符串常量判断
};