#pragma once

#define FONT_BOLD		1
#define FONT_ITALIC		2
#define FONT_UNDERLINE	4
#define FONT_STRIKE		8

class Font{
protected:
	wchar face[LF_FACESIZE];
	int point;
	int height;
	int width;
	int option;
	HFONT hFont;
public:
	void _init_v();
	Font(const wchar *_face,int _point,int _option=0);
	Font(const wchar *_face,int _height,int _width,int _option=0);
	virtual ~Font();
public:
	operator HFONT() const;
public:
	int getTextWidth(wchar *str);
};



