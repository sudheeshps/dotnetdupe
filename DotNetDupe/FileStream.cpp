#include "pch.h"
#include "System/IO/FileStream.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/IOException.h"
#include <filesystem>

#if defined(_WIN32)
#include "Win32Internal.h"
#endif

namespace fs = std::filesystem;

namespace {
    fs::path ToFsPath(const DotNetDupe::System::String& sPath) {
#if defined(_WIN32)
        return fs::path(DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(sPath.GetRawString()));
#else
        return fs::path(sPath.GetRawString());
#endif
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {
            FileStream::FileStream(const String& sPath, int iMode) : m_sPath(sPath), m_iMode(iMode) {
                std::ios_base::openmode openMode = std::ios_base::binary;

                if (iMode == 0) { // FileMode::CreateNew
                    if (fs::exists(ToFsPath(sPath))) {
                        throw IOException("File already exists.");
                    }
                    openMode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
                    m_bCanRead = true;
                    m_bCanWrite = true;
                    m_bCanSeek = true;
                } else if (iMode == 1) { // FileMode::Create
                    openMode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
                    m_bCanRead = true;
                    m_bCanWrite = true;
                    m_bCanSeek = true;
                } else if (iMode == 2) { // FileMode::Open
                    openMode |= std::ios_base::in | std::ios_base::out;
                    m_bCanRead = true;
                    m_bCanWrite = true;
                    m_bCanSeek = true;
                } else if (iMode == 3) { // FileMode::OpenOrCreate
                    openMode |= std::ios_base::in | std::ios_base::out;
                    m_bCanRead = true;
                    m_bCanWrite = true;
                    m_bCanSeek = true;
                } else if (iMode == 4) { // FileMode::Truncate
                    openMode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
                    m_bCanRead = true;
                    m_bCanWrite = true;
                    m_bCanSeek = true;
                } else if (iMode == 5) { // FileMode::Append
                    openMode |= std::ios_base::out | std::ios_base::app;
                    m_bCanRead = false;
                    m_bCanWrite = true;
                    m_bCanSeek = false;
                } else {
                    throw ArgumentException("Invalid FileMode");
                }

                m_fsFileStream.open(ToFsPath(sPath), openMode);
                if (!m_fsFileStream.is_open()) {
                    throw IOException("Could not open file.");
                }
                m_fsFileStream.exceptions(std::fstream::badbit | std::fstream::failbit);
            }

            FileStream::~FileStream() {
                Dispose();
            }

            bool FileStream::CanRead() const {
                return m_bCanRead;
            }

            bool FileStream::CanSeek() const {
                return m_bCanSeek;
            }

            bool FileStream::CanWrite() const {
                return m_bCanWrite;
            }

            long FileStream::GetLength() const {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                auto& mutableStream = const_cast<std::fstream&>(m_fsFileStream);
                long iCurrentPos = static_cast<long>(mutableStream.tellg());
                mutableStream.seekg(0, std::ios_base::end);
                long iLength = static_cast<long>(mutableStream.tellg());
                mutableStream.seekg(iCurrentPos);
                return iLength;
            }

            long FileStream::GetPosition() const {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                auto& mutableStream = const_cast<std::fstream&>(m_fsFileStream);
                return static_cast<long>(mutableStream.tellg());
            }

            void FileStream::SetPosition(long llValue) {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                m_fsFileStream.seekg(llValue);
            }

            void FileStream::Flush() {
                m_fsFileStream.flush();
            }

            int FileStream::Read(char* pBuffer, int iOffset, int nCount) {
                if (!CanRead()) {
                    throw IOException("Stream does not support reading.");
                }
                m_fsFileStream.read(pBuffer + iOffset, nCount);
                if (m_fsFileStream.eof()) {
                    m_fsFileStream.clear();
                }
                return static_cast<int>(m_fsFileStream.gcount());
            }

            long FileStream::Seek(long llOffset, int iOrigin) {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                std::ios_base::seekdir dir;
                if (iOrigin == 0) { // SeekOrigin::Begin
                    dir = std::ios_base::beg;
                } else if (iOrigin == 1) { // SeekOrigin::Current
                    dir = std::ios_base::cur;
                } else if (iOrigin == 2) { // SeekOrigin::End
                    dir = std::ios_base::end;
                } else {
                    throw ArgumentException("Invalid SeekOrigin");
                }
                m_fsFileStream.seekg(llOffset, dir);
                return static_cast<long>(m_fsFileStream.tellg());
            }

            void FileStream::SetLength(long llValue) {
                throw IOException("SetLength is not supported.");
            }

            void FileStream::Write(const char* pBuffer, int iOffset, int nCount) {
                if (!CanWrite()) {
                    throw IOException("Stream does not support writing.");
                }
                m_fsFileStream.write(pBuffer + iOffset, nCount);
            }

            void FileStream::Dispose() {
                if (m_fsFileStream.is_open()) {
                    m_fsFileStream.close();
                }
                m_bCanRead = false;
                m_bCanWrite = false;
                m_bCanSeek = false;
            }
        }
    }
}
