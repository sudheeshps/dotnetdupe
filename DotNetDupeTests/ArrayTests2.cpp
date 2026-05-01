#include "pch.h"
#include "gtest/gtest.h"
#include "System/Array.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace ArrayTests {
        TEST(ArrayTest2, Given_PredicateThatMatchesAnElement_When_ExistsIsCalled_Then_ReturnsTrue) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            bool result = arr.Exists([](int i) { return i == 3; });
            EXPECT_TRUE(result);
        }

        TEST(ArrayTest2, Given_PredicateThatDoesNotMatchAnElement_When_ExistsIsCalled_Then_ReturnsFalse) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            bool result = arr.Exists([](int i) { return i == 6; });
            EXPECT_FALSE(result);
        }

        TEST(ArrayTest2, Given_PredicateThatMatchesAnElement_When_FindIsCalled_Then_ReturnsTheFirstMatchingElement) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.Find([](int i) { return i > 2; });
            EXPECT_EQ(3, result);
        }

        TEST(ArrayTest2, Given_PredicateThatDoesNotMatchAnElement_When_FindIsCalled_Then_ReturnsDefaultValue) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.Find([](int i) { return i > 5; });
            EXPECT_EQ(0, result);
        }

        TEST(ArrayTest2, Given_PredicateThatMatchesElements_When_FindAllIsCalled_Then_ReturnsAnArrayWithTheMatchingElements) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            Array<int> result = arr.FindAll([](int i) { return i % 2 != 0; });
            EXPECT_EQ(3, result.GetLength());
            EXPECT_EQ(1, result [0]);
            EXPECT_EQ(3, result [1]);
            EXPECT_EQ(5, result [2]);
        }

        TEST(ArrayTest2, Given_PredicateThatMatchesAnElement_When_FindIndexIsCalled_Then_ReturnsTheIndexOfTheFirstMatchingElement) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.FindIndex([](int i) { return i > 2; });
            EXPECT_EQ(2, result);
        }

        TEST(ArrayTest2, Given_PredicateThatDoesNotMatchAnElement_When_FindIndexIsCalled_Then_ReturnsNegativeOne) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.FindIndex([](int i) { return i > 5; });
            EXPECT_EQ(-1, result);
        }

        TEST(ArrayTest2, Given_PredicateThatMatchesAnElement_When_FindLastIsCalled_Then_ReturnsTheLastMatchingElement) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.FindLast([](int i) { return i < 4; });
            EXPECT_EQ(3, result);
        }

        TEST(ArrayTest2, Given_PredicateThatDoesNotMatchAnElement_When_FindLastIsCalled_Then_ReturnsDefaultValue) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.FindLast([](int i) { return i > 5; });
            EXPECT_EQ(0, result);
        }

        TEST(ArrayTest2, Given_PredicateThatMatchesAnElement_When_FindLastIndexIsCalled_Then_ReturnsTheIndexOfTheLastMatchingElement) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.FindLastIndex([](int i) { return i < 4; });
            EXPECT_EQ(2, result);
        }

        TEST(ArrayTest2, Given_PredicateThatDoesNotMatchAnElement_When_FindLastIndexIsCalled_Then_ReturnsNegativeOne) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int result = arr.FindLastIndex([](int i) { return i > 5; });
            EXPECT_EQ(-1, result);
        }

        TEST(ArrayTest2, Given_AnAction_When_ForEachIsCalled_Then_TheActionIsPerformedOnEachElement) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            int sum = 0;
            arr.ForEach([&sum](int i) { sum += i; });
            EXPECT_EQ(15, sum);
        }

        TEST(ArrayTest2, Given_APredicateThatAllElementsMatch_When_TrueForAllIsCalled_Then_ReturnsTrue) {
            Array<int> arr = { 2, 4, 6, 8, 10 };
            bool result = arr.TrueForAll([](int i) { return i % 2 == 0; });
            EXPECT_TRUE(result);
        }

        TEST(ArrayTest2, Given_APredicateThatNotAllElementsMatch_When_TrueForAllIsCalled_Then_ReturnsFalse) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            bool result = arr.TrueForAll([](int i) { return i % 2 == 0; });
            EXPECT_FALSE(result);
        }
    }
}