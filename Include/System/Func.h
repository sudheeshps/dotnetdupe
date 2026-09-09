/// \file Func.h
/// \brief Encapsulates a method that has parameters and returns a value of the type specified by the TResult parameter.
///
/// Modeled after .NET System.Func delegates (ECMA-335).

#pragma once
#include <type_traits>
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            /// \brief Internal interface defining type-erased functor execution and cloning.
            template <typename TResult, typename... Args>
            struct IFunctorHolder {
                virtual ~IFunctorHolder() = default;
                virtual TResult Invoke(Args... args) = 0;
                virtual IFunctorHolder* Clone() const = 0;
            };

            /// \brief Internal wrapper holding a concrete callable functor object.
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

        /// \brief Primary template declaration for the Func delegate family.
        /// \tparam TResult The return type of the encapsulated method.
        /// \tparam Args The argument types passed to the encapsulated method.
        template<typename TResult, typename... Args>
        class Func;

        /// \class Func<TResult>
        /// \brief Encapsulates a method that has no parameters and returns a value of type TResult.
        ///
        /// \note Thread Safety: Invoking concurrent copies is thread-safe if the underlying functor is reentrant.
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        template<typename TResult>
        class Func<TResult> {
        public:
            /// \brief Initializes an empty instance of the Func delegate.
            Func() : m_pHolder(nullptr) {}

            /// \brief Initializes an empty instance from a null pointer.
            Func(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            /// \brief Constructs a Func delegate from an invocable callable object or lambda.
            /// \tparam F The invocable type.
            /// \param func The callable object to wrap.
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Func> && std::is_invocable_r_v<TResult, std::decay_t<F>>>>
            Func(F&& func) : m_pHolder(new Internal::FunctorHolder<std::decay_t<F>, TResult>(static_cast<F&&>(func))) {}

            /// \brief Copy constructor.
            /// \param other The delegate instance to copy.
            Func(const Func& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            /// \brief Move constructor.
            /// \param other The delegate instance to move from.
            Func(Func&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            /// \brief Copy assignment operator.
            /// \param other The delegate instance to copy.
            /// \return Reference to this delegate.
            Func& operator=(const Func& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            /// \brief Move assignment operator.
            /// \param other The delegate instance to move from.
            /// \return Reference to this delegate.
            Func& operator=(Func&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            /// \brief Destructor releasing functor resources.
            ~Func() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            /// \brief Invokes the encapsulated method and returns the result.
            /// \return Result of the method execution, or default constructed TResult if empty.
            TResult Invoke() const {
                if (m_pHolder) return m_pHolder->Invoke();
                return TResult();
            }
            
            /// \brief Function call operator forwarding to Invoke().
            /// \return Result of the method execution.
            TResult operator()() const {
                return Invoke();
            }
            
            /// \brief Checks if the delegate encapsulates a non-null target.
            /// \return True if non-null, false otherwise.
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IFunctorHolder<TResult>* m_pHolder;
        };

        /// \class Func<TResult, Arg1, Args...>
        /// \brief Encapsulates a method that has one or more parameters and returns a value of type TResult.
        ///
        /// \tparam TResult The return type.
        /// \tparam Arg1 The first argument type.
        /// \tparam Args Additional argument types.
        /// \note Thread Safety: Safe for concurrent invocation if the underlying callable is reentrant.
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        template<typename TResult, typename Arg1, typename... Args>
        class Func<TResult, Arg1, Args...> {
        public:
            /// \brief Initializes an empty instance of the parameterized Func delegate.
            Func() : m_pHolder(nullptr) {}

            /// \brief Initializes an empty instance from a null pointer.
            Func(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            /// \brief Constructs a Func delegate from an invocable callable object or lambda.
            /// \tparam F The invocable type.
            /// \param func The callable object to wrap.
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Func> && std::is_invocable_r_v<TResult, std::decay_t<F>, Arg1, Args...>>>
            Func(F&& func) : m_pHolder(new Internal::FunctorHolder<std::decay_t<F>, TResult, Arg1, Args...>(static_cast<F&&>(func))) {}

            /// \brief Copy constructor.
            /// \param other The delegate instance to copy.
            Func(const Func& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            /// \brief Move constructor.
            /// \param other The delegate instance to move from.
            Func(Func&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            /// \brief Copy assignment operator.
            /// \param other The delegate instance to copy.
            /// \return Reference to this delegate.
            Func& operator=(const Func& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            /// \brief Move assignment operator.
            /// \param other The delegate instance to move from.
            /// \return Reference to this delegate.
            Func& operator=(Func&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            /// \brief Destructor releasing functor resources.
            ~Func() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            /// \brief Invokes the encapsulated method with specified parameters and returns the result.
            /// \param arg1 First argument.
            /// \param args Additional arguments.
            /// \return Result of the method execution, or default constructed TResult if empty.
            TResult Invoke(Arg1 arg1, Args... args) const {
                if (m_pHolder) return m_pHolder->Invoke(arg1, args...);
                return TResult();
            }
            
            /// \brief Function call operator forwarding to Invoke().
            /// \param arg1 First argument.
            /// \param args Additional arguments.
            /// \return Result of the method execution.
            TResult operator()(Arg1 arg1, Args... args) const {
                return Invoke(arg1, args...);
            }
            
            /// \brief Checks if the delegate encapsulates a non-null target.
            /// \return True if non-null, false otherwise.
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IFunctorHolder<TResult, Arg1, Args...>* m_pHolder;
        };
    }
}
