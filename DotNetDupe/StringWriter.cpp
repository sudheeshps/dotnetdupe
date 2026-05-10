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

            void StringWriter::Flush() {
                // Nothing to flush
            }

            std::shared_ptr<Text::Encoding> StringWriter::GetEncoding() const {
                return Text::TextEncoding::UTF8();
            }

            String StringWriter::ToString() const {
                return _sb->ToString();
            }

            void StringWriter::Write(wchar_t value) {
                if (!_isOpen) return;
                _sb->Append(value);
            }

            void StringWriter::Write(const String& value) {
                if (!_isOpen) return;
                _sb->Append(value);
            }

            void StringWriter::Write(const wchar_t* value) {
                if (!_isOpen) return;
                _sb->Append(value);
            }
        }
    }
}
