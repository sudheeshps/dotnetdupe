#include "pch.h"
#include "System/Diagnostics/Process.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

namespace SystemTests {
    namespace ProcessTests {

        TEST(Process, Start_WhenValidCommand_StartsProcess) {
            // Given
#if defined(_WIN32)
            String sFileName = "cmd.exe";
            String sArguments = "/c exit 0";
#else
            String sFileName = "/bin/sh";
            String sArguments = "-c \"exit 0\"";
#endif

            // When
            auto pProcess = Process::Start(sFileName, sArguments);

            // Then
            ASSERT_FALSE(pProcess.IsNull());
            ASSERT_GT(pProcess->GetId(), 0);
            pProcess->WaitForExit();
            ASSERT_TRUE(pProcess->GetHasExited());
            ASSERT_EQ(pProcess->GetExitCode(), 0);
        }

        TEST(Process, Start_WhenInvalidCommand_ReturnsNull) {
            // Given
            String sFileName = "non_existent_command_12345";

            // When
            auto pProcess = Process::Start(sFileName);

            // Then
            ASSERT_TRUE(pProcess.IsNull());
        }

        TEST(Process, WaitForExit_WhenTimeoutReached_ReturnsFalse) {
            // Given
#if defined(_WIN32)
            String sFileName = "cmd.exe";
            String sArguments = "/c ping 127.0.0.1 -n 3 > nul";
#else
            String sFileName = "/bin/sleep";
            String sArguments = "3";
#endif
            auto pProcess = Process::Start(sFileName, sArguments);
            ASSERT_FALSE(pProcess.IsNull());

            // When
            bool bExited = pProcess->WaitForExit(100);

            // Then
            ASSERT_FALSE(bExited);
            pProcess->Kill();
        }

        TEST(Process, Kill_WhenCalled_TerminatesProcess) {
            // Given
#if defined(_WIN32)
            String sFileName = "cmd.exe";
            String sArguments = "/c ping 127.0.0.1 -n 10 > nul";
#else
            String sFileName = "/bin/sleep";
            String sArguments = "10";
#endif
            auto pProcess = Process::Start(sFileName, sArguments);
            ASSERT_FALSE(pProcess.IsNull());

            // When
            pProcess->Kill();

            // Then
            ASSERT_TRUE(pProcess->GetHasExited());
        }

        TEST(Process, GetExitCode_WhenFinished_ReturnsCorrectCode) {
            // Given
#if defined(_WIN32)
            String sFileName = "cmd.exe";
            String sArguments = "/c exit 123";
#else
            String sFileName = "/bin/sh";
            String sArguments = "-c \"exit 123\"";
#endif
            auto pProcess = Process::Start(sFileName, sArguments);
            ASSERT_FALSE(pProcess.IsNull());

            // When
            pProcess->WaitForExit();

            // Then
            ASSERT_TRUE(pProcess->GetHasExited());
            ASSERT_EQ(pProcess->GetExitCode(), 123);
        }

        TEST(Process, GetId_WhenRunning_ReturnsPositiveId) {
            // Given
#if defined(_WIN32)
            String sFileName = "cmd.exe";
            String sArguments = "/c ping 127.0.0.1 -n 2 > nul";
#else
            String sFileName = "/bin/sleep";
            String sArguments = "1";
#endif

            // When
            auto pProcess = Process::Start(sFileName, sArguments);

            // Then
            ASSERT_FALSE(pProcess.IsNull());
            ASSERT_GT(pProcess->GetId(), 0);
            pProcess->WaitForExit();
        }
    }
}
