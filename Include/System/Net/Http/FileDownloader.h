#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Action.h"
#include "System/SmartPointer.h"
#include "System/IO/Stream.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpRequestMessage.h"
#include <atomic>
#include <string>
#include <chrono>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class FileStream;
        }
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
                    long long TotalBytes{ 0 };
                    long long DownloadedBytes{ 0 };
                    long long RemainingBytes{ 0 };
                    double DownloadRateBytesPerSec{ 0.0 };
                    DownloadStatus Status{ DownloadStatus::NotStarted };
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
                    DOTNETDUPE_API void AddHeaders(const Collections::Generic::Dictionary<String, String>& headers);
                    DOTNETDUPE_API void SetUserAgent(const String& sUserAgent);

                private:
                    Collections::Generic::Dictionary<String, String> m_customHeaders;
                    String m_sUrl;
                    String m_sDestinationPath;
                    Action<DownloadProgress> m_progressCallback;

                    std::atomic<DownloadStatus> m_status{ DownloadStatus::NotStarted };
                    std::atomic<bool> m_bPauseRequested{ false };
                    std::atomic<long long> m_llTotalBytes{ 0 };
                    std::atomic<long long> m_llDownloadedBytes{ 0 };
                    std::atomic<double> m_dDownloadRate{ 0.0 };

                    HttpClient m_httpClient;

                    void DownloadLoop();
                    long long CheckExistingFileSize();
                    long long FetchContentLength();
                    SmartPointer<HttpRequestMessage> CreateGetRequest(long long llRangeStart);
                    void UpdateRate(long long llBytesInSession, const std::chrono::steady_clock::time_point& timeStart);
                    bool ReadAndWriteData(const SmartPointer<IO::Stream>& pStream, IO::FileStream& outFile);
                    void NotifyStateChange(DownloadStatus status);
                    void ExecuteDownload(long long llExistingBytes);
                    long long ParseContentLengthFromResponse(const SmartPointer<HttpResponseMessage>& pResponse);
                };

            }
        }
    }
}
