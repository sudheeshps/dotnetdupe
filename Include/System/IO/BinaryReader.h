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

            class BinaryReader : public Object, public IDisposable {
            public:
                DOTNETDUPE_API explicit BinaryReader(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);
                DOTNETDUPE_API explicit BinaryReader(SmartPointer<Stream> spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true);
                DOTNETDUPE_API virtual ~BinaryReader();

                DOTNETDUPE_API virtual void Close();
                DOTNETDUPE_API void Dispose() override;

                DOTNETDUPE_API Stream* GetBaseStream() const;
                DOTNETDUPE_API bool IsLittleEndian() const;
                DOTNETDUPE_API void SetLittleEndian(bool bIsLittleEndian);

                DOTNETDUPE_API virtual bool ReadBoolean();
                DOTNETDUPE_API virtual byte ReadByte();
                DOTNETDUPE_API virtual signed char ReadSByte();
                DOTNETDUPE_API virtual char ReadChar();
                DOTNETDUPE_API virtual Array<byte> ReadBytes(int iCount);
                DOTNETDUPE_API virtual int Read(Array<byte>& arrBuffer, int iIndex, int iCount);
                DOTNETDUPE_API virtual short ReadInt16();
                DOTNETDUPE_API virtual unsigned short ReadUInt16();
                DOTNETDUPE_API virtual int ReadInt32();
                DOTNETDUPE_API virtual unsigned int ReadUInt32();
                DOTNETDUPE_API virtual long long ReadInt64();
                DOTNETDUPE_API virtual unsigned long long ReadUInt64();
                DOTNETDUPE_API virtual float ReadSingle();
                DOTNETDUPE_API virtual double ReadDouble();
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
