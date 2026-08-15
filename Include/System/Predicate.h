#pragma once
#include "System/Func.h"
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        template<typename T>
        class Predicate {
        public:
            Predicate() : m_func() {}
            Predicate(decltype(nullptr)) : m_func(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Predicate> && std::is_invocable_r_v<bool, std::decay_t<F>, T>>>
            Predicate(F&& func) : m_func(static_cast<F&&>(func)) {}
            
            bool Invoke(T obj) const {
                return m_func ? m_func.Invoke(obj) : false;
            }
            
            bool operator()(T obj) const {
                return Invoke(obj);
            }
            
            explicit operator bool() const { return static_cast<bool>(m_func); }

        private:
            Func<bool, T> m_func;
        };
    }
}
