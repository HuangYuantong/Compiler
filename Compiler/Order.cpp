#include "Order.h"

Order::Order(Data &data) : data(data), Optimize(data)
{
	order_set = {
		{"+", "ADD"},
		{"-", "SUB"},
		{"*", "MUL"},
		{"/", "DIV"},
		{"<", "LT"},
		{">", "GT"},
		{"<=", "LE"},
		{">=", "GE"},
		{"!=", "NE"},
		{"==", "EQ"},
		{"&&", "AND"},
		{"||", "OR"},
	};
}
//
// 目标指令生成-及附属
//
void Order::order()
{
	data.Update_Offset(offset);
	data.Identity[0].add.offset = offset;
	Make_Active();
	Active_Node temp_active;
	for (auto i = data.Quater.begin(); i != data.Quater.end(); ++i)
	{
		temp_active = Active.top();
		Active.pop();
		//
		// if、while、for中的跳转语句,及函数相关语句
		//
		if (i->result == "")
		{
			//--------基本块开始--------
			// 跳转到：(wh, , , ) 下一个跳转的返填位置
			if (i->operation == "wh")
			{
				if (RDL.name != "" && RDL.active == true)
					Add_Order("ST", "R", RDL.name);
				RDL.name = "";
				SEM.push(data.Order.size());
			}
			//--------基本块结束--------
			// 向后跳转：(fj,object1, , )
			else if (i->operation == "fj")
			{
				// 若 RDL为空
				if (RDL.name == "")
					Add_Order("LD", "R", i->object1);
				// 若 RDL==object1
				else if (RDL.name == i->object1)
				{
					if (temp_active.object1 == true)
						Add_Order("ST", "R", i->object1);
				}
				// 若 RDL==D（D!=object1）
				else if (RDL.name != i->result)
				{
					if (RDL.active == true)
						Add_Order("ST", "R", RDL.name);
					Add_Order("LD", "R", i->object1);
				}
				SEM.push(Add_Order("FJ", "R", ""));
				RDL.name = "";
			}
			// 向后跳转：(el, , , )
			// 下一句为上一个跳转的返填位置
			else if (i->operation == "el")
			{
				if (RDL.name != "" && RDL.active == true)
					Add_Order("ST", "R", RDL.name);
				// 先返填上一条的转向地址
				int temp = SEM.top();
				SEM.pop();
				data.Order[temp].address = "[ord " + to_string(data.Order.size() + 1) + "]";
				// 再生成本句的转向指令
				SEM.push(Add_Order("JMP", "", ""));
			}
			// 跳转到：(ie, , , ) 上一个跳转的返填位置
			else if (i->operation == "ie")
			{
				if (RDL.name != "" && RDL.active == true)
					Add_Order("ST", "R", RDL.name);
				int temp = SEM.top();
				SEM.pop();
				data.Order[temp].address = "[ord " + to_string(data.Order.size()) + "]";
			}
			// 向前跳转：(jmp, , , )
			// 下一句为上一个跳转的返填位置
			else if (i->operation == "jmp")
			{
				if (RDL.name != "" && RDL.active == true)
					Add_Order("ST", "R", RDL.name);
				RDL.name = "";
				int temp = SEM.top();
				SEM.pop();
				data.Order[temp].address = "[ord " + to_string(data.Order.size() + 1) + "]";
				temp = SEM.top();
				SEM.pop();
				Add_Order("JMP", "", "[ord " + to_string(temp) + "]");
			}
			//--------函数相关--------
			else if (i->operation == "start")
				Add_Order(i->operation, i->object1,
						  "[New " + to_string(data.Identity[0].add.offset) + "]");
			else if (i->operation == "end")
				Add_Order(i->operation, i->object1, "[Release]");
			else
				Add_Order(i->operation, i->object1, "");
		}
		//
		// 操作：=
		//
		else if (i->operation == "=")
		{
			// 若 RDL为空
			if (RDL.name == "")
				Add_Order("LD", "R", i->object1);
			// 若 RDL==object1
			else if (RDL.name == i->object1)
			{
				if (temp_active.object1 == true)
					Add_Order("ST", "R", i->object1);
			}
			// 若 RDL==D（D!=object1 和 result）
			else if (RDL.name != i->result)
			{
				if (RDL.active == true)
					Add_Order("ST", "R", RDL.name);
				Add_Order("LD", "R", i->object1);
			}
			RDL = {i->result, temp_active.result};
		}
		//
		// 操作：其他操作符
		//
		else
		{
			// 若 RDL为空
			if (RDL.name == "")
			{
				Add_Order("LD", "R", i->object1);
				Add_Order(i->operation, "R", i->object2);
			}
			// 若 RDL==object1
			else if (RDL.name == i->object1)
			{
				if (temp_active.object1 == true)
					Add_Order("ST", "R", i->object1);
				Add_Order(i->operation, "R", i->object2);
			}
			// 若 RDL==object2
			else if (RDL.name == i->object2)
			{
				// 有可交换性的运算
				if (i->operation == "==" || i->operation == "!=" || i->operation == "+" || i->operation == "-" || i->operation == "*" || i->operation == "&&" || i->operation == "||")
				{
					if (temp_active.object2 == true)
						Add_Order("ST", "R", i->object2);
					Add_Order(i->operation, "R", i->object1);
				}
			}
			// 其他情况
			else
			{
				if (RDL.name != "" && RDL.active == true)
					Add_Order("ST", "R", RDL.name);
				Add_Order("LD", "R", i->object1);
				Add_Order(i->operation, "R", i->object2);
			}
			RDL = {i->result, temp_active.result};
		} // else

	} // for
}

int Order::Add_Order(string operation, string rgst, string object)
{
	Order_Node temp_order;
	// 第一项：目标指令
	auto temp = order_set.find(operation);
	if (temp != order_set.end())
		temp_order.operation = temp->second;
	else
		temp_order.operation = operation;
	// 第二项：rgst
	temp_order.rgst = rgst;
	// 第三项：虚拟内存位置(在I表offset)
	// 常数、为空、虚拟位置、命令时
	if (('0' <= object[0] && object[0] <= '9') || object[0] == '-' || object == "" || object[0] == '[')
		temp_order.address = object;
	// 变量名时
	else
	{
		int temp_offset = 0, size = data.Identity.size(), i = 0;
		for (; i < size; ++i)
		{
			if (data.Identity[i].name == object)
			{
				temp_offset = data.Identity[i].add.offset;
				break;
			}
		}
		if (i < size)
			temp_order.address = "[f" + to_string(level) + ":" + to_string(temp_offset) + "]";
		else
			temp_order.address = object;
	}
	data.Order.push_back(temp_order);
	return data.Order.size() - 1;
}

//
// 活跃信息表-及附属
//
void Order::Make_Active()
{
	int size = data.Quater.size();
	for (int i = size - 1; i >= 0; --i)
	{
		Active_Node temp;
		if (data.Quater[i].result == "")
		{
			// cout << "进行一次初始化\n";
			Reset_Identity_Active();
		}
		if (data.Quater[i].object1[0] == '[' || data.Quater[i].object1[0] == '-')
			temp.object1 = true;
		else if (data.Quater[i].object1[0] == '$' || ('A' <= data.Quater[i].object1[0] && data.Quater[i].object1[0] <= 'z'))
			temp.object1 = Search_Change(data.Quater[i].object1, true);
		if (data.Quater[i].object2[0] == '[' || data.Quater[i].object2[0] == '-')
			temp.object2 = true;
		else if (data.Quater[i].object2[0] == '$' || ('A' <= data.Quater[i].object2[0] && data.Quater[i].object2[0] <= 'z'))
			temp.object2 = Search_Change(data.Quater[i].object2, true);
		if (data.Quater[i].result[0] == '[' || data.Quater[i].result[0] == '-')
			temp.result = true;
		else if (data.Quater[i].result[0] == '$' || ('A' <= data.Quater[i].result[0] && data.Quater[i].result[0] <= 'z'))
			temp.result = Search_Change(data.Quater[i].result, false);
		Active.push(temp);
		// cout << "    "<<i<<" (" << Active.top().object1 << ", " << Active.top().object2 << ", " << Active.top().result << ")\n";
	}
	// system("pause");
}

void Order::Reset_Identity_Active()
{
	int size = data.Identity.size();
	for (int i = 0; i < size; ++i)
	{
		if (data.Identity[i].name[0] == '$')
			data.Identity[i].active = false;
		else
			data.Identity[i].active = true;
	}
}

bool Order::Search_Change(string name, bool to)
{
	bool temp = false;
	int size = data.Identity.size();
	for (int i = 0; i < size; ++i)
	{
		if (data.Identity[i].cat == "v")
		{
			if (data.Identity[i].name == name)
			{
				temp = data.Identity[i].active;
				data.Identity[i].active = to;
				break;
			}
		}
	}
	return temp;
}