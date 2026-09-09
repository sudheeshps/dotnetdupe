/// \file ArgumentNullException.h
/// \brief Defines the exception thrown when a null reference is passed to a method that does not accept it.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/ArgumentException.h"

namespace DotNetDupe {
	namespace System {

		/// \class ArgumentNullException
		/// \brief The exception that is thrown when a null reference is passed to a method that does not accept it as a valid argument.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.26 (System.ArgumentNullException).
		class ArgumentNullException : public ArgumentException {
		public:
			/// \brief Initializes a new instance of the ArgumentNullException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			ArgumentNullException(const String& sMessage) : ArgumentException(sMessage) { }
		};

	}
}
