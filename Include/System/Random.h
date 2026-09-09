/// \file Random.h
/// \brief Represents a pseudo-random number generator.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {

        /// \class Random
        /// \brief Represents a pseudo-random number generator, which is an algorithm that produces a sequence of numbers that meet certain statistical requirements for randomness.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.19 (System.Random).
        class Random : public Object {
        public:
            /// \brief Initializes a new instance of the Random class, using a time-dependent default seed value.
            DOTNETDUPE_API Random();

            /// \brief Initializes a new instance of the Random class, using the specified seed value.
            /// \param seed A number used to calculate a starting value for the pseudo-random number sequence.
            DOTNETDUPE_API Random(int seed);

            /// \brief Returns a non-negative random integer.
            /// \return A 32-bit signed integer that is greater than or equal to 0 and less than Int32.MaxValue.
            DOTNETDUPE_API virtual int Next();

            /// \brief Returns a non-negative random integer that is less than the specified maximum.
            /// \param maxValue The exclusive upper bound of the random number to be generated.
            /// \return A 32-bit signed integer that is greater than or equal to 0, and less than maxValue.
            DOTNETDUPE_API virtual int Next(int maxValue);

            /// \brief Returns a random integer that is within a specified range.
            /// \param minValue The inclusive lower bound of the random number returned.
            /// \param maxValue The exclusive upper bound of the random number returned.
            /// \return A 32-bit signed integer greater than or equal to minValue and less than maxValue.
            DOTNETDUPE_API virtual int Next(int minValue, int maxValue);

            /// \brief Fills the elements of a specified array of bytes with random numbers.
            /// \param buffer An array of bytes to contain random numbers.
            /// \param bufferSize The number of bytes to fill.
            DOTNETDUPE_API virtual void NextBytes(unsigned char* buffer, int bufferSize);

            /// \brief Returns a random floating-point number that is greater than or equal to 0.0, and less than 1.0.
            /// \return A double-precision floating point number that is greater than or equal to 0.0, and less than 1.0.
            DOTNETDUPE_API virtual double NextDouble();

        private:
            int _seed;
        };
    }
}