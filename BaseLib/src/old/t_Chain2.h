template <class DATA> class Chain;
template <class DATA> class ChainList;

#ifndef DEF_CHAIN2
#define DEF_CHAIN2

// ** ** ** ** Chain ** ** ** ** //

template <class DATA> class Chain{
protected:
	DATA data;
	Chain<DATA> *last;
	Chain<DATA> *next;
public:
	Chain(DATA _data,Chain<DATA> *_last,Chain<DATA> *_next);
	virtual ~Chain();
	Chain<DATA> *getChainLast();
	Chain<DATA> *getChainNext();
	Chain<DATA> *getChainBegin();
	Chain<DATA> *getChainEnd();
	Chain<DATA> *getChainItem(int index);
	void setData(DATA _data);
	DATA getData();
};

template <class DATA> void Chain<DATA>::setData(DATA _data)
{
	data=_data;
}
template <class DATA> DATA Chain<DATA>::getData()
{
	return data;
}

template <class DATA> Chain<DATA>::Chain(DATA _data,Chain<DATA> *_last,Chain<DATA> *_next)
{
	data=_data;
	last=_last;	if(last!=NULL){ ((Chain*)last)->next=(Chain<DATA>*)this; }
	next=_next; if(next!=NULL){ ((Chain*)next)->last=(Chain<DATA>*)this; }
}
template <class DATA> Chain<DATA>::~Chain()
{
	if(last!=NULL){ ((Chain*)last)->next=next; }
	if(next!=NULL){ ((Chain*)next)->last=last; }
}
template <class DATA> Chain<DATA> *Chain<DATA>::getChainLast()
{
	return (Chain<DATA>*)last;
}
template <class DATA> Chain<DATA> *Chain<DATA>::getChainNext()
{
	return (Chain<DATA>*)next;
}
template <class DATA> Chain<DATA> *Chain<DATA>::getChainBegin()
{
	Chain *p=this;
	while(p->last!=NULL){
		if(p->last==this)return NULL; //輪状
		p=p->last;
	}
	return (Chain<DATA>*)p;
}
template <class DATA> Chain<DATA> *Chain<DATA>::getChainEnd()
{
	Chain *p=this;
	while(p->next!=NULL){
		if(p->next==this)return NULL; //輪状
		p=p->next;
	}
	return (Chain<DATA>*)p;
}
template <class DATA> Chain<DATA> *Chain<DATA>::getChainItem(int index)
{
	Chain *p=this;
	int i;
	if(index>=0){
		for(i=0;i<index && p!=NULL;i++){
			p=p->next;
		}
		return (Chain<DATA>*)p;
	}else{
		for(i=0;i>index && p!=NULL;i--){
			p=p->last;
		}
		return (Chain<DATA>*)p;
	}
}

// ** ** ** ** ChainList ** ** ** ** //

template <class DATA> class ChainList{
protected:
	int count;
	Chain<DATA> *begin;
public:
	ChainList();
	virtual ~ChainList();
	bool put(int index,DATA _data);
	bool set(int index,DATA _data);
	DATA get(int index);
	bool remove(int index);
	void clear();
protected:
	Chain<DATA> *each_object;
public:
	bool eachFirst(DATA *_data);
	bool eachNext(DATA *_data);
	bool eachClose();
};

template <class DATA> bool ChainList<DATA>::eachFirst(DATA *_data)
{
	if(each_object!=NULL)return false;
	each_object=begin;
	if(each_object!=NULL){
		*_data=each_object->getData();
		return true;
	}else{
		return false;
	}
}
template <class DATA> bool ChainList<DATA>::eachNext(DATA *_data)
{
	if(each_object==NULL)return false;
	each_object=each_object->getChainNext();
	if(each_object!=NULL){
		*_data=each_object->getData();
		return true;
	}else{
		return false;
	}	
}
template <class DATA> bool ChainList<DATA>::eachClose()
{
	each_object=NULL;
	return true;
}

template <class DATA> ChainList<DATA>::ChainList()
{
	begin=NULL;
	count=0;
	each_object=NULL;
}
template <class DATA> ChainList<DATA>::~ChainList()
{
	clear();
}
template <class DATA> void ChainList<DATA>::clear()
{
	Chain<DATA> *p=begin,*del;
	while(p!=NULL){
		del=p;
		p=p->getChainNext();
		delete del;
	}
	begin=NULL;
	count=0;
}
template <class DATA> bool ChainList<DATA>::put(int index,DATA _data)
{
	if(index==-1)index=count; // -1 のときは、最後尾に追加する
	Chain<DATA> *ret;
	if(count==0 && index==0){
		ret=new Chain<DATA>(_data,NULL,NULL); count++;
		begin=ret;
	}else if(index>=0 && index<=count){
		ret=new Chain<DATA>(_data,begin->getChainItem(index-1),begin->getChainItem(index)); count++;
		begin=ret->getChainBegin();
	}else{
		ret=NULL; //不正なインデックスです
	}
	return (ret!=NULL);
}

template <class DATA> DATA ChainList<DATA>::get(int index)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を返す
	Chain<DATA> *ret;
	if(count==0){
		ret=NULL; //要素が存在しません
	}else if(index>=0 && index<count){
		ret=begin->getChainItem(index);
	}else{
		ret=NULL; //不正なインデックスです
	}
	if(ret!=NULL){
		return ret->getData();
	}else{
		return 0;
	}
}

template <class DATA> bool ChainList<DATA>::set(int index,DATA _data)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を返す
	Chain<DATA> *ret;
	if(count==0){
		ret=NULL; //要素が存在しません
	}else if(index>=0 && index<count){
		ret=begin->getChainItem(index);
	}else{
		ret=NULL; //不正なインデックスです
	}
	if(ret!=NULL){
		ret->setData(_data);
		return true;
	}else{
		return false;
	}
}

template <class DATA> bool ChainList<DATA>::remove(int index)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を削除する
	if(count==0){
		return false; //削除しない
	}else if(index>=0 && index<count){
		Chain<DATA> *del=begin->getChainItem(index);
		if(begin==del)begin=begin->getChainNext();
		delete del; count--;
		return true;
	}else{
		return NULL; //不正なインデックスです
	}
}

#endif
