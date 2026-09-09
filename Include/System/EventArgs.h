/// \file EventArgs.h
/// \brief Represents the base class for classes that contain event data.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {

        /// \class EventArgs
        /// \brief Represents the base class for classes that contain event data, and provides a value to use for events that do not include event data.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.21 (System.EventArgs).
        class EventArgs : public virtual Object {
        public:
            /// \brief Initializes a new instance of the EventArgs class.
            DOTNETDUPE_API EventArgs();

            /// \brief Virtual destructor for polymorphic event payload cleanup.
            DOTNETDUPE_API ~EventArgs() override = default;

            /// \brief Provides a value to use with events that do not have event data.
            /// \return A reference to an empty EventArgs singleton instance.
            DOTNETDUPE_API static const EventArgs& Empty();
        };

    }
}
