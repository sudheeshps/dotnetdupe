#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Uri.h"
#include "../DotNetDupe/UriBuilder.h"
#include "../DotNetDupe/String.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace UriBuilderTests {
        // Given: A Uri object
        // When: A UriBuilder is constructed from the Uri object
        // Then: The properties of the UriBuilder should match the Uri
        TEST(UriBuilderTest, Constructor_FromUri_Should_Initialize_Properties_Correctly) {
            // Given
            const String uriString(_T("http://www.test.com:8080/path/to/resource?query=1#fragment"));
            Uri uri(uriString);

            // When
            UriBuilder builder(uri);

            // Then
            EXPECT_EQ(builder.GetScheme(), _T("http"));
            EXPECT_EQ(builder.GetHost(), _T("www.test.com"));
            EXPECT_EQ(builder.GetPort(), 8080);
            EXPECT_EQ(builder.GetPath(), _T("/path/to/resource"));
            EXPECT_EQ(builder.GetQuery(), _T("query=1"));
            EXPECT_EQ(builder.GetFragment(), _T("fragment"));
        }

        // Given: A default UriBuilder
        // When: Properties are set individually
        // Then: The ToString method should return the correct URI string
        TEST(UriBuilderTest, ToString_Should_Return_Correct_Uri_String_After_Setting_Properties) {
            // Given
            UriBuilder builder;

            // When
            builder.SetScheme(_T("https"));
            builder.SetHost(_T("www.anothertest.com"));
            builder.SetPortValue(443);
            builder.SetPath(_T("/another/path"));
            builder.SetQuery(_T("q=2"));
            builder.SetFragment(_T("section2"));

            // Then
            EXPECT_EQ(builder.ToString(), _T("https://www.anothertest.com:443/another/path?q=2#section2"));
        }

        // Given: A UriBuilder with properties set
        // When: The GetUri method is called
        // Then: A Uri object with the correct properties should be returned
        TEST(UriBuilderTest, GetUri_Should_Return_Correct_Uri_Object) {
            // Given
            UriBuilder builder;
            builder.SetScheme(_T("ftp"));
            builder.SetHost(_T("ftp.test.com"));
            builder.SetPath(_T("/files"));

            // When
            Uri uri = builder.GetUri();

            // Then
            EXPECT_EQ(uri.GetScheme(), _T("ftp"));
            EXPECT_EQ(uri.GetHost(), _T("ftp.test.com"));
            EXPECT_EQ(uri.GetAbsolutePath(), _T("/files"));
        }
    }
}
