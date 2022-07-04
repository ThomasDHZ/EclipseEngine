#pragma once
#include <memory>
#include <iostream>
#include <alc.h>

class SoundDevice
{
private:
	static ALCdevice* alcDevice;
	static ALCcontext* alcContext;

public:
	SoundDevice();
	~SoundDevice();

	static void StartUp();
	static void Destroy();
};

