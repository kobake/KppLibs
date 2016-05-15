#pragma once

#include "c_Font.h"
#include <MathLib.h> // Align
using namespace math;

// -- -- -- -- 定数 -- -- -- -- //

// 装飾 (4～5ビット)
#define EX_NONE		0
#define EX_BOLD		16
#define EX_AROUND	32
#define EX_SHADOW	48

#define EX_FLAG(flag)	((flag) & 0x30) //110000


//色定数
//#define KCOLOR_NULL		0x1000000
//#define KCOLOR_DKGRAY	0x1000001
//#define KCOLOR_BLACK	0x0000000

#include "c_Brush.h"

#include <GraphicsLib.h> // Graphics

typedef void (*PAINTPROC)(CDcGraphics *g);


