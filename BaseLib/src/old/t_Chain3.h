template <class DATA> class BChain;
template <class DATA> class BChainList;

#ifndef DEF_CHAIN3
#define DEF_CHAIN3

// ** ** ** ** BChain ** ** ** ** //

template <class DATA> class BChain{
protected:
	DATA data;
	BChain<DATA> *last;
	BChain<DATA> *next;
public:
	BChain(DATA _data,BChain<DATA> *_last,BChain<DATA> *_next);
	virtual ~BChain();
	BChain<DATA> *getChainLast();
	BChain<DATA> *getChainNext();
	BChain<DATA> *getChainBegin();
	BChain<DATA> *getChainEnd();
	BChain<DATA> *getChainItem(int index);
	void setData(DATA _data);
	DATA getData();
};

template <class DATA> void BChain<DATA>::setData(DATA _data)
{
	data=_data;
}
template <class DATA> DATA BChain<DATA>::getData()
{
	return data;
}

template <class DATA> BChain<DATA>::BChain(DATA _data,BChain<DATA> *_last,BChain<DATA> *_next)
{
	data=_data;
	last=_last;	if(last!=NULL){ ((BChain*)last)->next=(BChain<DATA>*)this; }
	next=_next; if(next!=NULL){ ((BChain*)next)->last=(BChain<DATA>*)this; }
}
template <class DATA> BChain<DATA>::~BChain()
{
	if(last!=NULL){ ((BChain*)last)->next=next; }
	if(next!=NULL){ ((BChain*)next)->last=last; }
}
template <class DATA> BChain<DATA> *BChain<DATA>::getChainLast()
{
	return (BChain<DATA>*)last;
}
template <class DATA> BChain<DATA> *BChain<DATA>::getChainNext()
{
	return (BChain<DATA>*)next;
}
template <class DATA> BChain<DATA> *BChain<DATA>::getChainBegin()
{
	BChain *p=this;
	while(p->last!=NULL){
		if(p->last==this)return NULL; //輪状
		p=p->last;
	}
	return (BChain<DATA>*)p;
}
template <class DATA> BChain<DATA> *BChain<DATA>::getChainEnd()
{
	BChain *p=this;
	while(p->next!=NULL){
		if(p->next==this)return NULL; //輪状
		p=p->next;
	}
	return (BChain<DATA>*)p;
}
template <class DATA> BChain<DATA> *BChain<DATA>::getChainItem(int index)
{
	BChain *p=this;
	int i;
	if(index>=0){
		for(i=0;i<index && p!=NULL;i++){
			p=p->next;
		}
		return (BChain<DATA>*)p;
	}else{
		for(i=0;i>index && p!=NULL;i--){
			p=p->last;
		}
		return (BChain<DATA>*)p;
	}
}

// ** ** ** ** BChainList ** ** ** ** //

template <class DATA> class BChainList{
protected:
	int count;
	BChain<DATA> *begin;
public:
	BChainList();
	virtual ~BChainList();
	bool put(int index,DATA _data);
	bool set(int index,DATA _data);
	DATA get(int index);
	bool remove(int index);
	void clear();
protected:
	BChain<DATA> *each_object;
public:
	bool eachFirst(DATA *_data);
	bool eachNext(DATA *_data);
	bool eachClose();
};

template <class DATA> bool BChainList<DATA>::eachFirst(DATA *_data)
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
template <class DATA> bool BChainList<DATA>::eachNext(DATA *_data)
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
template <class DATA> bool BChainList<DATA>::eachClose()
{
	each_object=NULL;
	return true;
}

template <class DATA> BChainList<DATA>::BChainList()
{
	begin=NULL;
	count=0;
	each_object=NULL;
}
template <class DATA> BChainList<DATA>::~BChainList()
{
	clear();
}
template <class DATA> void BChainList<DATA>::clear()
{
	BChain<DATA> *p=begin,*del;
	while(p!=NULL){
		del=p;
		p=p->getChainNext();
		delete del;
	}
	begin=NULL;
	count=0;
}
template <class DATA> bool BChainList<DATA>::put(int index,DATA _data)
{
	if(index==-1)index=count; // -1 のときは、最後尾に追加する
	BChain<DATA> *ret;
	if(count==0 && index==0){
		ret=new BChain<DATA>(_data,NULL,NULL); count++;
		begin=ret;
	}else if(index>=0 && index<=count){
		ret=new BChain<DATA>(_data,begin->getChainItem(index-1),begin->getChainItem(index)); count++;
		begin=ret->getChainBegin();
	}else{
		ret=NULL; //不正なインデックスです
	}
	return (ret!=NULL);
}

template <class DATA> DATA BChainList<DATA>::get(int index)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を返す
	BChain<DATA> *ret;
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

template <class DATA> bool BChainList<DATA>::set(int index,DATA _data)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を返す
	BChain<DATA> *ret;
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

template <class DATA> bool BChainList<DATA>::remove(int index)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を削除する
	if(count==0){
		return false; //削除しない
	}else if(index>=0 && index<count){
		BChain<DATA> *del=begin->getChainItem(index);
		if(begin==del)begin=begin->getChainNext();
		delete del; count--;
		return true;
	}else{
		return NULL; //不正なインデックスです
	}
}

#endif
