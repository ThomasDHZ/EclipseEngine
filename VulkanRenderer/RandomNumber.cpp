#include "RandomNumber.h"

int RandomNumber::GenerateRandomInt(int Min, int Max)
{
	std::mt19937 rng = std::mt19937(std::random_device{}());
	std::uniform_int_distribution<> randomizer(Min, Max);
	float randomInt = randomizer(rng);
	return randomInt;
}

float RandomNumber::GenerateRandomFloat(float Min, float Max)
{
	std::mt19937 rng = std::mt19937(std::random_device{}());
	std::uniform_real_distribution<float> randomizer(Min, Max);
	float randomFloat = randomizer(rng);
	return randomFloat;
}
