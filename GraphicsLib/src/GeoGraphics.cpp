#include "common/_required.h"
#include "GeoGraphics.h"


namespace gr{

//éläpå`
void GeoGraphics::drawQuad(const Quad2& quad)
{
	for(int i=0;i<4;i++){
		DrawLine(quad.at(i),quad.at((i+1)%4));
	}
}
void GeoGraphics::fillQuad(const Quad2& quad)
{
	not_support(L"fillQuad");
}


} //namespace gr
