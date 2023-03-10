#pragma once
#pragma warning(disable : 4018)

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <Windows.h>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include "Others.h"
using namespace std;

constexpr int Wrong = -1;
constexpr int Succeed = 1;
constexpr int NotFind = -2;
constexpr int Offset = 4;
constexpr auto Temp_Variable = "$t"; //"TiaTV_aYsnFui_";

extern bool flag;     // 是否有错标记
extern bool flag_var; // 是否有可声明变量标记
extern int level;     // 当前所在函数

//
// Token、四元式、目标指令
//
typedef struct Token_Node // Token结构体
{
    string Type = "#";
    int Place = -1;
} Token_Node;
typedef struct Quater_Node // 四元式结构体
{
    string operation;
    string object1;
    string object2;
    string result;
} Quater_Node;
typedef struct Order_Node // 目标指令结构体
{
    string operation;
    string rgst;
    string address;
} Order_Node;

//
// 类型表结构体
//
typedef struct Array_Node // 数组表
{
    int type_p = -1;              // 类型指针
    int total_space = 1;          // 占空间总长度
    int dimension = 0;            // 维度
    vector<int> dimension_length; // 各维长度
} Array_Node;
typedef struct Type_Node
{
    string name;      //[0]:i,[1]:f,[2]:b,[3]:c
    int array_p = -1; // 数组表指针
} Type_Node;

//
// I表结构体
//
typedef struct I_Address_Node // I表的address结构体
{
    int info = -1;  // 层数或-1(表示储存的是长度)
    int offset = 0; // 区距(level)或长度，int:4, float:4, char:1, bool:1
} I_Address_Node;
typedef struct Identity_Node // I表结构体
{
    string name;         // name
    int type_p = -1;     // type,在Type表的位次
    string cat = "v";    // cat
    I_Address_Node add;  // address
    bool active = false; // 优化时位置 或 活跃信息
} Identity_Node;

class Data
{
public:
    //
    // 符号表
    //
    char Filter[4] = {' ', '\t', '\r', '\n'}; // 过滤符
    // K 关键字表 和 P 界符表，Data::Struct()构造
    vector<string> Keyword;  // 关键字表：K
    vector<string> Operator; // 运算符号：O
    vector<char> Separater;  // 分隔符：S

    // I 标识符表，Morphology::morphology()部分构造
    vector<Identity_Node> Identity;      // 标识符表：I
    vector<Identity_Node> Identity_Temp; // 临时变量的I临时储存

    // 类型表，Data::Struct()初始
    vector<Type_Node> Type;   // 类型表,[0]:i,[1]:f
    vector<Array_Node> Array; // 数组表

    // C 常数表，Morphology::morphology()构造
    vector<int> Constant_int;       // 整数表：CI
    vector<float> Constant_float;   // 实数表：浮点数CF
    vector<char> Constant_char;     // 字符表：CC
    vector<string> Constant_string; // 常数表：字符串CS
    //
    // Token、四元式、目标指令
    //
    Token_Node Token;           // 当前Token
    vector<Quater_Node> Quater; // 四元式序列
    vector<Order_Node> Order;   // 目标指令
    //
    // 符号表工具函数
    //
    // 显示函数
    void Show();         // 显示Data中数据
    void Show_Order_H(); // 显示目标指令、活动记录
    void Show_K_C();     // 显示关键字表、常数表
    void Show_T_A();     // 显示类型表、数组表
    // 功能函数
    int Find_Size(int type_p);       // 返回type_p类型的长度
    void Update_Offset(int &offset); // 将offset值更新到最新
    void Push_All_Identity_Temp();   // 将所有临时变量压入I表
    // 构造与清理函数
    bool Struct(); // 初始构建接口
    bool Clean();  // 清空Data
private:
    int Keyword_Struct();   // K表的初始化
    int Operator_Struct();  // P表-运算符的初始化
    int Separater_Struct(); // P表-分隔符的初始化
};
