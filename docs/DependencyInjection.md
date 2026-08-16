# DependencyInjection (ServiceCollection &amp; ServiceProvider)

**Namespace:** `DotNetDupe::Extensions::DependencyInjection`  
**Header:** `#include "Extensions/DependencyInjection/ServiceCollection.h"`, `#include "Extensions/DependencyInjection/ServiceProvider.h"`

Provides an Inversion of Control (IoC) container supporting Transient, Scoped, and Singleton service lifetimes, factory delegates, and hierarchical scoped resolution.

---

## `ServiceLifetime` Enum

```cpp
enum class ServiceLifetime {
    Singleton,
    Scoped,
    Transient
};
```

---

## `ServiceCollection` / `IServiceCollection`

Represents a collection of service descriptors.

### Registration Methods

#### Singleton Registrations
- `AddSingleton<TService, TImplementation>()`: Registers a singleton service resolved once per root container.
- `AddSingleton<TService>(factoryFn)`: Registers a singleton with a factory lambda.
- `AddSingleton<TService>(instancePtr)`: Registers a pre-existing singleton instance.

#### Transient Registrations
- `AddTransient<TService, TImplementation>()`: Creates a new instance on every resolution request.
- `AddTransient<TService>(factoryFn)`: Registers transient creation via factory delegate.

#### Scoped Registrations
- `AddScoped<TService, TImplementation>()`: Creates a single instance per `IServiceScope`.
- `AddScoped<TService>(factoryFn)`: Registers scoped creation via factory delegate.

---

## `ServiceProvider` / `IServiceProvider`

Provides resolution of registered service dependencies.

### Methods
- `template <typename T> SmartPointer<T> GetService()`: Resolves a registered service object of type `T`.
- `SmartPointer<Object> GetService(const std::type_index& serviceType) override`: Low-level resolution.
- `void Dispose() override`: Disposes all resolved singleton instances and clears cached references.

---

## `ServiceScope` & `ServiceScopeFactory`

- `SmartPointer<IServiceScope> CreateScope()`: Creates a new child scope where Scoped services are isolated.
- `SmartPointer<IServiceProvider> GetServiceProvider() const`: Accesses the scoped container.

---

## Example

```cpp
#include "System/Console.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::DependencyInjection;

class IGreeter : public Object {
public:
    virtual void Greet(const String& name) = 0;
};

class ConsoleGreeter : public IGreeter {
public:
    void Greet(const String& name) override {
        Console::WriteLine("Hello, {0}!", name);
    }
};

int main() {
    ServiceCollection services;
    services.AddTransient<IGreeter, ConsoleGreeter>();

    auto spProvider = services.BuildServiceProvider();

    auto spGreeter = spProvider->GetService<IGreeter>();
    if (spGreeter != nullptr) {
        spGreeter->Greet("Developer");
    }

    return 0;
}
```
