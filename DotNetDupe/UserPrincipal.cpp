#include "pch.h"
#include "System/Security/Principal/UserPrincipal.h"
#include "System/ArgumentException.h"

#include <vector>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <lm.h>
#include "Win32Internal.h"
#pragma comment(lib, "netapi32.lib")
using namespace DotNetDupe::System::Internal;
#else
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Principal {

                UserPrincipal::UserPrincipal() {
                }

                UserPrincipal::~UserPrincipal() {
                }

#if defined(_WIN32)
                static UserClass ClassifyWindowsUser(DWORD dwPriv, DWORD dwFlags) {
                    if (dwFlags & UF_ACCOUNTDISABLE) return UserClass::Guest;
                    if (dwPriv == USER_PRIV_ADMIN) return UserClass::Admin;
                    if (dwPriv == USER_PRIV_GUEST) return UserClass::Guest;
                    return UserClass::Normal;
                }

                static void PopulateWin32UserGroups(LPCWSTR pwszUser, Collections::Generic::List<String>& lstGroups, Collections::Generic::List<String>& lstPermissions) {
                    LPGROUP_USERS_INFO_0 pGroups = NULL;
                    DWORD dwEntriesRead = 0, dwTotalEntries = 0;

                    if (::NetUserGetGroups(NULL, pwszUser, 0, (LPBYTE*)&pGroups, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries) == NERR_Success) {
                        for (DWORD i = 0; i < dwEntriesRead; i++) {
                            std::string sGroup = StringConvertInternal::WCharToUtf8(pGroups[i].grui0_name);
                            lstPermissions.Add(String("GroupMember:") + String(sGroup.c_str()));
                            lstGroups.Add(String(sGroup.c_str()));
                        }
                        ::NetApiBufferFree(pGroups);
                    }
                }

                static UserInfo BuildWin32UserInfo(const USER_INFO_1* pUi) {
                    UserInfo info;
                    std::string sName = StringConvertInternal::WCharToUtf8(pUi->usri1_name);
                    info.sUsername = String(sName.c_str());

                    info.sDomain = "LOCAL";
                    info.sSidOrUid = "S-1-5-21-USER";
                    info.eUserClass = ClassifyWindowsUser(pUi->usri1_priv, pUi->usri1_flags);
                    info.bIsDisabled = (pUi->usri1_flags & UF_ACCOUNTDISABLE) != 0;
                    info.bIsPasswordRequired = (pUi->usri1_flags & UF_PASSWD_NOTREQD) == 0;
                    info.bIsAccountLocked = (pUi->usri1_flags & UF_LOCKOUT) != 0;

                    if (info.eUserClass == UserClass::Admin) {
                        info.lstPermissions.Add("AdministratorRights");
                        info.lstPermissions.Add("FullControl");
                    } else {
                        info.lstPermissions.Add("StandardUserRights");
                    }

                    PopulateWin32UserGroups(pUi->usri1_name, info.lstGroups, info.lstPermissions);
                    return info;
                }

                static void EnumerateWin32Users(Collections::Generic::List<UserInfo>& lstUsers) {
                    LPUSER_INFO_1 pBuf = NULL;
                    DWORD dwEntriesRead = 0, dwTotalEntries = 0, dwResumeHandle = 0;

                    NET_API_STATUS nStatus = ::NetUserEnum(NULL, 1, FILTER_NORMAL_ACCOUNT, (LPBYTE*)&pBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
                    if (nStatus == NERR_Success || nStatus == ERROR_MORE_DATA) {
                        for (DWORD i = 0; i < dwEntriesRead; i++) {
                            lstUsers.Add(BuildWin32UserInfo(&pBuf[i]));
                        }
                    }

                    if (pBuf != NULL) {
                        ::NetApiBufferFree(pBuf);
                    }
                }
#else
                static UserInfo BuildLinuxUserInfo(const struct passwd* pw) {
                    UserInfo info;
                    info.sUsername = String(pw->pw_name);
                    info.sDomain = "LOCAL";
                    info.sSidOrUid = String(std::to_string(pw->pw_uid).c_str());
                    info.bIsDisabled = false;
                    info.bIsPasswordRequired = true;
                    info.bIsAccountLocked = false;

                    if (pw->pw_uid == 0) {
                        info.eUserClass = UserClass::Admin;
                        info.lstPermissions.Add("RootPrivileges");
                        info.lstPermissions.Add("FullControl");
                    } else if (pw->pw_uid < 1000) {
                        info.eUserClass = UserClass::System;
                        info.lstPermissions.Add("SystemDaemonRights");
                    } else {
                        info.eUserClass = UserClass::Normal;
                        info.lstPermissions.Add("StandardUserRights");
                    }

                    return info;
                }

                static void EnumerateLinuxUsers(Collections::Generic::List<UserInfo>& lstUsers) {
                    setpwent();
                    struct passwd* pw;
                    while ((pw = getpwent()) != NULL) {
                        lstUsers.Add(BuildLinuxUserInfo(pw));
                    }
                    endpwent();
                }
#endif

                Collections::Generic::List<UserInfo> UserPrincipal::EnumerateUsers() {
                    Collections::Generic::List<UserInfo> lstUsers;

#if defined(_WIN32)
                    EnumerateWin32Users(lstUsers);
#else
                    EnumerateLinuxUsers(lstUsers);
#endif

                    return lstUsers;
                }

                UserInfo UserPrincipal::GetUser(const String& sUsername) {
                    if (sUsername.IsEmpty()) {
                        throw ArgumentException("Username cannot be empty.");
                    }

                    auto users = EnumerateUsers();
                    for (int i = 0; i < users.GetCount(); i++) {
                        if (users[i].sUsername.Equals(sUsername)) {
                            return users[i];
                        }
                    }

                    UserInfo fallback;
                    fallback.sUsername = sUsername;
                    fallback.sDomain = "LOCAL";
                    fallback.sSidOrUid = "S-1-5-UNKNOWN";
                    fallback.eUserClass = UserClass::Normal;
                    fallback.bIsDisabled = false;
                    fallback.bIsPasswordRequired = true;
                    fallback.bIsAccountLocked = false;
                    fallback.lstPermissions.Add("StandardUserRights");
                    return fallback;
                }

                UserInfo UserPrincipal::GetCurrent() {
#if defined(_WIN32)
                    WCHAR szName[256] = { 0 };
                    DWORD dwSize = 256;

                    if (::GetUserNameW(szName, &dwSize)) {
                        return GetUser(String(StringConvertInternal::WCharToUtf8(szName).c_str()));
                    }
#else
                    uid_t uid = getuid();
                    struct passwd* pw = getpwuid(uid);
                    if (pw != NULL) {
                        return GetUser(String(pw->pw_name));
                    }
#endif
                    return GetUser("CurrentSystemUser");
                }

            }
        }
    }
}
