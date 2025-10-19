#pragma once

#include "Common.h"
#include "Object.h"

namespace DotNetDupe {
    namespace System {
        class Random : public Object {
        public:
            DOTNETDUPE_API Random();
            DOTNETDUPE_API Random(int seed);

            DOTNETDUPE_API virtual int Next();
            DOTNETDUPE_API virtual int Next(int maxValue);
            DOTNETDUPE_API virtual int Next(int minValue, int maxValue);
            DOTNETDUPE_API virtual void NextBytes(unsigned char* buffer, int bufferSize);
            DOTNETDUPE_API virtual double NextDouble();
        private:
            int _seed;
        };
    }
}