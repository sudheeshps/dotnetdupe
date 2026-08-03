#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename T>
                class BlockingCollection : public Object {
                private:
                    mutable std::mutex m_mtxLock;
                    std::condition_variable m_cvAdd;
                    std::condition_variable m_cvTake;
                    std::queue<T> m_qQueue;
                    int m_iBoundedCapacity;
                    bool m_bIsAddingCompleted;

                public:
                    BlockingCollection() : m_iBoundedCapacity(-1), m_bIsAddingCompleted(false) {}
                    
                    explicit BlockingCollection(int iBoundedCapacity) : m_iBoundedCapacity(iBoundedCapacity), m_bIsAddingCompleted(false) {
                        if (iBoundedCapacity <= 0) {
                            throw System::ArgumentException("Bounded capacity must be greater than zero.");
                        }
                    }

                    void Add(const T& item) {
                        std::unique_lock<std::mutex> lock(m_mtxLock);
                        
                        if (m_bIsAddingCompleted) {
                            throw System::InvalidOperationException("The collection has been marked as complete for adding.");
                        }

                        if (m_iBoundedCapacity > 0) {
                            m_cvAdd.wait(lock, [this]() {
                                return m_bIsAddingCompleted || (int)m_qQueue.size() < m_iBoundedCapacity;
                            });

                            if (m_bIsAddingCompleted) {
                                throw System::InvalidOperationException("The collection has been marked as complete for adding.");
                            }
                        }

                        m_qQueue.push(item);
                        m_cvTake.notify_one();
                    }

                    bool TryAdd(const T& item, int iTimeoutMilliseconds = 0) {
                        std::unique_lock<std::mutex> lock(m_mtxLock);
                        
                        if (m_bIsAddingCompleted) {
                            return false;
                        }

                        if (m_iBoundedCapacity > 0 && (int)m_qQueue.size() >= m_iBoundedCapacity) {
                            if (iTimeoutMilliseconds <= 0) {
                                return false;
                            }

                            bool bWaitResult = m_cvAdd.wait_for(lock, std::chrono::milliseconds(iTimeoutMilliseconds), [this]() {
                                return m_bIsAddingCompleted || (int)m_qQueue.size() < m_iBoundedCapacity;
                            });

                            if (!bWaitResult || m_bIsAddingCompleted) {
                                return false;
                            }
                        }

                        m_qQueue.push(item);
                        m_cvTake.notify_one();
                        return true;
                    }

                    T Take() {
                        T item;
                        if (!TryTake(item, -1)) {
                            throw System::InvalidOperationException("The collection is empty and has been marked as complete for adding.");
                        }

                        return item;
                    }

                    bool TryTake(T& item, int iTimeoutMilliseconds = 0) {
                        std::unique_lock<std::mutex> lock(m_mtxLock);

                        if (m_qQueue.empty()) {
                            if (m_bIsAddingCompleted) {
                                return false;
                            }

                            if (iTimeoutMilliseconds == 0) {
                                return false;
                            }

                            if (iTimeoutMilliseconds < 0) {
                                m_cvTake.wait(lock, [this]() {
                                    return m_bIsAddingCompleted || !m_qQueue.empty();
                                });
                            } else {
                                m_cvTake.wait_for(lock, std::chrono::milliseconds(iTimeoutMilliseconds), [this]() {
                                    return m_bIsAddingCompleted || !m_qQueue.empty();
                                });
                            }

                            if (m_qQueue.empty()) {
                                return false;
                            }
                        }

                        item = m_qQueue.front();
                        m_qQueue.pop();

                        if (m_iBoundedCapacity > 0) {
                            m_cvAdd.notify_one();
                        }

                        return true;
                    }

                    void CompleteAdding() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_bIsAddingCompleted = true;
                        m_cvTake.notify_all();
                        m_cvAdd.notify_all();
                    }

                    bool IsAddingCompleted() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_bIsAddingCompleted;
                    }

                    bool IsCompleted() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_bIsAddingCompleted && m_qQueue.empty();
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return (int)m_qQueue.size();
                    }

                    int GetBoundedCapacity() const {
                        return m_iBoundedCapacity;
                    }

                    Array<T> ToArray() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        Array<T> arrResult((int)m_qQueue.size());
                        std::queue<T> qTemp = m_qQueue;
                        int iIndex = 0;

                        while (!qTemp.empty()) {
                            arrResult[iIndex++] = qTemp.front();
                            qTemp.pop();
                        }

                        return arrResult;
                    }
                };

            }
        }
    }
}
