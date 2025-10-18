#pragma once

#include "Common.h"
#include "Object.h"
#include "String.h"

namespace DotNetDupe
{
    namespace System
    {
        class Version : public Object
        {
        public:
            DOTNETDUPE_API Version(int major, int minor, int build, int revision);
            DOTNETDUPE_API Version(int major, int minor, int build);
            DOTNETDUPE_API Version(int major, int minor);
            DOTNETDUPE_API Version();

            DOTNETDUPE_API int GetMajor() const;
            DOTNETDUPE_API int GetMinor() const;
            DOTNETDUPE_API int GetBuild() const;
            DOTNETDUPE_API int GetRevision() const;

            DOTNETDUPE_API String ToString() const;

            DOTNETDUPE_API bool operator==(const Version& other) const;
            DOTNETDUPE_API bool operator!=(const Version& other) const;

        private:
            int _major;
            int _minor;
            int _build;
            int _revision;
        };
    }
}
