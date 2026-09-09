#include "pch.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/EndOfStreamException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/InvalidOperationException.h"
#include "System/BitConverter.h"
#include <cstring>

namespace DotNetDupe {
    namespace System {
        namespace IO {

            BinaryReader::BinaryReader(Stream* pStream, bool bLeaveOpen, bool bIsLittleEndian)
                : m_pStream(pStream),
                  m_bLeaveOpen(bLeaveOpen),
                  m_bIsLittleEndian(bIsLittleEndian),
                  m_bDisposed(false) {
                /// Guard: Validate stream pointer.
                if (m_pStream == nullptr) {
                    throw ArgumentNullException("pStream cannot be null.");
                }
            }

            BinaryReader::BinaryReader(SmartPointer<Stream> spStream, bool bLeaveOpen, bool bIsLittleEndian)
                : m_pStream(spStream.Get()),
                  m_spOwnedStream(spStream),
                  m_bLeaveOpen(bLeaveOpen),
                  m_bIsLittleEndian(bIsLittleEndian),
                  m_bDisposed(false) {
                /// Guard: Validate smart pointer stream.
                if (m_pStream == nullptr) {
                    throw ArgumentNullException("spStream cannot be null.");
                }
            }

            BinaryReader::~BinaryReader() {
                /// Release reader and stream resources.
                Dispose();
            }

            void BinaryReader::EnsureNotDisposed() const {
                /// Guard: Check reader lifecycle status.
                if (m_bDisposed) {
                    throw InvalidOperationException("BinaryReader is disposed.");
                }
            }

            void BinaryReader::Close() {
                /// Close and dispose reader.
                Dispose();
            }

            void BinaryReader::Dispose() {
                /// Deterministic resource cleanup.
                if (!m_bDisposed) {
                    m_bDisposed = true;
                    if (!m_bLeaveOpen && m_pStream != nullptr) {
                        m_pStream->Dispose();
                    }
                }
            }

            Stream* BinaryReader::GetBaseStream() const {
                return m_pStream;
            }

            bool BinaryReader::IsLittleEndian() const {
                return m_bIsLittleEndian;
            }

            void BinaryReader::SetLittleEndian(bool bIsLittleEndian) {
                m_bIsLittleEndian = bIsLittleEndian;
            }

            void BinaryReader::FillBuffer(char* pBuffer, int iCount) {
                /// Guard: Check active stream state.
                EnsureNotDisposed();

                /// Read required bytes in a loop until buffer is filled.
                int iTotalRead = 0;
                while (iTotalRead < iCount) {
                    int iRead = m_pStream->Read(pBuffer + iTotalRead, 0, iCount - iTotalRead);
                    if (iRead <= 0) {
                        throw EndOfStreamException("Unable to read beyond the end of the stream.");
                    }
                    iTotalRead += iRead;
                }
            }

            /// Reverse bytes in place for endian conversion.
            static void ReverseBytes(char* pBuf, int iSize) {
                for (int i = 0, j = iSize - 1; i < j; ++i, --j) {
                    char chTemp = pBuf[i];
                    pBuf[i] = pBuf[j];
                    pBuf[j] = chTemp;
                }
            }

            bool BinaryReader::ReadBoolean() {
                return ReadByte() != 0;
            }

            byte BinaryReader::ReadByte() {
                char chBuf = 0;
                FillBuffer(&chBuf, 1);
                return static_cast<byte>(chBuf);
            }

            signed char BinaryReader::ReadSByte() {
                return static_cast<signed char>(ReadByte());
            }

            char BinaryReader::ReadChar() {
                return static_cast<char>(ReadByte());
            }

            Array<byte> BinaryReader::ReadBytes(int iCount) {
                /// Guard: Validate count non-negative.
                if (iCount < 0) {
                    throw ArgumentOutOfRangeException("iCount must be non-negative.");
                }
                EnsureNotDisposed();

                /// Read each byte sequentially.
                Array<byte> arrBytes(iCount);
                for (int i = 0; i < iCount; ++i) {
                    arrBytes[i] = ReadByte();
                }
                return arrBytes;
            }

            int BinaryReader::Read(Array<byte>& arrBuffer, int iIndex, int iCount) {
                /// Guard: Validate bounds.
                EnsureNotDisposed();
                if (iIndex < 0 || iCount < 0 || (iIndex + iCount) > arrBuffer.GetLength()) {
                    throw ArgumentOutOfRangeException("Invalid index and count bounds.");
                }

                /// Read directly into buffer slice.
                int iRead = m_pStream->Read(reinterpret_cast<char*>(arrBuffer.GetData()) + iIndex, 0, iCount);
                return iRead < 0 ? 0 : iRead;
            }

            short BinaryReader::ReadInt16() {
                char szBuf[2] = { 0 };
                FillBuffer(szBuf, 2);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 2);
                }
                short iVal = 0;
                std::memcpy(&iVal, szBuf, 2);
                return iVal;
            }

            unsigned short BinaryReader::ReadUInt16() {
                return static_cast<unsigned short>(ReadInt16());
            }

            int BinaryReader::ReadInt32() {
                char szBuf[4] = { 0 };
                FillBuffer(szBuf, 4);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 4);
                }
                int iVal = 0;
                std::memcpy(&iVal, szBuf, 4);
                return iVal;
            }

            unsigned int BinaryReader::ReadUInt32() {
                return static_cast<unsigned int>(ReadInt32());
            }

            long long BinaryReader::ReadInt64() {
                char szBuf[8] = { 0 };
                FillBuffer(szBuf, 8);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 8);
                }
                long long llVal = 0;
                std::memcpy(&llVal, szBuf, 8);
                return llVal;
            }

            unsigned long long BinaryReader::ReadUInt64() {
                return static_cast<unsigned long long>(ReadInt64());
            }

            float BinaryReader::ReadSingle() {
                char szBuf[4] = { 0 };
                FillBuffer(szBuf, 4);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 4);
                }
                float fVal = 0.0f;
                std::memcpy(&fVal, szBuf, 4);
                return fVal;
            }

            double BinaryReader::ReadDouble() {
                char szBuf[8] = { 0 };
                FillBuffer(szBuf, 8);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 8);
                }
                double dVal = 0.0;
                std::memcpy(&dVal, szBuf, 8);
                return dVal;
            }

            String BinaryReader::ReadString(int iLength) {
                /// Guard: Validate length.
                if (iLength < 0) {
                    throw ArgumentOutOfRangeException("iLength must be non-negative.");
                }
                if (iLength == 0) {
                    return String("");
                }

                /// Read raw bytes and convert to null-terminated string.
                Array<byte> arrBytes = ReadBytes(iLength);
                Array<char> arrChars(iLength + 1);
                for (int i = 0; i < iLength; ++i) {
                    arrChars[i] = static_cast<char>(arrBytes[i]);
                }
                arrChars[iLength] = '\0';
                return String(arrChars.GetData());
            }

        }
    }
}
