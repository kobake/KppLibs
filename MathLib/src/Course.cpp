#if 0

#include "_required.h"
#include "Compass.h"

const char* Compass::getName() const
{
	switch(value()){
	case 0: return "West";
	case 1: return "North";
	case 2: return "East";
	case 3: return "South";
	}
	return "NoCompass";
}

//定数
const Compass Compass::West			(0);
const Compass Compass::North		(1);
const Compass Compass::East			(2);
const Compass Compass::South		(3);
const Compass Compass::NoCompass	(4);

//定数
const Course Course::Forward	(0); //前方
const Course Course::Righthand	(1); //右手方向
const Course Course::Backward	(2); //後方
const Course Course::Lefthand	(3); //左手方向
const Course Course::NoCourse	(4);

#endif
