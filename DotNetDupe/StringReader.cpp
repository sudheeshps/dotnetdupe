#include "pch.h"
#include "System/IO/StringReader.h"
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            StringReader::StringReader(const String& sSource)
                : m_sSource(sSource), m_iPos(0), m_nLength(sSource.GetLength()) {
            }

            void StringReader::Close() {
                m_iPos = m_nLength;
            }

            void StringReader::Dispose() {
                m_iPos = m_nLength;
            }

            int StringReader::Peek() {
                if (m_iPos >= m_nLength) return -1;
                return (int)(unsigned char)m_sSource[m_iPos];
            }

            int StringReader::Read() {
                if (m_iPos >= m_nLength) return -1;
                return (int)(unsigned char)m_sSource[m_iPos++];
            }

            int StringReader::Read(char* pBuffer, int iIndex, int nCount) {
                if (pBuffer == nullptr) return 0;
                int nRead = (std::min)(nCount, m_nLength - m_iPos);
                for (int i = 0; i < nRead; i++) {
                    pBuffer[iIndex + i] = (char)m_sSource[m_iPos + i];
                }
                m_iPos += nRead;
                return nRead;
            }

            String StringReader::ReadLine() {
                int iIdx = m_iPos;
                while (iIdx < m_nLength) {
                    char chCurrent = (char)m_sSource[iIdx];
                    if (chCurrent == '\r' || chCurrent == '\n') {
                        String sResult = m_sSource.Substring(m_iPos, iIdx - m_iPos);
                        m_iPos = iIdx + 1;
                        if (chCurrent == '\r' && m_iPos < m_nLength && (char)m_sSource[m_iPos] == '\n') {
                            m_iPos++;
                        }
                        return sResult;
                    }
                    iIdx++;
                }
                if (iIdx > m_iPos) {
                    String sResult = m_sSource.Substring(m_iPos, iIdx - m_iPos);
                    m_iPos = iIdx;
                    return sResult;
                }
                return String("");
            }

            String StringReader::ReadToEnd() {
                if (m_iPos >= m_nLength) return String("");
                String sResult = m_sSource.Substring(m_iPos, m_nLength - m_iPos);
                m_iPos = m_nLength;
                return sResult;
            }
        }
    }
}
