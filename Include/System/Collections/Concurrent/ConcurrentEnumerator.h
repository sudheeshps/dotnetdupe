/// \file ConcurrentEnumerator.h
/// \brief Thread-safe snapshot enumerator for concurrent collections.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/IEnumerator.h"
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                /// \class ConcurrentEnumerator
                /// \brief Enumerates a snapshot of elements from a concurrent collection.
                /// \tparam T The element type.
                ///
                /// Captures an immutable snapshot of the collection taken under mutual exclusion,
                /// allowing lock-free and thread-safe iteration without holding locks.
                template <typename T>
                class ConcurrentEnumerator : public virtual Generic::IEnumerator<T> {
                private:
                    Array<T> m_arrSnapshot;
                    int m_iIndex;
                    bool m_bStarted;

                public:
                    /// \brief Initializes an enumerator with the given snapshot array.
                    /// \param arrSnapshot Snapshot array of collection elements.
                    explicit ConcurrentEnumerator(Array<T> arrSnapshot)
                        : m_arrSnapshot(std::move(arrSnapshot)), m_iIndex(-1), m_bStarted(false) {}

                    /// \brief Gets the current element in the enumeration.
                    /// \return Reference to the current element.
                    const T& Current() const override {
                        return GetCurrent();
                    }

                    /// \brief Gets the current element in the enumeration.
                    /// \return Reference to the current element.
                    const T& GetCurrent() const override {
                        /// Guard: Validate iteration state.
                        if (!m_bStarted || m_iIndex < 0 || m_iIndex >= m_arrSnapshot.GetCount()) {
                            throw InvalidOperationException("Enumeration has either not started or has already finished.");
                        }
                        return m_arrSnapshot[m_iIndex];
                    }

                    /// \brief Advances the enumerator to the next element.
                    /// \return True if the enumerator was successfully advanced; false if at the end.
                    bool MoveNext() override {
                        /// Step: Advance position.
                        if (!m_bStarted) {
                            m_bStarted = true;
                            m_iIndex = 0;
                        } else {
                            m_iIndex++;
                        }
                        return m_iIndex < m_arrSnapshot.GetCount();
                    }

                    /// \brief Resets the enumerator before the first element.
                    void Reset() override {
                        m_iIndex = -1;
                        m_bStarted = false;
                    }
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe
