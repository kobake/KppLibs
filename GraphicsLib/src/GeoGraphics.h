#pragma once

#include "FilterGraphics.h"
#include "Type.h"
#include "../../MathLib/MathLib.h" //Quad2

namespace gr{

class GeoGraphics : public FilterGraphics{
private:
	typedef math::Quad2 Quad2;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	GeoGraphics(Graphics* g) : FilterGraphics(g){ }
	//�l�p�`
	void drawQuad(const Quad2& quad);
	void fillQuad(const Quad2& quad);
};


} //namespace gr
