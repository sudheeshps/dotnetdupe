#include "pch.h"
#include "System/Drawing/Imaging/ExifMetadata.h"
#include "System/IO/File.h"
#include "System/IO/FileStream.h"
#include <cstring>

namespace {
    using DotNetDupe::System::Array;
    using DotNetDupe::System::String;
    using DotNetDupe::System::Drawing::Imaging::ExifMetadata;

    unsigned short ReadU16(const byte* pData, int iOffset, bool bLittleEndian) noexcept {
        /// Read 16-bit unsigned integer with specified byte order.
        if (bLittleEndian) {
            return static_cast<unsigned short>(pData[iOffset] | (pData[iOffset + 1] << 8));
        }
        return static_cast<unsigned short>((pData[iOffset] << 8) | pData[iOffset + 1]);
    }

    unsigned int ReadU32(const byte* pData, int iOffset, bool bLittleEndian) noexcept {
        /// Read 32-bit unsigned integer with specified byte order.
        if (bLittleEndian) {
            return static_cast<unsigned int>(pData[iOffset]) | (static_cast<unsigned int>(pData[iOffset + 1]) << 8)
                | (static_cast<unsigned int>(pData[iOffset + 2]) << 16) | (static_cast<unsigned int>(pData[iOffset + 3]) << 24);
        }
        return (static_cast<unsigned int>(pData[iOffset]) << 24) | (static_cast<unsigned int>(pData[iOffset + 1]) << 16)
            | (static_cast<unsigned int>(pData[iOffset + 2]) << 8) | static_cast<unsigned int>(pData[iOffset + 3]);
    }

    String ReadExifAscii(const byte* pTiffBase, int iTiffLen, unsigned int uCount, unsigned int uValueOffset) {
        /// Extract null-terminated ASCII string from TIFF value offset.
        if (uCount == 0 || uCount > 256 || static_cast<int>(uValueOffset + uCount) > iTiffLen) return String("");
        std::string sVal(reinterpret_cast<const char*>(pTiffBase + uValueOffset), uCount);
        while (!sVal.empty() && (sVal.back() == '\0' || sVal.back() == ' ')) sVal.pop_back();
        return String(sVal.c_str());
    }

    void ApplyTiffEntry(
        const byte* pTiff,
        int iTiffLen,
        int iEntryPos,
        bool bLE,
        unsigned int& uSubIfdOffset,
        ExifMetadata& meta) {
        /// Decode a single 12-byte TIFF IFD entry and populate matching ExifMetadata fields.
        unsigned short uTag = ReadU16(pTiff, iEntryPos, bLE);
        unsigned short uType = ReadU16(pTiff, iEntryPos + 2, bLE);
        unsigned int uCount = ReadU32(pTiff, iEntryPos + 4, bLE);
        unsigned int uValOff = ReadU32(pTiff, iEntryPos + 8, bLE);
        unsigned short uShortVal = ReadU16(pTiff, iEntryPos + 8, bLE);
        if (uTag == 0x010F && uType == 2) meta.SetCameraMake(ReadExifAscii(pTiff, iTiffLen, uCount, uValOff));
        else if (uTag == 0x0110 && uType == 2) meta.SetCameraModel(ReadExifAscii(pTiff, iTiffLen, uCount, uValOff));
        else if (uTag == 0x0112 && uType == 3) meta.SetOrientation(static_cast<int>(uShortVal));
        else if ((uTag == 0x0132 || uTag == 0x9003) && uType == 2) meta.SetDateTaken(ReadExifAscii(pTiff, iTiffLen, uCount, uValOff));
        else if (uTag == 0xA002 || uTag == 0x0100) meta.SetWidth(static_cast<int>(uType == 3 ? uShortVal : uValOff));
        else if (uTag == 0xA003 || uTag == 0x0101) meta.SetHeight(static_cast<int>(uType == 3 ? uShortVal : uValOff));
        else if (uTag == 0x8769) uSubIfdOffset = uValOff;
    }

    void ParseTiffIfd(const byte* pTiff, int iTiffLen, unsigned int uIfdOffset, bool bLE, ExifMetadata& meta) {
        /// Parse all entries in a TIFF Image File Directory (IFD) and follow ExifSubIFD if present.
        if (uIfdOffset + 2 > static_cast<unsigned int>(iTiffLen)) return;
        unsigned short uNumEntries = ReadU16(pTiff, static_cast<int>(uIfdOffset), bLE);
        unsigned int uSubIfd = 0;
        for (int iIdx = 0; iIdx < static_cast<int>(uNumEntries); ++iIdx) {
            int iEntryPos = static_cast<int>(uIfdOffset) + 2 + (iIdx * 12);
            if (iEntryPos + 12 > iTiffLen) break;
            ApplyTiffEntry(pTiff, iTiffLen, iEntryPos, bLE, uSubIfd, meta);
        }
        if (uSubIfd > 0 && uSubIfd != uIfdOffset) ParseTiffIfd(pTiff, iTiffLen, uSubIfd, bLE, meta);
    }

    void ParseTiffHeader(const byte* pTiff, int iTiffLen, ExifMetadata& meta) {
        /// Validate II/MM TIFF byte order header and parse root IFD0.
        if (iTiffLen < 8) return;
        bool bLE = (pTiff[0] == 'I' && pTiff[1] == 'I');
        bool bBE = (pTiff[0] == 'M' && pTiff[1] == 'M');
        if (!bLE && !bBE) return;
        if (ReadU16(pTiff, 2, bLE) != 42) return;
        meta.SetHasExif(true);
        unsigned int uIfd0 = ReadU32(pTiff, 4, bLE);
        ParseTiffIfd(pTiff, iTiffLen, uIfd0, bLE, meta);
    }

    void InspectJpegSegment(const byte* pData, int iPos, int iSegLen, ExifMetadata& meta) {
        /// Inspect a single JPEG marker segment for APP1 Exif or SOF0/SOF2 dimensions.
        byte bMarker = pData[iPos + 1];
        if (bMarker == 0xE1 && iSegLen >= 14 && std::memcmp(pData + iPos + 4, "Exif\0\0", 6) == 0) {
            ParseTiffHeader(pData + iPos + 10, iSegLen - 8, meta);
        } else if ((bMarker == 0xC0 || bMarker == 0xC2) && iSegLen >= 7) {
            meta.SetHeight(static_cast<int>(ReadU16(pData, iPos + 5, false)));
            meta.SetWidth(static_cast<int>(ReadU16(pData, iPos + 7, false)));
            meta.SetHasExif(true);
        }
    }

    void ParseJpegBytes(const byte* pData, int iLen, ExifMetadata& meta) {
        /// Traverse JPEG segments starting after SOI (0xFFD8) to extract APP1 Exif and SOF dimensions.
        int iPos = 2;
        while (iPos + 4 <= iLen && pData[iPos] == 0xFF) {
            byte bMarker = pData[iPos + 1];
            if (bMarker == 0xD9 || bMarker == 0xDA) break;
            int iSegLen = static_cast<int>(ReadU16(pData, iPos + 2, false));
            if (iSegLen < 2 || iPos + 2 + iSegLen > iLen) break;
            InspectJpegSegment(pData, iPos, iSegLen, meta);
            iPos += 2 + iSegLen;
        }
    }

    void ParsePngOrBmpBytes(const byte* pData, int iLen, ExifMetadata& meta) {
        /// Extract width and height from PNG IHDR chunk or BMP BITMAPINFOHEADER.
        if (iLen >= 24 && pData[0] == 0x89 && pData[1] == 'P' && pData[2] == 'N' && pData[3] == 'G') {
            meta.SetWidth(static_cast<int>(ReadU32(pData, 16, false)));
            meta.SetHeight(static_cast<int>(ReadU32(pData, 20, false)));
            meta.SetHasExif(true);
        } else if (iLen >= 26 && pData[0] == 'B' && pData[1] == 'M') {
            meta.SetWidth(static_cast<int>(ReadU32(pData, 18, true)));
            int iRawH = static_cast<int>(ReadU32(pData, 22, true));
            meta.SetHeight(iRawH < 0 ? -iRawH : iRawH);
            meta.SetHasExif(true);
        }
    }
}

namespace DotNetDupe {
    namespace System {
        namespace Drawing {
            namespace Imaging {

                ExifMetadata::ExifMetadata()
                    : m_iWidth(0)
                    , m_iHeight(0)
                    , m_iOrientation(1)
                    , m_sDateTaken("")
                    , m_sCameraMake("")
                    , m_sCameraModel("")
                    , m_bHasExif(false) {
                    /// Initialize default ExifMetadata with normal orientation (1).
                }

                int ExifMetadata::GetWidth() const noexcept {
                    /// Return image width in pixels.
                    return m_iWidth;
                }

                void ExifMetadata::SetWidth(int iWidth) noexcept {
                    /// Set image width in pixels.
                    m_iWidth = iWidth;
                }

                int ExifMetadata::GetHeight() const noexcept {
                    /// Return image height in pixels.
                    return m_iHeight;
                }

                void ExifMetadata::SetHeight(int iHeight) noexcept {
                    /// Set image height in pixels.
                    m_iHeight = iHeight;
                }

                int ExifMetadata::GetOrientation() const noexcept {
                    /// Return EXIF orientation tag.
                    return m_iOrientation;
                }

                void ExifMetadata::SetOrientation(int iOrientation) noexcept {
                    /// Set EXIF orientation tag.
                    m_iOrientation = iOrientation;
                }

                String ExifMetadata::GetDateTaken() const {
                    /// Return EXIF DateTimeOriginal string.
                    return m_sDateTaken;
                }

                void ExifMetadata::SetDateTaken(const String& sDateTaken) {
                    /// Set EXIF DateTimeOriginal string.
                    m_sDateTaken = sDateTaken;
                }

                String ExifMetadata::GetCameraMake() const {
                    /// Return camera manufacturer make string.
                    return m_sCameraMake;
                }

                void ExifMetadata::SetCameraMake(const String& sCameraMake) {
                    /// Set camera manufacturer make string.
                    m_sCameraMake = sCameraMake;
                }

                String ExifMetadata::GetCameraModel() const {
                    /// Return camera model string.
                    return m_sCameraModel;
                }

                void ExifMetadata::SetCameraModel(const String& sCameraModel) {
                    /// Set camera model string.
                    m_sCameraModel = sCameraModel;
                }

                bool ExifMetadata::GetHasExif() const noexcept {
                    /// Return whether valid EXIF or image header metadata was discovered.
                    return m_bHasExif;
                }

                void ExifMetadata::SetHasExif(bool bHasExif) noexcept {
                    /// Set whether valid EXIF or image header metadata was discovered.
                    m_bHasExif = bHasExif;
                }

                ExifMetadata ExifMetadata::ExtractFromBytes(const Array<byte>& arrBytes) {
                    /// Guard: Ensure buffer has enough bytes for a magic header.
                    ExifMetadata meta;
                    int iLen = arrBytes.GetLength();
                    if (iLen < 4) return meta;

                    /// Dispatch to JPEG parser or PNG/BMP header parser.
                    const byte* pData = arrBytes.GetData();
                    if (pData[0] == 0xFF && pData[1] == 0xD8) {
                        ParseJpegBytes(pData, iLen, meta);
                    } else {
                        ParsePngOrBmpBytes(pData, iLen, meta);
                    }
                    return meta;
                }

                ExifMetadata ExifMetadata::ExtractFromFile(const String& sFilePath) {
                    /// Guard: Return empty metadata if file does not exist.
                    if (!IO::File::Exists(sFilePath)) return ExifMetadata();

                    /// Read up to the first 64KB of the image file to parse headers and APP1 EXIF tags.
                    IO::FileStream stm(sFilePath, 3);
                    long long llFileLen = stm.GetLength64();
                    int iReadLen = (llFileLen < 65536LL) ? static_cast<int>(llFileLen) : 65536;
                    Array<byte> arrHeader(iReadLen);
                    if (iReadLen > 0) {
                        stm.Read(reinterpret_cast<char*>(arrHeader.GetData()), 0, iReadLen);
                    }
                    return ExtractFromBytes(arrHeader);
                }

            }
        }
    }
}
