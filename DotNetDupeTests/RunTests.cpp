
#include "pch.h"
#include "gtest/gtest.h"
#include <signal.h>

int main(int argc, char** argv) {
#if !defined(_WIN32)
    signal(SIGPIPE, SIG_IGN);
#endif
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
