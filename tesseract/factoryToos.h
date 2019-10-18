#ifndef FACTORYTOOTH_H
#define FACTORYTOOTH_H
#include "RandomToos.h"
#include "ButtonCowboy.h"

class FactoryToos
{
public:
	static IBasicToos* Get();
};

IBasicToos* FactoryToos::Get()
{
	IBasicToos* res;
	switch(random(0, 2))
	{
		case 0: res = new RandomToos();
				break;
		case 1: res = new ButtonCowboy();
				break;
	}
	return res;
}

#endif
