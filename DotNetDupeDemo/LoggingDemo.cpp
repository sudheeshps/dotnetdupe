#include "System/Console.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include "Extensions/Logging/Logger.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/ConsoleLoggerProvider.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "Extensions/Logging/LogManager.h"
#include "Extensions/Logging/LoggerTextWriter.h"
#include "System/IO/File.h"
#include "Demos.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

class DemoService : public Object {
public:
    DemoService() = default;
};

void DemonstrateLogging() {
    Console::WriteLine("\n=== Logging and Diagnostics Demonstration ===");

    // 1. Initialize LoggerFactory
    auto factory = SmartPointer<LoggerFactory>::NewShared();

    // 2. Add custom formatted console logger
    Console::WriteLine("Configuring Custom Format Console Logger (Timestamp format: %H:%M:%S)...");
    LoggerConfiguration consoleConfig;
    consoleConfig.MinLevel = LogLevel::Debug;
    consoleConfig.IsJsonFormat = false;
    consoleConfig.PlainTextFormat = "=> [{Timestamp}] [SEVERITY: {Level}] [SCOPE: {Category}] - {Message} {Properties}";
    consoleConfig.TimestampFormat = "%H:%M:%S";
    auto customConsoleProvider = SmartPointer<ConsoleLoggerProvider>::NewShared(consoleConfig);
    factory->AddProvider(customConsoleProvider);

    // 3. Add JSON structured console logger (LogLevel::Warning and above)
    Console::WriteLine("Configuring JSON Structured Console Logger (Warning level and above)...");
    auto jsonConsoleProvider = SmartPointer<ConsoleLoggerProvider>::NewShared(true, LogLevel::Warning);
    factory->AddProvider(jsonConsoleProvider);

    // 4. Add structured JSON file logger with rollover configuration loaded from JSON
    String configFile = "logging_config.json";
    String logFilePath = "app_structured.log";
    if (IO::File::Exists(logFilePath)) {
        IO::File::Delete(logFilePath);
    }
    if (IO::File::Exists(configFile)) {
        IO::File::Delete(configFile);
    }

    String jsonConfig = "{"
                        "  \"MinLevel\": \"Information\","
                        "  \"IsJsonFormat\": true,"
                        "  \"FilePath\": \"app_structured.log\","
                        "  \"Rollover\": {"
                        "    \"EnableRollover\": true,"
                        "    \"MaxFileSizeInBytes\": 250,"
                        "    \"MaxBackupFiles\": 2"
                        "  }"
                        "}";
    IO::File::WriteAllText(configFile, jsonConfig);

    Console::WriteLine("Loading LoggerConfiguration from 'logging_config.json'...");
    LoggerConfiguration fileConfig = LoggerConfiguration::LoadFromFile(configFile);

    Console::WriteLine("Configuring Structured JSON File Logger from loaded config ('app_structured.log')...");
    auto fileProvider = SmartPointer<FileLoggerProvider>::NewShared(fileConfig);
    factory->AddProvider(fileProvider);

    // Clean up config file
    IO::File::Delete(configFile);

    // 5. Create category logger
    auto logger = factory->CreateLogger("DotNetDupe::DemoService");

    // 6. Demonstrate generic Logger wrapper Logger<T>
    Logger<DemoService> genericLogger(logger);

    // 7. Log plain diagnostic messages
    Console::WriteLine("\n--- Logging Plain Messages ---");
    genericLogger.Log(LogLevel::Debug, "Checking component connectivity...");
    genericLogger.Log(LogLevel::Information, "Service initialized successfully.");
    genericLogger.Log(LogLevel::Warning, "Disk space is nearing 85% capacity.");

    // 8. Demonstrate structured logging with key-value properties
    Console::WriteLine("\n--- Logging Structured Payload ---");
    Collections::Generic::Dictionary<String, String> properties;
    properties.Add("transactionId", "TX-89104");
    properties.Add("user", "bob_dev");
    properties.Add("action", "DatabaseQuery");
    properties.Add("elapsedMs", "142");

    genericLogger.Log(LogLevel::Information, "Database transaction completed.", properties);

    // 9. Log an error
    properties.Clear();
    properties.Add("errorCode", "ERR_CONN_TIMEOUT");
    properties.Add("destination", "http://127.0.0.1:19099");
    genericLogger.Log(LogLevel::Error, "Remote service call failed.", properties);

    // 10. Demonstrate LogManager (Option A Global Logging)
    Console::WriteLine("\n--- Demonstrating LogManager Global Category Logging ---");
    auto mgrLogger = LogManager::GetLogger("OrderService");
    mgrLogger->Log(LogLevel::Information, "OrderService initialized via LogManager::GetLogger(\"OrderService\")");

    auto typedMgrLogger = LogManager::GetLogger<DemoService>();
    typedMgrLogger->Log(LogLevel::Information, "Logged via LogManager::GetLogger<DemoService>()");

    // Demonstrate LoggerTextWriter log redirector
    Console::WriteLine("\n--- Demonstrating LoggerTextWriter Redirector ---");
    auto loggerWriter = SmartPointer<LoggerTextWriter>::NewShared("RedirectedCategory", LogLevel::Information);
    Console::SetOut(loggerWriter);
    Console::WriteLine("Console message redirected via LoggerTextWriter to LogManager logger");
    Console::SetOut(nullptr);

    // 11. Display contents of the structured log file
    Console::WriteLine("\n--- Displaying Contents of 'app_structured.log' ---");
    if (IO::File::Exists(logFilePath)) {
        String fileText = IO::File::ReadAllText(logFilePath);
        Console::WriteLine(fileText);
        IO::File::Delete(logFilePath);
    } else {
        Console::WriteLine("Log file was not generated.");
    }
    Console::WriteLine("=============================================");
}
