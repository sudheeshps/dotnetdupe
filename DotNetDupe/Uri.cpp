#include "pch.h"
#include "System/Uri.h"
#include <string>
#include <algorithm>
#include <cctype>

namespace DotNetDupe {
    namespace System {
        Uri::Uri(const String& uriString) : _uriString(uriString) {
            ParseUri();
        }

        void Uri::ParseUri() {
            std::string rawUri = (const char*)_uriString;
            size_t schemeEnd = rawUri.find(':');
            
            if (schemeEnd != std::string::npos) {
                _scheme = String(rawUri.substr(0, schemeEnd).c_str());
                
                size_t authorityStart = schemeEnd + 1;
                if (authorityStart + 1 < rawUri.length() && rawUri.substr(authorityStart, 2) == "//") {
                    authorityStart += 2;
                    size_t authorityEnd = rawUri.find_first_of("/?#", authorityStart);
                    if (authorityEnd == std::string::npos) {
                        _authority = String(rawUri.substr(authorityStart).c_str());
                    } else {
                        _authority = String(rawUri.substr(authorityStart, authorityEnd - authorityStart).c_str());
                    }
                }
            }

            size_t pathStart = rawUri.find("//");
            if (pathStart != std::string::npos) {
                pathStart = rawUri.find('/', pathStart + 2);
            } else {
                pathStart = rawUri.find(':');
                if (pathStart != std::string::npos) {
                    if (_scheme.Equals("mailto")) {
                        pathStart++; // For mailto, the path starts right after the colon
                    } else {
                        pathStart = rawUri.find('/', pathStart + 1);
                    }
                } else {
                    pathStart = 0;
                }
            }

            if (pathStart != std::string::npos) {
                size_t pathEnd = rawUri.find_first_of("?#", pathStart);
                if (pathEnd == std::string::npos) {
                    _path = String(rawUri.substr(pathStart).c_str());
                } else {
                    _path = String(rawUri.substr(pathStart, pathEnd - pathStart).c_str());
                }
            }
            if (_path.IsEmpty()) _path = String("/");

            size_t queryStart = rawUri.find('?');
            if (queryStart != std::string::npos) {
                size_t queryEnd = rawUri.find('#', queryStart);
                if (queryEnd == std::string::npos) {
                    _query = String(rawUri.substr(queryStart + 1).c_str());
                } else {
                    _query = String(rawUri.substr(queryStart + 1, queryEnd - queryStart - 1).c_str());
                }
            }

            size_t fragmentStart = rawUri.find('#');
            if (fragmentStart != std::string::npos) {
                _fragment = String(rawUri.substr(fragmentStart + 1).c_str());
            }

            // Simple Authority parsing
            std::string authorityStr = (const char*)_authority;
            if (!authorityStr.empty()) {
                size_t userInfoEnd = authorityStr.find('@');
                if (userInfoEnd != std::string::npos) {
                    _userInfo = String(authorityStr.substr(0, userInfoEnd).c_str());
                    authorityStr = authorityStr.substr(userInfoEnd + 1);
                }

                size_t portStart = authorityStr.find(':');
                if (portStart != std::string::npos) {
                    _host = String(authorityStr.substr(0, portStart).c_str());
                    _port = std::stoi(authorityStr.substr(portStart + 1));
                } else {
                    _host = String(authorityStr.c_str());
                    _port = 80; // Default
                }
            }
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
            return _uriString;
        }

        bool Uri::IsDefaultPort() const {
            if (_scheme.Equals("http") && _port == 80) return true;
            if (_scheme.Equals("https") && _port == 443) return true;
            return false;
        }

        bool Uri::IsFile() const {
            return _scheme.Equals("file");
        }

        bool Uri::IsLoopback() const {
            return _host.Equals("localhost") || _host.Equals("127.0.0.1") || _host.Equals("::1");
        }

        String Uri::EscapeDataString(const String& stringToEscape) {
            std::string s = (const char*)stringToEscape;
            std::string escaped;
            for (char c : s) {
                if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
                    escaped += c;
                } else {
                    char buf[4];
                    sprintf_s(buf, 4, "%%%02X", static_cast<unsigned char>(c));
                    escaped += buf;
                }
            }
            return String(escaped.c_str());
        }

        String Uri::UnescapeDataString(const String& stringToUnescape) {
            std::string s = (const char*)stringToUnescape;
            std::string unescaped;
            for (size_t i = 0; i < s.length(); ++i) {
                if (s[i] == '%' && i + 2 < s.length()) {
                    int value;
                    if (sscanf_s(s.substr(i + 1, 2).c_str(), "%x", &value) == 1) {
                        unescaped += static_cast<char>(value);
                        i += 2;
                    } else {
                        unescaped += s[i];
                    }
                } else {
                    unescaped += s[i];
                }
            }
            return String(unescaped.c_str());
        }
    }
}
