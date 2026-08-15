#include "pch.h"
#include "gtest/gtest.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/UdpClient.h"
#include "System/Threading/Thread.h"
#include <thread>
#include <vector>
#include <string>

#include "System/SystemException.h"
#include "System/IOException.h"
#include "System/Net/Sockets/SocketException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Threading;

namespace DotNetDupeTests {

    // --- TcpListener & TcpClient Tests ---

    TEST(SocketsTests, GivenActiveListener_WhenClientConnects_ThenConnectionSucceedsAndDataTransfers) {
        try {
            // Given
            int port = 18085;
            TcpListener listener("127.0.0.1", port);
            listener.Start();

            std::string serverReceivedData = "";
            std::string clientReceivedData = "";

            // Start server accept loop in a background thread
            std::thread serverThread([&listener, &serverReceivedData]() {
                try {
                    auto serverClient = listener.AcceptTcpClient();
                    auto stream = serverClient->GetStream();

                    char buffer[128] = {0};
                    int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
                    if (bytesRead > 0) {
                        serverReceivedData = std::string(buffer, bytesRead);
                    }

                    // Send echo response back
                    std::string response = "World";
                    stream->Write(response.c_str(), 0, static_cast<int>(response.length()));
                    serverClient->Close();
                } catch (...) {
                    // Fail silently or handle
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

            // Give server thread a moment to enter Accept
            Thread::Sleep(100);

            // When
            TcpClient client;
            client.Connect("127.0.0.1", port);

            auto clientStream = client.GetStream();
            std::string greeting = "Hello";
            clientStream->Write(greeting.c_str(), 0, static_cast<int>(greeting.length()));

            char clientBuffer[128] = {0};
            int clientBytesRead = clientStream->Read(clientBuffer, 0, sizeof(clientBuffer) - 1);
            if (clientBytesRead > 0) {
                clientReceivedData = std::string(clientBuffer, clientBytesRead);
            }
            client.Close();

            // Wait for server thread to finish to avoid data race on serverReceivedData
            if (serverThread.joinable()) {
                serverThread.join();
            }

            // Then
            EXPECT_EQ(serverReceivedData, "Hello");
            EXPECT_EQ(clientReceivedData, "World");
        } catch (const DotNetDupe::System::SystemException& ex) {
            FAIL() << "SystemException: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown in test body";
        }
    }

    // --- UdpClient Tests ---

    TEST(SocketsTests, GivenBoundUdpReceiver_WhenSenderSends_ThenDataIsReceivedCorrectly) {
        try {
            // Given
            int port = 18086;
            UdpClient receiver(port);

            std::string receivedMsg = "";
            String senderIp = "";
            int senderPort = 0;

            std::thread receiveThread([&receiver, &receivedMsg, &senderIp, &senderPort]() {
                try {
                    char buffer[128] = {0};
                    int bytesReceived = receiver.Receive(buffer, 0, sizeof(buffer) - 1, senderIp, senderPort);
                    if (bytesReceived > 0) {
                        receivedMsg = std::string(buffer, bytesReceived);
                    }
                } catch (...) {
                }
            });

            struct UdpGuard {
                std::thread& t;
                UdpClient& r;
                ~UdpGuard() {
                    r.Close();
                    if (t.joinable()) {
                        t.join();
                    }
                }
            } udpGuard{receiveThread, receiver};

            Thread::Sleep(100);

            // When
            UdpClient sender;
            std::string payload = "UdpHello";
            sender.Send(payload.c_str(), 0, static_cast<int>(payload.length()), "127.0.0.1", port);
            sender.Close();

            // Wait for receive thread to finish receiving the data
            if (receiveThread.joinable()) {
                receiveThread.join();
            }

            // Then
            EXPECT_EQ(receivedMsg.length(), 8);
            EXPECT_EQ(receivedMsg, "UdpHello");
            EXPECT_EQ(senderIp, "127.0.0.1");
            EXPECT_GT(senderPort, 0);
        } catch (const DotNetDupe::System::SystemException& ex) {
            FAIL() << "SystemException: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown in test body";
        }
    }

    // --- Error / Edge Cases Tests ---

    TEST(SocketsTests, GivenClosedSocket_WhenOperationsAttempted_ThenThrowsException) {
        // Given
        Socket socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
        socket.Close();

        // When & Then
        EXPECT_THROW(socket.Bind("127.0.0.1", 18087), SocketException);
    }

    TEST(SocketsTests, GivenStoppedListener_WhenAcceptAttempted_ThenThrowsException) {
        // Given
        TcpListener listener("127.0.0.1", 18088);
        // Do not call Start()

        // When & Then
        EXPECT_THROW(listener.AcceptTcpClient(), SocketException);
    }

    TEST(SocketsTests, GivenUnconnectedTcpClient_WhenGetStreamOrConnectFailed_ThenThrowsSocketException) {
        TcpClient client;
        EXPECT_FALSE(client.Connected());
        EXPECT_THROW(client.GetStream(), SocketException);
        EXPECT_THROW(client.Connect("127.0.0.1", 59999), SocketException);
    }

    TEST(SocketsTests, GivenDisposedNetworkStream_WhenReadWriteAttempted_ThenThrowsIOException) {
        SmartPointer<Socket> pSock = SmartPointer<Socket>::NewShared(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
        SmartPointer<NetworkStream> pNetStream = SmartPointer<NetworkStream>::NewShared(pSock, true);
        pNetStream->Close();

        char buf[10] = { 0 };
        EXPECT_FALSE(pNetStream->CanRead());
        EXPECT_FALSE(pNetStream->CanWrite());
        EXPECT_FALSE(pNetStream->CanSeek());
        EXPECT_THROW(pNetStream->Read(buf, 0, 10), IOException);
        EXPECT_THROW(pNetStream->Write("test", 0, 4), IOException);
    }

}
