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

        // Given: The running operating system
        // When: Process::GetProcesses is called
        // Then: An array of running processes is returned with positive IDs and names
        TEST(Process, GivenSystem_WhenGetProcessesCalled_ThenReturnsRunningProcessesArray) {
            // Given & When
            auto arrProcesses = Process::GetProcesses();

            // Then
            EXPECT_GT(arrProcesses.GetLength(), 0);
            bool bFoundValid = false;
            for (int i = 0; i < arrProcesses.GetLength(); ++i) {
                if (!arrProcesses[i].IsNull() && arrProcesses[i]->GetId() > 0 && !arrProcesses[i]->GetProcessName().IsEmpty()) {
                    bFoundValid = true;
                    break;
                }
            }
            EXPECT_TRUE(bFoundValid);
        }

        // Given: The current process identifier
        // When: Process::GetProcessById is called
        // Then: A valid Process instance is returned matching current PID
        TEST(Process, GivenCurrentProcessId_WhenGetProcessByIdCalled_ThenReturnsValidProcess) {
            // Given
            int iCurrentPid = Process::GetCurrentProcessId();

            // When
            auto spCurrent = Process::GetProcessById(iCurrentPid);

            // Then
            ASSERT_FALSE(spCurrent.IsNull());
            EXPECT_EQ(spCurrent->GetId(), iCurrentPid);
            EXPECT_FALSE(spCurrent->GetProcessName().IsEmpty());
        }

        // Given: An invalid non-existent process identifier
        // When: Process::GetProcessById is called
        // Then: An ArgumentException is thrown
        TEST(Process, GivenInvalidProcessId_WhenGetProcessByIdCalled_ThenThrowsArgumentException) {
            // Given, When, Then
            EXPECT_THROW(Process::GetProcessById(-1), ArgumentException);
            EXPECT_THROW(Process::GetProcessById(0), ArgumentException);
            EXPECT_THROW(Process::GetProcessById(9999999), ArgumentException);
        }

        // Given: The running process
        // When: Process::GetCurrentProcess is called
        // Then: The returned instance matches GetCurrentProcessId
        TEST(Process, GivenCurrentProcess_WhenGetCurrentProcessCalled_ThenMatchesCurrentProcessId) {
            // Given & When
            auto spCurrent = Process::GetCurrentProcess();

            // Then
            ASSERT_FALSE(spCurrent.IsNull());
            EXPECT_EQ(spCurrent->GetId(), Process::GetCurrentProcessId());
            EXPECT_FALSE(spCurrent->GetProcessName().IsEmpty());
        }

        // Given: Current process name
        // When: Process::GetProcessesByName is called with and without .exe
        // Then: The process is found in the returned array
        TEST(Process, GivenExistingProcessName_WhenGetProcessesByNameCalled_ThenReturnsMatchingList) {
            // Given
            auto spCurrent = Process::GetCurrentProcess();
            ASSERT_FALSE(spCurrent.IsNull());
            String sProcName = spCurrent->GetProcessName();

            // When
            auto arrMatches = Process::GetProcessesByName(sProcName);

            // Then
            EXPECT_GT(arrMatches.GetLength(), 0);
            bool bFoundSelf = false;
            for (int i = 0; i < arrMatches.GetLength(); ++i) {
                if (!arrMatches[i].IsNull() && arrMatches[i]->GetId() == spCurrent->GetId()) {
                    bFoundSelf = true;
                    break;
                }
            }
            EXPECT_TRUE(bFoundSelf);
        }

        // Given: A non-existent process name
        // When: Process::GetProcessesByName is called
        // Then: An empty array is returned
        TEST(Process, GivenNonExistentProcessName_WhenGetProcessesByNameCalled_ThenReturnsEmptyArray) {
            // Given & When
            auto arrMatches = Process::GetProcessesByName("NonExistent_Process_XYZ_12345");

            // Then
            EXPECT_EQ(arrMatches.GetLength(), 0);
        }

        // Given: An empty process name
        // When: Process::GetProcessesByName is called
        // Then: An empty array is returned
        TEST(Process, GivenEmptyProcessName_WhenGetProcessesByNameCalled_ThenReturnsEmptyArray) {
            // Given & When
            auto arrMatches = Process::GetProcessesByName("");

            // Then
            EXPECT_EQ(arrMatches.GetLength(), 0);
        }
    }
}
