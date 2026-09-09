#include "pch.h"
#include "System/EventArgs.h"

namespace DotNetDupe {
    namespace System {

        EventArgs::EventArgs() {
            /// Initialize empty event arguments payload.
        }

        const EventArgs& EventArgs::Empty() {
            /// Return static immutable empty event arguments singleton.
            static const EventArgs s_emptyArgs;
            return s_emptyArgs;
        }

    }
}
