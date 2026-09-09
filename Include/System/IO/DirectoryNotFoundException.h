/// \file DirectoryNotFoundException.h
/// \brief The exception that is thrown when part of a file or directory cannot be found.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
	namespace System {
		namespace IO {

			/// \class DirectoryNotFoundException
			/// \brief The exception that is thrown when part of a file or directory cannot be found.
			///
			/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
			class DirectoryNotFoundException : public IOException {
			public:
				/// \brief Initializes a new instance of the DirectoryNotFoundException class with a specified error message.
				/// \param sMessage The error message that explains the reason for the exception.
				DirectoryNotFoundException(const String& sMessage) : IOException(sMessage) { }
			};
		}

	}
}

