#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {
		namespace Threading {

			class WaitHandleCannotBeOpenedException : public SystemException {
			public:
				DOTNETDUPE_API WaitHandleCannotBeOpenedException(const String& sMessage) : SystemException(sMessage) { }
			};
		}

	}
}
