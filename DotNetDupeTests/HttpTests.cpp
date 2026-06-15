#include "pch.h"
#include "gtest/gtest.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpRequestMessage.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Dns.h"
#include "System/Net/HttpStatusCode.h"
#include "System/IO/MemoryStream.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Threading/Thread.h"
#include <thread>
#include <vector>
#include <string>
#include <cstring>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Threading;

namespace DotNetDupeTests {

    struct HttpTestGuard {
        std::thread& t;
        TcpListener& l;
        ~HttpTestGuard() {
            l.Stop();
            if (t.joinable()) {
                t.join();
            }
        }
    };

    // --- HttpMethod Tests ---

    TEST(HttpTests, GivenHttpMethod_WhenComparing_ThenCaseIsIgnored) {
        // Given
        HttpMethod m1("GET");
        HttpMethod m2("get");

        // When/Then
        EXPECT_TRUE(m1 == m2);
        EXPECT_FALSE(m1 != m2);
        EXPECT_EQ(m1.ToString(), "GET");
    }

    // --- HttpContent & MemoryStream Tests ---

    TEST(HttpTests, GivenStringContent_WhenCreated_ThenHeadersAndContentAreCorrect) {
        // Given
        String contentText("Hello World");

        // When
        StringContent content(contentText);

        // Then
        EXPECT_EQ(content.ReadAsString(), "Hello World");
        EXPECT_EQ(content.GetLength(), 11);
        EXPECT_EQ(content.GetHeaders()["Content-Type"], "text/plain; charset=utf-8");

        Array<char> bytes = content.ReadAsByteArray();
        EXPECT_EQ(bytes.GetLength(), 11);
        EXPECT_EQ(std::strncmp(bytes.GetData(), "Hello World", 11), 0);
    }

    TEST(HttpTests, GivenByteArrayContent_WhenCreatedWithOffset_ThenRangeIsRespected) {
        // Given
        Array<char> buffer({'A', 'B', 'C', 'D', 'E'});

        // When
        ByteArrayContent content(buffer, 1, 3); // 'B', 'C', 'D'

        // Then
        EXPECT_EQ(content.GetLength(), 3);
        EXPECT_EQ(content.ReadAsString(), "BCD");
        Array<char> readBytes = content.ReadAsByteArray();
        EXPECT_EQ(readBytes.GetLength(), 3);
        EXPECT_EQ(readBytes[0], 'B');
        EXPECT_EQ(readBytes[1], 'C');
        EXPECT_EQ(readBytes[2], 'D');
    }

    TEST(HttpTests, GivenMemoryStream_WhenWrittenAndRead_ThenDataIsPreserved) {
        // Given
        IO::MemoryStream stream;

        // When
        const char* writeData = "MemoryStreamTest";
        stream.Write(writeData, 0, 16);
        stream.SetPosition(0);

        char readData[32] = {0};
        int readBytes = stream.Read(readData, 0, 32);

        // Then
        EXPECT_EQ(readBytes, 16);
        EXPECT_EQ(std::strcmp(readData, "MemoryStreamTest"), 0);
        EXPECT_EQ(stream.GetLength(), 16);
    }

    // --- Dns Tests ---

    TEST(HttpTests, GivenLocalhostHostName_WhenResolvingAddresses_ThenLoopbackIsReturned) {
        // Given
        String host("localhost");

        // When
        Array<String> addresses = Dns::GetHostAddresses(host);

        // Then
        EXPECT_GT(addresses.GetLength(), 0);
        EXPECT_TRUE(addresses[0].Equals("127.0.0.1") || addresses[0].Equals("::1"));
    }

    // --- Mock HTTP Server & HttpClient Tests ---

    TEST(HttpTests, GivenMockHttpServer_WhenClientSendsGetRequest_ThenCorrectHeadersAndBodyAreReturned) {
        // Given
        int port = 18086;
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        std::thread serverThread([&listener]() {
            try {
                auto client = listener.AcceptTcpClient();
                auto stream = client->GetStream();

                // Read request
                char reqBuf[1024];
                int totalRead = stream->Read(reqBuf, 0, sizeof(reqBuf) - 1);
                
                // Send standard response
                std::string resp = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html; charset=utf-8\r\n"
                    "Content-Length: 12\r\n"
                    "Custom-Header: value123\r\n"
                    "\r\n"
                    "Hello Client";
                stream->Write(resp.c_str(), 0, static_cast<int>(resp.length()));
                client->Close();
            } catch (...) {}
        });

        HttpTestGuard guard{serverThread, listener};

        // Give server time to start accepting
        Thread::Sleep(100);

        try {
            // When
            HttpClient client;
            auto response = client.Get(String("http://127.0.0.1:18086/somepath"));

            // Then
            EXPECT_TRUE(response->IsSuccessStatusCode());
            EXPECT_EQ(response->GetStatusCode(), HttpStatusCode::OK);
            EXPECT_EQ(response->GetReasonPhrase(), "OK");
            EXPECT_EQ(response->GetHeaders()["Custom-Header"], "value123");
            EXPECT_EQ(response->GetHeaders()["Content-Type"], "text/html; charset=utf-8");

            auto content = response->GetContent();
            ASSERT_FALSE(content.IsNull());
            EXPECT_EQ(content->ReadAsString(), "Hello Client");
        } catch (const BasicException<char>& ex) {
            FAIL() << "Exception: " << ex.What();
        }
    }

    TEST(HttpTests, GivenMockHttpServer_WhenClientSendsPostWithContent_ThenServerReceivesIt) {
        // Given
        int port = 18087;
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        std::string requestBody = "";
        std::thread serverThread([&listener, &requestBody]() {
            try {
                auto client = listener.AcceptTcpClient();
                auto stream = client->GetStream();

                // Read request header & body
                std::string headerLine;
                long contentLength = 0;
                char c;
                while (true) {
                    int r = stream->Read(&c, 0, 1);
                    if (r <= 0) break;
                    if (c == '\n') {
                        if (headerLine == "\r" || headerLine.empty()) break; // End of headers
                        if (headerLine.rfind("Content-Length:", 0) == 0) {
                            contentLength = std::atol(headerLine.substr(15).c_str());
                        }
                        headerLine.clear();
                    } else {
                        headerLine += c;
                    }
                }

                if (contentLength > 0) {
                    std::vector<char> body(contentLength);
                    int readBytes = stream->Read(body.data(), 0, static_cast<int>(contentLength));
                    requestBody = std::string(body.data(), readBytes);
                }

                // Send success response
                std::string resp = 
                    "HTTP/1.1 201 Created\r\n"
                    "Content-Length: 0\r\n"
                    "\r\n";
                stream->Write(resp.c_str(), 0, static_cast<int>(resp.length()));
                client->Close();
            } catch (...) {}
        });

        HttpTestGuard guard{serverThread, listener};

        Thread::Sleep(100);

        try {
            // When
            HttpClient client;
            auto content = SmartPointer<StringContent>::NewShared("PostPayloadData");
            auto response = client.Post(String("http://127.0.0.1:18087/create"), content);

            // Then
            EXPECT_TRUE(response->IsSuccessStatusCode());
            EXPECT_EQ(response->GetStatusCode(), HttpStatusCode::Created);
            EXPECT_EQ(requestBody, "PostPayloadData");
        } catch (const BasicException<char>& ex) {
            FAIL() << "Exception: " << ex.What();
        }
    }

    TEST(HttpTests, GivenMockHttpServerWithChunkedTransfer_WhenClientReads_ThenBodyIsReconstructedCorrectly) {
        // Given
        int port = 18088;
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        std::thread serverThread([&listener]() {
            try {
                auto client = listener.AcceptTcpClient();
                auto stream = client->GetStream();

                // Read request
                char reqBuf[1024];
                stream->Read(reqBuf, 0, sizeof(reqBuf) - 1);

                // Send chunked response
                std::string resp = 
                    "HTTP/1.1 200 OK\r\n"
                    "Transfer-Encoding: chunked\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "4\r\n"
                    "Wiki\r\n"
                    "6\r\n"
                    "pedia \r\n"
                    "C\r\n"
                    "in \r\nchunks.\r\n"
                    "0\r\n"
                    "\r\n";
                stream->Write(resp.c_str(), 0, static_cast<int>(resp.length()));
                client->Close();
            } catch (...) {}
        });

        HttpTestGuard guard{serverThread, listener};

        Thread::Sleep(100);

        try {
            // When
            HttpClient client;
            auto response = client.Get(String("http://127.0.0.1:18088/chunked"));

            // Then
            EXPECT_TRUE(response->IsSuccessStatusCode());
            auto content = response->GetContent();
            ASSERT_FALSE(content.IsNull());
            EXPECT_EQ(content->ReadAsString(), "Wikipedia in \r\nchunks.");
        } catch (const BasicException<char>& ex) {
            FAIL() << "Exception: " << ex.What();
        }
    }

    TEST(HttpTests, GivenInvalidUrlScheme_WhenClientSendsRequest_ThenExceptionIsThrown) {
        // Given
        HttpClient client;

        // When/Then
        EXPECT_THROW(client.Get("ftp://example.com"), ArgumentException);
    }
}
