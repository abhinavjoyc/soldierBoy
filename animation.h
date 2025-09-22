#pragma once
#include "timer.h"
class Animation {
	Timer time;
	int frameCount;
public:
	Animation() :time(0) , frameCount(0)
	{	}
	Animation(int frameCount, float length):frameCount(frameCount), time(length)
	{   }
	float getlength() {
		return time.getlength();
	}
	int currentFrame()  {
		return    static_cast<int>(((time.getTime() / time.getlength()) * frameCount));
	}
	void step(float deltatime) {
		time.step(deltatime);
	}
};
