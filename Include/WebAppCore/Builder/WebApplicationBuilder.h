/// \file WebApplicationBuilder.h
/// \brief Builder pattern orchestrator for configuring services, controllers, and constructing WebApplication instances.

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

            /// \brief Facilitates initialization and configuration of dependency injection services, controllers, and web application hosting.
            ///
            /// Mirrors the ASP.NET Core `WebApplicationBuilder` hosting model, providing a unified container
            /// for registering IoC service dependencies and dispatching MVC/REST controller routes.
            ///
            /// \note Thread Safety: Instance methods are not thread-safe and must be invoked during application bootstrap before calling Build().
            /// \note Conforms to ASP.NET Core minimal hosting specifications and RFC 9110 HTTP architecture.
            /// \see WebApplication, ServiceCollection, ControllerRouteBuilder
            class WebApplicationBuilder : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Initializes a new instance of the WebApplicationBuilder class.
                DOTNETDUPE_API WebApplicationBuilder();

                /// \brief Destructor for WebApplicationBuilder.
                DOTNETDUPE_API ~WebApplicationBuilder() override = default;

                WebApplicationBuilder(const WebApplicationBuilder&) = delete;
                WebApplicationBuilder& operator=(const WebApplicationBuilder&) = delete;

                /// \brief Move constructor transferring service collections and controller route registrars.
                /// \param other WebApplicationBuilder rvalue to move from.
                DOTNETDUPE_API WebApplicationBuilder(WebApplicationBuilder&& other) noexcept;

                /// \brief Move assignment operator.
                /// \param other WebApplicationBuilder rvalue to move from.
                /// \return Reference to this builder instance.
                DOTNETDUPE_API WebApplicationBuilder& operator=(WebApplicationBuilder&& other) noexcept;

                /// \brief Gets the mutable service collection for dependency injection registrations.
                /// \return Reference to the internal ServiceCollection.
                DOTNETDUPE_API DotNetDupe::Extensions::DependencyInjection::ServiceCollection& GetServices() { return m_services; }

                /// \brief Registers an MVC/REST controller and configures its route endpoints under the given URI prefix.
                /// \tparam TController Controller class deriving from ControllerBase.
                /// \param prefix Base URI path prefix (e.g., "/api/users").
                /// \return Reference to the configured ControllerRouteBuilder for endpoint chaining.
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

                /// \brief Builds the dependency injection service provider and constructs the configured WebApplication host.
                /// \return SmartPointer to the constructed WebApplication instance.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<WebApplication> Build();

            private:
                DotNetDupe::Extensions::DependencyInjection::ServiceCollection m_services;
                DotNetDupe::System::Collections::Generic::List<WebApplication::ControllerRegistrar> m_controllerRegistrars;
            };

        }
    }
}
