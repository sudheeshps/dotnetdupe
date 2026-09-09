#include "pch.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {

            /// \brief Default constructor.
            WebApplicationBuilder::WebApplicationBuilder() = default;

            /// \brief Move constructor.
            WebApplicationBuilder::WebApplicationBuilder(WebApplicationBuilder&& other) noexcept
                : m_services(std::move(other.m_services)),
                  m_controllerRegistrars(std::move(other.m_controllerRegistrars)) {}

            /// \brief Move assignment operator.
            WebApplicationBuilder& WebApplicationBuilder::operator=(WebApplicationBuilder&& other) noexcept {
                /// Guard: Check for self-assignment.
                if (this != &other) {
                    /// Move: Transfer registered services and controller route registrars.
                    m_services = std::move(other.m_services);
                    m_controllerRegistrars = std::move(other.m_controllerRegistrars);
                }
                /// Return result: Return self reference.
                return *this;
            }

            /// \brief Builds the application instance with configured services and controllers.
            System::SmartPointer<WebApplication> WebApplicationBuilder::Build() {
                /// Initialize: Build root service provider from registered service collection.
                auto spProvider = m_services.BuildServiceProvider();

                /// Construct: Instantiate WebApplication instance with resolved IoC provider.
                auto spApp = System::SmartPointer<WebApplication>::NewShared(spProvider);

                /// Bind: Set self weak reference and forward controller route registrars.
                spApp->SetSelfPointer(spApp);
                spApp->SetControllerRegistrars(std::move(m_controllerRegistrars));

                /// Return result: Return configured WebApplication host instance.
                return spApp;
            }

        }
    }
}
