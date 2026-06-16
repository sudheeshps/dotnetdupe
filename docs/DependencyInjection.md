### Dependency Injection

Provides a lightweight, type-safe Inversion of Control (IoC) container supporting dependency registration, lifetime management (Singleton, Scoped, and Transient), scope isolation, and automatic `IDisposable` tracking and cleanup.

#### Namespaces
- `DotNetDupe::System` (for `IServiceProvider`)
- `DotNetDupe::Extensions::DependencyInjection` (for DI builder and lifetimes)

---

### Key Interfaces and Classes

#### `interface IServiceProvider`
Defines a mechanism for retrieving a service object (type-safe dependency resolution).

##### Methods
- `template <typename T> SmartPointer<T> GetService()`: Resolves a service of type `T`. Returns `nullptr` if not registered.
- `template <typename T> SmartPointer<T> GetRequiredService()`: Resolves a service of type `T`. Throws `InvalidOperationException` if not registered.

---

#### `class ServiceCollection`
Accumulates service descriptors and builds the service provider.

##### Methods
- `template <typename TService, typename TImplementation> ServiceCollection& AddSingleton()`: Registers a singleton service.
- `template <typename TService> ServiceCollection& AddSingleton(ServiceDescriptor::FactoryType fnFactory)`: Registers a singleton service using a custom lambda factory.
- `template <typename TService, typename TImplementation> ServiceCollection& AddScoped()`: Registers a scoped service.
- `template <typename TService> ServiceCollection& AddScoped(ServiceDescriptor::FactoryType fnFactory)`: Registers a scoped service using a custom lambda factory.
- `template <typename TService, typename TImplementation> ServiceCollection& AddTransient()`: Registers a transient service.
- `template <typename TService> ServiceCollection& AddTransient(ServiceDescriptor::FactoryType fnFactory)`: Registers a transient service using a custom lambda factory.
- `SmartPointer<IServiceProvider> BuildServiceProvider()`: Compiles the registrations and returns the root service provider.

---

#### `interface IServiceScope`
Tracks the lifetime of scoped and transient services resolved within its boundary. When disposed, all tracked services that implement `System::IO::IDisposable` are automatically disposed.

##### Methods
- `SmartPointer<IServiceProvider> GetServiceProvider() const`: Gets the scoped service provider instance.
- `void Dispose()`: Disposes the scope and all resolved resources within it.

---

#### `interface IServiceScopeFactory`
Used to create new `IServiceScope` instances.

##### Methods
- `SmartPointer<IServiceScope> CreateScope()`: Creates a new lifetime scope.

---

### Lifetime Summary

| Lifetime | Description | Cleanup |
| :--- | :--- | :--- |
| **Singleton** | A single instance is created once and shared globally. | Disposed when the root `ServiceProvider` is disposed. |
| **Scoped** | A single instance is created per lifetime scope. | Disposed when the active `IServiceScope` is disposed. |
| **Transient** | A new instance is created every time it is resolved. | Disposed when the active `IServiceScope` or resolving `ServiceProvider` is disposed. |

---

### Complete, Compile-Ready Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::DependencyInjection;

// 1. Define Interfaces and Implementations
class IDatabase : public virtual Object, public System::IO::IDisposable {
public:
    virtual ~IDatabase() = default;
    virtual void ExecuteQuery(const char* query) = 0;
};

class SqlDatabase : public IDatabase {
public:
    SqlDatabase() {
        Console::WriteLine("SqlDatabase Connection Opened.");
    }
    ~SqlDatabase() override {
        Console::WriteLine("SqlDatabase Destructed.");
    }
    void ExecuteQuery(const char* query) override {
        Console::Write("Executing query: ");
        Console::WriteLine(query);
    }
    void Dispose() override {
        Console::WriteLine("SqlDatabase Connection Closed (Disposed).");
    }
};

class IService : public virtual Object {
public:
    virtual ~IService() = default;
    virtual void Run() = 0;
};

class MyService : public IService {
public:
    MyService(SmartPointer<IDatabase> spDb) : m_spDb(spDb) {}
    
    void Run() override {
        m_spDb->ExecuteQuery("SELECT * FROM Users;");
    }
private:
    SmartPointer<IDatabase> m_spDb;
};

int main() {
    ServiceCollection services;

    // Register SqlDatabase as Scoped (one connection per scope)
    services.AddScoped<IDatabase, SqlDatabase>();

    // Register MyService as Transient, injecting IDatabase resolved from the provider
    services.AddTransient<IService>([](const SmartPointer<IServiceProvider>& sp) {
        auto spDb = sp->GetRequiredService<IDatabase>();
        return SmartPointer<Object>(SmartPointer<MyService>::NewShared(spDb));
    });

    // Build the root service provider
    auto spRootProvider = services.BuildServiceProvider();

    Console::WriteLine("--- Entering Scope 1 ---");
    {
        auto spScopeFactory = spRootProvider->GetRequiredService<IServiceScopeFactory>();
        auto spScope = spScopeFactory->CreateScope();
        auto spScopedProvider = spScope->GetServiceProvider();

        auto spService = spScopedProvider->GetRequiredService<IService>();
        spService->Run();
    } // Scope 1 ends here; SqlDatabase is automatically Disposed and Closed.
    Console::WriteLine("--- Exited Scope 1 ---\n");

    return 0;
}
```
