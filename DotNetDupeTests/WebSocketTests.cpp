#include "pch.h"
#include <gtest/gtest.h>
#include "System/Net/WebSockets/WebSocket.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Threading/Thread.h"
#include "System/Array.h"
#include <thread>
#include <atomic>
#include <string>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Net::WebSockets;

TEST(WebSocketTests, GivenSecWebSocketKey_WhenAcceptComputed_ThenMatchesRfcGuideline) {
    String key = "dGhlIHNhbXBsZSBub25jZQ==";
    String accept = WebSocket::ComputeSecWebSocketAccept(key);
    EXPECT_STREQ(accept.GetRawString(), "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=");
}

TEST(WebSocketTests, GivenWebSocket_WhenStateQueriedAndModified_ThenStateUpdatesCorrectly) {
    SmartPointer<NetworkStream> pNullStream;
    WebSocket ws(pNullStream);
    EXPECT_EQ(ws.GetState(), WebSocketState::Open);

    ws.SetState(WebSocketState::Connecting);
    EXPECT_EQ(ws.GetState(), WebSocketState::Connecting);

    ws.SetState(WebSocketState::Closed);
    EXPECT_EQ(ws.GetState(), WebSocketState::Closed);
}

TEST(WebSocketTests, GivenNullStream_WhenSendOrReceiveAttempted_ThenReturnsFalse) {
    SmartPointer<NetworkStream> pNullStream;
    WebSocket ws(pNullStream);

    EXPECT_FALSE(ws.SendAsync("Test"));

    Array<uint8_t> arrData(4);
    arrData[0] = 1; arrData[1] = 2; arrData[2] = 3; arrData[3] = 4;
    EXPECT_FALSE(ws.SendBytes(arrData));

    String msg;
    EXPECT_FALSE(ws.ReceiveText(msg));

    ws.Close();
    EXPECT_EQ(ws.GetState(), WebSocketState::Closed);
}

TEST(WebSocketTests, GivenWebSocketCommunication_WhenTextSentAndReceived_ThenFramesTransferredCorrectly) {
    int iPort = 19876;
    TcpListener listener("127.0.0.1", iPort);
    listener.Start();

    std::atomic<bool> bServerDone(false);
    std::string sServerReceived = "";

    std::thread serverThread([&listener, &sServerReceived, &bServerDone]() {
        try {
            auto pServerClient = listener.AcceptTcpClient();
            auto pStream = pServerClient->GetStream();
            WebSocket wsServer(pStream);

            String msg;
            if (wsServer.ReceiveText(msg)) {
                sServerReceived = msg.GetRawString();
                wsServer.SendAsync("Echo: " + msg);
            }
            Threading::Thread::Sleep(50);
            pServerClient->Close();
        } catch (...) {
            // Suppress background thread errors
            (void)0;
        }
        bServerDone = true;
    });

    struct ListenerGuard {
        std::thread& t;
        TcpListener& l;
        ~ListenerGuard() {
            try { l.Stop(); } catch (...) { (void)0; }
            if (t.joinable()) {
                t.join();
            }
        }
    } guard{serverThread, listener};

    Threading::Thread::Sleep(50);

    try {
        TcpClient client;
        client.Connect("127.0.0.1", iPort);
        auto pClientStream = client.GetStream();
        WebSocket wsClient(pClientStream);

        EXPECT_TRUE(wsClient.SendAsync("Hello WebSocket"));

        String response;
        if (wsClient.ReceiveText(response)) {
            EXPECT_STREQ(response.GetRawString(), "Echo: Hello WebSocket");
        }

        Array<uint8_t> arrData(4);
        arrData[0] = 10; arrData[1] = 20; arrData[2] = 30; arrData[3] = 40;
        EXPECT_TRUE(wsClient.SendBytes(arrData));

        wsClient.Close();
        client.Close();
    } catch (...) {
        // Suppress client errors
        (void)0;
    }

    listener.Stop();
    if (serverThread.joinable()) {
        serverThread.join();
    }

    EXPECT_EQ(sServerReceived, "Hello WebSocket");
}
