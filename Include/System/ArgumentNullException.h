#pragma once
#include "System/ArgumentException.h"

namespace DotNetDupe {
	namespace System {

		class ArgumentNullException : public ArgumentException {
		public:
			ArgumentNullException(const String& sMessage) : ArgumentException(sMessage) { }
		};

	}
}
