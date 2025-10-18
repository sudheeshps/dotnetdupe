#include "pch.h"
#include "Version.h"
#include <sstream>

namespace DotNetDupe
{
    namespace System
    {
        Version::Version(int major, int minor, int build, int revision)
            : _major(major), _minor(minor), _build(build), _revision(revision)
        {
        }

        Version::Version(int major, int minor, int build)
            : _major(major), _minor(minor), _build(build), _revision(0)
        {
        }

        Version::Version(int major, int minor)
            : _major(major), _minor(minor), _build(0), _revision(0)
        {
        }

        Version::Version()
            : _major(0), _minor(0), _build(0), _revision(0)
        {
        }

        int Version::GetMajor() const
        {
            return _major;
        }

        int Version::GetMinor() const
        {
            return _minor;
        }

        int Version::GetBuild() const
        {
            return _build;
        }

        int Version::GetRevision() const
        {
            return _revision;
        }

        String Version::ToString() const
        {
            std::wstringstream ss;
            ss << _major << L"." << _minor << L"." << _build << L"." << _revision;
            return String(ss.str().c_str());
        }

        bool Version::operator==(const Version& other) const
        {
            return _major == other._major &&
                   _minor == other._minor &&
                   _build == other._build &&
                   _revision == other._revision;
        }

        bool Version::operator!=(const Version& other) const
        {
            return !(*this == other);
        }

    }
}
