#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/EventArgs.h"
#include "System/EventHandler.h"
#include "System/SmartPointer.h"
#include "System/IO/Stream.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpRequestMessage.h"

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

                class DownloadProgressChangedEventArgs : public EventArgs {
                public:
                    DownloadProgressChangedEventArgs(long long llBytesReceived, long long llTotalBytesToReceive,
                                                     double dProgressPercentage, double dDownloadRateBytesPerSec,
                                                     DownloadStatus status)
                        : m_llBytesReceived(llBytesReceived),
                          m_llTotalBytesToReceive(llTotalBytesToReceive),
                          m_dProgressPercentage(dProgressPercentage),
                          m_dDownloadRateBytesPerSec(dDownloadRateBytesPerSec),
                          m_status(status) {}

                    long long GetBytesReceived() const { return m_llBytesReceived; }
                    long long GetTotalBytesToReceive() const { return m_llTotalBytesToReceive; }
                    double GetProgressPercentage() const { return m_dProgressPercentage; }
                    double GetDownloadRateBytesPerSec() const { return m_dDownloadRateBytesPerSec; }
                    DownloadStatus GetStatus() const { return m_status; }

                private:
                    long long m_llBytesReceived;
                    long long m_llTotalBytesToReceive;
                    double m_dProgressPercentage;
                    double m_dDownloadRateBytesPerSec;
                    DownloadStatus m_status;
                };

                class DownloadCompletedEventArgs : public EventArgs {
                public:
                    DownloadCompletedEventArgs(bool bSuccess, bool bCancelled, const String& sError)
                        : m_bSuccess(bSuccess),
                          m_bCancelled(bCancelled),
                          m_sError(sError) {}

                    bool IsSuccess() const { return m_bSuccess; }
                    bool IsCancelled() const { return m_bCancelled; }
                    String GetError() const { return m_sError; }

                private:
                    bool m_bSuccess;
                    bool m_bCancelled;
                    String m_sError;
                };

                class FileDownloader : public Object {
                private:
                    struct Impl;
                    SmartPointer<Impl> m_pImpl;

                public:
                    EventHandler<DownloadProgressChangedEventArgs>& DownloadProgressChanged;
                    EventHandler<DownloadCompletedEventArgs>& DownloadCompleted;

                    DOTNETDUPE_API FileDownloader(const String& sUrl, const String& sDestinationPath);
                    DOTNETDUPE_API ~FileDownloader() override;

                    DOTNETDUPE_API bool Start();
                    DOTNETDUPE_API void Pause();
                    DOTNETDUPE_API bool Resume();

                    DOTNETDUPE_API DownloadProgress GetProgress() const;
                    DOTNETDUPE_API DownloadStatus GetStatus() const;

                    DOTNETDUPE_API void AddHeaders(const Collections::Generic::Dictionary<String, String>& headers);
                    DOTNETDUPE_API void SetUserAgent(const String& sUserAgent);
                };

            }
        }
    }
}
