#include "System/String.h"
#include "System/IO/File.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Security/SslStream.h"
#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Net::Sockets;

#include "Demos.h"

void DemonstrateHttps() {
    using namespace DotNetDupe::System::Net::Security;
    using namespace DotNetDupe::System::Security::Cryptography::X509Certificates;
    Console::WriteLine("\n=== Demonstrate HTTPS Client & Server ===");
    int iPort = 19092;
    String sCertPath = "demo_test_cert.pem";
    String sKeyPath = "demo_test_key.pem";

    // Write temp certificates for the demo
    IO::File::WriteAllText(sCertPath, 
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
        "-----END CERTIFICATE-----\n");

    IO::File::WriteAllText(sKeyPath, 
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
        "-----END PRIVATE KEY-----\n");

    try {
        auto spCert = SmartPointer<X509Certificate2>::NewShared(sCertPath, sKeyPath);

        Console::WriteLine("Starting mock HTTPS server on 127.0.0.1:19092...");
        TcpListener listener("127.0.0.1", iPort);
        listener.Start();

        Thread serverThread([&listener, spCert]() {
            try {
                auto spServerClient = listener.AcceptTcpClient();
                Console::WriteLine("  [Server] Accepted client HTTPS connection!");
                auto spInnerStream = spServerClient->GetStream();
                
                auto spSslStream = SmartPointer<Net::Security::SslStream>::NewShared(spInnerStream, false);
                spSslStream->AuthenticateAsServer(spCert);

                // Simple request reader (read until headers end)
                char c;
                std::string sReqLine;
                while (spSslStream->Read(&c, 0, 1) > 0) {
                    if (c == '\n') {
                        if (sReqLine == "\r" || sReqLine.empty()) break;
                        sReqLine.clear();
                    } else {
                        sReqLine += c;
                    }
                }

                // Send back a mock HTTPS response
                std::string sResponse = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain; charset=utf-8\r\n"
                    "Content-Length: 35\r\n"
                    "Server: DotNetDupeMockHttpsServer/1.0\r\n"
                    "\r\n"
                    "Hello from DotNetDupe HTTPS Server!";
                spSslStream->Write(sResponse.c_str(), 0, static_cast<int>(sResponse.length()));
                spServerClient->Close();
            } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                Console::Write("  [Server] SocketException: ");
                Console::WriteLine(ex.What());
            } catch (const SystemException& ex) {
                Console::Write("  [Server] BasicException: ");
                Console::WriteLine(ex.What());
            } catch (const std::exception& ex) {
                Console::Write("  [Server] Exception: ");
                Console::WriteLine(ex.what());
            }
        });

        serverThread.Start();
        Thread::Sleep(100);

        try {
            using namespace DotNetDupe::System::Net::Http;
            HttpClient client;
            client.GetDefaultRequestHeaders().Add("User-Agent", "DotNetDupeClient/1.0");

            Console::WriteLine("Connecting HttpClient to https://127.0.0.1:19092/...");
            auto spResponse = client.Get("https://127.0.0.1:19092/");

            spResponse->EnsureSuccessStatusCode();

            Console::Write("Response Status Code: ");
            Console::WriteLine(static_cast<int>(spResponse->GetStatusCode()));

            Console::Write("Server header: ");
            Console::WriteLine(spResponse->GetHeaders()["Server"]);

            auto spContent = spResponse->GetContent();
            if (!spContent.IsNull()) {
                Console::Write("Response body: '");
                Console::Write(spContent->ReadAsString());
                Console::WriteLine("'");
            }
        } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
            Console::Write("  [Client] HTTPS RequestException: ");
            Console::WriteLine(ex.What());
        } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
            Console::Write("  [Client] HTTPS SocketException: ");
            Console::WriteLine(ex.What());
        } catch (const SystemException& ex) {
            Console::Write("  [Client] HTTPS Exception: ");
            Console::WriteLine(ex.What());
        }

        serverThread.Join();
        listener.Stop();
        Console::WriteLine("Mock HTTPS server stopped.");
    } catch (const SystemException& ex) {
        Console::Write("  [Demo] HTTPS Exception: ");
        Console::WriteLine(ex.What());
    }

    // Clean up cert files
    IO::File::Delete(sCertPath);
    IO::File::Delete(sKeyPath);
}
