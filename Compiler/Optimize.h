#pragma once
#include <sstream>
#include "Data.h"

typedef struct DAG_Node
{
    string id;          // 结点的值(一般是基础变量）或者是运算符
    int left = -1;      // 左子树位置
    int right = -1;     // 右子树位置
    vector<string> var; // 存放的值
} DAG_Node;

class Optimize
{
public:
    Optimize(Data &data); // 构造函数
    void optimize();      // 四元式优化进程接口,DAG优化四元式
    int offset;           // 填符号表，当前可用区距

private:
    int num = 0;
    vector<Quater_Node> new_Quater;        // 存储优化后的四元式
    Quater_Node new_Q;                     // 临时存储四元式
    int cnt = 0;                           // cnt 为当前结点的个数
    vector<Quater_Node> ans;               // 存储最后输出的优化后的四元式
    Quater_Node temporary_ans;             // 临时存储优化四元式
    bool *flas = NULL;                     // 标记可以输出的四元式
    vector<DAG_Node> DAG;                  // 节点
    DAG_Node dag;                          // 临时节点
    vector<Identity_Node> new_Identity;    // 临时使用的非临时标识符表：I
    Identity_Node linshi;                  // 临时非临时标识符点
    Identity_Node temporary_Identity_Temp; // 临时使用的临时标识符点
    Data &data;                            // 引用操作data
    bool find_Identity(string c);
    // 查找标识符表
    int find_Related_word(string c);                      // 查找关联词
    void find_Identity_Temp(string c);                    // 查找非临时变量
    bool find_var(int i, string c);                       // 遍历查找变量
    int add_node(string c);                               // 建立结点
    void add_operator(string c, string op, int l, int r); // 添加表达式
    void dfs(int i);                                      // 标记表达式结点
    void replace_DAG(int cnt, int temp1, int i);          // 替换多余变量
    void cout_DAG(int cnt);                               // 计算再优化数字型四元式
    string ok(int i);                                     // 查询结点的值，合并的部分的变量的节点可以删去
    string operatornum(string a, double b, double c);
    inline double str_change_num(string a); // string型转换成double型
};
