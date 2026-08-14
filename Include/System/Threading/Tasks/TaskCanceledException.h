#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
	namespace System {
		namespace Threading {
			namespace Tasks {

				class TaskCanceledException : public SystemException {
				public:
					TaskCanceledException(const String& sMessage) : SystemException(sMessage) { }
				};
			}
		}

	}
}
