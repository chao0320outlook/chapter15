#include "QueryResult.h"
#include <sstream>

TextQuery::TextQuery(ifstream & in)
{
	string  line, word;
	while (getline(in, line))
	{
		str->push_back(line);          //逐行保存
		auto n = str->size() - 1;
		istringstream word_d(line);

		while (word_d >> word)
		{
			auto &lines = map1[word];
			if (!lines)
			{
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}

}

QueryResult TextQuery::check(string & s) const
{
	static shared_ptr<set<line_no>> nodata(new set<line_no>);

	auto loc = map1.find(s);
	if (loc == map1.end())
		return QueryResult(s, nodata, str);
	else
		return QueryResult(s, loc->second, str);

}
ostream & print(ostream &os, const QueryResult &qr)
{
	os << qr.sought << "出现了" << qr.lines->size() << "次" << endl;
	for (auto num : *qr.lines)
		os << "\t(line " << num + 1 << ")" <<
		*(qr.str->begin() + num) << endl;
	return os;
}
