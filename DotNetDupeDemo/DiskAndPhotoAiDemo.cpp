#include "Demos.h"
#include "System/Console.h"
#include "System/Convert.h"
#include "System/IO/File.h"
#include "System/IO/FileInfo.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/Security/Cryptography/SHA256.h"
#include "System/Drawing/Imaging/RawImage.h"
#include "System/Drawing/Imaging/ExifMetadata.h"
#include "System/AI/VectorMath.h"
#include "System/AI/FaceRecognitionEngine.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Security::Cryptography;
using namespace DotNetDupe::System::Drawing::Imaging;
using namespace DotNetDupe::System::AI;
using namespace DotNetDupe::System::Collections::Generic;

void DemonstrateDiskAndPhotoAi() {
    Console::WriteLine("\n=== Disk Metadata, Fast Edge Hashing & Local Photo AI Demo ===");

    const String sTempDir = Path::Combine(Path::GetTempPath(), "DotNetDupe_DiskAiDemo");
    if (Directory::Exists(sTempDir)) {
        Directory::Delete(sTempDir, true);
    }
    Directory::CreateDirectory(sTempDir);

    const String sFileA = Path::Combine(sTempDir, "backup_sample.dat");
    const String sFileB = Path::Combine(sTempDir, "backup_clone.dat");
    File::WriteAllText(sFileA, "Verified backup payload for DotNetDupe 5.0.5");
    const bool bVerified = File::CopyVerified(sFileA, sFileB, true);

    const FileInfo infoA(sFileA);
    const String sHexDigest = SHA256::HashFileToHex(sFileA);
    Console::WriteLine(String("FileInfo Size: ") + Convert::ToString(infoA.GetLength64()) + " bytes");
    Console::WriteLine(String("Verified Clone Identical: ") + (bVerified ? "True" : "False"));
    Console::WriteLine(String("SHA-256 Hex: ") + sHexDigest);

    SmartPointer<RawImage> pPortrait = RawImage::Create(32, 32);
    for (int iY = 0; iY < 32; ++iY) {
        for (int iX = 0; iX < 32; ++iX) {
            const unsigned int uR = (iY == 11 && (iX == 11 || iX == 20)) ? 25U : 215U;
            const unsigned int uG = (iY == 11 && (iX == 11 || iX == 20)) ? 20U : 170U;
            const unsigned int uB = (iY == 11 && (iX == 11 || iX == 20)) ? 20U : 140U;
            pPortrait->SetPixelArgb(iX, iY, 0xFF000000U | (uR << 16) | (uG << 8) | uB);
        }
    }

    FaceRecognitionEngine engine;
    const List<DetectedFace> lstFaces = engine.DetectAndEmbedFaces(pPortrait);
    Console::WriteLine(String("Detected Faces in Synthetic Portrait: ") + Convert::ToString(lstFaces.GetCount()));

    Directory::Delete(sTempDir, true);
}
