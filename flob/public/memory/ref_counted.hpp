#pragma once

#include "core_types.hpp"

#include <atomic>

namespace flob
{
    template <typename T>
    class RefCounted
    {
    public:
        RefCounted() noexcept;

        RefCounted(const RefCounted&) = delete;
        RefCounted(RefCounted&&)      = delete;

        RefCounted& operator=(const RefCounted&) = delete;
        RefCounted& operator=(RefCounted&&)      = delete;

    public:
        auto retain() const -> void;
        auto release() const -> void;

        [[nodiscard]] auto retain_count() const -> uint32;

    protected:
        ~RefCounted() = default;

    private:
        mutable std::atomic<uint32> _retain_count;
    };

    //==============================================================================================
    // class : RefCounted
    //==============================================================================================

    template <typename T>
    RefCounted<T>::RefCounted() noexcept
        : _retain_count(1)
    {}

    template <typename T>
    auto RefCounted<T>::retain() const -> void
    {
        _retain_count.fetch_add(1, std::memory_order_relaxed);
    }

    template <typename T>
    auto RefCounted<T>::release() const -> void
    {
        if (_retain_count.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            delete static_cast<const T*>(this);
        }
    }

    template <typename T>
    auto RefCounted<T>::retain_count() const -> uint32
    {
        return _retain_count.load(std::memory_order_relaxed);
    }
}
