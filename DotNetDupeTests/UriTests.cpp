#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Uri.h"
#include "../DotNetDupe/String.h"

using namespace DotNetDupe::System;

namespace SystemTests
{
    namespace UriTests
    {
        // Given: A valid URI string
        // When: A Uri object is constructed
        // Then: The properties of the Uri object should be correctly parsed
        TEST(UriTest, Constructor_Should_Parse_Uri_Correctly)
        {
            // Given
            const String uriString(_T("http://www.test.com:8080/path/to/resource?query=1#fragment"));

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetScheme(), _T("http"));
            EXPECT_EQ(uri.GetHost(), _T("www.test.com"));
            EXPECT_EQ(uri.GetPort(), 8080);
            EXPECT_EQ(uri.GetAbsolutePath(), _T("/path/to/resource"));
            EXPECT_EQ(uri.GetQuery(), _T("?query=1"));
            EXPECT_EQ(uri.GetFragment(), _T("#fragment"));
            EXPECT_EQ(uri.GetAbsoluteUri(), uriString);
            EXPECT_EQ(uri.GetAuthority(), _T("www.test.com:8080"));
        }

        // Given: A URI string with no port
        // When: A Uri object is constructed
        // Then: The Port property should return the default for the scheme
        TEST(UriTest, Constructor_Should_Return_Default_Port_When_No_Port_Specified)
        {
            // Given
            const String uriString(_T("http://www.test.com/path/to/resource"));

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetPort(), 80);
        }

        // Given: A URI string with no path
        // When: A Uri object is constructed
        // Then: The AbsolutePath property should return "/"
        TEST(UriTest, Constructor_Should_Return_Slash_For_AbsolutePath_When_No_Path_Specified)
        {
            // Given
            const String uriString(_T("http://www.test.com"));

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetAbsolutePath(), _T("/"));
        }

        // Given: A URI string with no query
        // When: A Uri object is constructed
        // Then: The Query property should return an empty string
        TEST(UriTest, Constructor_Should_Return_Empty_String_For_Query_When_No_Query_Specified)
        {
            // Given
            const String uriString(_T("http://www.test.com/path"));

            // When
            Uri uri(uriString);

            // Then
            EXPECT_TRUE(uri.GetQuery().IsEmpty());
        }

        // Given: A URI string with no fragment
        // When: A Uri object is constructed
        // Then: The Fragment property should return an empty string
        TEST(UriTest, Constructor_Should_Return_Empty_String_For_Fragment_When_No_Fragment_Specified)
        {
            // Given
            const String uriString(_T("http://www.test.com/path"));

            // When
            Uri uri(uriString);

            // Then
            EXPECT_TRUE(uri.GetFragment().IsEmpty());
        }

        // Given: A simple mailto URI
        // When: A Uri object is constructed
        // Then: The properties are parsed correctly
        TEST(UriTest, Constructor_Should_Parse_Mailto_Scheme_Correctly)
        {
            // Given
            const String uriString(_T("mailto:test@example.com"));

            // When
            Uri uri(uriString);

            // Then
            EXPECT_EQ(uri.GetScheme(), _T("mailto"));
            EXPECT_EQ(uri.GetAbsolutePath(), _T("test@example.com"));
            EXPECT_EQ(uri.GetAbsoluteUri(), uriString);
        }
    }
}
