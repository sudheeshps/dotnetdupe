#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

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
            DOTNETDUPE_API String GetUserInfo() const;
            DOTNETDUPE_API String GetOriginalString() const;
            
            DOTNETDUPE_API String ToString() const;

            DOTNETDUPE_API bool IsDefaultPort() const;
            DOTNETDUPE_API bool IsFile() const;
            DOTNETDUPE_API bool IsLoopback() const;

            DOTNETDUPE_API static String EscapeDataString(const String& stringToEscape);
            DOTNETDUPE_API static String UnescapeDataString(const String& stringToUnescape);

        private:
            void ParseUri();

            String _uriString;
            String _scheme;
            String _authority;
            String _host;
            int _port;
            String _path;
            String _query;
            String _fragment;
            String _userInfo;
        };
    }
}
