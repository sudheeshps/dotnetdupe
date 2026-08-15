#include "pch.h"
#include "System/Net/Http/FileDownloader.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/IOException.h"
#include "System/UnknownException.h"
#include "System/Console.h"
#include "System/Uri.h"
#include "System/Convert.h"
#include "System/IO/FileStream.h"
#include "System/IO/File.h"
#include "System/Threading/Thread.h"
#include <chrono>
#include <cstdlib>
#include <atomic>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                struct FileDownloader::Impl : public Object {
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
                    SmartPointer<Threading::Thread> m_pWorkerThread;

                    long long CheckExistingFileSize() {
                        if (!IO::File::Exists(m_sDestinationPath)) return 0;
                        try {
                            IO::FileStream existingFile(m_sDestinationPath, 2); // FileMode::Open
                            return existingFile.GetLength();
                        } catch (const IO::IOException& ex) {
                            Console::WriteLine(String("[FileDownloader] Error checking file size: ") + ex.What());
                            return 0;
                        } catch (const SystemException& ex) {
                            Console::WriteLine(String("[FileDownloader] System Exception checking file: ") + ex.What());
                            return 0;
                        }
                    }

                    long long ParseContentLengthFromResponse(const HttpResponseMessagePtr& pResponse) {
                        if (pResponse.IsNull()) return -1;
                        auto& headers = pResponse->GetHeaders();
                        for (auto const& [sKey, sVal] : headers) {
                            if (sKey.ToLower() == "content-length") {
                                return std::atoll(sVal.GetRawString());
                            }
                        }
                        return -1;
                    }

                    long long FetchContentLength() {
                        try {
                            Console::WriteLine(String("[FileDownloader] Sending HEAD request to URL: ") + m_sUrl);
                            auto pRequest = HttpRequestMessagePtr::NewShared(HttpMethod("HEAD"), Uri(m_sUrl));
                            pRequest->GetHeaders().Add("Accept", "*/*");
                            pRequest->GetHeaders().Add("User-Agent", "DotNetDupe-FileDownloader/1.0");
                            for (auto const& [sKey, sVal] : m_customHeaders) pRequest->GetHeaders()[sKey] = sVal;
                            auto pResponse = m_httpClient.Send(pRequest);
                            return ParseContentLengthFromResponse(pResponse);
                        } catch (const HttpRequestException& ex) {
                            Console::WriteLine(String("[FileDownloader] HEAD request failed: ") + ex.What());
                            return -1;
                        } catch (const Sockets::SocketException& ex) {
                            Console::WriteLine(String("[FileDownloader] Socket error on HEAD request: ") + ex.What());
                            return -1;
                        } catch (const SystemException& ex) {
                            Console::WriteLine(String("[FileDownloader] HEAD Exception: ") + ex.What());
                            return -1;
                        }
                    }

                    HttpRequestMessagePtr CreateGetRequest(long long llRangeStart) {
                        auto pRequest = HttpRequestMessagePtr::NewShared(HttpMethod::Get, Uri(m_sUrl));
                        pRequest->GetHeaders().Add("Accept", "*/*");
                        pRequest->GetHeaders().Add("User-Agent", "DotNetDupe-FileDownloader/1.0");
                        pRequest->GetHeaders().Add("Accept-Encoding", "identity");
                        pRequest->GetHeaders().Add("Connection", "keep-alive");

                        for (auto const& [sKey, sVal] : m_customHeaders) {
                            pRequest->GetHeaders()[sKey] = sVal;
                        }

                        if (llRangeStart > 0) {
                            pRequest->GetHeaders().Add("Range", String("bytes=") + Convert::ToString(llRangeStart) + "-");
                        }
                        return pRequest;
                    }

                    void UpdateRate(long long llBytesInSession, const std::chrono::steady_clock::time_point& timeStart) {
                        auto timeNow = std::chrono::steady_clock::now();
                        double dElapsedSec = std::chrono::duration<double>(timeNow - timeStart).count();
                        if (dElapsedSec > 0.05) {
                            m_dDownloadRate = static_cast<double>(llBytesInSession) / dElapsedSec;
                        }
                    }

                    DownloadProgress GetProgress() {
                        DownloadProgress progress;
                        progress.TotalBytes = m_llTotalBytes.load();
                        progress.DownloadedBytes = m_llDownloadedBytes.load();
                        progress.RemainingBytes = (progress.TotalBytes >= progress.DownloadedBytes) 
                                                  ? (progress.TotalBytes - progress.DownloadedBytes) : 0;
                        progress.DownloadRateBytesPerSec = m_dDownloadRate.load();
                        progress.Status = m_status.load();
                        return progress;
                    }

                    void ProcessDownloadChunk(int iRead, const char* pBuffer, IO::FileStream& outFile, long long& llBytesSession, const std::chrono::steady_clock::time_point& timeStart) {
                        outFile.Write(pBuffer, 0, iRead);
                        outFile.Flush();
                        m_llDownloadedBytes += iRead;
                        llBytesSession += iRead;
                        UpdateRate(llBytesSession, timeStart);
                        if (m_progressCallback) m_progressCallback.Invoke(GetProgress());
                    }

                    bool ReadAndWriteData(const SmartPointer<IO::Stream>& pStream, IO::FileStream& outFile) {
                        char pBuffer[8192];
                        auto timeStart = std::chrono::steady_clock::now();
                        long long llBytesSession = 0;
                        try {
                            while (!m_bPauseRequested.load()) {
                                int iRead = pStream->Read(pBuffer, 0, sizeof(pBuffer));
                                if (iRead <= 0) break;
                                ProcessDownloadChunk(iRead, pBuffer, outFile, llBytesSession, timeStart);
                            }
                        } catch (const Exception& ex) {
                            Console::WriteLine(String("[FileDownloader] Stream error: ") + ex.What());
                            return false;
                        }
                        return !m_bPauseRequested.load();
                    }

                    void NotifyStateChange(DownloadStatus status) {
                        m_status = status;
                        m_dDownloadRate = 0.0;
                        if (status == DownloadStatus::Failed) Console::WriteLine("[FileDownloader] Download status changed to Failed.");
                        else if (status == DownloadStatus::Completed) Console::WriteLine("[FileDownloader] Download completed successfully.");
                        else if (status == DownloadStatus::Paused) Console::WriteLine("[FileDownloader] Download paused.");
                        if (m_progressCallback) m_progressCallback.Invoke(GetProgress());
                    }

                    void ExecuteDownload(long long llExistingBytes) {
                        auto pRequest = CreateGetRequest(llExistingBytes);
                        HttpResponseMessagePtr pResponse;
                        Console::WriteLine(String("[FileDownloader] Requesting URL: ") + m_sUrl);
                        try {
                            pResponse = m_httpClient.Send(pRequest, HttpCompletionOption::ResponseHeadersRead);
                        } catch (const HttpRequestException& ex) {
                            Console::WriteLine(String("[FileDownloader] HTTP request failed: ") + ex.What());
                            return NotifyStateChange(DownloadStatus::Failed);
                        } catch (const Sockets::SocketException& ex) {
                            Console::WriteLine(String("[FileDownloader] Socket connection failed: ") + ex.What());
                            return NotifyStateChange(DownloadStatus::Failed);
                        } catch (const SystemException& ex) {
                            Console::WriteLine(String("[FileDownloader] HTTP client exception: ") + ex.What());
                            return NotifyStateChange(DownloadStatus::Failed);
                        }

                        if (pResponse.IsNull()) return NotifyStateChange(DownloadStatus::Failed);

                        int iStatusCode = static_cast<int>(pResponse->GetStatusCode());
                        if (iStatusCode == 301 || iStatusCode == 302 || iStatusCode == 307 || iStatusCode == 308) {
                            auto& headers = pResponse->GetHeaders();
                            for (auto const& [sKey, sVal] : headers) {
                                if (sKey.ToLower() == "location") {
                                    Console::WriteLine(String("[FileDownloader] Redirecting (") + Convert::ToString(iStatusCode) + ") to: " + sVal);
                                    m_sUrl = sVal;
                                    return ExecuteDownload(llExistingBytes);
                                }
                            }
                        }

                        bool bPartial = (iStatusCode == 206);
                        if (!bPartial && iStatusCode != 200) {
                            Console::WriteLine(String("[FileDownloader] HTTP server returned error status code: ") + Convert::ToString(iStatusCode));
                            return NotifyStateChange(DownloadStatus::Failed);
                        }

                        long long llRangeContentLen = ParseContentLengthFromResponse(pResponse);
                        int iFileMode = (bPartial ? 5 : 1); // FileMode::Append or FileMode::Create
                        if (!bPartial) {
                            m_llDownloadedBytes = 0;
                            llExistingBytes = 0;
                        }
                        if (llRangeContentLen >= 0) m_llTotalBytes = llExistingBytes + llRangeContentLen;

                        auto pStream = pResponse->GetContent()->ReadAsStream();
                        if (pStream.IsNull()) return NotifyStateChange(DownloadStatus::Failed);

                        try {
                            bool bCompleted = false;
                            {
                                IO::FileStream outFile(m_sDestinationPath, iFileMode);
                                bCompleted = ReadAndWriteData(pStream, outFile);
                            } // outFile goes out of scope and releases the lock here
                            NotifyStateChange(bCompleted ? DownloadStatus::Completed : DownloadStatus::Paused);
                        } catch (const IO::IOException& ex) {
                            Console::WriteLine(String("[FileDownloader] File I/O exception: ") + ex.What());
                            NotifyStateChange(DownloadStatus::Failed);
                        }
                    }

                    void DownloadLoop() {
                        long long llExistingBytes = CheckExistingFileSize();
                        m_llDownloadedBytes = llExistingBytes;

                        long long llTotalContentLen = FetchContentLength();
                        if (llTotalContentLen >= 0) m_llTotalBytes = llTotalContentLen;
                        else if (m_llTotalBytes.load() == 0) m_llTotalBytes = llExistingBytes;

                        try {
                            ExecuteDownload(llExistingBytes);
                        } catch (const Exception& ex) {
                            Console::WriteLine(String("[FileDownloader] Download loop failed: ") + ex.What());
                            NotifyStateChange(DownloadStatus::Failed);
                        } catch (const std::exception& ex) {
                            UnknownException unk(ex.what());
                            Console::WriteLine(String("[FileDownloader] Download loop failed: ") + unk.What());
                            NotifyStateChange(DownloadStatus::Failed);
                        } catch (...) {
                            UnknownException unk("An unknown error occurred during download.");
                            Console::WriteLine(String("[FileDownloader] Download loop failed: ") + unk.What());
                            NotifyStateChange(DownloadStatus::Failed);
                        }
                    }
                };


                FileDownloader::FileDownloader(const String& sUrl, const String& sDestinationPath)
                    : m_pImpl(SmartPointer<Impl>::NewShared()) {
                    m_pImpl->m_sUrl = sUrl;
                    m_pImpl->m_sDestinationPath = sDestinationPath;
                    if (sUrl.IsEmpty()) throw ArgumentException("sUrl cannot be empty.");
                    if (sDestinationPath.IsEmpty()) throw ArgumentException("sDestinationPath cannot be empty.");
                    String sLower = sUrl.ToLower();
                    if (!sLower.StartsWith("https://", false) && !sLower.StartsWith("http://", false)) {
                        throw ArgumentException("Only HTTP/HTTPS URLs are supported.");
                    }
                }

                FileDownloader::~FileDownloader() {
                    m_pImpl->m_bPauseRequested = true;
                }

                DownloadProgress FileDownloader::GetProgress() const {
                    return m_pImpl->GetProgress();
                }

                DownloadStatus FileDownloader::GetStatus() const {
                    return m_pImpl->m_status.load();
                }

                void FileDownloader::SetProgressCallback(const Action<DownloadProgress>& callback) {
                    m_pImpl->m_progressCallback = callback;
                }

                void FileDownloader::Pause() {
                    if (m_pImpl->m_status.load() == DownloadStatus::Downloading) {
                        m_pImpl->m_bPauseRequested = true;
                        Console::WriteLine("[FileDownloader] Pause requested.");
                    }
                }

                bool FileDownloader::Start() {
                    if (m_pImpl->m_status.load() == DownloadStatus::Downloading) return false;

                    m_pImpl->m_bPauseRequested = false;
                    m_pImpl->m_llDownloadedBytes = 0;
                    m_pImpl->m_llTotalBytes = 0;
                    m_pImpl->m_dDownloadRate = 0.0;
                    m_pImpl->m_status = DownloadStatus::Downloading;
                    Console::WriteLine(String("[FileDownloader] Starting download from ") + m_pImpl->m_sUrl);

                    auto pImpl = m_pImpl;
                    m_pImpl->m_pWorkerThread = SmartPointer<Threading::Thread>::NewShared(Threading::ThreadStart([pImpl]() { pImpl->DownloadLoop(); }));
                    m_pImpl->m_pWorkerThread->Start();
                    return true;
                }

                bool FileDownloader::Resume() {
                    DownloadStatus currentStatus = m_pImpl->m_status.load();
                    if (currentStatus == DownloadStatus::Downloading || currentStatus == DownloadStatus::Completed) return false;

                    m_pImpl->m_bPauseRequested = false;
                    m_pImpl->m_status = DownloadStatus::Downloading;
                    Console::WriteLine(String("[FileDownloader] Resuming download for ") + m_pImpl->m_sDestinationPath);

                    auto pImpl = m_pImpl;
                    m_pImpl->m_pWorkerThread = SmartPointer<Threading::Thread>::NewShared(Threading::ThreadStart([pImpl]() { pImpl->DownloadLoop(); }));
                    m_pImpl->m_pWorkerThread->Start();
                    return true;
                }

                void FileDownloader::AddHeaders(const Collections::Generic::Dictionary<String, String>& headers) {
                    for (auto const& [sKey, sVal] : headers) {
                        m_pImpl->m_customHeaders[sKey] = sVal;
                    }
                }

                void FileDownloader::SetUserAgent(const String& sUserAgent) {
                    m_pImpl->m_customHeaders["User-Agent"] = sUserAgent;
                }

            }
        }
    }
}

