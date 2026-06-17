#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Func.h"
#include "System/IServiceProvider.h"
#include "WebAppCore/Http/HttpContext.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Net/Sockets/TcpListener.h"
#include <atomic>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {

            class WebApplicationBuilder;

            class WebApplication : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API explicit WebApplication(const DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>& spServices);
                DOTNETDUPE_API ~WebApplication() override;

                WebApplication(const WebApplication&) = delete;
                WebApplication& operator=(const WebApplication&) = delete;
                DOTNETDUPE_API WebApplication(WebApplication&&) noexcept;
                DOTNETDUPE_API WebApplication& operator=(WebApplication&&) noexcept;

                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<WebApplicationBuilder> CreateBuilder();
                
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> GetServices() const { return m_spServices; }

                DOTNETDUPE_API void MapGet(const DotNetDupe::System::String& pattern, DotNetDupe::System::Func<DotNetDupe::System::String, DotNetDupe::System::SmartPointer<Http::HttpContext>> handler);
                DOTNETDUPE_API void MapPost(const DotNetDupe::System::String& pattern, DotNetDupe::System::Func<DotNetDupe::System::String, DotNetDupe::System::SmartPointer<Http::HttpContext>> handler);

                DOTNETDUPE_API void Run(const DotNetDupe::System::String& url = "http://127.0.0.1:5000");
                DOTNETDUPE_API void Stop();

            private:
                void HandleConnection(DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::Sockets::TcpClient> spClient);
                void StartServerLoop(const DotNetDupe::System::String& host, int port);

                DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> m_spServices;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::Func<DotNetDupe::System::String, DotNetDupe::System::SmartPointer<Http::HttpContext>>> m_getHandlers;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::Func<DotNetDupe::System::String, DotNetDupe::System::SmartPointer<Http::HttpContext>>> m_postHandlers;
                
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::Sockets::TcpListener> m_pListener;
                std::atomic<bool> m_bRunning;
                DotNetDupe::System::String m_sHost;
                int m_nPort;
            };

        }
    }
}
