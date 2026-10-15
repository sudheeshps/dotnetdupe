#include "pch.h"
#include "gtest/gtest.h"
#include "System/Array.h"
#include "System/Threading/Tasks/Task.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Threading::Tasks;

TEST(TaskWaitAllTests, GivenMultipleRunningTasks_WhenWaitAllCalled_ThenAllTasksComplete) {
    bool bTask1Executed = false;
    bool bTask2Executed = false;

    SmartPointer<Task> pT1 = Task::Run([&bTask1Executed]() {
        Thread::Sleep(50);
        bTask1Executed = true;
    });

    SmartPointer<Task> pT2 = Task::Run([&bTask2Executed]() {
        Thread::Sleep(50);
        bTask2Executed = true;
    });

    Array<SmartPointer<Task>> arrTasks(2);
    arrTasks[0] = pT1;
    arrTasks[1] = pT2;

    bool bSuccess = Task::WaitAll(arrTasks, 2000);
    EXPECT_TRUE(bSuccess);
    EXPECT_TRUE(bTask1Executed);
    EXPECT_TRUE(bTask2Executed);
}
