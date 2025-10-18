#include "pch.h"
#include "Uri.h"
#include <regex>    // For URI parsing
#include <string>   // For std::stoi
#include <algorithm> // For std::all_of
#include <cctype>   // For ::isdigit

namespace DotNetDupe
{
    namespace System
    {
        // Define the full URI regex as a static constant.
        // This regex is a simplification and might not cover all edge cases or RFCs fully.
        // Group 1: Scheme with colon (e.g., "http:")
        // Group 2: Scheme (e.g., "http")
        // Group 3: Authority with "//" (e.g., "//www.example.com")
        // Group 4: Authority (e.g., "www.example.com")
        // Group 5: Path (e.g., "/path/to/resource")
        // Group 6: Query with "?" (e.g., "?key=value")
        // Group 7: Query (e.g., "key=value")
        // Group 8: Fragment with "#" (e.g., "#section")
        // Group 9: Fragment (e.g., "section")
        static const std::wregex fullUriRegex(L"^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?");

        // Helper function to extract a component using regex
        static String GetRegexGroup(const String& source, const std::wregex& regex, int groupIndex)
        {
            std::wsmatch match;
            // Use GetString() to get the underlying std::wstring for std::regex_search
            const std::wstring& stdSource = source.GetRawString();
            if (std::regex_search(stdSource, match, regex) && match.size() > groupIndex)
            {
                return match[groupIndex].str().c_str();
            }
            return String(L"");
        }

        Uri::Uri(const String& uriString) : _uriString(uriString)
        {
            // The constructor simply stores the URI string.
            // Parsing of components is done on demand in the getter methods.
        }

        String Uri::GetAbsolutePath() const
        {
            // The path component is typically group 5 in the full URI regex.
            return GetRegexGroup(_uriString, fullUriRegex, 5);
        }

        String Uri::GetAbsoluteUri() const
        {
            return _uriString;
        }

        String Uri::GetAuthority() const
        {
            // The authority component is typically group 4 in the full URI regex.
            return GetRegexGroup(_uriString, fullUriRegex, 4);
        }

        String Uri::GetHost() const
        {
            String authority = GetAuthority();
            const std::wstring& rawAuthority = authority.GetRawString();

            size_t atPos = rawAuthority.find_last_of(L'@');
            std::wstring hostAndPortStr = (atPos == std::wstring::npos) ? rawAuthority : rawAuthority.substr(atPos + 1);

            size_t bracketStart = hostAndPortStr.find(L'[');
            if (bracketStart != std::wstring::npos)
            {
                size_t bracketEnd = hostAndPortStr.find(L']', bracketStart);
                if (bracketEnd != std::wstring::npos)
                {
                    // Return IPv6 address part, e.g., [::1]
                    return String(hostAndPortStr.substr(bracketStart, bracketEnd - bracketStart + 1).c_str());
                }
            }

            size_t colonPos = hostAndPortStr.find_last_of(L':');
            if (colonPos != std::wstring::npos)
            {
                // Check if there are other colons before this one
                if (hostAndPortStr.find(L':') == colonPos)
                {
                    return String(hostAndPortStr.substr(0, colonPos).c_str());
                }
                // Multiple colons, assume IPv6 and return the whole thing
                return String(hostAndPortStr.c_str());
            }

            return String(hostAndPortStr.c_str());
        }

        int Uri::GetPort() const
        {
            String authority = GetAuthority();
            const std::wstring& rawAuthority = authority.GetRawString();

            size_t colonPos = rawAuthority.find_last_of(L':');

            // If no colon, no port
            if (colonPos == std::wstring::npos)
            {
                return -1;
            }

            // If there's an IPv6 address in brackets, colon must be after it
            size_t bracketEnd = rawAuthority.find_last_of(L']');
            if (bracketEnd != std::wstring::npos && colonPos < bracketEnd)
            {
                return -1; // Colon is part of IPv6 address
            }

            // If there are no brackets, but multiple colons, it's a bare IPv6 address
            if (bracketEnd == std::wstring::npos && rawAuthority.find(L':') != colonPos)
            {
                return -1;
            }

            std::wstring portStr = rawAuthority.substr(colonPos + 1);
            if (!portStr.empty() && std::all_of(portStr.begin(), portStr.end(), ::isdigit))
            {
                try
                {
                    return std::stoi(portStr);
                }
                catch (const std::exception&)
                {
                    // Catch exceptions from stoi, e.g., out of range
                    return -1;
                }
            }

            return -1;
        }
    
        String Uri::GetScheme() const
        {
            // The scheme component is typically group 2 in the full URI regex.
            return GetRegexGroup(_uriString, fullUriRegex, 2);
        }

        String Uri::GetQuery() const
        {
            // The query component is typically group 7 in the full URI regex.
            return GetRegexGroup(_uriString, fullUriRegex, 7);
        }

        String Uri::GetFragment() const
        {
            // The fragment component is typically group 9 in the full URI regex.
            return GetRegexGroup(_uriString, fullUriRegex, 9);
        }
    }
}
