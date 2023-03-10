#include "Grammar.h"

bool flag_i_w_f = false;
bool flag_temp_store = false;
int number_of_t = 1;

Grammar::Grammar(Data &data) : data(data), Morphology(data)
{
    flag_i_w_f = false;
    flag_temp_store = false;
    number_of_t = 1;
    infile = ifstream("Data.txt", ios::in);
    if (!infile)
    {
        cout << "\n  无Data.txt文件，读取失败" << endl;
        cout << "      请检查文件后再试\n    ";
        infile.close();
        system("pause");
    }
}

Grammar::~Grammar()
{
    infile.close();
}

//
// 递归向下子程序法实现前部分推导
//
bool Grammar::grammar()
{
    NEXTw();
    // 开始语法分析(递归向下子程序法)
    //"main"
    if (Token_Search_K_O_S() != "main")
        flag = false;
    if (flag == false)
        return flag;
    Add_Quater("start", "main", "", "");
    Identity_Node temp;
    temp.name = "main";
    temp.cat = "f";
    temp.add.info = level;
    data.Identity.push_back(temp);
    NEXTw();
    //"{"
    if (Token_Search_K_O_S() != "{")
        flag = false;
    if (flag == false)
        return flag;
    // cout << "进入main函数 {\n";
    NEXTw();
    // Sub_Program()分程序
    if (Sub_Program() == false)
        return flag;
    //"}"
    if (Token_Search_K_O_S() != "}")
        flag = false;
    if (flag == false)
        return flag;
    // cout << "} main函数结束\n\n";
    Add_Quater("end", "main", "", "");
    return flag;
}

bool Grammar::Sub_Program()
{
    // if (Type_Define() == false) return flag;
    if (Var_Define() == false)
        return flag;
    if (Sentence_Define() == false)
        return flag;

    return flag;
}

bool Grammar::Type_Define()
{
    //"TYPE"
    if (Token_Search_K_O_S() != "type")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    //"{"
    if (Token_Search_K_O_S() != "{")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    //"}"
    if (Token_Search_K_O_S() != "}")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    // cout << "Type_Define()结束\n";
    return flag;
}

bool Grammar::Var_Define()
{
    //"VAR"
    if (Token_Search_K_O_S() != "var")
        flag = false;
    if (flag == false)
        return flag;
    flag_var = true;
    NEXTw();
    //"{"
    if (Token_Search_K_O_S() != "{")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();

    while (Token_Search_K_O_S() != "}")
    {
        if (Variable_Define() == false)
            return flag;
    }
    flag_var = false;
    NEXTw();
    // cout << "Var_Define()结束\n";
    return flag;
}

bool Grammar::Variable_Define()
{
    // cout << "变量声明";
    int type = 0;
    // type
    string w = Token_Search_K_O_S();
    if (w == "int")
        type = 0;
    else if (w == "float")
        type = 1;
    else if (w == "bool")
        type = 2;
    else if (w == "char")
        type = 3;
    else
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    if (flag == false)
        return flag;
    //
    // Variable_Table
    // 将I表填完整
    // type_p[类型]：Type_Node指针，指向Type表的某个Type_Node
    // cat[种类]：本函数声明变量，都是v
    // add.info[地址]：若info=-1，表示自定义类型，add.offset储存长度信息，否则储存函数层数，main为0
    // add.offset[地址]：若info!=-1，表示为变量，add.offset储存该变量区距
    // （int:4, float:4, bool:1, char:1）
    //

    // Array_Define()
    // type后接array，声明数组,一行只可声明一个
    if (data.Token.Type == "K")
    {
        if (Token_Search_K_O_S() != "array")
        {
            flag = false;
            return flag;
        }
        NEXTw();
        // Identity：name、cat、add
        if (data.Token.Type != "I")
        {
            flag = false;
            return flag;
        }
        data.Identity[data.Token.Place].add.info = level;
        data.Identity[data.Token.Place].add.offset = offset;
        // 增加Array数组表
        Array_Node temp_array;
        temp_array.type_p = type; // 该数组类型为type
        NEXTw();
        do
        {
            //"["
            if (Token_Search_K_O_S() != "[")
            {
                flag = false;
                return flag;
            }
            NEXTw();
            // 整数CI
            if (data.Token.Type != "CI")
            {
                flag = false;
                return flag;
            }
            int temp_length = data.Constant_int[data.Token.Place];
            if (temp_length <= 0)
            {
                cout << "\n数组维度长度定义错误\n";
                flag = false;
                return flag;
            }
            temp_array.dimension_length.push_back(temp_length); // 各维长度
            ++temp_array.dimension;                             // 维度
            temp_array.total_space *= temp_length;              // 总元素个数
            NEXTw();
            //"]"
            if (Token_Search_K_O_S() != "]")
            {
                flag = false;
                return flag;
            }
            NEXTw();
        } while (Token_Search_K_O_S() == "[");
        // 占空间总长度 = 元素个数 * 元素类型长度
        temp_array.total_space *= data.Find_Size(type);
        // 更新offset
        offset += temp_array.total_space;
        data.Array.push_back(temp_array);
        // 增加Type类型表
        data.Type.push_back({"a", (int)data.Array.size() - 1});
        // Identity：type_p
        data.Identity[data.Identity.size() - 1].type_p = data.Type.size() - 1;
        //";"
        if (Token_Search_K_O_S() != ";")
        {
            flag = false;
            return flag;
        }
        NEXTw();
        return flag;
    }

    // Variable_Table()
    // int后接一个变量名，声明变量,一行可声明0个、或多个
    while (data.Token.Type == "I")
    {
        data.Identity[data.Token.Place].type_p = type;

        data.Identity[data.Token.Place].add.info = level;
        data.Identity[data.Token.Place].add.offset = offset;
        offset += data.Find_Size(type);
        // 一行可声明0个、或多个变量，以逗号','隔开
        NEXTw();
        if (Token_Search_K_O_S() == ",")
            NEXTw();
    }
    //";"
    if (Token_Search_K_O_S() != ";")
    {
        flag = false;
        return flag;
    }
    NEXTw();
    return flag;
}

bool Grammar::Sentence_Define()
{
    string temp = "";
    while ((temp = Token_Search_K_O_S()) != "}")
    {
        if (data.Token.Type == "CS")
        {
            NEXTw();
            if (Token_Search_K_O_S() != ";")
                flag = false;
            NEXTw();
        }
        else if (temp == "if")
        {
            if (If() == false)
                return flag;
        }
        else if (temp == "while")
        {
            if (While() == false)
                return flag;
        }
        else if (temp == "for")
        {
            if (For() == false)
                return flag;
        }
        else
        {
            if (Equal_Sentence() == false)
                return flag;
            NEXTw();
        }
    }
    // cout << "Sentence_Define()结束\n";
    return flag;
}

//
// 前部分推导：语句部分
//
bool Grammar::Equal_Sentence()
{
    // 式子首单词为Variable，可能是赋值语句
    if (data.Token.Type == "I")
    {
        string result;
        Find_Identity(data.Token.Place, result);
        char ch;
        while ((ch = infile.peek()) != EOF && (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'))
            ch = infile.get(); // 跳过所有过滤符
        ch = infile.peek();
        //"="
        if (ch == '=')
        {
            NEXTw();
            // cout << "有=，发现一个赋值等式：";
            NEXTw();
            // Math_Expression
            if (Math_Expression() == false)
                return flag;

            string object1 = SEM.top();
            SEM.pop();
            if (flag_temp_store == false)
                Add_Quater("=", object1, "", result);
            else
            {
                Quater_Node temp_q_node{"=", object1, "", result};
                temp_quater.push(temp_q_node);
            }
        }
        // 无=
        else
        {
            cout << "\n 表达式返回值被忽略\n";
            flag = false;
            return flag;
        }
    }
    // 式子首单词不为Variable，不是赋值语句
    else
    {
        cout << "\n 表达式返回值被忽略\n";
        flag = false;
        return flag;
    }
    return flag;
}

bool Grammar::Compare_Define()
{
    if (Compare_Sentence() == false)
        return flag;
    string ch = Token_Search_K_O_S();
    if (ch == "&&" || ch == "||")
    {
        // cout << "发现一个" << ch << "运算\n";
        string object1 = data.Quater[data.Quater.size() - 1].result;
        NEXTw();
        if (Compare_Sentence() == false)
            return flag;
        string object2 = data.Quater[data.Quater.size() - 1].result;
        string result;
        Make_T_Variable(result, 'b');
        Add_Quater(ch, object1, object2, result);
    }
    return flag;
}

bool Grammar::Compare_Sentence()
{
    if (Math_Expression() == false)
        return flag;
    string object1 = SEM.top();
    SEM.pop();

    string ch = Token_Search_K_O_S();
    if (ch == "==" || ch == "!=" || ch == "<" || ch == "<=" || ch == ">" || ch == ">=")
    {
        // cout << "有" << ch << ",发现一个Compare_Sentence:";
        NEXTw();
    }
    else
    {
        cout << "\n 缺少比较符号\n";
        flag = false;
        return flag;
    }
    if (Math_Expression() == false)
        return flag;
    string object2 = SEM.top();
    SEM.pop();

    string result;
    Make_T_Variable(result, 'b');
    Add_Quater(ch, object1, object2, result);
    return flag;
}

bool Grammar::If()
{
    //"if"
    if (Token_Search_K_O_S() != "if")
    {
        flag = false;
        return flag;
    }
    NEXTw();
    //"("
    if (Token_Search_K_O_S() != "(")
    {
        flag = false;
        return flag;
    }
    NEXTw();
    // Compare_Sentence
    flag_i_w_f = true;
    if (Compare_Define() == false)
        return flag;
    flag_i_w_f = false;
    //")"
    if (Token_Search_K_O_S() != ")")
    {
        flag = false;
        return flag;
    }
    string object1 = Temp_Variable + to_string(number_of_t - 1);
    Add_Quater("fj", object1, "", "");
    NEXTw();
    //"{"
    if (Token_Search_K_O_S() != "{")
    {
        flag = false;
        return flag;
    }
    NEXTw();
    // Sentence_Define()
    if (Token_Search_K_O_S() != "}")
        if (Sentence_Define() == false)
            return flag;
    //"}"
    NEXTw();
    cout << "(if结束)";
    //"else"
    if (Token_Search_K_O_S() == "else")
    {
        Add_Quater("el", "", "", "");
        NEXTw();
        //"{"
        if (Token_Search_K_O_S() != "{")
        {
            flag = false;
            return flag;
        }
        NEXTw();
        // Sentence_Define()
        if (Token_Search_K_O_S() != "}")
            if (Sentence_Define() == false)
                return flag;
        //"}"
        NEXTw();
        cout << "(else结束)";
    }
    Add_Quater("ie", "", "", "");
    return flag;
}

bool Grammar::While()
{
    //"while"
    if (Token_Search_K_O_S() != "while")
    {
        flag = false;
        return flag;
    }
    Add_Quater("wh", "", "", "");
    NEXTw();
    //"("
    if (Token_Search_K_O_S() != "(")
    {
        flag = false;
        return flag;
    }
    NEXTw();
    // Compare_Sentence
    flag_i_w_f = true;
    if (Compare_Define() == false)
        return flag;
    flag_i_w_f = false;
    //")"
    if (Token_Search_K_O_S() != ")")
    {
        flag = false;
        return flag;
    }
    string object1 = Temp_Variable + to_string(number_of_t - 1);
    Add_Quater("fj", object1, "", "");
    NEXTw();
    //"{"
    if (Token_Search_K_O_S() != "{")
    {
        flag = false;
        return flag;
    }
    NEXTw();
    // Sentence_Define()
    if (Token_Search_K_O_S() != "}")
        if (Sentence_Define() == false)
            return flag;
    //"}"
    Add_Quater("jmp", "", "", "");
    NEXTw();
    cout << "(while结束)";
    return flag;
}

bool Grammar::For()
{
    //"for"
    if (Token_Search_K_O_S() != "for")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    //"("
    if (Token_Search_K_O_S() != "(")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    {
        // Equal_Sentence
        if (Token_Search_K_O_S() != ";")
        {
            if (Equal_Sentence() == false)
                return flag;
        }
        //";"
        if (Token_Search_K_O_S() != ";")
            flag = false;
        if (flag == false)
            return flag;
        Add_Quater("wh", "", "", "");
        NEXTw();

        // Compare_Sentence
        if (Compare_Define() == false)
            return flag;
        //";"
        if (Token_Search_K_O_S() != ";")
            flag = false;
        if (flag == false)
            return flag;
        string object1 = Temp_Variable + to_string(number_of_t - 1);
        Add_Quater("fj", object1, "", "");
        NEXTw();

        if (Token_Search_K_O_S() != ")")
        {
            // Equal_Sentence
            flag_i_w_f = true;
            flag_temp_store = true;
            if (Equal_Sentence() == false)
                return flag;
            flag_temp_store = false;
            flag_i_w_f = false;
        }
    }
    //")"
    if (Token_Search_K_O_S() != ")")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();
    //"{"
    if (Token_Search_K_O_S() != "{")
        flag = false;
    if (flag == false)
        return flag;
    NEXTw();

    // Sentence_Define
    if (Token_Search_K_O_S() != "}")
        if (Sentence_Define() == false)
            return flag;
    //"}"
    NEXTw();
    while (!temp_quater.empty())
    {
        data.Quater.push_back(temp_quater.front());
        temp_quater.pop();
    }
    Add_Quater("jmp", "", "", "");
    cout << "(for结束)";

    return flag;
}

//
// 递归向下子程序法附属函数
//
string Grammar::Token_Search_K_O_S()
{
    string w = "";
    if (data.Token.Type == "K")
        w = data.Keyword[data.Token.Place];
    else if (data.Token.Type == "O")
        w = data.Operator[data.Token.Place];
    else if (data.Token.Type == "S")
        w = data.Separater[data.Token.Place];
    return w;
}

int Grammar::Add_Quater(string operation, string object1, string object2, string result)
{
    data.Quater.push_back({operation, object1, object2, result});
    return data.Quater.size() - 1;
}

void Grammar::Make_T_Variable(string &name, char type)
{
    name = Temp_Variable + to_string(number_of_t++);
    int type_p = 1;
    if (type == 'i')
        type_p = 0;
    if (type == 'f')
        type_p = 1;
    else if (type == 'b')
        type_p = 2;
    else if (type == 'c')
        type_p = 3;
    Identity_Node temp{name, type_p, "v", {level, 0}};
    data.Identity_Temp.push_back(temp);
}

bool Grammar::Find_Identity(int place, string &location)
{
    Identity_Node &temp_identity = data.Identity[place];
    if (temp_identity.type_p < 4)
        location = temp_identity.name;
    // 该变量为数组元素
    else
    {
        // 找到该变量所对应的数组类型信息
        Array_Node &temp_array = data.Array[data.Type[temp_identity.type_p].array_p];
        NEXTw();
        // 数组逆波兰式生成四元式
        stack<string> SYN_array; // 语法栈
        stack<string> SEM_array; // 语义栈
        int length = temp_array.total_space;
        SEM_array.push(to_string(temp_identity.add.offset));
        // 该变量后需要跟随dimension个[n]
        for (int i = 0; i < temp_array.dimension; ++i)
        {
            //"["
            if (Token_Search_K_O_S() != "[")
            {
                cout << "\n数组元素访问错误\n";
                flag = false;
                return flag;
            }
            SYN_array.push("+");
            length /= temp_array.dimension_length[i];
            SEM_array.push(to_string(length));
            NEXTw();
            // 算术表达式
            if (Math_Expression() == false)
                return flag;
            string object = SEM.top();
            SEM.pop();
            SEM_array.push(object);
            //"]"
            if (Token_Search_K_O_S() != "]")
            {
                flag = false;
                return flag;
            }
            // SYN_array.push('*')、再运算
            string operation = "*";
            string object2 = SEM_array.top();
            SEM_array.pop();
            string object1 = SEM_array.top();
            SEM_array.pop();
            string result;
            Make_T_Variable(result, 'f');
            Add_Quater(operation, object1, object2, result);
            SEM_array.push(result);
            if (i != temp_array.dimension - 1)
                NEXTw();
        }
        // 把栈弹空，计算完结果
        string operation = "+";
        for (int i = 0; i < temp_array.dimension; ++i)
        {
            if (SYN_array.empty())
            {
                cout << "\n数组元素访问错误\n";
                flag = false;
                return flag;
            }
            SYN_array.pop();
            string object2 = SEM_array.top();
            SEM_array.pop();
            string object1 = SEM_array.top();
            SEM_array.pop();
            string result;
            // 最后一个临时变量的特殊处理，做成：:$t
            // 以p类型作为临时变量加入Identity
            if (i != temp_array.dimension - 1)
                Make_T_Variable(result, 'f');
            else
            {
                result = "->t" + to_string(number_of_t++);
                data.Identity_Temp.push_back({result, 4, "p", {level, 0}});
            }
            Add_Quater(operation, object1, object2, result);
            SEM_array.push(result);
        }
        // 最终结果存在SEM_array里
        string result = SEM_array.top();
        SEM_array.pop();
        result = "[f" + to_string(level) + ":" + result + "]";
        // data.Identity_Temp.push_back({ result,4,"p", {level,0} });
        // 将其转化为虚拟地址形式，并返回
        location = result;
    }
    return flag;
}

//
// 算术表达式LL(1)整体处理(=, ;)
//
bool Grammar::Math_Expression()
{
    // 临时变量初始
    char ch;     // 当前处理的单词
    char top_ch; // 分析栈栈顶元素

    ch = Math_Expression_ch();

    SYN.push('#');
    SYN.push('E');

    while (!SYN.empty())
    {
        top_ch = SYN.top();
        SYN.pop();

        if (top_ch == '@')
        {
            string operate = SEM_Operate.top();
            SEM_Operate.pop();
            string object2 = SEM.top();
            SEM.pop();
            string object1 = SEM.top();
            SEM.pop();
            string result;
            Make_T_Variable(result, 'f');
            if (flag_temp_store == false)
                Add_Quater(operate, object1, object2, result);
            else
            {
                Quater_Node temp_q_node{operate, object1, object2, result};
                temp_quater.push(temp_q_node);
            }
            SEM.push(result);
        }
        else if (top_ch == '$')
        {
            SEM.push(SEM_Object);
        }
        else if (top_ch != ch)
        {
            flag = Math_Expression_Search(top_ch, ch);
            if (flag == false)
                break;
        }
        else if (top_ch == '#' && ch == '#')
            break;
        else
        {
            NEXTw();
            ch = Math_Expression_ch();
        }
    }
    return flag;
}

bool Grammar::Math_Expression_Search(char top_ch, char ch)
{
    switch (top_ch)
    {
    case 'E':
        if (ch == 'C' || ch == 'I' || ch == '(')
        {
            SYN.push('e');
            SYN.push('T');
            // printf("E->Te\n");
        }
        else
            flag = false;
        break;
    case 'e':
        if (ch == '+' || ch == '-')
        {
            SYN.push('e');
            SYN.push('@');
            string temp;
            temp += ch;
            SEM_Operate.push(temp);
            SYN.push('T');
            SYN.push(ch);
            // printf("e->%cTe\n",ch);
        }
        else if (ch == ')' || ch == '#')
            ; // printf("e->^\n");
        else
            flag = false;
        break;
    case 'T':
        if (ch == 'C' || ch == 'I' || ch == '(')
        {
            SYN.push('t');
            SYN.push('F');
            // printf("T->Ft\n");
        }
        else
            flag = false;
        break;
    case 't':
        if (ch == '+' || ch == '-' || ch == ')' || ch == '#')
            ; // printf("t->^\n");
        else if (ch == '*' || ch == '/')
        {
            SYN.push('t');
            SYN.push('@');
            string temp;
            temp += ch;
            SEM_Operate.push(temp);
            SYN.push('F');
            SYN.push(ch);
            // printf("t->%cFt\n",ch);
        }
        else
            flag = false;
        break;
    case 'F':
        if (ch == 'C' || ch == 'I')
        {
            SYN.push('$');
            SYN.push(ch);
            // printf("F->%c\n",ch);
        }
        else if (ch == '(')
        {
            SYN.push(')');
            SYN.push('E');
            SYN.push('(');
            // printf("F->(E)\n");
        }
        else
            flag = false;
        break;
    default:
        flag = false;
    }
    return flag;
}

char Grammar::Math_Expression_ch()
{
    char ch = ' ';
    string temp;
    // 遇到整数
    if (data.Token.Type == "CI")
    {
        ch = 'C';
        temp = to_string(data.Constant_int[data.Token.Place]);
        SEM_Object = temp;
    }
    // 遇到小数
    else if (data.Token.Type == "CF")
    {
        ch = 'C';
        temp = to_string(data.Constant_float[data.Token.Place]);
        SEM_Object = temp;
    }
    // 遇到字符
    else if (data.Token.Type == "CC")
    {
        ch = 'C';
        // ASCII码运算
        temp += to_string(data.Constant_char[data.Token.Place]);
        //'a'符号表示
        /*temp = "\'";
        temp +=data.Constant_char[data.Token.Place];
        temp += "\'";*/
        SEM_Object = temp;
    }
    // 遇到true、false
    else if (data.Token.Type == "K")
    {
        ch = 'C';
        temp = data.Keyword[data.Token.Place];
        // true符号表示
        // if (temp != "true" && temp != "false") flag = false;
        // ASCII码运算
        if (temp == "true")
            temp = "1";
        else if (temp == "false")
            temp = "0";
        else
            flag = false;
        SEM_Object = temp;
    }
    // 遇到变量
    else if (data.Token.Type == "I")
    {
        ch = 'I';
        Find_Identity(data.Token.Place, temp);
        SEM_Object = temp;
    }
    // 遇到运算符(+ - * /)
    else if (data.Token.Type == "O")
    {
        temp = data.Operator[data.Token.Place];
        if (temp == "+" || temp == "-" || temp == "*" || temp == "/")
            ch = temp[0];
        else
            ch = '#';
    }
    // 分隔符
    else if (data.Token.Type == "S")
        ch = data.Separater[data.Token.Place];
    else
        flag = false;

    if (ch == ';' || ch == ']')
        ch = '#';
    if (ch == ')' && flag_i_w_f == true)
    {
        char temp;
        while ((temp = infile.peek()) != EOF && (temp == ' ' || temp == '\t' || temp == '\r' || temp == '\n'))
            temp = infile.get(); // 跳过所有过滤符
        temp = infile.peek();
        if (temp == '{')
            ch = '#';
    }
    return ch;
}
