#pragma once

namespace DotNetDupe {
    namespace System {

        /// \enum UriComponents
        /// \brief Specifies the parts of a Uniform Resource Identifier (URI) to return.
        ///
        /// Modeled after .NET System.UriComponents and compliant with RFC 3986.
        /// Allows bitwise composition of URI subcomponents for parsing and reconstruction.
        enum class UriComponents {
            Scheme = 1,                     ///< The scheme component (e.g., "http", "https", "ftp").
            UserInfo = 2,                   ///< User name and password authorization data.
            Host = 4,                       ///< Domain name or IP address.
            Port = 8,                       ///< Network port number.
            Path = 16,                      ///< Resource path component.
            Query = 32,                     ///< Query string starting after the question mark.
            Fragment = 64,                  ///< Fragment identifier starting after the hash character.
            StrongPort = 128,               ///< The port number, including explicit default port numbers.
            NormalizedHost = 256,           ///< Canonicalized host name.
            KeepDelimiter = 1073741824,     ///< Preserves component delimiter character.
            AbsoluteUri = 32767,            ///< Complete canonical URI string.
            PathAndQuery = 48,              ///< Path and query components combined.
            SchemeAndServer = 13,           ///< Scheme, host, and port combined.
            HttpRequestUrl = 61,            ///< Full HTTP URL without user credentials or fragment.
            UriPath = 50,                   ///< Scheme, authority, and path combined.
            HostAndPort = 12,               ///< Host and port combined.
            UserInfoAndHostAndPort = 15,    ///< UserInfo, host, and port combined.
            SafeUnescaped = 8191            ///< Safe unescaped characters preserved.
        };

        /// \enum UriFormat
        /// \brief Controls character escaping and normalization during URI component extraction.
        ///
        /// Modeled after .NET System.UriFormat.
        enum class UriFormat {
            UriEscaped = 1,     ///< Characters are percent-escaped according to RFC 3986 rules.
            Unescaped = 2,      ///< Characters are unescaped to their literal character values.
            SafeUnescaped = 3   ///< Reserved characters remain escaped, while unreserved are unescaped.
        };

    }
}
