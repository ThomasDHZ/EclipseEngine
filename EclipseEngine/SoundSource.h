#pragma once
#include <al.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SoundSource
{
private:
	ALuint Buffer = 0;
	ALuint Source = 0;
	float Pitch = 1.0f;
	float Gain = 1.0f;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Velocity = glm::vec3(0.0f);
	bool LoopSound = false;

public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint playBuffer);
	void Destroy();
};

