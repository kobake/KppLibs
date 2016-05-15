#pragma once

#define SOUND_TYPE_NONE		0
#define SOUND_TYPE_MIDI		1
#define SOUND_TYPE_WAVE		2

class MciSound{
public:
	MciSound(const wchar_t *_alias=NULL);
	virtual ~MciSound();
	bool loadFile(const wchar_t *fpath,int _type);
	void closeFile();
	void play(bool loop=false);
	void stop();
private:
	wchar_t alias[64];
	int type;
};



