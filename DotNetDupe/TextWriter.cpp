#include "pch.h"
#include "System/IO/TextWriter.h"
#include "System/Convert.h"
#include <tchar.h>

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

            void TextWriter::Write(bool value) {
                Write(Convert::ToString(value));
            }

            void TextWriter::Write(wchar_t value) {
                // Base class does not know how to write, should be overridden
            }

            void TextWriter::Write(const wchar_t* value) {
                if (value == nullptr) return;
                Write(String(value));
            }

            void TextWriter::Write(const String& value) {
                for (int i = 0; i < value.GetLength(); i++) {
                    Write((wchar_t)value[i]);
                }
            }

            void TextWriter::Write(int value) {
                Write(Convert::ToString(value));
            }

            void TextWriter::Write(long long value) {
                Write(Convert::ToString(value));
            }

            void TextWriter::Write(float value) {
                Write(Convert::ToString(value));
            }

            void TextWriter::Write(double value) {
                Write(Convert::ToString(value));
            }

            void TextWriter::WriteLine() {
                Write(GetNewLine());
            }

            void TextWriter::WriteLine(bool value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(wchar_t value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(const wchar_t* value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(const String& value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(int value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(long long value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(float value) {
                Write(value);
                WriteLine();
            }

            void TextWriter::WriteLine(double value) {
                Write(value);
                WriteLine();
            }

            const String& TextWriter::GetNewLine() {
                static String newLine(_T("\r\n"));
                return newLine;
            }
        }
    }
}
