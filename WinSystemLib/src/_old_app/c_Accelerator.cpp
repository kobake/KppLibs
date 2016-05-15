#include "include_sys.h"
#include "c_Accelerator.h"
#include "c_App.h"


BYTE Accelerator::_flag_to_virt(int flag)
{
	BYTE v=FVIRTKEY;	// | FNOINVERT;
	if(flag & ACCELF_CONTROL)	v|=FCONTROL;
	if(flag & ACCELF_SHIFT)		v|=FSHIFT;
	return v;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
Accelerator::Accelerator()
{
	table=NULL;
	ntable=0;
	hAccel=NULL;
	changed=false;
}
Accelerator::~Accelerator()
{
	if(hAccel!=NULL){
		DestroyAcceleratorTable(hAccel);
		hAccel=NULL;
	}
	free(table);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  実使用  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int Accelerator::translate(Window *wnd,MSG *msg)
{
	return translate(wnd->getHWND(),msg);
}
int Accelerator::translate(HWND hwnd,MSG *msg)
{
	_updateAccelerator();
	if(hAccel!=NULL){
		return TranslateAccelerator(hwnd,hAccel,msg); //変換が行われた場合 0 以外の値、行われなかった場合 0 。
	}else{
		return 0; //変換を行わないので 0 を返す
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  ハンドル操作  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Accelerator::_destroyAccelerator()
{
	if(hAccel!=NULL){
		DestroyAcceleratorTable(hAccel);
		hAccel=NULL;
		changed=false;
	}
}
void Accelerator::_updateAccelerator()
{
	if(changed){
		_destroyAccelerator();
		if(ntable>0){
			hAccel=CreateAcceleratorTable(table,ntable);
		}else{
			hAccel=NULL;
		}
		changed=false;
	}
}

void Accelerator::loadAccelerator(App *app,wchar* resourcename)
{
	//アクセラレータ破棄
	clearEntries();
	_updateAccelerator();
	//ロード
	hAccel=LoadAccelerators(app->getInstance(),resourcename);
	if(hAccel!=NULL){
		//エントリーロード
		ntable=CopyAcceleratorTable(hAccel,NULL,0);
		table=(ACCEL*)realloc(table,sizeof(ACCEL)*ntable);
		CopyAcceleratorTable(hAccel,table,ntable);
	}
	changed=false;
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  エントリー操作  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Accelerator::setEntries(ACCELDEF *acc)
{
	clearEntries();
	for(ACCELDEF *p=acc;p->cmd!=-1;p++)addEntry(p->cmd,p->key,p->flag);
}
void Accelerator::clearEntries()
{
	free(table);
	ntable=0;
	changed=true;
}

//
// 使い方: addEntry(IDM_FILE_OPEN,L'O',ACCELF_CONTROL | ACCELF_SHIFT);
//
void Accelerator::addEntry(int cmd,int key,int flag)
{
	//同じ要素がないかチェック
	if(findEntry(cmd,key,flag)==-1){
		ntable++;
		table=(ACCEL*)realloc(table,sizeof(ACCEL)*ntable);
		table[ntable-1].cmd=cmd;
		table[ntable-1].key=key;
		table[ntable-1].fVirt=_flag_to_virt(flag);
		changed=true;
	}
}
int Accelerator::findEntry(int cmd,int key,int flag)
{
	ACCEL tmp;
	tmp.cmd=cmd;
	tmp.key=key;
	tmp.fVirt=_flag_to_virt(flag);
	//
	for(int i=0;i<ntable;i++){
		if(memcmp(&table[i],&tmp,sizeof(ACCEL))==0){
			return i;
		}
	}
	return -1;
}
void Accelerator::deleteEntry(int index)
{
	if(index>=0 && index<ntable){
		memmove(&table[index],&table[index+1],sizeof(ACCEL)*(ntable-index-1));
		ntable--;
		if(ntable!=0){
			table=(ACCEL*)realloc(table,sizeof(ACCEL)*ntable);
		}else{
			free(table);
			table=NULL;
		}
		changed=true;
	}
}
void Accelerator::deleteEntry(int cmd,int key,int flag)
{
	deleteEntry(findEntry(cmd,key,flag));
}


