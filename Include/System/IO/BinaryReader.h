#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/IO/IDisposable.h"
#include "System/IO/Stream.h"
#include "System/Array.h"
#include "System/String.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

            /// \class BinaryReader
            /// \brief Reads primitive data types as binary values in a specific encoding.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.54 and IEEE 754 specifications.
            /// Supports configurable endianness (little-endian by default) and wraps underlying Stream
            /// objects with optional ownership semantics via SmartPointer.
            class BinaryReader : public Object, public IDisposable {
            public:
                /// \brief Initializes a new instance of the BinaryReader class based on the specified stream and endianness.
                /// \param pStream The input stream.
                /// \param bLeaveOpen true to leave the stream open after the BinaryReader object is disposed; otherwise, false.
                /// \param bIsLittleEndian true to read multi-byte values in little-endian format; false for big-endian.
                /// \throws ArgumentNullException If pStream is null.
                DOTNETDUPE_API explicit BinaryReader(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);

                /// \brief Initializes a new instance of the BinaryReader class taking ownership of the specified stream.
                /// \param spStream The smart-pointer managed input stream.
                /// \param bLeaveOpen true to leave the stream open after disposal; otherwise, false.
                /// \param bIsLittleEndian true for little-endian; false for big-endian.
                /// \throws ArgumentNullException If spStream is null.
                DOTNETDUPE_API explicit BinaryReader(SmartPointer<Stream> spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);

                /// \brief Destructor ensuring deterministic cleanup and closing.
                DOTNETDUPE_API virtual ~BinaryReader();

                /// \brief Closes the current reader and the underlying stream if configured.
                DOTNETDUPE_API virtual void Close();

                /// \brief Releases all unmanaged resources used by the BinaryReader.
                DOTNETDUPE_API void Dispose() override;

                /// \brief Exposes access to the underlying stream.
                /// \return The underlying stream.
                DOTNETDUPE_API Stream* GetBaseStream() const;

                /// \brief Gets whether values are decoded in little-endian order.
                /// \return true if little-endian; false if big-endian.
                DOTNETDUPE_API bool IsLittleEndian() const;

                /// \brief Sets the endianness for binary decoding.
                /// \param bIsLittleEndian true for little-endian; false for big-endian.
                DOTNETDUPE_API void SetLittleEndian(bool bIsLittleEndian);

                /// \brief Reads a Boolean value from the current stream and advances the current position by one byte.
                /// \return true if byte is non-zero; otherwise, false.
                DOTNETDUPE_API virtual bool ReadBoolean();

                /// \brief Reads the next byte from the current stream and advances the current position by one byte.
                /// \return The next byte read from the current stream.
                DOTNETDUPE_API virtual byte ReadByte();

                /// \brief Reads a signed byte from this stream and advances the current position by one byte.
                /// \return A signed byte read from the current stream.
                DOTNETDUPE_API virtual signed char ReadSByte();

                /// \brief Reads the next character from the current stream.
                /// \return A character read from the current stream.
                DOTNETDUPE_API virtual char ReadChar();

                /// \brief Reads the specified number of bytes from the current stream into a byte array.
                /// \param iCount The number of bytes to read.
                /// \return A byte array containing data read from the underlying stream.
                DOTNETDUPE_API virtual Array<byte> ReadBytes(int iCount);

                /// \brief Reads the specified number of bytes from the stream into an array starting at a specific index.
                /// \param arrBuffer The buffer to read data into.
                /// \param iIndex The starting point in buffer.
                /// \param iCount The number of bytes to read.
                /// \return The number of bytes read into buffer.
                DOTNETDUPE_API virtual int Read(Array<byte>& arrBuffer, int iIndex, int iCount);

                /// \brief Reads a 2-byte signed integer from the current stream.
                /// \return A 2-byte signed integer read from the current stream.
                DOTNETDUPE_API virtual short ReadInt16();

                /// \brief Reads a 2-byte unsigned integer from the current stream.
                /// \return A 2-byte unsigned integer read from the current stream.
                DOTNETDUPE_API virtual unsigned short ReadUInt16();

                /// \brief Reads a 4-byte signed integer from the current stream.
                /// \return A 4-byte signed integer read from the current stream.
                DOTNETDUPE_API virtual int ReadInt32();

                /// \brief Reads a 4-byte unsigned integer from the current stream.
                /// \return A 4-byte unsigned integer read from the current stream.
                DOTNETDUPE_API virtual unsigned int ReadUInt32();

                /// \brief Reads an 8-byte signed integer from the current stream.
                /// \return An 8-byte signed integer read from the current stream.
                DOTNETDUPE_API virtual long long ReadInt64();

                /// \brief Reads an 8-byte unsigned integer from the current stream.
                /// \return An 8-byte unsigned integer read from the current stream.
                DOTNETDUPE_API virtual unsigned long long ReadUInt64();

                /// \brief Reads a 4-byte floating point value from the current stream.
                /// \return A 4-byte floating point value read from the current stream.
                DOTNETDUPE_API virtual float ReadSingle();

                /// \brief Reads an 8-byte floating point value from the current stream.
                /// \return An 8-byte floating point value read from the current stream.
                DOTNETDUPE_API virtual double ReadDouble();

                /// \brief Reads a string of specified byte length from the current stream.
                /// \param iLength The number of bytes to read.
                /// \return The string being read.
                DOTNETDUPE_API virtual String ReadString(int iLength);

            private:
                Stream* m_pStream;
                SmartPointer<Stream> m_spOwnedStream;
                bool m_bLeaveOpen;
                bool m_bIsLittleEndian;
                bool m_bDisposed;

                void EnsureNotDisposed() const;
                void FillBuffer(char* pBuffer, int iCount);
            };

            using BinaryReaderPtr = SmartPointer<BinaryReader>;

        }
    }
}
