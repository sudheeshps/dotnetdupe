#include "pch.h"
#include "System/Security/Principal/UserPrincipal.h"
#include "System/ArgumentException.h"
#include "System/UnauthorizedAccessException.h"

#include <vector>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <lm.h>
#include <sddl.h>
#include "Win32Internal.h"
#include "System/ComponentModel/Win32Exception.h"
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "advapi32.lib")
using namespace DotNetDupe::System::Internal;
#else
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include "System/SystemException.h"
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

                    NET_API_STATUS nStatus = ::NetUserGetGroups(NULL, pwszUser, 0, (LPBYTE*)&pGroups, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries);
                    if (nStatus == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied querying user groups. Administrator privileges required.");
                    }

                    if (nStatus == NERR_Success) {
                        for (DWORD i = 0; i < dwEntriesRead; i++) {
                            std::string sGroup = StringConvertInternal::WCharToUtf8(pGroups[i].grui0_name);
                            lstPermissions.Add(String("GroupMember:") + sGroup.c_str());
                            lstGroups.Add(sGroup.c_str());
                        }
                        ::NetApiBufferFree(pGroups);
                    }
                }

                static void PopulateWin32SidAndDomain(LPCWSTR pwszUser, UserInfo& info) {
                    BYTE sidBuffer[SECURITY_MAX_SID_SIZE];
                    DWORD cbSid = sizeof(sidBuffer);
                    WCHAR szDomain[256] = { 0 };
                    DWORD cchDomain = 256;
                    SID_NAME_USE peUse;

                    if (::LookupAccountNameW(NULL, pwszUser, (PSID)sidBuffer, &cbSid, szDomain, &cchDomain, &peUse)) {
                        LPWSTR pszStringSid = NULL;
                        if (::ConvertSidToStringSidW((PSID)sidBuffer, &pszStringSid)) {
                            info.sSidOrUid = StringConvertInternal::WCharToUtf8(pszStringSid).c_str();
                            ::LocalFree(pszStringSid);
                        }
                        if (cchDomain > 0) {
                            info.sDomain = StringConvertInternal::WCharToUtf8(szDomain).c_str();
                        }
                    }
                }

                static UserInfo BuildWin32UserInfo(const USER_INFO_1* pUi) {
                    UserInfo info;
                    std::string sName = StringConvertInternal::WCharToUtf8(pUi->usri1_name);
                    info.sUsername = sName.c_str();

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

                    PopulateWin32SidAndDomain(pUi->usri1_name, info);
                    PopulateWin32UserGroups(pUi->usri1_name, info.lstGroups, info.lstPermissions);
                    return info;
                }

                static void EnumerateWin32Users(Collections::Generic::List<UserInfo>& lstUsers) {
                    LPUSER_INFO_1 pBuf = NULL;
                    DWORD dwEntriesRead = 0, dwTotalEntries = 0, dwResumeHandle = 0;

                    NET_API_STATUS nStatus = ::NetUserEnum(NULL, 1, FILTER_NORMAL_ACCOUNT, (LPBYTE*)&pBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
                    if (nStatus == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied enumerating user accounts. Administrator privileges required.");
                    }

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
                    info.sUsername = pw->pw_name;
                    info.sDomain = "LOCAL";
                    info.sSidOrUid = std::to_string(pw->pw_uid).c_str();
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
                    errno = 0;
                    setpwent();
                    struct passwd* pw;
                    while ((pw = getpwent()) != NULL) {
                        lstUsers.Add(BuildLinuxUserInfo(pw));
                    }
                    if (errno == EACCES || errno == EPERM) {
                        endpwent();
                        throw UnauthorizedAccessException("Access denied enumerating user accounts. Root privileges required.");
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

#if defined(_WIN32)
                static UserInfo QueryWin32User(const String& sUsername) {
                    std::wstring wUsername = StringConvertInternal::Utf8ToWChar(sUsername.GetRawString() ? sUsername.GetRawString() : "");
                    LPUSER_INFO_1 pBuf = NULL;
                    NET_API_STATUS nStatus = ::NetUserGetInfo(NULL, wUsername.c_str(), 1, (LPBYTE*)&pBuf);
                    if (nStatus == NERR_Success && pBuf != NULL) {
                        UserInfo info = BuildWin32UserInfo(pBuf);
                        ::NetApiBufferFree(pBuf);
                        return info;
                    }
                    if (nStatus == ERROR_ACCESS_DENIED) throw UnauthorizedAccessException("Access denied querying user information for: " + sUsername);
                    if (nStatus == NERR_UserNotFound || nStatus == ERROR_NO_SUCH_USER) throw ArgumentException("User not found: " + sUsername);
                    throw ComponentModel::Win32Exception(nStatus, "Failed to query user information for: " + sUsername);
                }

                static std::string GetCurrentWin32UserName() {
                    WCHAR szName[256] = { 0 };
                    DWORD dwSize = 256;
                    if (!::GetUserNameW(szName, &dwSize)) {
                        DWORD dwErr = ::GetLastError();
                        if (dwErr == ERROR_ACCESS_DENIED) throw UnauthorizedAccessException("Access denied querying current user name.");
                        throw ComponentModel::Win32Exception(dwErr, "Failed to get current user name.");
                    }
                    return StringConvertInternal::WCharToUtf8(szName);
                }
#else
                static UserInfo QueryLinuxUser(const String& sUsername) {
                    errno = 0;
                    struct passwd* pw = getpwnam(sUsername.GetRawString());
                    if (pw == NULL) {
                        if (errno == EACCES || errno == EPERM) throw UnauthorizedAccessException("Access denied querying user information for: " + sUsername);
                        throw ArgumentException("User not found: " + sUsername);
                    }
                    return BuildLinuxUserInfo(pw);
                }

                static UserInfo QueryCurrentLinuxUser() {
                    errno = 0;
                    struct passwd* pw = getpwuid(getuid());
                    if (pw == NULL) {
                        if (errno == EACCES || errno == EPERM) throw UnauthorizedAccessException("Access denied querying current user information.");
                        throw SystemException("Failed to query current user information.");
                    }
                    return BuildLinuxUserInfo(pw);
                }
#endif

                UserInfo UserPrincipal::GetUser(const String& sUsername) {
                    if (sUsername.IsEmpty()) throw ArgumentException("Username cannot be empty.");
#if defined(_WIN32)
                    return QueryWin32User(sUsername);
#else
                    return QueryLinuxUser(sUsername);
#endif
                }

                UserInfo UserPrincipal::GetCurrent() {
#if defined(_WIN32)
                    return GetUser(GetCurrentWin32UserName().c_str());
#else
                    return QueryCurrentLinuxUser();
#endif
                }

            }
        }
    }
}
