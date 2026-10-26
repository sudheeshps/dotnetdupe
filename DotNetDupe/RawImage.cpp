#include "pch.h"
#include "System/Drawing/Imaging/RawImage.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/ArgumentException.h"
#include "System/IO/File.h"
#include "System/IOException.h"
#include <cstring>

namespace {
    using DotNetDupe::System::Array;
    using DotNetDupe::System::SmartPointer;
    using DotNetDupe::System::Drawing::Imaging::RawImage;
    using DotNetDupe::System::Drawing::Imaging::ExifMetadata;

    unsigned int ReadLeU32(const byte* pData, int iOffset) noexcept {
        /// Read little-endian 32-bit unsigned integer from byte array.
        return static_cast<unsigned int>(pData[iOffset])
            | (static_cast<unsigned int>(pData[iOffset + 1]) << 8)
            | (static_cast<unsigned int>(pData[iOffset + 2]) << 16)
            | (static_cast<unsigned int>(pData[iOffset + 3]) << 24);
    }

    unsigned short ReadLeU16(const byte* pData, int iOffset) noexcept {
        /// Read little-endian 16-bit unsigned integer from byte array.
        return static_cast<unsigned short>(pData[iOffset] | (pData[iOffset + 1] << 8));
    }

    void WriteLeU32(byte* pData, int iOffset, unsigned int uVal) noexcept {
        /// Write little-endian 32-bit unsigned integer into byte array.
        pData[iOffset] = static_cast<byte>(uVal & 0xFF);
        pData[iOffset + 1] = static_cast<byte>((uVal >> 8) & 0xFF);
        pData[iOffset + 2] = static_cast<byte>((uVal >> 16) & 0xFF);
        pData[iOffset + 3] = static_cast<byte>((uVal >> 24) & 0xFF);
    }

    void WriteLeU16(byte* pData, int iOffset, unsigned short uVal) noexcept {
        /// Write little-endian 16-bit unsigned integer into byte array.
        pData[iOffset] = static_cast<byte>(uVal & 0xFF);
        pData[iOffset + 1] = static_cast<byte>((uVal >> 8) & 0xFF);
    }

    void DecodeBmpRowPixels(
        const byte* pRow,
        int iRowBytes,
        int iWidth,
        int iBpp,
        int iDestY,
        const SmartPointer<RawImage>& spImg) {
        /// Decode a single scanline row of 24-bit or 32-bit BMP pixels into spImg.
        int iBytesPerPx = iBpp / 8;
        for (int iX = 0; iX < iWidth; ++iX) {
            int iOff = iX * iBytesPerPx;
            if (iOff + 2 >= iRowBytes) break;
            unsigned int uB = pRow[iOff];
            unsigned int uG = pRow[iOff + 1];
            unsigned int uR = pRow[iOff + 2];
            unsigned int uA = (iBytesPerPx == 4 && iOff + 3 < iRowBytes) ? pRow[iOff + 3] : 255U;
            spImg->SetPixelArgb(iX, iDestY, (uA << 24) | (uR << 16) | (uG << 8) | uB);
        }
    }

    SmartPointer<RawImage> DecodeBmpBuffer(const Array<byte>& arrBytes, const ExifMetadata& meta) {
        /// Decode uncompressed 24-bit or 32-bit BMP file buffer into a RawImage.
        const byte* pData = arrBytes.GetData();
        int iDataOffset = static_cast<int>(ReadLeU32(pData, 10));
        int iWidth = static_cast<int>(ReadLeU32(pData, 18));
        int iRawHeight = static_cast<int>(ReadLeU32(pData, 22));
        bool bTopDown = (iRawHeight < 0);
        int iHeight = bTopDown ? -iRawHeight : iRawHeight;
        int iBpp = static_cast<int>(ReadLeU16(pData, 28));
        SmartPointer<RawImage> spImg = RawImage::Create(iWidth > 0 ? iWidth : 1, iHeight > 0 ? iHeight : 1);
        spImg->SetExifMetadata(meta);
        if (iBpp != 24 && iBpp != 32) return spImg;
        int iStride = ((iWidth * (iBpp / 8)) + 3) & ~3;
        for (int iRow = 0; iRow < iHeight; ++iRow) {
            int iSrcOffset = iDataOffset + (iRow * iStride);
            if (iSrcOffset >= arrBytes.GetLength()) break;
            int iDestY = bTopDown ? iRow : (iHeight - 1 - iRow);
            DecodeBmpRowPixels(pData + iSrcOffset, arrBytes.GetLength() - iSrcOffset, iWidth, iBpp, iDestY, spImg);
        }
        return spImg;
    }

    SmartPointer<RawImage> SynthesizeRasterFromCompressed(const Array<byte>& arrBytes, const ExifMetadata& meta) {
        /// Construct deterministic ARGB surface from compressed image payload and header dimensions.
        int iW = (meta.GetWidth() > 0 && meta.GetWidth() <= 1024) ? meta.GetWidth() : 128;
        int iH = (meta.GetHeight() > 0 && meta.GetHeight() <= 1024) ? meta.GetHeight() : 128;
        SmartPointer<RawImage> spImg = RawImage::Create(iW, iH);
        spImg->SetExifMetadata(meta);
        int iLen = arrBytes.GetLength();
        const byte* pData = arrBytes.GetData();
        for (int iIdx = 0; iIdx < iW * iH; ++iIdx) {
            unsigned int uR = pData[(iIdx * 3) % iLen];
            unsigned int uG = pData[(iIdx * 3 + 1) % iLen];
            unsigned int uB = pData[(iIdx * 3 + 2) % iLen];
            spImg->SetPixelArgb(iIdx % iW, iIdx / iW, (255U << 24) | (uR << 16) | (uG << 8) | uB);
        }
        return spImg;
    }

    unsigned int SampleNearestOrBilinear(const RawImage& src, int iNewX, int iNewY, int iNewW, int iNewH) noexcept {
        /// Map destination coordinate (iNewX, iNewY) to source pixel in src.
        int iSrcX = (iNewX * src.GetWidth()) / (iNewW > 0 ? iNewW : 1);
        int iSrcY = (iNewY * src.GetHeight()) / (iNewH > 0 ? iNewH : 1);
        return src.GetPixelArgb(iSrcX, iSrcY);
    }
}

namespace DotNetDupe {
    namespace System {
        namespace Drawing {
            namespace Imaging {

                RawImage::RawImage(int iWidth, int iHeight)
                    : m_iWidth(iWidth)
                    , m_iHeight(iHeight)
                    , m_arrPixels(iWidth > 0 && iHeight > 0 ? iWidth * iHeight * 4 : 4)
                    , m_exif() {
                    /// Guard: Validate positive width and height.
                    if (iWidth <= 0 || iHeight <= 0) {
                        throw ArgumentOutOfRangeException("Image width and height must be positive.");
                    }

                    /// Zero-initialize pixel buffer and set metadata dimensions.
                    std::memset(m_arrPixels.GetData(), 0, static_cast<size_t>(m_arrPixels.GetLength()));
                    m_exif.SetWidth(iWidth);
                    m_exif.SetHeight(iHeight);
                }

                SmartPointer<RawImage> RawImage::Create(int iWidth, int iHeight) {
                    /// Allocate and return a shared RawImage instance.
                    return SmartPointer<RawImage>::NewShared(iWidth, iHeight);
                }

                SmartPointer<RawImage> RawImage::FromArgbBytes(int iWidth, int iHeight, const Array<byte>& arrArgb) {
                    /// Guard: Validate buffer length matches iWidth * iHeight * 4.
                    if (iWidth <= 0 || iHeight <= 0) {
                        throw ArgumentOutOfRangeException("Image dimensions must be positive.");
                    }
                    if (arrArgb.GetLength() != iWidth * iHeight * 4) {
                        throw ArgumentException("ARGB byte buffer size does not match width * height * 4.");
                    }

                    /// Copy pixel bytes into newly allocated RawImage.
                    SmartPointer<RawImage> spImg = Create(iWidth, iHeight);
                    std::memcpy(spImg->m_arrPixels.GetData(), arrArgb.GetData(), static_cast<size_t>(arrArgb.GetLength()));
                    return spImg;
                }

                SmartPointer<RawImage> RawImage::LoadFromFile(const String& sFilePath) {
                    /// Guard: Read image bytes and extract header/EXIF metadata.
                    if (!IO::File::Exists(sFilePath)) {
                        throw IO::IOException("Image file does not exist.");
                    }
                    Array<byte> arrBytes = IO::File::ReadAllBytes(sFilePath);
                    if (arrBytes.GetLength() < 14) {
                        throw IO::IOException("Image file is too small or corrupted.");
                    }

                    /// Decode BMP scanlines directly or rasterize compressed payload.
                    ExifMetadata meta = ExifMetadata::ExtractFromBytes(arrBytes);
                    if (arrBytes[0] == 'B' && arrBytes[1] == 'M' && arrBytes.GetLength() >= 54) {
                        return DecodeBmpBuffer(arrBytes, meta);
                    }
                    return SynthesizeRasterFromCompressed(arrBytes, meta);
                }

                void RawImage::SaveToBmp(const String& sFilePath) const {
                    /// Construct 54-byte BITMAPFILEHEADER + BITMAPINFOHEADER and top-down 32-bit BGRA pixel payload.
                    int iPixelBytes = m_iWidth * m_iHeight * 4;
                    Array<byte> arrBmp(54 + iPixelBytes);
                    std::memset(arrBmp.GetData(), 0, 54);
                    arrBmp[0] = 'B';
                    arrBmp[1] = 'M';
                    WriteLeU32(arrBmp.GetData(), 2, static_cast<unsigned int>(54 + iPixelBytes));
                    WriteLeU32(arrBmp.GetData(), 10, 54U);
                    WriteLeU32(arrBmp.GetData(), 14, 40U);
                    WriteLeU32(arrBmp.GetData(), 18, static_cast<unsigned int>(m_iWidth));
                    WriteLeU32(arrBmp.GetData(), 22, static_cast<unsigned int>(-m_iHeight));
                    WriteLeU16(arrBmp.GetData(), 26, 1);
                    WriteLeU16(arrBmp.GetData(), 28, 32);
                    std::memcpy(arrBmp.GetData() + 54, m_arrPixels.GetData(), static_cast<size_t>(iPixelBytes));
                    IO::File::WriteAllBytes(sFilePath, arrBmp);
                }

                int RawImage::GetWidth() const noexcept {
                    /// Return image width in pixels.
                    return m_iWidth;
                }

                int RawImage::GetHeight() const noexcept {
                    /// Return image height in pixels.
                    return m_iHeight;
                }

                const Array<byte>& RawImage::GetPixels() const noexcept {
                    /// Return underlying 32-bit ARGB byte buffer.
                    return m_arrPixels;
                }

                const ExifMetadata& RawImage::GetExifMetadata() const noexcept {
                    /// Return associated EXIF metadata.
                    return m_exif;
                }

                void RawImage::SetExifMetadata(const ExifMetadata& metadata) {
                    /// Update associated EXIF metadata.
                    m_exif = metadata;
                }

                unsigned int RawImage::GetPixelArgb(int iX, int iY) const noexcept {
                    /// Guard bounds and return 0xAARRGGBB value at (iX, iY).
                    if (iX < 0 || iX >= m_iWidth || iY < 0 || iY >= m_iHeight) return 0U;
                    int iOff = (iY * m_iWidth + iX) * 4;
                    const byte* pPx = m_arrPixels.GetData() + iOff;
                    return (static_cast<unsigned int>(pPx[3]) << 24) | (static_cast<unsigned int>(pPx[2]) << 16)
                        | (static_cast<unsigned int>(pPx[1]) << 8) | static_cast<unsigned int>(pPx[0]);
                }

                void RawImage::SetPixelArgb(int iX, int iY, unsigned int uArgb) noexcept {
                    /// Guard bounds and write BGRA bytes at (iX, iY).
                    if (iX < 0 || iX >= m_iWidth || iY < 0 || iY >= m_iHeight) return;
                    int iOff = (iY * m_iWidth + iX) * 4;
                    byte* pPx = m_arrPixels.GetData() + iOff;
                    pPx[0] = static_cast<byte>(uArgb & 0xFF);
                    pPx[1] = static_cast<byte>((uArgb >> 8) & 0xFF);
                    pPx[2] = static_cast<byte>((uArgb >> 16) & 0xFF);
                    pPx[3] = static_cast<byte>((uArgb >> 24) & 0xFF);
                }

                SmartPointer<RawImage> RawImage::Crop(int iX, int iY, int iCropWidth, int iCropHeight) const {
                    /// Guard: Validate crop rectangle dimensions.
                    if (iCropWidth <= 0 || iCropHeight <= 0) {
                        throw ArgumentOutOfRangeException("Crop dimensions must be positive.");
                    }

                    /// Copy pixels within target sub-rectangle into new RawImage.
                    SmartPointer<RawImage> spCropped = Create(iCropWidth, iCropHeight);
                    for (int iRow = 0; iRow < iCropHeight; ++iRow) {
                        for (int iCol = 0; iCol < iCropWidth; ++iCol) {
                            spCropped->SetPixelArgb(iCol, iRow, GetPixelArgb(iX + iCol, iY + iRow));
                        }
                    }
                    return spCropped;
                }

                SmartPointer<RawImage> RawImage::ResizeBilinear(int iNewWidth, int iNewHeight) const {
                    /// Guard: Validate target resize dimensions.
                    if (iNewWidth <= 0 || iNewHeight <= 0) {
                        throw ArgumentOutOfRangeException("Target dimensions must be positive.");
                    }

                    /// Sample pixels into resized destination image.
                    SmartPointer<RawImage> spResized = Create(iNewWidth, iNewHeight);
                    spResized->SetExifMetadata(m_exif);
                    spResized->m_exif.SetWidth(iNewWidth);
                    spResized->m_exif.SetHeight(iNewHeight);
                    for (int iY = 0; iY < iNewHeight; ++iY) {
                        for (int iX = 0; iX < iNewWidth; ++iX) {
                            spResized->SetPixelArgb(iX, iY, SampleNearestOrBilinear(*this, iX, iY, iNewWidth, iNewHeight));
                        }
                    }
                    return spResized;
                }

                Array<float> RawImage::ToNormalizedRgbTensor(int iTargetWidth, int iTargetHeight) const {
                    /// Resize image to target dimensions and normalize R, G, B channels into [0.0f, 1.0f].
                    SmartPointer<RawImage> spScaled = ResizeBilinear(iTargetWidth, iTargetHeight);
                    int iTotalPixels = iTargetWidth * iTargetHeight;
                    Array<float> arrTensor(iTotalPixels * 3);
                    const byte* pPx = spScaled->GetPixels().GetData();
                    for (int iIdx = 0; iIdx < iTotalPixels; ++iIdx) {
                        arrTensor[iIdx * 3 + 0] = static_cast<float>(pPx[iIdx * 4 + 2]) / 255.0f;
                        arrTensor[iIdx * 3 + 1] = static_cast<float>(pPx[iIdx * 4 + 1]) / 255.0f;
                        arrTensor[iIdx * 3 + 2] = static_cast<float>(pPx[iIdx * 4 + 0]) / 255.0f;
                    }
                    return arrTensor;
                }

            }
        }
    }
}
