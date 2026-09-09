/// \file InvalidOperationException.h
/// \brief Defines the exception thrown when a method call is invalid for the object's current state.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {

		/// \class InvalidOperationException
		/// \brief The exception that is thrown when a method call is invalid for the object's current state.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.28 (System.InvalidOperationException).
		class InvalidOperationException : public SystemException {
		public:
			/// \brief Initializes a new instance of the InvalidOperationException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			InvalidOperationException(const String& sMessage) : SystemException(sMessage) { }
		};

	}
}
