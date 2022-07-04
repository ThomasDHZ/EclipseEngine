#include "SoundSource.h"

SoundSource::SoundSource()
{
	alGenSources(1, &Source);
	alSourcef(Source, AL_PITCH, Pitch);
	alSourcef(Source, AL_GAIN, Gain);
	alSource3f(Source, AL_POSITION, Position.x, Position.y, Position.z);
	alSource3f(Source, AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
	alSourcei(Source, AL_LOOPING, LoopSound);
	alSourcei(Source, AL_BUFFER, Buffer);
}

SoundSource::~SoundSource()
{
}

void SoundSource::Play(const ALuint playBuffer)
{
	if (playBuffer != Buffer)
	{
		Buffer = playBuffer;
		alSourcei(Source, AL_BUFFER, Buffer);
	}

	alSourcePlay(Source);
}

void SoundSource::Destroy()
{
	alDeleteSources(1, &Source);
}
