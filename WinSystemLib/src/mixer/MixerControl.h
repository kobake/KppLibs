#pragma once

#include <windows.h>
#include <mmsystem.h>

class MixerControl{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	MixerControl(HMIXER _hMixer,MIXERCONTROL* _mxc);
	virtual ~MixerControl();
	//��{���̎擾
	DWORD getControlType() const;
	DWORD getControlID() const;
	const wchar_t* getName() const;
	//DWORD�l
	DWORD getUnsigned() const;
	void setUnsigned(DWORD value);
	//�u�[���l
	bool getBool() const;
	void setBool(bool value);
protected:
	//�����⏕
	void _getValue(void* paDetails,int cbDetails) const;
	void _setValue(const void* paDetails,int cbDetails);
private:
	HMIXER hMixer;
	MIXERCONTROL mxc;
};


