#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include "System/ArgumentException.h"
#include "System/Collections/Generic/ICollection.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Generic/IEnumerator.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class LinkedList;

                /// \class LinkedListNode
                /// \brief Represents a node in a LinkedList.
                /// \tparam T Specifies the element type of the linked list.
                template <typename T>
                class LinkedListNode : public Object {
                public:
                    T Value;                    ///< The value contained in the node.
                    LinkedListNode<T>* Next;     ///< Gets the next node in the LinkedList.
                    LinkedListNode<T>* Previous; ///< Gets the previous node in the LinkedList.

                    /// \brief Initializes a new instance of the LinkedListNode class, containing the specified value.
                    /// \param val The value to contain in the LinkedListNode.
                    LinkedListNode(const T& val) : Value(val), Next(nullptr), Previous(nullptr) {}

                    void* operator new(size_t size) {
                        return System::AllocateCollectionBuffer(size);
                    }
                    void operator delete(void* p) {
                        System::FreeCollectionBuffer(p);
                    }
                };

                /// \class LinkedListEnumerator
                /// \brief Enumerates the elements of a LinkedList.
                /// \tparam T The element type.
                template <typename T>
                class LinkedListEnumerator : public virtual IEnumerator<T> {
                private:
                    const LinkedList<T>* m_pList;
                    LinkedListNode<T>* m_pCurrentNode;
                    bool m_bStarted;

                public:
                    /// \brief Constructs an enumerator for the given list.
                    /// \param pList Pointer to the LinkedList instance.
                    explicit LinkedListEnumerator(const LinkedList<T>* pList)
                        : m_pList(pList), m_pCurrentNode(nullptr), m_bStarted(false) {}

                    /// \brief Gets the current element in the collection.
                    /// \return Reference to the current element.
                    const T& Current() const override { return GetCurrent(); }

                    /// \brief Gets the current element in the collection.
                    /// \return Reference to the current element.
                    const T& GetCurrent() const override {
                        /// Guard: Validate iteration state.
                        if (!m_bStarted || m_pCurrentNode == nullptr) {
                            throw InvalidOperationException("Enumeration has either not started or has already finished.");
                        }
                        return m_pCurrentNode->Value;
                    }

                    /// \brief Advances to the next element.
                    /// \return True if advanced; false if reached end.
                    bool MoveNext() override {
                        /// Advance current node pointer.
                        if (!m_bStarted) {
                            m_bStarted = true;
                            m_pCurrentNode = m_pList->GetFirst();
                        } else if (m_pCurrentNode != nullptr) {
                            m_pCurrentNode = m_pCurrentNode->Next;
                        }
                        return m_pCurrentNode != nullptr;
                    }

                    /// \brief Resets enumerator before the first element.
                    void Reset() override {
                        m_pCurrentNode = nullptr;
                        m_bStarted = false;
                    }
                };

                /// \class LinkedList
                /// \brief Represents a doubly linked list.
                /// \tparam T Specifies the element type of the linked list.
                /// \note Conforms to ECMA-335 Partition IV Section 5.42 (System.Collections.Generic.LinkedList<T>).
                ///       Provides constant time O(1) insertion and removal at both ends.
                template <typename T>
                class LinkedList : public virtual ICollection<T>,
                                   public virtual IReadOnlyCollection<T> {
                private:
                    LinkedListNode<T>* m_pHead;
                    LinkedListNode<T>* m_pTail;
                    int m_iCount;

                public:
                    using typename IEnumerable<T>::Iterator;
                    using IEnumerable<T>::begin;
                    using IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the LinkedList class that is empty.
                    LinkedList() : m_pHead(nullptr), m_pTail(nullptr), m_iCount(0) {}

                    /// \brief Destructor. Clears all nodes.
                    ~LinkedList() override {
                        Clear();
                    }

                    /// \brief Gets the number of nodes actually contained in the LinkedList.
                    /// \return The number of nodes contained in the LinkedList.
                    int GetCount() const override { return m_iCount; }

                    /// \brief Gets a value indicating whether the collection is read-only.
                    /// \return False for LinkedList.
                    bool IsReadOnly() const override { return false; }

                    /// \brief Gets the first node of the LinkedList.
                    /// \return The first LinkedListNode of the LinkedList.
                    LinkedListNode<T>* GetFirst() const { return m_pHead; }

                    /// \brief Gets the last node of the LinkedList.
                    /// \return The last LinkedListNode of the LinkedList.
                    LinkedListNode<T>* GetLast() const { return m_pTail; }

                    /// \brief Adds an item to the end of the collection.
                    /// \param item The item to add.
                    void Add(const T& item) override {
                        AddLast(item);
                    }

                    /// \brief Adds a new node containing the specified value at the start of the list.
                    /// \param value The value to add.
                    /// \return The newly created node.
                    LinkedListNode<T>* AddFirst(const T& value) {
                        /// Allocate node and link at head.
                        LinkedListNode<T>* pNode = new LinkedListNode<T>(value);
                        if (!m_pHead) {
                            m_pHead = m_pTail = pNode;
                        } else {
                            pNode->Next = m_pHead;
                            m_pHead->Previous = pNode;
                            m_pHead = pNode;
                        }
                        m_iCount++;
                        return pNode;
                    }

                    /// \brief Adds a new node containing the specified value at the end of the list.
                    /// \param value The value to add.
                    /// \return The newly created node.
                    LinkedListNode<T>* AddLast(const T& value) {
                        /// Allocate node and link at tail.
                        LinkedListNode<T>* pNode = new LinkedListNode<T>(value);
                        if (!m_pTail) {
                            m_pHead = m_pTail = pNode;
                        } else {
                            m_pTail->Next = pNode;
                            pNode->Previous = m_pTail;
                            m_pTail = pNode;
                        }
                        m_iCount++;
                        return pNode;
                    }

                    /// \brief Removes the node at the start of the list.
                    void RemoveFirst() {
                        /// Guard: Verify list is not empty.
                        if (!m_pHead) throw System::InvalidOperationException("LinkedList is empty.");
                        LinkedListNode<T>* pTemp = m_pHead;
                        m_pHead = m_pHead->Next;
                        if (m_pHead) {
                            m_pHead->Previous = nullptr;
                        } else {
                            m_pTail = nullptr;
                        }
                        delete pTemp;
                        m_iCount--;
                    }

                    /// \brief Removes the node at the end of the list.
                    void RemoveLast() {
                        /// Guard: Verify list is not empty.
                        if (!m_pTail) throw System::InvalidOperationException("LinkedList is empty.");
                        LinkedListNode<T>* pTemp = m_pTail;
                        m_pTail = m_pTail->Previous;
                        if (m_pTail) {
                            m_pTail->Next = nullptr;
                        } else {
                            m_pHead = nullptr;
                        }
                        delete pTemp;
                        m_iCount--;
                    }

                    /// \brief Removes the first occurrence of the specified value.
                    /// \param value The value to remove.
                    /// \return True if removed; false otherwise.
                    bool Remove(const T& value) override {
                        /// Search for matching node and unlink.
                        LinkedListNode<T>* pCurr = m_pHead;
                        while (pCurr) {
                            if (pCurr->Value == value) {
                                UnlinkNode(pCurr);
                                delete pCurr;
                                m_iCount--;
                                return true;
                            }
                            pCurr = pCurr->Next;
                        }
                        return false;
                    }

                    /// \brief Determines whether a value is in the LinkedList.
                    /// \param value The value to locate.
                    /// \return True if found; false otherwise.
                    bool Contains(const T& value) const override {
                        /// Search for matching value.
                        LinkedListNode<T>* pCurr = m_pHead;
                        while (pCurr) {
                            if (pCurr->Value == value) return true;
                            pCurr = pCurr->Next;
                        }
                        return false;
                    }

                    /// \brief Removes all nodes from the LinkedList.
                    void Clear() override {
                        /// Free all nodes in traversal sequence.
                        LinkedListNode<T>* pCurr = m_pHead;
                        while (pCurr) {
                            LinkedListNode<T>* pNext = pCurr->Next;
                            delete pCurr;
                            pCurr = pNext;
                        }
                        m_pHead = m_pTail = nullptr;
                        m_iCount = 0;
                    }

                    /// \brief Copies elements to an array starting at a particular array index.
                    /// \param array Destination array.
                    /// \param arrayIndex Starting destination index.
                    void CopyTo(Array<T>& array, int arrayIndex) const override {
                        /// Guard: Validate destination boundaries.
                        if (arrayIndex < 0 || arrayIndex + m_iCount > array.GetLength()) {
                            throw ArgumentException("Target array is too small or index is invalid.");
                        }
                        LinkedListNode<T>* pCurr = m_pHead;
                        int iIdx = arrayIndex;
                        while (pCurr) {
                            array[iIdx++] = pCurr->Value;
                            pCurr = pCurr->Next;
                        }
                    }

                    /// \brief Copies the elements of the LinkedList to a new array.
                    /// \return An array containing copies of the elements.
                    Array<T> ToArray() const {
                        /// Allocate and populate result array.
                        Array<T> arrResult(m_iCount);
                        CopyTo(arrResult, 0);
                        return arrResult;
                    }

                    /// \brief Returns an enumerator that iterates through the collection.
                    /// \return An enumerator for the LinkedList.
                    IEnumeratorPtr<T> GetEnumerator() const override {
                        return IEnumeratorPtr<T>(new LinkedListEnumerator<T>(this), true);
                    }

                private:
                    void UnlinkNode(LinkedListNode<T>* pCurr) {
                        if (pCurr->Previous) {
                            pCurr->Previous->Next = pCurr->Next;
                        } else {
                            m_pHead = pCurr->Next;
                        }
                        if (pCurr->Next) {
                            pCurr->Next->Previous = pCurr->Previous;
                        } else {
                            m_pTail = pCurr->Previous;
                        }
                    }
                };

            }
        }
    }
}
