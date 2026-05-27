#include "pch.h"
#include "gtest/gtest.h"
#include "System/Uri.h"
#include "System/String.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace UriTests {
        // Given: A valid URI string
        // When: A Uri object is constructed
        // Then: The properties of the Uri object should be correctly parsed
        TEST(UriTest, Constructor_Should_Parse_Uri_Correctly) {
            // Given
            const String uriString("http://www.test.com:8080/path/to/resource?query=1#fragment");

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetScheme(), "http");
            EXPECT_EQ(uri.GetHost(), "www.test.com");
            EXPECT_EQ(uri.GetPort(), 8080);
            EXPECT_EQ(uri.GetAbsolutePath(), "/path/to/resource");
            EXPECT_EQ(uri.GetQuery(), "query=1");
            EXPECT_EQ(uri.GetFragment(), "fragment");
            EXPECT_EQ(uri.GetAbsoluteUri(), uriString);
            EXPECT_EQ(uri.GetAuthority(), "www.test.com:8080");
        }

        // Given: A URI string with no port
        // When: A Uri object is constructed
        // Then: The Port property should return the default for the scheme
        TEST(UriTest, Constructor_Should_Return_Default_Port_When_No_Port_Specified) {
            // Given
            const String uriString("http://www.test.com/path/to/resource");

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetPort(), 80);
        }

        // Given: A URI string with no path
        // When: A Uri object is constructed
        // Then: The AbsolutePath property should return "/"
        TEST(UriTest, Constructor_Should_Return_Slash_For_AbsolutePath_When_No_Path_Specified) {
            // Given
            const String uriString("http://www.test.com");

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetAbsolutePath(), "/");
        }

        // Given: A URI string with no query
        // When: A Uri object is constructed
        // Then: The Query property should return an empty string
        TEST(UriTest, Constructor_Should_Return_Empty_String_For_Query_When_No_Query_Specified) {
            // Given
            const String uriString("http://www.test.com/path");

            // When
            Uri uri(uriString);

            // Then
            EXPECT_TRUE(uri.GetQuery().IsEmpty());
        }

        // Given: A URI string with no fragment
        // When: A Uri object is constructed
        // Then: The Fragment property should return an empty string
        TEST(UriTest, Constructor_Should_Return_Empty_String_For_Fragment_When_No_Fragment_Specified) {
            // Given
            const String uriString("http://www.test.com/path");

            // When
            Uri uri(uriString);

            // Then
            EXPECT_TRUE(uri.GetFragment().IsEmpty());
        }

        // Given: A simple mailto URI
        // When: A Uri object is constructed
        // Then: The properties are parsed correctly
        TEST(UriTest, Constructor_Should_Parse_Mailto_Scheme_Correctly) {
            // Given
            const String uriString("mailto:test@example.com");

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetScheme(), "mailto");
            EXPECT_EQ(uri.GetAbsolutePath(), "test@example.com");
            EXPECT_EQ(uri.GetAbsoluteUri(), uriString);
        }
    }
}
