#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {

		/// \class ArgumentException
		/// \brief The exception that is thrown when one of the arguments provided to a method is not valid.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.25 (System.ArgumentException).
		class ArgumentException : public SystemException {
		public:
			/// \brief Initializes a new instance of the ArgumentException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			ArgumentException(const String& sMessage) : SystemException(sMessage) { }
		};

	}
}
