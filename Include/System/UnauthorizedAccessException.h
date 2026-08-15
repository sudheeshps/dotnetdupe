#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
	namespace System {

		class UnauthorizedAccessException : public SystemException {
		public:
			DOTNETDUPE_API UnauthorizedAccessException();
			DOTNETDUPE_API UnauthorizedAccessException(const String& sMessage) : SystemException(sMessage) { }
			DOTNETDUPE_API UnauthorizedAccessException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) { }
		};

	}
}
