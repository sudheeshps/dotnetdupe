# FileDownloader, DownloadProgress &amp; DownloadStatus

**Namespace:** `DotNetDupe::System::Net::Http`  
**Header:** `#include "System/Net/Http/FileDownloader.h"`

A high-level HTTP/HTTPS file download manager with progress tracking, transfer speed measurement, pause/resume capability, custom HTTP headers, and deterministic file handle closing.

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

## `DownloadProgress` Struct

```cpp
struct DownloadProgress {
    long long TotalBytes;
    long long DownloadedBytes;
    long long RemainingBytes;
    double DownloadRateBytesPerSec;
    DownloadStatus Status;
};
```

---

## `FileDownloader` Class

### Syntax
```cpp
class FileDownloader : public Object;
```

---

## Constructors

### `FileDownloader(const String& sUrl, const String& sDestinationPath)`
Initializes a new `FileDownloader` targeting a remote URL and saving to a local destination file path.

---

## Member Functions

### `bool Start()`
Begins downloading the file. Returns `true` upon completion or `false` on failure.

### `void Pause()`
Pauses the active download.

### `bool Resume()`
Resumes a paused download, requesting remaining bytes using HTTP range requests.

### `DownloadProgress GetProgress() const`
Gets the current progress snapshot (total bytes, bytes transferred, speed in B/s, status).

### `DownloadStatus GetStatus() const`
Gets the current download state.

### `void SetProgressCallback(const Action<DownloadProgress>& callback)`
Registers a delegate to be notified as download chunks are transferred.

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

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;

int main() {
    FileDownloader downloader(
        "https://speed.cloudflare.com/__down?bytes=10485760", 
        "downloaded_10mb.dat"
    );

    downloader.SetUserAgent("DotNetDupe-Downloader/4.0");

    downloader.SetProgressCallback([](DownloadProgress progress) {
        double pct = (progress.TotalBytes > 0) 
            ? ((double)progress.DownloadedBytes / progress.TotalBytes * 100.0) 
            : 0.0;
        double speedKbps = progress.DownloadRateBytesPerSec / 1024.0;
        Console::WriteLine("Progress: {0:F1}% ({1} KB/s)", pct, speedKbps);
    });

    Console::WriteLine("Starting file download...");
    bool success = downloader.Start();
    Console::WriteLine("Download result: {0}", success ? "Success" : "Failed");

    return 0;
}
```
