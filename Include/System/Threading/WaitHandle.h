#pragma once
#include "Common.h"
#include "System/Object.h"
namespace DotNetDupe {
	namespace System {
		namespace Threading {
			class WaitHandle : public Object {
			public:
				DOTNETDUPE_API virtual ~WaitHandle() { }
				DOTNETDUPE_API virtual bool WaitOne() = 0;
				DOTNETDUPE_API virtual bool WaitOne(int millisecondsTimeout) = 0;
			};

			class LockWaitHandle : public WaitHandle {
			public:
				DOTNETDUPE_API virtual ~LockWaitHandle() { }
				DOTNETDUPE_API virtual int Release(int releaseCount = 1) = 0;
			};
		}
	}
}