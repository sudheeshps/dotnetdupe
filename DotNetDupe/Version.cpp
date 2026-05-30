#include "pch.h"
#include "System/Version.h"
#include <sstream>

namespace DotNetDupe {
    namespace System {
        Version::Version(int iMajor, int iMinor, int iBuild, int iRevision)
            : m_iMajor(iMajor), m_iMinor(iMinor), m_iBuild(iBuild), m_iRevision(iRevision) { }

        Version::Version(int iMajor, int iMinor, int iBuild)
            : m_iMajor(iMajor), m_iMinor(iMinor), m_iBuild(iBuild), m_iRevision(0) { }

        Version::Version(int iMajor, int iMinor)
            : m_iMajor(iMajor), m_iMinor(iMinor), m_iBuild(0), m_iRevision(0) { }

        Version::Version()
            : m_iMajor(0), m_iMinor(0), m_iBuild(0), m_iRevision(0) { }

        int Version::GetMajor() const {
            return m_iMajor;
        }

        int Version::GetMinor() const {
            return m_iMinor;
        }

        int Version::GetBuild() const {
            return m_iBuild;
        }

        int Version::GetRevision() const {
            return m_iRevision;
        }

        String Version::ToString() const {
            std::stringstream ss;
            ss << m_iMajor << "." << m_iMinor << "." << m_iBuild << "." << m_iRevision;
            return String(ss.str().c_str());
        }

        bool Version::operator==(const Version& vOther) const {
            return m_iMajor == vOther.m_iMajor &&
                m_iMinor == vOther.m_iMinor &&
                m_iBuild == vOther.m_iBuild &&
                m_iRevision == vOther.m_iRevision;
        }

        bool Version::operator!=(const Version& other) const {
            return !(*this == other);
        }

    }
}
