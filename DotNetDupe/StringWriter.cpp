#include "pch.h"
#include "System/IO/StringWriter.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            StringWriter::StringWriter()
                : m_pSb(new Text::StringBuilder()), m_bIsOpen(true) {
            }

            StringWriter::StringWriter(Text::StringBuilder& sbOutput)
                : m_pSb(&sbOutput), m_bIsOpen(true) {
            }

            void StringWriter::Close() {
                m_bIsOpen = false;
            }

            void StringWriter::Dispose() {
                m_bIsOpen = false;
            }

            void StringWriter::Flush() {
                // Nothing to flush
            }

            Text::EncodingPtr StringWriter::GetEncoding() const {
                return Text::TextEncoding::UTF8();
            }

            String StringWriter::ToString() const {
                return m_pSb->ToString();
            }

            void StringWriter::Write(bool bValue) {
                TextWriter::Write(bValue);
            }

            void StringWriter::Write(char chValue) {
                if (!m_bIsOpen) return;
                m_pSb->Append(chValue);
            }

            void StringWriter::Write(const String& sValue) {
                if (!m_bIsOpen) return;
                m_pSb->Append(sValue);
            }

            void StringWriter::Write(const char* pValue) {
                if (!m_bIsOpen) return;
                m_pSb->Append(pValue);
            }

            void StringWriter::Write(int iValue) {
                TextWriter::Write(iValue);
            }

            void StringWriter::Write(long long llValue) {
                TextWriter::Write(llValue);
            }

            void StringWriter::Write(float fValue) {
                TextWriter::Write(fValue);
            }

            void StringWriter::Write(double dValue) {
                TextWriter::Write(dValue);
            }

            void StringWriter::WriteLine() {
                TextWriter::WriteLine();
            }

            void StringWriter::WriteLine(bool bValue) {
                TextWriter::WriteLine(bValue);
            }

            void StringWriter::WriteLine(char chValue) {
                TextWriter::WriteLine(chValue);
            }

            void StringWriter::WriteLine(const char* pValue) {
                TextWriter::WriteLine(pValue);
            }

            void StringWriter::WriteLine(const String& sValue) {
                TextWriter::WriteLine(sValue);
            }

            void StringWriter::WriteLine(int iValue) {
                TextWriter::WriteLine(iValue);
            }

            void StringWriter::WriteLine(long long llValue) {
                TextWriter::WriteLine(llValue);
            }

            void StringWriter::WriteLine(float fValue) {
                TextWriter::WriteLine(fValue);
            }

            void StringWriter::WriteLine(double dValue) {
                TextWriter::WriteLine(dValue);
            }
        }
    }
}
