#pragma once

#include "FilterGraphics.h"
#include "Type.h"
#include "../../MathLib/MathLib.h" //Quad2

namespace gr{

class GeoGraphics : public FilterGraphics{
private:
	typedef math::Quad2 Quad2;
public:
	//コンストラクタ・デストラクタ
	GeoGraphics(Graphics* g) : FilterGraphics(g){ }
	//四角形
	void drawQuad(const Quad2& quad);
	void fillQuad(const Quad2& quad);
};


} //namespace gr
