#include "pch.h"
#include "System/ComponentModel/Win32Exception.h"
#include "System/String.h"

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#else
#include <cstring>
#include <cerrno>
#endif

namespace DotNetDupe {
    namespace System {
        namespace ComponentModel {

            static String FormatErrorMessage(int nErrorCode) {
#if defined(_WIN32)
                LPWSTR lpMsgBuf = nullptr;
                DWORD dwLen = ::FormatMessageW(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr,
                    static_cast<DWORD>(nErrorCode),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    reinterpret_cast<LPWSTR>(&lpMsgBuf),
                    0,
                    nullptr
                );

                if (dwLen > 0 && lpMsgBuf != nullptr) {
                    while (dwLen > 0 && (lpMsgBuf[dwLen - 1] == L'\r' || lpMsgBuf[dwLen - 1] == L'\n')) {
                        lpMsgBuf[--dwLen] = L'\0';
                    }
                    std::string sUtf8 = StringConvertInternal::WCharToUtf8(lpMsgBuf);
                    ::LocalFree(lpMsgBuf);
                    return String(sUtf8.c_str());
                }
                return String("Unknown system error");
#else
                const char* pchErr = std::strerror(nErrorCode);
                return pchErr ? String(pchErr) : String("Unknown system error");
#endif
            }

            Win32Exception::Win32Exception()
#if defined(_WIN32)
                : Win32Exception(static_cast<int>(::GetLastError()))
#else
                : Win32Exception(errno)
#endif
            {
            }

            Win32Exception::Win32Exception(int nNativeErrorCode)
                : SystemException(FormatErrorMessage(nNativeErrorCode)), m_nNativeErrorCode(nNativeErrorCode)
            {
            }

            Win32Exception::Win32Exception(int nNativeErrorCode, const String& sMessage)
                : SystemException(sMessage), m_nNativeErrorCode(nNativeErrorCode)
            {
            }

            Win32Exception::Win32Exception(const String& sMessage)
#if defined(_WIN32)
                : SystemException(sMessage), m_nNativeErrorCode(static_cast<int>(::GetLastError()))
#else
                : SystemException(sMessage), m_nNativeErrorCode(errno)
#endif
            {
            }

        }
    }
}
