#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Http/HttpClient.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Net::Sockets;

#include "Demos.h"

void DemonstrateHttp() {
    Console::WriteLine("\n=== Demonstrate HTTP Client ===");
    int port = 19091;

    try {
        Console::WriteLine("Starting mock HTTP server on 127.0.0.1:19091...");
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        Thread serverThread([&listener]() {
            try {
                auto serverClient = listener.AcceptTcpClient();
                Console::WriteLine("  [Server] Accepted client HTTP connection!");
                auto stream = serverClient->GetStream();

                // Simple request reader (read until headers end)
                char c;
                std::string reqLine;
                while (stream->Read(&c, 0, 1) > 0) {
                    if (c == '\n') {
                        if (reqLine == "\r" || reqLine.empty()) break;
                        reqLine.clear();
                    } else {
                        reqLine += c;
                    }
                }

                // Send back a mock HTTP response
                std::string response = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain; charset=utf-8\r\n"
                    "Content-Length: 34\r\n"
                    "Server: DotNetDupeMockServer/1.0\r\n"
                    "\r\n"
                    "Hello from DotNetDupe HTTP Server!";
                stream->Write(response.c_str(), 0, static_cast<int>(response.length()));
                serverClient->Close();
            } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                Console::Write("  [Server] SocketException: ");
                Console::WriteLine(ex.What());
            } catch (const BasicException<char>& ex) {
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

            Console::WriteLine("Connecting HttpClient to http://127.0.0.1:19091/...");
            auto response = client.Get("http://127.0.0.1:19091/");

            response->EnsureSuccessStatusCode();

            Console::Write("Response Status Code: ");
            Console::WriteLine(static_cast<int>(response->GetStatusCode()));

            Console::Write("Server header: ");
            Console::WriteLine(response->GetHeaders()["Server"]);

            auto content = response->GetContent();
            if (!content.IsNull()) {
                Console::Write("Response body: '");
                Console::Write(content->ReadAsString());
                Console::WriteLine("'");
            }
        } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
            Console::Write("  [Client] HTTP RequestException: ");
            Console::WriteLine(ex.What());
        } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
            Console::Write("  [Client] HTTP SocketException: ");
            Console::WriteLine(ex.What());
        } catch (const BasicException<char>& ex) {
            Console::Write("  [Client] HTTP Exception: ");
            Console::WriteLine(ex.What());
        }

        serverThread.Join();
        listener.Stop();
        Console::WriteLine("Mock HTTP server stopped.");
    } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
        Console::Write("  [Demo] HTTP RequestException: ");
        Console::WriteLine(ex.What());
    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
        Console::Write("  [Demo] HTTP SocketException: ");
        Console::WriteLine(ex.What());
    } catch (const BasicException<char>& ex) {
        Console::Write("  [Demo] HTTP Exception: ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("  [Demo] std::exception: ");
        Console::WriteLine(ex.what());
    }
}
