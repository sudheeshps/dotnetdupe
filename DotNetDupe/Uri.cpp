#include "pch.h"
#include "Uri.h"
#include <string>
#include <algorithm>
#include <cctype>

namespace DotNetDupe {
    namespace System {
        Uri::Uri(const String& uriString) : _uriString(uriString) {
            ParseUri();
        }

        void Uri::ParseUri() {
            const std::wstring& rawUri = _uriString.GetRawString();
            size_t schemeEnd = rawUri.find(L':');
            
            if (schemeEnd != std::wstring::npos) {
                _scheme = String(rawUri.substr(0, schemeEnd).c_str());
                
                size_t authorityStart = schemeEnd + 1;
                if (rawUri.substr(authorityStart, 2) == L"//") {
                    authorityStart += 2;
                    size_t authorityEnd = rawUri.find_first_of(L"/?#", authorityStart);
                    if (authorityEnd == std::wstring::npos) {
                        _authority = String(rawUri.substr(authorityStart).c_str());
                    } else {
                        _authority = String(rawUri.substr(authorityStart, authorityEnd - authorityStart).c_str());
                    }
                }
            }

            size_t pathStart = rawUri.find(L"//");
            if (pathStart != std::wstring::npos) {
                pathStart = rawUri.find(L'/', pathStart + 2);
            } else {
                pathStart = rawUri.find(L':');
                if (pathStart != std::wstring::npos) {
                    if (_scheme.Equals(_T("mailto"))) {
                        pathStart++; // For mailto, the path starts right after the colon
                    } else {
                        pathStart = rawUri.find(L'/', pathStart + 1);
                    }
                } else {
                    pathStart = 0;
                }
            }

            if (pathStart != std::wstring::npos) {
                size_t pathEnd = rawUri.find_first_of(L"?#", pathStart);
                if (pathEnd == std::wstring::npos) {
                    _absolutePath = String(rawUri.substr(pathStart).c_str());
                } else {
                    _absolutePath = String(rawUri.substr(pathStart, pathEnd - pathStart).c_str());
                }
            }
            else if (_scheme.GetLength() > 0 && _authority.GetLength() > 0 && _absolutePath.IsEmpty()) {
                _absolutePath = _T("/");
            }

            size_t queryStart = rawUri.find(L'?');
            if (queryStart != std::wstring::npos) {
                size_t queryEnd = rawUri.find(L'#', queryStart);
                if (queryEnd == std::wstring::npos) {
                    _query = String(rawUri.substr(queryStart + 1).c_str());
                } else {
                    _query = String(rawUri.substr(queryStart + 1, queryEnd - (queryStart + 1)).c_str());
                }
            }

            size_t fragmentStart = rawUri.find(L'#');
            if (fragmentStart != std::wstring::npos) {
                _fragment = String(rawUri.substr(fragmentStart + 1).c_str());
            }
        }

        String Uri::GetAbsolutePath() const {
            return _absolutePath;
        }

        String Uri::GetAbsoluteUri() const {
            return _uriString;
        }

        String Uri::GetAuthority() const {
            return _authority;
        }

        String Uri::GetHost() const {
            const std::wstring& rawAuthority = _authority.GetRawString();

            size_t atPos = rawAuthority.find_last_of(L'@');
            std::wstring hostAndPortStr = (atPos == std::wstring::npos) ? rawAuthority : rawAuthority.substr(atPos + 1);

            size_t bracketStart = hostAndPortStr.find(L'[');
            if (bracketStart != std::wstring::npos) {
                size_t bracketEnd = hostAndPortStr.find(L']', bracketStart);
                if (bracketEnd != std::wstring::npos) {
                    return String(hostAndPortStr.substr(bracketStart, bracketEnd - bracketStart + 1).c_str());
                }
            }

            size_t colonPos = hostAndPortStr.find_last_of(L':');
            if (colonPos != std::wstring::npos) {
                if (hostAndPortStr.find(L':') == colonPos) {
                    return String(hostAndPortStr.substr(0, colonPos).c_str());
                }
                return String(hostAndPortStr.c_str());
            }

            return String(hostAndPortStr.c_str());
        }

        int Uri::GetPort() const {
            const std::wstring& rawAuthority = _authority.GetRawString();
            size_t colonPos = rawAuthority.find_last_of(L':');

            if (colonPos == std::wstring::npos) {
                if (_scheme.Equals(_T("http"))) {
                    return 80;
                }
                return -1;
            }

            size_t bracketEnd = rawAuthority.find_last_of(L']');
            if (bracketEnd != std::wstring::npos && colonPos < bracketEnd) {
                return -1;
            }

            if (bracketEnd == std::wstring::npos && rawAuthority.find(L':') != colonPos) {
                return -1;
            }

            std::wstring portStr = rawAuthority.substr(colonPos + 1);
            if (!portStr.empty() && std::all_of(portStr.begin(), portStr.end(), ::isdigit)) {
                try {
                    return std::stoi(portStr);
                }
                catch (const std::exception&) {
                    return -1;
                }
            }

            return -1;
        }

        String Uri::GetScheme() const {
            return _scheme;
        }

        String Uri::GetQuery() const {
            return _query;
        }

        String Uri::GetFragment() const {
            return _fragment;
        }
    }
}
