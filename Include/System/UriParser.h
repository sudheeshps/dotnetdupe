
#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/UriEnums.h"
#include <map>

namespace DotNetDupe {
    namespace System {
        class Uri;

        class UriParser : public Object {
        public:
            DOTNETDUPE_API static void Register(UriParser* uriParser, const String& schemeName, int defaultPort);
            DOTNETDUPE_API static bool IsKnownScheme(const String& schemeName);
            DOTNETDUPE_API static UriParser* GetParser(const String& schemeName);

            DOTNETDUPE_API UriParser();
        public:
            DOTNETDUPE_API virtual String GetComponents(const Uri& uri, UriComponents components, UriFormat format);

        private:
            static std::map<String, std::pair<UriParser*, int>> _parsers;
        };

        class GenericUriParser : public UriParser {
        public:
            DOTNETDUPE_API GenericUriParser();
        };
    }
}
