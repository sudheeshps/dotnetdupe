#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Uri.h"

#ifdef SetPort
#undef SetPort
#endif

namespace DotNetDupe {
    namespace System {

        /// \class UriBuilder
        /// \brief Provides a custom constructor for uniform resource identifiers (URIs) and modifies URIs for the Uri class.
        ///
        /// Modeled after .NET System.UriBuilder. Simplifies programmatic assembly, modification,
        /// and formatting of individual URI segments (scheme, host, port, path, query, fragment, credentials).
        class UriBuilder : public Object {
        public:
            /// \brief Initializes a new instance of the UriBuilder class.
            DOTNETDUPE_API UriBuilder();

            /// \brief Initializes a new instance of the UriBuilder class with the specified URI.
            /// \param uri A URI string to parse into components.
            DOTNETDUPE_API UriBuilder(const String& uri);

            /// \brief Initializes a new instance of the UriBuilder class with the specified Uri instance.
            /// \param uri An existing Uri object to clone into mutable builder state.
            DOTNETDUPE_API UriBuilder(const Uri& uri);

            /// \brief Initializes a new instance with the specified scheme and host.
            /// \param scheme The protocol scheme.
            /// \param host The DNS host name or IP address.
            DOTNETDUPE_API UriBuilder(const String& scheme, const String& host);

            /// \brief Initializes a new instance with the specified scheme, host, and port number.
            /// \param scheme The protocol scheme.
            /// \param host The DNS host name or IP address.
            /// \param port The port number for the service.
            DOTNETDUPE_API UriBuilder(const String& scheme, const String& host, int port);

            /// \brief Gets the scheme name of the URI.
            /// \return The scheme string.
            DOTNETDUPE_API String GetScheme() const;

            /// \brief Sets the scheme name of the URI.
            /// \param value The scheme name string.
            DOTNETDUPE_API void SetScheme(const String& value);

            /// \brief Gets the Domain Name System (DNS) host name or IP address of a server.
            /// \return The host name string.
            DOTNETDUPE_API String GetHost() const;

            /// \brief Sets the Domain Name System (DNS) host name or IP address of a server.
            /// \param value The host name string.
            DOTNETDUPE_API void SetHost(const String& value);

            /// \brief Gets the port number of the URI.
            /// \return The port number, or -1 if not specified.
            DOTNETDUPE_API int GetPort() const;

            /// \brief Sets the port number of the URI.
            /// \param value The port number to configure.
            DOTNETDUPE_API void SetPort(int value);

            /// \brief Gets the path to the resource referenced by the URI.
            /// \return The path string.
            DOTNETDUPE_API String GetPath() const;

            /// \brief Sets the path to the resource referenced by the URI.
            /// \param value The resource path.
            DOTNETDUPE_API void SetPath(const String& value);

            /// \brief Gets any query information included in the URI.
            /// \return The query string.
            DOTNETDUPE_API String GetQuery() const;

            /// \brief Sets any query information included in the URI.
            /// \param value The query string to configure.
            DOTNETDUPE_API void SetQuery(const String& value);

            /// \brief Gets the fragment portion of the URI.
            /// \return The fragment string.
            DOTNETDUPE_API String GetFragment() const;

            /// \brief Sets the fragment portion of the URI.
            /// \param value The fragment string.
            DOTNETDUPE_API void SetFragment(const String& value);

            /// \brief Gets the user name associated with the user that accesses the URI.
            /// \return The username credential.
            DOTNETDUPE_API String GetUserName() const;

            /// \brief Sets the user name associated with the user that accesses the URI.
            /// \param value The username credential string.
            DOTNETDUPE_API void SetUserName(const String& value);

            /// \brief Gets the password associated with the user that accesses the URI.
            /// \return The password credential.
            DOTNETDUPE_API String GetPassword() const;

            /// \brief Sets the password associated with the user that accesses the URI.
            /// \param value The password credential string.
            DOTNETDUPE_API void SetPassword(const String& value);

            /// \brief Gets the Uri instance constructed by the specified UriBuilder instance.
            /// \return A new Uri instance matching the configured components.
            DOTNETDUPE_API Uri GetUri();

            /// \brief Formats the configured components into a canonical URI string.
            /// \return The assembled URI string.
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
