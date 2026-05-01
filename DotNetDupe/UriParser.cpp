
#include "pch.h"
#include "System/UriParser.h"
#include "System/Uri.h"

namespace DotNetDupe {
    namespace System {
        std::map<String, std::pair<UriParser*, int>> UriParser::_parsers;

        UriParser::UriParser() { }

        void UriParser::Register(UriParser* uriParser, const String& schemeName, int defaultPort) {
            _parsers [schemeName] = std::make_pair(uriParser, defaultPort);
        }

        bool UriParser::IsKnownScheme(const String& schemeName) {
            return _parsers.find(schemeName) != _parsers.end();
        }

        UriParser* UriParser::GetParser(const String& schemeName) {
            auto it = _parsers.find(schemeName);
            if (it != _parsers.end()) {
                return it->second.first;
            }
            return nullptr;
        }

        String UriParser::GetComponents(const Uri& uri, UriComponents components, UriFormat format) {
            // This is a base implementation that can be overridden.
            // It's not fully featured, but provides a starting point.
            return uri.GetAbsoluteUri();
        }

        GenericUriParser::GenericUriParser() : UriParser() { }
    }
}
