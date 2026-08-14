#include "pch.h"
#include <gtest/gtest.h>
#include "System/Exception.h"
#include "System/SystemException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/UnknownException.h"
#include "System/OutOfMemoryException.h"
#include "System/ComponentModel/Win32Exception.h"
#include "System/Security/SecurityException.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/TimeoutException.h"
#include "System/Diagnostics/Process.h"
#include "System/Diagnostics/EventLog.h"
#include "System/Diagnostics/EtwLogReader.h"
#include "System/Security/Principal/UserPrincipal.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/IO/FileNotFoundException.h"
#include "System/IO/DirectoryNotFoundException.h"
#include "System/IO/Directory.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/Text/Json/JsonException.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::ComponentModel;
using namespace DotNetDupe::System::Security;
using namespace DotNetDupe::System::Security::Principal;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Text::Json;

#if defined(_WIN32)
#include <windows.h>
static bool IsElevatedProcess() {
    BOOL bElevated = FALSE;
    HANDLE hToken = NULL;
    if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize = sizeof(TOKEN_ELEVATION);
        if (::GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            bElevated = elevation.TokenIsElevated != 0;
        }
        ::CloseHandle(hToken);
    }
    return bElevated != FALSE;
}
#else
static bool IsElevatedProcess() {
    return geteuid() == 0;
}
#endif

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
    EXPECT_THROW(proc.Start(), FileNotFoundException);
}

TEST(ExceptionTests, GivenDirectory_WhenNonExistentDirectoryDeleted_ThrowsIOException) {
    EXPECT_THROW(Directory::Delete("C:\\NonExistentPath_12345_XYZ"), IOException);
}

TEST(ExceptionTests, GivenManualResetEvent_WhenTimeoutExpires_ThrowsTimeoutException) {
    ManualResetEvent evt(false);
    EXPECT_THROW(evt.WaitOne(1), TimeoutException);
}

TEST(ExceptionTests, GivenEventLog_WhenEmptySourceProvided_ThrowsArgumentException) {
    EXPECT_THROW(EventLog::WriteEntry("", "Test message"), ArgumentException);
    EXPECT_THROW(EventLog::Delete(""), ArgumentException);
    EXPECT_THROW(EventLog::CreateEventSource("", "Application"), ArgumentException);
}

TEST(ExceptionTests, GivenEtwLogReader_WhenEmptyChannelProvided_ThrowsArgumentException) {
    EXPECT_THROW(EtwLogReader::ReadEvents(""), ArgumentException);
}

TEST(ExceptionTests, GivenEtwLogReader_WhenAlreadyListening_ThrowsInvalidOperationException) {
    EtwLogReader reader;
    reader.StartListening("Application", [](const EtwEvent&) {});
    EXPECT_THROW(reader.StartListening("Application", [](const EtwEvent&) {}), InvalidOperationException);
    reader.StopListening();
}

TEST(ExceptionTests, GivenTcpClient_WhenInvalidPortConnected_ThrowsSocketException) {
    TcpClient client;
    EXPECT_THROW(client.Connect("127.0.0.1", 1), SocketException);
}

TEST(ExceptionTests, GivenJsonSerializer_WhenInvalidJsonDeserialized_ThrowsJsonException) {
    EXPECT_THROW(JsonSerializer::Deserialize<String>("{ unclosed invalid json"), JsonException);
}

TEST(ExceptionTests, GivenElevationPrivileges_WhenQueryingUserPrincipal_BehavesAccordingToPrivileges) {
    bool bIsElevated = IsElevatedProcess();
    if (bIsElevated) {
        auto users = UserPrincipal::EnumerateUsers();
        EXPECT_GE(users.GetCount(), 1);
        UserInfo current = UserPrincipal::GetCurrent();
        EXPECT_FALSE(current.sUsername.IsEmpty());
        EXPECT_FALSE(current.sDomain.IsEmpty());
    } else {
        try {
            auto users = UserPrincipal::EnumerateUsers();
            EXPECT_GE(users.GetCount(), 1);
        } catch (const UnauthorizedAccessException& ex) {
            EXPECT_TRUE(String(ex.What()).Contains("Access denied"));
        }
    }
}
