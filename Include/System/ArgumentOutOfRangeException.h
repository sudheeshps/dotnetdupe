#pragma once
#include "System/ArgumentException.h"

namespace DotNetDupe {
	namespace System {

		class ArgumentOutOfRangeException : public ArgumentException {
		public:
			ArgumentOutOfRangeException(const String& sMessage) : ArgumentException(sMessage) { }
		};

	}
}
