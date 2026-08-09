# FileDownloader

The `FileDownloader` class in `DotNetDupe::System::Net::Http` provides resilient HTTP/HTTPS file downloading support built upon `HttpClient` with pause, resume, auto-resumption of partial downloads via HTTP Range headers, and progress tracking including total/downloaded/remaining bytes and real-time download rates.

## Class Definition

```cpp
namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {
                enum class DownloadStatus {
                    NotStarted,
                    Downloading,
                    Paused,
                    Completed,
                    Failed
                };

                struct DownloadProgress {
                    long long TotalBytes;
                    long long DownloadedBytes;
                    long long RemainingBytes;
                    double DownloadRateBytesPerSec;
                    DownloadStatus Status;
                };

                class FileDownloader : public Object {
                public:
                    DOTNETDUPE_API FileDownloader(const String& sUrl, const String& sDestinationPath);
                    DOTNETDUPE_API ~FileDownloader();

                    DOTNETDUPE_API bool Start();
                    DOTNETDUPE_API void Pause();
                    DOTNETDUPE_API bool Resume();

                    DOTNETDUPE_API DownloadProgress GetProgress() const;
                    DOTNETDUPE_API DownloadStatus GetStatus() const;

                    DOTNETDUPE_API void SetProgressCallback(const Action<DownloadProgress>& callback);
                };
            }
        }
    }
}
```

## Features

- **Pause and Resume Support**: Safely request pause at any time during an active download.
- **Resilient Resume (HTTP Range)**: Automatically detects existing partially downloaded files on disk and requests HTTP Range byte offsets (`Range: bytes=X-`) to resume seamlessly without redownloading previously fetched content.
- **Real-time Download Metrics**: Calculates total bytes, downloaded bytes, remaining bytes, and live download speed in bytes per second (`DownloadRateBytesPerSec`).
- **Progress Callbacks**: Receive real-time progress updates via `System::Action<DownloadProgress>`.

## Usage Example

```cpp
#include "System/Net/FileDownloader.h"
#include "System/Console.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Threading;

int main() {
    String url = "http://example.com/largefile.zip";
    String dest = "C:\\Downloads\\largefile.zip";

    FileDownloader downloader(url, dest);

    downloader.SetProgressCallback([](DownloadProgress progress) {
        Console::WriteLine(String::Format("Downloaded: {0}/{1} bytes | Rate: {2} B/s",
            String::FromInt64(progress.DownloadedBytes),
            String::FromInt64(progress.TotalBytes),
            String::FromInt32(static_cast<int>(progress.DownloadRateBytesPerSec))));
    });

    // Start downloading asynchronously
    downloader.Start();

    Thread::Sleep(2000);

    // Pause download
    downloader.Pause();
    Console::WriteLine("Download paused.");

    Thread::Sleep(1000);

    // Resume download from saved byte offset
    downloader.Resume();
    Console::WriteLine("Download resumed.");

    while (downloader.GetStatus() == DownloadStatus::Downloading) {
        Thread::Sleep(100);
    }

    if (downloader.GetStatus() == DownloadStatus::Completed) {
        Console::WriteLine("Download finished successfully!");
    }

    return 0;
}
```
