/// \file UserPrincipal.h
/// \brief User principal and account enumeration operations per POSIX and Win32 security APIs.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Principal {

                /// \enum UserClass
                /// \brief Defines user account classification categories.
                enum class UserClass {
                    Guest = 0,   ///< Guest or anonymous account.
                    Normal = 1,  ///< Standard authenticated user account.
                    Admin = 2,   ///< Administrative or root user account.
                    System = 3   ///< System service or daemon account.
                };

                /// \struct UserInfo
                /// \brief Represents platform user account information and privileges.
                struct UserInfo {
                    String sUsername;                                  ///< User login or account name.
                    String sDomain;                                    ///< Domain or host machine name.
                    String sSidOrUid;                                  ///< Security identifier (SID on Windows, UID on POSIX).
                    UserClass eUserClass;                              ///< Classification level of the user.
                    bool bIsDisabled;                                  ///< Indicates whether the account is disabled.
                    bool bIsPasswordRequired;                          ///< Indicates whether a password is required.
                    bool bIsAccountLocked;                             ///< Indicates whether the account is currently locked out.
                    Collections::Generic::List<String> lstGroups;      ///< List of security groups the user belongs to.
                    Collections::Generic::List<String> lstPermissions; ///< List of assigned permission strings.
                };

                /// \class UserPrincipal
                /// \brief Provides static methods for managing and enumerating user accounts and current process principal.
                class UserPrincipal : public Object {
                public:
                    /// \brief Initializes a new instance of the UserPrincipal class.
                    DOTNETDUPE_API UserPrincipal();

                    /// \brief Releases resources used by the UserPrincipal.
                    DOTNETDUPE_API virtual ~UserPrincipal();

                    /// \brief Enumerates all registered local user accounts on the host system.
                    /// \return A list of UserInfo records for all accounts.
                    DOTNETDUPE_API static Collections::Generic::List<UserInfo> EnumerateUsers();

                    /// \brief Retrieves account information for a specified username.
                    /// \param sUsername The account username to look up.
                    /// \return UserInfo record corresponding to the username.
                    DOTNETDUPE_API static UserInfo GetUser(const String& sUsername);

                    /// \brief Retrieves account information for the currently executing process user.
                    /// \return UserInfo record representing the current security principal.
                    DOTNETDUPE_API static UserInfo GetCurrent();
                };

            }
        }
    }
}
