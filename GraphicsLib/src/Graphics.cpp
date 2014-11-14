#include "common/_required.h"
#include "Graphics.h"
//#include "myexception.h"
#include <StringLib.h>
#include <DebugLib.h>
#include "../../GraphicsLib/src/CImage.h"
using namespace util;
#include <vector>
using namespace math;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       デバッグ時通知                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Graphics::not_support(const wchar_t* method) const
{
	wprintf(L"method [%s] is not support\n",method);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
Graphics::Graphics()
{
	reserve_color[0]=reserve_color[1]=CColor(0,0,0);
	line_height=-1;
	setAreaWidth(-1);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      ラッパー設定                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Graphics::setLineHeight(int _line_height)
{
	line_height=_line_height;
}

void Graphics::setColor2(const CColor& color0,const CColor& color1)
{
	reserve_color[0]=color0;
	reserve_color[1]=color1;
	SetColor(reserve_color[0]);
}

void Graphics::setAreaWidth(int _width)
{
	if(_width==-1)
		area_width=INT_MAX;
	else
		area_width=_width;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ラッパー                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
int Graphics::fontHeight()
{
	return fontSize(L"A",1).h;
}

int Graphics::fontWidth(const wchar_t* str)
{
	return fontSize(str).w;
}

CBoxSize Graphics::fontSize(const wchar_t* str)
{
	return fontSize(str,wcslen(str));
}



void Graphics::drawImage(t_Point& ptDst,const CImage& imgSrc)
{
	drawImage(ptDst,imgSrc,math::CRect(imgSrc.GetSize()));
}

void Graphics::drawImage(const t_Point& ptDst,const CImage& imgSrc,const t_Rect& ptSrc,const math::Align& align)
{
	drawImage(align.adjust(ptDst,imgSrc.GetSize()),imgSrc);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       文字列ラッパー                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Graphics::drawString(const wchar_t* str,const t_Point& pos,const math::Align& align)
{
	drawString(str,align.adjust(pos,fontSize(str)));
}

void Graphics::drawStringEx(const wchar_t* str,const t_Point& pos,Decoration dec,const math::Align& align)
{
	_drawStringAlignImp(str,pos,area_width,dec,align);
}



//装飾付き
void Graphics::_drawStringDecImp(const wchar_t* str,const t_Point& pos,Decoration dec)
{
	if(dec & DEC_NORMAL){
		drawString(str,pos);
	}
	else if(dec & DEC_BOLD){
		drawString(str,pos);
		drawString(str,pos+t_Point(1,0));
	}
	else if(dec & DEC_AROUND){
		int i,j;
		SetColor(reserve_color[1]);
		for(j=-1;j<=1;j++){
			for(i=-1;i<=1;i++){
				if(i!=0 || j!=0){
					drawString(str,pos+t_Point(i,j));
				}
			}
		}
		SetColor(reserve_color[0]);
		drawString(str,pos);
	}
	else if(dec & DEC_SHADOW){
		SetColor(reserve_color[1]);
		drawString(str,pos+t_Point(1,1));
		SetColor(reserve_color[0]);
		drawString(str,pos);
	}else{
		drawString(str,pos);
	}
}

//配置機能と装飾付き
void Graphics::_drawStringAlignImp(const wchar_t* str,const t_Point& _pos,int area_width,Decoration dec,const math::Align& align)
{
	int lh=line_height;
	if(lh==-1)lh=fontSize(L"A").h;
	t_Point pos=_pos;
	//行に分解
	vector<wstring> lines=_splitString(str,area_width);
	//描画
	for(size_t i=0;i<lines.size();i++){
		const wchar_t* line=lines[i].c_str();
		_drawStringDecImp(line,align.adjust(pos,fontSize(line)),dec);
		pos.y+=lh;
	}
}

vector<wstring> Graphics::_splitString(const wchar_t* str,int area_width) //###もっと最適化可能
{
	int str_len=(int)wcslen(str);
	int i,j;
	wstring line;
	vector<wstring> ret;
	//
	i=0;
	while(i<str_len){
		//1行取得 → line
		line=L"";
		for(j=i+1;j<=str_len;j++){
			if(str[j-1]==L'\n'){
				line.assign(&str[i],&str[j-1]);
				break;
			}
			else if( fontSize(&str[i],j-i).w > area_width ){
				j--;
				assert(i<j);
				line.assign(&str[i],&str[j]);
				break;
			}
		}
		if(j==str_len+1){
			line.assign(&str[i],&str[str_len]);
		}
		//配列生成
		ret.push_back(line);
		//次の行へ
		i=j;
	}
	return ret;
}
