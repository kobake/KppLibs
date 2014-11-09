#include "common/_required.h"
#include "CDib.h"
#include <StringLib.h>
#include <windows.h>
#include <DebugLib.h>
using namespace std;
using namespace math;

static int getWidthBytes(int width,int bitcount)
{
	int wbytes=(width*bitcount+7)/8; //1ビットでもはみ出ていれば1バイトとしてカウントする
	wbytes=(wbytes+3)/4*4; //1バイトでもはみ出ていれば4バイトとしてカウントする
	return wbytes;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         内部構造                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDib::_construct(const math::CBoxSize& _size,int depth,int size_image)
{
	//まずは解放
	_destroy();

	//データ幅
	m_wbytes=getWidthBytes(_size.w,depth);

	//イメージサイズ計算
	if(size_image==0)size_image=m_wbytes*_size.h;

	//確保
	m_bmpinfo=(BITMAPINFO*)new byte[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256];
	m_bits=new byte[size_image];

	//BITMAPINFOHEADER設定
	BITMAPINFOHEADER* bmh=&m_bmpinfo->bmiHeader;
	bmh->biSize=sizeof(BITMAPINFOHEADER);
	bmh->biWidth=_size.w;
	bmh->biHeight=_size.h;
	bmh->biPlanes=1;
	bmh->biBitCount=depth;
	bmh->biCompression=BI_RGB;
	bmh->biSizeImage=size_image;
	bmh->biXPelsPerMeter=0;
	bmh->biYPelsPerMeter=0;
	bmh->biClrUsed=0;
	bmh->biClrImportant=0;

	//パレットのデフォルト色
	if(depth==1){
		m_bmpinfo->bmiColors[0]=CColor(0,0,0).win_color2();
		m_bmpinfo->bmiColors[1]=CColor(255,255,255).win_color2();
	}
}
void CDib::_destroy()
{
	delete[] ((byte*)m_bmpinfo); m_bmpinfo=NULL;
	m_delete2(m_bits);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#include <IoLib.h> // CFileSystem

CDib::CDib()
: m_bmpinfo(NULL), m_bits(NULL), m_wbytes(0)
{
	_construct(math::CBoxSize(1,1),1,0);
}

CDib::CDib(const BITMAPINFO& bmpinfo, const void* bits)
: m_bmpinfo(NULL), m_bits(NULL), m_wbytes(0)
{
	Assign(bmpinfo, bits);
}

CDib::CDib(const math::CBoxSize& _size,int depth)
: m_bmpinfo(NULL), m_bits(NULL), m_wbytes(0)
{
	_construct(_size,depth,0);
}

CDib::CDib(const wchar_t* path)
: m_bmpinfo(NULL), m_bits(NULL), m_wbytes(0)
{
	CFileInputStream in(path,L"rb");
	Read(in);
}

CDib::~CDib()
{
	_destroy();
}

void CDib::Assign(const BITMAPINFO& bmpinfo, const void* bits)
{
	_construct(
		math::CBoxSize(bmpinfo.bmiHeader.biWidth, bmpinfo.bmiHeader.biHeight),
		bmpinfo.bmiHeader.biBitCount,
		0
	);
	//コピー
	memcpy(m_bits, bits, this->GetBitsSize());
	memcpy(m_bmpinfo->bmiColors, bmpinfo.bmiColors, sizeof(RGBQUAD) * this->GetPaletteCount());
}

void CDib::Assign(HDC hdc)
{
	//サイズ取得
	int w = ::GetDeviceCaps(hdc, HORZRES);
	int h = ::GetDeviceCaps(hdc, VERTRES);

	//メモリDC作成
	HBITMAP hbmpTmp = ::CreateCompatibleBitmap(hdc, w, h);
	HDC hdcTmp = ::CreateCompatibleDC(hdc);
	::SelectObject(hdcTmp, hbmpTmp);

	//メモリDCにコピー
	::BitBlt(hdcTmp, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

	//ビットマップ取得
	this->_construct(math::CBoxSize(w,h), 24, 0);
	::GetDIBits(hdcTmp, hbmpTmp, 0, h, this->m_bits, this->m_bmpinfo, 0);

	//メモリDC解放
	::DeleteDC(hdcTmp);
	::DeleteObject(hbmpTmp);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          コピー                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CDib* CDib::Clone() const
{
	//###未実装
	return NULL;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           変換                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDib::Resize(const math::CBoxSize& new_size)
{
	//前のグラフィック情報は保持しない
	_construct(new_size,GetBitsPerPixel(),0);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           情報                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CBoxSize CDib::GetSize() const
{
	return math::CBoxSize(m_bmpinfo->bmiHeader.biWidth,tabs(m_bmpinfo->bmiHeader.biHeight));
}
int CDib::GetPaletteCount() const
{
	int nDepth = m_bmpinfo->bmiHeader.biBitCount;
	switch(nDepth){
	case 1: return 2;
	case 2: return 4;
	case 4: return 16;
	case 8: return 256;
	default:
		return 0;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           描画                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDib::DrawTo(HDC hdc, const math::CPoint& ptDst)
{
	math::CBoxSize size = this->GetSize();
	::StretchDIBits(
		hdc,
		ptDst.x, ptDst.y, size.w, size.h,
		0, 0, size.w, size.h,
		this->m_bits,
		this->m_bmpinfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ストリーム                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//sizeof(BITMAPINFOHEADER)=40

//m_bmpinfo,m_wbytes,m_bits変更
void CDib::Read(CInputStream& in)
{
	// -- -- ファイル情報の解析 -- -- //
	//BITMAPFILEHEADER
	const WORD BM=(*((WORD*)"BM"));
	const WORD TM=(*((WORD*)"TM")); //###テスト用
	BITMAPFILEHEADER bmpfheader;
	in.Read(&bmpfheader,sizeof(bmpfheader));
	if(bmpfheader.bfType!=BM && bmpfheader.bfType!=TM){
		throw NotDib(L"ビットマップではありません");
	}
	//BITMAPINFOHEADER
	BITMAPINFOHEADER bmiheader;
	in.Read(&bmiheader,sizeof(bmiheader));
	int bitcount=bmiheader.biBitCount;
	if(bitcount!=1 && bitcount!=4 && bitcount!=8 && bitcount!=16 && bitcount!=24 && bitcount!=32){
		throw DibException(L"2色, 16色, 256色, 2^16色, 2^24色, 2^32色 以外の形式には対応していません");
	}
	if(bmiheader.biCompression!=BI_RGB){
		throw DibException(L"圧縮された形式には対応していません");
	}
	//定義色数
	int maxcolors=0;
	if(bmiheader.biBitCount==1)maxcolors=2;
	if(bmiheader.biBitCount==4)maxcolors=16;
	if(bmiheader.biBitCount==8)maxcolors=256;
	if(bmiheader.biClrUsed==0){
		bmiheader.biClrUsed=maxcolors;
	}
	// -- -- メモリ確保 -- -- //
	int h=tabs(bmiheader.biHeight);
	_construct(math::CBoxSize(bmiheader.biWidth,h),bmiheader.biBitCount,bmiheader.biSizeImage);
	// -- -- データ準備 -- -- //
	memcpy(&m_bmpinfo->bmiHeader,&bmiheader,sizeof(bmiheader));
	//RGBQUAD
	RGBQUAD defcolor={0,0,0,0};
	in.Read(m_bmpinfo->bmiColors,sizeof(RGBQUAD)*bmiheader.biClrUsed);
	for(int i=bmiheader.biClrUsed;i<maxcolors;i++){ //残りを 0 で埋める
		m_bmpinfo->bmiColors[i]=defcolor;
	}
	//m_bits (bmiheader.biHeightが負の場合は、トップダウン→ボトムアップに修正)
	in.seek(bmpfheader.bfOffBits,Io::ESEEK_BEGIN);
	if(bmiheader.biHeight>0){
		//そのまま読み込み
		in.Read(m_bits,bmiheader.biSizeImage);
	}else{
		//トップダウン→ボトムアップに修正
		for(int j=0;j<h;j++){
			in.Read(&m_bits[(h-1-j)*m_wbytes],m_wbytes);
		}
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           転送                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!ピクセル転送
void CDib::SetBits(const math::CPoint& ptDst,const CDib& dibSrc,const math::CRect& rcSrc)
{
	//非圧縮のみ対応
	assert(m_bmpinfo->bmiHeader.biCompression==BI_RGB);

	int mybitcount=m_bmpinfo->bmiHeader.biBitCount;

	//実際に転送する矩形(l,t,r,b)を計算
	if(!CRect(GetSize()).IsContains(ptDst))return; //転送先が範囲外の場合、終了
	int l=ptDst.x;
	int t=ptDst.y;
	int r=l+rcSrc.Width();  if(r>GetSize().w)r=GetSize().w;
	int b=t+rcSrc.Height(); if(b>GetSize().h)b=GetSize().h;

	//転送
	math::CPoint p=ptDst;
	math::CPoint q=rcSrc.GetUpperLeft();
	for(int j=t;j<b;j++){
		for(int i=l;i<r;i++){
			SetPixel(p,dibSrc.GetPixel(q));
			p.x++;
			q.x++;
		}
		p.y++;
		q.y++;
		p.x=0;
		q.x=0;
	}
}

struct Color16{
	ushort b : 5;
	ushort g : 5;
	ushort r : 5;
	ushort a : 1; //未使用
};

void CDib::SetPixel(const math::CPoint& pt,const CColor& color)
{
	//非圧縮のみ対応
	assert(m_bmpinfo->bmiHeader.biCompression==BI_RGB);

	int h=tabs(m_bmpinfo->bmiHeader.biHeight);
	int mybitcount=m_bmpinfo->bmiHeader.biBitCount;

	//RGB: ペタっと転送
	if(mybitcount==16){
		Color16* p=(Color16*)&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		p->r=color.r*31/255;
		p->g=color.g*31/255;
		p->b=color.b*31/255;
		p->a=0;
	}
	else if(mybitcount==24){
		byte* p=&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		p[0]=color.b;
		p[1]=color.g;
		p[2]=color.r;
	}
	else if(mybitcount==32){
		byte* p=&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		p[0]=color.b;
		p[1]=color.g;
		p[2]=color.r;
	}
	//パレット: 近い色を探して転送
	else if(mybitcount==1){
		assert(0); //###未実装
	}
	else if(mybitcount==4){
		assert(0); //###未実装
	}
	else if(mybitcount==8){
//###未実装
		byte* p=&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		RGBQUAD* pal=m_bmpinfo->bmiColors;
		int npal=(1L<<mybitcount);
		int dist=300;
		for(int i=0;i<npal;i++){
		}
		*p=0; //てすと
	}
}

CColor CDib::GetPixel(const math::CPoint& pt) const
{
	//非圧縮のみ対応
	assert(m_bmpinfo->bmiHeader.biCompression==BI_RGB);

	int h=tabs(m_bmpinfo->bmiHeader.biHeight);
	int mybitcount=m_bmpinfo->bmiHeader.biBitCount;

	//RGB: ペタっと転送
	if(mybitcount==16){
		Color16* p=(Color16*)&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		return CColor(p->r,p->g,p->b);
	}
	else if(mybitcount==24){
		byte* p=&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		return CColor(p[0],p[1],p[2]);
	}
	else if(mybitcount==32){
		byte* p=&m_bits[m_wbytes*(h-1-pt.y)+pt.x*mybitcount/8];
		return CColor(p[0],p[1],p[2]);
	}
	//パレット: パレットを参照して転送
	else if(mybitcount==1){
		assert(0); //###未実装
	}
	else if(mybitcount==4){
		assert(0); //###未実装
	}
	else if(mybitcount==8){
		assert(0); //###未実装
	}
	return CColor(0,0,0);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    実装インターフェース                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

size_t CDib::GetBitmapInfoSize() const
{
	return sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256;
}

size_t CDib::GetBitsSize() const
{
	return m_bmpinfo->bmiHeader.biSizeImage;
}

