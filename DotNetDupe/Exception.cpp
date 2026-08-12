#include "pch.h"
#include "System/Exception.h"
#include "System/SystemException.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        Exception::Exception() : std::exception("Exception of type 'DotNetDupe::System::Exception' was thrown.") {}
        
        Exception::Exception(const String& sMessage) : std::exception(sMessage.GetRawString() ? sMessage.GetRawString() : "") {
        }
        
        Exception::Exception(const String& sMessage, const Exception& innerException) : std::exception(sMessage.GetRawString() ? sMessage.GetRawString() : ""), m_innerException(new Exception(innerException)) {}
        
        Exception::Exception(const Exception& other) : std::exception(other), m_innerException(other.m_innerException ? new Exception(*other.m_innerException) : nullptr) {}
        
        Exception& Exception::operator=(const Exception& other) {
            if (this != &other) {
                std::exception::operator=(other);
                delete m_innerException;
                m_innerException = other.m_innerException ? new Exception(*other.m_innerException) : nullptr;
            }
            return *this;
        }
        
        Exception::~Exception() { delete m_innerException; }

        SystemException::SystemException() : Exception() {}
        SystemException::SystemException(const char* pchMessage) : Exception(String(pchMessage)) {}
        SystemException::SystemException(const String& message) : Exception(message) {}
        SystemException::SystemException(const String& message, const Exception& innerException) : Exception(message, innerException) {}
    }
}
