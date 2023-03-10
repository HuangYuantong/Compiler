#pragma once

#include "Morphology.h"

extern bool flag_i_w_f;      // 是否在比较语句的标记
extern bool flag_temp_store; // 是否将四元式临时存储的标记
extern int number_of_t;      // 临时变量的序号

class Grammar : private Morphology // 语法分析并生成data中的Quater
{
public:
    Grammar(Data &data); // 构造函数
    ~Grammar();          // 析构函数
    bool grammar();      // 语法分析进程接口,语法制导生成四元式

private:
    Data &data;          // 引用操作data
    int offset = Offset; // 填符号表，当前可用区距

    // 递归向下子程序法实现前部分推导
    bool Sub_Program(); //<Sub_Program> -> <Type_Define><Var_Define><Sentence_ Define>
    bool Type_Define(); //<Type_Define> -> "TYPE" "{" {<Type_Sentence>} "}"
    bool Var_Define();  //<Var_Define> -> "VAR {" <Variable_Define> "}"
                       // 变量声明语句整体处理
    bool Variable_Define(); //<Variable_Define>
    bool Sentence_Define(); //<Sentence_ Define> -> { <Equal_Sentence>";" |<If>|<While>|<For> }
                            // 赋值语句整体处理
    bool Equal_Sentence();   //<Equal_Sentence>
    bool Compare_Define();   //<Compare_Define> -> <Compare_Sentence> {ω4 <Compare_Sentence>}
    bool Compare_Sentence(); //<Equal_Sentence> -> <Math_Expression> ω3  <Math_Expression>
    bool If();               // If->”if (“ Compare_Define”) {“ Sentence_ Define”}”
    bool While();            // While->”while (“ Compare_Define”) {“ Sentence_ Define”}”
    bool For();              // For->”for (“ Equal_Sentence Compare_Define Equal_Sentence”) {“ Sentence_ Define”}”
    // 递归向下子程序法附属函数
    string Token_Search_K_O_S();                                                     // Token查找K、O、S表单词string
    int Add_Quater(string operation, string object1, string object2, string result); // 将新四元式压入data四元式序列
    queue<Quater_Node> temp_quater;                                                  // 四元式临时储存:For()中使用
    void Make_T_Variable(string &name, char type);                                   // 生成一个临时变量名为name，类型指向type
    bool Find_Identity(int place, string &location);                                 // 返回:变量名、或数组元素的位置的虚拟地址

    // 算术表达式LL(1)整体处理(=,;)
    bool Math_Expression();                            // 算术表达式分析接口
    stack<char> SYN;                                   // 语法栈
    stack<string> SEM;                                 // 语义栈
    stack<string> SEM_Operate;                         // 语义栈-运算符
    string SEM_Object;                                 // 语义栈-终结符
    bool Math_Expression_Search(char top_ch, char ch); // 算术表达式LL(1)分析表
    char Math_Expression_ch();                         // 算术表达式分析附属函数
};
