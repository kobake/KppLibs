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

//�萔
const Compass Compass::West			(0);
const Compass Compass::North		(1);
const Compass Compass::East			(2);
const Compass Compass::South		(3);
const Compass Compass::NoCompass	(4);

//�萔
const Course Course::Forward	(0); //�O��
const Course Course::Righthand	(1); //�E�����
const Course Course::Backward	(2); //���
const Course Course::Lefthand	(3); //�������
const Course Course::NoCourse	(4);

#endif
