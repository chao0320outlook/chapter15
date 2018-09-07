#pragma once
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <memory>

using namespace std;

class QueryResult;

class TextQuery
{
public:
	using line_no = vector<string>::size_type;
	TextQuery(ifstream &in);
	QueryResult check(string& s) const;
private:
	shared_ptr<vector<string>>  str;                //文本内容 逐行保存
	map<string, shared_ptr<set<line_no>>> map1;     //单词以及出现行号的set
};
class  QueryResult
{
	friend ostream& print(ostream &os, const QueryResult&qr);
public:
	QueryResult(string s,
		shared_ptr<set<line_no>> p,
		shared_ptr<vector<string>> f) :
		sought(s), lines(p), str(f) {}

private:
	string sought;
	shared_ptr<set<line_no>> lines;    //出现的行号
	shared_ptr<vector<string>>  str;   //文本内容 逐行保存
};
