#include "SoundEffectManager.h"

std::vector<ALuint> SoundEffectManager::SoundEffectBufferList;

SoundEffectManager::SoundEffectManager()
{
}

SoundEffectManager::~SoundEffectManager()
{
}

ALuint SoundEffectManager::AddSoundEffect(const char* filename)
{
	SNDFILE* SoundFile = nullptr;
	SF_INFO SoundFileInfo = {};
	SoundFile = sf_open(filename, SFM_READ, &SoundFileInfo);
	if (SoundFile == nullptr)
	{
		std::cout << stderr << "Couldn't open sound file: " << filename << std::endl;
		std::cout << sf_strerror << std::endl;
		return 0;
	}
	if (SoundFileInfo.frames < 1 ||
		SoundFileInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / SoundFileInfo.channels)
	{
		std::cout << stderr << "Bad Sample Count" << std::endl;
		std::cout << sf_strerror << std::endl;

		sf_close(SoundFile);
		return 0;
	}

	ALenum Format = AL_NONE;
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
			return 0;
		}
	};

	short* MemoryBuffer = nullptr;
	MemoryBuffer = static_cast<short*>(malloc((size_t)(SoundFileInfo.frames * SoundFileInfo.channels) * sizeof(short)));

	sf_count_t Frames = sf_readf_short(SoundFile, MemoryBuffer, SoundFileInfo.frames);
	if (Frames < 1)
	{
		free(MemoryBuffer);
		sf_close(SoundFile);

		std::cout << "Failed to read samples" << std::endl;
		return 0;
	}

	ALsizei Bytes = (ALsizei)(Frames * SoundFileInfo.channels) * (ALsizei)sizeof(short);

	ALuint Buffer = 0;
	alGenBuffers(1, &Buffer);
	alBufferData(Buffer, Format, MemoryBuffer, Bytes, SoundFileInfo.samplerate);

	free(MemoryBuffer);
	sf_close(SoundFile);

	ALenum Error = alGetError();
	if (Error != AL_NO_ERROR)
	{
		std::cout << "OpenAL Error: " << Error << std::endl;
		if (Buffer &&
			alIsBuffer(Buffer))
		{
			alDeleteBuffers(1, &Buffer);
		}
	}

	SoundEffectBufferList.emplace_back(Buffer);
	return Buffer;
}

bool SoundEffectManager::RemoveSoundEffect(const ALuint& buffer)
{
	for (int x = 0; x < SoundEffectBufferList.size(); x++)
	{
		if (buffer == SoundEffectBufferList[x])
		{
			alDeleteBuffers(1, &SoundEffectBufferList[x]);
			SoundEffectBufferList.erase(SoundEffectBufferList.begin() + x);
			return true;
		}
	}
	return false;
}

void SoundEffectManager::StartUp()
{
	alDeleteBuffers(SoundEffectBufferList.size(), SoundEffectBufferList.data());
	SoundEffectBufferList.clear();
}

void SoundEffectManager::Destroy()
{
	alDeleteBuffers(SoundEffectBufferList.size(), SoundEffectBufferList.data());
	SoundEffectBufferList.clear();
}
