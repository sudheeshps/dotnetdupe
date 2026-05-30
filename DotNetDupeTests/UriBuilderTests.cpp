#include "pch.h"
#include "gtest/gtest.h"
#include "System/Uri.h"
#include "System/UriBuilder.h"
#include "System/String.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace UriBuilderTests {
        // Given: A Uri object
        // When: A UriBuilder is constructed from the Uri object
        // Then: The properties of the UriBuilder should match the Uri
        TEST(UriBuilderTest, Constructor_FromUri_Should_Initialize_Properties_Correctly) {
            // Given
            const String uriString("http://www.test.com:8080/path/to/resource?query=1#fragment");
            Uri uri(uriString);

            // When
            UriBuilder builder(uri);

            // Then
            EXPECT_EQ(builder.GetScheme(), "http");
            EXPECT_EQ(builder.GetHost(), "www.test.com");
            EXPECT_EQ(builder.GetPort(), 8080);
            EXPECT_EQ(builder.GetPath(), "/path/to/resource");
            EXPECT_EQ(builder.GetQuery(), "query=1");
            EXPECT_EQ(builder.GetFragment(), "fragment");
        }

        // Given: A default UriBuilder
        // When: Properties are set individually
        // Then: The ToString method should return the correct URI string
        TEST(UriBuilderTest, ToString_Should_Return_Correct_Uri_String_After_Setting_Properties) {
            // Given
            UriBuilder builder;

            // When
            builder.SetScheme("https");
            builder.SetHost("www.anothertest.com");
            builder.SetPort(443);
            builder.SetPath("/another/path");
            builder.SetQuery("q=2");
            builder.SetFragment("section2");

            // Then
            EXPECT_EQ(builder.ToString(), "https://www.anothertest.com:443/another/path?q=2#section2");
        }

        // Given: A UriBuilder with properties set
        // When: The GetUri method is called
        // Then: A Uri object with the correct properties should be returned
        TEST(UriBuilderTest, GetUri_Should_Return_Correct_Uri_Object) {
            // Given
            UriBuilder builder;
            builder.SetScheme("ftp");
            builder.SetHost("ftp.test.com");
            builder.SetPath("/files");

            // When
            Uri uri = builder.GetUri();

            // Then
            EXPECT_EQ(uri.GetScheme(), "ftp");
            EXPECT_EQ(uri.GetHost(), "ftp.test.com");
            EXPECT_EQ(uri.GetAbsolutePath(), "/files");
        }
    }
}
