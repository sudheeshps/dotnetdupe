#pragma once
#include <functional>
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        template<typename TResult, typename... Args>
        class Func {
        public:
            Func() = default;
            Func(std::nullptr_t) : _func(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Func> && std::is_invocable_r_v<TResult, F, Args...>>>
            Func(F&& func) : _func(std::forward<F>(func)) {}
            
            TResult Invoke(Args... args) const {
                return _func(args...);
            }
            
            TResult operator()(Args... args) const {
                return Invoke(args...);
            }
            
            explicit operator bool() const { return (bool)_func; }

        private:
            std::function<TResult(Args...)> _func;
        };
    }
}
