#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Controllers/ControllerRouteBuilder.h"
#include <vector>
#include <functional>

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
                    // Register the controller class in the DI container
                    m_services.AddTransient<TController, TController>();

                    // Create route builder
                    auto routeBuilder = DotNetDupe::System::SmartPointer<DotNetDupe::WebAppCore::Controllers::ControllerRouteBuilder<TController>>::NewShared(prefix);
                    auto& ref = *routeBuilder;

                    // Capture lambda to bind routes later on WebApplication
                    m_controllerRegistrars.push_back([routeBuilder](const DotNetDupe::System::SmartPointer<WebApplication>& app) {
                        routeBuilder->Register(app);
                    });

                    return ref;
                }

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<WebApplication> Build();

            private:
                DotNetDupe::Extensions::DependencyInjection::ServiceCollection m_services;
                std::vector<std::function<void(const DotNetDupe::System::SmartPointer<WebApplication>&)>> m_controllerRegistrars;
            };

        }
    }
}
