#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Generic/IEnumerator.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class Stack;

                /// \class StackEnumerator
                /// \brief Enumerates elements of a Stack in LIFO order.
                /// \tparam T The element type.
                template <typename T>
                class StackEnumerator : public virtual IEnumerator<T> {
                private:
                    const Stack<T>* m_pStack;
                    int m_iIndex;
                    bool m_bStarted;

                public:
                    /// \brief Constructs an enumerator for the given Stack.
                    explicit StackEnumerator(const Stack<T>* pStack)
                        : m_pStack(pStack), m_iIndex(-1), m_bStarted(false) {}

                    /// \brief Gets current element in the stack.
                    const T& Current() const override { return GetCurrent(); }

                    /// \brief Gets current element in the stack.
                    const T& GetCurrent() const override {
                        if (!m_bStarted || m_iIndex < 0 || m_iIndex >= m_pStack->GetCount()) {
                            throw InvalidOperationException("Enumeration has either not started or has already finished.");
                        }
                        return m_pStack->GetAt(m_pStack->GetCount() - 1 - m_iIndex);
                    }

                    /// \brief Advances to next element.
                    bool MoveNext() override {
                        if (!m_bStarted) {
                            m_bStarted = true;
                            m_iIndex = 0;
                        } else {
                            m_iIndex++;
                        }
                        return m_iIndex < m_pStack->GetCount();
                    }

                    /// \brief Resets enumerator.
                    void Reset() override {
                        m_iIndex = -1;
                        m_bStarted = false;
                    }
                };

                /// \class Stack
                /// \brief Represents a variable size last-in-first-out (LIFO) collection of instances of the same specified type.
                ///
                /// \tparam T Specifies the type of elements in the stack.
                /// \note Conforms to ECMA-335 Partition IV Section 5.41 (System.Collections.Generic.Stack<T>).
                ///       Implements LIFO collection semantics with Push, Pop, Peek, and non-throwing Try* variants.
                template <typename T>
                class Stack : public virtual IReadOnlyCollection<T> {
                private:
                    List<T> m_lstItems;

                public:
                    using typename IEnumerable<T>::Iterator;
                    using IEnumerable<T>::begin;
                    using IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the Stack class that is empty.
                    Stack() = default;

                    /// \brief Gets the number of elements contained in the Stack.
                    /// \return The number of elements contained in the Stack.
                    int GetCount() const override { return m_lstItems.GetCount(); }

                    /// \brief Gets element at internal zero-based index.
                    const T& GetAt(int index) const { return m_lstItems[index]; }

                    /// \brief Inserts an object at the top of the Stack.
                    /// \param item The object to push onto the Stack.
                    void Push(const T& item) {
                        m_lstItems.Add(item);
                    }

                    /// \brief Removes and returns the object at the top of the Stack.
                    /// \return The object removed from the top of the Stack.
                    /// \throws InvalidOperationException The Stack is empty.
                    T Pop() {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Stack is empty.");
                        }
                        int lastIndex = m_lstItems.GetCount() - 1;
                        T item = m_lstItems[lastIndex];
                        m_lstItems.RemoveAt(lastIndex);
                        return item;
                    }

                    /// \brief Returns the object at the top of the Stack without removing it.
                    /// \return The object at the top of the Stack.
                    /// \throws InvalidOperationException The Stack is empty.
                    T Peek() const {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Stack is empty.");
                        }
                        return m_lstItems[m_lstItems.GetCount() - 1];
                    }

                    /// \brief Removes the object at the top of the Stack and copies it to the result parameter.
                    /// \param result Output parameter receiving the removed object.
                    /// \return true if an object was successfully removed; false if the Stack is empty.
                    bool TryPop(T& result) {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        int lastIndex = m_lstItems.GetCount() - 1;
                        result = m_lstItems[lastIndex];
                        m_lstItems.RemoveAt(lastIndex);
                        return true;
                    }

                    /// \brief Returns a value that indicates whether there is an object at the top of the Stack.
                    /// \param result Output parameter receiving the peeked object.
                    /// \return true if there is an object at the top of the Stack; false if the Stack is empty.
                    bool TryPeek(T& result) const {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        result = m_lstItems[m_lstItems.GetCount() - 1];
                        return true;
                    }

                    /// \brief Removes all objects from the Stack.
                    void Clear() {
                        m_lstItems.Clear();
                    }

                    /// \brief Determines whether an element is in the Stack.
                    /// \param item The object to locate in the Stack.
                    /// \return true if item is found in the Stack; otherwise, false.
                    bool Contains(const T& item) const {
                        return m_lstItems.Contains(item);
                    }

                    /// \brief Copies elements to an array in LIFO order.
                    /// \param array Destination array.
                    /// \param arrayIndex Starting index.
                    void CopyTo(Array<T>& array, int arrayIndex) const {
                        if (arrayIndex < 0 || arrayIndex + GetCount() > array.GetLength()) {
                            throw ArgumentException("Target array is too small or index is invalid.");
                        }
                        int count = m_lstItems.GetCount();
                        for (int i = 0; i < count; ++i) {
                            array[arrayIndex + i] = m_lstItems[count - 1 - i];
                        }
                    }

                    /// \brief Copies the Stack to a new array in LIFO order.
                    /// \return A new array containing copies of the elements of the Stack.
                    Array<T> ToArray() const {
                        int count = m_lstItems.GetCount();
                        Array<T> arrResult(count);
                        for (int i = 0; i < count; ++i) {
                            arrResult[i] = m_lstItems[count - 1 - i];
                        }
                        return arrResult;
                    }

                    /// \brief Returns an enumerator that iterates through the Stack.
                    /// \return An enumerator for the Stack.
                    IEnumeratorPtr<T> GetEnumerator() const override {
                        return IEnumeratorPtr<T>(new StackEnumerator<T>(this), true);
                    }
                };

            }
        }
    }
}
