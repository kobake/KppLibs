#if 0

#include <BaseLib.h>
#include "MciSound.h"
//#include "c_StringList.h"
//#include "f_window.h"
//#include <WinSystemLib.h>
#include "util/mywstring_funcs.h"
#include <windows.h>
using namespace util;

LRESULT CALLBACK WndProcSound(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
void SoundEnd();


static MciSound *soundLoop=NULL;

LRESULT CALLBACK WndProcSound(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
	case MM_MCINOTIFY:
		if(wParam==MCI_NOTIFY_SUCCESSFUL){
			if(soundLoop!=NULL){
				soundLoop->play(true);
			}
		}
		break;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0L;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 総合管理 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

static HWND hwndSound=NULL;
static StringList aliaslist;
static int useindex[256];
#define ALIAS_COMMON L"soundX"

static void SoundInit()
{
	if(hwndSound==NULL){
		hwndSound=CreateAWindow(app->getInstance(),app->getPrevInstance(),L"winSound",WndProcSound,(HBRUSH)(COLOR_BTNFACE+1),NULL,NULL,0,WS_POPUP,L"",-2,-2,0,0,NULL,NULL);
		app->reserveOnEnd(SoundEnd);
		memset(useindex,0,sizeof(int)*256);
		aliaslist.put(-1,ALIAS_COMMON);
	}
}
static void SoundEnd()
{
	DestroyWindow(hwndSound);
}
static const wchar_t *aliasGet(const wchar_t *_alias)
{
	int i; const wchar_t *p;
	if(_alias!=NULL){
	}else if(_alias==NULL){
		for(i=0;i<256;i++){
			if(useindex[i]==0){
				p=tmp_swprintf(L"s%03dsound",i);
				if(!aliaslist.exist(p)){
					_alias=p;
					useindex[i]=1;
					break;
				}else{
					useindex[i]=1;
				}
			}
		}
		if(_alias==NULL)_alias=ALIAS_COMMON;
	}
	if(!aliaslist.exist(_alias)){
		aliaslist.put(-1,_alias);
	}
	return _alias;
}
static void aliasRemove(const wchar_t *_alias)
{
	int i=aliaslist.indexOf(_alias);
	if(i>=0)aliaslist.remove(i);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
MciSound::MciSound(const wchar_t *_alias)
{
	SoundInit();
	type=SOUND_TYPE_NONE;
	_alias=aliasGet(_alias);
	wcssafecpy(alias,_alias,countof(alias));
}
MciSound::~MciSound()
{
	closeFile();
	aliasRemove(alias);
	if(soundLoop==this)soundLoop=NULL;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- ファイル入出力  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool MciSound::loadFile(const wchar_t *fpath,int _type)
{
	closeFile();
	type=_type;
	if(type==SOUND_TYPE_MIDI){
		wchar_t *p=tmp_swprintf(L"open \"%s\" type sequencer alias %s",fpath,alias);
		mciSendString(p,NULL,0,NULL);
	}else if(type==SOUND_TYPE_WAVE){
		wchar_t *p=tmp_swprintf(L"open \"%s\" type waveaudio alias %s",fpath,alias);
		mciSendString(p,NULL,0,NULL);
	}
	return true;
}
void MciSound::closeFile()
{
	if(type!=SOUND_TYPE_NONE){
		if(soundLoop==this)soundLoop=NULL;
		wchar_t *p=tmp_swprintf(L"close %s",alias);
		mciSendString(p,NULL,0,NULL);
	}
	type=SOUND_TYPE_NONE;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 再生  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void MciSound::play(bool loop)
{
	wchar_t *p;
	if(type==SOUND_TYPE_MIDI){
		if(loop)soundLoop=this;
		p=tmp_swprintf(L"play %s from 0 notify",alias);
		mciSendString(p,NULL,0,hwndSound);
	}else if(type==SOUND_TYPE_WAVE){
		p=tmp_swprintf(L"play %s",alias);
		mciSendString(p,NULL,0,NULL);
	}
}
void MciSound::stop()
{
	if(type!=SOUND_TYPE_NONE){
		if(soundLoop==this)soundLoop=NULL;
		wchar_t *p=tmp_swprintf(L"stop %s",alias);
		mciSendString(p,NULL,0,NULL);
	}
}

#endif
