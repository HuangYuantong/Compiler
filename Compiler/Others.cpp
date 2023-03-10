#include <iostream>
#include <stdio.h>
#include "Others.h"
using namespace std;

bool Is_S(char ch) // 字符是否是：数字
{
    if (ch >= '0' && ch <= '9')
        return true;
    return false;
}

bool Is_Dz_Xz(char ch) // 字符是否是：大小字母
{
    if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
        return true;
    return false;
}

bool Is_Dz_Xz_S_X(char ch) // 字符是否是：大小字母/数字/下划线
{
    if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ('0' <= ch && ch <= '9') || ch == '_')
        return true;
    return false;
}

void Vision(char choose)
{
    system("CLS");
    switch (choose)
    {
    case '0':
        cout << "            ║      ·编译阶段·     ║\n";
        cout << "            ╰-----------------------╯\n";
        break;
    case 'm':
        cout << "\n\n                主菜单\n";
        cout << "   ╭═══════════════════════════╮\n";
        cout << "   ║    > 1 未优化数据         ║\n";
        cout << "   ║    --------------         ║\n";
        cout << "   ║    > 2 优化后的数据       ║\n";
        cout << "   ║    -----------------      ║\n";
        cout << "   ║    > 3 重新编译           ║\n";
        cout << "   ║    -------------          ║\n";
        cout << "   ║    > 0 退出               ║\n";
        cout << "   ╰═══════════════════════════╯\n";
        cout << "    ·请选择:";
        break;
    case 's':
        cout << "\n\n                数据显示\n";
        cout << "   ╭═════════════════════════════╮\n";
        cout << "   ║   > 1 I表      > 2 四元式   ║\n";
        cout << "   ║   -----------------------   ║\n";
        cout << "   ║   > 3 目标指令与活动记录    ║\n";
        cout << "   ║   ----------------------    ║\n";
        cout << "   ║   > 4 K表、常量表           ║\n";
        cout << "   ║   ------------------        ║\n";
        cout << "   ║   > 5 类型表、数组表        ║\n";
        cout << "   ║   ------------------        ║\n";
        cout << "   ║   > 0 返回                  ║\n";
        cout << "   ╰═════════════════════════════╯\n";
        cout << "    ·请选择:";
        break;
    case 'f':
        cout << "\n";
        cout << "\n ·是否要生成Token、I表、C表文件？\n";
        cout << " ╭════════════════════╮\n";
        cout << " ║     > 1 是         ║\n";
        cout << " ║     --------       ║\n";
        cout << " ║     > 0 否         ║\n";
        cout << " ║     --------       ║\n";
        cout << " ╰════════════════════╯\n";
        cout << " ·请选择:";
        break;
    case 'e':
        cout << "\n     ******************\n";
        cout << "      请检查文件后再试\n";
        cout << "     ******************\n   ";
        break;
    default:
        cout << "Vision函数传参错误\n";
        system("pause");
        break;
    }
}
