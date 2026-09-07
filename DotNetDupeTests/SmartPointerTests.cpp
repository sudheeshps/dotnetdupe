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
    class AbstractBase { public: virtual void Pure() = 0; virtual ~AbstractBase() {} };

    TEST_F(SmartPointerImprovisationTest, DefaultConstructor_DoesNotAllocateAbstractType) {
        // Given / When
        SmartPointer<AbstractBase> pSmart;

        // Then
        ASSERT_TRUE(pSmart.IsNull());
    }

    // --- Nullptr & Raw Pointer Comparison Tests ---

    TEST_F(SmartPointerImprovisationTest, NullptrComparisons) {
        SmartPointer<DestructionTracker> pNull(nullptr);
        SmartPointer<DestructionTracker> pValid;

        // Comparisons with nullptr literal
        ASSERT_TRUE(pNull == nullptr);
        ASSERT_TRUE(nullptr == pNull);
        ASSERT_FALSE(pNull != nullptr);
        ASSERT_FALSE(nullptr != pNull);

        ASSERT_FALSE(pValid == nullptr);
        ASSERT_FALSE(nullptr == pValid);
        ASSERT_TRUE(pValid != nullptr);
        ASSERT_TRUE(nullptr != pValid);
    }

    TEST_F(SmartPointerImprovisationTest, RawPointerComparisons) {
        auto* pRaw = new DestructionTracker();
        SmartPointer<DestructionTracker> pSmart(pRaw);

        ASSERT_TRUE(pSmart == pRaw);
        ASSERT_TRUE(pRaw == pSmart);
        ASSERT_FALSE(pSmart != pRaw);
        ASSERT_FALSE(pRaw != pSmart);

        DestructionTracker other;
        ASSERT_TRUE(pSmart != &other);
        ASSERT_TRUE(&other != pSmart);
        ASSERT_FALSE(pSmart == &other);
    }

    // --- SFINAE and Overload Disambiguation Tests ---

    class BaseA { public: virtual ~BaseA() = default; };
    class BaseB { public: virtual ~BaseB() = default; };
    class DerivedA : public BaseA { public: virtual ~DerivedA() = default; };
    class DerivedB : public BaseB { public: virtual ~DerivedB() = default; };

    static int OverloadTester(SmartPointer<BaseA>) { return 1; }
    static int OverloadTester(SmartPointer<BaseB>) { return 2; }

    TEST_F(SmartPointerImprovisationTest, SFINAEConstrainedConvertingConstructor) {
        // Converting constructor should work for related hierarchy
        auto pDerived = SmartPointer<DerivedA>::NewShared();
        SmartPointer<BaseA> pBase = pDerived;
        ASSERT_TRUE(pBase != nullptr);
        ASSERT_EQ(pDerived.GetRefCount(), 2);

        // Disambiguated function overloading without C2668 ambiguity
        ASSERT_EQ(OverloadTester(pDerived), 1);

        auto pDerivedB = SmartPointer<DerivedB>::NewShared();
        ASSERT_EQ(OverloadTester(pDerivedB), 2);
    }

    // --- DynamicCast & StaticCast Tests ---

    class PolyBase { public: virtual ~PolyBase() = default; virtual int GetId() { return 1; } };
    class PolyDerived : public PolyBase { public: int GetId() override { return 2; } };

    TEST_F(SmartPointerImprovisationTest, DynamicAndStaticCasting) {
        auto pDerived = SmartPointer<PolyDerived>::NewShared();
        SmartPointer<PolyBase> pBase = pDerived;
        ASSERT_EQ(pBase.GetRefCount(), 2);

        // Member DynamicCast
        auto pCasted = pBase.DynamicCast<PolyDerived>();
        ASSERT_TRUE(pCasted != nullptr);
        ASSERT_EQ(pCasted->GetId(), 2);
        ASSERT_EQ(pBase.GetRefCount(), 3);

        // Static DynamicCast
        auto pStaticCasted = SmartPointer<PolyDerived>::DynamicCast(pBase);
        ASSERT_TRUE(pStaticCasted != nullptr);
        ASSERT_EQ(pStaticCasted->GetId(), 2);

        // Free function DynamicPointerCast
        auto pFreeCasted = DynamicPointerCast<PolyDerived>(pBase);
        ASSERT_TRUE(pFreeCasted != nullptr);
        ASSERT_EQ(pFreeCasted->GetId(), 2);

        // Free function DynamicCast
        auto pFreeCast2 = DynamicCast<PolyDerived>(pBase);
        ASSERT_TRUE(pFreeCast2 != nullptr);
        ASSERT_EQ(pFreeCast2->GetId(), 2);

        // StaticPointerCast
        auto pStat = StaticPointerCast<PolyDerived>(pBase);
        ASSERT_TRUE(pStat != nullptr);
        ASSERT_EQ(pStat->GetId(), 2);
    }

    // --- EnableSharedFromThis Tests ---

    class SharedSelfTracker : public EnableSharedFromThis<SharedSelfTracker> {
    public:
        static int s_iDestructCount;
        int m_val{42};
        SharedSelfTracker() = default;
        ~SharedSelfTracker() { s_iDestructCount++; }

        SmartPointer<SharedSelfTracker> GetSelf() {
            return SharedFromThis();
        }
    };

    int SharedSelfTracker::s_iDestructCount = 0;

    TEST_F(SmartPointerImprovisationTest, EnableSharedFromThis_SharesOwnershipProperly) {
        SharedSelfTracker::s_iDestructCount = 0;
        {
            auto p1 = SmartPointer<SharedSelfTracker>::NewShared();
            ASSERT_EQ(p1.GetRefCount(), 1);

            {
                auto p2 = p1->GetSelf();
                ASSERT_EQ(p1.GetRefCount(), 2);
                ASSERT_EQ(p2.GetRefCount(), 2);
                ASSERT_EQ(p2->m_val, 42);
                ASSERT_TRUE(p1 == p2);
            }
            ASSERT_EQ(p1.GetRefCount(), 1);
            ASSERT_EQ(SharedSelfTracker::s_iDestructCount, 0);
        }
        ASSERT_EQ(SharedSelfTracker::s_iDestructCount, 1);
    }

    TEST_F(SmartPointerImprovisationTest, EnableSharedFromThis_ThrowsWhenNotShared) {
        SharedSelfTracker unmanaged;
        ASSERT_THROW(unmanaged.SharedFromThis(), SystemException);

        SmartPointer<SharedSelfTracker> pUnique; // default is unique ownership
        ASSERT_THROW(pUnique->SharedFromThis(), SystemException);
    }
}
