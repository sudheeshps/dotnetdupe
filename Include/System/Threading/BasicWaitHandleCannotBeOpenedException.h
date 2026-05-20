#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            template <class CharT> class BasicWaitHandleCannotBeOpenedException : public BasicSystemException<CharT> {
            public:
                BasicWaitHandleCannotBeOpenedException(const CharT* pchMessage);
            };
            template <class CharT>
            inline BasicWaitHandleCannotBeOpenedException<CharT>::BasicWaitHandleCannotBeOpenedException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
        }
    }
}
