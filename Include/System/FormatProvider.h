/// \file FormatProvider.h
/// \brief Defines the IFormatProvider<T> interface for retrieving formatting objects.
///
/// Modeled after .NET System.IFormatProvider (ECMA-335).

#pragma once
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        /// \interface IFormatProvider
        /// \brief Provides a mechanism for retrieving an object to control formatting.
        ///
        /// \tparam T The type of format service to retrieve.
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        template <class T>
        class IFormatProvider {
        public:
            /// \brief Virtual destructor for polymorphic cleanup.
            virtual ~IFormatProvider() = default;

            /// \brief Returns an object that provides formatting services for the specified type.
            /// \param formatType An object that specifies the type of format object to return.
            /// \return An instance of the object that provides formatting services, or nullptr if unavailable.
            virtual Object* GetFormat(const T* formatType) = 0;
        };
    }
}
