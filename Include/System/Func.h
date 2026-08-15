#pragma once
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            template <typename TResult, typename... Args>
            struct IFunctorHolder {
                virtual ~IFunctorHolder() = default;
                virtual TResult Invoke(Args... args) = 0;
                virtual IFunctorHolder* Clone() const = 0;
            };

            template <typename F, typename TResult, typename... Args>
            struct FunctorHolder : IFunctorHolder<TResult, Args...> {
                F m_fn;
                FunctorHolder(const F& fn) : m_fn(fn) {}
                FunctorHolder(F&& fn) : m_fn(static_cast<F&&>(fn)) {}

                TResult Invoke(Args... args) override {
                    return m_fn(args...);
                }

                IFunctorHolder<TResult, Args...>* Clone() const override {
                    return new FunctorHolder<F, TResult, Args...>(m_fn);
                }
            };
        }

        // Primary Template
        template<typename TResult, typename... Args>
        class Func;

        // Specialization: Parameterless Func (0 Arguments)
        template<typename TResult>
        class Func<TResult> {
        public:
            Func() : m_pHolder(nullptr) {}
            Func(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Func> && std::is_invocable_r_v<TResult, std::decay_t<F>>>>
            Func(F&& func) : m_pHolder(new Internal::FunctorHolder<std::decay_t<F>, TResult>(static_cast<F&&>(func))) {}

            Func(const Func& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            Func(Func&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            Func& operator=(const Func& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            Func& operator=(Func&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            ~Func() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            TResult Invoke() const {
                if (m_pHolder) return m_pHolder->Invoke();
                return TResult();
            }
            
            TResult operator()() const {
                return Invoke();
            }
            
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IFunctorHolder<TResult>* m_pHolder;
        };

        // Partial Specialization: Parameterized Func (1 or more arguments)
        template<typename TResult, typename Arg1, typename... Args>
        class Func<TResult, Arg1, Args...> {
        public:
            Func() : m_pHolder(nullptr) {}
            Func(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Func> && std::is_invocable_r_v<TResult, std::decay_t<F>, Arg1, Args...>>>
            Func(F&& func) : m_pHolder(new Internal::FunctorHolder<std::decay_t<F>, TResult, Arg1, Args...>(static_cast<F&&>(func))) {}

            Func(const Func& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            Func(Func&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            Func& operator=(const Func& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            Func& operator=(Func&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            ~Func() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            TResult Invoke(Arg1 arg1, Args... args) const {
                if (m_pHolder) return m_pHolder->Invoke(std::forward<Arg1>(arg1), std::forward<Args>(args)...);
                return TResult();
            }
            
            TResult operator()(Arg1 arg1, Args... args) const {
                return Invoke(std::forward<Arg1>(arg1), std::forward<Args>(args)...);
            }
            
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IFunctorHolder<TResult, Arg1, Args...>* m_pHolder;
        };
    }
}
