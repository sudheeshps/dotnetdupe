#include "pch.h"
#include "System/IO/FileStream.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/IOException.h"
#include <filesystem>
#include <fstream>

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
            struct FileStream::Impl {
                std::fstream fs;
            };

            static std::ios_base::openmode DetermineOpenMode(int iMode, const String& sPath, bool& bCanRead, bool& bCanWrite, bool& bCanSeek) {
                if (iMode < 0 || iMode > 5) throw ArgumentException("Invalid FileMode");
                if (iMode == 0 && fs::exists(ToFsPath(sPath))) throw IOException("File already exists.");
                if (iMode == 5) {
                    bCanRead = false; bCanWrite = true; bCanSeek = false;
                    return std::ios_base::binary | std::ios_base::out | std::ios_base::app;
                }
                bCanRead = true; bCanWrite = true; bCanSeek = true;
                std::ios_base::openmode m = std::ios_base::binary | std::ios_base::in | std::ios_base::out;
                if (iMode == 0 || iMode == 1 || iMode == 4) m |= std::ios_base::trunc;
                return m;
            }

            FileStream::FileStream(const String& sPath, int iMode) : m_pImpl(new Impl()), m_sPath(sPath), m_iMode(iMode) {
                std::ios_base::openmode openMode = DetermineOpenMode(iMode, sPath, m_bCanRead, m_bCanWrite, m_bCanSeek);
                m_pImpl->fs.open(ToFsPath(sPath), openMode);
                if (!m_pImpl->fs.is_open()) throw IOException("Could not open file.");
            }

            FileStream::~FileStream() {
                Dispose();
                if (m_pImpl) {
                    delete m_pImpl;
                    m_pImpl = nullptr;
                }
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
                if (!CanSeek() || !m_pImpl) {
                    throw IOException("Stream does not support seeking.");
                }
                auto& mutableStream = m_pImpl->fs;
                long iCurrentPos = static_cast<long>(mutableStream.tellg());
                mutableStream.seekg(0, std::ios_base::end);
                long iLength = static_cast<long>(mutableStream.tellg());
                mutableStream.seekg(iCurrentPos);
                return iLength;
            }

            long FileStream::GetPosition() const {
                if (!CanSeek() || !m_pImpl) {
                    throw IOException("Stream does not support seeking.");
                }
                return static_cast<long>(m_pImpl->fs.tellg());
            }

            void FileStream::SetPosition(long llValue) {
                if (!CanSeek() || !m_pImpl) {
                    throw IOException("Stream does not support seeking.");
                }
                m_pImpl->fs.seekg(llValue);
            }

            void FileStream::Flush() {
                if (!m_pImpl) return;
                m_pImpl->fs.flush();
                if (m_pImpl->fs.bad()) {
                    throw IOException("Flush failed.");
                }
            }

            int FileStream::Read(char* pBuffer, int iOffset, int nCount) {
                if (!CanRead() || !m_pImpl) {
                    throw IOException("Stream does not support reading.");
                }
                m_pImpl->fs.read(pBuffer + iOffset, nCount);
                if (m_pImpl->fs.bad()) {
                    throw IOException("A read error occurred.");
                }
                if (m_pImpl->fs.eof()) {
                    m_pImpl->fs.clear();
                }
                return static_cast<int>(m_pImpl->fs.gcount());
            }

            long FileStream::Seek(long llOffset, int iOrigin) {
                if (!CanSeek() || !m_pImpl) {
                    throw IOException("Stream does not support seeking.");
                }
                std::ios_base::seekdir dir;
                if (iOrigin == 0) {
                    dir = std::ios_base::beg;
                } else if (iOrigin == 1) {
                    dir = std::ios_base::cur;
                } else if (iOrigin == 2) {
                    dir = std::ios_base::end;
                } else {
                    throw ArgumentException("Invalid SeekOrigin");
                }
                m_pImpl->fs.seekg(llOffset, dir);
                return static_cast<long>(m_pImpl->fs.tellg());
            }

            void FileStream::SetLength(long llValue) {
                throw IOException("SetLength is not supported.");
            }

            void FileStream::Write(const char* pBuffer, int iOffset, int nCount) {
                if (!CanWrite() || !m_pImpl) {
                    throw IOException("Stream does not support writing.");
                }
                m_pImpl->fs.write(pBuffer + iOffset, nCount);
                if (m_pImpl->fs.bad() || m_pImpl->fs.fail()) {
                    throw IOException("A write error occurred.");
                }
            }

            void FileStream::Dispose() {
                if (m_pImpl && m_pImpl->fs.is_open()) {
                    m_pImpl->fs.close();
                }
                m_bCanRead = false;
                m_bCanWrite = false;
                m_bCanSeek = false;
            }
        }
    }
}
