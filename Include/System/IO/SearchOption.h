/// \file SearchOption.h
/// \brief Specifies whether to search the current directory, or the current directory and all subdirectories.

#pragma once

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \enum SearchOption
            /// \brief Specifies whether to search the current directory, or the current directory and all subdirectories.
            ///
            /// Conforms to ECMA-335 Partition IV Section 5.49 (System.IO.SearchOption).
            enum class SearchOption {
                /// \brief Includes only the current directory in a search operation.
                TopDirectoryOnly = 0,

                /// \brief Includes the current directory and all its subdirectories in a search operation.
                AllDirectories = 1
            };
        }
    }
}
