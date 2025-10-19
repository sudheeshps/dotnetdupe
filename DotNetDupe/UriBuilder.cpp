#include "pch.h"
#include "UriBuilder.h"
#include "String.h"
#include <sstream>

namespace DotNetDupe {
    namespace System {
        UriBuilder::UriBuilder() : _port(-1) { }

        UriBuilder::UriBuilder(const String& uri) {
            Uri u(uri);
            _scheme = u.GetScheme();
            _host = u.GetHost();
            _port = u.GetPort();
            _path = u.GetAbsolutePath();
            _query = u.GetQuery();
            _fragment = u.GetFragment();
        }

        UriBuilder::UriBuilder(const Uri& uri) {
            _scheme = uri.GetScheme();
            _host = uri.GetHost();
            _port = uri.GetPort();
            _path = uri.GetAbsolutePath();
            _query = uri.GetQuery();
            _fragment = uri.GetFragment();
        }

        String UriBuilder::GetScheme() const {
            return _scheme;
        }

        void UriBuilder::SetScheme(const String& scheme) {
            _scheme = scheme;
        }

        String UriBuilder::GetHost() const {
            return _host;
        }

        void UriBuilder::SetHost(const String& host) {
            _host = host;
        }

        int UriBuilder::GetPort() const {
            return _port;
        }

        void UriBuilder::SetPortValue(int port) {
            _port = port;
        }

        String UriBuilder::GetPath() const {
            return _path;
        }

        void UriBuilder::SetPath(const String& path) {
            _path = path;
        }

        String UriBuilder::GetQuery() const {
            return _query;
        }

        void UriBuilder::SetQuery(const String& query) {
            _query = query;
        }

        String UriBuilder::GetFragment() const {
            return _fragment;
        }

        void UriBuilder::SetFragment(const String& fragment) {
            _fragment = fragment;
        }

        String UriBuilder::GetUserName() const {
            return _userName;
        }

        void UriBuilder::SetUserName(const String& userName) {
            _userName = userName;
        }

        String UriBuilder::GetPassword() const {
            return _password;
        }

        void UriBuilder::SetPassword(const String& password) {
            _password = password;
        }

        Uri UriBuilder::GetUri() const {
            return Uri(ToString());
        }

        String UriBuilder::ToString() const {
            std::wstringstream wss;
            if (!_scheme.IsEmpty()) {
                wss << _scheme.GetRawString() << _T("://");
            }

            if (!_userName.IsEmpty()) {
                wss << _userName.GetRawString() << _T(":");
            }

            if (!_password.IsEmpty()) {
                wss << _password.GetRawString() << _T("@");
            }

            if (!_host.IsEmpty()) {
                wss << _host.GetRawString();
            }

            if (_port != -1) {
                wss << _T(":") << _port;
            }

            if (!_path.IsEmpty()) {
                wss << _path.GetRawString();
            }

            if (!_query.IsEmpty()) {
                wss << _T("?") << _query.GetRawString();
            }

            if (!_fragment.IsEmpty()) {
                wss << _T("#") << _fragment.GetRawString();
            }

            return String(wss.str().c_str());
        }
    }
}
