/// \file FileNotFoundException.h
/// \brief The exception that is thrown when an attempt to access a file that does not exist on disk fails.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
	namespace System {
		namespace IO {

			/// \class FileNotFoundException
			/// \brief The exception that is thrown when an attempt to access a file that does not exist on disk fails.
			///
			/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
			class FileNotFoundException : public IOException {
			public:
				/// \brief Initializes a new instance of the FileNotFoundException class with a specified error message.
				/// \param sMessage The error message that explains the reason for the exception.
				FileNotFoundException(const String& sMessage) : IOException(sMessage) { }
			};
		}

	}
}

