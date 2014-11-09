#include "_required.h"
#include "Mixer.h"
#include "std/myexception.h"
#include "util/AbTable.h"
#include "../Cast.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Mixer::Mixer(int mixer_id,HWND hwndCallback)
{
	last_error=MMSYSERR_NOERROR;
	hMixer=NULL;

	for(int i=0;i<16;i++){
		dst_lines[i]=NULL;
		src_lines[i]=NULL;
	}

	Open(mixer_id,hwndCallback);
}

Mixer::~Mixer()
{
	Close();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    オープン・クローズ                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Mixer::Open(int mixer_id,HWND hwndCallback)
{
	MMRESULT ret;

	//既に開いていたら、閉じる
	Close();

	//デバイス情報の取得
	ret=mixerGetDevCaps(mixer_id,&mcaps,sizeof(MIXERCAPS));
	if(ret!=MMSYSERR_NOERROR){
		throw myexception(L"mixerGetDevCaps failed.");
	}

	//オープン
	DWORD callback=PointerToDword(hwndCallback);
	DWORD flag=(callback!=0)?CALLBACK_WINDOW:0;
	ret=mixerOpen(&hMixer,mixer_id,callback,NULL,flag);
	if(ret!=MMSYSERR_NOERROR){
		throw myexception(L"mixerOpen failed.");
	}
}

void Mixer::Close()
{
	//ラインキャッシュ削除
	for(int i=0;i<16;i++){
		m_delete(dst_lines[i]);
	}
	for(int i=0;i<16;i++){
		m_delete(src_lines[i]);
	}

	//ミキサークローズ
	if(hMixer!=NULL){
		mixerClose(hMixer);
		hMixer=NULL;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      基本情報の取得                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


const wchar_t* Mixer::getName()
{
	if(hMixer!=NULL){
		return mcaps.szPname;
	}else{
		return NULL;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         実装補助                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

MixerLine* Mixer::_createLine(int dest_index,int src_index,int type)
{
	MMRESULT ret;
	
	MIXERLINE mxl;
	mxl.cbStruct=sizeof(MIXERLINE);
	mxl.dwDestination=dest_index;
	mxl.dwSource=src_index;

	if(type==MIXERLINE_DESTINATION){
		ret=mixerGetLineInfo((HMIXEROBJ)hMixer,&mxl,MIXER_GETLINEINFOF_DESTINATION);
	}else if(type==MIXERLINE_SOURCE){
		ret=mixerGetLineInfo((HMIXEROBJ)hMixer,&mxl,MIXER_GETLINEINFOF_SOURCE);
	}else{
		ret=MMSYSERR_INVALPARAM;
	}

	if(ret!=MMSYSERR_NOERROR){
		last_error=ret;
		return NULL;
	}else{
		return new MixerLine(hMixer,&mxl);
	}
}

MixerLine* Mixer::_createLineByType(DWORD type)
{
	MMRESULT ret;
	
	MIXERLINE mxl;
	mxl.cbStruct=sizeof(MIXERLINE);
//	mxl.dwDestination=dest_index;
//	mxl.dwSource=src_index;
	mxl.dwComponentType=type;

	ret=mixerGetLineInfo((HMIXEROBJ)hMixer,&mxl,MIXER_GETLINEINFOF_COMPONENTTYPE);

	if(ret!=MMSYSERR_NOERROR){
		last_error=ret;
		return NULL;
	}else{
		return new MixerLine(hMixer,&mxl);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        出力ライン                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int Mixer::getDestinationCount()
{
	if(hMixer!=NULL){
		return mcaps.cDestinations;
	}else{
		return 0;
	}
}

MixerLine* Mixer::createDestinationLine(int dst_index)
{
	return _createLine(dst_index,0,MIXERLINE_DESTINATION);
}

MixerLine* Mixer::createDestinationLineByType(Type _type)
{
	static AbTable<Type,DWORD> table;
	if(table.size()==0){
		table.regist(SPEAKER,MIXERLINE_COMPONENTTYPE_DST_SPEAKERS);
	}

	DWORD type=table.a_to_b(_type,0);
	if(type==0){
		last_error=MMSYSERR_INVALPARAM;
		return NULL;
	}else{
		return _createLineByType(type);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        入力ライン                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

MixerLine* Mixer::createSourceLine(int dest_index,int src_index)
{
	return _createLine(dest_index,src_index,MIXERLINE_SOURCE);
}


MixerLine* Mixer::createSourceLineByType(Type _type)
{
	static AbTable<Type,DWORD> table;
	if(table.size()==0){
		table.regist(WAVEOUT, MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT);
		table.regist(LINEIN,  MIXERLINE_COMPONENTTYPE_SRC_LINE);
	}

	DWORD type=table.a_to_b(_type,0);
	if(type==0){
		last_error=MMSYSERR_INVALPARAM;
		return NULL;
	}else{
		return _createLineByType(type);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   ミキサーコントロール                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

MixerControl* Mixer::createControlByID(DWORD ctl_id)
{
	MMRESULT ret;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mxc;
	mxlc.cbStruct=sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID=0; //無視
	mxlc.dwControlID=ctl_id; //mxlc.dwControlTypeと共有体
	mxlc.cControls=1;
	mxlc.cbmxctrl=sizeof(MIXERCONTROL);
	mxlc.pamxctrl=&mxc;
	ret=mixerGetLineControls((HMIXEROBJ)hMixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYID);
	if(ret!=MMSYSERR_NOERROR){
		last_error=ret;
		return NULL;
	}else{
		return new MixerControl(hMixer,&mxc);
	}
}
