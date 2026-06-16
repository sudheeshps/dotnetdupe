#include "System/IServiceProvider.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"
#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

namespace DependencyInjectionDemo {
    class IDatabaseConnection : public virtual DotNetDupe::System::Object, public DotNetDupe::System::IO::IDisposable {
    public:
        virtual ~IDatabaseConnection() = default;
        virtual void Query(const char* query) = 0;
    };

    class SqlDatabaseConnection : public IDatabaseConnection {
    public:
        SqlDatabaseConnection() {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Constructed.");
        }
        ~SqlDatabaseConnection() override {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Destructed.");
        }
        void Query(const char* query) override {
            DotNetDupe::System::Console::Write("  [SqlDatabaseConnection] Executing query: ");
            DotNetDupe::System::Console::WriteLine(query);
        }
        void Dispose() override {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Disposed.");
        }
    };

    class IRepository : public virtual DotNetDupe::System::Object {
    public:
        virtual ~IRepository() = default;
        virtual void SaveData(const char* data) = 0;
    };

    class SqlRepository : public IRepository {
    public:
        SqlRepository(DotNetDupe::System::SmartPointer<IDatabaseConnection> spConn) : m_spConn(spConn) {
            DotNetDupe::System::Console::WriteLine("  [SqlRepository] Constructed.");
        }
        ~SqlRepository() override {
            DotNetDupe::System::Console::WriteLine("  [SqlRepository] Destructed.");
        }
        void SaveData(const char* data) override {
            DotNetDupe::System::Console::Write("  [SqlRepository] Saving data: ");
            DotNetDupe::System::Console::WriteLine(data);
            m_spConn->Query("INSERT INTO Data VALUES (...)");
        }
    private:
        DotNetDupe::System::SmartPointer<IDatabaseConnection> m_spConn;
    };
}

void DemonstrateDependencyInjection() {
    using namespace DotNetDupe::System;
    using namespace DotNetDupe::Extensions::DependencyInjection;
    using namespace DependencyInjectionDemo;

    Console::WriteLine("\n=== Dependency Injection Demonstration ===");
    try {
        ServiceCollection services;

        // Register IDatabaseConnection as Scoped
        services.AddScoped<IDatabaseConnection, SqlDatabaseConnection>();

        // Register IRepository as Transient, using a lambda factory to inject IDatabaseConnection
        services.AddTransient<IRepository>([](const SmartPointer<IServiceProvider>& sp) {
            auto spConn = sp->GetRequiredService<IDatabaseConnection>();
            return SmartPointer<Object>(SmartPointer<SqlRepository>::NewShared(spConn));
        });

        Console::WriteLine("Building ServiceProvider...");
        auto spProvider = services.BuildServiceProvider();

        Console::WriteLine("\n--- Scope 1 Boundary ---");
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spScopedProvider = spScope->GetServiceProvider();

            Console::WriteLine("Resolving repository in Scope 1...");
            auto spRepo1 = spScopedProvider->GetRequiredService<IRepository>();
            spRepo1->SaveData("User1");

            Console::WriteLine("Resolving repository in Scope 1 again...");
            auto spRepo2 = spScopedProvider->GetRequiredService<IRepository>();
            spRepo2->SaveData("User2");
        } // Scope 1 disposed, SqlDatabaseConnection should be disposed

        Console::WriteLine("\n--- Scope 2 Boundary ---");
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spScopedProvider = spScope->GetServiceProvider();

            Console::WriteLine("Resolving repository in Scope 2...");
            auto spRepo = spScopedProvider->GetRequiredService<IRepository>();
            spRepo->SaveData("User3");
        } // Scope 2 disposed, SqlDatabaseConnection should be disposed

    } catch (const BasicException<char>& ex) {
        Console::Write("Error during Dependency Injection demonstration: ");
        Console::WriteLine(ex.What());
    }
    Console::WriteLine("==========================================");
}
