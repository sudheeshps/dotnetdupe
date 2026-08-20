#pragma once
#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {

        class EventArgs : public virtual Object {
        public:
            DOTNETDUPE_API EventArgs();
            DOTNETDUPE_API ~EventArgs() override = default;

            DOTNETDUPE_API static const EventArgs& Empty();
        };

    }
}
