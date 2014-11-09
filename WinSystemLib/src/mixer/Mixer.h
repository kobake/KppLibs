#pragma once

#include "MixerLine.h"

class MixerInfo{
public:
	static int getDeviceNum()
	{
		return mixerGetNumDevs();
	}
};

/*
MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT
MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER
MIXERLINE_COMPONENTTYPE_SRC_ANALOG (������ײ݂�����ꍇ������)
MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC
MIXERLINE_COMPONENTTYPE_SRC_LINE
MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE
MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE
MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT (PC�X�s�[�J)
*/

#include <windef.h> //HWND

class Mixer{
public:
	enum Type{
		SPEAKER=1,
		WAVEOUT,
		LINEIN,
	};
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Mixer(int mixer_id,HWND hwndCallback=NULL);
	virtual ~Mixer();
	//�I�[�v���E�N���[�Y
	void Open(int mixer_id,HWND hwndCallback=NULL);
	void Close();
	//��{���̎擾
	const wchar_t* getName();
	//�o�̓��C��
	int getDestinationCount();
	MixerLine* createDestinationLine(int dest_index);
	MixerLine* createDestinationLineByType(Type _type);
	//���̓��C��
	MixerLine* createSourceLine(int dest_index,int src_index);
	MixerLine* createSourceLineByType(Type _type);
	//�~�L�T�[�R���g���[��
	MixerControl* createControlByID(DWORD ctl_id);
	//�����A�N�Z�X
	HMIXER _getHMIXER() const{ return hMixer; }
protected:
	//�����⏕
	MixerLine* _createLine(int dest_index,int src_index,int type);
	MixerLine* _createLineByType(DWORD type);
private:
	MMRESULT last_error;
	HMIXER hMixer;
	MIXERCAPS mcaps;
	//
	MixerLine* dst_lines[16];
	MixerLine* src_lines[16];
};

