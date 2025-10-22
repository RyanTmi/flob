#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace flob
{
    template <typename T>
    concept RefCountable = requires(T t) {
        { t.retain() } -> std::same_as<void>;
        { t.release() } -> std::same_as<void>;
    };

    template <RefCountable T>
    class Ref final
    {
    public:
        constexpr Ref() noexcept;
        constexpr Ref(std::nullptr_t) noexcept;

        template <RefCountable U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        constexpr Ref(U* ptr) noexcept;

        ~Ref() noexcept;

        constexpr Ref(const Ref& other) noexcept;
        constexpr Ref(Ref&& other) noexcept;

        template <RefCountable U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        constexpr Ref(const Ref<U>& other) noexcept;

        template <RefCountable U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        constexpr Ref(Ref<U>&& other) noexcept;

        auto operator=(const Ref& other) noexcept -> Ref&;
        auto operator=(Ref&& other) noexcept -> Ref&;

        template <RefCountable U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        auto operator=(const Ref<U>& other) noexcept -> Ref&;

        template <RefCountable U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        auto operator=(Ref<U>&& other) noexcept -> Ref&;

    public:
        auto operator*() const noexcept -> T&;
        auto operator->() const noexcept -> T*;

        operator bool() const noexcept;

        auto operator!=(const Ref& other) const noexcept;

        auto get() const noexcept -> T*;

        auto reset(T* ptr = nullptr) noexcept -> void;
        auto swap(Ref& other) noexcept -> void;

        auto detach() noexcept -> T*;

        template <RefCountable U>
        auto as() const noexcept -> Ref<U>
        {
            auto ref = Ref<U>(static_cast<U*>(_ptr));
            ref.retain();
            return ref;
        }

        template <RefCountable U>
        auto cast_ptr() const noexcept -> U*
        {
            return static_cast<U*>(_ptr);
        }

    private:
        template <RefCountable U>
        friend class Ref;

        auto retain() noexcept -> void;
        auto release() noexcept -> void;

    private:
        T* _ptr;
    };

    template <RefCountable T, typename... Args>
    auto make_ref(Args&&... args) -> Ref<T>
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }

    //==============================================================================================
    // class : Ref
    //==============================================================================================

    template <RefCountable T>
    constexpr Ref<T>::Ref() noexcept
        : _ptr(nullptr)
    {}

    template <RefCountable T>
    constexpr Ref<T>::Ref(std::nullptr_t) noexcept
        : _ptr(nullptr)
    {}

    template <RefCountable T>
    template <RefCountable U, typename>
    constexpr Ref<T>::Ref(U* ptr) noexcept
        : _ptr(reinterpret_cast<T*>(ptr))
    {}

    template <RefCountable T>
    Ref<T>::~Ref() noexcept
    {
        release();
    }

    template <RefCountable T>
    constexpr Ref<T>::Ref(const Ref& other) noexcept
        : _ptr(other._ptr)
    {
        retain();
    }

    template <RefCountable T>
    constexpr Ref<T>::Ref(Ref&& other) noexcept
        : _ptr(other._ptr)
    {
        other._ptr = nullptr;
    }

    template <RefCountable T>
    template <RefCountable U, typename>
    constexpr Ref<T>::Ref(const Ref<U>& other) noexcept
        : _ptr(reinterpret_cast<T*>(other._ptr))
    {
        retain();
    }

    template <RefCountable T>
    template <RefCountable U, typename>
    constexpr Ref<T>::Ref(Ref<U>&& other) noexcept
        : _ptr(reinterpret_cast<T*>(other._ptr))
    {
        other._ptr = nullptr;
    }

    template <RefCountable T>
    auto Ref<T>::operator=(const Ref& other) noexcept -> Ref&
    {
        if (this != &other)
        {
            other.retain();
            release();
            _ptr = other._ptr;
        }
        return *this;
    }

    template <RefCountable T>
    auto Ref<T>::operator=(Ref&& other) noexcept -> Ref&
    {
        if (this != &other)
        {
            release();
            _ptr       = other._ptr;
            other._ptr = nullptr;
        }
        return *this;
    }

    template <RefCountable T>
    template <RefCountable U, typename>
    auto Ref<T>::operator=(const Ref<U>& other) noexcept -> Ref&
    {
        if (this != &other)
        {
            other.retain();
            release();
            _ptr = reinterpret_cast<T*>(other._ptr);
        }
        return *this;
    }

    template <RefCountable T>
    template <RefCountable U, typename>
    auto Ref<T>::operator=(Ref<U>&& other) noexcept -> Ref&
    {
        if (this != &other)
        {
            release();
            _ptr       = reinterpret_cast<T*>(other._ptr);
            other._ptr = nullptr;
        }
        return *this;
    }

    template <RefCountable T>
    auto Ref<T>::get() const noexcept -> T*
    {
        return _ptr;
    }

    template <RefCountable T>
    auto Ref<T>::operator*() const noexcept -> T&
    {
        return *_ptr;
    }

    template <RefCountable T>
    auto Ref<T>::operator->() const noexcept -> T*
    {
        return _ptr;
    }

    template <RefCountable T>
    Ref<T>::operator bool() const noexcept
    {
        return _ptr != nullptr;
    }

    template <RefCountable T>
    auto Ref<T>::operator!=(const Ref& other) const noexcept
    {
        return _ptr != other._ptr;
    }

    template <RefCountable T>
    auto Ref<T>::reset(T* ptr) noexcept -> void
    {
        release();
        _ptr = ptr;
    }

    template <RefCountable T>
    auto Ref<T>::swap(Ref& other) noexcept -> void
    {
        std::swap(_ptr, other._ptr);
    }

    template <RefCountable T>
    auto Ref<T>::detach() noexcept -> T*
    {
        T* ptr = _ptr;
        _ptr   = nullptr;
        return ptr;
    }

    template <RefCountable T>
    auto Ref<T>::retain() noexcept -> void
    {
        if (_ptr)
        {
            _ptr->retain();
        }
    }

    template <RefCountable T>
    auto Ref<T>::release() noexcept -> void
    {
        if (_ptr)
        {
            _ptr->release();
            _ptr = nullptr;
        }
    }
}
