#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Environment.h"

using namespace DotNetDupe::System;

namespace SystemTests
{
    namespace EnvironmentTests
    {
        TEST(EnvironmentTest, When_GetMachineNameIsCalled_Then_ReturnsANonEmptyString)
        {
            String machineName = Environment::GetMachineName();
            EXPECT_FALSE(machineName.IsEmpty());
        }

        TEST(EnvironmentTest, When_GetUserNameIsCalled_Then_ReturnsANonEmptyString)
        {
            String userName = Environment::GetUserName();
            EXPECT_FALSE(userName.IsEmpty());
        }

        TEST(EnvironmentTest, When_GetProcessorCountIsCalled_Then_ReturnsAValueGreaterThanZero)
        {
            int processorCount = Environment::GetProcessorCount();
            EXPECT_GT(processorCount, 0);
        }

        TEST(EnvironmentTest, When_GetNewLineIsCalled_Then_ReturnsTheCorrectNewLineString)
        {
            String newLine = Environment::GetNewLine();
            EXPECT_EQ(String(_T("\r\n")), newLine);
        }
    }
}
