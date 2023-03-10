#include "Morphology.h"

Morphology::Morphology(Data &a) : data(a) {}

//
// 判断函数
//
bool Morphology::IsFilter(char ch)
{
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
        return true;
    return false;
}

int Morphology::IsSeparater(char ch)
{
    for (int i = 0; i < data.Separater.size(); ++i)
    {
        if (data.Separater[i] == ch)
        {
            cout << ch;
            Token_Node token;
            token.Type = "S";
            token.Place = i;
            data.Token = token;
            return i;
        }
    }
    return NotFind;
}

int Morphology::IsKeyword(string word)
{
    for (int i = 0; i < data.Keyword.size(); ++i)
        if (data.Keyword[i] == word)
        {
            cout << word;
            Token_Node token;
            token.Type = "K";
            token.Place = i;
            data.Token = token;
            return i;
        }
    return NotFind;
}

int Morphology::IsOperator(char ch)
{
    string word = "";
    // 发现一个2字符的运算符
    if (((ch == '<' || ch == '=' || ch == '>' || ch == '!') && infile.peek() == '=') //<=、==、>=、!=
        || (ch == '+' && (infile.peek() == '+' || infile.peek() == '='))             //++、+=
        || (ch == '-' && (infile.peek() == '-' || infile.peek() == '='))             //--、-=
        || (ch == '&' && (infile.peek() == '&'))                                     //&&
        || (ch == '|' && (infile.peek() == '|'))                                     //||
    )
    {
        word += ch;
        ch = infile.get();
        word += ch;
    }
    // 发现一个1字符的运算符
    else
        word += ch;

    for (int i = 0; i < data.Operator.size(); ++i)
        if (data.Operator[i] == word)
        {
            cout << word;
            Token_Node token;
            token.Type = "O";
            token.Place = i;
            data.Token = token;
            return i;
        }
    return NotFind;
}

int Morphology::IsIdentity(string word)
{
    for (int i = 0; i < data.Identity.size(); ++i)
    {
        if (data.Identity[i].name == word)
        {
            //
            // 变量重复声明
            // 检测，若当前可进行变量声明，则重复定义
            cout << word;
            if (flag_var == true)
            {
                flag = false;
                cout << "变量重复声明\n";
            }
            Token_Node token;
            token.Type = "I";
            token.Place = i;
            data.Token = token;
            return i;
        }
    }
    // 如果I表没有找到，加入
    //
    // 使用未声明变量
    // 检测，若当前不可进行变量声明，则未声明变量
    cout << word;
    if (flag_var == false)
    {
        flag = false;
        cout << "使用未声明的变量\n";
    }
    Identity_Node temp;
    temp.name = word;
    temp.type_p = NULL;
    data.Identity.push_back(temp);
    Token_Node token;
    token.Type = "I";
    token.Place = data.Identity.size() - 1;
    data.Token = token;
    return data.Identity.size() - 1;
}

int Morphology::IsConstant_int(int number)
{
    for (int i = 0; i < data.Constant_int.size(); ++i)
    {
        if (data.Constant_int[i] == number)
        {
            cout << number << "(整数)";
            Token_Node token;
            token.Type = "CI";
            token.Place = i;
            data.Token = token;
            return i;
        }
    }
    // 如果CI表没有找到，加入
    cout << number << "(整数)";
    data.Constant_int.push_back(number);
    Token_Node token;
    token.Type = "CI";
    token.Place = data.Constant_int.size() - 1;
    data.Token = token;
    return data.Constant_int.size() - 1;
}

int Morphology::IsConstant_float(float number)
{
    for (int i = 0; i < data.Constant_float.size(); ++i)
    {
        if (data.Constant_float[i] == number)
        {
            cout << number << "(浮点数)";
            Token_Node token;
            token.Type = "CF";
            token.Place = i;
            data.Token = token;
            return i;
        }
    }
    // 如果CF表没有找到，加入
    cout << number << "(浮点数)";
    data.Constant_float.push_back(number);
    Token_Node token;
    token.Type = "CF";
    token.Place = data.Constant_float.size() - 1;
    data.Token = token;
    return data.Constant_float.size() - 1;
}

int Morphology::IsConstant_char(char ch)
{
    for (int i = 0; i < data.Constant_char.size(); ++i)
    {
        if (data.Constant_char[i] == ch)
        {
            cout << ch << "(字符)";
            Token_Node token;
            token.Type = "CC";
            token.Place = i;
            data.Token = token;
            return i;
        }
    }
    // 如果CC表没有找到，加入
    cout << ch << "(字符)";
    data.Constant_char.push_back(ch);
    Token_Node token;
    token.Type = "CC";
    token.Place = data.Constant_char.size() - 1;
    data.Token = token;
    return data.Constant_char.size() - 1;
}

int Morphology::IsConstant_string(string word)
{
    for (int i = 0; i < data.Constant_string.size(); ++i)
    {
        if (data.Constant_string[i] == word)
        {
            cout << word << '\"' << "(字符串)";
            Token_Node token;
            token.Type = "CS";
            token.Place = i;
            data.Token = token;
            return i;
        }
    }
    // 如果CS表没有找到，加入
    cout << word << "(字符串)";
    data.Constant_string.push_back(word);
    Token_Node token;
    token.Type = "CS";
    token.Place = data.Constant_string.size() - 1;
    data.Token = token;
    return data.Constant_string.size() - 1;
}

//
// morphology函数
//
int Morphology::NEXTw()
{
    int place = -1;
    char ch = ' ';
    string word = "";

    while ((ch = infile.peek()) != EOF && (IsFilter(ch)))
    {
        ch = infile.get(); // 跳过所有过滤符
        cout << ch;
    }
    while ((ch = infile.get()) != EOF) // 读一个单词
    {
        // 判断是否为分隔符
        if ((place = IsSeparater(ch)) != NotFind)
        {
            //''的特殊情况
            if (ch == '\'')
            {
                ch = infile.get();
                if (ch != '\'') // 2个'间有内容
                {
                    if (ch == '\\') // 遇到转义符号
                    {
                        ch = infile.get();
                        if (ch == '\'')
                            ch = '\'';
                        else if (ch == '\\')
                            ch = '\\';
                        else if (ch == 't')
                            ch = '\t';
                        else if (ch == 'r')
                            ch = '\r';
                        else if (ch == 'n')
                            ch = '\n';
                        else
                        {
                            cout << "\n 字符表达式输出错误\n";
                            flag = false;
                            return Wrong;
                        }
                    }
                    IsConstant_char(ch);
                    ch = infile.get();
                }
                if (ch != '\'')
                {
                    cout << "\n 字符表达式输出超过一个字符\n";
                    flag = false;
                    return Wrong;
                }
            }
            //""的特殊情况
            else if (ch == '"')
            {
                while (infile.peek() != '"')
                {
                    ch = infile.get();
                    word += ch;
                }
                if (word != "")
                    IsConstant_string(word);
                // 读掉第二个"
                infile >> ch;
            }
            break;
        }

        // 判断是否为运算符
        else if ((place = IsOperator(ch)) != NotFind)
            break;

        // 判断是否为关键字：K 或标识符：I
        else if (Is_Dz_Xz(ch)) // 只能是字母开头
        {
            char type = 'K';
            word += ch;
            while (Is_Dz_Xz_S_X(infile.peek())) // 读入字符串：关键字或标识符
            {
                if (type == 'K') // 若出现数字、大写字母、下划线则不是K
                    if (('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'Z') || (ch == '_'))
                        type = 'I';
                ch = infile.get();
                word += ch;
            }
            if (type == 'K')
                place = IsKeyword(word);

            if (type == 'I' || place == NotFind)
                place = IsIdentity(word); // 不是关键字则一定是标识符
            break;
        }

        // 判断是否为常数
        else if (Is_S(ch)) // 只能是数字开头
        {
            if (Number_Generator(ch))
                break;
            else
            {
                flag = false;
                return Wrong;
            }
        }

        else // 其他未定义字符
        {
            cout << "\n 使用了未定义的字符\n";
            flag = false;
            return Wrong;
        }

    } // while()

    if (ch == EOF) // 文件已读完
    {
        data.Token.Type = "#"; // 最后Token加一个(#,-1)
        data.Token.Place = -1;
        return EOF;
    }
    else
    {
        // cout << "(" << data.Token.Type << "," << data.Token.Place << ")\n";
        return Succeed;
    }
} // NEXTw()

bool Morphology::Number_Generator(char ch)
{
    // 分别是：数字、指数、小数位数、指数符号1/-1、类型0/1
    int n, e, m, e_state, type;
    e = m = type = 0;
    e_state = 1;
    // 完成前半部分n、m、type的构造
    n = ch - '0';
    while (Is_S(infile.peek()) || infile.peek() == '.')
    {
        if (infile.peek() != '.')
        {
            ch = infile.get();
            n = (10 * n + (ch - '0'));
            if (type == 1)
                m++;
        }
        // infile.peek()为小数点'.'时
        else if (type == 1)
        {
            cout << "\n 错误使用多个小数点\n";
            return false;
        }
        else
        {
            type = 1;
            ch = infile.get();
        }
    }
    // 若为科学计数法有e
    if (infile.peek() == 'e')
    {
        ch = infile.get();
        if (infile.peek() == '-')
        {
            ch = infile.get();
            e_state = -1;
        }
        if (infile.peek() == '+')
            ch = infile.get();
        while (Is_S(infile.peek()))
        {
            ch = infile.get();
            e = (10 * e + (ch - '0'));
        }
    }
    if (type == 0)
    {
        int place = IsConstant_int(n);
    }
    else if (type == 1)
    {
        float number = float(n * pow(10, (e_state * e) - m));
        int place = IsConstant_float(number);
    }
    if (Is_Dz_Xz_S_X(infile.peek()))
    {
        cout << "\n 数字后连接非法字符\n";
        return false;
    }
    return true;
}