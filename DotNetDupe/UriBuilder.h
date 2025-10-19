#pragma once

#include "Common.h"
#include "Object.h"
#include "String.h"
#include "Uri.h"

namespace DotNetDupe {
    namespace System {
        class UriBuilder : public Object {
        public:
            DOTNETDUPE_API UriBuilder();
            DOTNETDUPE_API UriBuilder(const String& uri);
            DOTNETDUPE_API UriBuilder(const Uri& uri);

            DOTNETDUPE_API String GetScheme() const;
            DOTNETDUPE_API void SetScheme(const String& scheme);

            DOTNETDUPE_API String GetHost() const;
            DOTNETDUPE_API void SetHost(const String& host);

            DOTNETDUPE_API int GetPort() const;
            DOTNETDUPE_API void SetPortValue(int port);

            DOTNETDUPE_API String GetPath() const;
            DOTNETDUPE_API void SetPath(const String& path);

            DOTNETDUPE_API String GetQuery() const;
            DOTNETDUPE_API void SetQuery(const String& query);

            DOTNETDUPE_API String GetFragment() const;
            DOTNETDUPE_API void SetFragment(const String& fragment);

            DOTNETDUPE_API String GetUserName() const;
            DOTNETDUPE_API void SetUserName(const String& userName);

            DOTNETDUPE_API String GetPassword() const;
            DOTNETDUPE_API void SetPassword(const String& password);

            DOTNETDUPE_API Uri GetUri() const;
            DOTNETDUPE_API String ToString() const;

        private:
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
