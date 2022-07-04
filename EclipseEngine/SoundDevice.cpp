#include "SoundDevice.h"
ALCdevice* SoundDevice::alcDevice = nullptr;
ALCcontext* SoundDevice::alcContext = nullptr;


SoundDevice::SoundDevice()
{
}

SoundDevice::~SoundDevice()
{
}

void SoundDevice::StartUp()
{
	alcDevice = alcOpenDevice(nullptr);
	if (!alcDevice)
	{
		std::cout << "Sound device isn't working." << std::endl;
	}

	alcContext = alcCreateContext(alcDevice, nullptr);
	if (!alcContext)
	{
		std::cout << "Couldn't get sound context" << std::endl;
	}

	if (!alcMakeContextCurrent(alcContext))
	{
		std::cout << "Couldn't make context current" << std::endl;
	}
}

void SoundDevice::Destroy()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(alcContext);
	alcCloseDevice(alcDevice);

	alcContext = nullptr;
	alcDevice = nullptr;
}
