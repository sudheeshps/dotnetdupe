#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {
		namespace Threading {

			class ThreadStateException : public SystemException {
			public:
				ThreadStateException(const String& sMessage) : SystemException(sMessage) { }
			};
		}

	}
}
