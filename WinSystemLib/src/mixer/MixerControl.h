#pragma once

#include <windows.h>
#include <mmsystem.h>

class MixerControl{
public:
	//コンストラクタ・デストラクタ
	MixerControl(HMIXER _hMixer,MIXERCONTROL* _mxc);
	virtual ~MixerControl();
	//基本情報の取得
	DWORD getControlType() const;
	DWORD getControlID() const;
	const wchar_t* getName() const;
	//DWORD値
	DWORD getUnsigned() const;
	void setUnsigned(DWORD value);
	//ブール値
	bool getBool() const;
	void setBool(bool value);
protected:
	//実装補助
	void _getValue(void* paDetails,int cbDetails) const;
	void _setValue(const void* paDetails,int cbDetails);
private:
	HMIXER hMixer;
	MIXERCONTROL mxc;
};


