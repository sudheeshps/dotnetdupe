#pragma once
#include "System/Threading/WaitHandle.h"
#include "System/String.h"
#include <mutex>
#include <condition_variable>
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class EventWaitHandle : public WaitHandle {
            public:
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset);
                DOTNETDUPE_API EventWaitHandle(const String& sName, bool initialState = false, bool manualReset = false, bool openAlways = true);
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways = true);
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways, bool& bCreatedNew);
                DOTNETDUPE_API virtual ~EventWaitHandle();
                DOTNETDUPE_API bool Set();
                DOTNETDUPE_API bool Reset();
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                DOTNETDUPE_API static EventWaitHandle* OpenExisting(const String& sName);
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, EventWaitHandle*& pResult);
            protected:
                bool _state;
                bool _manualReset;
                String _name;
                void* _hHandle;
                std::mutex _mutex;
                std::condition_variable _cv;
            };
        }
    }
}