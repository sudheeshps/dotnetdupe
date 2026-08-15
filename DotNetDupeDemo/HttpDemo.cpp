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

/**
 * @brief Reads incoming HTTP request headers until double CRLF.
 */
static void ReadHttpRequestHeaders(const SmartPointer<NetworkStream>& spStream) {
    char c = 0;
    std::string reqLine;
    while (spStream->Read(&c, 0, 1) > 0) {
        if (c == '\n') {
            if (reqLine == "\r" || reqLine.empty()) break;
            reqLine.clear();
        } else {
            reqLine += c;
        }
    }
}

/**
 * @brief Transmits a mock HTTP/1.1 200 OK response payload.
 */
static void SendMockHttpResponse(const SmartPointer<NetworkStream>& spStream) {
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
        "Content-Length: 34\r\n"
        "Server: DotNetDupeMockServer/1.0\r\n\r\n"
        "Hello from DotNetDupe HTTP Server!";
    spStream->Write(response.c_str(), 0, static_cast<int>(response.length()));
}

/**
 * @brief Handles a single mock HTTP client connection on the server thread.
 */
static void ProcessMockServerClient(TcpListener& listener) {
    try {
        auto spServerClient = listener.AcceptTcpClient();
        Console::WriteLine("  [Server] Accepted client HTTP connection!");
        auto spStream = spServerClient->GetStream();
        ReadHttpRequestHeaders(spStream);
        SendMockHttpResponse(spStream);
        spServerClient->Close();
    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
        Console::WriteLine(String("  [Server] SocketException: ") + ex.What());
    } catch (const std::exception& ex) {
        Console::WriteLine(String("  [Server] Exception: ") + ex.what());
    }
}

/**
 * @brief Inspects and displays the HTTP response metadata and body content.
 */
static void PrintHttpResponseDetails(const SmartPointer<Net::Http::HttpResponseMessage>& spResponse) {
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
}

/**
 * @brief Executes HttpClient GET request against mock server endpoint.
 */
static void ExecuteHttpClientGet(int port) {
    try {
        using namespace DotNetDupe::System::Net::Http;
        HttpClient client;
        client.GetDefaultRequestHeaders().Add("User-Agent", "DotNetDupeClient/1.0");
        Console::WriteLine("Connecting HttpClient to http://127.0.0.1:19091/...");
        auto spResponse = client.Get("http://127.0.0.1:19091/");
        spResponse->EnsureSuccessStatusCode();
        PrintHttpResponseDetails(spResponse);
    } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
        Console::WriteLine(String("  [Client] HTTP RequestException: ") + ex.What());
    } catch (const SystemException& ex) {
        Console::WriteLine(String("  [Client] Exception: ") + ex.What());
    }
}

/**
 * @brief Demonstrates the HTTP client and server capabilities of DotNetDupe.
 */
void DemonstrateHttp() {
    Console::WriteLine("\n=== Demonstrate HTTP Client ===");
    const int port = 19091;
    try {
        Console::WriteLine("Starting mock HTTP server on 127.0.0.1:19091...");
        TcpListener listener("127.0.0.1", port);
        listener.Start();
        Thread serverThread([&listener]() { ProcessMockServerClient(listener); });
        serverThread.Start();
        Thread::Sleep(100);
        ExecuteHttpClientGet(port);
        serverThread.Join();
        listener.Stop();
        Console::WriteLine("Mock HTTP server stopped.");
    } catch (const std::exception& ex) {
        Console::WriteLine(String("  [Demo] Exception: ") + ex.what());
    }
}
