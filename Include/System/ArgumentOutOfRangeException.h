#pragma once
#include "System/ArgumentException.h"

namespace DotNetDupe {
	namespace System {

		/// \class ArgumentOutOfRangeException
		/// \brief The exception that is thrown when the value of an argument is outside the allowable range of values as defined by the invoked method.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.27 (System.ArgumentOutOfRangeException).
		class ArgumentOutOfRangeException : public ArgumentException {
		public:
			/// \brief Initializes a new instance of the ArgumentOutOfRangeException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			ArgumentOutOfRangeException(const String& sMessage) : ArgumentException(sMessage) { }
		};

	}
}
