/// \file UriParser.h
/// \brief Parses a new URI scheme.
///
/// Standard Citation: RFC 3986 Uniform Resource Identifier (URI): Generic Syntax.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/UriEnums.h"
#include <map>

namespace DotNetDupe {
    namespace System {

        class Uri;

        /// \class UriParser
        /// \brief Parses a new URI scheme and provides scheme-specific component extraction.
        ///
        /// Modeled after .NET System.UriParser. Manages registered scheme parsers and handles
        /// custom parsing behaviors for standard and application-defined protocol schemes.
        class UriParser : public Object {
        public:
            /// \brief Registers a custom parser and default port for a specific URI scheme.
            /// \param uriParser Pointer to the parser instance to register.
            /// \param schemeName The protocol scheme name (e.g. "http", "custom").
            /// \param defaultPort The default TCP/UDP port associated with the scheme.
            DOTNETDUPE_API static void Register(UriParser* uriParser, const String& schemeName, int defaultPort);

            /// \brief Indicates whether the parser for a scheme is registered.
            /// \param schemeName The scheme name to check.
            /// \return True if scheme is registered; otherwise false.
            DOTNETDUPE_API static bool IsKnownScheme(const String& schemeName);

            /// \brief Retrieves the registered parser associated with a scheme.
            /// \param schemeName The scheme name to look up.
            /// \return Pointer to the registered UriParser, or nullptr if not found.
            DOTNETDUPE_API static UriParser* GetParser(const String& schemeName);

            /// \brief Constructs a default instance of UriParser.
            DOTNETDUPE_API UriParser();

            /// \brief Gets the components from a URI formatted as specified.
            /// \param uri The source Uri to extract components from.
            /// \param components Bitwise combination of UriComponents to extract.
            /// \param format The escaping format specification.
            /// \return Extracted component string.
            DOTNETDUPE_API virtual String GetComponents(const Uri& uri, UriComponents components, UriFormat format);

        private:
            static std::map<String, std::pair<UriParser*, int>> _parsers;
        };

        /// \class GenericUriParser
        /// \brief A customizable parser for a hierarchical URI.
        class GenericUriParser : public UriParser {
        public:
            /// \brief Constructs a new GenericUriParser instance.
            DOTNETDUPE_API GenericUriParser();
        };

    }
}
