#include "pch.h"
#include "gtest/gtest.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/Logger.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/ConsoleLoggerProvider.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include "Extensions/Logging/LogManager.h"
#include "Extensions/Logging/LoggerTextWriter.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

namespace LoggingTests {

    class TestLogService {};

    TEST(LoggingTests, GivenCategoryName_WhenLogManagerGetLogger_ReturnsValidLogger) {
        // Given
        LogManager::Reset();
        String category = "OrderService";

        // When
        auto logger = LogManager::GetLogger(category);

        // Then
        ASSERT_FALSE(logger.IsNull());
        
        // Caching verification
        auto loggerCached = LogManager::GetLogger(category);
        ASSERT_TRUE(logger == loggerCached);

        LogManager::Reset();
    }

    TEST(LoggingTests, GivenTypeT_WhenLogManagerGetLoggerTyped_ReturnsTypedLoggerInstance) {
        // Given
        LogManager::Reset();

        // When
        auto typedLogger = LogManager::GetLogger<TestLogService>();

        // Then
        ASSERT_FALSE(typedLogger.IsNull());

        LogManager::Reset();
    }

    TEST(LoggingTests, GivenCategory_WhenGetConsoleLogger_ReturnsDedicatedConsoleLogger) {
        // Given
        LogManager::Reset();
        LoggerConfiguration config;
        config.MinLevel = LogLevel::Debug;
        LogManager::Configure(config);

        // When
        auto consoleLogger = LogManager::GetConsoleLogger("UIEvents");

        // Then
        ASSERT_FALSE(consoleLogger.IsNull());
        ASSERT_TRUE(consoleLogger->IsEnabled(LogLevel::Debug));

        LogManager::Reset();
    }

    TEST(LoggingTests, GivenCategory_WhenGetFileLogger_ReturnsDedicatedFileLogger) {
        // Given
        LogManager::Reset();
        String filePath = "logmanager_file_test.log";
        if (IO::File::Exists(filePath)) IO::File::Delete(filePath);

        LoggerConfiguration config;
        config.FilePath = filePath;
        config.MinLevel = LogLevel::Information;
        LogManager::Configure(config);

        // When
        auto fileLogger = LogManager::GetFileLogger("AuditCategory");

        // Then
        ASSERT_FALSE(fileLogger.IsNull());
        fileLogger->Log(LogLevel::Information, "LogManager direct file test");

        // Cleanup
        LogManager::Reset();
        if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
    }

    TEST(LoggingTests, GivenLoggerFactory_WhenProviderAdded_LogsSuccessfully) {
        String logFilePath = "test_output.log";
        
        // Cleanup pre-existing
        if (IO::File::Exists(logFilePath)) {
            IO::File::Delete(logFilePath);
        }

        {
            auto factory = SmartPointer<LoggerFactory>::NewShared();
            
            // Add console provider (plain text)
            auto consoleProvider = SmartPointer<ConsoleLoggerProvider>::NewShared(false, LogLevel::Debug);
            factory->AddProvider(consoleProvider);
            
            // Add file provider (JSON format)
            auto fileProvider = SmartPointer<FileLoggerProvider>::NewShared(logFilePath, true, LogLevel::Information);
            factory->AddProvider(fileProvider);

            // Create template logger
            auto logger = factory->CreateLogger("LoggingTests::TestCategory");
            
            // Log plain message (Trace is disabled, Debug is console-only, Info is both)
            logger->Log(LogLevel::Trace, "Trace message - should not log");
            logger->Log(LogLevel::Debug, "Debug message - console only");
            
            // Log structured message with properties
            Collections::Generic::Dictionary<String, String> properties;
            properties.Add("user", "Alice");
            properties.Add("ip", "192.168.1.1");
            logger->Log(LogLevel::Information, "User logged in", properties);

            // Assert logger enablement
            ASSERT_TRUE(logger->IsEnabled(LogLevel::Information));
            ASSERT_FALSE(logger->IsEnabled(LogLevel::Trace));
        } // Out of scope: closes file stream

        // Verify file content
        ASSERT_TRUE(IO::File::Exists(logFilePath));
        String logContent = IO::File::ReadAllText(logFilePath);
        
        // Assert JSON structures in structured file logging
        ASSERT_TRUE(logContent.Contains("timestamp"));
        ASSERT_TRUE(logContent.Contains("Information"));
        ASSERT_TRUE(logContent.Contains("User logged in"));
        ASSERT_TRUE(logContent.Contains("properties"));
        ASSERT_TRUE(logContent.Contains("Alice"));
        ASSERT_TRUE(logContent.Contains("192.168.1.1"));

        // File should NOT contain Debug message (FileLogger minLevel was Info)
        ASSERT_FALSE(logContent.Contains("Debug message"));
        
        // Cleanup
        if (IO::File::Exists(logFilePath)) {
            IO::File::Delete(logFilePath);
        }
    }

    TEST(LoggingTests, GivenLoggerConfiguration_WhenRolloverTriggered_RotatesFilesCorrectly) {
        String logFilePath = "rollover_test.log";
        
        // Clean up previous files
        if (IO::File::Exists(logFilePath)) IO::File::Delete(logFilePath);
        if (IO::File::Exists(logFilePath + ".1")) IO::File::Delete(logFilePath + ".1");
        if (IO::File::Exists(logFilePath + ".2")) IO::File::Delete(logFilePath + ".2");
        if (IO::File::Exists(logFilePath + ".3")) IO::File::Delete(logFilePath + ".3");

        {
            LoggerConfiguration config;
            config.FilePath = logFilePath;
            config.MinLevel = LogLevel::Information;
            config.IsJsonFormat = false;
            config.PlainTextFormat = "{Message}"; // Keep output exact for byte sizes
            config.Rollover.EnableRollover = true;
            config.Rollover.MaxFileSizeInBytes = 50; // Roll over after 50 bytes
            config.Rollover.MaxBackupFiles = 2; // Keep at most 2 backups

            auto factory = SmartPointer<LoggerFactory>::NewShared();
            auto fileProvider = SmartPointer<FileLoggerProvider>::NewShared(config);
            factory->AddProvider(fileProvider);

            auto logger = factory->CreateLogger("RolloverTest");

            // Write logs of size 15 bytes to trigger size rotation.
            // On Windows, std::endl writes 2 bytes (\r\n), making each line 17 bytes.
            // Under a 50-byte limit, 3 logs take 51 bytes. The 4th log triggers the first rollover.
            // The 5th and 6th logs in the new file take 34 bytes. The 7th log triggers the second rollover.
            logger->Log(LogLevel::Information, "MsgA_15bytes___");
            logger->Log(LogLevel::Information, "MsgB_15bytes___");
            logger->Log(LogLevel::Information, "MsgC_15bytes___");
            logger->Log(LogLevel::Information, "MsgD_15bytes___");
            logger->Log(LogLevel::Information, "MsgE_15bytes___");
            logger->Log(LogLevel::Information, "MsgF_15bytes___");
            logger->Log(LogLevel::Information, "MsgG_15bytes___");
        } // Stream closes

        // Assert file structures
        ASSERT_TRUE(IO::File::Exists(logFilePath)); // Active log
        ASSERT_TRUE(IO::File::Exists(logFilePath + ".1")); // Backup 1
        ASSERT_TRUE(IO::File::Exists(logFilePath + ".2")); // Backup 2
        ASSERT_FALSE(IO::File::Exists(logFilePath + ".3")); // Max backup files limit is 2

        // Cleanup
        if (IO::File::Exists(logFilePath)) IO::File::Delete(logFilePath);
        if (IO::File::Exists(logFilePath + ".1")) IO::File::Delete(logFilePath + ".1");
        if (IO::File::Exists(logFilePath + ".2")) IO::File::Delete(logFilePath + ".2");
    }

    TEST(LoggingTests, GivenJsonConfig_WhenLoaded_ParsesCorrectly) {
        String json = "{"
                      "  \"MinLevel\": \"Debug\","
                      "  \"IsJsonFormat\": true,"
                      "  \"PlainTextFormat\": \"{Timestamp} {Message}\","
                      "  \"TimestampFormat\": \"HH:mm:ss\","
                      "  \"FilePath\": \"test_json_load.log\","
                      "  \"Rollover\": {"
                      "    \"EnableRollover\": true,"
                      "    \"MaxFileSizeInBytes\": 1048576,"
                      "    \"MaxBackupFiles\": 5"
                      "  }"
                      "}";

        auto config = LoggerConfiguration::LoadFromJson(json);
        EXPECT_EQ(config.MinLevel, LogLevel::Debug);
        EXPECT_TRUE(config.IsJsonFormat);
        EXPECT_EQ(config.PlainTextFormat, "{Timestamp} {Message}");
        EXPECT_EQ(config.TimestampFormat, "HH:mm:ss");
        EXPECT_EQ(config.FilePath, "test_json_load.log");
        EXPECT_TRUE(config.Rollover.EnableRollover);
        EXPECT_EQ(config.Rollover.MaxFileSizeInBytes, 1048576);
        EXPECT_EQ(config.Rollover.MaxBackupFiles, 5);
    }

    TEST(LoggingTests, GivenJsonConfigCasingVariations_WhenLoaded_ParsesCorrectly) {
        String json = "{"
                      "  \"min_level\": \"warning\","
                      "  \"is_json_format\": false,"
                      "  \"plain_text_format\": \"[{Level}] {Message}\","
                      "  \"timestamp_format\": \"yyyy-MM-dd\","
                      "  \"file_path\": \"casing_test.log\","
                      "  \"rollover\": {"
                      "    \"enable_rollover\": true,"
                      "    \"max_file_size_in_bytes\": 2048,"
                      "    \"max_backup_files\": 10"
                      "  }"
                      "}";

        auto config = LoggerConfiguration::LoadFromJson(json);
        EXPECT_EQ(config.MinLevel, LogLevel::Warning);
        EXPECT_FALSE(config.IsJsonFormat);
        EXPECT_EQ(config.PlainTextFormat, "[{Level}] {Message}");
        EXPECT_EQ(config.TimestampFormat, "yyyy-MM-dd");
        EXPECT_EQ(config.FilePath, "casing_test.log");
        EXPECT_TRUE(config.Rollover.EnableRollover);
        EXPECT_EQ(config.Rollover.MaxFileSizeInBytes, 2048);
        EXPECT_EQ(config.Rollover.MaxBackupFiles, 10);
    }

    TEST(LoggingTests, GivenJsonConfigLogLevelAsInt_WhenLoaded_ParsesCorrectly) {
        String json = "{"
                      "  \"MinLevel\": 4"
                      "}";

        auto config = LoggerConfiguration::LoadFromJson(json);
        EXPECT_EQ(config.MinLevel, LogLevel::Error);
    }

    TEST(LoggingTests, GivenJsonConfigFile_WhenLoadedFromFile_ParsesCorrectly) {
        String testFile = "temp_config.json";
        String json = "{"
                      "  \"MinLevel\": \"Critical\","
                      "  \"FilePath\": \"file_load.log\""
                      "}";

        if (IO::File::Exists(testFile)) {
            IO::File::Delete(testFile);
        }

        IO::File::WriteAllText(testFile, json);

        auto config = LoggerConfiguration::LoadFromFile(testFile);
        EXPECT_EQ(config.MinLevel, LogLevel::Critical);
        EXPECT_EQ(config.FilePath, "file_load.log");

        IO::File::Delete(testFile);
    }

    TEST(LoggingTests, GivenNonExistentFile_WhenLoadFromFileCalled_ThrowsIOException) {
        String invalidFile = "this_file_does_not_exist_xyz.json";
        if (IO::File::Exists(invalidFile)) {
            IO::File::Delete(invalidFile);
        }

        EXPECT_THROW({
            LoggerConfiguration::LoadFromFile(invalidFile);
        }, DotNetDupe::System::IO::IOException);
    }

    TEST(LoggingTests, GivenInvalidLogLevel_WhenParseLogLevelCalled_ThrowsArgumentException) {
        EXPECT_THROW({
            ParseLogLevel("invalid_log_level_value");
        }, DotNetDupe::System::ArgumentException);
    }

    TEST(LoggingTests, GivenEmptyFilePath_WhenFileLoggerProviderCreated_AutoGeneratesPathAndDirectory) {
        // Given
        String emptyPath = "";

        // When
        FileLoggerProvider* pProv = new FileLoggerProvider(emptyPath);
        SmartPointer<FileLoggerProvider> provider(pProv, true);
        String resolvedPath = provider->GetFilePath();

        // Then
        ASSERT_TRUE(resolvedPath.EndsWith("logs/app.log", false) || resolvedPath.EndsWith("logs\\app.log", false));
        ASSERT_TRUE(IO::File::Exists(resolvedPath));

        // Cleanup
        provider = nullptr;
        try {
            if (IO::File::Exists(resolvedPath)) IO::File::Delete(resolvedPath);
        } catch (...) {}
    }

    TEST(LoggingTests, GivenRelativeFilePath_WhenFileLoggerProviderCreated_ResolvesFullPathAndCreatesDirectory) {
        // Given
        String relativePath = "custom_dir/test_relative.log";

        // When
        FileLoggerProvider* pProv = new FileLoggerProvider(relativePath);
        SmartPointer<FileLoggerProvider> provider(pProv, true);
        String resolvedPath = provider->GetFilePath();

        // Then
        ASSERT_TRUE(IO::Path::IsPathFullyQualified(resolvedPath));
        ASSERT_TRUE(IO::File::Exists(resolvedPath));

        // Cleanup
        provider = nullptr;
        try {
            if (IO::File::Exists(resolvedPath)) IO::File::Delete(resolvedPath);
        } catch (...) {}
    }

    TEST(LoggingTests, GivenDefaultConstructor_WhenFileLoggerProviderCreated_InheritsLogManagerConfiguration) {
        // Given
        LogManager::Reset();
        LoggerConfiguration config;
        config.FilePath = "inherited_provider.log";
        config.MinLevel = LogLevel::Warning;
        LogManager::Configure(config);

        try {
            if (IO::File::Exists(config.FilePath)) IO::File::Delete(config.FilePath);
        } catch (...) {}

        // When
        FileLoggerProvider* pProv = new FileLoggerProvider();
        SmartPointer<FileLoggerProvider> provider(pProv, true);
        auto logger = provider->CreateLogger("InheritedCategory");

        // Then
        ASSERT_FALSE(logger->IsEnabled(LogLevel::Information));
        ASSERT_TRUE(logger->IsEnabled(LogLevel::Warning));

        // Cleanup
        logger = nullptr;
        provider = nullptr;
        LogManager::Reset();
        try {
            if (IO::File::Exists(config.FilePath)) IO::File::Delete(config.FilePath);
        } catch (...) {}
    }

    TEST(LoggingTests, GivenLoggerTextWriter_WhenConsoleSetOut_RedirectsToLogger) {
        // Given
        LogManager::Reset();
        String filePath = "redirect_textwriter_test.log";
        try {
            if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
        } catch (...) {}

        LoggerConfiguration config;
        config.FilePath = filePath;
        LogManager::Configure(config);

        FileLoggerProvider* pProvRaw = new FileLoggerProvider(config);
        SmartPointer<FileLoggerProvider> fileProv(pProvRaw, true);
        auto redirector = SmartPointer<LoggerTextWriter>::NewShared("Redirector", LogLevel::Information);
        Console::SetOut(redirector);

        // When
        Console::WriteLine("Redirected message test");

        // Then
        ASSERT_TRUE(IO::File::Exists(filePath));

        // Cleanup
        Console::SetOut(nullptr);
        redirector = nullptr;
        fileProv = nullptr;
        LogManager::Reset();
        try {
            if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
        } catch (...) {}
    }

    TEST(LoggingTests, GivenProcessIdAndThreadId_WhenPlainTextOrJsonFormatSpecified_IncludesIdsInLogOutput) {
        // Given
        LogManager::Reset();
        String filePath = "proc_thread_id_test.log";
        try {
            if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
        } catch (...) {}

        LoggerConfiguration config;
        config.FilePath = filePath;
        config.PlainTextFormat = "[PID:{ProcessId}] [TID:{ThreadId}] [{Level}] {Message}";
        config.IsJsonFormat = false;

        FileLoggerProvider* pProvRaw = new FileLoggerProvider(config);
        SmartPointer<FileLoggerProvider> fileProv(pProvRaw, true);
        auto logger = fileProv->CreateLogger("TestProcessThreadCategory");

        // When
        logger->Log(LogLevel::Information, "Test message with IDs");

        // Force flush/close
        fileProv = nullptr;

        // Then
        ASSERT_TRUE(IO::File::Exists(filePath));
        String fileContent = IO::File::ReadAllText(filePath);
        EXPECT_TRUE(fileContent.Contains("PID:"));
        EXPECT_TRUE(fileContent.Contains("TID:"));
        EXPECT_TRUE(fileContent.Contains("Test message with IDs"));

        // Cleanup
        LogManager::Reset();
        try {
            if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
        } catch (...) {}
    }

    TEST(LoggingTests, GivenDynamicPropertiesInFormat_WhenLogged_InterpolatesCustomPropertiesCorrectly) {
        // Given
        LogManager::Reset();
        String filePath = "dynamic_props_test.log";
        try {
            if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
        } catch (...) {}

        LoggerConfiguration config;
        config.FilePath = filePath;
        config.PlainTextFormat = "[TRACE:{TraceId}] [{Level}] [{Category}] => {Message} [{Properties}]";
        config.IsJsonFormat = false;

        auto fileProv = DotNetDupe::System::SmartPointer<FileLoggerProvider>::NewShared(config);
        auto logger = fileProv->CreateLogger("OrderService");

        Collections::Generic::Dictionary<String, String> props;
        props.Add("TraceId", "tx_98765");
        props.Add("UserId", "user_42");

        // When
        logger->Log(LogLevel::Information, "Order placed successfully", props);

        // Force flush/close
        fileProv = nullptr;

        // Then
        ASSERT_TRUE(IO::File::Exists(filePath));
        String fileContent = IO::File::ReadAllText(filePath);
        EXPECT_TRUE(fileContent.Contains("[TRACE:tx_98765]"));
        EXPECT_TRUE(fileContent.Contains("[OrderService]"));
        EXPECT_TRUE(fileContent.Contains("Order placed successfully"));
        EXPECT_TRUE(fileContent.Contains("UserId: user_42"));
        EXPECT_FALSE(fileContent.Contains("{TraceId}"));

        // Cleanup
        LogManager::Reset();
        try {
            if (IO::File::Exists(filePath)) IO::File::Delete(filePath);
        } catch (...) {}
    }
}

