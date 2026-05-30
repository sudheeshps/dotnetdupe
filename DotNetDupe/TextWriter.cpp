#include "pch.h"
#include "System/IO/TextWriter.h"
#include "System/Convert.h"
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            void TextWriter::Close() {
                Dispose();
            }

            void TextWriter::Dispose() {
                // Base implementation does nothing
            }

            void TextWriter::Flush() {
                // Base implementation does nothing
            }

            void TextWriter::Write(bool bValue) {
                Write(Convert::ToString(bValue));
            }

            void TextWriter::Write(char chValue) {
                // Base class does not know how to write, should be overridden
            }

            void TextWriter::Write(const char* pValue) {
                if (pValue == nullptr) return;
                Write(String(pValue));
            }

            void TextWriter::Write(const String& sValue) {
                for (int iIdx = 0; iIdx < sValue.GetLength(); iIdx++) {
                    Write((char)sValue[iIdx]);
                }
            }

            void TextWriter::Write(int iValue) {
                Write(Convert::ToString(iValue));
            }

            void TextWriter::Write(long long llValue) {
                Write(Convert::ToString(llValue));
            }

            void TextWriter::Write(float fValue) {
                Write(Convert::ToString(fValue));
            }

            void TextWriter::Write(double dValue) {
                Write(Convert::ToString(dValue));
            }

            void TextWriter::WriteLine() {
                Write(GetNewLine());
            }

            void TextWriter::WriteLine(bool bValue) {
                Write(bValue);
                WriteLine();
            }

            void TextWriter::WriteLine(char chValue) {
                Write(chValue);
                WriteLine();
            }

            void TextWriter::WriteLine(const char* pValue) {
                Write(pValue);
                WriteLine();
            }

            void TextWriter::WriteLine(const String& sValue) {
                Write(sValue);
                WriteLine();
            }

            void TextWriter::WriteLine(int iValue) {
                Write(iValue);
                WriteLine();
            }

            void TextWriter::WriteLine(long long llValue) {
                Write(llValue);
                WriteLine();
            }

            void TextWriter::WriteLine(float fValue) {
                Write(fValue);
                WriteLine();
            }

            void TextWriter::WriteLine(double dValue) {
                Write(dValue);
                WriteLine();
            }

            const String& TextWriter::GetNewLine() {
                static String sNewLine("\r\n");
                return sNewLine;
            }
        }
    }
}
