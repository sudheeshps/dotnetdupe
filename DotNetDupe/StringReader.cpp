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

            static int FindNewlineIndex(const String& sSource, int startPos, int length) {
                for (int i = startPos; i < length; ++i) {
                    char c = (char)sSource[i];
                    if (c == '\r' || c == '\n') return i;
                }
                return -1;
            }

            String StringReader::ReadLine() {
                if (m_iPos >= m_nLength) return String("");
                int nlIdx = FindNewlineIndex(m_sSource, m_iPos, m_nLength);
                if (nlIdx != -1) {
                    String sResult = m_sSource.Substring(m_iPos, nlIdx - m_iPos);
                    char c = (char)m_sSource[nlIdx];
                    m_iPos = nlIdx + 1;
                    if (c == '\r' && m_iPos < m_nLength && (char)m_sSource[m_iPos] == '\n') m_iPos++;
                    return sResult;
                }
                String sResult = m_sSource.Substring(m_iPos, m_nLength - m_iPos);
                m_iPos = m_nLength;
                return sResult;
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
