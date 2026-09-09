/// \file ArithmeticException.h
/// \brief The exception that is thrown for errors in an arithmetic, casting, or conversion operation.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {
		/// \class ArithmeticException
		/// \brief The exception that is thrown for errors in an arithmetic, casting, or conversion operation.
		///
		/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
		class ArithmeticException : public SystemException {
		public:
			/// \brief Initializes a new instance of the ArithmeticException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			ArithmeticException(const String& sMessage) : SystemException(sMessage) { }
		};
	}
}

