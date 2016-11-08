#ifndef PLACEMENT_PTR_H
#define PLACEMENT_PTR_H

#include <memory>
#include <type_traits>

template<typename _Tp>
struct empty_delete {
    empty_delete() {}

    void operator()(_Tp* __ptr) const {
        static_assert(sizeof(_Tp) > 0, "can't delete pointer to incomplete type");
        (void)__ptr;
    }
};

template<typename Type, size_t TypeSize = sizeof(Type), size_t TypeAlign = alignof(Type)>
class placement_ptr : public std::unique_ptr<Type, empty_delete<Type>> {
    using StoreType = typename std::aligned_storage<TypeSize, TypeAlign>::type;
    template<typename, size_t, size_t> friend class placement_ptr;

public:
    template<typename T = Type, typename... Args,
    typename std::enable_if<std::is_constructible<T, Args...>{}, int>::type = 0>
    placement_ptr(Args&&... args)
    : std::unique_ptr<T, empty_delete<T>>(new (&_store) T(std::forward<Args>(args)...)) {
    }

    placement_ptr()
    : std::unique_ptr<Type, empty_delete<Type>>(nullptr) {
    }

    template<typename OtherType, size_t OtherSize, size_t OtherAlign,
    typename std::enable_if<std::is_convertible<OtherType*, Type*>{} && std::is_same<StoreType, typename std::aligned_storage<OtherSize, OtherAlign>::type>{}, int>::type = 0>
    placement_ptr(placement_ptr<OtherType, OtherSize, OtherAlign>&& o)
    : std::unique_ptr<Type, empty_delete<Type>>(o.get()) {
        std::swap(_store, o._store);
    }

    virtual ~placement_ptr() {
        Type* ptr = (this->get());
        if (ptr != nullptr) {
            ptr->~Type();
        }
    }

    template<typename T = Type>
    void reset(T&& o) {
        std::swap(*this, o);
    }

private:
    StoreType _store;
};

#endif // PLACEMENT_PTR_H
