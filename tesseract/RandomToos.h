#ifndef RANDOMTOOS_H
#define RANDOMTOOS_H
#include "basictoos.h"

class RandomToos : public IBasicToos
{
public:
	RandomToos(){};
	int Run();
	~RandomToos(){};
};

int RandomToos::Run()
{
	return random(0, 1);
}

#endif
