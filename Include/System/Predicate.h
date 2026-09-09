/// \file Predicate.h
/// \brief Represents the method that defines a set of criteria and determines whether the specified object meets those criteria.
///
/// Modeled after .NET System.Predicate<T> (ECMA-335).

#pragma once
#include "System/Func.h"
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        /// \class Predicate
        /// \brief Represents the method that defines a set of criteria and determines whether the specified object meets those criteria.
        ///
        /// \tparam T The type of the object to compare.
        /// \note Thread Safety: Safe for concurrent invocation if the encapsulated function object is reentrant.
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        template<typename T>
        class Predicate {
        public:
            /// \brief Initializes an empty instance of the Predicate class.
            Predicate() : m_func() {}

            /// \brief Initializes an empty instance from a null pointer.
            Predicate(decltype(nullptr)) : m_func(nullptr) {}
            
            /// \brief Constructs a Predicate delegate from a callable object or lambda.
            /// \tparam F The invocable type.
            /// \param func The callable object evaluating criteria on an instance of T.
            template<typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Predicate> && std::is_invocable_r_v<bool, std::decay_t<F>, T>>>
            Predicate(F&& func) : m_func(static_cast<F&&>(func)) {}
            
            /// \brief Evaluates the predicate criteria against the provided object.
            /// \param obj The target object to evaluate.
            /// \return True if the object matches the criteria, false otherwise.
            bool Invoke(T obj) const {
                return m_func ? m_func.Invoke(obj) : false;
            }
            
            /// \brief Function call operator forwarding to Invoke().
            /// \param obj The target object to evaluate.
            /// \return True if the object matches the criteria, false otherwise.
            bool operator()(T obj) const {
                return Invoke(obj);
            }
            
            /// \brief Checks if the predicate encapsulates a non-null target callable.
            /// \return True if non-null, false otherwise.
            explicit operator bool() const { return static_cast<bool>(m_func); }

        private:
            Func<bool, T> m_func;
        };
    }
}
