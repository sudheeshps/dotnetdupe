/// \file AggregateException.h
/// \brief Represents one or more errors that occur during application execution.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/Exception.h"

namespace DotNetDupe {
	namespace System {

		/// \class AggregateException
		/// \brief Represents one or more errors that occur during application execution.
		///
		/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
		class AggregateException : public Exception {
		public:
			/// \brief Initializes a new instance of the AggregateException class with a specified error message.
			/// \param sMessage The error message that explains the reason for the exception.
			AggregateException(const String& sMessage) : Exception(sMessage) { }
		};

	}
}

