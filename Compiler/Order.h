#pragma once

#include "Optimize.h"

typedef struct Active_Node // 活跃信息表节点
{
	bool object1 = false;
	bool object2 = false;
	bool result = false;
} Active_Node;

typedef struct RDL_Node // 寄存器描述表节点
{
	string name = "";
	bool active = false;
} RDL_Node;

class Order : public Optimize
{
public:
	Order(Data &data); // 构造函数
	void order();	   // 将四元式生成目标指令

private:
	Data &data; // 引用操作data

	// 目标指令生成-附属
	RDL_Node RDL;												 // 寄存器描述表
	stack<int> SEM;												 // 语义栈,登记待返填的目标地址
	int Add_Order(string operation, string rgst, string object); // 添加一条目标指令，返回其位次
	map<string, string> order_set;								 // 目标指令集合

	// 活跃信息表-及附属
	stack<Active_Node> Active;				  // 四元式活跃信息存储
	void Make_Active();						  // 填写活跃信息表Active
	void Reset_Identity_Active();			  // 初始化I表中的活跃信息
	bool Search_Change(string name, bool to); // 返回name的活跃信息，并将其改为to
};
