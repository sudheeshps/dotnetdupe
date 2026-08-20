#include "pch.h"
#include "gtest/gtest.h"
#include "System/EventArgs.h"
#include "System/EventHandler.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"
#include <cmath>

using namespace DotNetDupe::System;

namespace SystemTests {

    // Custom EventArgs class for testing
    class TemperatureChangedEventArgs : public EventArgs {
    public:
        TemperatureChangedEventArgs(double dOldTemp, double dNewTemp, const String& sLocation)
            : m_dOldTemperature(dOldTemp), m_dNewTemperature(dNewTemp), m_sLocation(sLocation) {}

        double GetOldTemperature() const { return m_dOldTemperature; }
        double GetNewTemperature() const { return m_dNewTemperature; }
        String GetLocation() const { return m_sLocation; }

    private:
        double m_dOldTemperature;
        double m_dNewTemperature;
        String m_sLocation;
    };

    // Test helper publisher class
    class Thermostat : public virtual Object {
    public:
        EventHandler<TemperatureChangedEventArgs> TemperatureChanged;
        EventHandler<> PowerStateChanged;

        Thermostat(double dInitialTemp) : m_dCurrentTemperature(dInitialTemp), m_bPoweredOn(true) {}

        void SetTemperature(double dNewTemp) {
            constexpr double dTemperatureEpsilon = 1e-9;
            if (std::fabs(dNewTemp - m_dCurrentTemperature) > dTemperatureEpsilon) {
                double dOld = m_dCurrentTemperature;
                m_dCurrentTemperature = dNewTemp;
                TemperatureChangedEventArgs args(dOld, dNewTemp, "Living Room");
                TemperatureChanged.Invoke(this, args);
            }
        }

        void TogglePower() {
            m_bPoweredOn = !m_bPoweredOn;
            PowerStateChanged(this, EventArgs::Empty());
        }

        double GetTemperature() const { return m_dCurrentTemperature; }
        bool IsPoweredOn() const { return m_bPoweredOn; }

    private:
        double m_dCurrentTemperature;
        bool m_bPoweredOn;
    };

    // Test subscriber class for member method binding
    class LoggerSubscriber {
    public:
        int m_nLogCount = 0;
        double m_dLastLoggedTemp = 0.0;

        void OnTemperatureChanged(const void* pSender, const TemperatureChangedEventArgs& e) {
            (void)pSender;
            m_nLogCount++;
            m_dLastLoggedTemp = e.GetNewTemperature();
        }
    };

    // Free function handler for testing
    static int s_nFreeFuncCalls = 0;
    static void TestFreeFunctionHandler(const void* pSender, const EventArgs& e) {
        (void)pSender;
        (void)e;
        s_nFreeFuncCalls++;
    }

    class EventHandlerTests : public ::testing::Test {
    protected:
        void SetUp() override {
            s_nFreeFuncCalls = 0;
        }
    };

    TEST_F(EventHandlerTests, EventArgs_Empty_ReturnsConsistentInstance) {
        // Given / When
        const EventArgs& empty1 = EventArgs::Empty();
        const EventArgs& empty2 = EventArgs::Empty();

        // Then
        ASSERT_EQ(&empty1, &empty2);
    }

    TEST_F(EventHandlerTests, EventHandler_DefaultConstructor_IsEmpty) {
        // Given / When
        EventHandler<> handler;

        // Then
        ASSERT_TRUE(handler.IsEmpty());
        ASSERT_EQ(handler.GetSubscriberCount(), 0u);
        ASSERT_FALSE(static_cast<bool>(handler));
    }

    TEST_F(EventHandlerTests, EventHandler_InvokeEmpty_DoesNotThrowOrCrash) {
        // Given
        EventHandler<> handler;

        // When / Then
        handler.Invoke(nullptr, EventArgs::Empty());
        handler(nullptr, EventArgs::Empty());
        SUCCEED();
    }

    TEST_F(EventHandlerTests, EventHandler_SingleSubscriber_ReceivesEventAndSender) {
        // Given
        Thermostat thermostat(20.0);
        const void* pCapturedSender = nullptr;
        bool bInvoked = false;

        thermostat.PowerStateChanged += [&](const void* pSender, const EventArgs& e) {
            (void)e;
            pCapturedSender = pSender;
            bInvoked = true;
        };

        // When
        thermostat.TogglePower();

        // Then
        ASSERT_TRUE(bInvoked);
        ASSERT_EQ(pCapturedSender, &thermostat);
        ASSERT_FALSE(thermostat.IsPoweredOn());
    }

    TEST_F(EventHandlerTests, EventHandler_Multicast_ExecutesSubscribersInOrder) {
        // Given
        EventHandler<TemperatureChangedEventArgs> handler;
        std::vector<int> vExecutionOrder;

        handler += [&](const void* pSender, const TemperatureChangedEventArgs& e) {
            (void)pSender; (void)e;
            vExecutionOrder.push_back(1);
        };
        handler += [&](const void* pSender, const TemperatureChangedEventArgs& e) {
            (void)pSender; (void)e;
            vExecutionOrder.push_back(2);
        };
        handler += [&](const void* pSender, const TemperatureChangedEventArgs& e) {
            (void)pSender; (void)e;
            vExecutionOrder.push_back(3);
        };

        // When
        TemperatureChangedEventArgs args(20.0, 25.0, "Kitchen");
        handler.Invoke(nullptr, args);

        // Then
        ASSERT_EQ(handler.GetSubscriberCount(), 3u);
        ASSERT_EQ(vExecutionOrder.size(), 3u);
        ASSERT_EQ(vExecutionOrder[0], 1);
        ASSERT_EQ(vExecutionOrder[1], 2);
        ASSERT_EQ(vExecutionOrder[2], 3);
    }

    TEST_F(EventHandlerTests, EventHandler_UnsubscribeByToken_RemovesTargetOnly) {
        // Given
        EventHandler<> handler;
        int iCalls1 = 0;
        int iCalls2 = 0;

        size_t nToken1 = (handler += [&](const void*, const EventArgs&) { iCalls1++; });
        handler += [&](const void*, const EventArgs&) { iCalls2++; };

        ASSERT_EQ(handler.GetSubscriberCount(), 2u);

        // When - First invocation
        handler(nullptr, EventArgs::Empty());
        ASSERT_EQ(iCalls1, 1);
        ASSERT_EQ(iCalls2, 1);

        // When - Unsubscribe token1
        bool bRemoved = (handler -= nToken1);
        ASSERT_TRUE(bRemoved);
        ASSERT_EQ(handler.GetSubscriberCount(), 1u);

        // Second invocation
        handler(nullptr, EventArgs::Empty());

        // Then
        ASSERT_EQ(iCalls1, 1);
        ASSERT_EQ(iCalls2, 2);

        // Removing non-existent token returns false
        ASSERT_FALSE(handler.Remove(99999));
        ASSERT_FALSE(handler -= nToken1);
    }

    TEST_F(EventHandlerTests, EventHandler_CustomEventArgs_TransfersDataAccurately) {
        // Given
        Thermostat thermostat(21.5);
        double dCapturedOld = 0.0;
        double dCapturedNew = 0.0;
        String sCapturedLoc;

        thermostat.TemperatureChanged += [&](const void* pSender, const TemperatureChangedEventArgs& e) {
            (void)pSender;
            dCapturedOld = e.GetOldTemperature();
            dCapturedNew = e.GetNewTemperature();
            sCapturedLoc = e.GetLocation();
        };

        // When
        thermostat.SetTemperature(24.0);

        // Then
        ASSERT_DOUBLE_EQ(dCapturedOld, 21.5);
        ASSERT_DOUBLE_EQ(dCapturedNew, 24.0);
        ASSERT_EQ(sCapturedLoc, "Living Room");
    }

    TEST_F(EventHandlerTests, EventHandler_MemberFunctionBinding_InvokesMethod) {
        // Given
        Thermostat thermostat(18.0);
        LoggerSubscriber logger;

        thermostat.TemperatureChanged.Add(&logger, &LoggerSubscriber::OnTemperatureChanged);

        // When
        thermostat.SetTemperature(22.0);
        thermostat.SetTemperature(23.5);

        // Then
        ASSERT_EQ(logger.m_nLogCount, 2);
        ASSERT_DOUBLE_EQ(logger.m_dLastLoggedTemp, 23.5);
    }

    TEST_F(EventHandlerTests, EventHandler_FreeFunctionBinding_InvokesFunction) {
        // Given
        EventHandler<> handler;
        handler.Add(&TestFreeFunctionHandler);

        // When
        handler(nullptr, EventArgs::Empty());

        // Then
        ASSERT_EQ(s_nFreeFuncCalls, 1);
    }

    TEST_F(EventHandlerTests, EventHandler_Clear_RemovesAllSubscribers) {
        // Given
        EventHandler<> handler;
        int iCount = 0;
        handler += [&](const void*, const EventArgs&) { iCount++; };
        handler += [&](const void*, const EventArgs&) { iCount++; };
        ASSERT_EQ(handler.GetSubscriberCount(), 2u);

        // When
        handler.Clear();

        // Then
        ASSERT_TRUE(handler.IsEmpty());
        ASSERT_EQ(handler.GetSubscriberCount(), 0u);
        handler(nullptr, EventArgs::Empty());
        ASSERT_EQ(iCount, 0);
    }

    TEST_F(EventHandlerTests, EventHandler_CopyAndMove_PreservesSubscriptions) {
        // Given
        EventHandler<> handler1;
        int iCalls = 0;
        handler1 += [&](const void*, const EventArgs&) { iCalls++; };

        // When - Copy
        EventHandler<> handlerCopy = handler1;
        handlerCopy(nullptr, EventArgs::Empty());
        ASSERT_EQ(iCalls, 1);

        // When - Move
        EventHandler<> handlerMoved = std::move(handler1);
        handlerMoved(nullptr, EventArgs::Empty());
        ASSERT_EQ(iCalls, 2);
        ASSERT_TRUE(handler1.IsEmpty());
    }

    TEST_F(EventHandlerTests, EventHandler_CombineWithOperatorPlusEquals_MergesSubscribers) {
        // Given
        EventHandler<> handlerA;
        EventHandler<> handlerB;
        int iCallsA = 0;
        int iCallsB = 0;

        handlerA += [&](const void*, const EventArgs&) { iCallsA++; };
        handlerB += [&](const void*, const EventArgs&) { iCallsB++; };

        // When
        handlerA += handlerB;

        // Then
        ASSERT_EQ(handlerA.GetSubscriberCount(), 2u);
        handlerA(nullptr, EventArgs::Empty());
        ASSERT_EQ(iCallsA, 1);
        ASSERT_EQ(iCallsB, 1);
    }

}
