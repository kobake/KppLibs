#pragma once


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  ex_vector  -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

template <class T> class ex_vector : public vector<T>{
public:
	int index(const T& value) const
	{
		vector<T>::const_iterator p=begin(),q=end();
		int i=0;
		while(p!=q){
			if(*p==value){
				return i;
			}
			p++;
			i++;
		}
		return -1;
	}
	using vector<T>::push_back;
	void push_back(const vector<T>& vec)
	{
		vector<T>::const_iterator p=vec.begin();
		vector<T>::const_iterator q=vec.end();
		while(p!=q){
			push_back(*p);
			p++;
		}
	}
	// read/write
	void write(FileStream *out) const
	{		
		out->writeInt(size());
		vector<T>::const_iterator p=begin();
		vector<T>::const_iterator q=end();
		while(p!=q){
			out->ex_write(*p);
			p++;
		}
	}
	void read(FileStream *in)
	{
		T t;
		clear();
		int i,n=in->readInt();
		for(i=0;i<n;i++){
			in->ex_read(t);
			push_back(t);
		}
	}
};




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  storable_vector  -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// -- -- -- -- -- -- 宣言 -- -- -- -- -- -- //
//コピーコンストラクタは必要なし（デフォルトのままでOK）
template <class T> class storable_vector : public vector<T>{
private:
	vector<T> v_st;
public:
	storable_vector();
	void store();
	void restore();
};

// -- -- -- -- -- -- 定義 -- -- -- -- -- -- //
template <class T> storable_vector<T>::storable_vector()
: v_st()
{
}

template <class T> void storable_vector<T>::store()
{
	v_st.assign(this->begin(),this->end());
}

template <class T> void storable_vector<T>::restore()
{
	this->assign(v_st.begin(),v_st.end());
}



