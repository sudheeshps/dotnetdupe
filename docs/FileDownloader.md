# FileDownloader, DownloadProgressChangedEventArgs &amp; DownloadCompletedEventArgs

**Namespace:** `DotNetDupe::System::Net::Http`  
**Header:** `#include "System/Net/Http/FileDownloader.h"`

A high-level HTTP/HTTPS file download manager with multicast progress tracking, transfer speed measurement, pause/resume capability, custom HTTP headers, deterministic file handle closing, and idiomatic C# .NET `EventHandler` delegates.

---

## `DownloadStatus` Enum

```cpp
enum class DownloadStatus {
    NotStarted,
    Downloading,
    Paused,
    Completed,
    Failed
};
```

---

## `DownloadProgressChangedEventArgs` Class

```cpp
class DownloadProgressChangedEventArgs : public EventArgs {
public:
    long long GetBytesReceived() const;
    long long GetTotalBytesToReceive() const;
    double GetProgressPercentage() const;
    double GetDownloadRateBytesPerSec() const;
    DownloadStatus GetStatus() const;
};
```

---

## `DownloadCompletedEventArgs` Class

```cpp
class DownloadCompletedEventArgs : public EventArgs {
public:
    bool IsSuccess() const;
    bool IsCancelled() const;
    String GetError() const;
};
```

---

## `FileDownloader` Class

### Syntax
```cpp
class FileDownloader : public Object;
```

---

## Events

### `EventHandler<DownloadProgressChangedEventArgs> DownloadProgressChanged`
Fires periodically as data chunks are downloaded, providing current transfer progress, percentage, transfer speed, and status.

### `EventHandler<DownloadCompletedEventArgs> DownloadCompleted`
Fires when the download terminates, either upon successful completion, user pause/cancellation, or fatal network/IO failure.

---

## Constructors

### `FileDownloader(const String& sUrl, const String& sDestinationPath)`
Initializes a new `FileDownloader` targeting a remote URL and saving to a local destination file path.

---

## Member Functions

### `bool Start()`
Begins downloading the file asynchronously on a background worker thread. Returns `true` if started; `false` if already downloading.

### `void Pause()`
Pauses the active download.

### `bool Resume()`
Resumes a paused download, requesting remaining bytes using HTTP range requests (`Range: bytes=X-`).

### `DownloadProgress GetProgress() const`
Gets the current progress snapshot (total bytes, bytes transferred, speed in B/s, status).

### `DownloadStatus GetStatus() const`
Gets the current download state.

### `void AddHeaders(const Dictionary<String, String>& headers)`
Appends custom request headers to the download connection.

### `void SetUserAgent(const String& sUserAgent)`
Sets a custom `User-Agent` header for the download request.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Net/Http/FileDownloader.h"
#include "System/String.h"
#include "System/Convert.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Threading;

int main() {
    FileDownloader downloader(
        "https://speed.cloudflare.com/__down?bytes=10485760", 
        "downloaded_10mb.dat"
    );

    downloader.SetUserAgent("DotNetDupe-Downloader/4.0");

    // Multicast subscription for progress
    downloader.DownloadProgressChanged += [](const void* pSender, const DownloadProgressChangedEventArgs& e) {
        (void)pSender;
        double speedKbps = e.GetDownloadRateBytesPerSec() / 1024.0;
        Console::WriteLine("Progress: {0}% ({1}/{2} bytes) | Speed: {3} KB/s",
            (int)e.GetProgressPercentage(),
            e.GetBytesReceived(),
            e.GetTotalBytesToReceive(),
            (int)speedKbps);
    };

    // Subscription for completion
    downloader.DownloadCompleted += [](const void* pSender, const DownloadCompletedEventArgs& e) {
        (void)pSender;
        if (e.IsSuccess()) {
            Console::WriteLine("Download completed successfully!");
        } else {
            Console::WriteLine("Download terminated: " + e.GetError());
        }
    };

    Console::WriteLine("Starting file download...");
    downloader.Start();

    while (downloader.GetStatus() == DownloadStatus::Downloading) {
        Thread::Sleep(100);
    }

    return 0;
}
```
