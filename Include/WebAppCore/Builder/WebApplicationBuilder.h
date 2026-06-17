#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "WebAppCore/Builder/WebApplication.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {

            class WebApplicationBuilder : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API WebApplicationBuilder();
                DOTNETDUPE_API ~WebApplicationBuilder() override = default;

                WebApplicationBuilder(const WebApplicationBuilder&) = delete;
                WebApplicationBuilder& operator=(const WebApplicationBuilder&) = delete;
                DOTNETDUPE_API WebApplicationBuilder(WebApplicationBuilder&&) noexcept;
                DOTNETDUPE_API WebApplicationBuilder& operator=(WebApplicationBuilder&&) noexcept;

                DOTNETDUPE_API DotNetDupe::Extensions::DependencyInjection::ServiceCollection& GetServices() { return m_services; }

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<WebApplication> Build();

            private:
                DotNetDupe::Extensions::DependencyInjection::ServiceCollection m_services;
            };

        }
    }
}
