#include "pch.h"
#include "System/Exception.h"
#include "System/SystemException.h"
#include "System/String.h"
#include "System/UnauthorizedAccessException.h"
#include "System/UnknownException.h"
#include "System/OutOfMemoryException.h"
#include "System/Security/SecurityException.h"
#include "System/IOException.h"

#include <cstring>

namespace DotNetDupe {
    namespace System {
        Exception::Exception() : std::runtime_error("Exception of type 'DotNetDupe::System::Exception' was thrown.") {
        }
        
        Exception::Exception(const String& sMessage) : std::runtime_error(sMessage.GetRawString() ? sMessage.GetRawString() : "") {
        }
        
        Exception::Exception(const String& sMessage, const Exception& innerException) : std::runtime_error(sMessage.GetRawString() ? sMessage.GetRawString() : ""), m_pInnerException(new Exception(innerException)) {
        }
        
        Exception::Exception(const Exception& other) : std::runtime_error(other), m_pInnerException(other.m_pInnerException ? new Exception(*other.m_pInnerException) : nullptr) {
        }
        
        Exception& Exception::operator=(const Exception& other) {
            if (this != &other) {
                std::runtime_error::operator=(other);
                delete m_pInnerException;
                m_pInnerException = other.m_pInnerException ? new Exception(*other.m_pInnerException) : nullptr;
            }
            return *this;
        }
        
        Exception::~Exception() { 
            delete m_pInnerException; 
        }

        SystemException::SystemException() : Exception() {}
        SystemException::SystemException(const String& message) : Exception(message) {}
        SystemException::SystemException(const String& message, const Exception& innerException) : Exception(message, innerException) {}

        UnauthorizedAccessException::UnauthorizedAccessException() : SystemException("Attempted to perform an unauthorized operation.") {}
        UnknownException::UnknownException() : SystemException("An unknown or unhandled non-DotNetDupe exception occurred.") {}
        OutOfMemoryException::OutOfMemoryException() : SystemException("Insufficient memory to continue the execution of the program.") {}
        Security::SecurityException::SecurityException() : SystemException("A security error has been detected.") {}
        IO::IOException::IOException() : SystemException("I/O error occurred.") {}
    }
}
