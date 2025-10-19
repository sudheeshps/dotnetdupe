
#pragma once

namespace DotNetDupe {
    namespace System {
        enum class UriComponents {
            Scheme = 1,
            UserInfo = 2,
            Host = 4,
            Port = 8,
            Path = 16,
            Query = 32,
            Fragment = 64,
            StrongPort = 128,
            NormalizedHost = 256,
            KeepDelimiter = 1073741824,
            AbsoluteUri = 32767,
            PathAndQuery = 48,
            SchemeAndServer = 13,
            HttpRequestUrl = 61,
            UriPath = 50,
            HostAndPort = 12,
            UserInfoAndHostAndPort = 15,
            SafeUnescaped = 8191
        };

        enum class UriFormat {
            UriEscaped = 1,
            Unescaped = 2,
            SafeUnescaped = 3
        };
    }
}
