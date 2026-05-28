#pragma once
#include <functional>
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        template<typename T>
        class Predicate {
        public:
            Predicate() = default;
            Predicate(std::nullptr_t) : _func(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Predicate> && std::is_invocable_r_v<bool, F, T>>>
            Predicate(F&& func) : _func(std::forward<F>(func)) {}
            
            bool Invoke(T obj) const {
                return _func(obj);
            }
            
            bool operator()(T obj) const {
                return Invoke(obj);
            }
            
            explicit operator bool() const { return (bool)_func; }

        private:
            std::function<bool(T)> _func;
        };
    }
}
