#include "pch.h"
#include "gtest/gtest.h"
#include "System/SmartPointer.h"
#include "System/SystemException.h"
#include "System/Version.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    
    // Helper class to track destruction
    class DestructionTracker {
    public:
        static int s_iDestructionCount;
        DestructionTracker() { }
        ~DestructionTracker() { s_iDestructionCount++; }
        void DoSomething() {}
    };

    int DestructionTracker::s_iDestructionCount = 0;

    class SmartPointerImprovisationTest : public ::testing::Test {
    protected:
        void SetUp() override {
            DestructionTracker::s_iDestructionCount = 0;
        }
    };

    TEST_F(SmartPointerImprovisationTest, DefaultConstructor_AllocatesConcreteType) {
        // Given / When
        {
            SmartPointer<DestructionTracker> pSmart;
            
            // Then
            ASSERT_FALSE(pSmart.IsNull());
            ASSERT_EQ(pSmart.GetRefCount(), 0);
        }
        ASSERT_EQ(DestructionTracker::s_iDestructionCount, 1);
    }

    TEST_F(SmartPointerImprovisationTest, SharedConstructor_AllocatesAndEnablesSharing) {
        // Given / When
        {
            SmartPointer<DestructionTracker> pSmart1(true);
            ASSERT_EQ(pSmart1.GetRefCount(), 1);

            {
                SmartPointer<DestructionTracker> pSmart2 = pSmart1;
                ASSERT_EQ(pSmart1.GetRefCount(), 2);
            }
            ASSERT_EQ(pSmart1.GetRefCount(), 1);
        }
        ASSERT_EQ(DestructionTracker::s_iDestructionCount, 1);
    }

    TEST_F(SmartPointerImprovisationTest, NewStaticHelper_SupportsConstructorArgs) {
        // Given / When
        auto pVersion = SmartPointer<Version>::New(2, 5, 0);

        // Then
        ASSERT_EQ(pVersion->GetMajor(), 2);
        ASSERT_EQ(pVersion->GetMinor(), 5);
        ASSERT_EQ(pVersion->GetBuild(), 0);
    }

    TEST_F(SmartPointerImprovisationTest, NewSharedStaticHelper_SupportsConstructorArgsAndSharing) {
        // Given / When
        auto pVersion1 = SmartPointer<Version>::NewShared(1, 2, 3);
        
        // Then
        ASSERT_EQ(pVersion1.GetRefCount(), 1);
        ASSERT_EQ(pVersion1->GetMajor(), 1);
        
        {
            auto pVersion2 = pVersion1;
            ASSERT_EQ(pVersion1.GetRefCount(), 2);
            ASSERT_EQ(pVersion2.GetRefCount(), 2);
        }
        ASSERT_EQ(pVersion1.GetRefCount(), 1);
    }

    TEST_F(SmartPointerImprovisationTest, NullptrConstructor_InitializesToNull) {
        // Given / When
        SmartPointer<DestructionTracker> pSmart(nullptr);

        // Then
        ASSERT_TRUE(pSmart.IsNull());
    }

    // Note: This test verifies that abstract classes don't auto-allocate
    // (We use a dummy abstract class for testing if needed, but TextReader is a good example)
    class AbstractBase { public: virtual void Pure() = 0; virtual ~AbstractBase() {} };

    TEST_F(SmartPointerImprovisationTest, DefaultConstructor_DoesNotAllocateAbstractType) {
        // Given / When
        SmartPointer<AbstractBase> pSmart;

        // Then
        ASSERT_TRUE(pSmart.IsNull());
    }
}
