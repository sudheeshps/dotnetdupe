#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

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

                /// \class LinkedList
                /// \brief Represents a doubly linked list.
                ///
                /// \tparam T Specifies the element type of the linked list.
                /// \note Conforms to ECMA-335 Partition IV Section 5.42 (System.Collections.Generic.LinkedList<T>).
                ///       Provides constant time O(1) insertion and removal at both ends.
                template <typename T>
                class LinkedList : public Object {
                private:
                    LinkedListNode<T>* m_pHead;
                    LinkedListNode<T>* m_pTail;
                    int m_iCount;

                public:
                    /// \brief Initializes a new instance of the LinkedList class that is empty.
                    LinkedList() : m_pHead(nullptr), m_pTail(nullptr), m_iCount(0) {}

                    /// \brief Destructor. Clears all nodes.
                    ~LinkedList() override {
                        Clear();
                    }

                    /// \brief Gets the number of nodes actually contained in the LinkedList.
                    /// \return The number of nodes contained in the LinkedList.
                    int GetCount() const { return m_iCount; }

                    /// \brief Gets the first node of the LinkedList.
                    /// \return The first LinkedListNode of the LinkedList.
                    LinkedListNode<T>* GetFirst() const { return m_pHead; }

                    /// \brief Gets the last node of the LinkedList.
                    /// \return The last LinkedListNode of the LinkedList.
                    LinkedListNode<T>* GetLast() const { return m_pTail; }

                    LinkedListNode<T>* AddFirst(const T& value) {
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

                    LinkedListNode<T>* AddLast(const T& value) {
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

                    void RemoveFirst() {
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

                    void RemoveLast() {
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

                    bool Remove(const T& value) {
                        LinkedListNode<T>* pCurr = m_pHead;
                        while (pCurr) {
                            if (pCurr->Value == value) {
                                if (pCurr->Previous) pCurr->Previous->Next = pCurr->Next;
                                else m_pHead = pCurr->Next;

                                if (pCurr->Next) pCurr->Next->Previous = pCurr->Previous;
                                else m_pTail = pCurr->Previous;

                                delete pCurr;
                                m_iCount--;
                                return true;
                            }
                            pCurr = pCurr->Next;
                        }
                        return false;
                    }

                    bool Contains(const T& value) const {
                        LinkedListNode<T>* pCurr = m_pHead;
                        while (pCurr) {
                            if (pCurr->Value == value) return true;
                            pCurr = pCurr->Next;
                        }
                        return false;
                    }

                    void Clear() {
                        LinkedListNode<T>* pCurr = m_pHead;
                        while (pCurr) {
                            LinkedListNode<T>* pNext = pCurr->Next;
                            delete pCurr;
                            pCurr = pNext;
                        }
                        m_pHead = m_pTail = nullptr;
                        m_iCount = 0;
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult(m_iCount);
                        LinkedListNode<T>* pCurr = m_pHead;
                        int iIndex = 0;
                        while (pCurr) {
                            arrResult[iIndex++] = pCurr->Value;
                            pCurr = pCurr->Next;
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}
