#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/InvalidOperationException.h"
#include <typeindex>

namespace DotNetDupe {
    namespace System {
        /// \brief Defines a mechanism for retrieving a service object; that is, an object that provides custom support to other objects.
        /// \details Modeled after System.IServiceProvider in .NET Base Class Library (ECMA-335).
        /// Serves as the base resolution contract implemented by IoC containers across DotNetDupe.
        /// \cite ECMA-335
        class IServiceProvider : public Object {
        public:
            /// \brief Virtual destructor for polymorphic interface cleanup.
            virtual ~IServiceProvider() = default;

            /// \brief Gets the service object of the specified type.
            /// \param serviceType An object that specifies the type of service object to get.
            /// \return A service object of type serviceType, or null if there is no service object of type serviceType.
            virtual SmartPointer<Object> GetService(const std::type_index& serviceType) = 0;

            /// \brief Gets the service object of the specified generic type T.
            /// \tparam T The type of service object to get.
            /// \return A service object of type T, or null if no such service is registered.
            template <typename T>
            SmartPointer<T> GetService() {
                SmartPointer<Object> spObj = GetService(typeid(T));
                if (spObj.IsNull()) {
                    return SmartPointer<T>(nullptr);
                }
                return spObj.template DynamicCast<T>();
            }

            /// \brief Gets service of type T, throwing InvalidOperationException if not found.
            /// \tparam T The type of service object to get.
            /// \return A service object of type T.
            /// \throws InvalidOperationException Thrown if the service cannot be resolved.
            template <typename T>
            SmartPointer<T> GetRequiredService() {
                SmartPointer<T> spService = GetService<T>();
                if (spService.IsNull()) {
                    throw InvalidOperationException("Required service not registered.");
                }
                return spService;
            }
        };
    }
}
