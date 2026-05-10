#include "pch.h"
#include "System/IO/TextReader.h"
#include "System/Text/StringBuilder.h"

using namespace DotNetDupe::System::Text;

namespace DotNetDupe {
    namespace System {
        namespace IO {
            void TextReader::Close() {
                Dispose();
            }

            void TextReader::Dispose() {
                // Base implementation does nothing
            }

            int TextReader::Peek() {
                return -1;
            }

            int TextReader::Read() {
                return -1;
            }

            int TextReader::Read(wchar_t* buffer, int index, int count) {
                if (buffer == nullptr) return 0;
                int n = 0;
                while (n < count) {
                    int ch = Read();
                    if (ch == -1) break;
                    buffer[index + n] = (wchar_t)ch;
                    n++;
                }
                return n;
            }

            String TextReader::ReadLine() {
                StringBuilder sb;
                while (true) {
                    int ch = Read();
                    if (ch == -1) {
                        if (sb.GetLength() > 0) return sb.ToString();
                        return String();
                    }
                    if (ch == L'\r' || ch == L'\n') {
                        if (ch == L'\r' && Peek() == L'\n') Read();
                        return sb.ToString();
                    }
                    sb.Append((wchar_t)ch);
                }
            }

            String TextReader::ReadToEnd() {
                StringBuilder sb;
                int ch;
                while ((ch = Read()) != -1) {
                    sb.Append((wchar_t)ch);
                }
                return sb.ToString();
            }
        }
    }
}
