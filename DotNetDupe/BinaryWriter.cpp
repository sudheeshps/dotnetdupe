#include "pch.h"
#include "System/IO/BinaryWriter.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/InvalidOperationException.h"
#include "System/BitConverter.h"
#include <cstring>

namespace DotNetDupe {
    namespace System {
        namespace IO {

            BinaryWriter::BinaryWriter(Stream* pStream, bool bLeaveOpen, bool bIsLittleEndian)
                : m_pStream(pStream),
                  m_bLeaveOpen(bLeaveOpen),
                  m_bIsLittleEndian(bIsLittleEndian),
                  m_bDisposed(false) {
                if (m_pStream == nullptr) {
                    throw ArgumentNullException("pStream cannot be null.");
                }
            }

            BinaryWriter::BinaryWriter(SmartPointer<Stream> spStream, bool bLeaveOpen, bool bIsLittleEndian)
                : m_pStream(spStream.Get()),
                  m_spOwnedStream(spStream),
                  m_bLeaveOpen(bLeaveOpen),
                  m_bIsLittleEndian(bIsLittleEndian),
                  m_bDisposed(false) {
                if (m_pStream == nullptr) {
                    throw ArgumentNullException("spStream cannot be null.");
                }
            }

            BinaryWriter::~BinaryWriter() {
                Dispose();
            }

            void BinaryWriter::EnsureNotDisposed() const {
                if (m_bDisposed) {
                    throw InvalidOperationException("BinaryWriter is disposed.");
                }
            }

            void BinaryWriter::Close() {
                Dispose();
            }

            void BinaryWriter::Dispose() {
                if (!m_bDisposed) {
                    m_bDisposed = true;
                    if (!m_bLeaveOpen && m_pStream != nullptr) {
                        m_pStream->Dispose();
                    }
                }
            }

            void BinaryWriter::Flush() {
                EnsureNotDisposed();
                m_pStream->Flush();
            }

            Stream* BinaryWriter::GetBaseStream() const {
                return m_pStream;
            }

            bool BinaryWriter::IsLittleEndian() const {
                return m_bIsLittleEndian;
            }

            void BinaryWriter::SetLittleEndian(bool bIsLittleEndian) {
                m_bIsLittleEndian = bIsLittleEndian;
            }

            long BinaryWriter::Seek(long lOffset, int iOrigin) {
                EnsureNotDisposed();
                return m_pStream->Seek(lOffset, iOrigin);
            }

            void BinaryWriter::WriteInternal(const char* pBuffer, int iCount) {
                EnsureNotDisposed();
                m_pStream->Write(pBuffer, 0, iCount);
            }

            static void ReverseBytes(char* pBuf, int iSize) {
                for (int i = 0, j = iSize - 1; i < j; ++i, --j) {
                    char chTemp = pBuf[i];
                    pBuf[i] = pBuf[j];
                    pBuf[j] = chTemp;
                }
            }

            void BinaryWriter::Write(bool bValue) {
                byte bVal = bValue ? 1 : 0;
                Write(bVal);
            }

            void BinaryWriter::Write(byte bValue) {
                char chBuf = static_cast<char>(bValue);
                WriteInternal(&chBuf, 1);
            }

            void BinaryWriter::Write(signed char chValue) {
                char chBuf = static_cast<char>(chValue);
                WriteInternal(&chBuf, 1);
            }

            void BinaryWriter::Write(const Array<byte>& arrBuffer) {
                Write(arrBuffer, 0, arrBuffer.GetLength());
            }

            void BinaryWriter::Write(const Array<byte>& arrBuffer, int iIndex, int iCount) {
                EnsureNotDisposed();
                if (iIndex < 0 || iCount < 0 || (iIndex + iCount) > arrBuffer.GetLength()) {
                    throw ArgumentOutOfRangeException("Invalid index and count bounds.");
                }
                const char* pData = reinterpret_cast<const char*>(arrBuffer.GetData()) + iIndex;
                WriteInternal(pData, iCount);
            }

            void BinaryWriter::Write(short iValue) {
                char szBuf[2] = { 0 };
                std::memcpy(szBuf, &iValue, 2);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 2);
                }
                WriteInternal(szBuf, 2);
            }

            void BinaryWriter::Write(unsigned short uValue) {
                Write(static_cast<short>(uValue));
            }

            void BinaryWriter::Write(int iValue) {
                char szBuf[4] = { 0 };
                std::memcpy(szBuf, &iValue, 4);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 4);
                }
                WriteInternal(szBuf, 4);
            }

            void BinaryWriter::Write(unsigned int uValue) {
                Write(static_cast<int>(uValue));
            }

            void BinaryWriter::Write(long long llValue) {
                char szBuf[8] = { 0 };
                std::memcpy(szBuf, &llValue, 8);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 8);
                }
                WriteInternal(szBuf, 8);
            }

            void BinaryWriter::Write(unsigned long long ullValue) {
                Write(static_cast<long long>(ullValue));
            }

            void BinaryWriter::Write(float fValue) {
                char szBuf[4] = { 0 };
                std::memcpy(szBuf, &fValue, 4);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 4);
                }
                WriteInternal(szBuf, 4);
            }

            void BinaryWriter::Write(double dValue) {
                char szBuf[8] = { 0 };
                std::memcpy(szBuf, &dValue, 8);
                if (m_bIsLittleEndian != BitConverter::IsLittleEndian) {
                    ReverseBytes(szBuf, 8);
                }
                WriteInternal(szBuf, 8);
            }

            void BinaryWriter::Write(const String& sValue) {
                int iLen = sValue.GetLength();
                if (iLen > 0) {
                    WriteInternal(sValue.GetRawString(), iLen);
                }
            }

        }
    }
}
