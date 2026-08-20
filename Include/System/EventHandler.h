#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/EventArgs.h"
#include "System/Action.h"
#include "System/Collections/Generic/List.h"
#include <type_traits>
#include <utility>

namespace DotNetDupe {
    namespace System {

        template <typename TEventArgs = EventArgs>
        class EventHandler {
        private:
            struct SubscriberEntry {
                size_t m_nToken;
                Action<const void*, const TEventArgs&> m_fnAction;

                SubscriberEntry() : m_nToken(0), m_fnAction() {}
                SubscriberEntry(size_t nToken, const Action<const void*, const TEventArgs&>& fnAction)
                    : m_nToken(nToken), m_fnAction(fnAction) {}

                bool operator==(const SubscriberEntry& other) const {
                    return m_nToken == other.m_nToken;
                }
            };

            Collections::Generic::List<SubscriberEntry> m_lstSubscribers;
            size_t m_nNextToken;

        public:
            EventHandler() : m_lstSubscribers(), m_nNextToken(1) {}

            EventHandler(const EventHandler& other)
                : m_lstSubscribers(other.m_lstSubscribers), m_nNextToken(other.m_nNextToken) {}

            EventHandler(EventHandler&& other) noexcept
                : m_lstSubscribers(std::move(other.m_lstSubscribers)), m_nNextToken(other.m_nNextToken) {
                other.m_nNextToken = 1;
            }

            EventHandler& operator=(const EventHandler& other) {
                if (this != &other) {
                    m_lstSubscribers = other.m_lstSubscribers;
                    m_nNextToken = other.m_nNextToken;
                }
                return *this;
            }

            EventHandler& operator=(EventHandler&& other) noexcept {
                if (this != &other) {
                    m_lstSubscribers = std::move(other.m_lstSubscribers);
                    m_nNextToken = other.m_nNextToken;
                    other.m_nNextToken = 1;
                }
                return *this;
            }

            ~EventHandler() = default;

            size_t Add(const Action<const void*, const TEventArgs&>& fnHandler) {
                size_t nAssignedToken = m_nNextToken++;
                m_lstSubscribers.Add(SubscriberEntry(nAssignedToken, fnHandler));
                return nAssignedToken;
            }

            template <typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, EventHandler> && !std::is_same_v<std::decay_t<F>, Action<const void*, const TEventArgs&>> && std::is_invocable_v<std::decay_t<F>, const void*, const TEventArgs&>>>
            size_t Add(F&& fnHandler) {
                return Add(Action<const void*, const TEventArgs&>(std::forward<F>(fnHandler)));
            }

            template <typename TClass>
            size_t Add(TClass* pInstance, void (TClass::*pMethod)(const void*, const TEventArgs&)) {
                return Add([pInstance, pMethod](const void* pSender, const TEventArgs& e) {
                    if (pInstance != nullptr && pMethod != nullptr) {
                        (pInstance->*pMethod)(pSender, e);
                    }
                });
            }

            size_t operator+=(const Action<const void*, const TEventArgs&>& fnHandler) {
                return Add(fnHandler);
            }

            template <typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, EventHandler> && !std::is_same_v<std::decay_t<F>, Action<const void*, const TEventArgs&>> && std::is_invocable_v<std::decay_t<F>, const void*, const TEventArgs&>>>
            size_t operator+=(F&& fnHandler) {
                return Add(std::forward<F>(fnHandler));
            }

            EventHandler& operator+=(const EventHandler& other) {
                int iCount = other.m_lstSubscribers.GetCount();
                for (int i = 0; i < iCount; ++i) {
                    Add(other.m_lstSubscribers[i].m_fnAction);
                }
                return *this;
            }

            bool Remove(size_t nToken) {
                for (int i = 0; i < m_lstSubscribers.GetCount(); ++i) {
                    if (m_lstSubscribers[i].m_nToken == nToken) {
                        m_lstSubscribers.RemoveAt(i);
                        return true;
                    }
                }
                return false;
            }

            bool operator-=(size_t nToken) {
                return Remove(nToken);
            }

            void Clear() {
                m_lstSubscribers.Clear();
            }

            void Invoke(const void* pSender, const TEventArgs& e) const {
                int iCount = m_lstSubscribers.GetCount();
                for (int i = 0; i < iCount; ++i) {
                    m_lstSubscribers[i].m_fnAction.Invoke(pSender, e);
                }
            }

            void operator()(const void* pSender, const TEventArgs& e) const {
                Invoke(pSender, e);
            }

            bool IsEmpty() const {
                return m_lstSubscribers.GetCount() == 0;
            }

            size_t GetSubscriberCount() const {
                return static_cast<size_t>(m_lstSubscribers.GetCount());
            }

            explicit operator bool() const {
                return !IsEmpty();
            }
        };

        template <typename TEventArgs = EventArgs>
        EventHandler() -> EventHandler<TEventArgs>;

    }
}
