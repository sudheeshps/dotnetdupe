#include "System/Net/Http/FileDownloader.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/Convert.h"
#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <string>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateFileDownloader() {
    Console::WriteLine("\n=== Demonstrate FileDownloader (Pause/Resume & Progress) ===");
    int iPort = 19095;

    try {
        TcpListener listener("127.0.0.1", iPort);
        listener.Start();

        std::string sChunk1(20000, 'A');
        std::string sChunk2(20000, 'B');

        Thread serverThread([&listener, sChunk1, sChunk2]() {
            try {
                // HEAD request
                auto headClient = listener.AcceptTcpClient();
                auto headStream = headClient->GetStream();
                char buf[512];
                headStream->Read(buf, 0, 512);

                std::string sHeadResp = "HTTP/1.1 200 OK\r\nContent-Length: 40000\r\nConnection: close\r\n\r\n";
                headStream->Write(sHeadResp.data(), 0, static_cast<int>(sHeadResp.size()));
                headClient->Close();

                // 1st GET request
                auto getClient1 = listener.AcceptTcpClient();
                auto getStream1 = getClient1->GetStream();
                getStream1->Read(buf, 0, 512);

                std::string sGetResp1 = "HTTP/1.1 200 OK\r\nContent-Length: 40000\r\nConnection: close\r\n\r\n";
                getStream1->Write(sGetResp1.data(), 0, static_cast<int>(sGetResp1.size()));
                getStream1->Write(sChunk1.data(), 0, static_cast<int>(sChunk1.size()));
                Thread::Sleep(100);
                getClient1->Close();

                // 2nd GET Range request
                auto getClient2 = listener.AcceptTcpClient();
                auto getStream2 = getClient2->GetStream();
                getStream2->Read(buf, 0, 512);

                std::string sGetResp2 = "HTTP/1.1 206 Partial Content\r\nContent-Length: 20000\r\nConnection: close\r\n\r\n";
                getStream2->Write(sGetResp2.data(), 0, static_cast<int>(sGetResp2.size()));
                getStream2->Write(sChunk2.data(), 0, static_cast<int>(sChunk2.size()));
                getClient2->Close();

                listener.Stop();
            } catch (...) {}
        });

        String sDest = IO::Path::Combine({ IO::Path::GetTempPath(), "demo_download.bin" });
        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

        String sUrl = String("https://127.0.0.1:") + Convert::ToString(iPort) + "/large_file.bin";
        FileDownloader downloader(sUrl, sDest);
        downloader.SetUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

        downloader.SetProgressCallback([](DownloadProgress progress) {
            Console::WriteLine(String("  [Progress] Downloaded: ") + Convert::ToString(progress.DownloadedBytes) +
                " / " + Convert::ToString(progress.TotalBytes) + " bytes (Remaining: " +
                Convert::ToString(progress.RemainingBytes) + ") | Rate: " +
                Convert::ToString(static_cast<int>(progress.DownloadRateBytesPerSec)) + " B/s");
        });

        Console::WriteLine("Starting download...");
        downloader.Start();

        Thread::Sleep(50);
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
        serverThread.Join();

        if (IO::File::Exists(sDest)) IO::File::Delete(sDest);

    } catch (const SystemException& ex) {
        Console::Write("FileDownloader Demo Exception: ");
        Console::WriteLine(ex.What());
    }
}
