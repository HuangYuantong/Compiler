#include "Data.h"

bool flag = true;
bool flag_var = false;
int level = 0;
//
// 显示函数
//
void Data::Show()
{
    while (1)
    {
        system("CLS");
        Vision('s');
        char c;
        cin >> c;
        while (c < '0' || c > '5')
        {
            cout << "\n  ·输入错误，请重新选择:";
            cin.ignore(1024, '\n');
            cin >> c;
        }
        cin.ignore(1024, '\n');
        if (c == '0')
            break;
        else if (c == '1')
        {
            system("CLS");
            cout << "\n                I表\n";
            cout << " ╭═════════════════════════════════╮\n";
            cout << "   name    type_p    cat      add\n";
            for (auto i = 0; i < Identity.size(); ++i)
            {
                cout << "   " << left << setw(7) << Identity[i].name << "  ";
                if (Identity[i].type_p != -1)
                    cout << Type[Identity[i].type_p].name << "-tp"
                         << "      ";
                else
                    cout << "          ";
                cout << Identity[i].cat << "       ";
                cout << setw(2) << Identity[i].add.info << "," << Identity[i].add.offset << "\n";
            }
            cout << "  ╰═════════════════════════════════╯\n       ";
        }
        else if (c == '2')
        {
            system("CLS");
            cout << "\n                四元式\n";
            cout << "  ╭═════════════════════════════════╮\n";

            for (auto i = 0; i < Quater.size(); ++i)
            {
                cout << right << setw(4) << i
                     << " ( " << left << setw(4) << Quater[i].operation << ","
                     << setw(7) << Quater[i].object1 << ","
                     << setw(7) << Quater[i].object2 << ","
                     << setw(7) << Quater[i].result << ")\n";
            }
            cout << "  ╰═════════════════════════════════╯\n       ";
        }
        else if (c == '3')
            Show_Order_H();
        else if (c == '4')
            Show_K_C();
        else
            Show_T_A();
        system("pause");
    }
}

void Data::Show_Order_H()
{
    int size = Identity.size();
    system("CLS");
    cout << "\n           目标指令                      活动记录\n";
    cout << "  ╭═════════════════════════╮";
    cout << "\t ╭══════════════════════╮\n";
    for (auto i = 0; i < Order.size(); ++i)
    {
        cout << "  " << right << setw(3) << i
             << " (" << left << setw(4) << Order[i].operation << ", "
             << setw(2) << Order[i].rgst << ","
             << left << setw(10) << Order[i].address << ")";
        if (i < size + 2)
        {
            cout << "\t"
                 << " ║   ";
            if (i == 0)
                cout << "Old SP = 0         ║\n";
            else if (i == 1)
                cout << "返回地址           ║\n";
            else if (i == 2)
                cout << "参数个数 = 0       ║\n";
            else
                cout << left << setw(7) << Identity[i - 2].name
                     << setw(2) << "(" << Identity[i - 2].add.info << "," << Identity[i - 2].add.offset << ")\t║\n";
        }
        else
        {
            if (i == size + 2)
                cout << "\t ╰══════════════════════╯\n";
            else
                cout << "\n";
        }
    }
    cout << "  ╰═════════════════════════╯\n       ";
}

void Data::Show_K_C()
{
    system("CLS");
    cout << "\n\n          关键字表 界符表 常量表\n";
    cout << "╭════════════════════════════════════════╮\n\n";
    cout << "              关键字表：共" << Keyword.size() << "项\n";
    for (int i = 0; i < Keyword.size(); i++)
    {
        cout << setw(6) << Keyword[i] << " ";
        if ((i + 1) % 5 == 0)
            cout << "\n";
    }
    cout << "\n\n          -----------------------\n";
    cout << "               界符表：共" << Separater.size() + Operator.size() << "项\n";
    cout << " <分隔符> ";
    for (int i = 0; i < Separater.size(); i++)
        cout << Separater[i] << "  ";
    cout << "\n <运算符> ";
    for (int i = 0; i < Operator.size(); i++)
    {
        cout << Operator[i] << "  ";
    }
    cout << "\n\n          -----------------------\n";
    cout << "                  常量表\n";
    if (Constant_int.size())
    {
        cout << " ·常量CI表：共" << Constant_int.size() << "项\n   ";
        for (int i = 0; i < Constant_int.size(); i++)
            cout << Constant_int[i] << "  ";
    }
    if (Constant_float.size())
    {
        cout << "\n ·实量CF表：共" << Constant_float.size() << "项\n   ";
        for (int i = 0; i < Constant_float.size(); i++)
            cout << Constant_float[i] << "  ";
    }
    if (Constant_char.size())
    {
        cout << "\n ·字符常量CT表：共" << Constant_char.size() << "项\n   ";
        for (int i = 0; i < Constant_char.size(); i++)
            cout << Constant_char[i] << "  ";
    }
    if (Constant_string.size())
    {
        cout << "\n ·字符串常量ST表：共" << Constant_string.size() << "项\n";
        for (int i = 0; i < Constant_string.size(); i++)
            cout << "   " << Constant_string[i] << "\n";
    }
    cout << "\n╰════════════════════════════════════════╯\n";
}

void Data::Show_T_A()
{
    system("CLS");
    cout << "\n\n            类型表 数组表\n";
    cout << "╭═══════════════════════════════════╮\n\n";
    cout << "           类型表：共" << Type.size() << "项\n";
    for (int i = 0; i < Type.size(); i++)
    {
        cout << "  ( " << Type[i].name;
        if (Type[i].array_p != -1)
            cout << ",->Array[" << Type[i].array_p << "] )";
        else
            cout << "  -   )";
        if ((i + 1) % 2 == 0)
            cout << "\n";
    }
    cout << "\n         数组表：共" << Array.size() << "项\n";
    cout << " 数据类型 总长度 维度  各维度长度\n";
    for (int i = 0; i < Array.size(); i++)
    {
        cout << "  " << Type[Array[i].type_p].name << "-tp   "
             << setw(4) << Array[i].total_space << "      " << Array[i].dimension << "     ";
        for (int j = 0; j < Array[i].dimension; j++)
            cout << Array[i].dimension_length[j] << "，";
        cout << "\n";
    }
    cout << "╰═══════════════════════════════════╯\n";
}

//
// 功能函数
//
int Data::Find_Size(int type_p)
{
    if (Type[type_p].name == "i" || Type[type_p].name == "f")
        return 4;
    else if (Type[type_p].name == "b" || Type[type_p].name == "c")
        return 1;
    else if (Type[type_p].name == "p")
        return 2;
    else
        return Array[Type[type_p].array_p].total_space;
}

void Data::Update_Offset(int &offset)
{
    auto tail = Identity.back();
    offset = tail.add.offset;
    offset += Find_Size(tail.type_p);
}

void Data::Push_All_Identity_Temp()
{
    int size = Identity_Temp.size();
    int offset;
    Update_Offset(offset);
    for (int i = 0; i < size; ++i)
    {
        auto temp = Identity_Temp[i];
        temp.add.offset = offset;
        offset += Find_Size(temp.type_p);
        Identity.push_back(temp);
    }
    vector<Identity_Node>().swap(Identity_Temp);
}

//
// 构建函数
//
bool Data::Struct()
{
    flag = true;
    flag_var = false;
    level = 0;
    // K、O、S表初始化
    int temp = Keyword_Struct();
    temp += Operator_Struct();
    temp += Separater_Struct();
    if (temp != 0)
    {
        Vision('e');
        return false;
    }
    // Type类型表初始化
    Type.push_back({
        "i",
    });
    Type.push_back({
        "f",
    });
    Type.push_back({
        "b",
    });
    Type.push_back({
        "c",
    });
    Type.push_back({
        "p",
    });
    return true;
}

bool Data::Clean()
{
    vector<string>().swap(Keyword);
    vector<string>().swap(Operator);
    vector<char>().swap(Separater);
    vector<Identity_Node>().swap(Identity);
    vector<Identity_Node>().swap(Identity_Temp);
    vector<Type_Node>().swap(Type);
    vector<Array_Node>().swap(Array);
    vector<int>().swap(Constant_int);
    vector<float>().swap(Constant_float);
    vector<char>().swap(Constant_char);
    vector<string>().swap(Constant_string);
    vector<Quater_Node>().swap(Quater);
    vector<Order_Node>().swap(Order);
    return true;
}

int Data::Keyword_Struct()
{
    ifstream infile("Keyword.txt", ios::in);
    if (!infile)
    {
        puts(" 无Keyword.txt文件，读取失败");
        infile.close();
        return 1;
    }
    string temp;
    while (infile.good())
    {
        infile >> temp;
        Keyword.push_back(temp);
    }
    // for(int i=0;i<Keyword.size();++i) cout<<i+1<<":"<<Keyword[i]<<endl; system("pause");
    infile.close();
    return 0;
}

int Data::Operator_Struct()
{
    ifstream infile("Operator.txt", ios::in);
    if (!infile)
    {
        puts(" 无Operator.txt文件，读取失败");
        infile.close();
        return 1;
    }
    string temp;
    while (infile.good())
    {
        infile >> temp;
        Operator.push_back(temp);
    }
    // for(int i=0;i<Operator.size();++i) cout<<i+1<<":"<<Operator[i]<<endl;
    infile.close();
    return 0;
}

int Data::Separater_Struct()
{
    ifstream infile("Separater.txt", ios::in);
    if (!infile)
    {
        puts(" 无Separater.txt文件，读取失败");
        infile.close();
        return 1;
    }
    char temp;
    while (infile.good())
    {
        infile >> temp;
        Separater.push_back(temp);
    }
    // for (int i = 0; i < Separater.size(); ++i) cout << i + 1 << ":" << Separater[i] << endl;
    infile.close();
    return 0;
}
