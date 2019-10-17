#ifndef FACTORYTOOTH_H
#define FACTORYTOOTH_H
#include "RandomToos.h"

class FactoryToos
{
public:
	static IBasicToos* Get();
};

IBasicToos* FactoryToos::Get()
{
	IBasicToos* res;
	switch(random(0, 1))
	{
		case 0: res = new RandomToos();
				break;
	}
	return res;
}

#endif
