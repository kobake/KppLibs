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

//’è”
const Compass Compass::West			(0);
const Compass Compass::North		(1);
const Compass Compass::East			(2);
const Compass Compass::South		(3);
const Compass Compass::NoCompass	(4);

//’è”
const Course Course::Forward	(0); //‘O•û
const Course Course::Righthand	(1); //‰Eè•ûŒü
const Course Course::Backward	(2); //Œã•û
const Course Course::Lefthand	(3); //¶è•ûŒü
const Course Course::NoCourse	(4);

#endif
