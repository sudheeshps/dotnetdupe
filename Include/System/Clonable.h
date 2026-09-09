/// \file Clonable.h
/// \brief Defines the IClonable interface for creating duplicate instances of an object.
///
/// Modeled after .NET System.ICloneable (ECMA-335).

#pragma once
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        /// \interface IClonable
        /// \brief Supports cloning, which creates a new instance of a class with the same value as an existing instance.
        ///
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        class IClonable {
        public:
            /// \brief Virtual destructor for polymorphic cleanup.
            virtual ~IClonable() = default;

            /// \brief Creates a new object that is a copy of the current instance.
            /// \return A new object that is a copy of this instance.
            virtual Object Clone() = 0;
        };
    }
}
