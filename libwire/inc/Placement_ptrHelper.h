#ifndef PLACEMENT_PTRHELPER_H
#define PLACEMENT_PTRHELPER_H

#include <type_traits>

#include "Placement_ptr.h"

// template<typename Base>
// struct is_base_of_multiple<Base> {
//     constexpr static bool value = true;
// };
//
// template<typename Base, typename OneDerived, typename... Derived>
// struct is_base_of_multiple<Base, OneDerived, Derived...> {
//     constexpr static bool value = std::is_base_of<Base, OneDerived>::value && is_base_of_multiple<Base, Derived...>::value;
// };
//
// template<typename Expr, typename First, typename... Derived>
// constexpr auto m_expr_of_multiple(First a, Derived&&... args) {
//     return Expr(a, m_expr_of_multiple<Expr>(std::forward<Derived>(args)...));
// }
/*
template<typename Expr, typename First, typename Second>
constexpr auto m_expr_of_multiple(First a, Second b) {
    return Expr(a, b);
}*/

template<typename T>
constexpr T&& vmin(T&& val)
{
    return std::forward<T>(val);
}

template<typename T0, typename T1, typename... Ts>
constexpr auto vmin(T0&& val1, T1&& val2, Ts&&... vs)
{
    return (val1 < val2) ?
    vmin(val1, std::forward<Ts>(vs)...) :
    vmin(val2, std::forward<Ts>(vs)...);
}

template<typename T>
constexpr T&& vmax(T&& val)
{
    return std::forward<T>(val);
}

template<typename T0, typename T1, typename... Ts>
constexpr auto vmax(T0&& val1, T1&& val2, Ts&&... vs)
{
    return (val1 > val2) ?
    vmax(val1, std::forward<Ts>(vs)...) :
    vmax(val2, std::forward<Ts>(vs)...);
}

template<typename Base, typename... Derived>
struct Placement_ptrBaseAndDerivedInfos {
    constexpr Placement_ptrBaseAndDerivedInfos() {
//         static_assert(is_base_of_multiple<Base, Derived...>::value, "All class must be derived from base class.");
    }
    using BaseClass = Base;
    constexpr static size_t StoreSize = vmax(sizeof(Base), sizeof(Derived)...);
    constexpr static size_t StoreAlign = vmin(alignof(Base), alignof(Derived)...);

    using PlacementPtrType = placement_ptr<BaseClass, StoreSize, StoreAlign>;
};

#endif // PLACEMENT_PTRHELPER_H
