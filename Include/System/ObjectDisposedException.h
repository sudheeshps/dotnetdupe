#pragma once
#include "System/InvalidOperationException.h"

namespace DotNetDupe {
	namespace System {

		/// \class ObjectDisposedException
		/// \brief The exception that is thrown when an operation is performed on a disposed object.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.34 (System.ObjectDisposedException).
		class ObjectDisposedException : public InvalidOperationException {
		public:
			/// \brief Initializes a new instance of the ObjectDisposedException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			ObjectDisposedException(const String& sMessage) : InvalidOperationException(sMessage) { }
		};

	}
}
