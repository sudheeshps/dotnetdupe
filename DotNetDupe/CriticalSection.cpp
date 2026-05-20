#include "pch.h"
#include "System/Threading/CriticalSection.h"
#include <windows.h>

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            CriticalSection::CriticalSection() {
                CRITICAL_SECTION* cs = new CRITICAL_SECTION();
                // Initialize with a spin count for high performance in user mode.
                // 4000 is a common value used by the Windows heap manager and .NET.
                InitializeCriticalSectionAndSpinCount(cs, 4000);
                _internalSection = cs;
            }

            CriticalSection::~CriticalSection() {
                CRITICAL_SECTION* cs = static_cast<CRITICAL_SECTION*>(_internalSection);
                DeleteCriticalSection(cs);
                delete cs;
            }

            void CriticalSection::Enter() {
                EnterCriticalSection(static_cast<CRITICAL_SECTION*>(_internalSection));
            }

            void CriticalSection::Leave() {
                LeaveCriticalSection(static_cast<CRITICAL_SECTION*>(_internalSection));
            }

            bool CriticalSection::TryEnter() {
                return TryEnterCriticalSection(static_cast<CRITICAL_SECTION*>(_internalSection)) != 0;
            }
        }
    }
}