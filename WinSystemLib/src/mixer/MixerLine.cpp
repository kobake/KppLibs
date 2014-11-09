#include "_required.h"
#include "MixerLine.h"
#include "util/AbTable.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
MixerLine::MixerLine(HMIXER _hMixer,MIXERLINE* _mxl)
{
	last_error=MMSYSERR_NOERROR;
	hMixer=_hMixer;
	mxl=*_mxl;
	ret_MixerControl=NULL;
}
MixerLine::~MixerLine()
{
	if(ret_MixerControl!=NULL){
		free(ret_MixerControl);
		ret_MixerControl=NULL;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       基本情報の取得                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
const wchar_t* MixerLine::getName()
{
	return mxl.szName;
}
int MixerLine::getConnectionNum()
{
	return mxl.cConnections;
}
int MixerLine::getControlNum()
{
	return mxl.cControls;
}
DWORD MixerLine::getComponentType()
{
	return mxl.dwComponentType;
}
DWORD MixerLine::getLineID()
{
	return mxl.dwLineID;
}
MMRESULT MixerLine::getLastError()
{
	return last_error;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     コントロールの取得                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


MixerControl* MixerLine::createControl(ControlType _type)
{
	static AbTable<ControlType,DWORD> table;
	if(table.size()==0){
		table.regist(VOLUME,         MIXERCONTROL_CONTROLTYPE_VOLUME);
		table.regist(MUTE,           MIXERCONTROL_CONTROLTYPE_MUTE);
	}


	DWORD type=table.a_to_b(_type,0);
	if(type==0){
		last_error=MIXERR_INVALCONTROL;
		return NULL;
	}
	MMRESULT ret;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mxc;
	mxlc.cbStruct=sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID=this->getLineID();
	mxlc.dwControlType=type; //mxlc.dwControlIDと共有体
	mxlc.cControls=1;
	mxlc.cbmxctrl=sizeof(MIXERCONTROL);
	mxlc.pamxctrl=&mxc;
	ret=mixerGetLineControls((HMIXEROBJ)hMixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE);
	if(ret!=MMSYSERR_NOERROR){
		last_error=ret;
		return NULL;
	}else{
		return new MixerControl(hMixer,&mxc);
	}
}

MixerControl** MixerLine::getControls()
{
	MMRESULT ret;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL* mxc=(MIXERCONTROL*)calloc(getControlNum(),sizeof(MIXERCONTROL));
	mxlc.cbStruct=sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID=this->getLineID();
	mxlc.dwControlID=0; //or mxlc.dwControlType=0; (おそらく未使用)
	mxlc.cControls=this->getControlNum();
	mxlc.cbmxctrl=sizeof(MIXERCONTROL);
	mxlc.pamxctrl=mxc;
	ret=mixerGetLineControls((HMIXEROBJ)hMixer,&mxlc,MIXER_GETLINECONTROLSF_ALL);
	if(ret!=MMSYSERR_NOERROR){
		last_error=ret;
		free(ret_MixerControl);
		ret_MixerControl=NULL;
	}else{
		int i;
		ret_MixerControl=(MixerControl**)realloc(ret_MixerControl,sizeof(MixerControl*)*getControlNum());
		for(i=0;i<getControlNum();i++){
			ret_MixerControl[i]=new MixerControl(hMixer,&mxc[i]);
		}
	}
	free(mxc);
	return ret_MixerControl;
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      入力系統の取得                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

MixerLine* MixerLine::getSourceLine(int src_index)
{
	MMRESULT ret;
	
	MIXERLINE new_mxl;
	new_mxl.cbStruct=sizeof(MIXERLINE);
	new_mxl.dwDestination=mxl.dwDestination;
	new_mxl.dwSource=src_index;

	ret=mixerGetLineInfo((HMIXEROBJ)hMixer,&new_mxl,MIXER_GETLINEINFOF_SOURCE);

	if(ret!=MMSYSERR_NOERROR){
		last_error=ret;
		return NULL;
	}else{
		return new MixerLine(hMixer,&new_mxl);
	}
}
