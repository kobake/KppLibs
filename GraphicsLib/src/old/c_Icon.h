#pragma once

class App;

class Icon{
protected:
	HICON hIcon;
	bool is_system;
public:
	friend class Window;
	//コンストラクタ・デストラクタ
	Icon(App *app,int id,int w=0,int h=0);
	Icon(Icon *src);
	virtual ~Icon();
	Icon *clone();
	//
	HICON _getHICON(){ return hIcon; }
};


