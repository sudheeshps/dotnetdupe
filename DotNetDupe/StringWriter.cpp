#include "pch.h"
#include "System/IO/StringWriter.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            StringWriter::StringWriter()
                : m_pSb(new Text::StringBuilder()), m_bIsOpen(true) {
                /// Step: Construct new StringBuilder instance.
            }

            StringWriter::StringWriter(Text::StringBuilder& sbOutput)
                : m_pSb(&sbOutput), m_bIsOpen(true) {
                /// Step: Bind to provided StringBuilder instance.
            }

            void StringWriter::Close() {
                /// Step: Close the string writer.
                m_bIsOpen = false;
            }

            void StringWriter::Dispose() {
                /// Step: Mark string writer as closed and disposed.
                m_bIsOpen = false;
            }

            void StringWriter::Flush() {
                /// Note: In-memory string writer requires no flush operations.
            }

            Text::EncodingPtr StringWriter::GetEncoding() const {
                /// Return: UTF-8 encoding pointer.
                return Text::TextEncoding::UTF8();
            }

            String StringWriter::ToString() const {
                /// Return: Serialized string from the underlying builder.
                return m_pSb->ToString();
            }

            void StringWriter::Write(bool bValue) {
                /// Forward: Delegate boolean serialization to base TextWriter.
                TextWriter::Write(bValue);
            }

            void StringWriter::Write(char chValue) {
                /// Guard: Check if writer is open.
                if (!m_bIsOpen) return;
                /// Step: Append character to underlying builder.
                m_pSb->Append(chValue);
            }

            void StringWriter::Write(const String& sValue) {
                /// Guard: Check if writer is open.
                if (!m_bIsOpen) return;
                /// Step: Append string to underlying builder.
                m_pSb->Append(sValue);
            }

            void StringWriter::Write(const char* pValue) {
                /// Guard: Check if writer is open.
                if (!m_bIsOpen) return;
                /// Step: Append C-string to underlying builder.
                m_pSb->Append(pValue);
            }

            void StringWriter::Write(int iValue) {
                /// Forward: Delegate integer serialization to base TextWriter.
                TextWriter::Write(iValue);
            }

            void StringWriter::Write(long long llValue) {
                /// Forward: Delegate 64-bit integer serialization to base TextWriter.
                TextWriter::Write(llValue);
            }

            void StringWriter::Write(float fValue) {
                /// Forward: Delegate float serialization to base TextWriter.
                TextWriter::Write(fValue);
            }

            void StringWriter::Write(double dValue) {
                /// Forward: Delegate double serialization to base TextWriter.
                TextWriter::Write(dValue);
            }

            void StringWriter::WriteLine() {
                /// Forward: Write newline via base TextWriter.
                TextWriter::WriteLine();
            }

            void StringWriter::WriteLine(bool bValue) {
                /// Forward: Write boolean line via base TextWriter.
                TextWriter::WriteLine(bValue);
            }

            void StringWriter::WriteLine(char chValue) {
                /// Forward: Write character line via base TextWriter.
                TextWriter::WriteLine(chValue);
            }

            void StringWriter::WriteLine(const char* pValue) {
                /// Forward: Write C-string line via base TextWriter.
                TextWriter::WriteLine(pValue);
            }

            void StringWriter::WriteLine(const String& sValue) {
                /// Forward: Write string line via base TextWriter.
                TextWriter::WriteLine(sValue);
            }

            void StringWriter::WriteLine(int iValue) {
                /// Forward: Write integer line via base TextWriter.
                TextWriter::WriteLine(iValue);
            }

            void StringWriter::WriteLine(long long llValue) {
                /// Forward: Write 64-bit integer line via base TextWriter.
                TextWriter::WriteLine(llValue);
            }

            void StringWriter::WriteLine(float fValue) {
                /// Forward: Write float line via base TextWriter.
                TextWriter::WriteLine(fValue);
            }

            void StringWriter::WriteLine(double dValue) {
                /// Forward: Write double line via base TextWriter.
                TextWriter::WriteLine(dValue);
            }
        }
    }
}
