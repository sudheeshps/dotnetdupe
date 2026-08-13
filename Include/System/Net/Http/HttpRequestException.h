#pragma once
#include "System/Exception.h"

namespace DotNetDupe {
	namespace System {
		namespace Net {
			namespace Http {
				class HttpRequestException : public Exception {
				public:
					HttpRequestException(const String& sMessage) : Exception(sMessage) { }
				};
			}
		}

	}
}
