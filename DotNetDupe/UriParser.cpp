
#include "pch.h"
#include "System/UriParser.h"
#include "System/Uri.h"
#include <map>
#include <utility>

namespace DotNetDupe {
    namespace System {
        std::map<String, std::pair<UriParser*, int>> UriParser::_parsers;

        UriParser::UriParser() {
            /// Default initialization of base URI parser.
        }

        void UriParser::Register(UriParser* uriParser, const String& schemeName, int defaultPort) {
            /// Associate parser instance and default port with given scheme name.
            _parsers [schemeName] = std::make_pair(uriParser, defaultPort);
        }

        bool UriParser::IsKnownScheme(const String& schemeName) {
            /// Check if scheme is registered in parser map.
            return _parsers.find(schemeName) != _parsers.end();
        }

        UriParser* UriParser::GetParser(const String& schemeName) {
            /// Step: Search for registered parser matching scheme name.
            auto it = _parsers.find(schemeName);
            if (it != _parsers.end()) {
                return it->second.first;
            }
            return nullptr;
        }

        String UriParser::GetComponents(const Uri& uri, UriComponents components, UriFormat format) {
            /// Base implementation: return absolute URI string.
            return uri.GetAbsoluteUri();
        }

        GenericUriParser::GenericUriParser() : UriParser() {
            /// Default constructor initializing generic URI parser.
        }
    }
}
