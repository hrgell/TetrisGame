#pragma once
#include <string>

class FpsCounter
{
public:
	float maxfps;
	float fps;
	float fraction;
	float position;
	std::string msg;
	FpsCounter();
	~FpsCounter();
	void Initialize(float maxfps, float fps);
	void ReInitialize(float fps);
	void SetFraction();
	bool Tick(size_t tick);
	void speedup();
};

