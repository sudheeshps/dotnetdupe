/// \file FormatException.h
/// \brief Defines the exception thrown when the format of an argument is invalid or not compliant with specifications.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {

		/// \class FormatException
		/// \brief The exception that is thrown when the format of an argument is invalid, or when a composite format string is not well formed.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.29 (System.FormatException).
		class FormatException : public SystemException {
		public:
			/// \brief Initializes a new instance of the FormatException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			FormatException(const String& sMessage) : SystemException(sMessage) { }
		};

	}
}
