
#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/UriParser.h"
#include "../DotNetDupe/String.h"
#include "../DotNetDupe/Uri.h"

using namespace DotNetDupe::System;

namespace SystemTests
{
    namespace UriParserTests
    {
        class CustomParser : public UriParser
        {
        public:
            String GetComponents(const Uri& uri, UriComponents components, UriFormat format) override
            {
                if (components == UriComponents::Host)
                {
                    return _T("custom.host");
                }
                return UriParser::GetComponents(uri, components, format);
            }
        };

        TEST(UriParserTest, RegisterAndIsKnownScheme_Should_Work_Correctly)
        {
            // Given
            const String schemeName = _T("custom");
            CustomParser parser;

            // When
            UriParser::Register(&parser, schemeName, 1234);

            // Then
            EXPECT_TRUE(UriParser::IsKnownScheme(schemeName));
            EXPECT_FALSE(UriParser::IsKnownScheme(_T("unknown")));
        }

        TEST(UriParserTest, CustomParser_Should_Return_Custom_Host)
        {
            // Given
            const String uriString = _T("custom://www.test.com");
            Uri uri(uriString);
            CustomParser parser;

            // When
            String host = parser.GetComponents(uri, UriComponents::Host, UriFormat::Unescaped);

            // Then
            EXPECT_EQ(host, String(_T("custom.host")));
        }
    }
}
