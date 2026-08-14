#include "pch.h"
#include "System/UriBuilder.h"
#include <string>

namespace DotNetDupe {
    namespace System {
        UriBuilder::UriBuilder() : _port(-1) {}
        UriBuilder::UriBuilder(const String& uri) : _uri(uri), _port(-1) {
            // Simple parsing to initialize fields if needed
        }
        UriBuilder::UriBuilder(const Uri& uri)
            : _scheme(uri.GetScheme()), _host(uri.GetHost()), _port(uri.GetPort()),
              _path(uri.GetAbsolutePath()), _query(uri.GetQuery()), _fragment(uri.GetFragment()) {
            _uri = uri.ToString();
        }
        UriBuilder::UriBuilder(const String& scheme, const String& host)
            : _scheme(scheme), _host(host), _port(-1) {}
        UriBuilder::UriBuilder(const String& scheme, const String& host, int port)
            : _scheme(scheme), _host(host), _port(port) {}

        String UriBuilder::GetScheme() const { return _scheme; }
        void UriBuilder::SetScheme(const String& value) { _scheme = value; }

        String UriBuilder::GetHost() const { return _host; }
        void UriBuilder::SetHost(const String& value) { _host = value; }

        int UriBuilder::GetPort() const { return _port; }
        void UriBuilder::SetPort(int value) { _port = value; }

        String UriBuilder::GetPath() const { return _path; }
        void UriBuilder::SetPath(const String& value) { _path = value; }

        String UriBuilder::GetQuery() const { return _query; }
        void UriBuilder::SetQuery(const String& value) { _query = value; }

        String UriBuilder::GetFragment() const { return _fragment; }
        void UriBuilder::SetFragment(const String& value) { _fragment = value; }

        String UriBuilder::GetUserName() const { return _userName; }
        void UriBuilder::SetUserName(const String& value) { _userName = value; }

        String UriBuilder::GetPassword() const { return _password; }
        void UriBuilder::SetPassword(const String& value) { _password = value; }

        Uri UriBuilder::GetUri() {
            return Uri(ToString());
        }

        static void AppendAuthority(std::string& result, const String& host, const String& user, const String& pass, int port) {
            if (host.IsEmpty()) return;
            result += "//";
            if (!user.IsEmpty()) {
                result += (const char*)user;
                if (!pass.IsEmpty()) {
                    result += ":"; result += (const char*)pass;
                }
                result += "@";
            }
            result += (const char*)host;
            if (port != -1) {
                result += ":"; result += std::to_string(port);
            }
        }

        static void AppendPathAndQuery(std::string& result, const String& path, const String& query, const String& fragment) {
            if (!path.IsEmpty()) {
                if (path[0] != '/') result += "/";
                result += (const char*)path;
            }
            if (!query.IsEmpty()) {
                if (query[0] != '?') result += "?";
                result += (const char*)query;
            }
            if (!fragment.IsEmpty()) {
                if (fragment[0] != '#') result += "#";
                result += (const char*)fragment;
            }
        }

        String UriBuilder::ToString() {
            std::string result;
            if (!_scheme.IsEmpty()) {
                result += (const char*)_scheme; result += ":";
            }
            AppendAuthority(result, _host, _userName, _password, _port);
            AppendPathAndQuery(result, _path, _query, _fragment);
            return String(result.c_str());
        }
    }
}
