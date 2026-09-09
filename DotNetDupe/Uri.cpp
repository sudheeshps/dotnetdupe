#include "pch.h"
#include "System/Uri.h"
#include <string>
#include <algorithm>
#include <cctype>

namespace DotNetDupe {
    namespace System {
        Uri::Uri(const String& uriString) : _uriString(uriString) {
            /// Parse components of URI string upon construction.
            ParseUri();
        }

        static void ParseSchemeAndAuthority(const std::string& rawUri, String& scheme, String& authority) {
            /// Step: Find scheme delimiter and extract URI scheme.
            size_t schemeEnd = rawUri.find(':');
            if (schemeEnd == std::string::npos) return;
            scheme = String(rawUri.substr(0, schemeEnd).c_str());

            /// Step: Extract authority component if double slash follows scheme.
            size_t authStart = schemeEnd + 1;
            if (authStart + 1 < rawUri.length() && rawUri.substr(authStart, 2) == "//") {
                authStart += 2;
                size_t authEnd = rawUri.find_first_of("/?#", authStart);
                authority = (authEnd == std::string::npos) 
                    ? String(rawUri.substr(authStart).c_str())
                    : String(rawUri.substr(authStart, authEnd - authStart).c_str());
            }
        }

        static void ParsePath(const std::string& rawUri, const String& scheme, String& path) {
            /// Step: Locate start of path after authority or scheme delimiter.
            size_t pathStart = rawUri.find("//");
            if (pathStart != std::string::npos) {
                pathStart = rawUri.find('/', pathStart + 2);
            } else {
                size_t colon = rawUri.find(':');
                pathStart = (colon != std::string::npos) 
                    ? (scheme.Equals("mailto") ? colon + 1 : rawUri.find('/', colon + 1)) 
                    : 0;
            }

            /// Step: Extract path substring before query or fragment indicators.
            if (pathStart != std::string::npos) {
                size_t pathEnd = rawUri.find_first_of("?#", pathStart);
                path = (pathEnd == std::string::npos)
                    ? String(rawUri.substr(pathStart).c_str())
                    : String(rawUri.substr(pathStart, pathEnd - pathStart).c_str());
            }
            if (path.IsEmpty()) path = String("/");
        }

        static void ParseQueryAndFragment(const std::string& rawUri, String& query, String& fragment) {
            /// Step: Extract query substring following question mark.
            size_t qStart = rawUri.find('?');
            if (qStart != std::string::npos) {
                size_t qEnd = rawUri.find('#', qStart);
                query = (qEnd == std::string::npos)
                    ? String(rawUri.substr(qStart + 1).c_str())
                    : String(rawUri.substr(qStart + 1, qEnd - qStart - 1).c_str());
            }

            /// Step: Extract fragment substring following hash.
            size_t fStart = rawUri.find('#');
            if (fStart != std::string::npos) {
                fragment = String(rawUri.substr(fStart + 1).c_str());
            }
        }

        static void ParseAuthorityComponents(const String& authority, const String& scheme, String& userInfo, String& host, int& port) {
            /// Guard: If authority string is empty, return defaults.
            std::string authStr = (const char*)authority;
            if (authStr.empty()) return;

            /// Step: Extract optional user info preceding '@'.
            size_t userEnd = authStr.find('@');
            if (userEnd != std::string::npos) {
                userInfo = String(authStr.substr(0, userEnd).c_str());
                authStr = authStr.substr(userEnd + 1);
            }

            /// Step: Extract host name and explicit or default port based on scheme.
            size_t portStart = authStr.find(':');
            if (portStart != std::string::npos) {
                host = String(authStr.substr(0, portStart).c_str());
                port = std::stoi(authStr.substr(portStart + 1));
            } else {
                host = String(authStr.c_str());
                port = (scheme.ToLower() == "https") ? 443 : 80;
            }
        }

        void Uri::ParseUri() {
            /// Step: Parse URI into standard RFC components.
            std::string rawUri = (const char*)_uriString;
            ParseSchemeAndAuthority(rawUri, _scheme, _authority);
            ParsePath(rawUri, _scheme, _path);
            ParseQueryAndFragment(rawUri, _query, _fragment);
            ParseAuthorityComponents(_authority, _scheme, _userInfo, _host, _port);
        }

        String Uri::GetScheme() const { return _scheme; }
        String Uri::GetAbsoluteUri() const { return _uriString; }
        String Uri::GetAuthority() const { return _authority; }
        String Uri::GetHost() const { return _host; }
        int Uri::GetPort() const { return _port; }
        String Uri::GetAbsolutePath() const { return _path; }
        String Uri::GetQuery() const { return _query; }
        String Uri::GetFragment() const { return _fragment; }
        String Uri::GetUserInfo() const { return _userInfo; }
        String Uri::GetOriginalString() const { return _uriString; }
        
        String Uri::ToString() const {
            /// Return original URI string.
            return _uriString;
        }

        bool Uri::IsDefaultPort() const {
            /// Step: Check if configured port matches standard scheme default.
            if (_scheme.Equals("http") && _port == 80) return true;
            if (_scheme.Equals("https") && _port == 443) return true;
            return false;
        }

        bool Uri::IsFile() const {
            /// Step: Check if scheme matches file protocol.
            return _scheme.Equals("file");
        }

        bool Uri::IsLoopback() const {
            /// Step: Check if host corresponds to loopback address or localhost.
            return _host.Equals("localhost") || _host.Equals("127.0.0.1") || _host.Equals("::1");
        }

        String Uri::EscapeDataString(const String& stringToEscape) {
            /// Step: Percent-encode non-unreserved ASCII characters per RFC 3986.
            std::string s = (const char*)stringToEscape;
            std::string escaped;
            for (char c : s) {
                if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
                    escaped += c;
                } else {
                    char buf[4];
                    snprintf(buf, 4, "%%%02X", static_cast<unsigned char>(c));
                    escaped += buf;
                }
            }
            return String(escaped.c_str());
        }

        static void UnescapePercentChar(const std::string& s, size_t& i, std::string& unescaped) {
            /// Step: Decode two hex digits following percent symbol.
            int value;
#if defined(_WIN32)
            if (sscanf_s(s.substr(i + 1, 2).c_str(), "%x", &value) == 1) {
#else
            if (sscanf(s.substr(i + 1, 2).c_str(), "%x", &value) == 1) {
#endif
                unescaped += static_cast<char>(value);
                i += 2;
            } else {
                unescaped += s[i];
            }
        }

        String Uri::UnescapeDataString(const String& stringToUnescape) {
            /// Step: Iterate through string and unescape percent-encoded hex sequences.
            std::string s = (const char*)stringToUnescape;
            std::string unescaped;
            for (size_t i = 0; i < s.length(); ++i) {
                if (s[i] == '%' && i + 2 < s.length()) {
                    UnescapePercentChar(s, i, unescaped);
                } else {
                    unescaped += s[i];
                }
            }
            return String(unescaped.c_str());
        }
    }
}
