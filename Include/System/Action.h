#pragma once
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            template <typename... Args>
            struct IActionHolder {
                virtual ~IActionHolder() = default;
                virtual void Invoke(Args... args) = 0;
                virtual IActionHolder* Clone() const = 0;
            };

            template <typename F, typename... Args>
            struct ActionHolder : IActionHolder<Args...> {
                F m_fn;
                ActionHolder(const F& fn) : m_fn(fn) {}
                ActionHolder(F&& fn) : m_fn(static_cast<F&&>(fn)) {}

                void Invoke(Args... args) override {
                    m_fn(args...);
                }

                IActionHolder<Args...>* Clone() const override {
                    return new ActionHolder<F, Args...>(m_fn);
                }
            };
        }

        template<typename... Args>
        class Action {
        public:
            Action() : m_pHolder(nullptr) {}
            Action(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Action> && std::is_invocable_v<std::decay_t<F>, Args...>>>
            Action(F&& func) : m_pHolder(new Internal::ActionHolder<std::decay_t<F>, Args...>(static_cast<F&&>(func))) {}

            Action(const Action& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            Action(Action&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            Action& operator=(const Action& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            Action& operator=(Action&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            ~Action() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            void Invoke(Args... args) const {
                if (m_pHolder) m_pHolder->Invoke(args...);
            }
            
            void operator()(Args... args) const {
                Invoke(args...);
            }
            
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IActionHolder<Args...>* m_pHolder;
        };
    }
}
