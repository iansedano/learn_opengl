#include <random>
#include <chrono>
#include "random.h"

int getRandomNumber(int min, int max)
{
	int seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::mt19937 generator(seed);
	std::uniform_int_distribution<uint_least32_t> distribute(min, max);

	return distribute(generator);
}