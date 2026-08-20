#include "pch.h"
#include "System/Version.h"
#include "System/ArgumentException.h"
#include "System/FormatException.h"
#include <sstream>
#include <vector>
#include <string>

namespace DotNetDupe {
    namespace System {

        static bool ParseComponent(const std::string& sPart, int& iVal) {
            if (sPart.empty()) return false;
            for (char c : sPart) {
                if (c < '0' || c > '9') return false;
            }
            try {
                long long llVal = std::stoll(sPart);
                if (llVal < 0 || llVal > 2147483647LL) return false;
                iVal = static_cast<int>(llVal);
                return true;
            } catch (...) {
                return false;
            }
        }

        static bool SplitVersionParts(const String& sInput, std::vector<std::string>& vecParts) {
            if (sInput.IsEmpty()) return false;
            std::string s = sInput.GetRawString();
            if (s.empty() || s.front() == '.' || s.back() == '.') return false;
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, '.')) {
                if (item.empty()) return false;
                vecParts.push_back(item);
            }
            return (vecParts.size() >= 2 && vecParts.size() <= 4);
        }

        static bool TryParseInternal(const String& sInput, int& iMajor, int& iMinor, int& iBuild, int& iRevision, int& iCount) {
            std::vector<std::string> vecParts;
            if (!SplitVersionParts(sInput, vecParts)) return false;
            iCount = static_cast<int>(vecParts.size());
            int arrVals[4] = { 0, 0, 0, 0 };
            for (size_t i = 0; i < vecParts.size(); ++i) {
                if (!ParseComponent(vecParts[i], arrVals[i])) return false;
            }
            iMajor = arrVals[0];
            iMinor = arrVals[1];
            iBuild = (iCount >= 3) ? arrVals[2] : 0;
            iRevision = (iCount == 4) ? arrVals[3] : 0;
            return true;
        }

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

        Version Version::Parse(const String& sInput) {
            if (sInput.IsEmpty()) {
                throw ArgumentException("Input string cannot be empty.");
            }
            Version vResult;
            if (!TryParse(sInput, vResult)) {
                throw FormatException("Input string was not in a correct format.");
            }
            return vResult;
        }

        bool Version::TryParse(const String& sInput, Version& vResult) {
            int iMajor = 0, iMinor = 0, iBuild = 0, iRevision = 0, iCount = 0;
            if (!TryParseInternal(sInput, iMajor, iMinor, iBuild, iRevision, iCount)) {
                return false;
            }
            if (iCount == 2) vResult = Version(iMajor, iMinor);
            else if (iCount == 3) vResult = Version(iMajor, iMinor, iBuild);
            else vResult = Version(iMajor, iMinor, iBuild, iRevision);
            return true;
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
