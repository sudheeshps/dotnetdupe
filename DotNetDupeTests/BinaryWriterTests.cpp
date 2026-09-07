#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/BinaryWriter.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/MemoryStream.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/InvalidOperationException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

namespace SystemIOTests {
    namespace BinaryWriterTests {

        TEST(BinaryWriterTests, Given_NullStream_When_Constructed_Then_ThrowsArgumentNullException) {
            // Given / When / Then
            EXPECT_THROW({
                BinaryWriter writer(static_cast<Stream*>(nullptr));
            }, ArgumentNullException);

            EXPECT_THROW({
                BinaryWriter writer(SmartPointer<Stream>(nullptr));
            }, ArgumentNullException);
        }

        TEST(BinaryWriterTests, Given_PrimitivesWritten_When_ReadBack_Then_ValuesMatch) {
            // Given
            MemoryStream stream;
            BinaryWriter writer(&stream, true, true);

            // When
            writer.Write(true);
            writer.Write(static_cast<byte>(0xAB));
            writer.Write(static_cast<short>(1234));
            writer.Write(567890);
            writer.Write(3.14159f);
            writer.Write(String("DICOM"));
            writer.Flush();

            // Then
            stream.SetPosition(0);
            BinaryReader reader(&stream, true, true);
            EXPECT_TRUE(reader.ReadBoolean());
            EXPECT_EQ(0xAB, reader.ReadByte());
            EXPECT_EQ(1234, reader.ReadInt16());
            EXPECT_EQ(567890, reader.ReadInt32());
            EXPECT_FLOAT_EQ(3.14159f, reader.ReadSingle());
            EXPECT_STREQ("DICOM", reader.ReadString(5).GetRawString());
        }

        TEST(BinaryWriterTests, Given_BigEndianWriter_When_ReadBigEndian_Then_ValuesMatch) {
            // Given
            MemoryStream stream;
            BinaryWriter writer(&stream, true, false); // Big-Endian

            // When
            writer.Write(static_cast<unsigned short>(0x1234));
            writer.Write(0xAABBCCDDu);
            writer.Flush();

            // Then
            stream.SetPosition(0);
            BinaryReader reader(&stream, true, false); // Big-Endian
            EXPECT_EQ(0x1234, reader.ReadUInt16());
            EXPECT_EQ(0xAABBCCDDu, reader.ReadUInt32());
        }

        TEST(BinaryWriterTests, Given_ArraySlice_When_Written_Then_ExactBytesWritten) {
            // Given
            MemoryStream stream;
            BinaryWriter writer(&stream, true);

            Array<byte> arrBytes(4);
            arrBytes[0] = 10;
            arrBytes[1] = 20;
            arrBytes[2] = 30;
            arrBytes[3] = 40;

            // When
            writer.Write(arrBytes, 1, 2); // 20, 30
            writer.Flush();

            // Then
            stream.SetPosition(0);
            BinaryReader reader(&stream, true);
            EXPECT_EQ(20, reader.ReadByte());
            EXPECT_EQ(30, reader.ReadByte());
        }

        TEST(BinaryWriterTests, Given_InvalidBounds_When_WriteInvoked_Then_ThrowsArgumentOutOfRangeException) {
            // Given
            MemoryStream stream;
            BinaryWriter writer(&stream, true);
            Array<byte> arrBytes(2);

            // When / Then
            EXPECT_THROW({
                writer.Write(arrBytes, -1, 1);
            }, ArgumentOutOfRangeException);

            EXPECT_THROW({
                writer.Write(arrBytes, 1, 5);
            }, ArgumentOutOfRangeException);
        }

        TEST(BinaryWriterTests, Given_DisposedWriter_When_WriteInvoked_Then_ThrowsInvalidOperationException) {
            // Given
            MemoryStream stream;
            BinaryWriter writer(&stream, true);

            // When
            writer.Dispose();

            // Then
            EXPECT_THROW({
                writer.Write(10);
            }, InvalidOperationException);
        }

    }
}
