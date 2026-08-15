#pragma once
#include <type_traits>
#include <utility>

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

        // Primary Template
        template<typename... Args>
        class Action;

        // Specialization: Parameterless Action (0 Arguments)
        template<>
        class Action<> {
        public:
            Action() : m_pHolder(nullptr) {}
            Action(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Action> && std::is_invocable_v<std::decay_t<F>>>>
            Action(F&& func) : m_pHolder(new Internal::ActionHolder<std::decay_t<F>>(static_cast<F&&>(func))) {}

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
            
            void Invoke() const {
                if (m_pHolder) m_pHolder->Invoke();
            }
            
            void operator()() const {
                Invoke();
            }
            
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IActionHolder<>* m_pHolder;
        };

        // Partial Specialization: Parameterized Action (1 or more arguments)
        template<typename Arg1, typename... Args>
        class Action<Arg1, Args...> {
        public:
            Action() : m_pHolder(nullptr) {}
            Action(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Action> && std::is_invocable_v<std::decay_t<F>, Arg1, Args...>>>
            Action(F&& func) : m_pHolder(new Internal::ActionHolder<std::decay_t<F>, Arg1, Args...>(static_cast<F&&>(func))) {}

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
            
            void Invoke(Arg1 arg1, Args... args) const {
                if (m_pHolder) m_pHolder->Invoke(arg1, args...);
            }
            
            void operator()(Arg1 arg1, Args... args) const {
                Invoke(arg1, args...);
            }
            
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IActionHolder<Arg1, Args...>* m_pHolder;
        };
    }
}
