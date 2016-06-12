#include "_required.h"
#include "MixerControl.h"
#include <BaseLib.h> // myexception

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
MixerControl::MixerControl(HMIXER _hMixer,MIXERCONTROL* _mxc)
{
	hMixer=_hMixer;
	mxc=*_mxc;
}
MixerControl::~MixerControl()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       ��{���̎擾                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

DWORD MixerControl::getControlType() const
{
	return mxc.dwControlType;
}

DWORD MixerControl::getControlID() const
{
	return mxc.dwControlID;
}

const wchar_t* MixerControl::getName() const
{
	return mxc.szName;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �����⏕                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void MixerControl::_getValue(void* paDetails,int cbDetails) const
{
	MMRESULT ret;
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct=sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID=this->getControlID();
	mxcd.cChannels=1; //??
	mxcd.cMultipleItems=0; //??
	mxcd.cbDetails=cbDetails;
	mxcd.paDetails=paDetails;
	ret=mixerGetControlDetails((HMIXEROBJ)hMixer,&mxcd,MIXER_GETCONTROLDETAILSF_VALUE);
	if(ret!=MMSYSERR_NOERROR){
		throw myexception(L"mixerGetControlDetails failed.");
	}
}
void MixerControl::_setValue(const void* paDetails,int cbDetails)
{
	MMRESULT ret;
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct=sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID=this->getControlID();
	mxcd.cChannels=1; //??
	mxcd.cMultipleItems=0; //??
	mxcd.cbDetails=cbDetails;
	mxcd.paDetails=(void*)paDetails; //###���v�Ȃ͂�
	ret=mixerSetControlDetails((HMIXEROBJ)hMixer,&mxcd,MIXER_SETCONTROLDETAILSF_VALUE);
	if(ret!=MMSYSERR_NOERROR){
		throw myexception(L"mixerSetControlDetails failed.");
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          DWORD�l                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
DWORD MixerControl::getUnsigned() const
{
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	try{
		_getValue(&mxcd_u,sizeof(MIXERCONTROLDETAILS_UNSIGNED));
		return mxcd_u.dwValue;
	}catch(const wexception&){
	}
	return 0; //�G���[�l
}

void MixerControl::setUnsigned(DWORD value)
{	
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	mxcd_u.dwValue=value;
	_setValue(&mxcd_u,sizeof(MIXERCONTROLDETAILS_UNSIGNED));
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �u�[���l                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool MixerControl::getBool() const
{
	MIXERCONTROLDETAILS_BOOLEAN mxcd_b;
	try{
		_getValue(&mxcd_b,sizeof(MIXERCONTROLDETAILS_BOOLEAN));
		return mxcd_b.fValue!=0;
	}catch(const wexception&){
	}
	return false; //�G���[�l
}

void MixerControl::setBool(bool value)
{
	MIXERCONTROLDETAILS_BOOLEAN mxcd_b;
	mxcd_b.fValue=value?1:0;
    _setValue(&mxcd_b,sizeof(MIXERCONTROLDETAILS_BOOLEAN));
}

