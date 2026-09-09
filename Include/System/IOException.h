/// \file IOException.h
/// \brief The exception that is thrown when an I/O error occurs.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
	namespace System {
		namespace IO {
			/// \class IOException
			/// \brief The exception that is thrown when an I/O error occurs.
			///
			/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
			class IOException : public SystemException {
			public:
				/// \brief Initializes a new instance of the IOException class with a default message.
				DOTNETDUPE_API IOException();

				/// \brief Initializes a new instance of the IOException class with a specified error message.
				/// \param sMessage The error message that explains the reason for the exception.
				IOException(const String& sMessage) : SystemException(sMessage) { }

				/// \brief Initializes a new instance of the IOException class with a specified error message and inner exception.
				/// \param sMessage The error message that explains the reason for the exception.
				/// \param innerException The exception that is the cause of the current exception.
				IOException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) { }
			};
		}
	}
}

