#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe
{
    namespace System
    {
        class Version : public Object
        {
        public:
            DOTNETDUPE_API Version(int iMajor, int iMinor, int iBuild, int iRevision);
            DOTNETDUPE_API Version(int iMajor, int iMinor, int iBuild);
            DOTNETDUPE_API Version(int iMajor, int iMinor);
            DOTNETDUPE_API Version();

            DOTNETDUPE_API int GetMajor() const;
            DOTNETDUPE_API int GetMinor() const;
            DOTNETDUPE_API int GetBuild() const;
            DOTNETDUPE_API int GetRevision() const;

            DOTNETDUPE_API String ToString() const;

            DOTNETDUPE_API bool operator==(const Version& vOther) const;
            DOTNETDUPE_API bool operator!=(const Version& vOther) const;

        private:
            int m_iMajor;
            int m_iMinor;
            int m_iBuild;
            int m_iRevision;
        };
    }
}
