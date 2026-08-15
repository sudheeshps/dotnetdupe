#include "pch.h"
#include "gtest/gtest.h"
#include "System/Net/Security/SslStream.h"
#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Threading/Thread.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/IO/File.h"
#include <thread>
#include <string>
#include <filesystem>
#include <iostream>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
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
        "vy4bNlvZuoWwBMUqyVyUNlw=\n"
        "-----END PRIVATE KEY-----\n";

    class SslStreamTests : public ::testing::Test {
    protected:
        String m_sCertPath;
        String m_sKeyPath;
        static int s_iTestCounter;

        void SetUp() override {
            int iId = s_iTestCounter++;
            auto pathTempDir = std::filesystem::temp_directory_path();
            auto pathCert = pathTempDir / ("temp_test_cert_" + std::to_string(iId) + ".pem");
            auto pathKey = pathTempDir / ("temp_test_key_" + std::to_string(iId) + ".pem");

            m_sCertPath = String(pathCert.string().c_str());
            m_sKeyPath = String(pathKey.string().c_str());

            IO::File::WriteAllText(m_sCertPath, s_sTestCertPem);
            IO::File::WriteAllText(m_sKeyPath, s_sTestKeyPem);
        }

        void TearDown() override {
            IO::File::Delete(m_sCertPath);
            IO::File::Delete(m_sKeyPath);
        }
    };

    int SslStreamTests::s_iTestCounter = 0;

    // --- X509Certificate2 Tests ---

    TEST_F(SslStreamTests, GivenValidCertAndKeyFiles_WhenX509Certificate2Created_ThenSucceeds) {
        // Given (files created in SetUp)

        // When/Then
        try {
            auto spCert = SmartPointer<X509Certificate2>::NewShared(m_sCertPath, m_sKeyPath);
            EXPECT_NE(spCert->GetInternalCert(), nullptr);
            EXPECT_NE(spCert->GetInternalKey(), nullptr);
        } catch (const SystemException& ex) {
            FAIL() << "BasicException caught: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception caught: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception caught";
        }
    }

    TEST_F(SslStreamTests, GivenNonExistentCertFile_WhenX509Certificate2Created_ThenThrowsIOException) {
        // Given
        String sInvalidCertPath("nonexistent_cert.pem");

        // When/Then
        EXPECT_THROW({
            auto spCert = SmartPointer<X509Certificate2>::NewShared(sInvalidCertPath, m_sKeyPath);
        }, IO::IOException);
    }

    TEST_F(SslStreamTests, GivenInvalidCertFileFormat_WhenX509Certificate2Created_ThenThrowsArgumentException) {
        // Given
        int iId = s_iTestCounter++;
        auto pathTempDir = std::filesystem::temp_directory_path();
        auto pathInvalidCert = pathTempDir / ("temp_invalid_cert_" + std::to_string(iId) + ".pem");
        String sInvalidCertPath = String(pathInvalidCert.string().c_str());

        IO::File::WriteAllText(sInvalidCertPath, "Not a PEM file content");

        // When/Then
        EXPECT_THROW({
            auto spCert = SmartPointer<X509Certificate2>::NewShared(sInvalidCertPath, m_sKeyPath);
        }, ArgumentException);

        IO::File::Delete(sInvalidCertPath);
    }

    // --- SslStream Integration Tests ---

    TEST_F(SslStreamTests, GivenActiveSslListener_WhenClientConnectsAndPerformsHandshake_ThenDataTransfersSecurely) {
        try {
            // Given
            int iPort = 19095;
            TcpListener listener("127.0.0.1", iPort);
            listener.Start();

            auto spCert = SmartPointer<X509Certificate2>::NewShared(m_sCertPath, m_sKeyPath);
            std::string sServerReceivedData = "";
            std::string sClientReceivedData = "";

            // Start server TLS acceptor loop in a background thread
            std::thread serverThread([&listener, spCert, &sServerReceivedData]() {
                try {
                    auto spServerClient = listener.AcceptTcpClient();
                    auto spInnerStream = spServerClient->GetStream();
                    
                    auto spSslStream = SmartPointer<SslStream>::NewShared(spInnerStream, false);
                    spSslStream->AuthenticateAsServer(spCert);

                    char arrBuffer[128] = {0};
                    int iBytesRead = spSslStream->Read(arrBuffer, 0, sizeof(arrBuffer) - 1);
                    if (iBytesRead > 0) {
                        sServerReceivedData = std::string(arrBuffer, iBytesRead);
                    }

                    std::string sResponse = "Secure World";
                    spSslStream->Write(sResponse.c_str(), 0, static_cast<int>(sResponse.length()));
                    spSslStream->Dispose();
                } catch (...) {
                    // Fail silently in worker thread
                }
            });

            struct TcpGuard {
                std::thread& t;
                TcpListener& l;
                ~TcpGuard() {
                    l.Stop();
                    if (t.joinable()) {
                        t.join();
                    }
                }
            } tcpGuard{serverThread, listener};

            // Give server thread a moment to start and accept
            Thread::Sleep(100);

            // When
            TcpClient client;
            client.Connect("127.0.0.1", iPort);

            auto spClientInner = client.GetStream();
            auto spClientSsl = SmartPointer<SslStream>::NewShared(spClientInner, false);
            
            // Client handshake
            spClientSsl->AuthenticateAsClient("localhost");

            // Write encrypted data
            std::string sGreeting = "Secure Hello";
            spClientSsl->Write(sGreeting.c_str(), 0, static_cast<int>(sGreeting.length()));

            // Read encrypted echo response
            char arrClientBuffer[128] = {0};
            int iClientBytesRead = spClientSsl->Read(arrClientBuffer, 0, sizeof(arrClientBuffer) - 1);
            if (iClientBytesRead > 0) {
                sClientReceivedData = std::string(arrClientBuffer, iClientBytesRead);
            }
            client.Close();

            if (serverThread.joinable()) {
                serverThread.join();
            }

            // Then
            EXPECT_EQ(sServerReceivedData, "Secure Hello");
            EXPECT_EQ(sClientReceivedData, "Secure World");
        } catch (const SystemException& ex) {
            FAIL() << "Library Exception: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown in test body";
        }
    }
}
