#pragma once

#include "Common.h"
#include "System/SystemException.h"
#include <atomic>
#include <type_traits>
#include <utility>

namespace DotNetDupe {
    namespace System {
        // Helper trait to check if a type is complete at compile time
        template <typename T, typename = void>
        struct IsComplete : std::false_type {};

        template <typename T>
        struct IsComplete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

        /**
         * @brief A unified Smart Pointer that supports both unique and shared ownership semantics.
         * 
         * Improvised Interface:
         * - SmartPointer<T> p;       -> Automatically allocates new T() (if T is not abstract). Unique ownership.
         * - SmartPointer<T> p(true); -> Automatically allocates new T() and enables Shared ownership.
         * - SmartPointer<T> p(ptr);  -> Takes ownership of an existing raw pointer. Unique ownership.
         */
        template <typename T>
        class SmartPointer {
            template <typename U>
            friend class SmartPointer;
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
                        m_pnRefCount = nullptr;
                        if (bIsShared) {
                            m_pnRefCount = new int(1);
                        }
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
            SmartPointer(T* pPtr, bool bIsShared) : m_pObject(pPtr), m_pnRefCount(nullptr) {
                if (bIsShared && pPtr != nullptr) {
                    m_pnRefCount = new int(1);
                }
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
                    (*m_pnRefCount)++;
                }
            }

            template <typename U>
            SmartPointer(const SmartPointer<U>& objOther) : m_pObject(objOther.m_pObject), m_pnRefCount(objOther.m_pnRefCount) {
                if (objOther.m_pnRefCount == nullptr && objOther.m_pObject != nullptr) {
                    throw SystemException("Cannot copy a Unique SmartPointer. Use Move semantics or initialize as Shared.");
                }
                if (m_pnRefCount != nullptr) {
                    (*m_pnRefCount)++;
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
                        (*m_pnRefCount)++;
                    }
                }
                return *this;
            }

            template <typename U>
            SmartPointer& operator=(const SmartPointer<U>& objOther) {
                if (objOther.m_pnRefCount == nullptr && objOther.m_pObject != nullptr) {
                    throw SystemException("Cannot copy a Unique SmartPointer.");
                }
                InternalCleanup();
                m_pObject = objOther.m_pObject;
                m_pnRefCount = objOther.m_pnRefCount;
                if (m_pnRefCount != nullptr) {
                    (*m_pnRefCount)++;
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

            template <typename U>
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

            template <typename U>
            SmartPointer& operator=(SmartPointer<U>&& objOther) noexcept {
                InternalCleanup();
                m_pObject = objOther.m_pObject;
                m_pnRefCount = objOther.m_pnRefCount;
                objOther.m_pObject = nullptr;
                objOther.m_pnRefCount = nullptr;
                return *this;
            }

            // --- Static Factory Helpers (C#-like instantiation) ---

            /**
             * @brief Creates a new SmartPointer with variadic arguments for T's constructor.
             */
            template <typename... Args>
            static SmartPointer<T> New(Args&&... args) {
                return SmartPointer<T>(new T(std::forward<Args>(args)...));
            }

            /**
             * @brief Creates a new Shared SmartPointer with variadic arguments for T's constructor.
             */
            template <typename... Args>
            static SmartPointer<T> NewShared(Args&&... args) {
                return SmartPointer<T>(new T(std::forward<Args>(args)...), true);
            }

            // --- API Methods ---

            /**
             * @brief Attaches a new raw pointer to the SmartPointer.
             * @param pPtr The new pointer to manage.
             * @param bIsShared Ownership mode for the new pointer.
             */
            void Attach(T* pPtr, bool bIsShared = false) {
                InternalCleanup();
                m_pObject = pPtr;
                if (bIsShared && pPtr != nullptr) {
                    m_pnRefCount = new int(1);
                } else {
                    m_pnRefCount = nullptr;
                }
            }

            /**
             * @brief Detaches the managed object and returns it.
             * The SmartPointer will no longer own the object.
             * @return The raw pointer to the object.
             */
            T* Detach() {
                T* pTemp = m_pObject;
                if (m_pnRefCount != nullptr) {
                    // Release this instance's ownership without destroying the object
                    m_pnRefCount = nullptr;
                }
                m_pObject = nullptr;
                return pTemp;
            }

            /**
             * @brief Gets the raw pointer.
             */
            T* Get() const { return m_pObject; }

            /**
             * @brief Checks if the SmartPointer is null.
             */
            bool IsNull() const { return m_pObject == nullptr; }

            /**
             * @brief Dynamically casts the managed pointer to another type U and returns a new SmartPointer<U> sharing ownership.
             */
            template <typename U>
            SmartPointer<U> DynamicCast() const {
                U* pCast = dynamic_cast<U*>(m_pObject);
                if (!pCast) return SmartPointer<U>(nullptr);
                
                SmartPointer<U> spRet(nullptr);
                spRet.m_pObject = pCast;
                spRet.m_pnRefCount = m_pnRefCount;
                if (m_pnRefCount != nullptr) {
                    (*m_pnRefCount)++;
                }
                return spRet;
            }

            /**
             * @brief Gets the current reference count. Returns 0 for Unique or Null pointers.
             */
            int GetRefCount() const {
                return (m_pnRefCount != nullptr) ? *m_pnRefCount : 0;
            }

            // --- Operators ---

            T& operator*() const { return *m_pObject; }
            T* operator->() const { return m_pObject; }
            explicit operator bool() const { return m_pObject != nullptr; }

        private:
            void InternalCleanup() {
                if (m_pObject != nullptr) {
                    if (m_pnRefCount == nullptr) {
                        // Unique mode: Delete immediately
                        delete m_pObject;
                    } else {
                        // Shared mode: Decrement and delete if zero
                        (*m_pnRefCount)--;
                        if (*m_pnRefCount == 0) {
                            delete m_pObject;
                            delete m_pnRefCount;
                        }
                    }
                }
                m_pObject = nullptr;
                m_pnRefCount = nullptr;
            }

            T* m_pObject;
            int* m_pnRefCount;
        };
    }
}
