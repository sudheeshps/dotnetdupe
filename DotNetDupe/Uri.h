#pragma once

#include "Common.h"
#include "Object.h"
#include "String.h"
#include "UriEnums.h"

namespace DotNetDupe
{
    namespace System
    {
        class Uri : public Object
        {
        public:
            DOTNETDUPE_API Uri(const String& uriString);

            DOTNETDUPE_API String GetAbsolutePath() const;
            DOTNETDUPE_API String GetAbsoluteUri() const;
            DOTNETDUPE_API String GetAuthority() const;
            DOTNETDUPE_API String GetHost() const;
            DOTNETDUPE_API int GetPort() const;
            DOTNETDUPE_API String GetScheme() const;
            DOTNETDUPE_API String GetQuery() const;
            DOTNETDUPE_API String GetFragment() const;

        private:
            String _uriString;
        };
    }
}
