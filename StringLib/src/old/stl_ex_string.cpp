#include "include_sys.h"
#include "stl_ex_string.h"

void ex_string::removeQuote()
{
	if(length()>=2 && *begin()==L'"' && *last()==L'"'){
		erase(begin(),begin()+1);
		erase(last(),last()+1);
	}
}

void ex_string::removeLf()
{
	if(*last()==L'\n'){
		erase(last(),last()+1);
	}
}

void ex_string::removeCrlf()
{
	if(*last()==L'\n'){
		erase(last(),last()+1);
		if(*last()==L'\r'){
			erase(last(),last()+1);
		}
	}
}

int ex_string::toInt()
{
	return _wtoi(c_str());
}

ex_string ex_string::replace(const wstring& from,const wstring& to) const
{
	int n,m=0;
	wstring ret=L"";
	while(1){
		n=find(from,m);
		if(n!=wstring::npos){
			ret+=substr(0,n);
			ret+=to;
			m=n+from.length();
		}else{
			ret+=substr(m);
			break;
		}
	}
	return ret;
}


