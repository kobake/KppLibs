#pragma once

#include <vector>

template <class A,class B>
class AbTable{
private:
	struct Element{
		A a;
		B b;
	};
	typedef std::vector<Element> table_type;
public:
	//“o˜^
	void regist(const A& a,const B& b)
	{
		Element e={a,b};
		table.push_back(e);
	}
	//ó‘Ô
	size_t size() const
	{
		return table.size();
	}
	//ŒŸõ
	B a_to_b(const A& a,const B& def) const
	{
		table_type::const_iterator p=table.begin(),q=table.end();
		while(p!=q){
			if(a==p->a)return p->b;
			p++;
		}
		return def;
	}
	A b_to_a(const B& b,const A& def) const
	{
		table_type::const_iterator p=table.begin(),q=table.end();
		while(p!=q){
			if(b==p->b)return p->a;
			p++;
		}
		return def;
	}
private:
	table_type table;
};



