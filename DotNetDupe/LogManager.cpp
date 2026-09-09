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
                static std::unordered_map<std::string, DotNetDupe::System::SmartPointer<ILogger>> s_pLoggerCache;
                static std::unordered_map<std::string, DotNetDupe::System::SmartPointer<ILogger>> s_pConsoleLoggerCache;
                static std::unordered_map<std::string, DotNetDupe::System::SmartPointer<ILogger>> s_pFileLoggerCache;

                void EnsureFactoryInitializedLocked() {
                    /// Lazy-initialize global logger factory and default providers.
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
                /// Synchronize global cache lookup.
                std::lock_guard<std::mutex> lk(s_mutex);
                std::string sKey = sCategoryName.GetRawString();

                /// Check if logger is already cached.
                auto it = s_pLoggerCache.find(sKey);
                if (it != s_pLoggerCache.end()) {
                    return it->second;
                }

                /// Initialize factory and instantiate category logger.
                EnsureFactoryInitializedLocked();
                auto pLogger = s_pFactory->CreateLogger(sCategoryName);
                s_pLoggerCache[sKey] = pLogger;
                return pLogger;
            }

            DotNetDupe::System::SmartPointer<ILogger> LogManager::GetConsoleLogger(const DotNetDupe::System::String& sCategoryName) {
                /// Synchronize console cache lookup.
                std::lock_guard<std::mutex> lk(s_mutex);
                std::string sKey = sCategoryName.GetRawString();

                /// Check if console logger is already cached.
                auto it = s_pConsoleLoggerCache.find(sKey);
                if (it != s_pConsoleLoggerCache.end()) {
                    return it->second;
                }

                /// Initialize provider and instantiate console logger.
                EnsureFactoryInitializedLocked();
                auto pLogger = s_pConsoleProvider->CreateLogger(sCategoryName);
                s_pConsoleLoggerCache[sKey] = pLogger;
                return pLogger;
            }

            DotNetDupe::System::SmartPointer<ILogger> LogManager::GetFileLogger(const DotNetDupe::System::String& sCategoryName) {
                /// Synchronize file cache lookup.
                std::lock_guard<std::mutex> lk(s_mutex);
                std::string sKey = sCategoryName.GetRawString();

                /// Check if file logger is already cached.
                auto it = s_pFileLoggerCache.find(sKey);
                if (it != s_pFileLoggerCache.end()) {
                    return it->second;
                }

                /// Initialize provider and instantiate file logger.
                EnsureFactoryInitializedLocked();
                auto pLogger = s_pFileProvider->CreateLogger(sCategoryName);
                s_pFileLoggerCache[sKey] = pLogger;
                return pLogger;
            }

            void LogManager::Configure(const LoggerConfiguration& config) {
                /// Synchronize global reconfiguration.
                std::lock_guard<std::mutex> lk(s_mutex);
                s_globalConfig = config;
                s_pLoggerCache.clear();
                s_pConsoleLoggerCache.clear();
                s_pFileLoggerCache.clear();
                
                /// Re-initialize factory and default providers with new configuration.
                s_pFactory = DotNetDupe::System::SmartPointer<LoggerFactory>::NewShared();
                s_pConsoleProvider = DotNetDupe::System::SmartPointer<ConsoleLoggerProvider>::NewShared(s_globalConfig);
                s_pFileProvider = DotNetDupe::System::SmartPointer<FileLoggerProvider>::NewShared(s_globalConfig);
                s_pFactory->AddProvider(s_pConsoleProvider);
                s_pFactory->AddProvider(s_pFileProvider);
            }

            const LoggerConfiguration& LogManager::GetConfiguration() {
                /// Return active global configuration under lock.
                std::lock_guard<std::mutex> lk(s_mutex);
                return s_globalConfig;
            }

            void LogManager::AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) {
                /// Guard: Ignore null provider.
                if (pProvider.IsNull()) return;

                /// Synchronize provider addition and invalidate cache.
                std::lock_guard<std::mutex> lk(s_mutex);
                EnsureFactoryInitializedLocked();
                s_pFactory->AddProvider(pProvider);
                s_pLoggerCache.clear();
            }

            void LogManager::Reset() {
                /// Synchronize reset and purge all cached loggers and providers.
                std::lock_guard<std::mutex> lk(s_mutex);
                s_globalConfig = LoggerConfiguration();
                s_pLoggerCache.clear();
                s_pConsoleLoggerCache.clear();
                s_pFileLoggerCache.clear();
                s_pConsoleProvider = nullptr;
                s_pFileProvider = nullptr;
                s_pFactory = nullptr;
            }

        }
    }
}
