#include "MusicPlayer.h"

MusicPlayer::MusicPlayer()
{
}

MusicPlayer::~MusicPlayer()
{
}

void MusicPlayer::StartUp(const char* filename)
{
	alGenSources(1, &Source);
	alGenBuffers(BufferCount, SoundBuffers);

	std::size_t frame_size;

	SoundFile = sf_open(filename, SFM_READ, &SoundFileInfo);
	if (SoundFile == nullptr)
	{
		std::cout << stderr << "Couldn't open sound file: " << filename << std::endl;
		std::cout << sf_strerror << std::endl;
	}
	if (SoundFileInfo.frames < 1 ||
		SoundFileInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / SoundFileInfo.channels)
	{
		std::cout << stderr << "Bad Sample Count" << std::endl;
		std::cout << sf_strerror << std::endl;

		sf_close(SoundFile);
	}

	Format = AL_NONE;
	switch (SoundFileInfo.channels)
	{
	case 1: Format = AL_FORMAT_MONO16; break;
	case 2: Format = AL_FORMAT_STEREO16; break;
	case 3:
	{
		if (sf_command(SoundFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			Format = AL_FORMAT_BFORMAT2D_16;
		}
		break;
	}
	case 4:
	{
		if (sf_command(SoundFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			Format = AL_FORMAT_BFORMAT3D_16;
		}
		break;
	}
	default:
	{
		std::cout << stderr << "Bad Channel Count" << std::endl;
		sf_close(SoundFile);
	}
	};

	frame_size = ((size_t)BufferSamples * (size_t)SoundFileInfo.channels) * sizeof(short);
	MemoryBuffer = static_cast<short*>(malloc(frame_size));

	alSourceRewind(Source);
	alSourcei(Source, AL_BUFFER, 0);

	ALsizei x;
	for (x = 0; x < BufferCount; x++)
	{
		{
			sf_count_t BufferSize = sf_readf_short(SoundFile, MemoryBuffer, BufferSamples);
			if (BufferSize < 1)
			{
				break;
			}

			BufferSize *= SoundFileInfo.channels * (sf_count_t)sizeof(short);
			alBufferData(SoundBuffers[x], Format, MemoryBuffer, (ALsizei)BufferSize, SoundFileInfo.samplerate);
		}
	}
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "Couldn't buffering for playback." << std::endl;
	}

	alSourceQueueBuffers(Source, x, SoundBuffers);
}

void MusicPlayer::Play()
{
	alSourcePlay(Source);
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "Couldn't start playback." << std::endl;
	}
}

void MusicPlayer::Pause()
{
	alSourcePause(Source);
}

void MusicPlayer::Stop()
{
	alSourceStop(Source);
}

void MusicPlayer::UpdateBufferStream()
{
	ALint processed;
	ALint state;

	alGetError();
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	if (state == AL_STOPPED || 
		state == AL_PAUSED)
	{
		return;
	}

	alGetSourcei(Source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "Couldn't check buffer status." << std::endl;
	}

	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(Source, 1, &bufid);
		processed--;

		slen = sf_readf_short(SoundFile, MemoryBuffer, BufferSamples);
		if (slen > 0)
		{
			slen *= SoundFileInfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, Format, MemoryBuffer, (ALsizei)slen, SoundFileInfo.samplerate);
			alSourceQueueBuffers(Source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			std::cout << "Couldn't load music buffer." << std::endl;
		}
	}

	if (state != AL_PLAYING && 
		state != AL_PAUSED)
	{
		ALint queued;
		alGetSourcei(Source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
		{
			return;
		}

		alSourcePlay(Source);
		if (alGetError() != AL_NO_ERROR)
		{
			std::cout << "Couldn't restart music." << std::endl;
		}
	}
}

void MusicPlayer::Destroy()
{
	if (SoundFile != nullptr)
	{
		sf_close(SoundFile);
		SoundFile = nullptr;
	}

	alDeleteSources(1, &Source);

	free(MemoryBuffer);
	alDeleteBuffers(BufferCount, SoundBuffers);
}
