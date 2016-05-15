#include "common/_required.h"
#include "CObserver.h"
#include <map>
using namespace std;

//�Ǘ��e�[�u��
typedef multimap<CSubject*, CObserver*> MapType;
static MapType g_table;
typedef MapType::value_type PairType;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         CSubject                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CSubject::~CSubject()
{
	g_table.erase(this);
}

void CSubject::AttachObserver(CObserver* pcObserver)
{
	//�o�^
	g_table.insert(PairType(this,pcObserver));
}

void CSubject::DetachObserver(CObserver* pcObserver)
{
	//����
	MapType::iterator p = g_table.find(this), q = g_table.end();
	while(p!=q && p->first==this){
		if(p->second==pcObserver){
			//��������
			g_table.erase(p);
			return;
		}
		p++;
	}
	//�������ɗ�����������s
}

//Subject�ύX�ʒm: Subject��Observers
void CSubject::NotifyToObservers(int nNotifyCode, int nNotifyParam)
{
	//�SObserver�ɒʒm
	MapType::iterator p = g_table.find(this), q = g_table.end();
	while(p!=q && p->first==this){
		p->second->OnSubjectUpdate(nNotifyCode,nNotifyParam,this);
		p++;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        CObserver                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CObserver::~CObserver()
{
	MapType::iterator p,q;
retry:
	p = g_table.begin(), q = g_table.end();
	while(p!=q){
		if(p->second==this){
			g_table.erase(p);
			goto retry;
		}
		p++;
	}
}

