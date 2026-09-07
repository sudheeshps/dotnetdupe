#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/MemoryStream.h"
#include "System/IO/EndOfStreamException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/InvalidOperationException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

namespace SystemIOTests {
    namespace BinaryReaderTests {

        TEST(BinaryReaderTests, Given_NullStream_When_Constructed_Then_ThrowsArgumentNullException) {
            // Given / When / Then
            EXPECT_THROW({
                BinaryReader reader(static_cast<Stream*>(nullptr));
            }, ArgumentNullException);

            EXPECT_THROW({
                BinaryReader reader(SmartPointer<Stream>(nullptr));
            }, ArgumentNullException);
        }

        TEST(BinaryReaderTests, Given_PrimitiveBytes_When_Read_Then_ValuesMatch) {
            // Given
            Array<char> arrData(16);
            arrData[0] = 1; // bool
            arrData[1] = static_cast<char>(0xFE); // byte
            arrData[2] = -5; // sbyte
            arrData[3] = 'Z'; // char
            // int16 0x0102 little-endian
            arrData[4] = 0x02;
            arrData[5] = 0x01;
            // int32 0x01020304 little-endian
            arrData[6] = 0x04;
            arrData[7] = 0x03;
            arrData[8] = 0x02;
            arrData[9] = 0x01;

            MemoryStream stream(arrData);
            BinaryReader reader(&stream, true, true);

            // When / Then
            EXPECT_TRUE(reader.ReadBoolean());
            EXPECT_EQ(0xFE, reader.ReadByte());
            EXPECT_EQ(-5, reader.ReadSByte());
            EXPECT_EQ('Z', reader.ReadChar());
            EXPECT_EQ(0x0102, reader.ReadInt16());
            EXPECT_EQ(0x01020304, reader.ReadInt32());
            EXPECT_TRUE(reader.IsLittleEndian());
        }

        TEST(BinaryReaderTests, Given_BigEndianData_When_ReadBigEndian_Then_SwapsBytesCorrectly) {
            // Given
            Array<char> arrData(6);
            // int16 0x0102 big-endian
            arrData[0] = 0x01;
            arrData[1] = 0x02;
            // uint32 0xAABBCCDD big-endian
            arrData[2] = static_cast<char>(0xAA);
            arrData[3] = static_cast<char>(0xBB);
            arrData[4] = static_cast<char>(0xCC);
            arrData[5] = static_cast<char>(0xDD);

            MemoryStream stream(arrData);
            BinaryReader reader(&stream, true, false); // Big-Endian

            // When / Then
            EXPECT_FALSE(reader.IsLittleEndian());
            EXPECT_EQ(0x0102, reader.ReadInt16());
            EXPECT_EQ(0xAABBCCDDu, reader.ReadUInt32());
        }

        TEST(BinaryReaderTests, Given_StringData_When_ReadString_Then_ReturnsString) {
            // Given
            Array<char> arrData(5);
            arrData[0] = 'H';
            arrData[1] = 'E';
            arrData[2] = 'L';
            arrData[3] = 'L';
            arrData[4] = 'O';

            MemoryStream stream(arrData);
            BinaryReader reader(&stream, true);

            // When
            String sVal = reader.ReadString(5);

            // Then
            EXPECT_STREQ("HELLO", sVal.GetRawString());
        }

        TEST(BinaryReaderTests, Given_EndOfStream_When_ReadAttempted_Then_ThrowsEndOfStreamException) {
            // Given
            Array<char> arrData(1);
            arrData[0] = 42;

            MemoryStream stream(arrData);
            BinaryReader reader(&stream, true);

            // When
            reader.ReadByte();

            // Then
            EXPECT_THROW({
                reader.ReadByte();
            }, EndOfStreamException);
        }

        TEST(BinaryReaderTests, Given_DisposedReader_When_ReadCalled_Then_ThrowsInvalidOperationException) {
            // Given
            Array<char> arrData(4);
            MemoryStream stream(arrData);
            BinaryReader reader(&stream, true);

            // When
            reader.Dispose();

            // Then
            EXPECT_THROW({
                reader.ReadByte();
            }, InvalidOperationException);
        }

    }
}
