#include <iostream>

#include "Grammar.h"
#include "Order.h"
using namespace std;

int main()
{
	Data data, test_data;
	// 是否重新编译的标记
	bool need_compile = true;
	// 进入主菜单
	while (1)
	{
		system("CLS");
		system("color F9");
		// 编译器前端
		if (need_compile)
		{
			Vision('0');
			data.Clean();
			if (data.Struct())
			{
				Grammar grammar(data);
				grammar.grammar();
				if (flag)
				{
					cout << "\n            ╭-----------------------╮\n";
					cout << "            ║    > 语法分析正确 <   ║\n";
					cout << "            ╰-----------------------╯\n";
					need_compile = false;
					test_data = data;
					system("pause");
				}
				else
				{
					system("color 04");
					cout << "\n            ╭-----------------------╮\n";
					cout << "            ║    * 语法分析错误 *   ║\n";
					cout << "            ╰-----------------------╯\n";
					system("pause");
					continue;
				}
			}
			else
			{
				Vision('e');
				system("pause");
				continue;
			}
		}
		else
			test_data = data;
		// 编译器后端
		Vision('m');
		char c;
		cin >> c;
		while (c < '0' || c > '3')
		{
			cout << "\n  ·输入错误，请重新选择:";
			cin.ignore(1024, '\n');
			cin >> c;
		}
		cin.ignore(1024, '\n');
		if (c == '0')
			break;
		// 不进行优化需将所有临时变量压入I表中
		else if (c == '1')
		{
			Order order(test_data);
			test_data.Push_All_Identity_Temp();
			order.order();
			test_data.Show();
		}
		// 进行优化调用optimize()
		else if (c == '2')
		{
			Order order(test_data);
			order.optimize();
			order.order();
			test_data.Show();
		}
		else
			need_compile = true;
	}
	return 0;
}
