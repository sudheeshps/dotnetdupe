/// \file SmartPointer.h
/// \brief Provides reference-counted and weak pointer memory management primitives ensuring zero raw ownership.

#pragma once

#include "Common.h"
#include "System/SystemException.h"
#include <type_traits>
#include <utility>

#if defined(_WIN32)
#ifndef _INTERLOCKED_DECLARED_
#define _INTERLOCKED_DECLARED_
extern "C" long __cdecl _InterlockedIncrement(long volatile* Addend);
extern "C" long __cdecl _InterlockedDecrement(long volatile* Addend);
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#endif
#endif

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            inline long AtomicIncrement(volatile long* pLocation) {
#if defined(_WIN32)
                return _InterlockedIncrement(pLocation);
#else
                return __sync_add_and_fetch(pLocation, 1);
#endif
            }

            inline long AtomicDecrement(volatile long* pLocation) {
#if defined(_WIN32)
                return _InterlockedDecrement(pLocation);
#else
                return __sync_sub_and_fetch(pLocation, 1);
#endif
            }
        }

        // Helper trait to check if a type is complete at compile time
        template <typename T, typename = void>
        struct IsComplete : std::false_type {};

        template <typename T>
        struct IsComplete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

        class EnableSharedFromThisBase {
        protected:
            constexpr EnableSharedFromThisBase() noexcept : m_pnRefCount(nullptr) {}
            EnableSharedFromThisBase(const EnableSharedFromThisBase&) noexcept : m_pnRefCount(nullptr) {}
            EnableSharedFromThisBase& operator=(const EnableSharedFromThisBase&) noexcept { return *this; }
            virtual ~EnableSharedFromThisBase() = default;

        public:
            mutable volatile long* m_pnRefCount{nullptr};
        };

        template <typename T>
        class EnableSharedFromThis;

        /// \brief A unified smart pointer that supports both unique and shared ownership semantics.
        ///
        /// Provides high-performance automatic lifetime management without raw pointer leaks:
        /// - Unique mode: Zero-overhead RAII ownership (default).
        /// - Shared mode: Atomic reference counting with intrusive or external reference counter blocks.
        /// Thread-safe for reference count increments and decrements in shared mode.
        ///
        /// \note Conforms to DotNetDupe RAII Memory Management Standards (Quality Gate 8 & 11).
        /// \see Object, EnableSharedFromThis
        template <typename T>
        class SmartPointer {
            template <typename U>
            friend class SmartPointer;
            template <typename U>
            friend class EnableSharedFromThis;
        public:
            // --- Auto-Allocating Constructors ---

            /**
             * @brief Default constructor. 
             * For concrete types: Automatically allocates a new instance of T.
             * For abstract types: Initializes to nullptr.
             */
            SmartPointer() {
                if constexpr (IsComplete<T>::value) {
                    if constexpr (!std::is_abstract_v<T> && std::is_default_constructible_v<T>) {
                        m_pObject = new T();
                        m_pnRefCount = nullptr;
                    } else {
                        m_pObject = nullptr;
                        m_pnRefCount = nullptr;
                    }
                } else {
                    m_pObject = nullptr;
                    m_pnRefCount = nullptr;
                }
            }

            /**
             * @brief Constructor with ownership mode flag.
             * Automatically allocates a new instance of T.
             * @param bIsShared If true, enables reference counting (Shared mode).
             */
            explicit SmartPointer(bool bIsShared) {
                if constexpr (IsComplete<T>::value) {
                    if constexpr (!std::is_abstract_v<T> && std::is_default_constructible_v<T>) {
                        m_pObject = new T();
                        m_pnRefCount = bIsShared ? new long(1) : nullptr;
                        SetupEnableSharedFromThis(m_pObject);
                    } else {
                        m_pObject = nullptr;
                        m_pnRefCount = nullptr;
                    }
                } else {
                    m_pObject = nullptr;
                    m_pnRefCount = nullptr;
                }
            }

            // --- Raw Pointer / Explicit Constructors ---

            /**
             * @brief Constructor for explicit raw pointer attachment.
             * @param pPtr The raw pointer to take ownership of.
             */
            explicit SmartPointer(T* pPtr) : m_pObject(pPtr), m_pnRefCount(nullptr) {}

            /**
             * @brief Constructor that specifies ownership mode for a raw pointer.
             * @param pPtr The raw pointer to take ownership of.
             * @param bIsShared If true, enables reference counting (Shared mode).
             */
            SmartPointer(T* pPtr, bool bIsShared)
                : m_pObject(pPtr),
                  m_pnRefCount((bIsShared && pPtr != nullptr) ? new long(1) : nullptr) {
                SetupEnableSharedFromThis(m_pObject);
            }

            /**
             * @brief Explicit null constructor.
             */
            SmartPointer(std::nullptr_t) : m_pObject(nullptr), m_pnRefCount(nullptr) {}

            /**
             * @brief Destructor. Cleans up the managed object based on ownership mode.
             */
            ~SmartPointer() {
                InternalCleanup();
            }

            // --- Copy Semantics ---

            /**
             * @brief Copy constructor. Only permitted if the source is in Shared mode.
             * @throw SystemException If the source pointer is in Unique mode.
             */
            SmartPointer(const SmartPointer& objOther) : m_pObject(nullptr), m_pnRefCount(nullptr) {
                if (objOther.m_pnRefCount == nullptr && objOther.m_pObject != nullptr) {
                    throw SystemException("Cannot copy a Unique SmartPointer. Use Move semantics or initialize as Shared.");
                }
                m_pObject = objOther.m_pObject;
                m_pnRefCount = objOther.m_pnRefCount;
                if (m_pnRefCount != nullptr) {
                    Internal::AtomicIncrement(m_pnRefCount);
                }
            }

            template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
            SmartPointer(const SmartPointer<U>& objOther) : m_pObject(objOther.m_pObject), m_pnRefCount(objOther.m_pnRefCount) {
                if (objOther.m_pnRefCount == nullptr && objOther.m_pObject != nullptr) {
                    throw SystemException("Cannot copy a Unique SmartPointer. Use Move semantics or initialize as Shared.");
                }
                m_pObject = objOther.m_pObject;
                m_pnRefCount = objOther.m_pnRefCount;
                if (m_pnRefCount != nullptr) {
                    Internal::AtomicIncrement(m_pnRefCount);
                }
            }

            /**
             * @brief Copy assignment operator. Only permitted if the source is in Shared mode.
             */
            SmartPointer& operator=(const SmartPointer& objOther) {
                if (this != &objOther) {
                    if (objOther.m_pnRefCount == nullptr && objOther.m_pObject != nullptr) {
                        throw SystemException("Cannot copy a Unique SmartPointer.");
                    }
                    InternalCleanup();
                    m_pObject = objOther.m_pObject;
                    m_pnRefCount = objOther.m_pnRefCount;
                    if (m_pnRefCount != nullptr) {
                        Internal::AtomicIncrement(m_pnRefCount);
                    }
                }
                return *this;
            }

            template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
            SmartPointer& operator=(const SmartPointer<U>& objOther) {
                if (objOther.m_pnRefCount == nullptr && objOther.m_pObject != nullptr) {
                    throw SystemException("Cannot copy a Unique SmartPointer.");
                }
                InternalCleanup();
                m_pObject = objOther.m_pObject;
                m_pnRefCount = objOther.m_pnRefCount;
                if (m_pnRefCount != nullptr) {
                    Internal::AtomicIncrement(m_pnRefCount);
                }
                return *this;
            }

            // --- Move Semantics ---

            /**
             * @brief Move constructor. Transfers ownership from the source.
             */
            SmartPointer(SmartPointer&& objOther) noexcept 
                : m_pObject(objOther.m_pObject), m_pnRefCount(objOther.m_pnRefCount) {
                objOther.m_pObject = nullptr;
                objOther.m_pnRefCount = nullptr;
            }

            template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
            SmartPointer(SmartPointer<U>&& objOther) noexcept 
                : m_pObject(objOther.m_pObject), m_pnRefCount(objOther.m_pnRefCount) {
                objOther.m_pObject = nullptr;
                objOther.m_pnRefCount = nullptr;
            }

            /**
             * @brief Move assignment operator. Transfers ownership from the source.
             */
            SmartPointer& operator=(SmartPointer&& objOther) noexcept {
                if (this != &objOther) {
                    InternalCleanup();
                    m_pObject = objOther.m_pObject;
                    m_pnRefCount = objOther.m_pnRefCount;
                    objOther.m_pObject = nullptr;
                    objOther.m_pnRefCount = nullptr;
                }
                return *this;
            }

            template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
            SmartPointer& operator=(SmartPointer<U>&& objOther) noexcept {
                if (static_cast<const void*>(this) != static_cast<const void*>(&objOther)) {
                    InternalCleanup();
                    m_pObject = objOther.m_pObject;
                    m_pnRefCount = objOther.m_pnRefCount;
                    objOther.m_pObject = nullptr;
                    objOther.m_pnRefCount = nullptr;
                }
                return *this;
            }

            // --- Factory Methods ---

            /**
             * @brief Creates a Unique SmartPointer, default constructing T.
             */
            static SmartPointer<T> NewUnique() {
                return SmartPointer<T>(new T(), false);
            }

            /**
             * @brief Creates a Unique SmartPointer, forwarding arguments to T's constructor.
             */
            template <typename Arg1, typename... Args>
            static SmartPointer<T> NewUnique(Arg1&& arg1, Args&&... args) {
                return SmartPointer<T>(new T(std::forward<Arg1>(arg1), std::forward<Args>(args)...), false);
            }

            /**
             * @brief Creates a Shared SmartPointer, default constructing T.
             */
            static SmartPointer<T> NewShared() {
                return SmartPointer<T>(new T(), true);
            }

            /**
             * @brief Creates a Shared SmartPointer, forwarding arguments to T's constructor.
             */
            template <typename Arg1, typename... Args>
            static SmartPointer<T> NewShared(Arg1&& arg1, Args&&... args) {
                return SmartPointer<T>(new T(std::forward<Arg1>(arg1), std::forward<Args>(args)...), true);
            }

            // --- Static Factory Helpers (C#-like instantiation) ---

            /**
             * @brief Creates a new SmartPointer (default construction).
             */
            static SmartPointer<T> New() {
                return NewUnique();
            }

            /**
             * @brief Creates a new SmartPointer with variadic arguments for T's constructor.
             */
            template <typename Arg1, typename... Args>
            static SmartPointer<T> New(Arg1&& arg1, Args&&... args) {
                return NewUnique(std::forward<Arg1>(arg1), std::forward<Args>(args)...);
            }

            // --- Conversion / Compatibility Aliases ---

            /**
             * @brief Alias for NewUnique (default construction).
             */
            static SmartPointer<T> MakeUnique() {
                return NewUnique();
            }

            /**
             * @brief Alias for NewUnique. Provided for compatibility.
             */
            template <typename Arg1, typename... Args>
            static SmartPointer<T> MakeUnique(Arg1&& arg1, Args&&... args) {
                return NewUnique(std::forward<Arg1>(arg1), std::forward<Args>(args)...);
            }

            /**
             * @brief Alias for NewShared (default construction).
             */
            static SmartPointer<T> MakeShared() {
                return NewShared();
            }

            /**
             * @brief Alias for NewShared. Provided for compatibility.
             */
            template <typename Arg1, typename... Args>
            static SmartPointer<T> MakeShared(Arg1&& arg1, Args&&... args) {
                return NewShared(std::forward<Arg1>(arg1), std::forward<Args>(args)...);
            }

            // --- API Methods ---

            /**
             * @brief Attaches a new raw pointer to the SmartPointer.
             * @param pPtr The new pointer to manage.
             * @param bIsShared Ownership mode for the new pointer.
             */
            void Attach(T* pPtr, bool bIsShared = false) {
                Reset(pPtr, bIsShared);
            }

            // --- Utility Methods ---

            /**
             * @brief Resets the SmartPointer to null or a new object in Unique mode.
             * @param pPtr Optional new raw pointer to manage.
             */
            void Reset(T* pPtr = nullptr) {
                InternalCleanup();
                m_pObject = pPtr;
                m_pnRefCount = nullptr;
            }

            /**
             * @brief Resets the SmartPointer with a specific ownership mode.
             * @param pPtr The raw pointer to manage.
             * @param bIsShared If true, enables reference counting.
             */
            void Reset(T* pPtr, bool bIsShared) {
                InternalCleanup();
                m_pObject = pPtr;
                m_pnRefCount = (bIsShared && pPtr != nullptr) ? new long(1) : nullptr;
                SetupEnableSharedFromThis(m_pObject);
            }

            /**
             * @brief Detaches the managed object and returns it.
             * The SmartPointer will no longer own the object.
             * @return The raw pointer to the object.
             */
            T* Detach() {
                T* pTemp = m_pObject;
                m_pnRefCount = nullptr;
                m_pObject = nullptr;
                return pTemp;
            }

            /**
             * @brief Gets the raw pointer.
             */
            T* Get() const noexcept { return m_pObject; }

            /**
             * @brief Checks if the SmartPointer is null.
             */
            bool IsNull() const noexcept { return m_pObject == nullptr; }

            /**
             * @brief Dynamically casts the managed pointer to another type U and returns a new SmartPointer<U> sharing ownership.
             */
            template <typename U>
            SmartPointer<U> DynamicCast() const {
                U* pCast = dynamic_cast<U*>(m_pObject);
                if (!pCast) return SmartPointer<U>(nullptr);
                return SmartPointer<U>(pCast, m_pnRefCount);
            }

            /**
             * @brief Statically casts the managed pointer to another type U and returns a new SmartPointer<U> sharing ownership.
             */
            template <typename U>
            SmartPointer<U> StaticCast() const {
                U* pCast = static_cast<U*>(m_pObject);
                if (!pCast) return SmartPointer<U>(nullptr);
                return SmartPointer<U>(pCast, m_pnRefCount);
            }

            /**
             * @brief Const casts the managed pointer to another type U and returns a new SmartPointer<U> sharing ownership.
             */
            template <typename U>
            SmartPointer<U> ConstCast() const {
                U* pCast = const_cast<U*>(m_pObject);
                if (!pCast) return SmartPointer<U>(nullptr);
                return SmartPointer<U>(pCast, m_pnRefCount);
            }

            template <typename From>
            static SmartPointer<T> DynamicCast(const SmartPointer<From>& sp) {
                return sp.template DynamicCast<T>();
            }

            template <typename From>
            static SmartPointer<T> StaticCast(const SmartPointer<From>& sp) {
                return sp.template StaticCast<T>();
            }

            template <typename From>
            static SmartPointer<T> ConstCast(const SmartPointer<From>& sp) {
                return sp.template ConstCast<T>();
            }

            /**
             * @brief Gets the current reference count. Returns 0 for Unique or Null pointers.
             */
            int GetRefCount() const noexcept {
                return (m_pnRefCount != nullptr) ? static_cast<int>(*m_pnRefCount) : 0;
            }

            // --- Operators ---

            T& operator*() const { return *m_pObject; }
            T* operator->() const noexcept { return m_pObject; }
            explicit operator bool() const noexcept { return m_pObject != nullptr; }

            bool operator==(const SmartPointer& other) const noexcept {
                return m_pObject == other.m_pObject;
            }

            bool operator!=(const SmartPointer& other) const noexcept {
                return m_pObject != other.m_pObject;
            }

            template <typename U>
            bool operator==(const SmartPointer<U>& other) const noexcept {
                return m_pObject == other.Get();
            }

            template <typename U>
            bool operator!=(const SmartPointer<U>& other) const noexcept {
                return m_pObject != other.Get();
            }

            bool operator==(std::nullptr_t) const noexcept {
                return m_pObject == nullptr;
            }

            bool operator!=(std::nullptr_t) const noexcept {
                return m_pObject != nullptr;
            }

            friend bool operator==(std::nullptr_t, const SmartPointer& sp) noexcept {
                return sp.m_pObject == nullptr;
            }

            friend bool operator!=(std::nullptr_t, const SmartPointer& sp) noexcept {
                return sp.m_pObject != nullptr;
            }

            template <typename U>
            bool operator==(const U* pOther) const noexcept {
                return m_pObject == pOther;
            }

            template <typename U>
            bool operator!=(const U* pOther) const noexcept {
                return m_pObject != pOther;
            }

            template <typename U>
            friend bool operator==(const U* pOther, const SmartPointer& sp) noexcept {
                return pOther == sp.m_pObject;
            }

            template <typename U>
            friend bool operator!=(const U* pOther, const SmartPointer& sp) noexcept {
                return pOther != sp.m_pObject;
            }

        private:
            SmartPointer(T* pPtr, volatile long* pnRefCount)
                : m_pObject(pPtr), m_pnRefCount(pnRefCount) {
                if (m_pnRefCount != nullptr) {
                    Internal::AtomicIncrement(m_pnRefCount);
                }
                SetupEnableSharedFromThis(m_pObject);
            }

            template <typename U>
            void SetupEnableSharedFromThis(U* pPtr) {
                if constexpr (IsComplete<U>::value) {
                    if (pPtr != nullptr && m_pnRefCount != nullptr) {
                        if constexpr (std::is_base_of_v<EnableSharedFromThisBase, U>) {
                            static_cast<EnableSharedFromThisBase*>(pPtr)->m_pnRefCount = m_pnRefCount;
                        } else if constexpr (std::is_polymorphic_v<U>) {
                            if (auto* pBase = dynamic_cast<EnableSharedFromThisBase*>(pPtr)) {
                                pBase->m_pnRefCount = m_pnRefCount;
                            }
                        }
                    }
                }
            }

            void InternalCleanup() {
                /// Shared ownership mode: atomically decrement reference counter and free if zero.
                if (m_pnRefCount != nullptr) {
                    if (Internal::AtomicDecrement(m_pnRefCount) == 0) {
                        if (m_pObject != nullptr) delete m_pObject;
                        delete const_cast<long*>(m_pnRefCount);
                    }
                } else if (m_pObject != nullptr) {
                    /// Unique ownership mode: directly delete managed object without atomic overhead.
                    delete m_pObject;
                }
                m_pObject = nullptr;
                m_pnRefCount = nullptr;
            }

            T* m_pObject;
            volatile long* m_pnRefCount;
        };

        template <typename T>
        class EnableSharedFromThis : public EnableSharedFromThisBase {
        public:
            SmartPointer<T> SharedFromThis() {
                if (m_pnRefCount == nullptr) {
                    throw SystemException("SharedFromThis called on an object that is not managed by a shared SmartPointer.");
                }
                return SmartPointer<T>(static_cast<T*>(this), m_pnRefCount);
            }

            SmartPointer<const T> SharedFromThis() const {
                if (m_pnRefCount == nullptr) {
                    throw SystemException("SharedFromThis called on an object that is not managed by a shared SmartPointer.");
                }
                return SmartPointer<const T>(static_cast<const T*>(this), m_pnRefCount);
            }

        protected:
            constexpr EnableSharedFromThis() noexcept = default;
            EnableSharedFromThis(const EnableSharedFromThis&) noexcept : EnableSharedFromThisBase() {}
            EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept { return *this; }
            virtual ~EnableSharedFromThis() = default;
        };

        template <typename To, typename From>
        inline SmartPointer<To> DynamicPointerCast(const SmartPointer<From>& sp) {
            return sp.template DynamicCast<To>();
        }

        template <typename To, typename From>
        inline SmartPointer<To> StaticPointerCast(const SmartPointer<From>& sp) {
            return sp.template StaticCast<To>();
        }

        template <typename To, typename From>
        inline SmartPointer<To> ConstPointerCast(const SmartPointer<From>& sp) {
            return sp.template ConstCast<To>();
        }

        template <typename To, typename From>
        inline SmartPointer<To> DynamicCast(const SmartPointer<From>& sp) {
            return sp.template DynamicCast<To>();
        }

        template <typename To, typename From>
        inline SmartPointer<To> StaticCast(const SmartPointer<From>& sp) {
            return sp.template StaticCast<To>();
        }
    }
}
