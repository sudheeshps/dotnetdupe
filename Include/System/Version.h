/// \file Version.h
/// \brief Represents the version number of an assembly, operating system, or application component.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe
{
    namespace System
    {
        /// \brief Represents the version number for an assembly, operating system, or component.
        ///
        /// Conforms to .NET System.Version with 2, 3, or 4 component formats:
        /// `major.minor[.build[.revision]]`. Immutable once constructed and thread-safe for concurrent read access.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.6 (System.Version).
        /// \see String, Environment
        class Version : public Object
        {
        public:
            /// \brief Initializes a new instance of the Version class using specified major, minor, build, and revision numbers.
            /// \param iMajor The major version component.
            /// \param iMinor The minor version component.
            /// \param iBuild The build version component.
            /// \param iRevision The revision version component.
            DOTNETDUPE_API Version(int iMajor, int iMinor, int iBuild, int iRevision);

            /// \brief Initializes a new instance of the Version class using specified major, minor, and build numbers.
            /// \param iMajor The major version component.
            /// \param iMinor The minor version component.
            /// \param iBuild The build version component.
            DOTNETDUPE_API Version(int iMajor, int iMinor, int iBuild);

            /// \brief Initializes a new instance of the Version class using specified major and minor numbers.
            /// \param iMajor The major version component.
            /// \param iMinor The minor version component.
            DOTNETDUPE_API Version(int iMajor, int iMinor);

            /// \brief Initializes a new instance of the Version class to 0.0.0.0.
            DOTNETDUPE_API Version();

            /// \brief Gets the value of the major component of the version number.
            /// \return An integer representing the major version number.
            DOTNETDUPE_API int GetMajor() const;

            /// \brief Gets the value of the minor component of the version number.
            /// \return An integer representing the minor version number.
            DOTNETDUPE_API int GetMinor() const;

            /// \brief Gets the value of the build component of the version number.
            /// \return An integer representing the build version number.
            DOTNETDUPE_API int GetBuild() const;

            /// \brief Gets the value of the revision component of the version number.
            /// \return An integer representing the revision version number.
            DOTNETDUPE_API int GetRevision() const;

            /// \brief Converts the value of the current Version object to its equivalent String representation.
            /// \return A string representation of the version in major.minor.build.revision format.
            DOTNETDUPE_API String ToString() const;

            /// \brief Converts the string representation of a version number to an equivalent Version object.
            /// \param sInput String containing 2 to 4 integer components separated by dots.
            /// \return A Version object equivalent to the string representation.
            /// \throws ArgumentException Thrown when string format is invalid or component values are out of range.
            DOTNETDUPE_API static Version Parse(const String& sInput);

            /// \brief Tries to convert the string representation of a version number to an equivalent Version object.
            /// \param sInput String containing the version representation.
            /// \param vResult Output Version instance populated on success.
            /// \return True if parsing succeeded; otherwise, false.
            DOTNETDUPE_API static bool TryParse(const String& sInput, Version& vResult);

            /// \brief Determines whether two Version instances are equal.
            /// \param vOther The Version instance to compare with the current instance.
            /// \return True if all version components match; otherwise, false.
            DOTNETDUPE_API bool operator==(const Version& vOther) const;

            /// \brief Determines whether two Version instances are not equal.
            /// \param vOther The Version instance to compare with the current instance.
            /// \return True if any version component differs; otherwise, false.
            DOTNETDUPE_API bool operator!=(const Version& vOther) const;

        private:
            int m_iMajor;
            int m_iMinor;
            int m_iBuild;
            int m_iRevision;
        };
    }
}
