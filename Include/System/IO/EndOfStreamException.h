/// \file EndOfStreamException.h
/// \brief The exception that is thrown when reading is attempted past the end of a stream.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
	namespace System {
		namespace IO {

			/// \class EndOfStreamException
			/// \brief The exception that is thrown when reading is attempted past the end of a stream.
			///
			/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
			class EndOfStreamException : public IOException {
			public:
				/// \brief Initializes a new instance of the EndOfStreamException class with a specified error message.
				/// \param sMessage The error message that explains the reason for the exception.
				EndOfStreamException(const String& sMessage) : IOException(sMessage) { }
			};
		}

	}
}

