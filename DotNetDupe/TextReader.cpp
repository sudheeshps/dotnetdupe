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

            int TextReader::Read(char* pBuffer, int iIndex, int nCount) {
                /// Guard: Check null buffer.
                if (pBuffer == nullptr) return 0;

                /// Read characters into buffer up to nCount.
                int iN = 0;
                while (iN < nCount) {
                    int iCh = Read();
                    if (iCh == -1) break;
                    pBuffer[iIndex + iN] = (char)iCh;
                    iN++;
                }
                return iN;
            }

            String TextReader::ReadLine() {
                StringBuilder sbOutput;

                /// Read character loop consuming up to CR/LF or EOF.
                while (true) {
                    int iCh = Read();
                    if (iCh == -1) {
                        if (sbOutput.GetLength() > 0) return sbOutput.ToString();
                        return String("");
                    }
                    if (iCh == '\r' || iCh == '\n') {
                        if (iCh == '\r' && Peek() == '\n') Read();
                        return sbOutput.ToString();
                    }
                    sbOutput.Append((char)iCh);
                }
            }

            String TextReader::ReadToEnd() {
                StringBuilder sbOutput;

                /// Consume all characters until EOF.
                int iCh;
                while ((iCh = Read()) != -1) {
                    sbOutput.Append((char)iCh);
                }
                return sbOutput.ToString();
            }
        }
    }
}
