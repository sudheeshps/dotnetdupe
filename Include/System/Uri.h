#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/UriEnums.h"

namespace DotNetDupe {
    namespace System {
        class Uri : public Object {
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
            void ParseUri();

            String _uriString;
            String _scheme;
            String _authority;
            String _absolutePath;
            String _query;
            String _fragment;
        };
    }
}
