// chapter15.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>
#include "QueryResult.h"


using namespace std;

class Quote
{
	friend double print_total(ostream &os, const Quote &, size_t);
public:
	Quote() = default;
	Quote(const string &book,double sales_price):bookNo(book),price(sales_price){}

	virtual Quote* clone()const & { return new Quote(*this); }
	virtual Quote* clone() && { return new Quote(std::move(*this)); }

	Quote(const Quote&) = default;
	Quote( Quote&&) = default;
	Quote& operator=(const Quote&) = default;
	Quote& operator=(Quote&&) = default;

	const string & isbn() const { return bookNo; }
	virtual double net_price(size_t n)const { return n * price; }   
	virtual ~Quote() = default;           

	virtual void debug()const
	{
		cout << "Quote" << endl;
		cout << "ISBN: " << bookNo << endl;
		cout << "Price: " << price << endl;
	}
	void changge_name() { bookNo = "龙族I"; }
private:
	string bookNo;                 //数目编号

protected:
	double price = 0.0;
};
double print_total(ostream & os, const Quote &quote, size_t n)
{
	auto ret = quote.net_price(n);
	os << "ISBN:" << quote.isbn() << endl;
	os << "sold:" << n << " total prices:" << ret << endl;
	return ret;
}

class Disc_quote :public Quote
{
public:
	Disc_quote() = default;
	Disc_quote(const string& book, double sales_price, size_t n, double dis) :
		Quote(book, sales_price), min_number(n), discount(dis) {}

	Disc_quote(const Disc_quote& temp):Quote(temp), 
		min_number(temp.min_number), discount(temp.discount)
	{
		cout << "Disc 复制构造" << endl;
	}
	Disc_quote(Disc_quote&& temp) :Quote(std::move(temp)),
		min_number(std::move(temp.min_number)), discount(std::move(temp.discount))
	{
		cout << "Disc 移动构造" << endl;
	}
	
	Disc_quote& operator=(const Disc_quote& temp)
	{
		Quote::operator=(temp);
		min_number = temp.min_number;
		discount = temp.discount;
		cout << "Disc 复制赋值运算" << endl;
		return  *this;
	}
	Disc_quote& operator=(Disc_quote&& temp)
	{
		if (this != &temp)
		{
			Quote::operator=(std::move(temp));
			min_number =std::move(temp.min_number);
			discount = std::move(temp.discount);
		}
		cout << "Disc 移动赋值运算" << endl;
		return *this;
	}
	double net_price(size_t n) const = 0;   //纯虚函数

	const size_t & show_min_number()const { return min_number; }
	const double & show_discount()const { return discount; }
private:
	size_t min_number = 0;    //优惠最少数量
	double discount = 0.0;    //打折折扣
};

class bulk_quote : public Disc_quote
{
public:
	bulk_quote() = default;
	bulk_quote(const string& book, double sales_price, size_t n, double dis) :
		Disc_quote(book,sales_price,n,dis){}

    bulk_quote* clone()const & { return new bulk_quote(*this); }
	bulk_quote* clone() && { return new bulk_quote(std::move(*this)); }

	bulk_quote(const bulk_quote& temp):Disc_quote(temp)
	{
		cout << "Bulk 复制构造" << endl;
	}
	bulk_quote(bulk_quote&& temp) :Disc_quote(std::move(temp)) 
	{
		cout << "Bulk 移动构造" << endl;
	}

	bulk_quote& operator=(const bulk_quote& temp)
	{
		Disc_quote::operator=(temp);
		cout << "Bulk 复制赋值运算" << endl;
		return *this;
	}
	bulk_quote& operator=(bulk_quote&& temp)
	{
		Disc_quote::operator=(std::move(temp));
		cout << "Bulk 移动赋值运算" << endl;
		return *this;
	}

	double net_price(size_t n)const override;

	void debug()const override
	{
		cout << "Bulk_Quote" << endl;
		cout << "ISBN: " << isbn() << endl;
		cout << "Price: " << price << endl;
		cout << "折扣: " << show_discount() << endl;
		cout << "折扣数目: " << show_min_number() << endl;
	}
};
double bulk_quote::net_price(size_t n) const
{
	if (n >= show_min_number())
		return n * (1 - show_discount())*price;
	else
		return n * price;
}

class number_quote : public Disc_quote
{
public:
	number_quote() = default;
	//using Disc_quote::Disc_quote;//继承构造函数 或使用下面的两行程序
	number_quote(const string& book,double sales_price, size_t n, double dis) :
		Disc_quote(book, sales_price, n, dis) {}

	number_quote* clone()const & { return new number_quote(*this); }
	number_quote* clone() && { return new number_quote(std::move(*this)); }

	//若不定义复制构造和复制赋值，系统将合成，并使用上一级的复制构造和复制赋值
	//默认情况不会合成移动构造和移动赋值

	double net_price(size_t n)const override;

	void debug()const override
	{
		cout << "Number_Quote" << endl;
		cout << "ISBN: " << (*this).isbn() << endl;
		cout << "Price: " << price << endl;
		cout << "折扣: " << show_discount() << endl;
		cout << "折扣数目: " << show_min_number() << endl;
	}
};
double number_quote::net_price(size_t n) const
{
	if (n >= show_min_number())
		return price* show_min_number() +(n- show_min_number())*(1- show_discount())*price;
	else
		return n * price;
}

class Basket
{
public:
	void add_item(const Quote& sale)
	{
		items.insert(shared_ptr<Quote>(sale.clone()));
	}
	void add_item(Quote &&sale)
	{
		items.insert(shared_ptr<Quote>(std::move(sale).clone()));
	}

	double total_receipt(ostream& os) const;
private:
	static bool compare(const shared_ptr<Quote> &lhs,
		const shared_ptr<Quote> &rhs)
	{
		return lhs->isbn() < rhs->isbn();
	}
	multiset<shared_ptr<Quote>, decltype(compare)*> items{ compare };
};
double Basket::total_receipt(ostream & os) const
{
	double sum = 0.0;
	for (auto ptr=items.cbegin();ptr!=items.cend();ptr=items.upper_bound(*ptr))
	{
		sum += print_total(os, **ptr, items.count(*ptr));
	}
	return sum;
}



//虚基类
class Query_bass 
{
	friend class Query;
protected:
	using line_no = TextQuery::line_no;
	virtual ~Query_bass() = default;
private:
	//返回与当前Query匹配的QueryResult
	virtual QueryResult eval(const TextQuery&) const = 0;
	//rep是表示查询的一个string
	virtual string rep() const = 0;
};

//接口类
class Query
{
	//运算符需要访问接受shared_ptr的构造函数，而该函数是私有的
	friend Query operator~(const Query &);
	friend Query operator|(const Query &, const Query&);
	friend Query operator&(const Query &, const Query&);
public:
	Query(const string&);
	QueryResult eval(const TextQuery& t) const
	{
		return q->eval(t);
	}
	string rep()const { return q->rep(); }

private:
	Query(shared_ptr<Query_bass> query):q(query){}
	shared_ptr<Query_bass> q;
};


class NotQuery:public Query_bass
{
	friend Query operator~(const Query &);
	NotQuery(const Query &q):query(q){}

	string rep()const { return "(" + query.rep() + ")"; }
	QueryResult eval(const TextQuery&) const;
	NotQuery(const TextQuery&);

	Query query;
};
Query operator~(const Query &operand)
{
	return shared_ptr<Query_bass>(new NotQuery(operand));
}
QueryResult NotQuery::eval(const TextQuery & text) const
{
	//通过Query对象对eval进行虚调用
	auto result = query.eval(text);
	//开始时结果set为空
	auto ret_lines = make_shared<set<line_no>>();
	//我们必须对运算对象出现的所有行进行迭代
	auto sz = result.begin(), end = result.end();


}

class BindaryQuery :public Query_bass
{
protected:
	BindaryQuery(const Query&l, const Query&r, string s):
		lhs(l),rhs(r),opSym(s){}
	string rep()const { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; }

	Query lhs, rhs;         //左侧与右侧运算对象
	string opSym;           //运算符名字
};

class AndQuery :public BindaryQuery
{
	friend Query operator&(const Query&, const Query&);
	AndQuery(const Query &left,const Query &right):
		BindaryQuery(left,right,"&"){}
	QueryResult eval(const TextQuery &)const;

};
Query operator&(const Query &lhs, const Query &rhs)
{
	return shared_ptr<Query_bass>(new AndQuery(lhs, rhs));
}
QueryResult AndQuery::eval(const TextQuery &) const
{
	return ;
}

class OrQuery:public BindaryQuery
{
	friend Query operator|(const Query&,const Query&);
	OrQuery(const Query&lhs,const Query&rhs):
		BindaryQuery(lhs,rhs,"|"){}
	QueryResult eval(const TextQuery&) const;
};
Query operator|(const Query &lhs, const Query &rhs)
{
	return shared_ptr<Query_bass>(new OrQuery(lhs, rhs));
}
QueryResult OrQuery::eval(const TextQuery & text) const
{   //返回每个运算对象的QueryResult
	auto right=rhs.eval(text),left=lhs.eval(text);
	//将左侧行号加入 ret_lines
	auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());

	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());
}

int main()
{
	Quote b1("龙族I", 128);
	bulk_quote b2("龙族II", 128.0, 10, 0.2);
	number_quote b3("龙族III", 128.0, 5, 0.2);

	vector<Quote>vec;
	vector<shared_ptr<Quote>> vec_2;

	vec_2.push_back(make_shared<Quote>("龙族I", 128));
	vec_2.push_back(make_shared<bulk_quote>("龙族II", 128.0, 10, 0.2));
	vec_2.push_back(make_shared<number_quote>("龙族III", 128.0, 5, 0.2));

	//for (auto& i : vec_2)
	//{
	//	i->debug();
	//}

	Basket bas,bas_2;
	bas.add_item(b1);
	bas.add_item(b2);
	bas.add_item(b3);
	auto sum= bas.total_receipt(cout);
	cout << "总价" << sum << endl;

	bas_2.add_item(Quote("龙族I", 128));
	bas_2.add_item(bulk_quote("龙族II", 128.0, 10, 0.2));
	bas_2.add_item(number_quote("龙族III", 128.0, 5, 0.2));
	auto sum_2=bas_2.total_receipt(cout);
	cout << "总价" << sum_2 << endl;

	bulk_quote b_2(b2),b_2_1;
	b_2_1 = b_2;
	bulk_quote b_2_2(std::move(bulk_quote("龙族II", 128.0, 10, 0.2))),b_2_3;
	b_2_3 = std::move(bulk_quote("龙族II", 128.0, 10, 0.2));

	number_quote b_3(b3), b_3_1;
	b_3_1 = b_3;

	//使用 using Disc_quote::Disc_quote;//继承构造函数    
	//number_quote b_3_2(std::move(bulk_quote("龙族II", 128.0, 10, 0.2))), b_3_3;
	//b_3_3 = std::move(bulk_quote("龙族II", 128.0, 10, 0.2));

	vec.push_back(b2);
	vec.push_back(b2);

	print_total(cout, b1, 10);
	print_total(cout, b2, 10);
	print_total(cout, b3, 10);

	b1.debug();
	b2.debug();
	b3.debug();



	/*新版本以下两句可以略去*/
	system("pause");
	return 0;
}

