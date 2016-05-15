#ifndef _Brush_
#define _Brush_

//ブラシ定数（スタイルとストック兼用）
#define BRUSH_NORMAL	0
#define BRUSH_SOLID		0
#define BRUSH_NULL		1
#define BRUSH_HATCHED	2

//ブラシ定数（ストック専用）
#define BRUSH_BLACK		3
#define BRUSH_WHITE		4

//GetStockObject変換テーブル
#define BRUSH_STOCK_TABLE \
{\
	WHITE_BRUSH,	\
	NULL_BRUSH,		\
	NULL_BRUSH,		\
	BLACK_BRUSH,	\
	WHITE_BRUSH,	\
}

struct BRUSH_PARAM{
	int color;
	int flag;
	int stock;
	void set(int _color,int _flag)
	{
		color=_color;
		flag=_flag;
		stock=-1;
	}
	void set(int _stock)
	{
		color=flag=0;
		stock=_stock;
	}
	bool isStock()
	{
		return stock!=-1;
	}
};

class Brush{
protected:
	HBRUSH hBrush;
	BRUSH_PARAM brush_param;
public:
	//コンストラクタ・デストラクタ
	void _init_v();
	Brush(int color,int flag);
	Brush(int stock);
	Brush(BRUSH_PARAM *_brush_param);
	Brush(Brush &right);
	Brush *clone();
	~Brush();
	//
	bool eqParam(BRUSH_PARAM *_brush_param);
	BRUSH_PARAM getParam(){ return brush_param; }
	HBRUSH getHBRUSH(){ return hBrush; }
};

#endif
