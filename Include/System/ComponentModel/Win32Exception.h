#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        namespace ComponentModel {

            class Win32Exception : public SystemException {
            private:
                int m_nNativeErrorCode;

            public:
                DOTNETDUPE_API Win32Exception();
                DOTNETDUPE_API explicit Win32Exception(int nNativeErrorCode);
                DOTNETDUPE_API Win32Exception(int nNativeErrorCode, const String& sMessage);
                DOTNETDUPE_API Win32Exception(const String& sMessage);

                DOTNETDUPE_API int GetNativeErrorCode() const { return m_nNativeErrorCode; }
            };

        }
    }
}
