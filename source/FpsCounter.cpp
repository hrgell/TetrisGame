#include "stdafx.h"
#include "FpsCounter.h"


FpsCounter::FpsCounter()
	: maxfps(0), fps(0), fraction(0), position(0), msg()
{
}

FpsCounter::~FpsCounter()
{
}

void FpsCounter::Initialize(float maxfps, float fps)
{
	this->maxfps = maxfps;
	this->fps = fps;
	SetFraction();
}

void FpsCounter::ReInitialize(float fps)
{
	this->fps = fps;
	SetFraction();
}

void FpsCounter::SetFraction()
{
	fraction = (fps < maxfps) ? maxfps / fps : 1;
}

bool FpsCounter::Tick(size_t tick)
{
	if (position < tick) {
		position += fraction;
		return true;
	}
	return false;
}

void FpsCounter::speedup() {
	if (fps < maxfps) {
		fps *= 1.1;
		if (fps > maxfps)
			fps = maxfps;
		SetFraction();
	}
}
