/// \file TaskStatus.h
/// \brief Represents the current stage in the lifecycle of an asynchronous Task.

#pragma once

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            namespace Tasks {
                
                /// \brief Represents the current stage in the lifecycle of a Task.
                enum class TaskStatus {
                    Created,
                    WaitingToRun,
                    Running,
                    RanToCompletion,
                    Canceled,
                    Faulted
                };

            }
        }
    }
}
