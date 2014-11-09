#pragma once

#include "../GraphicsLib/src/Type.h"


//#include <windef.h>
typedef struct tagPOINT WIN_POINT; //※これはSDKのバージョンにより違うので注意
typedef struct tagRECT WIN_RECT; //※これはSDKのバージョンにより違うので注意
typedef struct tagSIZE WIN_SIZE; //※これはSDKのバージョンにより違うので注意


// -- -- キャスト -- -- //
inline const WIN_POINT*	win_point(const math::Vec2Base<long>* p)	{ return (const WIN_POINT*)&p->x; }
inline WIN_POINT*		win_point(math::Vec2Base<long>* p)			{ return (WIN_POINT*)&p->x; }

inline const WIN_RECT*	win_rect(const math::RectBase<long>* p)		{ return (const WIN_RECT*)&p->m_left; }
inline WIN_RECT*		win_rect(math::RectBase<long>* p)			{ return (WIN_RECT*)&p->m_left; }

inline const WIN_SIZE*	win_size(const math::CBoxSize* p)					{ return (const WIN_SIZE*)&p->w; }
inline WIN_SIZE*		win_size(math::CBoxSize* p)						{ return (WIN_SIZE*)&p->w; }

