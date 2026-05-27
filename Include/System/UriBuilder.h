#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Uri.h"

namespace DotNetDupe {
    namespace System {
        class UriBuilder : public Object {
        public:
            DOTNETDUPE_API UriBuilder();
            DOTNETDUPE_API UriBuilder(const String& uri);
            DOTNETDUPE_API UriBuilder(const Uri& uri);
            DOTNETDUPE_API UriBuilder(const String& scheme, const String& host);
            DOTNETDUPE_API UriBuilder(const String& scheme, const String& host, int port);

            DOTNETDUPE_API String GetScheme() const;
            DOTNETDUPE_API void SetScheme(const String& value);

            DOTNETDUPE_API String GetHost() const;
            DOTNETDUPE_API void SetHost(const String& value);

            DOTNETDUPE_API int GetPort() const;
            DOTNETDUPE_API void SetPort(int value);

            DOTNETDUPE_API String GetPath() const;
            DOTNETDUPE_API void SetPath(const String& value);

            DOTNETDUPE_API String GetQuery() const;
            DOTNETDUPE_API void SetQuery(const String& value);

            DOTNETDUPE_API String GetFragment() const;
            DOTNETDUPE_API void SetFragment(const String& value);

            DOTNETDUPE_API String GetUserName() const;
            DOTNETDUPE_API void SetUserName(const String& value);

            DOTNETDUPE_API String GetPassword() const;
            DOTNETDUPE_API void SetPassword(const String& value);

            DOTNETDUPE_API Uri GetUri();
            DOTNETDUPE_API String ToString();

        private:
            String _uri;
            String _scheme;
            String _host;
            int _port;
            String _path;
            String _query;
            String _fragment;
            String _userName;
            String _password;
        };
    }
}
