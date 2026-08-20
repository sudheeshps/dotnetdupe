#include "pch.h"
#include "System/EventArgs.h"

namespace DotNetDupe {
    namespace System {

        EventArgs::EventArgs() {
        }

        const EventArgs& EventArgs::Empty() {
            static const EventArgs s_emptyArgs;
            return s_emptyArgs;
        }

    }
}
