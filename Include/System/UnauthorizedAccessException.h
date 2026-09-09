/// \file UnauthorizedAccessException.h
/// \brief The exception that is thrown when the operating system denies access because of an I/O error or a specific type of security error.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
	namespace System {

		/// \class UnauthorizedAccessException
		/// \brief The exception that is thrown when the operating system denies access because of an I/O or security error.
		///
		/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
		class UnauthorizedAccessException : public SystemException {
		public:
			/// \brief Initializes a new instance of the UnauthorizedAccessException class with a default message.
			DOTNETDUPE_API UnauthorizedAccessException();

			/// \brief Initializes a new instance of the UnauthorizedAccessException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			DOTNETDUPE_API UnauthorizedAccessException(const String& sMessage) : SystemException(sMessage) { }

			/// \brief Initializes a new instance of the UnauthorizedAccessException class with a specified error message and inner exception.
			/// \param sMessage The error message that explains the reason for the exception.
			/// \param innerException The exception that is the cause of the current exception.
			DOTNETDUPE_API UnauthorizedAccessException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) { }
		};

	}
}

