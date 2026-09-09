/// \file Uri.h
/// \brief Provides an object representation of a Uniform Resource Identifier (URI) and easy access to its parts.
///
/// Standard Citation: RFC 3986 Uniform Resource Identifier (URI): Generic Syntax.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {

        /// \class Uri
        /// \brief Represents an object representation of a Uniform Resource Identifier (URI) and provides easy access to the parts of the URI.
        ///
        /// Modeled after .NET System.Uri and adhering to the IETF RFC 3986 Uniform Resource Identifier specification.
        /// Parses scheme, authority, host, port, path, query, and fragment parts with percent-encoding and unescaping facilities.
        ///
        /// \note Thread Safety: Uri instances are immutable after construction and thread-safe for concurrent inspection.
        class Uri : public Object {
        public:
            /// \brief Initializes a new instance of the Uri class with the specified URI string.
            /// \param uriString A string representing the URI to parse.
            DOTNETDUPE_API Uri(const String& uriString);

            /// \brief Gets the absolute path of the URI.
            /// \return A string containing the path component of the URI.
            DOTNETDUPE_API String GetAbsolutePath() const;

            /// \brief Gets the absolute URI string representation.
            /// \return The fully qualified URI string.
            DOTNETDUPE_API String GetAbsoluteUri() const;

            /// \brief Gets the authority (host, optional port, and optional user information).
            /// \return The authority string.
            DOTNETDUPE_API String GetAuthority() const;

            /// \brief Gets the host component of this instance.
            /// \return The host name or IP address.
            DOTNETDUPE_API String GetHost() const;

            /// \brief Gets the port number of this URI.
            /// \return An integer containing the port number assigned to this URI.
            DOTNETDUPE_API int GetPort() const;

            /// \brief Gets the scheme name for this URI.
            /// \return The protocol scheme (e.g., "http", "https", "file").
            DOTNETDUPE_API String GetScheme() const;

            /// \brief Gets any query information included in the specified URI.
            /// \return The query string excluding the leading question mark.
            DOTNETDUPE_API String GetQuery() const;

            /// \brief Gets the escaped URI fragment.
            /// \return The fragment portion of the URI excluding the leading '#'.
            DOTNETDUPE_API String GetFragment() const;

            /// \brief Gets the user name, password, or other user-specific information associated with the specified URI.
            /// \return The user information string.
            DOTNETDUPE_API String GetUserInfo() const;

            /// \brief Gets the original URI string passed to the constructor.
            /// \return The raw unmodified input string.
            DOTNETDUPE_API String GetOriginalString() const;

            /// \brief Gets the canonical string representation for the specified Uri instance.
            /// \return The canonical URI string.
            DOTNETDUPE_API String ToString() const;

            /// \brief Determines whether the port value of the URI is the default for this scheme.
            /// \return True if the port is default for the scheme (e.g. 80 for HTTP, 443 for HTTPS); otherwise false.
            DOTNETDUPE_API bool IsDefaultPort() const;

            /// \brief Determines whether the specified Uri is a file URI.
            /// \return True if scheme is "file"; otherwise false.
            DOTNETDUPE_API bool IsFile() const;

            /// \brief Determines whether the specified Uri references the local host.
            /// \return True if host is loopback ("localhost", "127.0.0.1", "::1"); otherwise false.
            DOTNETDUPE_API bool IsLoopback() const;

            /// \brief Converts a string to its escaped representation according to RFC 3986.
            /// \param stringToEscape The string to escape.
            /// \return An escaped string where non-alphanumeric characters are percent-encoded.
            DOTNETDUPE_API static String EscapeDataString(const String& stringToEscape);

            /// \brief Converts a string to its unescaped representation according to RFC 3986.
            /// \param stringToUnescape The percent-encoded string to decode.
            /// \return The decoded string with hexadecimal percent sequences restored to characters.
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
