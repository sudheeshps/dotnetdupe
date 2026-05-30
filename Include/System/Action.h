#pragma once
#include <functional>
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        template<typename... Args>
        class Action {
        public:
            Action() = default;
            Action(std::nullptr_t) : _func(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Action> && std::is_invocable_v<F, Args...>>>
            Action(F&& func) : _func(std::forward<F>(func)) {}
            
            void Invoke(Args... args) const {
                if (_func) _func(args...);
            }
            
            void operator()(Args... args) const {
                Invoke(args...);
            }
            
            explicit operator bool() const { return (bool)_func; }

        private:
            std::function<void(Args...)> _func;
        };
    }
}
