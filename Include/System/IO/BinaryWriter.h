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

            class BinaryWriter : public Object, public IDisposable {
            public:
                DOTNETDUPE_API explicit BinaryWriter(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);
                DOTNETDUPE_API explicit BinaryWriter(SmartPointer<Stream> spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);
                DOTNETDUPE_API virtual ~BinaryWriter();

                DOTNETDUPE_API virtual void Close();
                DOTNETDUPE_API void Dispose() override;
                DOTNETDUPE_API virtual void Flush();

                DOTNETDUPE_API Stream* GetBaseStream() const;
                DOTNETDUPE_API bool IsLittleEndian() const;
                DOTNETDUPE_API void SetLittleEndian(bool bIsLittleEndian);
                DOTNETDUPE_API virtual long Seek(long lOffset, int iOrigin);

                DOTNETDUPE_API virtual void Write(bool bValue);
                DOTNETDUPE_API virtual void Write(byte bValue);
                DOTNETDUPE_API virtual void Write(char chValue);
                DOTNETDUPE_API virtual void Write(signed char chValue);
                DOTNETDUPE_API virtual void Write(const Array<byte>& arrBuffer);
                DOTNETDUPE_API virtual void Write(const Array<byte>& arrBuffer, int iIndex, int iCount);
                DOTNETDUPE_API virtual void Write(short iValue);
                DOTNETDUPE_API virtual void Write(unsigned short uValue);
                DOTNETDUPE_API virtual void Write(int iValue);
                DOTNETDUPE_API virtual void Write(unsigned int uValue);
                DOTNETDUPE_API virtual void Write(long long llValue);
                DOTNETDUPE_API virtual void Write(unsigned long long ullValue);
                DOTNETDUPE_API virtual void Write(float fValue);
                DOTNETDUPE_API virtual void Write(double dValue);
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
