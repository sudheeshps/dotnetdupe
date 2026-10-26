#include "pch.h"
#include "gtest/gtest.h"
#include "System/Drawing/Imaging/ExifMetadata.h"
#include "System/Drawing/Imaging/RawImage.h"
#include "System/AI/VectorMath.h"
#include "System/AI/FaceRecognitionEngine.h"
#include "System/IO/File.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Drawing::Imaging;
using namespace DotNetDupe::System::AI;
using namespace DotNetDupe::System::Collections::Generic;

namespace {
    class MockFaceBackend : public IFaceRecognitionBackend {
    public:
        List<DetectedFace> DetectAndEmbedFaces(const SmartPointer<RawImage>& pImage) override {
            if (!pImage) {
                throw ArgumentNullException("pImage");
            }
            Array<float> arrEmb(FaceRecognitionEngine::EmbeddingDimension);
            arrEmb[0] = 1.0f;
            List<DetectedFace> lst;
            lst.Add(DetectedFace(2, 3, 10, 12, 0.98f, arrEmb));
            return lst;
        }

        Array<float> ComputeRegionEmbedding(
            const SmartPointer<RawImage>& pImage,
            int iX,
            int iY,
            int iWidth,
            int iHeight) override {
            (void)pImage;
            (void)iX;
            (void)iY;
            (void)iWidth;
            (void)iHeight;
            Array<float> arrEmb(FaceRecognitionEngine::EmbeddingDimension);
            arrEmb[1] = 1.0f;
            return arrEmb;
        }
    };
}

TEST(ExifMetadataTests, GivenPngAndBmpHeaders_WhenExtracted_ThenParsesDimensionsAndMetadata) {
    ExifMetadata emptyMeta;
    EXPECT_FALSE(emptyMeta.GetHasExif());

    Array<byte> arrFakePng(32);
    const byte arrPngSig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    for (int i = 0; i < 8; ++i) {
        arrFakePng[i] = arrPngSig[i];
    }
    arrFakePng[16] = 0x00;
    arrFakePng[17] = 0x00;
    arrFakePng[18] = 0x07;
    arrFakePng[19] = 0x80;
    arrFakePng[20] = 0x00;
    arrFakePng[21] = 0x00;
    arrFakePng[22] = 0x04;
    arrFakePng[23] = 0x38;

    const ExifMetadata meta = ExifMetadata::ExtractFromBytes(arrFakePng);
    EXPECT_EQ(meta.GetWidth(), 1920);
    EXPECT_EQ(meta.GetHeight(), 1080);
}

TEST(RawImageTests, GivenArgbImage_WhenSavedLoadedCroppedAndResized_ThenPreservesPixelsAndDimensions) {
    EXPECT_THROW(RawImage(0, 10), ArgumentOutOfRangeException);
    RawImage img(16, 16);
    EXPECT_EQ(img.GetWidth(), 16);
    EXPECT_EQ(img.GetHeight(), 16);
    EXPECT_EQ(img.GetPixels().GetLength(), 16 * 16 * 4);

    const unsigned int uExpectedColor = 0xFFD2A082U;
    img.SetPixelArgb(4, 5, uExpectedColor);
    EXPECT_EQ(img.GetPixelArgb(4, 5), uExpectedColor);

    SmartPointer<RawImage> pResized = img.ResizeBilinear(8, 8);
    ASSERT_TRUE(static_cast<bool>(pResized));
    EXPECT_EQ(pResized->GetWidth(), 8);
    EXPECT_EQ(pResized->GetHeight(), 8);

    SmartPointer<RawImage> pCropped = img.Crop(2, 2, 8, 8);
    ASSERT_TRUE(static_cast<bool>(pCropped));
    EXPECT_EQ(pCropped->GetWidth(), 8);
    EXPECT_EQ(pCropped->GetHeight(), 8);

    const Array<float> arrTensor = img.ToNormalizedRgbTensor(4, 4);
    EXPECT_EQ(arrTensor.GetLength(), 4 * 4 * 3);

    const String sTempBmp = Path::Combine(Path::GetTempPath(), "dotnetdupe_test_img.bmp");
    img.SaveToBmp(sTempBmp);
    SmartPointer<RawImage> pLoaded = RawImage::LoadFromFile(sTempBmp);
    ASSERT_TRUE(static_cast<bool>(pLoaded));
    EXPECT_EQ(pLoaded->GetWidth(), 16);
    EXPECT_EQ(pLoaded->GetHeight(), 16);
    EXPECT_EQ(pLoaded->GetPixelArgb(4, 5), uExpectedColor);

    File::Delete(sTempBmp);
}

TEST(VectorMathTests, GivenFloatVectors_WhenMathOperationsCalled_ThenReturnsAccurateResults) {
    Array<float> arrA(3);
    arrA[0] = 3.0f;
    arrA[1] = 4.0f;
    arrA[2] = 0.0f;

    Array<float> arrB(3);
    arrB[0] = 3.0f;
    arrB[1] = 4.0f;
    arrB[2] = 0.0f;

    EXPECT_NEAR(VectorMath::Magnitude(arrA), 5.0f, 1e-5f);
    EXPECT_NEAR(VectorMath::DotProduct(arrA, arrB), 25.0f, 1e-5f);
    EXPECT_NEAR(VectorMath::CosineSimilarity(arrA, arrB), 1.0f, 1e-5f);
    EXPECT_NEAR(VectorMath::EuclideanDistance(arrA, arrB), 0.0f, 1e-5f);

    const Array<float> arrNorm = VectorMath::L2Normalize(arrA);
    EXPECT_NEAR(VectorMath::Magnitude(arrNorm), 1.0f, 1e-5f);

    EXPECT_THROW(VectorMath::Magnitude(Array<float>(0)), ArgumentException);
    EXPECT_THROW(VectorMath::DotProduct(arrA, Array<float>(2)), ArgumentException);
}

TEST(FaceRecognitionEngineTests, GivenSyntheticPortraitImages_WhenDetectedAndCompared_ThenMatchesSamePersonAndDistinguishesDifferent) {
    SmartPointer<RawImage> pPortraitA1 = RawImage::Create(32, 32);
    SmartPointer<RawImage> pPortraitA2 = RawImage::Create(32, 32);
    SmartPointer<RawImage> pPortraitB = RawImage::Create(32, 32);

    for (int iY = 0; iY < 32; ++iY) {
        for (int iX = 0; iX < 32; ++iX) {
            const unsigned int uR = (iY == 11 && (iX == 11 || iX == 20)) ? 30U : 220U;
            const unsigned int uG = (iY == 11 && (iX == 11 || iX == 20)) ? 25U : 175U;
            const unsigned int uB = (iY == 11 && (iX == 11 || iX == 20)) ? 25U : 145U;
            const unsigned int uArgbA = 0xFF000000U | (uR << 16) | (uG << 8) | uB;
            pPortraitA1->SetPixelArgb(iX, iY, uArgbA);
            pPortraitA2->SetPixelArgb(iX, iY, uArgbA);

            const unsigned int uBr = static_cast<unsigned int>((iX * 7) & 0xFF);
            const unsigned int uBg = static_cast<unsigned int>((iY * 9) & 0xFF);
            const unsigned int uArgbB = 0xFF000000U | (uBr << 16) | (uBg << 8) | 240U;
            pPortraitB->SetPixelArgb(iX, iY, uArgbB);
        }
    }

    FaceRecognitionEngine engine;
    const List<DetectedFace> lstFaces1 = engine.DetectAndEmbedFaces(pPortraitA1);
    const List<DetectedFace> lstFaces2 = engine.DetectAndEmbedFaces(pPortraitA2);
    ASSERT_EQ(lstFaces1.GetCount(), 1);
    ASSERT_EQ(lstFaces2.GetCount(), 1);
    EXPECT_EQ(lstFaces1[0].GetEmbedding().GetLength(), FaceRecognitionEngine::EmbeddingDimension);
    EXPECT_TRUE(FaceRecognitionEngine::IsSamePerson(
        lstFaces1[0].GetEmbedding(),
        lstFaces2[0].GetEmbedding(),
        0.95f));

    const Array<float> arrRegionB = engine.ComputeFaceRegionEmbedding(pPortraitB, 8, 8, 16, 16);
    EXPECT_FALSE(FaceRecognitionEngine::IsSamePerson(
        lstFaces1[0].GetEmbedding(),
        arrRegionB,
        0.95f));

    SmartPointer<IFaceRecognitionBackend> pMock(SmartPointer<MockFaceBackend>::NewShared());
    engine.SetBackend(pMock);
    EXPECT_EQ(engine.GetBackend().Get(), pMock.Get());
    const List<DetectedFace> lstMockFaces = engine.DetectAndEmbedFaces(pPortraitA1);
    ASSERT_EQ(lstMockFaces.GetCount(), 1);
    EXPECT_EQ(lstMockFaces[0].GetX(), 2);
    EXPECT_EQ(lstMockFaces[0].GetWidth(), 10);
}
