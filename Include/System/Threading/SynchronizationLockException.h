#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {
		namespace Threading {

			class SynchronizationLockException : public SystemException {
			public:
				SynchronizationLockException(const String& sMessage) : SystemException(sMessage) { }
			};
		}

	}
}
