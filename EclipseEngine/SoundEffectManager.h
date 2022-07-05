#pragma once
#include <memory>
#include <iostream>
#include <al.h>
#include <alext.h>
#include <vector>
#include <sndfile.h>

class SoundEffectManager
{
private:
	static std::vector<ALuint> SoundEffectBufferList;

public:

	SoundEffectManager();
	~SoundEffectManager();

	static ALuint AddSoundEffect(const char* filename);
	static bool RemoveSoundEffect(const ALuint& buffer);

	static void StartUp();
	static void Destroy();
};

