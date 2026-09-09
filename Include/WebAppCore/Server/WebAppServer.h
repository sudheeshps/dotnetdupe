/// \file WebAppServer.h
/// \brief Static file web server and application hosting wrapper for WebApplication.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "WebAppCore/Builder/WebApplication.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Server {

            /// \brief High-level HTTP web server providing static file serving, MIME detection, and lifecycle management.
            ///
            /// Wraps a configured WebApplication host to automatically serve static content (HTML, CSS, JS, images, fonts)
            /// from a configured web root folder while dispatching API routes and WebSocket upgrades.
            ///
            /// \note Thread Safety: Thread-safe for serving requests; configuration methods must be called prior to Run().
            /// \note Conforms to RFC 9110 (HTTP Semantics) and RFC 9112 (HTTP/1.1).
            /// \see WebApplication, HttpContext
            class WebAppServer : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Constructs a WebAppServer wrapping the specified WebApplication host and web root path.
                /// \param app SmartPointer to the underlying WebApplication host.
                /// \param webRoot Directory path for static file assets (default "wwwroot").
                /// \throws DotNetDupe::System::ArgumentException if app is null.
                DOTNETDUPE_API explicit WebAppServer(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::String& webRoot = "wwwroot");

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~WebAppServer() override = default;

                WebAppServer(const WebAppServer&) = delete;
                WebAppServer& operator=(const WebAppServer&) = delete;

                /// \brief Move constructor.
                /// \param other WebAppServer rvalue to move from.
                DOTNETDUPE_API WebAppServer(WebAppServer&& other) noexcept;

                /// \brief Move assignment operator.
                /// \param other WebAppServer rvalue to move from.
                /// \return Reference to this WebAppServer instance.
                DOTNETDUPE_API WebAppServer& operator=(WebAppServer&& other) noexcept;

                /// \brief Sets the local filesystem directory path from which static files are served.
                /// \param webRoot Directory path (e.g., "wwwroot" or "public").
                DOTNETDUPE_API void SetWebRoot(const DotNetDupe::System::String& webRoot) { m_sWebRoot = webRoot; }

                /// \brief Gets the configured web root directory path.
                /// \return Directory path string.
                DOTNETDUPE_API DotNetDupe::System::String GetWebRoot() const { return m_sWebRoot; }

                /// \brief Enables static file serving with the specified default document fallback.
                /// \param defaultDocument File name to serve for directory root requests (default "index.html").
                DOTNETDUPE_API void EnableStaticFiles(const DotNetDupe::System::String& defaultDocument = "index.html");

                /// \brief Starts the underlying WebApplication server listener on the specified URL and thread pool size.
                /// \param url Listening URL (default "http://localhost:8080/index.html").
                /// \param threadCount Minimum worker threads for connection handling (default 10).
                DOTNETDUPE_API void Run(const DotNetDupe::System::String& url = "http://localhost:8080/index.html", int threadCount = 10);

                /// \brief Stops the running web server and active TCP listener.
                DOTNETDUPE_API void Stop();

                /// \brief Resolves the standard MIME content type string based on a file path extension.
                /// \param filePath Path or filename to examine.
                /// \return Matching MIME type (e.g., "text/html; charset=utf-8", "image/png", or "application/octet-stream").
                DOTNETDUPE_API static DotNetDupe::System::String GetMimeType(const DotNetDupe::System::String& filePath);

            private:
                DotNetDupe::System::SmartPointer<Builder::WebApplication> m_spApp;
                DotNetDupe::System::String m_sWebRoot;
                DotNetDupe::System::String m_sDefaultDocument;
                bool m_bStaticFilesEnabled;
            };

        }
    }
}
