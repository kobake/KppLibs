#pragma once

class App;

class Icon{
protected:
	HICON hIcon;
	bool is_system;
public:
	friend class Window;
	//�R���X�g���N�^�E�f�X�g���N�^
	Icon(App *app,int id,int w=0,int h=0);
	Icon(Icon *src);
	virtual ~Icon();
	Icon *clone();
	//
	HICON _getHICON(){ return hIcon; }
};


