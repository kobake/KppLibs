#pragma once

HFONT CreateSizeFont(char *face,int fh,int fw);
HFONT CreatePointFont(char *face,int point);
HDC CreateSizeDC(int w,int h,BOOL clear);
