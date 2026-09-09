/// \file HttpRequestException.h
/// \brief Exception thrown when an HTTP request or connection failure occurs.

#pragma once
#include "System/Exception.h"

namespace DotNetDupe {
	namespace System {
		namespace Net {
			namespace Http {
				/// \class HttpRequestException
				/// \brief A base exception class thrown by HttpClient and HttpContent classes when an error occurs during HTTP processing.
				///
				/// Inherits from DotNetDupe::System::Exception to ensure custom exception compliance.
				class HttpRequestException : public Exception {
				public:
					/// \brief Initializes a new instance of the HttpRequestException class with a specified error message.
					/// \param sMessage The error message that explains the reason for the exception.
					HttpRequestException(const String& sMessage) : Exception(sMessage) { }
				};
			}
		}
	}
}
