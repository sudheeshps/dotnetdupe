#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {
		namespace Threading {
			class AbandonedMutexException : public SystemException {
			public:
				AbandonedMutexException(const String& sMessage) : SystemException(sMessage) { }
			};
		}
	}
}
