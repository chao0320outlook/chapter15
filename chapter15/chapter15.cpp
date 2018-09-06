// chapter15.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

using namespace std;

class Quote
{
public:
	Quote() = default;
	Quote(const string &book, double sales_price):bookNo(book),price(sales_price){}

	string isbn()const { return bookNo; }
	virtual double net_price(size_t n)const { return n * price; }
	virtual ~Quote() = default;

private:
	string bookNo;

protected:
	double price = 0.0;
};

int main()
{
    
}

