#pragma once
#include <type_traits>
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            /// \struct IActionHolder
            /// \brief Type-erased polymorphic interface for invocable action delegates.
            template <typename... Args>
            struct IActionHolder {
                virtual ~IActionHolder() = default;
                virtual void Invoke(Args... args) = 0;
                virtual IActionHolder* Clone() const = 0;
            };

            /// \struct ActionHolder
            /// \brief Concrete wrapper holding a callable functor or lambda instance.
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

        /// \class Action
        /// \brief Encapsulates a method that has parameters and does not return a value.
        ///
        /// Modeled after .NET System.Action delegates. Provides type-erased functor wrapping
        /// for free functions, member functions, functors, and C++ lambdas.
        template<typename... Args>
        class Action;

        /// \class Action<>
        /// \brief Encapsulates a parameterless method that does not return a value.
        template<>
        class Action<> {
        public:
            /// \brief Default constructor creating an empty unassigned Action.
            Action() : m_pHolder(nullptr) {}

            /// \brief Constructs an empty Action from nullptr.
            Action(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            /// \brief Constructs an Action wrapping the specified callable functor.
            /// \tparam F The callable functor type.
            /// \param func The callable object.
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Action> && std::is_invocable_v<std::decay_t<F>>>>
            Action(F&& func) : m_pHolder(new Internal::ActionHolder<std::decay_t<F>>(static_cast<F&&>(func))) {}

            /// \brief Copy constructor performing deep clone of target delegate.
            /// \param other The Action to copy.
            Action(const Action& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            /// \brief Move constructor transferring delegate ownership.
            /// \param other The Action to move from.
            Action(Action&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            /// \brief Copy assignment operator.
            Action& operator=(const Action& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            /// \brief Move assignment operator.
            Action& operator=(Action&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            /// \brief Destructor releasing target callable holder.
            ~Action() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            /// \brief Invokes the wrapped action delegate.
            void Invoke() const {
                if (m_pHolder) m_pHolder->Invoke();
            }
            
            /// \brief Function call operator executing Invoke.
            void operator()() const {
                Invoke();
            }
            
            /// \brief Boolean conversion testing whether a target delegate is bound.
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IActionHolder<>* m_pHolder;
        };

        /// \class Action<Arg1, Args...>
        /// \brief Encapsulates a method that has one or more parameters and does not return a value.
        template<typename Arg1, typename... Args>
        class Action<Arg1, Args...> {
        public:
            /// \brief Default constructor.
            Action() : m_pHolder(nullptr) {}

            /// \brief Nullptr constructor.
            Action(decltype(nullptr)) : m_pHolder(nullptr) {}
            
            /// \brief Constructs an Action wrapping the specified callable object.
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Action> && std::is_invocable_v<std::decay_t<F>, Arg1, Args...>>>
            Action(F&& func) : m_pHolder(new Internal::ActionHolder<std::decay_t<F>, Arg1, Args...>(static_cast<F&&>(func))) {}

            /// \brief Copy constructor.
            Action(const Action& other) : m_pHolder(other.m_pHolder ? other.m_pHolder->Clone() : nullptr) {}

            /// \brief Move constructor.
            Action(Action&& other) noexcept : m_pHolder(other.m_pHolder) {
                other.m_pHolder = nullptr;
            }

            /// \brief Copy assignment operator.
            Action& operator=(const Action& other) {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder ? other.m_pHolder->Clone() : nullptr;
                }
                return *this;
            }

            /// \brief Move assignment operator.
            Action& operator=(Action&& other) noexcept {
                if (this != &other) {
                    delete m_pHolder;
                    m_pHolder = other.m_pHolder;
                    other.m_pHolder = nullptr;
                }
                return *this;
            }

            /// \brief Destructor.
            ~Action() {
                delete m_pHolder;
                m_pHolder = nullptr;
            }
            
            /// \brief Invokes the wrapped action delegate with arguments.
            void Invoke(Arg1 arg1, Args... args) const {
                if (m_pHolder) m_pHolder->Invoke(arg1, args...);
            }
            
            /// \brief Function call operator executing Invoke.
            void operator()(Arg1 arg1, Args... args) const {
                Invoke(arg1, args...);
            }
            
            /// \brief Boolean conversion operator.
            explicit operator bool() const { return m_pHolder != nullptr; }

        private:
            Internal::IActionHolder<Arg1, Args...>* m_pHolder;
        };

    }
}
