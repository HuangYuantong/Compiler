#include "Optimize.h"
string Related_word[] = {"wh", "le", "ie", "for", "if", "jmp", "fj", "end"};

Optimize::Optimize(Data &data) : data(data)
{
    int place = data.Identity.size() - 1;
    offset = data.Identity[place].add.offset;
    offset += data.Find_Size(data.Identity[place].type_p);
}

void Optimize::optimize()
{
    cnt = 0;

    // 输入四元式数量以及四元式内容
    int size = data.Quater.size();
    for (int i = 0; i < size; i++)
    {
        // 判断是否两个都是数字，是的话需要计算存储
        if (Is_S(data.Quater[i].object1[0]) && Is_S(data.Quater[i].object2[0]))
        {
            double a1 = str_change_num(data.Quater[i].object1);
            double a2 = str_change_num(data.Quater[i].object2);
            data.Quater[i].object1 = operatornum(data.Quater[i].operation, a1, a2);
            data.Quater[i].object2 = "";
            data.Quater[i].operation = "=";
        }
        // 使用add_node建立结点
        int l = add_node(data.Quater[i].object1); // 左子树
        int r = add_node(data.Quater[i].object2); // 右子树
        string result = data.Quater[i].result;
        // 将表达式构成树
        add_operator(data.Quater[i].result, data.Quater[i].operation, l, r);
        int temp1;
        temp1 = DAG[cnt - 1].left;
        replace_DAG(cnt, temp1, i); // 替换多余变量，大多是临时变量
    }
    cout_DAG(cnt); // 计算再优化
    flas = new bool[cnt];
    // 优化四元式并存储
    for (int i = 0; i < cnt; i++)
    {
        // 有左子树的节点就是一个表达式
        if (DAG[i].left != -1)
        {
            // 得到存储的替代值
            temporary_ans.result = ok(i);
            // 左右子树节点
            DAG_Node ll = DAG[DAG[i].left];
            DAG_Node rr = DAG[DAG[i].right];

            // 如果这个结点的左子树不是空的，值就是他的var
            // 如果是空的，那就是左子树本身的值
            temporary_ans.object1 = ll.left != -1 ? ok(DAG[i].left) : ll.id;
            temporary_ans.operation = DAG[i].id;
            temporary_ans.object2 = rr.left != -1 ? ok(DAG[i].right) : rr.id;

            ans.push_back(temporary_ans);
            // ans[i][4] = "\0";
        }
        else // 没有左子树为空
        {
            temporary_ans.result = "";
            temporary_ans.object1 = "";
            temporary_ans.operation = "";
            temporary_ans.object2 = "";
            ans.push_back(temporary_ans);
        }
    }
    for (int i = 0; i < cnt; i++) // 每个基本块内标记应该保留的四元式
    {
        int first = i;
        for (int h = first + 1; h < cnt; h++)
        {
            if (find_Related_word(ans[h].operation)) // 遇到基本块的结束点
            {
                int last = h;
                new_Identity = data.Identity;
                for (int m = last; m >= first; m--) // 基本块内逆序查找，防止多次定义
                    if (find_Identity(ans[m].result) || find_Related_word(ans[m].operation))
                        dfs(m);
                break;
            }
        }
    }

    for (int i = 0; i < cnt; i++)
    {
        if (flas[i] == 1)
        {
            new_Q.operation = ans[i].operation;
            new_Q.object1 = ans[i].object1;
            new_Q.object2 = ans[i].object2;
            new_Q.result = ans[i].result;
            find_Identity_Temp(new_Q.result);
            new_Quater.push_back(new_Q);
        }
    }
    data.Quater.swap(new_Quater);
}

void Optimize::replace_DAG(int cnt, int temp1, int i)
{
    if (DAG[cnt - 1].id == "=" && data.Quater[i].object2 == "") // 删掉赋值等式里不需要的变量链接，如一些非临时变量
    {

        if (DAG[temp1].id == "=")
        {

            DAG[cnt - 1].left = DAG[temp1].left;
            DAG[cnt - 1].right = DAG[temp1].right;
        }
        else if (DAG[temp1].id == "+" || DAG[temp1].id == "-" || DAG[temp1].id == "*" || DAG[i].id == "/")
        {
            DAG[cnt - 1].id = DAG[temp1].id;
            DAG[cnt - 1].left = DAG[temp1].left;
            DAG[cnt - 1].right = DAG[temp1].right;
        }
    }
}

void Optimize::cout_DAG(int cnt)
{
    int nn = 0;
    for (int i = 0; i < cnt - 1; i++)
    {
        nn = i;
        int l = -2;
        int r = -2;
        if (DAG[nn].id == "=" && DAG[DAG[nn].left].id == "=")
        {
            DAG[nn].left = DAG[DAG[nn].left].left;
        }
        if (DAG[i].id == "+" || DAG[i].id == "-" || DAG[i].id == "*" || DAG[i].id == "/") // 判断是不是运算的四元式
        {
            l = DAG[i].left;
            r = DAG[i].right;
        }
        else
            continue;
        if (Is_S(DAG[l].id[0]) && Is_S(DAG[r].id[0])) // 两个object是数
        {
            double a1 = str_change_num(DAG[l].id);
            double a2 = str_change_num(DAG[r].id);
            string object1 = operatornum(DAG[i].id, a1, a2);
            string object2 = "";
            string operation = "=";
            int ll = add_node(object1); // 左子树
            int rr = add_node(object2); // 右子树
            DAG[i].id = operation;
            DAG[i].left = ll;
            DAG[i].right = rr;
        }
        else if (Is_S(DAG[l].id[0]) && !Is_S(DAG[r].id[0])) // object1是数
        {
            if (DAG[r].id == "=")
            {
                r = DAG[r].left;
            }
            else
                continue;
            if (Is_S(DAG[r].id[0]))
            {
                double a1 = str_change_num(DAG[l].id);
                double a2 = str_change_num(DAG[r].id);
                string object1 = operatornum(DAG[i].id, a1, a2);
                string object2 = "";
                string operation = "=";
                int ll = add_node(object1); // 左子树
                int rr = add_node(object2); // 右子树
                DAG[i].id = operation;
                DAG[i].left = ll;
                DAG[i].right = rr;
            }
            else
                continue;
        }
        else if (!Is_S(DAG[l].id[0]) && Is_S(DAG[r].id[0])) // object2是数
        {
            if (DAG[l].id == "=")
            {
                l = DAG[l].left;
            }
            else
                continue;
            if (Is_S(DAG[l].id[0]))
            {
                double a1 = str_change_num(DAG[l].id);
                double a2 = str_change_num(DAG[r].id);
                string object1 = operatornum(DAG[i].id, a1, a2);
                string object2 = "";
                string operation = "=";
                int ll = add_node(object1); // 左子树
                int rr = add_node(object2); // 右子树
                DAG[i].id = operation;
                DAG[i].left = ll;
                DAG[i].right = rr;
            }
            else
                continue;
        }
        else // 都不是数
        {
            if (DAG[l].id == "=" && DAG[r].id == "=")
            {
                l = DAG[l].left;
                r = DAG[r].left;
            }
            else
                continue;
            if (Is_S(DAG[l].id[0]) && Is_S(DAG[r].id[0]))
            {
                double a1 = str_change_num(DAG[l].id);
                double a2 = str_change_num(DAG[r].id);
                string object1 = operatornum(DAG[i].id, a1, a2);
                string object2 = "";
                string operation = "=";

                int ll = add_node(object1); // 左子树
                int rr = add_node(object2); // 右子树
                DAG[i].id = operation;
                DAG[i].left = ll;
                DAG[i].right = rr;
            }
            else
                continue;
        }
    }
}

void Optimize::find_Identity_Temp(string C)
{
    int t = 0;
    int length = data.Identity_Temp.size();
    for (int j = 0; j < length; j++)
    {
        if (C == data.Identity_Temp[j].name)
        {
            temporary_Identity_Temp.active = data.Identity_Temp[j].active;
            temporary_Identity_Temp.add = data.Identity_Temp[j].add;
            temporary_Identity_Temp.cat = data.Identity_Temp[j].cat;
            temporary_Identity_Temp.name = data.Identity_Temp[j].name;
            temporary_Identity_Temp.type_p = data.Identity_Temp[j].type_p;
            temporary_Identity_Temp.add.info = 0;
            temporary_Identity_Temp.add.offset = offset;
            // 修改当前offset记录变量
            offset += data.Find_Size(temporary_Identity_Temp.type_p);
            data.Identity.push_back(temporary_Identity_Temp);
            swap(data.Identity_Temp[j], data.Identity_Temp[length - 1]); // 删除已经用过的标识符
            data.Identity_Temp.pop_back();
            break;
        }
        else
            t = t + 1;
    }
}

bool Optimize::find_Identity(string C)
{
    if (C[0] == '[' || C[0] == '-')
        return true;
    int t = 0;
    int length = new_Identity.size();
    for (int j = 0; j < length; j++)
    {
        if (C == new_Identity[j].name)
        {
            swap(new_Identity[j], new_Identity[length - 1]); // 删除已经用过的标识符
            new_Identity.pop_back();
            return true;
        }
        else
            t = t + 1;
    }
    if (t == length)
        return false;
    return false;
}

int Optimize::find_Related_word(string c)
{
    for (int j = 0; j < 8; j++)
        if (c == Related_word[j])
            return 1;
    return 0;
}

bool Optimize::find_var(int i, string c)
{
    // 遍历一个结点的所有值
    int len = DAG[i].var.size();
    for (int k = 0; k < len; k++)
        if (DAG[i].var[k] == c)
            return true;
    return false;
}

int Optimize::add_node(string c)
{
    // 遍历当前已经建立的结点
    for (int j = cnt - 1; j >= 0; j--)
        // 如果该结点建立过，或者和其他结点运算内容一样。合并这些变量
        if (DAG[j].id == c || find_var(j, c))
            return j;
    // 需要新建立结点
    dag.id = c;
    DAG.push_back(dag);
    dag.id = "";
    dag.left = -1;
    dag.right = -1;
    dag.var.clear();
    // if(c=="")return -1;
    return cnt++;
}

void Optimize::add_operator(string c, string op, int l, int r)
{
    string temp;
    for (int j = cnt - 1; j >= 0; j--)
    {
        // 如果该表达式已经存在，将该表达式左边的值放到结点的var中即可。
        if (op == DAG[j].id && DAG[j].left == l && DAG[j].right == r)
        {
            num = j;
            DAG[j].var.push_back(c);
            if (find_Identity(c) && !find_Identity(DAG[j].var[0])) // c是非临时变量，把它放在var[0]处
            {
                int length = DAG[j].var.size();
                temp = DAG[j].var[0];
                DAG[j].var[0] = c;
                DAG[j].var[length - 1] = temp;
            }
            return;
        }
    }
    // 如果没有，则需要把整个表达式都加进去
    // id是运算符符，左右结点为变量，结果存到父节点的var中
    dag.id = op;
    dag.left = l;
    dag.right = r;
    dag.var.push_back(c);
    DAG.push_back(dag);
    dag.id = "";
    dag.left = -1;
    dag.right = -1;
    dag.var.clear();
    cnt++;
    num = cnt - 1;
}

void Optimize::dfs(int i)
{

    if (DAG[i].left != -1)
    {
        flas[i] = 1;
        dfs(DAG[i].left); // 左右子树的值的表达式需不需要保留
        dfs(DAG[i].right);
    }
    else
        flas[i] = 0;
}

string Optimize::ok(int i)
{
    int len = DAG[i].var.size();
    return DAG[i].var[0];
}

string Optimize::operatornum(string a, double b, double c)
{
    double x = 0.0;
    stringstream ss; // stringstream需要sstream头文件
    string str;
    if (a == "/" && b != 0.0)
        x = b / c;
    else if (a == "/" && b == 0.0)
        x = 0.0;
    else if (a == "*")
        x = b * c;
    else if (a == "-")
        x = b - c;
    else if (a == "+")
        x = b + c;
    ss << x;
    ss >> str;
    return str;
}

inline double Optimize::str_change_num(string a)
{
    double b = atof(a.c_str());
    return b;
}
