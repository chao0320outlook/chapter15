// chapter15.20.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#ifndef BASE_H
#define BASE_H

class  Base
{
public:
	void pub_mem();
protected:
	int pro_mem;
private:
	char pri_mem;
};

struct Pub_derv :public Base
{
	int f() { return pro_mem; }

	//错误，私有成员不可访问
	char g() { return pri_mem; }
};
struct Pri_derv :private Base
{
	int f1() { return pro_mem; }

	//错误，私有成员也不可访问
	char g1() { return pri_mem; }
};
struct Pro_derv :protected Base
{
	int f2() { return pro_mem; }

	//错误，私有成员也不可访问
	char g3() { return pri_mem; }
};

struct Derivated_from_Public :public Pub_derv
{
	int use() { return pro_mem; }
};
struct Derivated_from_Protected :public Pro_derv
{
	int use() { return pro_mem; }
};
struct Derivated_from_Private :public Pri_derv
{
	//错误，不可访问
	int use() { return pro_mem; }
};
#endif BASE_H


int main()
{
	Pub_derv d1;
	Pri_derv d2;
	Pro_derv d3;
	Derivated_from_Public dd1;
	Derivated_from_Protected dd2;
	Derivated_from_Private dd3;
	d1.pub_mem();
	d2.pub_mem();             //错误，不可访问，该对象成员都为私有
	d3.pub_mem();
	dd1.pub_mem();
	dd2.pub_mem();
	dd3.pub_mem();

	Base *p = &d1;
	p = &d2;//不允许对不可访问的基类进行转换
	p = &d3;//不允许对不可访问的基类进行转换
	p = &dd1;
	p = &dd2;//不允许对不可访问的基类进行转换
	p = &dd3;//不允许对不可访问的基类进行转换


	/*新版本以下两句可以略去*/
	system("pause");
	return 0;
}

