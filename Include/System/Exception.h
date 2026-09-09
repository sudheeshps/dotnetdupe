/// \file Exception.h
/// \brief Represents errors that occur during application execution and serves as the ultimate base exception.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "Common.h"
#include <stdexcept>

namespace DotNetDupe {
	namespace System {
		class String;

		/// \class Exception
		/// \brief Represents errors that occur during application execution.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.23 (System.Exception).
		///       Serves as the ultimate base class for all exceptions throughout DotNetDupe.
		class Exception : public std::runtime_error {
		private:
			Exception* m_pInnerException = nullptr;
		public:
			/// \brief Initializes a new instance of the Exception class with a default system-supplied message.
			DOTNETDUPE_API Exception();

			/// \brief Initializes a new instance of the Exception class with a specified error message.
			/// \param sMessage The message that describes the error.
			DOTNETDUPE_API Exception(const String& sMessage);

			/// \brief Initializes a new instance of the Exception class with a specified error message and a reference to the inner exception that is the cause of this exception.
			/// \param sMessage The error message that explains the reason for the exception.
			/// \param innerException The exception that is the cause of the current exception.
			DOTNETDUPE_API Exception(const String& sMessage, const Exception& innerException);

			/// \brief Copy constructor. Deep copies inner exception if present.
			DOTNETDUPE_API Exception(const Exception& other);

			/// \brief Copy assignment operator.
			DOTNETDUPE_API Exception& operator=(const Exception& other);

			/// \brief Virtual destructor.
			DOTNETDUPE_API virtual ~Exception();

			/// \brief Gets a message that describes the current exception.
			/// \return The error message string.
			const char* What() const { return what(); }
		};
	}
}
