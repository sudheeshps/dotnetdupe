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
    fs::path ToFsPath(const DotNetDupe::System::String& path) {
#if defined(_WIN32)
        return fs::path(DotNetDupe::System::Internal::Utf8ToWChar(path.GetRawString()));
#else
        return fs::path(path.GetRawString());
#endif
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {
            FileStream::FileStream(const String& path, int mode) : _path(path), _mode(mode) {
                std::ios_base::openmode openMode = std::ios_base::binary;

                if (mode == 0) { // FileMode::CreateNew
                    if (fs::exists(ToFsPath(path))) {
                        throw IOException("File already exists.");
                    }
                    openMode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
                    _canRead = true;
                    _canWrite = true;
                    _canSeek = true;
                } else if (mode == 1) { // FileMode::Create
                    openMode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
                    _canRead = true;
                    _canWrite = true;
                    _canSeek = true;
                } else if (mode == 2) { // FileMode::Open
                    openMode |= std::ios_base::in | std::ios_base::out;
                    _canRead = true;
                    _canWrite = true;
                    _canSeek = true;
                } else if (mode == 3) { // FileMode::OpenOrCreate
                    openMode |= std::ios_base::in | std::ios_base::out;
                    _canRead = true;
                    _canWrite = true;
                    _canSeek = true;
                } else if (mode == 4) { // FileMode::Truncate
                    openMode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
                    _canRead = true;
                    _canWrite = true;
                    _canSeek = true;
                } else if (mode == 5) { // FileMode::Append
                    openMode |= std::ios_base::out | std::ios_base::app;
                    _canRead = false;
                    _canWrite = true;
                    _canSeek = false;
                } else {
                    throw ArgumentException("Invalid FileMode");
                }

                _fileStream.open(ToFsPath(path), openMode);
                if (!_fileStream.is_open()) {
                    throw IOException("Could not open file.");
                }
                _fileStream.exceptions(std::fstream::badbit | std::fstream::failbit);
            }

            FileStream::~FileStream() {
                Dispose();
            }

            bool FileStream::CanRead() const {
                return _canRead;
            }

            bool FileStream::CanSeek() const {
                return _canSeek;
            }

            bool FileStream::CanWrite() const {
                return _canWrite;
            }

            long FileStream::GetLength() const {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                auto& mutableStream = const_cast<std::fstream&>(_fileStream);
                long currentPos = static_cast<long>(mutableStream.tellg());
                mutableStream.seekg(0, std::ios_base::end);
                long length = static_cast<long>(mutableStream.tellg());
                mutableStream.seekg(currentPos);
                return length;
            }

            long FileStream::GetPosition() const {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                auto& mutableStream = const_cast<std::fstream&>(_fileStream);
                return static_cast<long>(mutableStream.tellg());
            }

            void FileStream::SetPosition(long value) {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                _fileStream.seekg(value);
            }

            void FileStream::Flush() {
                _fileStream.flush();
            }

            int FileStream::Read(char* buffer, int offset, int count) {
                if (!CanRead()) {
                    throw IOException("Stream does not support reading.");
                }
                _fileStream.read(buffer + offset, count);
                if (_fileStream.eof()) {
                    _fileStream.clear();
                }
                return static_cast<int>(_fileStream.gcount());
            }

            long FileStream::Seek(long offset, int origin) {
                if (!CanSeek()) {
                    throw IOException("Stream does not support seeking.");
                }
                std::ios_base::seekdir dir;
                if (origin == 0) { // SeekOrigin::Begin
                    dir = std::ios_base::beg;
                } else if (origin == 1) { // SeekOrigin::Current
                    dir = std::ios_base::cur;
                } else if (origin == 2) { // SeekOrigin::End
                    dir = std::ios_base::end;
                } else {
                    throw ArgumentException("Invalid SeekOrigin");
                }
                _fileStream.seekg(offset, dir);
                return static_cast<long>(_fileStream.tellg());
            }

            void FileStream::SetLength(long value) {
                throw IOException("SetLength is not supported.");
            }

            void FileStream::Write(const char* buffer, int offset, int count) {
                if (!CanWrite()) {
                    throw IOException("Stream does not support writing.");
                }
                _fileStream.write(buffer + offset, count);
            }

            void FileStream::Dispose() {
                if (_fileStream.is_open()) {
                    _fileStream.close();
                }
            }
        }
    }
}
