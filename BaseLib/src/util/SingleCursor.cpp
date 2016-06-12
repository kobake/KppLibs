#include <BaseLib.h>
#include "SingleCursor.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
SingleCursor::SingleCursor()
{
	setRange(0,0);
	pos=range_min;
	loop=false;
}
SingleCursor::SingleCursor(int _range_min,int _range_max,bool _loop)
{
	setRange(_range_min,_range_max);
	pos=range_min;
	loop=_loop;
}
SingleCursor::~SingleCursor()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           動作                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void SingleCursor::setRange(int _range_min,int _range_max)
{
	if(_range_min>_range_max)throw invalid_range();
	range_min=_range_min;
	range_max=_range_max;
	if(pos<range_min)pos=range_min;
	if(pos>range_max)pos=range_max;
}

void SingleCursor::setLoop(bool _loop)
{
	loop=_loop;
}

void SingleCursor::setPos(int _pos)
{
	if(_pos<range_min || _pos>range_max)throw invalid_pos();
	pos=_pos;
}

void SingleCursor::move(int offset)
{
	pos+=offset;
	if(loop){
		while(pos<range_min)pos+=range_max-range_min+1;
		while(pos>range_max)pos-=range_max-range_min+1;
	}else{
		if(pos<range_min)pos=range_min;
		if(pos>range_max)pos=range_max;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ラッパー                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void SingleCursor::setRange(int _range_min,int _range_max,bool _loop)
{
	setRange(_range_min,_range_max);
	setLoop(_loop);
}
