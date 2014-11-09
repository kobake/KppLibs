#pragma once

#define MIXERLINE_DESTINATION	0
#define MIXERLINE_SOURCE		1

#include "MixerControl.h"


class MixerLine{
public:
	enum ControlType{
		VOLUME=1,
		MUTE,
	};
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	MixerLine(HMIXER _hMixer,MIXERLINE* _mxl);
	virtual ~MixerLine();
	//��{���̎擾
	const wchar_t* getName();
	int getConnectionNum();
	int getControlNum();
	DWORD getLineID();
	DWORD getComponentType();
	MMRESULT getLastError();
	//�R���g���[���̎擾
	MixerControl** getControls();
	MixerControl* createControl(ControlType type);
	//���͌n���̎擾
	MixerLine* getSourceLine(int src_index);
private:
	MMRESULT last_error;
	HMIXER hMixer;
	MIXERLINE mxl;
	MixerControl** ret_MixerControl;
};

