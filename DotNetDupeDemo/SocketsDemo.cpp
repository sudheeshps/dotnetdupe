#include "System/String.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/SocketException.h"

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

void DemonstrateSockets() {
    Console::WriteLine("\n--- Sockets Demonstration ---");
    int port = 19090;

    try {
        Console::WriteLine("Starting TcpListener on 127.0.0.1:19090...");
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        Thread serverThread([&listener]() {
            try {
                auto serverClient = listener.AcceptTcpClient();
                Console::WriteLine("  [Server] Accepted client connection!");
                auto stream = serverClient->GetStream();

                char buffer[128] = {0};
                int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
                if (bytesRead > 0) {
                    Console::Write("  [Server] Received: '");
                    Console::Write(buffer);
                    Console::WriteLine("'");
                }

                String response = "World";
                stream->Write(response.GetRawString(), 0, response.GetLength());
                serverClient->Close();
            } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                Console::Write("  [Server] SocketException: ");
                Console::WriteLine(ex.What());
            } catch (const DotNetDupe::System::BasicException<char>& ex) {
                Console::Write("  [Server] BasicException: ");
                Console::WriteLine(ex.What());
            } catch (const std::exception& ex) {
                Console::Write("  [Server] std::exception: ");
                Console::WriteLine(ex.what());
            }
        });

        serverThread.Start();
        Thread::Sleep(100);

        try {
            Console::WriteLine("Connecting TcpClient to 127.0.0.1:19090...");
            TcpClient client;
            client.Connect("127.0.0.1", port);

            auto stream = client.GetStream();
            String msg = "Hello";
            stream->Write(msg.GetRawString(), 0, msg.GetLength());

            char buffer[128] = {0};
            int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                Console::Write("  [Client] Received back: '");
                Console::Write(buffer);
                Console::WriteLine("'");
            }
            client.Close();
        } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
            Console::Write("  [Client] SocketException: ");
            Console::WriteLine(ex.What());
        } catch (const DotNetDupe::System::BasicException<char>& ex) {
            Console::Write("  [Client] BasicException: ");
            Console::WriteLine(ex.What());
        } catch (const std::exception& ex) {
            Console::Write("  [Client] std::exception: ");
            Console::WriteLine(ex.what());
        }

        serverThread.Join();
        listener.Stop();
        Console::WriteLine("TcpListener stopped.");
    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
        Console::Write("  [Demo] SocketException: ");
        Console::WriteLine(ex.What());
    } catch (const DotNetDupe::System::BasicException<char>& ex) {
        Console::Write("  [Demo] BasicException: ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("  [Demo] std::exception: ");
        Console::WriteLine(ex.what());
    }
}
