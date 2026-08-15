#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "WebAppCore/Builder/WebApplication.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Server {

            class WebAppServer : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API explicit WebAppServer(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::String& webRoot = "wwwroot");
                DOTNETDUPE_API ~WebAppServer() override = default;

                WebAppServer(const WebAppServer&) = delete;
                WebAppServer& operator=(const WebAppServer&) = delete;
                DOTNETDUPE_API WebAppServer(WebAppServer&&) noexcept;
                DOTNETDUPE_API WebAppServer& operator=(WebAppServer&&) noexcept;

                DOTNETDUPE_API void SetWebRoot(const DotNetDupe::System::String& webRoot) { m_sWebRoot = webRoot; }
                DOTNETDUPE_API DotNetDupe::System::String GetWebRoot() const { return m_sWebRoot; }

                DOTNETDUPE_API void EnableStaticFiles(const DotNetDupe::System::String& defaultDocument = "index.html");
                DOTNETDUPE_API void Run(const DotNetDupe::System::String& url = "http://localhost:8080/index.html", int threadCount = 10);
                DOTNETDUPE_API void Stop();

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
