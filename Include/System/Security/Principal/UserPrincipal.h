#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Principal {

                enum class UserClass {
                    Guest = 0,
                    Normal = 1,
                    Admin = 2,
                    System = 3
                };

                struct UserInfo {
                    String sUsername;
                    String sDomain;
                    String sSidOrUid;
                    UserClass eUserClass;
                    bool bIsDisabled;
                    bool bIsPasswordRequired;
                    bool bIsAccountLocked;
                    Collections::Generic::List<String> lstGroups;
                    Collections::Generic::List<String> lstPermissions;
                };

                class UserPrincipal : public Object {
                public:
                    DOTNETDUPE_API UserPrincipal();
                    DOTNETDUPE_API virtual ~UserPrincipal();

                    DOTNETDUPE_API static Collections::Generic::List<UserInfo> EnumerateUsers();
                    DOTNETDUPE_API static UserInfo GetUser(const String& sUsername);
                    DOTNETDUPE_API static UserInfo GetCurrent();
                };

            }
        }
    }
}
