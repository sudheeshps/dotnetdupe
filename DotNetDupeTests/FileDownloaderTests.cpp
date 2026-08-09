#include "pch.h"
#include "gtest/gtest.h"
#include "System/Net/Http/FileDownloader.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Security/SslStream.h"
#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/Convert.h"
#include "System/Threading/Thread.h"
#include "System/ArgumentException.h"
#include <thread>
#include <string>
#include <vector>
#include <filesystem>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Net::Security;
using namespace DotNetDupe::System::Security::Cryptography::X509Certificates;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Threading;

namespace DotNetDupeTests {

    static const char* s_sTestCertPem =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDCTCCAfGgAwIBAgIUOw6LfZ3oCoBlz1U0uvdKQArrtkkwDQYJKoZIhvcNAQEL\n"
        "BQAwFDESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTI2MDYxNjA3NTAzN1oXDTM2MDYx\n"
        "MzA3NTAzN1owFDESMBAGA1UEAwwJbG9jYWxob3N0MIIBIjANBgkqhkiG9w0BAQEF\n"
        "AAOCAQ8AMIIBCgKCAQEA3uZU9MHVG1hQB98STU7HfWOvsvnNVdQShmWp8uxpYua3\n"
        "dQ/6hYfTCISDK82QQ4trv9qBN33QzT0PbfQVg/+EZe+mCZdSOlxR61AKpApTY8GG\n"
        "n+T4Vc+WJKUPlEkYY8eP8hP0SQNjH+oPhjvEyHhYL0+Bgv2bhi8QhrSzkG/2C28u\n"
        "jmCWz19nj0+x2HJRcQzQ2NyBGfyY1SecSs+aZ49hpAgGL4hdgqP7klQNnIjyICt9\n"
        "ldMYIyMxEOIyZFByVjCda2Kyet+UFZUxDFxOnqpUOLpWNhnGd4L9S94xS/UE1PC0\n"
        "v/QYa2AwR3+4BCLZLuXllPTgdaSWRn5TFDkAojE2dQIDAQABo1MwUTAdBgNVHQ4E\n"
        "FgQUIJd2NudarHCx1F6ev4BNf/KTKM4wHwYDVR0jBBgwFoAUIJd2NudarHCx1F6e\n"
        "v4BNf/KTKM4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAdDVb\n"
        "AgxQIyknjuLEjhyjYlOBfvxmfneuMULa2EZcx6C+gZW+WvrKttMl972RP6Bj71Rc\n"
        "KaXUpf3l2MuRqNIVNj7gWIPyQDN7bJpY0zxTneRMNgYwLn+YxB5b3AZb2/1XsfvJ\n"
        "ynncvVbLkSKHxMJHj/BMUbNkea4Z9RuU5aoBsBCO/GCYfQsf0g5aWcXCpepV6/Dq\n"
        "ur+G8JIUFCmjihPipYjDLPw4IHtuKQMKr43rZHMoAeyfphbX6RlS0NH0X0LOGTOL\n"
        "CGariC58/NHl8dbdQz3JZ/EI5D+ZK7QgnEBoFrot6lLfEJHZEYrmn2WuyBmxHZNC\n"
        "p5nX46yXDcMYhJlhEg==\n"
        "-----END CERTIFICATE-----\n";

    static const char* s_sTestKeyPem =
        "-----BEGIN PRIVATE KEY-----\n"
        "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDe5lT0wdUbWFAH\n"
        "3xJNTsd9Y6+y+c1V1BKGZany7Gli5rd1D/qFh9MIhIMrzZBDi2u/2oE3fdDNPQ9t\n"
        "9BWD/4Rl76YJl1I6XFHrUAqkClNjwYaf5PhVz5YkpQ+USRhjx4/yE/RJA2Mf6g+G\n"
        "O8TIeFgvT4GC/ZuGLxCGtLOQb/YLby6OYJbPX2ePT7HYclFxDNDY3IEZ/JjVJ5xK\n"
        "z5pnj2GkCAYviF2Co/uSVA2ciPIgK32V0xgjIzEQ4jJkUHJWMJ1rYrJ635QVlTEM\n"
        "XE6eqlQ4ulY2GcZ3gv1L3jFL9QTU8LS/9BhrYDBHf7gEItku5eWU9OB1pJZGflMU\n"
        "OQCiMTZ1AgMBAAECggEAA1cEMt6sqMo7+MI47fueNZzGxzHDB+QzmtvvHfq44Yh/\n"
        "ezuQD2wnl1sU9BUSclCmfqtxgDlcZZbiok0j/WRYwYZDwKUJ0GINwC7k2exlr3r4\n"
        "33B44CBKKaZng1tNCEG3QHNYF7f7RkEllmdS3YYgHCVe1W2jZPFTmBEhF6r1ztoG\n"
        "eSAw6FaEEPzCUUNiMP2Cc3/MUEjdbs+kYPIoTab7HoKGoWTz4GN+DlCDB8hxwjZw\n"
        "PRScWmlH79ji3IqH7zbKZf7vXIUxz2JyMh9Bplbs0FZU3NFXUFBA0wn3Zk0PZsL0\n"
        "hPMBo7G1WoMskilJydhFI2MJyMKgsI2Q8IeXRqZmQQKBgQD2CXqEmf7txv3ELTM3\n"
        "weavoXpFkb/Uc6PjDMKNkxG5stqmtZAkFrr25966khiYS3GZvOSsR0IvZ8ZmZIiU\n"
        "iebewmUHeiTcZxUHL2pxc7zXpV3cO2o0+EFnRQZUdYa6imursw5YmSjCdSSpGV2k\n"
        "HFN9qy0mpHrcO8OkJPxpeCe2SQKBgQDn7QCnL8q7m0QIQPY/pdeL1ldVDvgEmIBI\n"
        "/cfm0YyTOC2Y2X3jkJJaOSt1YBU6YGfqOrrzrIc8zemPJXqT0+t5od2CehEuaAwB\n"
        "DAMhDZSM7k4WGFoG5hT4wMV/9h2NmKJrnfdSq0hcjvWgjoWciGO/EuxwWrOECA48\n"
        "uerIogpOzQKBgB2neM/gq7xG/rEVbZXr1f0XLmCSN5fItFkaYZykvD8wknADdDMV\n"
        "VkWKHutHzpUUisWn4bvt/skt7djLhza4BE6XCYxEKYszFXK/QUoZVAp90PlJY/ys\n"
        "a12iESg+3hU/t8iDFViSZTpC2j9TYlj0XQ/JgPCmZHSE84imOJIRvF+ZAoGAChmS\n"
        "G7XDGGRslAdwMdlhB5/v8a5TMrAJdiRHHlAl1QtvQObHyq8zEGbI87WZdw9XtyWT\n"
        "mqT6xrUY5Lj3G5Tw/j0ETnhOYzz4uNrzD+9tdq2yDNhGo2JFq4eJkkb/hnkNKa6Z\n"
        "ZKo83nsDS1ipjOWLoniKf2ITyz4mkJJwHQihdkkCgYEA07WQ8VbbHzqkXBdTizMb\n"
        "cmg1Fcr7Pa+oLdVq6kR1scG2zO07Bdn7QazJJONoWoBgdXYuTAJ4BFwDktJ75iLW\n"
        "gReIy0WeSxPdEUIzHnPIORC6ksYPoV/J2unCEUX1OrATd2ubAmTVgcIREqbyh3IB\n"
        "vy4bNlvZuoWwBMUqyVyUNlv=\n"
        "-----END PRIVATE KEY-----\n";

    struct ServerGuard {
        std::thread& t;
        TcpListener& l;
        ~ServerGuard() {
            l.Stop();
            if (t.joinable()) {
                t.join();
            }
        }
    };

    TEST(FileDownloaderTests, GivenInvalidConstructorArgs_WhenInstantiated_ThenThrowsArgumentException) {
        // Given / When / Then
        EXPECT_THROW(FileDownloader downloader("", "test.bin"), ArgumentException);
        EXPECT_THROW(FileDownloader downloader("https://localhost:8080/file", ""), ArgumentException);
        EXPECT_THROW(FileDownloader downloader("ftp://localhost:8080/file", "test.bin"), ArgumentException);
    }

    TEST(FileDownloaderTests, GivenInvalidUrl_WhenStarted_ThenStatusIsFailed) {
        // Given
        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "test_invalid_url.bin" });
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

        FileDownloader downloader("https://invalid.domain.that.does.not.exist.local/file.bin", sDest);

        // When
        bool bStarted = downloader.Start();
        EXPECT_TRUE(bStarted);

        // Wait for failure
        int iRetries = 0;
        while (downloader.GetStatus() == DownloadStatus::Downloading && iRetries < 50) {
            Thread::Sleep(50);
            iRetries++;
        }

        // Then
        EXPECT_EQ(downloader.GetStatus(), DownloadStatus::Failed);
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);
    }

    TEST(FileDownloaderTests, GivenMockHttpServer_WhenDownloadingFile_ThenCompletesSuccessfully) {
        // Given
        int iPort = 18881;
        TcpListener server(iPort);
        server.Start();

        auto pathTempDir = std::filesystem::temp_directory_path();
        auto pathCert = pathTempDir / "temp_fd_cert.pem";
        auto pathKey = pathTempDir / "temp_fd_key.pem";
        IO::File::WriteAllText(String(pathCert.string().c_str()), s_sTestCertPem);
        IO::File::WriteAllText(String(pathKey.string().c_str()), s_sTestKeyPem);
        auto spCert = SmartPointer<X509Certificate2>::NewShared(String(pathCert.string().c_str()), String(pathKey.string().c_str()));

        std::string sPayload = "0123456789ABCDEF0123456789ABCDEF"; // 32 bytes
        std::thread serverThread([&server, spCert, sPayload]() {
            try {
                // HEAD request response over SSL
                auto clientHead = server.AcceptTcpClient();
                auto sslHead = SmartPointer<SslStream>::NewShared(clientHead->GetStream(), false);
                sslHead->AuthenticateAsServer(spCert);
                char buf[512];
                sslHead->Read(buf, 0, 512);

                std::string sHeadResp = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(sPayload.size()) + "\r\nConnection: close\r\n\r\n";
                sslHead->Write(sHeadResp.data(), 0, static_cast<int>(sHeadResp.size()));
                sslHead->Dispose();
                clientHead->Close();

                // GET request response over SSL
                auto clientGet = server.AcceptTcpClient();
                auto sslGet = SmartPointer<SslStream>::NewShared(clientGet->GetStream(), false);
                sslGet->AuthenticateAsServer(spCert);
                sslGet->Read(buf, 0, 512);

                std::string sGetResp = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(sPayload.size()) + "\r\nConnection: close\r\n\r\n" + sPayload;
                sslGet->Write(sGetResp.data(), 0, static_cast<int>(sGetResp.size()));
                sslGet->Dispose();
                clientGet->Close();
            } catch (...) {}
        });
        ServerGuard guard{ serverThread, server };

        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "test_download_full.bin" });
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

        String sUrl = String("https://127.0.0.1:") + Convert::ToString(iPort) + "/file.bin";
        FileDownloader downloader(sUrl, sDest);
        downloader.SetUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

        bool bCallbackInvoked = false;
        downloader.SetProgressCallback([&bCallbackInvoked](DownloadProgress progress) {
            bCallbackInvoked = true;
        });

        // When
        downloader.Start();

        int iRetries = 0;
        while (downloader.GetStatus() == DownloadStatus::Downloading && iRetries < 50) {
            Thread::Sleep(50);
            iRetries++;
        }

        // Then
        EXPECT_EQ(downloader.GetStatus(), DownloadStatus::Completed);
        EXPECT_TRUE(IO::File::Exists(sDest));
        EXPECT_EQ(IO::File::ReadAllText(sDest), String(sPayload.c_str()));
        EXPECT_TRUE(bCallbackInvoked);

        DownloadProgress progress = downloader.GetProgress();
        EXPECT_EQ(progress.DownloadedBytes, static_cast<long long>(sPayload.size()));
        EXPECT_EQ(progress.TotalBytes, static_cast<long long>(sPayload.size()));
        EXPECT_EQ(progress.RemainingBytes, 0);

        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);
        IO::File::Delete(String(pathCert.string().c_str()));
        IO::File::Delete(String(pathKey.string().c_str()));
    }

    TEST(FileDownloaderTests, GivenPartialFile_WhenResuming_ThenSendsRangeHeaderAndCompletes) {
        // Given
        int iPort = 18882;
        TcpListener server(iPort);
        server.Start();

        auto pathTempDir = std::filesystem::temp_directory_path();
        auto pathCert = pathTempDir / "temp_fd_res_cert.pem";
        auto pathKey = pathTempDir / "temp_fd_res_key.pem";
        IO::File::WriteAllText(String(pathCert.string().c_str()), s_sTestCertPem);
        IO::File::WriteAllText(String(pathKey.string().c_str()), s_sTestKeyPem);
        auto spCert = SmartPointer<X509Certificate2>::NewShared(String(pathCert.string().c_str()), String(pathKey.string().c_str()));

        std::string sPart1 = "0123456789"; // 10 bytes
        std::string sPart2 = "ABCDEF0123456789ABCDEF"; // 22 bytes
        std::thread serverThread([&server, spCert, sPart2]() {
            try {
                // HEAD request response over SSL
                auto clientHead = server.AcceptTcpClient();
                auto sslHead = SmartPointer<SslStream>::NewShared(clientHead->GetStream(), false);
                sslHead->AuthenticateAsServer(spCert);
                char buf[512];
                sslHead->Read(buf, 0, 512);

                std::string sHeadResp = "HTTP/1.1 200 OK\r\nContent-Length: 32\r\nConnection: close\r\n\r\n";
                sslHead->Write(sHeadResp.data(), 0, static_cast<int>(sHeadResp.size()));
                sslHead->Dispose();
                clientHead->Close();

                // GET Range request response over SSL
                auto clientGet = server.AcceptTcpClient();
                auto sslGet = SmartPointer<SslStream>::NewShared(clientGet->GetStream(), false);
                sslGet->AuthenticateAsServer(spCert);
                int iRead = sslGet->Read(buf, 0, 512);
                std::string sReq(buf, iRead);

                EXPECT_NE(sReq.find("Range: bytes=10-"), std::string::npos);

                std::string sGetResp = "HTTP/1.1 206 Partial Content\r\nContent-Length: " + std::to_string(sPart2.size()) + "\r\nConnection: close\r\n\r\n" + sPart2;
                sslGet->Write(sGetResp.data(), 0, static_cast<int>(sGetResp.size()));
                sslGet->Dispose();
                clientGet->Close();
            } catch (...) {}
        });
        ServerGuard guard{ serverThread, server };

        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "test_download_resume.bin" });
        IO::File::WriteAllText(sDest, String(sPart1.c_str()));

        String sUrl = String("https://127.0.0.1:") + Convert::ToString(iPort) + "/file.bin";
        FileDownloader downloader(sUrl, sDest);

        // When
        downloader.Resume();

        int iRetries = 0;
        while (downloader.GetStatus() == DownloadStatus::Downloading && iRetries < 50) {
            Thread::Sleep(50);
            iRetries++;
        }

        // Then
        EXPECT_EQ(downloader.GetStatus(), DownloadStatus::Completed);
        EXPECT_TRUE(IO::File::Exists(sDest));
        EXPECT_EQ(IO::File::ReadAllText(sDest), String("0123456789ABCDEF0123456789ABCDEF"));

        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);
        IO::File::Delete(String(pathCert.string().c_str()));
        IO::File::Delete(String(pathKey.string().c_str()));
    }

    TEST(FileDownloaderTests, GivenOngoingDownload_WhenPaused_ThenStatusChangesToPaused) {
        // Given
        int iPort = 18883;
        TcpListener server(iPort);
        server.Start();

        auto pathTempDir = std::filesystem::temp_directory_path();
        auto pathCert = pathTempDir / "temp_fd_pau_cert.pem";
        auto pathKey = pathTempDir / "temp_fd_pau_key.pem";
        IO::File::WriteAllText(String(pathCert.string().c_str()), s_sTestCertPem);
        IO::File::WriteAllText(String(pathKey.string().c_str()), s_sTestKeyPem);
        auto spCert = SmartPointer<X509Certificate2>::NewShared(String(pathCert.string().c_str()), String(pathKey.string().c_str()));

        std::thread serverThread([&server, spCert]() {
            try {
                // HEAD response over SSL
                auto clientHead = server.AcceptTcpClient();
                auto sslHead = SmartPointer<SslStream>::NewShared(clientHead->GetStream(), false);
                sslHead->AuthenticateAsServer(spCert);
                char buf[512];
                sslHead->Read(buf, 0, 512);

                std::string sHeadResp = "HTTP/1.1 200 OK\r\nContent-Length: 100000\r\nConnection: close\r\n\r\n";
                sslHead->Write(sHeadResp.data(), 0, static_cast<int>(sHeadResp.size()));
                sslHead->Dispose();
                clientHead->Close();

                // GET response over SSL - write slowly
                auto clientGet = server.AcceptTcpClient();
                auto sslGet = SmartPointer<SslStream>::NewShared(clientGet->GetStream(), false);
                sslGet->AuthenticateAsServer(spCert);
                sslGet->Read(buf, 0, 512);

                std::string sGetHeader = "HTTP/1.1 200 OK\r\nContent-Length: 100000\r\nConnection: close\r\n\r\n";
                sslGet->Write(sGetHeader.data(), 0, static_cast<int>(sGetHeader.size()));

                std::string chunk(1000, 'X');
                for (int i = 0; i < 50; ++i) {
                    sslGet->Write(chunk.data(), 0, static_cast<int>(chunk.size()));
                    Thread::Sleep(20);
                }
                sslGet->Dispose();
                clientGet->Close();
            } catch (...) {}
        });
        ServerGuard guard{ serverThread, server };

        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "test_download_pause.bin" });
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

        String sUrl = String("https://127.0.0.1:") + Convert::ToString(iPort) + "/file.bin";
        FileDownloader downloader(sUrl, sDest);

        downloader.Start();
        Thread::Sleep(50);

        // When
        downloader.Pause();

        int iRetries = 0;
        while (downloader.GetStatus() == DownloadStatus::Downloading && iRetries < 50) {
            Thread::Sleep(50);
            iRetries++;
        }

        // Then
        EXPECT_EQ(downloader.GetStatus(), DownloadStatus::Paused);

        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);
        IO::File::Delete(String(pathCert.string().c_str()));
        IO::File::Delete(String(pathKey.string().c_str()));
    }

    TEST(FileDownloaderTests, GivenRedirectResponse_WhenDownloadingFile_ThenFollowsLocationHeader) {
        // Given
        int iPort = 18884;
        TcpListener server(iPort);
        server.Start();

        auto pathTempDir = std::filesystem::temp_directory_path();
        auto pathCert = pathTempDir / "temp_fd_red_cert.pem";
        auto pathKey = pathTempDir / "temp_fd_red_key.pem";
        IO::File::WriteAllText(String(pathCert.string().c_str()), s_sTestCertPem);
        IO::File::WriteAllText(String(pathKey.string().c_str()), s_sTestKeyPem);
        auto spCert = SmartPointer<X509Certificate2>::NewShared(String(pathCert.string().c_str()), String(pathKey.string().c_str()));

        std::string sPayload = "RedirectDataContent123";
        std::thread serverThread([&server, spCert, iPort, sPayload]() {
            try {
                // HEAD response
                auto clientHead = server.AcceptTcpClient();
                auto sslHead = SmartPointer<SslStream>::NewShared(clientHead->GetStream(), false);
                sslHead->AuthenticateAsServer(spCert);
                char buf[512];
                sslHead->Read(buf, 0, 512);
                std::string sHeadResp = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(sPayload.size()) + "\r\nConnection: close\r\n\r\n";
                sslHead->Write(sHeadResp.data(), 0, static_cast<int>(sHeadResp.size()));
                sslHead->Dispose();
                clientHead->Close();

                // GET 302 Redirect response
                auto clientGet1 = server.AcceptTcpClient();
                auto sslGet1 = SmartPointer<SslStream>::NewShared(clientGet1->GetStream(), false);
                sslGet1->AuthenticateAsServer(spCert);
                sslGet1->Read(buf, 0, 512);

                std::string sTargetUrl = "https://127.0.0.1:" + std::to_string(iPort) + "/final_target.bin";
                std::string s302Resp = "HTTP/1.1 302 Found\r\nLocation: " + sTargetUrl + "\r\nConnection: close\r\n\r\n";
                sslGet1->Write(s302Resp.data(), 0, static_cast<int>(s302Resp.size()));
                sslGet1->Dispose();
                clientGet1->Close();

                // GET final request response
                auto clientGet2 = server.AcceptTcpClient();
                auto sslGet2 = SmartPointer<SslStream>::NewShared(clientGet2->GetStream(), false);
                sslGet2->AuthenticateAsServer(spCert);
                sslGet2->Read(buf, 0, 512);

                std::string sGetResp2 = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(sPayload.size()) + "\r\nConnection: close\r\n\r\n" + sPayload;
                sslGet2->Write(sGetResp2.data(), 0, static_cast<int>(sGetResp2.size()));
                sslGet2->Dispose();
                clientGet2->Close();
            } catch (...) {}
        });
        ServerGuard guard{ serverThread, server };

        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "test_download_redirect.bin" });
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

        String sInitialUrl = String("https://127.0.0.1:") + Convert::ToString(iPort) + "/initial.bin";
        FileDownloader downloader(sInitialUrl, sDest);

        // When
        downloader.Start();

        int iRetries = 0;
        while (downloader.GetStatus() == DownloadStatus::Downloading && iRetries < 50) {
            Thread::Sleep(50);
            iRetries++;
        }

        // Then
        EXPECT_EQ(downloader.GetStatus(), DownloadStatus::Completed);
        EXPECT_TRUE(IO::File::Exists(sDest));
        EXPECT_EQ(IO::File::ReadAllText(sDest), String(sPayload.c_str()));

        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);
        IO::File::Delete(String(pathCert.string().c_str()));
        IO::File::Delete(String(pathKey.string().c_str()));
    }

}
