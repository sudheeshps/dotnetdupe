#pragma once
#include <fstream>
#include <mutex>
#include <memory>
#include "System/Object.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {
            struct FileLoggerContext : public DotNetDupe::System::Object {
                std::shared_ptr<std::ofstream> fileStream;
                std::shared_ptr<std::mutex> fileMutex;
                FileLoggerContext() = default;
                ~FileLoggerContext() override = default;
            };
        }
    }
}
