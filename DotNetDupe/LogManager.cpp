#include "pch.h"
#include "Extensions/Logging/LogManager.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/ConsoleLoggerProvider.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "Extensions/Logging/FileLogger.h"
#include <mutex>
#include <unordered_map>
#include <string>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            namespace {
                static std::mutex s_mutex;
                static LoggerConfiguration s_globalConfig;
                static DotNetDupe::System::SmartPointer<LoggerFactory> s_pFactory = nullptr;
                static DotNetDupe::System::SmartPointer<ConsoleLoggerProvider> s_pConsoleProvider = nullptr;
                static DotNetDupe::System::SmartPointer<FileLoggerProvider> s_pFileProvider = nullptr;
                static std::unordered_map<std::string, DotNetDupe::System::SmartPointer<ILogger>> s_loggerCache;
                static std::unordered_map<std::string, DotNetDupe::System::SmartPointer<ILogger>> s_consoleLoggerCache;
                static std::unordered_map<std::string, DotNetDupe::System::SmartPointer<ILogger>> s_fileLoggerCache;

                void EnsureFactoryInitializedLocked() {
                    if (!s_pFactory) {
                        s_pFactory = DotNetDupe::System::SmartPointer<LoggerFactory>::NewShared();
                        s_pConsoleProvider = DotNetDupe::System::SmartPointer<ConsoleLoggerProvider>::NewShared(s_globalConfig);
                        s_pFileProvider = DotNetDupe::System::SmartPointer<FileLoggerProvider>::NewShared(s_globalConfig);
                        s_pFactory->AddProvider(s_pConsoleProvider);
                        s_pFactory->AddProvider(s_pFileProvider);
                    }
                }
            }

            DotNetDupe::System::SmartPointer<ILogger> LogManager::GetLogger(const DotNetDupe::System::String& sCategoryName) {
                std::lock_guard<std::mutex> lk(s_mutex);
                std::string key = sCategoryName.GetRawString();

                auto it = s_loggerCache.find(key);
                if (it != s_loggerCache.end()) {
                    return it->second;
                }

                EnsureFactoryInitializedLocked();
                auto pLogger = s_pFactory->CreateLogger(sCategoryName);
                s_loggerCache[key] = pLogger;
                return pLogger;
            }

            DotNetDupe::System::SmartPointer<ILogger> LogManager::GetConsoleLogger(const DotNetDupe::System::String& sCategoryName) {
                std::lock_guard<std::mutex> lk(s_mutex);
                std::string key = sCategoryName.GetRawString();

                auto it = s_consoleLoggerCache.find(key);
                if (it != s_consoleLoggerCache.end()) {
                    return it->second;
                }

                EnsureFactoryInitializedLocked();
                auto pLogger = s_pConsoleProvider->CreateLogger(sCategoryName);
                s_consoleLoggerCache[key] = pLogger;
                return pLogger;
            }

            DotNetDupe::System::SmartPointer<ILogger> LogManager::GetFileLogger(const DotNetDupe::System::String& sCategoryName) {
                std::lock_guard<std::mutex> lk(s_mutex);
                std::string key = sCategoryName.GetRawString();

                auto it = s_fileLoggerCache.find(key);
                if (it != s_fileLoggerCache.end()) {
                    return it->second;
                }

                EnsureFactoryInitializedLocked();
                auto pLogger = s_pFileProvider->CreateLogger(sCategoryName);
                s_fileLoggerCache[key] = pLogger;
                return pLogger;
            }

            void LogManager::Configure(const LoggerConfiguration& config) {
                std::lock_guard<std::mutex> lk(s_mutex);
                s_globalConfig = config;
                s_loggerCache.clear();
                s_consoleLoggerCache.clear();
                s_fileLoggerCache.clear();
                
                s_pFactory = DotNetDupe::System::SmartPointer<LoggerFactory>::NewShared();
                s_pConsoleProvider = DotNetDupe::System::SmartPointer<ConsoleLoggerProvider>::NewShared(s_globalConfig);
                s_pFileProvider = DotNetDupe::System::SmartPointer<FileLoggerProvider>::NewShared(s_globalConfig);
                s_pFactory->AddProvider(s_pConsoleProvider);
                s_pFactory->AddProvider(s_pFileProvider);
            }

            const LoggerConfiguration& LogManager::GetConfiguration() {
                std::lock_guard<std::mutex> lk(s_mutex);
                return s_globalConfig;
            }

            void LogManager::AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) {
                std::lock_guard<std::mutex> lk(s_mutex);
                EnsureFactoryInitializedLocked();
                s_pFactory->AddProvider(pProvider);
                s_loggerCache.clear();
            }

            void LogManager::Reset() {
                std::lock_guard<std::mutex> lk(s_mutex);
                s_globalConfig = LoggerConfiguration();
                s_loggerCache.clear();
                s_consoleLoggerCache.clear();
                s_fileLoggerCache.clear();
                s_pConsoleProvider = nullptr;
                s_pFileProvider = nullptr;
                s_pFactory = nullptr;
            }

        }
    }
}
