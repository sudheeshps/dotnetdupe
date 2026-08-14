#include "pch.h"
#include <gtest/gtest.h>
#include "System/Exception.h"
#include "System/SystemException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/UnknownException.h"
#include "System/OutOfMemoryException.h"
#include "System/ComponentModel/Win32Exception.h"
#include "System/Security/SecurityException.h"
#include "System/Diagnostics/Process.h"
#include "System/IO/FileNotFoundException.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::ComponentModel;
using namespace DotNetDupe::System::Security;
using namespace DotNetDupe::System::Diagnostics;

TEST(ExceptionTests, GivenUnauthorizedAccessException_WhenDefaultConstructorCalled_ThenHasDefaultMessage) {
    UnauthorizedAccessException ex;
    EXPECT_TRUE(String(ex.What()).Contains("unauthorized"));
}

TEST(ExceptionTests, GivenUnauthorizedAccessException_WhenCustomMessageProvided_ThenPreservesMessage) {
    UnauthorizedAccessException ex("Access to the specified resource is denied.");
    EXPECT_STREQ(ex.What(), "Access to the specified resource is denied.");
}

TEST(ExceptionTests, GivenUnauthorizedAccessException_WhenInnerExceptionProvided_ThenPreservesMessage) {
    Exception inner("Underlying OS permission denied");
    UnauthorizedAccessException ex("Higher level failure", inner);
    EXPECT_STREQ(ex.What(), "Higher level failure");
}

TEST(ExceptionTests, GivenUnknownException_WhenDefaultConstructorCalled_ThenHasDefaultMessage) {
    UnknownException ex;
    EXPECT_TRUE(String(ex.What()).Contains("unknown") || String(ex.What()).Contains("unhandled"));
}

TEST(ExceptionTests, GivenUnknownException_WhenCustomMessageProvided_ThenPreservesMessage) {
    UnknownException ex("An unexpected ellipsis exception occurred.");
    EXPECT_STREQ(ex.What(), "An unexpected ellipsis exception occurred.");
}

TEST(ExceptionTests, GivenOutOfMemoryException_WhenDefaultConstructorCalled_ThenHasDefaultMessage) {
    OutOfMemoryException ex;
    EXPECT_TRUE(String(ex.What()).Contains("memory"));
}

TEST(ExceptionTests, GivenOutOfMemoryException_WhenCustomMessageProvided_ThenPreservesMessage) {
    OutOfMemoryException ex("Failed to allocate 100MB buffer.");
    EXPECT_STREQ(ex.What(), "Failed to allocate 100MB buffer.");
}

TEST(ExceptionTests, GivenSecurityException_WhenDefaultConstructorCalled_ThenHasDefaultMessage) {
    SecurityException ex;
    EXPECT_TRUE(String(ex.What()).Contains("security"));
}

TEST(ExceptionTests, GivenSecurityException_WhenCustomMessageProvided_ThenPreservesMessage) {
    SecurityException ex("Token signature validation failed.");
    EXPECT_STREQ(ex.What(), "Token signature validation failed.");
}

TEST(ExceptionTests, GivenWin32Exception_WhenErrorCodeProvided_ThenStoresErrorCodeAndFormatsMessage) {
    Win32Exception ex(5); // 5 = ERROR_ACCESS_DENIED / EACCES
    EXPECT_EQ(ex.GetNativeErrorCode(), 5);
    EXPECT_FALSE(String(ex.What()).IsEmpty());
}

TEST(ExceptionTests, GivenWin32Exception_WhenCustomMessageAndErrorCodeProvided_ThenStoresBoth) {
    Win32Exception ex(1314, "Privilege not held.");
    EXPECT_EQ(ex.GetNativeErrorCode(), 1314);
    EXPECT_STREQ(ex.What(), "Privilege not held.");
}

TEST(ExceptionTests, GivenWin32Exception_WhenCustomMessageOnlyProvided_ThenStoresMessage) {
    Win32Exception ex("Custom win32 error message");
    EXPECT_STREQ(ex.What(), "Custom win32 error message");
}

TEST(ExceptionTests, GivenProcessInstance_WhenNonExistentFileStarted_ThrowsFileNotFoundException) {
    Process proc;
    proc.SetStartInfo(ProcessStartInfo("C:\\NonExistentPath\\NonExistentBinary12345.exe"));
    EXPECT_THROW(proc.Start(), DotNetDupe::System::IO::FileNotFoundException);
}
