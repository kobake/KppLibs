template <class CLASS> class ChainElement;
template <class DATA,class ELEMENT> class ChainElementList;


#ifndef DEF_CHAIN
#define DEF_CHAIN


// ** ** ** ** ChainElement ** ** ** ** //

template <class CLASS> class ChainElement{
protected:
	CLASS *last;
	CLASS *next;
public:
	ChainElement(CLASS *_last,CLASS *_next);
	virtual ~ChainElement();
	CLASS *getChainLast();
	CLASS *getChainNext();
	CLASS *getChainBegin();
	CLASS *getChainEnd();
	CLASS *getChainItem(int index);
	//###※VCのバグによりfriendできないので仮処置としてpublic関数を用意
	void _set_last(CLASS *_last){ last=_last; }
	void _set_next(CLASS *_next){ next=_next; }
	CLASS *_get_last(){ return last; }
	CLASS *_get_next(){ return next; }
};

template <class CLASS> ChainElement<CLASS>::ChainElement(CLASS *_last,CLASS *_next)
{
	last=_last;	if(last!=NULL){ ((ChainElement*)last)->next=(CLASS*)this; }
	next=_next; if(next!=NULL){ ((ChainElement*)next)->last=(CLASS*)this; }
}
template <class CLASS> ChainElement<CLASS>::~ChainElement()
{
	if(last!=NULL){ ((ChainElement*)last)->next=next; }
	if(next!=NULL){ ((ChainElement*)next)->last=last; }
}
template <class CLASS> CLASS *ChainElement<CLASS>::getChainLast()
{
	return (CLASS*)last;
}
template <class CLASS> CLASS *ChainElement<CLASS>::getChainNext()
{
	return (CLASS*)next;
}
template <class CLASS> CLASS *ChainElement<CLASS>::getChainBegin()
{
	ChainElement *p=this;
	while(p->last!=NULL){
		if(p->last==this)return NULL; //輪状
		p=p->last;
	}
	return (CLASS*)p;
}
template <class CLASS> CLASS *ChainElement<CLASS>::getChainEnd()
{
	ChainElement *p=this;
	while(p->next!=NULL){
		if(p->next==this)return NULL; //輪状
		p=p->next;
	}
	return (CLASS*)p;
}
template <class CLASS> CLASS *ChainElement<CLASS>::getChainItem(int index)
{
	ChainElement *p=this;
	int i;
	if(index>=0){
		for(i=0;i<index && p!=NULL;i++){
			p=p->next;
		}
		return (CLASS*)p;
	}else{
		for(i=0;i>index && p!=NULL;i--){
			p=p->last;
		}
		return (CLASS*)p;
	}
}


// ** ** ** ** ChainElementList ** ** ** ** //

// ※ 注意：ELEMENT は ChainElement の派生でなければならない

#include "type.h"

template <class ELEMENT, class DATA> class ChainElementList{
protected:
	int count;
	ELEMENT *begin;
public:
	ChainElementList();
	virtual ~ChainElementList();
	ELEMENT *put(DATA *_data,int index);
	ELEMENT *get(int index);
	bool remove(int index);
	void clear();
	int size(){ return count; }
	void sort(COMPPROC comp);
};

template <class ELEMENT, class DATA> void ChainElementList<ELEMENT,DATA>::sort(COMPPROC comp)
{
	ELEMENT *p,*q;
	bool changed;
	do{
		changed=false;
		p=begin;
		while(p!=NULL){
			q=p->_get_next(); //###p->next;
			if(q==NULL)break;
			if(comp(p,q)>0){
				p->_set_next(q->_get_next()); //###p->next=q->next;
				q->_set_last(p->_get_last()); //###q->last=p->last;
				p->_set_last(q); //###p->last=
				q->_set_next(p); //###q->next=
				if(q->_get_last()!=NULL)q->_get_last()->_set_next(q); //###q->last!= ###q->last->next=
				if(p->_get_next()!=NULL)p->_get_next()->_set_last(p); //###p->next!= ###p->next->last=
				if(begin==p)begin=q;
				changed=true;
			}else{
				p=q;
			}
		}
	}while(changed);
}

template <class ELEMENT, class DATA> ChainElementList<ELEMENT,DATA>::ChainElementList()
{
	begin=NULL;
	count=0;
}
template <class ELEMENT, class DATA> ChainElementList<ELEMENT,DATA>::~ChainElementList()
{
	clear();
}
template <class ELEMENT, class DATA> void ChainElementList<ELEMENT,DATA>::clear()
{
	ELEMENT *p=begin,*del;
	while(p!=NULL){
		del=p;
		p=p->getChainNext();
		delete del;
	}
	begin=NULL;
	count=0;
}
template <class ELEMENT, class DATA> ELEMENT *ChainElementList<ELEMENT,DATA>::put(DATA *_data,int index)
{
	if(index==-1)index=count; // -1 のときは、最後尾に追加する
	ELEMENT *ret;
	if(count==0 && index==0){
		ret=new ELEMENT(_data,NULL,NULL); count++;
		begin=ret;
	}else if(index>=0 && index<=count){
		ret=new ELEMENT(_data,begin->getChainItem(index-1),begin->getChainItem(index)); count++;
		begin=ret->getChainBegin();
	}else{
		ret=NULL; //不正なインデックスです
	}
	return ret;
}
template <class ELEMENT, class DATA> ELEMENT *ChainElementList<ELEMENT,DATA>::get(int index)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を返す
	if(count==0){
		return NULL; //要素が存在しません
	}else if(index>=0 && index<count){
		return begin->getChainItem(index);
	}else{
		return NULL; //不正なインデックスです
	}
}
template <class ELEMENT, class DATA> bool ChainElementList<ELEMENT,DATA>::remove(int index)
{
	if(index==-1)index=count-1; // -1 のときは、最後尾の要素を削除する
	if(count==0){
		return false; //削除しない
	}else if(index>=0 && index<count){
		ELEMENT *del=begin->getChainItem(index);
		if(begin==del)begin=begin->getChainNext();
		delete del; count--;
		return true;
	}else{
		return NULL; //不正なインデックスです
	}
}


#endif

#include "t_Chain2.h"
