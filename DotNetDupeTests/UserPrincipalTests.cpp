#include "pch.h"
#include "gtest/gtest.h"
#include "System/Security/Principal/UserPrincipal.h"
#include "System/ArgumentException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Security::Principal;

TEST(UserPrincipalTests, GivenUserPrincipal_WhenEnumerateUsersCalled_ThenReturnsUserList) {
    // Given & When
    auto users = UserPrincipal::EnumerateUsers();

    // Then
    EXPECT_GE(users.GetCount(), 1);
    bool bHasName = false;
    for (int i = 0; i < users.GetCount(); i++) {
        if (!users[i].sUsername.IsEmpty()) {
            bHasName = true;
            break;
        }
    }
    EXPECT_TRUE(bHasName);
}

TEST(UserPrincipalTests, GivenCurrentSystemUser_WhenGetCurrentCalled_ThenReturnsValidUserInfo) {
    // Given & When
    UserInfo currentUser = UserPrincipal::GetCurrent();

    // Then
    EXPECT_FALSE(currentUser.sUsername.IsEmpty());
    EXPECT_FALSE(currentUser.sDomain.IsEmpty());
    EXPECT_GE(currentUser.lstPermissions.GetCount(), 1);
}

TEST(UserPrincipalTests, GivenKnownUsername_WhenGetUserCalled_ThenReturnsUserRightsAndClass) {
    // Given
    UserInfo currentUser = UserPrincipal::GetCurrent();

    // When
    UserInfo fetchedUser = UserPrincipal::GetUser(currentUser.sUsername);

    // Then
    EXPECT_TRUE(fetchedUser.sUsername.Equals(currentUser.sUsername));
    EXPECT_FALSE(fetchedUser.sSidOrUid.IsEmpty());
}

TEST(UserPrincipalTests, GivenEmptyUsername_WhenGetUserCalled_ThenThrowsArgumentException) {
    // Given, When & Then
    EXPECT_THROW(UserPrincipal::GetUser(""), ArgumentException);
}
