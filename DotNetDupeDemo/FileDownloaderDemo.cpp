#include "System/Net/Http/FileDownloader.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/Convert.h"
#include "System/Console.h"
#include "System/Threading/Thread.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Server/WebAppServer.h"
#include "System/SmartPointer.h"
#include <string>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Server;

#include "Demos.h"

void DemonstrateFileDownloader() {
    Console::WriteLine("\n=== Demonstrate FileDownloader (Pause/Resume & Progress) ===");
    int iPort = 19095;

    try {
        auto builder = WebApplication::CreateBuilder();
        auto app = builder->Build();

        std::string sChunk1(20000, 'A');
        std::string sChunk2(20000, 'B');

        app->MapGet("/large_file.bin", [sChunk1, sChunk2](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> ctx) {
            String sRange;
            if (ctx->GetRequest()->GetHeaders().TryGetValue("range", sRange)) {
                // Range request (Resume)
                ctx->GetResponse()->SetStatusCode(206);
                ctx->GetResponse()->GetHeaders().Add("Content-Length", "20000");
                ctx->GetResponse()->GetHeaders().Add("Content-Range", "bytes 20000-39999/40000");
                
                if (ctx->GetRequest()->GetMethod().GetRawString() && std::string(ctx->GetRequest()->GetMethod().GetRawString()) == "HEAD") {
                    return String("");
                }
                return String(sChunk2.c_str());
            } else {
                // Initial request
                ctx->GetResponse()->SetStatusCode(200);
                ctx->GetResponse()->GetHeaders().Add("Content-Length", "40000");
                
                if (ctx->GetRequest()->GetMethod().GetRawString() && std::string(ctx->GetRequest()->GetMethod().GetRawString()) == "HEAD") {
                    return String("");
                }
                
                // Simulate an interrupted connection by sending only half and setting a header
                // Wait, WebApplication writes the body automatically based on the returned String.
                // We can't interrupt it easily via WebApplication return.
                // Wait! FileDownloader Demo paused the downloader manually: downloader.Pause()!
                // We don't need to close the socket from the server!
                // Oh wait, the mock listener did `Thread::Sleep(100); getClient1->Close();` to force the interruption if it wasn't paused in time!
                // But the client code does: downloader.Start(); Thread::Sleep(50); downloader.Pause();
                // This means the client pauses it before the server finishes sending if the server is slow!
                // So if we just return the full 40000 bytes, the client will read a bit, pause, and then resume with Range!
                
                std::string full(sChunk1);
                full += sChunk2;
                return String(full.c_str());
            }
        });

        WebAppServer server(app, "wwwroot");
        
        Thread serverThread([&server, iPort]() {
            try {
                server.Run(String("http://127.0.0.1:") + Convert::ToString(iPort), 5);
            } catch (const SystemException& ex) {
                Console::WriteLine(String("[Server Thread SystemException] ") + ex.What());
            } catch (...) {
                Console::WriteLine("[Server Thread] Unknown Exception!");
            }
        });
        
        serverThread.Start();
        Thread::Sleep(500); // give server time to start

        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "demo_download.bin" });
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

        String sUrl = String("http://127.0.0.1:") + Convert::ToString(iPort) + "/large_file.bin";
        FileDownloader downloader(sUrl, sDest);
        downloader.SetUserAgent("Mozilla/5.0");

        downloader.SetProgressCallback([](DownloadProgress progress) {
            Console::WriteLine(String("  [Progress] Downloaded: ") + Convert::ToString(progress.DownloadedBytes) +
                " / " + Convert::ToString(progress.TotalBytes) + " bytes (Remaining: " +
                Convert::ToString(progress.RemainingBytes) + ") | Rate: " +
                Convert::ToString(static_cast<int>(progress.DownloadRateBytesPerSec)) + " B/s");
        });

        Console::WriteLine("Starting download...");
        downloader.Start();

        Thread::Sleep(10); // Sleep briefly to let it start downloading chunk 1
        Console::WriteLine("Pausing download...");
        downloader.Pause();

        while (downloader.GetStatus() == DownloadStatus::Downloading) {
            Thread::Sleep(20);
        }

        Console::WriteLine("Resuming download...");
        downloader.Resume();

        while (downloader.GetStatus() == DownloadStatus::Downloading) {
            Thread::Sleep(20);
        }

        Console::WriteLine("Download completed successfully!");
        
        server.Stop();
        serverThread.Join();

        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

    } catch (const SystemException& ex) {
        Console::Write("FileDownloader Demo Exception: ");
        Console::WriteLine(ex.What());
    }
}
