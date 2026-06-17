#include "pch.h"
#include "gtest/gtest.h"
#include "System/SmartPointer.h"
#include "System/InvalidOperationException.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::DependencyInjection;

namespace DependencyInjectionTests {

    class IMyService : public virtual Object {
    public:
        virtual ~IMyService() = default;
        virtual void DoWork() = 0;
    };

    class MyService : public IMyService {
    public:
        void DoWork() override {}
    };

    class IDisposableService : public virtual Object, public DotNetDupe::System::IO::IDisposable {
    public:
        static int s_iDisposeCount;
        IDisposableService() {}
        ~IDisposableService() override = default;
        void Dispose() override { s_iDisposeCount++; }
    };
    int IDisposableService::s_iDisposeCount = 0;

    class INestedService : public virtual Object {
    public:
        virtual ~INestedService() = default;
        virtual SmartPointer<IMyService> GetMyService() = 0;
    };

    class NestedService : public INestedService {
    public:
        NestedService(SmartPointer<IMyService> spMyService) : m_spMyService(std::move(spMyService)) {}
        SmartPointer<IMyService> GetMyService() override { return m_spMyService; }
    private:
        SmartPointer<IMyService> m_spMyService;
    };

    class DependencyInjectionTest : public ::testing::Test {
    protected:
        void SetUp() override {
            IDisposableService::s_iDisposeCount = 0;
        }
    };

    TEST_F(DependencyInjectionTest, GivenSingletonRegistered_WhenResolved_ReturnsSameInstance) {
        // Given
        ServiceCollection services;
        services.AddSingleton<IMyService, MyService>();

        // When
        auto spProvider = services.BuildServiceProvider();
        auto spService1 = spProvider->GetService<IMyService>();
        auto spService2 = spProvider->GetService<IMyService>();

        // Then
        ASSERT_FALSE(spService1.IsNull());
        ASSERT_FALSE(spService2.IsNull());
        ASSERT_EQ(spService1.Get(), spService2.Get());
    }

    TEST_F(DependencyInjectionTest, GivenTransientRegistered_WhenResolved_ReturnsDifferentInstances) {
        // Given
        ServiceCollection services;
        services.AddTransient<IMyService, MyService>();

        // When
        auto spProvider = services.BuildServiceProvider();
        auto spService1 = spProvider->GetService<IMyService>();
        auto spService2 = spProvider->GetService<IMyService>();

        // Then
        ASSERT_FALSE(spService1.IsNull());
        ASSERT_FALSE(spService2.IsNull());
        ASSERT_NE(spService1.Get(), spService2.Get());
    }

    TEST_F(DependencyInjectionTest, GivenScopedRegistered_WhenResolvedFromRoot_ThrowsException) {
        // Given
        ServiceCollection services;
        services.AddScoped<IMyService, MyService>();

        // When
        auto spProvider = services.BuildServiceProvider();

        // Then
        ASSERT_THROW({
            spProvider->GetService<IMyService>();
        }, InvalidOperationException);
    }

    TEST_F(DependencyInjectionTest, GivenScopedRegistered_WhenResolvedWithinScope_ReturnsSameInstance) {
        // Given
        ServiceCollection services;
        services.AddScoped<IMyService, MyService>();

        // When
        auto spProvider = services.BuildServiceProvider();
        auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
        auto spScope = spScopeFactory->CreateScope();
        auto spScopedProvider = spScope->GetServiceProvider();

        auto spService1 = spScopedProvider->GetService<IMyService>();
        auto spService2 = spScopedProvider->GetService<IMyService>();

        // Then
        ASSERT_FALSE(spService1.IsNull());
        ASSERT_FALSE(spService2.IsNull());
        ASSERT_EQ(spService1.Get(), spService2.Get());
    }

    TEST_F(DependencyInjectionTest, GivenScopedRegistered_WhenResolvedInDifferentScopes_ReturnsDifferentInstances) {
        // Given
        ServiceCollection services;
        services.AddScoped<IMyService, MyService>();

        // When
        auto spProvider = services.BuildServiceProvider();
        auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
        
        auto spScope1 = spScopeFactory->CreateScope();
        auto spService1 = spScope1->GetServiceProvider()->GetService<IMyService>();

        auto spScope2 = spScopeFactory->CreateScope();
        auto spService2 = spScope2->GetServiceProvider()->GetService<IMyService>();

        // Then
        ASSERT_FALSE(spService1.IsNull());
        ASSERT_FALSE(spService2.IsNull());
        ASSERT_NE(spService1.Get(), spService2.Get());
    }

    TEST_F(DependencyInjectionTest, GivenNestedDependencies_WhenResolved_InjectsSuccessfully) {
        // Given
        ServiceCollection services;
        services.AddTransient<IMyService, MyService>();
        services.AddTransient<INestedService>([](const SmartPointer<IServiceProvider>& sp) {
            auto spMyService = sp->GetRequiredService<IMyService>();
            return SmartPointer<Object>(SmartPointer<NestedService>::NewShared(spMyService));
        });

        // When
        auto spProvider = services.BuildServiceProvider();
        auto spNested = spProvider->GetRequiredService<INestedService>();

        // Then
        ASSERT_FALSE(spNested.IsNull());
        ASSERT_FALSE(spNested->GetMyService().IsNull());
    }

    TEST_F(DependencyInjectionTest, GivenDisposableSingleton_WhenProviderDisposed_DisposesSingleton) {
        // Given
        ServiceCollection services;
        services.AddSingleton<IDisposableService, IDisposableService>();

        // When
        {
            auto spProvider = services.BuildServiceProvider();
            auto spService = spProvider->GetRequiredService<IDisposableService>();
            ASSERT_EQ(IDisposableService::s_iDisposeCount, 0);
        } // Provider goes out of scope and is disposed here

        // Then
        ASSERT_EQ(IDisposableService::s_iDisposeCount, 1);
    }

    TEST_F(DependencyInjectionTest, GivenDisposableScoped_WhenScopeDisposed_DisposesScoped) {
        // Given
        ServiceCollection services;
        services.AddScoped<IDisposableService, IDisposableService>();

        // When
        auto spProvider = services.BuildServiceProvider();
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spService = spScope->GetServiceProvider()->GetRequiredService<IDisposableService>();
            ASSERT_EQ(IDisposableService::s_iDisposeCount, 0);
        } // Scope goes out of scope and is disposed here

        // Then
        ASSERT_EQ(IDisposableService::s_iDisposeCount, 1);
    }

    TEST_F(DependencyInjectionTest, GivenUnregisteredService_WhenGetRequiredServiceCalled_ThrowsException) {
        // Given
        ServiceCollection services;
        auto spProvider = services.BuildServiceProvider();

        // When / Then
        ASSERT_THROW({
            spProvider->GetRequiredService<IMyService>();
        }, InvalidOperationException);
    }
}
