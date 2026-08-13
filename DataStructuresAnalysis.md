# Container Refactoring Feasibility Analysis

This document analyzes the feasibility of refactoring `PriorityQueue`, `SortedDictionary`, and `SortedSet` to use higher-level components rather than raw pointer arrays, with a focus on eliminating manual memory management while retaining existing performance characteristics.

## 1. `PriorityQueue<TElement, TPriority>` using `List<T>`
**Feasibility:** Highly Feasible and Recommended

*   **Current Implementation:** A binary min-heap backed by a dynamically allocated raw array (`ElementPriorityPair* m_pItems`). Capacity is manually managed via `EnsureCapacity()`.
*   **Proposed Implementation:** A binary min-heap backed by a `List<ElementPriorityPair>`.
*   **Performance Impact:** **None.** Both `List` and raw arrays provide identical $O(1)$ indexed access required for `SiftUp` and `SiftDown` heap operations. Adding elements to the end of a `List` is amortized $O(1)$, exactly matching the current manual `EnsureCapacity()` logic.
*   **Benefits:** Complete elimination of raw pointer manipulations (`m_pItems`, `m_iCapacity`, placement `new`, explicit destructors). The code will become significantly smaller, safer, and entirely self-managing.

## 2. `SortedDictionary<TKey, TValue>` using `Dictionary<TKey, TValue>`
**Feasibility:** Not Recommended (Loss of Order)

*   **Current Implementation:** An ordered dynamic array of `KeyValuePair` items, manually allocated. Lookups use $O(\log N)$ Binary Search. Inserts and Removals use $O(N)$ shifting.
*   **Analysis of using `Dictionary`:** The `Dictionary` we built in Phase 2 is a hash-based container offering $O(1)$ lookups, but it **destroys ordering**. A `SortedDictionary` fundamentally requires iterating keys in sorted order. If we back it with a `Dictionary`, we would have to maintain a separate sorted `List<TKey>`, rendering the $O(1)$ dictionary redundant and doubling memory usage.
*   **Alternative Recommendation (Use `List` instead):** We can replace the raw ordered array in `SortedDictionary` with a **`List<KeyValuePair>`**.
    *   **Performance:** Maintains the exact same $O(\log N)$ binary search lookup and $O(N)$ shifting insert/remove behavior as the current raw pointer implementation.
    *   **Benefits:** Eliminates all raw pointer management and manual capacity shifting while preserving the exact same performance profile and strict ordering.

## 3. `SortedSet<T>` using `HashSet<T>`
**Feasibility:** Not Recommended (Loss of Order)

*   **Current Implementation:** An ordered dynamic array of `T` items. Lookups use $O(\log N)$ Binary Search. Inserts and Removals use $O(N)$ shifting.
*   **Analysis of using `HashSet`:** Just like `Dictionary`, `HashSet` is an unordered $O(1)$ hash table. Using it as the backbone for `SortedSet` defeats the purpose of keeping elements sequentially sorted.
*   **Alternative Recommendation (Use `List` instead):** We can replace the raw ordered array in `SortedSet` with a **`List<T>`**.
    *   **Performance:** Maintains the exact same $O(\log N)$ binary search lookup and $O(N)$ shifting insert/remove behavior as the current raw pointer implementation.
    *   **Benefits:** Eliminates manual capacity handling, explicit destructors, and placement new logic, making it ABI-safe and much cleaner.

---

### Final Proposal
Rather than using hash-based containers for sorted structures (which breaks ordering), we can dramatically simplify `PriorityQueue`, `SortedDictionary`, and `SortedSet` by refactoring all three to use **`List<T>`** as their internal backing store.

Because the current implementations are already using raw contiguous dynamic arrays, replacing them with our new ABI-safe `List<T>` dynamic array will result in **zero performance degradation** while completely eliminating all manual pointer manipulations and placement new/delete operations from these classes.
