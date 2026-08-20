#include "System/Console.h"
#include "System/String.h"
#include "System/EventArgs.h"
#include "System/EventHandler.h"
#include "Demos.h"

using namespace DotNetDupe::System;

namespace {

    class SensorAlertEventArgs : public EventArgs {
    public:
        SensorAlertEventArgs(const String& sSensorName, double dValue, const String& sSeverity)
            : m_sSensorName(sSensorName), m_dValue(dValue), m_sSeverity(sSeverity) {}

        String GetSensorName() const { return m_sSensorName; }
        double GetValue() const { return m_dValue; }
        String GetSeverity() const { return m_sSeverity; }

    private:
        String m_sSensorName;
        double m_dValue;
        String m_sSeverity;
    };

    class EnvironmentalMonitor : public virtual Object {
    public:
        EventHandler<SensorAlertEventArgs> SensorAlert;
        EventHandler<> SystemArmed;

        void ArmSystem() {
            Console::WriteLine("  [Publisher] Arming environmental sensors...");
            SystemArmed.Invoke(this, EventArgs::Empty());
        }

        void TriggerReading(const String& sSensor, double dVal, const String& sSev) {
            Console::WriteLine(String::Format("  [Publisher] Sensor reading triggered: {0} = {1}", sSensor, String::Format("{0}", dVal)));
            SensorAlertEventArgs args(sSensor, dVal, sSev);
            SensorAlert(this, args);
        }
    };

    class SecurityDashboard {
    public:
        void OnSensorAlert(const void* pSender, const SensorAlertEventArgs& e) {
            (void)pSender;
            Console::WriteLine(String::Format("  [SecurityDashboard] ALERT: {0} ({1}) reading={2}",
                e.GetSensorName(), e.GetSeverity(), String::Format("{0}", e.GetValue())));
        }
    };

}

void DemonstrateEventHandler() {
    Console::WriteLine("\n--- EventHandler & EventArgs Demonstration ---");

    EnvironmentalMonitor monitor;
    SecurityDashboard dashboard;

    // 1. Subscribe parameterless/standard event
    monitor.SystemArmed += [](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        Console::WriteLine("  [Subscriber 1] SystemArmed received! Monitoring active.");
    };

    // 2. Multicast subscriptions to custom generic EventHandler
    size_t nLoggerToken = (monitor.SensorAlert += [](const void* pSender, const SensorAlertEventArgs& e) {
        (void)pSender;
        Console::WriteLine(String::Format("  [AuditLog] Recorded event for sensor '{0}'", e.GetSensorName()));
    });

    monitor.SensorAlert.Add(&dashboard, &SecurityDashboard::OnSensorAlert);

    // 3. Fire events
    monitor.ArmSystem();
    monitor.TriggerReading("SmokeDetector_01", 85.5, "CRITICAL");

    // 4. Unsubscribe one listener via token
    Console::WriteLine("  [Demo] Unsubscribing AuditLog listener...");
    monitor.SensorAlert -= nLoggerToken;

    // 5. Fire event again - only SecurityDashboard will receive
    monitor.TriggerReading("TempSensor_02", 45.0, "WARNING");
    Console::WriteLine("  [Demo] Active SensorAlert subscribers: " + String::Format("{0}", (int)monitor.SensorAlert.GetSubscriberCount()));
}
