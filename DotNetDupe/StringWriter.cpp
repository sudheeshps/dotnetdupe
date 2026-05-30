#include "pch.h"
#include "System/IO/StringWriter.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            StringWriter::StringWriter()
                : _sb(new Text::StringBuilder()), _isOpen(true) {
            }

            StringWriter::StringWriter(Text::StringBuilder& sb)
                : _sb(&sb), _isOpen(true) {
            }

            void StringWriter::Close() {
                _isOpen = false;
            }

            void StringWriter::Dispose() {
                _isOpen = false;
            }

            void StringWriter::Flush() {
                // Nothing to flush
            }

            std::shared_ptr<Text::Encoding> StringWriter::GetEncoding() const {
                return Text::TextEncoding::UTF8();
            }

            String StringWriter::ToString() const {
                return _sb->ToString();
            }

            void StringWriter::Write(bool value) {
                TextWriter::Write(value);
            }

            void StringWriter::Write(char value) {
                if (!_isOpen) return;
                _sb->Append(value);
            }

            void StringWriter::Write(const String& value) {
                if (!_isOpen) return;
                _sb->Append(value);
            }

            void StringWriter::Write(const char* value) {
                if (!_isOpen) return;
                _sb->Append(value);
            }

            void StringWriter::Write(int value) {
                TextWriter::Write(value);
            }

            void StringWriter::Write(long long value) {
                TextWriter::Write(value);
            }

            void StringWriter::Write(float value) {
                TextWriter::Write(value);
            }

            void StringWriter::Write(double value) {
                TextWriter::Write(value);
            }

            void StringWriter::WriteLine() {
                TextWriter::WriteLine();
            }

            void StringWriter::WriteLine(bool value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(char value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(const char* value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(const String& value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(int value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(long long value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(float value) {
                TextWriter::WriteLine(value);
            }

            void StringWriter::WriteLine(double value) {
                TextWriter::WriteLine(value);
            }
        }
    }
}
