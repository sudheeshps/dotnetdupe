/// \file FileDownloader.h
/// \brief Resumable, multi-threaded HTTP file downloader with progress tracking and event notifications.

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

                /// \enum DownloadStatus
                /// \brief State machine values representing the lifecycle of an ongoing file download.
                enum class DownloadStatus {
                    /// \brief Transfer has not been initiated.
                    NotStarted,
                    /// \brief Transfer is actively receiving bytes over the network.
                    Downloading,
                    /// \brief Transfer is temporarily suspended.
                    Paused,
                    /// \brief Transfer finished successfully and file was verified.
                    Completed,
                    /// \brief Transfer encountered an unrecoverable network or disk I/O failure.
                    Failed
                };

                /// \struct DownloadProgress
                /// \brief Snapshot of download telemetry metrics including transferred bytes and bandwidth speed.
                struct DownloadProgress {
                    /// \brief Total expected length in bytes from HTTP Content-Length header, or 0 if chunked.
                    long long TotalBytes{ 0 };
                    /// \brief Number of bytes written to local destination file.
                    long long DownloadedBytes{ 0 };
                    /// \brief Estimated remaining bytes.
                    long long RemainingBytes{ 0 };
                    /// \brief Instantaneous transfer speed in bytes per second.
                    double DownloadRateBytesPerSec{ 0.0 };
                    /// \brief Current lifecycle state of the download task.
                    DownloadStatus Status{ DownloadStatus::NotStarted };
                };

                /// \class DownloadProgressChangedEventArgs
                /// \brief Event arguments emitted periodically during byte transfer.
                class DownloadProgressChangedEventArgs : public EventArgs {
                public:
                    /// \brief Initializes a new instance with current metrics snapshot.
                    /// \param llBytesReceived Cumulative bytes written so far.
                    /// \param llTotalBytesToReceive Expected total file size.
                    /// \param dProgressPercentage Percentage completion (0.0 to 100.0).
                    /// \param dDownloadRateBytesPerSec Current transfer rate in bytes/sec.
                    /// \param status Current download status.
                    DownloadProgressChangedEventArgs(long long llBytesReceived, long long llTotalBytesToReceive,
                                                     double dProgressPercentage, double dDownloadRateBytesPerSec,
                                                     DownloadStatus status)
                        : m_llBytesReceived(llBytesReceived),
                          m_llTotalBytesToReceive(llTotalBytesToReceive),
                          m_dProgressPercentage(dProgressPercentage),
                          m_dDownloadRateBytesPerSec(dDownloadRateBytesPerSec),
                          m_status(status) {}

                    /// \brief Gets the total bytes received so far.
                    long long GetBytesReceived() const { return m_llBytesReceived; }
                    /// \brief Gets the total size in bytes expected to receive.
                    long long GetTotalBytesToReceive() const { return m_llTotalBytesToReceive; }
                    /// \brief Gets progress percentage between 0.0 and 100.0.
                    double GetProgressPercentage() const { return m_dProgressPercentage; }
                    /// \brief Gets the current download rate in bytes per second.
                    double GetDownloadRateBytesPerSec() const { return m_dDownloadRateBytesPerSec; }
                    /// \brief Gets the current download status.
                    DownloadStatus GetStatus() const { return m_status; }

                private:
                    long long m_llBytesReceived;
                    long long m_llTotalBytesToReceive;
                    double m_dProgressPercentage;
                    double m_dDownloadRateBytesPerSec;
                    DownloadStatus m_status;
                };

                /// \class DownloadCompletedEventArgs
                /// \brief Event arguments emitted when transfer finishes or fails.
                class DownloadCompletedEventArgs : public EventArgs {
                public:
                    /// \brief Initializes completion event arguments.
                    /// \param bSuccess true if download fully succeeded.
                    /// \param bCancelled true if cancelled by user request.
                    /// \param sError Error description string if failed.
                    DownloadCompletedEventArgs(bool bSuccess, bool bCancelled, const String& sError)
                        : m_bSuccess(bSuccess),
                          m_bCancelled(bCancelled),
                          m_sError(sError) {}

                    /// \brief Indicates whether the file was successfully downloaded.
                    bool IsSuccess() const { return m_bSuccess; }
                    /// \brief Indicates whether the download was intentionally cancelled.
                    bool IsCancelled() const { return m_bCancelled; }
                    /// \brief Gets error information if an exception occurred during transfer.
                    String GetError() const { return m_sError; }

                private:
                    bool m_bSuccess;
                    bool m_bCancelled;
                    String m_sError;
                };

                /// \class FileDownloader
                /// \brief Coordinates asynchronous, chunked, and resumable file downloads with pause/resume support.
                ///
                /// Conforms to RFC 9110 (HTTP Range Requests).
                class FileDownloader : public Object {
                private:
                    struct Impl;
                    SmartPointer<Impl> m_pImpl;

                public:
                    /// \brief Multicast event triggered when chunk progress occurs.
                    EventHandler<DownloadProgressChangedEventArgs>& DownloadProgressChanged;
                    /// \brief Multicast event triggered when transfer completes or terminates.
                    EventHandler<DownloadCompletedEventArgs>& DownloadCompleted;

                    /// \brief Initializes a new instance of FileDownloader with source URL and destination path.
                    /// \param sUrl Remote HTTP/HTTPS download source.
                    /// \param sDestinationPath Local filesystem destination file path.
                    DOTNETDUPE_API FileDownloader(const String& sUrl, const String& sDestinationPath);
                    /// \brief Cancels active downloads and releases resources.
                    DOTNETDUPE_API ~FileDownloader() override;

                    /// \brief Begins the asynchronous download on a background worker thread.
                    /// \return true if started successfully; false if already running.
                    DOTNETDUPE_API bool Start();

                    /// \brief Suspends byte streaming without deleting partially received data.
                    DOTNETDUPE_API void Pause();

                    /// \brief Resumes a paused download using HTTP Range headers if supported.
                    /// \return true if resumed; false otherwise.
                    DOTNETDUPE_API bool Resume();

                    /// \brief Queries instantaneous download telemetry metrics.
                    /// \return Current DownloadProgress snapshot.
                    DOTNETDUPE_API DownloadProgress GetProgress() const;

                    /// \brief Queries current operational status.
                    /// \return Current DownloadStatus value.
                    DOTNETDUPE_API DownloadStatus GetStatus() const;

                    /// \brief Appends custom HTTP request headers to outgoing requests.
                    /// \param headers Dictionary containing header key-value pairs.
                    DOTNETDUPE_API void AddHeaders(const Collections::Generic::Dictionary<String, String>& headers);

                    /// \brief Overrides the default User-Agent request header string.
                    /// \param sUserAgent Custom User-Agent string.
                    DOTNETDUPE_API void SetUserAgent(const String& sUserAgent);
                };

            }
        }
    }
}
