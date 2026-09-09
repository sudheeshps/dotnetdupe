#pragma once
#include "System/Threading/EventWaitHandle.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            /// \class ManualResetEvent
            /// \brief Notifies one or more waiting threads that an event has occurred.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.65 specifications.
            /// When signaled, remains signaled until manually reset via Reset(). Allows multiple
            /// waiting threads to release concurrently upon signal.
            class ManualResetEvent : public EventWaitHandle {
            public:
                /// \brief Initializes a new instance of the ManualResetEvent class with a Boolean value indicating whether to set the initial state to signaled.
                /// \param initialState true to set the initial state to signaled; false to set the initial state to nonsignaled.
                DOTNETDUPE_API ManualResetEvent(bool initialState);

                /// \brief Initializes a new instance of the ManualResetEvent class with a name.
                /// \param sName The name of the system event.
                /// \param initialState true to set initial state to signaled; false for nonsignaled.
                /// \param openAlways true to open existing or create new.
                DOTNETDUPE_API ManualResetEvent(const String& sName, bool initialState = false, bool openAlways = true);

                /// \brief Initializes a new instance of the ManualResetEvent class with initial state and name.
                /// \param initialState true to set initial state to signaled; false for nonsignaled.
                /// \param sName The name of the system event.
                /// \param openAlways true to open existing or create new.
                DOTNETDUPE_API ManualResetEvent(bool initialState, const String& sName, bool openAlways = true);

                /// \brief Initializes a new instance of the ManualResetEvent class with creation status output.
                /// \param initialState true to set initial state to signaled; false for nonsignaled.
                /// \param sName The name of the system event.
                /// \param openAlways true to open existing or create new.
                /// \param bCreatedNew When this method returns, contains true if a local or system event was created; false if it already existed.
                DOTNETDUPE_API ManualResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew);

                /// \brief Opens an existing named system manual-reset event.
                /// \param sName The name of the system event to open.
                /// \return An object that represents the named system event.
                /// \throws WaitHandleCannotBeOpenedException If the named event does not exist.
                DOTNETDUPE_API static SmartPointer<ManualResetEvent> OpenExisting(const String& sName);

                /// \brief Opens an existing named system manual-reset event, and returns a value that indicates whether the operation succeeded.
                /// \param sName The name of the system event to open.
                /// \param pResult When this method returns, contains a ManualResetEvent object if the call succeeded, or null if it failed.
                /// \return true if the named event was opened successfully; otherwise, false.
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, SmartPointer<ManualResetEvent>& pResult);
            };
        }
    }
}