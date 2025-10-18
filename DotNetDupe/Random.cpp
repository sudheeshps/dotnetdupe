#include "pch.h"
#include "Random.h"
#include <random>

namespace DotNetDupe
{
    namespace System
    {
        Random::Random() : _seed(0) {}

        Random::Random(int seed) : _seed(seed) {}

        int Random::Next()
        {
            std::mt19937 gen(_seed);
            std::uniform_int_distribution<> distrib(0, 2147483647);
            return distrib(gen);
        }

        int Random::Next(int maxValue)
        {
            std::mt19937 gen(_seed);
            std::uniform_int_distribution<> distrib(0, maxValue);
            return distrib(gen);
        }

        int Random::Next(int minValue, int maxValue)
        {
            std::mt19937 gen(_seed);
            std::uniform_int_distribution<> distrib(minValue, maxValue);
            return distrib(gen);
        }

        void Random::NextBytes(unsigned char* buffer, int bufferSize)
        {
            std::mt19937 gen(_seed);
            std::uniform_int_distribution<> distrib(0, 255);
            for (int i = 0; i < bufferSize; ++i)
            {
                buffer[i] = static_cast<unsigned char>(distrib(gen));
            }
        }

        double Random::NextDouble()
        {
            std::mt19937 gen(_seed);
            std::uniform_real_distribution<> distrib(0.0, 1.0);
            return distrib(gen);
        }
    }
}
