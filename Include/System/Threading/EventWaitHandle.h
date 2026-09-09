/// \file EventWaitHandle.h
/// \brief Represents a thread synchronization event supporting automatic and manual reset modes.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"
#include "System/SmartPointer.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            /// \class EventWaitHandle
            /// \brief Represents a thread synchronization event.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.63 specifications.
            /// Supports both automatic and manual reset modes, named system events for IPC,
            /// and cross-platform condition variable signaling.
            class EventWaitHandle : public WaitHandle {
            public:
                /// \brief Initializes a new instance of the EventWaitHandle class, specifying whether the wait handle is initially signaled, and whether it resets automatically or manually.
                /// \param initialState true to set the initial state to signaled; false to set it to nonsignaled.
                /// \param manualReset true for manual-reset mode; false for auto-reset mode.
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset);

                /// \brief Initializes a new instance of the EventWaitHandle class with a name.
                /// \param sName The name of the system event.
                /// \param initialState true to set initial state to signaled; false for nonsignaled.
                /// \param manualReset true for manual-reset mode; false for auto-reset mode.
                /// \param openAlways true to open existing or create new.
                DOTNETDUPE_API EventWaitHandle(const String& sName, bool initialState = false, bool manualReset = false, bool openAlways = true);

                /// \brief Initializes a new instance of the EventWaitHandle class with parameters.
                /// \param initialState true to set initial state to signaled.
                /// \param manualReset true for manual-reset mode.
                /// \param sName The name of the system event.
                /// \param openAlways true to open existing or create new.
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways = true);

                /// \brief Initializes a new instance of the EventWaitHandle class with creation status output.
                /// \param initialState true to set initial state to signaled.
                /// \param manualReset true for manual-reset mode.
                /// \param sName The name of the system event.
                /// \param openAlways true to open existing or create new.
                /// \param bCreatedNew When this method returns, contains true if a local or system event was created; false if it already existed.
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways, bool& bCreatedNew);

                /// \brief Destructor closing event handle.
                DOTNETDUPE_API ~EventWaitHandle() override;

                /// \brief Sets the state of the event to signaled, allowing one or more waiting threads to proceed.
                /// \return true if the function succeeds; otherwise, false.
                DOTNETDUPE_API bool Set();

                /// \brief Sets the state of the event to nonsignaled, causing threads to block.
                /// \return true if the function succeeds; otherwise, false.
                DOTNETDUPE_API bool Reset();

                /// \brief Blocks the current thread until the current WaitHandle receives a signal.
                /// \return true if the current instance receives a signal; otherwise, false.
                DOTNETDUPE_API bool WaitOne() override;

                /// \brief Blocks the current thread until the current instance receives a signal, using a 32-bit signed integer to measure the time interval.
                /// \param millisecondsTimeout The number of milliseconds to wait, or Timeout::Infinite (-1) to wait indefinitely.
                /// \return true if the current instance receives a signal; otherwise, false.
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                /// \brief Opens an existing named system event wait handle.
                /// \param sName The name of the system event to open.
                /// \return An object that represents the named system event.
                /// \throws WaitHandleCannotBeOpenedException If the named event does not exist.
                DOTNETDUPE_API static SmartPointer<EventWaitHandle> OpenExisting(const String& sName);

                /// \brief Opens an existing named system event wait handle, and returns a value that indicates whether the operation succeeded.
                /// \param sName The name of the system event to open.
                /// \param pResult When this method returns, contains an EventWaitHandle object if the call succeeded, or null if it failed.
                /// \return true if the named event was opened successfully; otherwise, false.
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, SmartPointer<EventWaitHandle>& pResult);

                struct Impl;
            protected:
                bool _state;
                bool _manualReset;
                String _name;
                void* _hHandle;
                Impl* _pImpl;
            };
        }
    }
}