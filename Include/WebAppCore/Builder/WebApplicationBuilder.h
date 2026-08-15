#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Controllers/ControllerRouteBuilder.h"

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

                template <typename TController>
                DotNetDupe::WebAppCore::Controllers::ControllerRouteBuilder<TController>& AddController(const DotNetDupe::System::String& prefix) {
                    // Create route builder
                    auto routeBuilder = DotNetDupe::System::SmartPointer<DotNetDupe::WebAppCore::Controllers::ControllerRouteBuilder<TController>>::NewShared(prefix);
                    auto& ref = *routeBuilder;

                    // Capture lambda to bind routes later on WebApplication
                    m_controllerRegistrars.Add([routeBuilder](const DotNetDupe::System::SmartPointer<WebApplication>& app) {
                        routeBuilder->Register(app);
                    });

                    return ref;
                }

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<WebApplication> Build();

            private:
                DotNetDupe::Extensions::DependencyInjection::ServiceCollection m_services;
                DotNetDupe::System::Collections::Generic::List<WebApplication::ControllerRegistrar> m_controllerRegistrars;
            };

        }
    }
}
