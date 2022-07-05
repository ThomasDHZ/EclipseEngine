#pragma once
#include <memory>
#include <iostream>
#include <al.h>
#include <alext.h>
#include <sndfile.h>

class MusicPlayer
{
private:
	static constexpr int BufferSamples = 8192;
	static constexpr int BufferCount = 4;
	
	ALuint Source;
	ALuint SoundBuffers[BufferCount];
	SNDFILE* SoundFile;
	SF_INFO SoundFileInfo;
	short* MemoryBuffer;
	ALenum Format;

public:
	MusicPlayer();
	~MusicPlayer();

	void StartUp(const char* filename);
	void Play();
	void Pause();
	void Stop();
	void UpdateBufferStream();
	void Destroy();
};

