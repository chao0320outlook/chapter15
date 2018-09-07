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
	shared_ptr<vector<string>>  str;                //�ı����� ���б���
	map<string, shared_ptr<set<line_no>>> map1;     //�����Լ������кŵ�set
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
	shared_ptr<set<line_no>> lines;    //���ֵ��к�
	shared_ptr<vector<string>>  str;   //�ı����� ���б���
};
