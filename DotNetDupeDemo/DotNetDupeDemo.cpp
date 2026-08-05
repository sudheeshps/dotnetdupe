#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

int main() {
    DemonstrateConsole();
    DemonstrateString();
    DemonstrateBitConverter();
    DemonstrateConvert();
    DemonstrateTimeProvider();
    DemonstrateStopwatch();
    DemonstrateStringBuilder();
    DemonstrateCollections();
    DemonstrateGuid();
    DemonstrateEnvironment();
    DemonstrateTimeZone();
    DemonstrateTextIO();
    DemonstrateFileAttributes();
    DemonstrateThreading();
    DemonstrateSynchronization();
    DemonstrateLockRAII();
    DemonstrateProcess();
    DemonstrateThreadPool();
    DemonstrateTask();
    DemonstrateJson();
    DemonstrateSockets();
    DemonstrateHttp();
    DemonstrateHttps();
    DemonstrateJwt();
    DemonstrateDependencyInjection();
    DemonstrateWebApplication();
    DemonstrateDatabase();
    DemonstrateLogging();
    DemonstrateEventLog();
    DemonstrateEtwLogReader();
    DemonstrateRealtimeTelemetry();
    RunWebAppServerDemo();
    DemonstratePushNotifications();
    RunUserPrincipalDemo();
    DemonstrateTerminalSession();
    RunConcurrentCollectionsDemo();
    RunGenericCollectionsDemo();
    
    Console::WriteLine("\n--- Demonstration Complete ---");
    Console::WriteLine("Press Enter to exit...");
    Console::ReadLine();
    return 0;
}
