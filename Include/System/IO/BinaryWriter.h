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

            /// \class BinaryWriter
            /// \brief Writes primitive types in binary to a stream and supports writing strings in a specific encoding.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.55 and IEEE 754 specifications.
            /// Supports configurable endianness (little-endian by default) and wraps underlying Stream
            /// objects with optional ownership semantics via SmartPointer.
            class BinaryWriter : public Object, public IDisposable {
            public:
                /// \brief Initializes a new instance of the BinaryWriter class based on the specified stream and endianness.
                /// \param pStream The output stream.
                /// \param bLeaveOpen true to leave the stream open after the BinaryWriter object is disposed; otherwise, false.
                /// \param bIsLittleEndian true to write multi-byte values in little-endian format; false for big-endian.
                /// \throws ArgumentNullException If pStream is null.
                DOTNETDUPE_API explicit BinaryWriter(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);

                /// \brief Initializes a new instance of the BinaryWriter class taking ownership of the specified stream.
                /// \param spStream The smart-pointer managed output stream.
                /// \param bLeaveOpen true to leave the stream open after disposal; otherwise, false.
                /// \param bIsLittleEndian true for little-endian; false for big-endian.
                /// \throws ArgumentNullException If spStream is null.
                DOTNETDUPE_API explicit BinaryWriter(SmartPointer<Stream> spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);

                /// \brief Destructor releasing writer resources.
                DOTNETDUPE_API virtual ~BinaryWriter();

                /// \brief Closes the current BinaryWriter and the underlying stream if configured.
                DOTNETDUPE_API virtual void Close();

                /// \brief Releases all unmanaged resources used by the BinaryWriter.
                DOTNETDUPE_API void Dispose() override;

                /// \brief Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.
                DOTNETDUPE_API virtual void Flush();

                /// \brief Exposes access to the underlying stream.
                /// \return The underlying stream.
                DOTNETDUPE_API Stream* GetBaseStream() const;

                /// \brief Gets whether values are encoded in little-endian order.
                /// \return true if little-endian; false if big-endian.
                DOTNETDUPE_API bool IsLittleEndian() const;

                /// \brief Sets the endianness for binary encoding.
                /// \param bIsLittleEndian true for little-endian; false for big-endian.
                DOTNETDUPE_API void SetLittleEndian(bool bIsLittleEndian);

                /// \brief Sets the position within the current stream.
                /// \param lOffset A byte offset relative to origin.
                /// \param iOrigin A field of SeekOrigin indicating the reference point from which the new position is to be obtained.
                /// \return The position with the current stream.
                DOTNETDUPE_API virtual long Seek(long lOffset, int iOrigin);

                /// \brief Writes a one-byte Boolean value to the current stream, with 0 representing false and 1 representing true.
                /// \param bValue The Boolean value to write (0 or 1).
                DOTNETDUPE_API virtual void Write(bool bValue);

                /// \brief Writes an unsigned byte to the current stream and advances the stream position by one byte.
                /// \param bValue The unsigned byte to write.
                DOTNETDUPE_API virtual void Write(byte bValue);

                /// \brief Writes a character to the current stream and advances the stream position.
                /// \param chValue The character to write.
                DOTNETDUPE_API virtual void Write(char chValue);

                /// \brief Writes a signed byte to the current stream and advances the stream position by one byte.
                /// \param chValue The signed byte to write.
                DOTNETDUPE_API virtual void Write(signed char chValue);

                /// \brief Writes a byte array to the underlying stream.
                /// \param arrBuffer A byte array containing data to write.
                DOTNETDUPE_API virtual void Write(const Array<byte>& arrBuffer);

                /// \brief Writes a region of a byte array to the underlying stream.
                /// \param arrBuffer A byte array containing data to write.
                /// \param iIndex The starting point in buffer at which to begin writing.
                /// \param iCount The number of bytes to write.
                DOTNETDUPE_API virtual void Write(const Array<byte>& arrBuffer, int iIndex, int iCount);

                /// \brief Writes a two-byte signed integer to the current stream and advances the stream position by two bytes.
                /// \param iValue The two-byte signed integer to write.
                DOTNETDUPE_API virtual void Write(short iValue);

                /// \brief Writes a two-byte unsigned integer to the current stream and advances the stream position by two bytes.
                /// \param uValue The two-byte unsigned integer to write.
                DOTNETDUPE_API virtual void Write(unsigned short uValue);

                /// \brief Writes a four-byte signed integer to the current stream and advances the stream position by four bytes.
                /// \param iValue The four-byte signed integer to write.
                DOTNETDUPE_API virtual void Write(int iValue);

                /// \brief Writes a four-byte unsigned integer to the current stream and advances the stream position by four bytes.
                /// \param uValue The four-byte unsigned integer to write.
                DOTNETDUPE_API virtual void Write(unsigned int uValue);

                /// \brief Writes an eight-byte signed integer to the current stream and advances the stream position by eight bytes.
                /// \param llValue The eight-byte signed integer to write.
                DOTNETDUPE_API virtual void Write(long long llValue);

                /// \brief Writes an eight-byte unsigned integer to the current stream and advances the stream position by eight bytes.
                /// \param ullValue The eight-byte unsigned integer to write.
                DOTNETDUPE_API virtual void Write(unsigned long long ullValue);

                /// \brief Writes a four-byte floating-point value to the current stream and advances the stream position by four bytes.
                /// \param fValue The four-byte floating-point value to write.
                DOTNETDUPE_API virtual void Write(float fValue);

                /// \brief Writes an eight-byte floating-point value to the current stream and advances the stream position by eight bytes.
                /// \param dValue The eight-byte floating-point value to write.
                DOTNETDUPE_API virtual void Write(double dValue);

                /// \brief Writes a string to this stream.
                /// \param sValue The value to write.
                DOTNETDUPE_API virtual void Write(const String& sValue);

            private:
                Stream* m_pStream;
                SmartPointer<Stream> m_spOwnedStream;
                bool m_bLeaveOpen;
                bool m_bIsLittleEndian;
                bool m_bDisposed;

                void EnsureNotDisposed() const;
                void WriteInternal(const char* pBuffer, int iCount);
            };

            using BinaryWriterPtr = SmartPointer<BinaryWriter>;

        }
    }
}
