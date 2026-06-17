#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/InvalidOperationException.h"
#include <typeindex>

namespace DotNetDupe {
    namespace System {
        class IServiceProvider : public Object {
        public:
            virtual ~IServiceProvider() = default;
            virtual SmartPointer<Object> GetService(const std::type_index& serviceType) = 0;

            template <typename T>
            SmartPointer<T> GetService() {
                SmartPointer<Object> spObj = GetService(typeid(T));
                if (spObj.IsNull()) {
                    return SmartPointer<T>(nullptr);
                }
                return spObj.template DynamicCast<T>();
            }

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
