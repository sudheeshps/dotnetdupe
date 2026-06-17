#include "pch.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {

            WebApplicationBuilder::WebApplicationBuilder() = default;

            WebApplicationBuilder::WebApplicationBuilder(WebApplicationBuilder&& other) noexcept
                : m_services(std::move(other.m_services)),
                  m_controllerRegistrars(std::move(other.m_controllerRegistrars)) {}

            WebApplicationBuilder& WebApplicationBuilder::operator=(WebApplicationBuilder&& other) noexcept {
                if (this != &other) {
                    m_services = std::move(other.m_services);
                    m_controllerRegistrars = std::move(other.m_controllerRegistrars);
                }
                return *this;
            }

            System::SmartPointer<WebApplication> WebApplicationBuilder::Build() {
                auto spProvider = m_services.BuildServiceProvider();
                auto spApp = System::SmartPointer<WebApplication>::NewShared(spProvider);
                spApp->SetSelfPointer(spApp);
                spApp->SetControllerRegistrars(std::move(m_controllerRegistrars));
                return spApp;
            }

        }
    }
}
