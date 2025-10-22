#pragma once

#include "core_types.hpp"

#include <algorithm>
#include <format>
#include <initializer_list>
#include <new>
#include <stdexcept>

namespace flob
{
    template <typename T>
    class Vector
    {
    public:
        //--------------------------------------------------------------------------------------------------------------
        // Constructors
        //--------------------------------------------------------------------------------------------------------------

        constexpr Vector() noexcept;
        constexpr explicit Vector(usize count) noexcept;
        constexpr Vector(usize count, const T& value) noexcept;
        constexpr Vector(std::initializer_list<T> init) noexcept;

        constexpr Vector(const Vector& other) noexcept;
        constexpr Vector(Vector&& other) noexcept;

        constexpr ~Vector() noexcept;

    public:
        constexpr auto operator=(const Vector& other) noexcept -> Vector&;
        constexpr auto operator=(Vector&& other) noexcept -> Vector&;

    public:
        //--------------------------------------------------------------------------------------------------------------
        // Element access
        //--------------------------------------------------------------------------------------------------------------

        constexpr auto               at(usize index) -> T&;
        [[nodiscard]] constexpr auto at(usize index) const -> const T&;

        constexpr auto operator[](usize index) noexcept -> T&;
        constexpr auto operator[](usize index) const noexcept -> const T&;

        constexpr auto               front() noexcept -> T&;
        [[nodiscard]] constexpr auto front() const noexcept -> const T&;

        constexpr auto               back() noexcept -> T&;
        [[nodiscard]] constexpr auto back() const noexcept -> const T&;

        constexpr auto               data() noexcept -> T*;
        [[nodiscard]] constexpr auto data() const noexcept -> const T*;

        //--------------------------------------------------------------------------------------------------------------
        // Iterators
        //--------------------------------------------------------------------------------------------------------------

        constexpr auto               begin() noexcept -> T*;
        [[nodiscard]] constexpr auto begin() const noexcept -> const T*;
        [[nodiscard]] constexpr auto cbegin() const noexcept -> const T*;

        constexpr auto               end() noexcept -> T*;
        [[nodiscard]] constexpr auto end() const noexcept -> const T*;
        [[nodiscard]] constexpr auto cend() const noexcept -> const T*;

        //--------------------------------------------------------------------------------------------------------------
        // Capacity
        //--------------------------------------------------------------------------------------------------------------

        [[nodiscard]] constexpr auto empty() const noexcept -> bool;
        [[nodiscard]] constexpr auto size() const noexcept -> usize;
        [[nodiscard]] constexpr auto capacity() const noexcept -> usize;

        constexpr auto reserve(usize size) noexcept -> void;

        //--------------------------------------------------------------------------------------------------------------
        // Modifiers
        //--------------------------------------------------------------------------------------------------------------

        constexpr auto clear() noexcept -> void;

        constexpr auto erase(const T* first, const T* last) noexcept -> T*;
        constexpr auto erase(const T* pos) noexcept -> T*;

        constexpr auto push_back(const T& value) noexcept -> void;
        constexpr auto push_back(T&& value) noexcept -> void;

        template <typename... Args>
        constexpr auto emplace_back(Args&&... args) noexcept -> T&;

        constexpr auto pop_back() noexcept -> void;

        constexpr auto resize(usize size) noexcept -> void;
        constexpr auto resize(usize size, const T& value) noexcept -> void;

    private:
        static constexpr auto growth_factor = 1.5f;

        auto realloc(usize capacity) -> void;

    private:
        T*    _data;
        usize _size;
        usize _capacity;
    };

    template <typename T>
    constexpr Vector<T>::Vector() noexcept
        : _data(nullptr)
        , _size(0)
        , _capacity(0)
    {}

    template <typename T>
    constexpr Vector<T>::Vector(usize count) noexcept
        : Vector(count, T())
    {}

    template <typename T>
    constexpr Vector<T>::Vector(usize count, const T& value) noexcept
        : _data(nullptr)
        , _size(0)
        , _capacity(0)
    {
        realloc(count);
        _size = count;
        for (usize i = 0; i < _size; ++i)
        {
            new (&_data[i]) T(value);
        }
    }

    template <typename T>
    constexpr Vector<T>::Vector(std::initializer_list<T> init) noexcept
        : _data(nullptr)
        , _size(0)
        , _capacity(0)
    {
        realloc(init.size());
        for (const T& elem : init)
        {
            new (&_data[_size]) T(elem);
            ++_size;
        }
    }

    template <typename T>
    constexpr Vector<T>::Vector(const Vector& other) noexcept
        : _data(nullptr)
        , _size(0)
        , _capacity(0)
    {
        realloc(other._capacity);

        for (usize i = 0; i < other._size; ++i)
        {
            new (&_data[i]) T(other._data[i]);
        }
        _size = other._size;
    }

    template <typename T>
    constexpr Vector<T>::Vector(Vector&& other) noexcept
        : _data(std::move(other._data))
        , _size(other._size)
        , _capacity(other._capacity)
    {
        other._data     = nullptr;
        other._size     = 0;
        other._capacity = 0;
    }

    template <typename T>
    constexpr Vector<T>::~Vector() noexcept
    {
        clear();
        // ::operator delete(_data, _capacity * sizeof(T));
        ::operator delete(_data);
    }

    template <typename T>
    constexpr auto Vector<T>::operator=(const Vector& other) noexcept -> Vector&
    {
        if (this != &other)
        {
            clear();
            reserve(other._capacity);

            for (usize i = 0; i < other._size; ++i)
            {
                new (&_data[i]) T(other._data[i]);
            }
            _size = other._size;
        }
        return *this;
    }

    template <typename T>
    constexpr auto Vector<T>::operator=(Vector&& other) noexcept -> Vector&
    {
        if (this != &other)
        {
            clear();
            // ::operator delete(_data, _capacity * sizeof(T));
            ::operator delete(_data);

            _data     = other._data;
            _size     = other._size;
            _capacity = other._capacity;

            other._data     = nullptr;
            other._size     = 0;
            other._capacity = 0;
        }
        return *this;
    }

    template <typename T>
    constexpr auto Vector<T>::at(usize index) -> T&
    {
        if (index >= _size)
        {
            throw std::out_of_range("Vector::at() index out of range");
        }
        return _data[index];
    }

    template <typename T>
    constexpr auto Vector<T>::at(usize index) const -> const T&
    {
        if (index >= _size)
        {
            throw std::out_of_range("Vector::at() index out of range");
        }
        return _data[index];
    }

    template <typename T>
    constexpr auto Vector<T>::operator[](usize index) noexcept -> T&
    {
        return _data[index];
    }

    template <typename T>
    constexpr auto Vector<T>::operator[](usize index) const noexcept -> const T&
    {
        return _data[index];
    }

    template <typename T>
    constexpr auto Vector<T>::front() noexcept -> T&
    {
        return _data[0];
    }

    template <typename T>
    constexpr auto Vector<T>::front() const noexcept -> const T&
    {
        return _data[0];
    }

    template <typename T>
    constexpr auto Vector<T>::back() noexcept -> T&
    {
        return _data[_size - 1];
    }

    template <typename T>
    constexpr auto Vector<T>::back() const noexcept -> const T&
    {
        return _data[_size - 1];
    }

    template <typename T>
    constexpr auto Vector<T>::data() noexcept -> T*
    {
        return _data;
    }

    template <typename T>
    constexpr auto Vector<T>::data() const noexcept -> const T*
    {
        return _data;
    }

    template <typename T>
    constexpr auto Vector<T>::begin() noexcept -> T*
    {
        return _data;
    }

    template <typename T>
    constexpr auto Vector<T>::begin() const noexcept -> const T*
    {
        return _data;
    }

    template <typename T>
    constexpr auto Vector<T>::cbegin() const noexcept -> const T*
    {
        return _data;
    }

    template <typename T>
    constexpr auto Vector<T>::end() noexcept -> T*
    {
        return _data + _size;
    }

    template <typename T>
    constexpr auto Vector<T>::end() const noexcept -> const T*
    {
        return _data + _size;
    }

    template <typename T>
    constexpr auto Vector<T>::cend() const noexcept -> const T*
    {
        return _data + _size;
    }

    template <typename T>
    constexpr auto Vector<T>::empty() const noexcept -> bool
    {
        return _size == 0;
    }

    template <typename T>
    constexpr auto Vector<T>::size() const noexcept -> usize
    {
        return _size;
    }

    template <typename T>
    constexpr auto Vector<T>::capacity() const noexcept -> usize
    {
        return _capacity;
    }

    template <typename T>
    constexpr auto Vector<T>::reserve(usize size) noexcept -> void
    {
        if (size > _capacity)
        {
            realloc(size);
        }
    }

    template <typename T>
    constexpr auto Vector<T>::clear() noexcept -> void
    {
        for (usize i = 0; i < _size; ++i)
        {
            _data[i].~T();
        }

        _size = 0;
    }

    template <typename T>
    constexpr auto Vector<T>::erase(const T* first, const T* last) noexcept -> T*
    {
        // Compute iterators into our buffer.
        auto write  = begin() + (first - _data);
        auto read   = begin() + (last - _data);
        auto it_end = end();

        // Shift the elements after [first,last) down to `write`.
        for (; read != it_end; ++read, ++write)
        {
            *write = std::move(*read);
        }

        // Destroy the now-unused tail elements.
        auto new_size = write - begin();
        for (auto destroy_it = begin() + new_size; destroy_it != it_end; ++destroy_it)
        {
            destroy_it->~T();
        }

        _size = new_size;
        return begin() + (first - _data);
    }

    template <typename T>
    constexpr auto Vector<T>::erase(const T* pos) noexcept -> T*
    {
        auto idx = pos - _data;
        return erase(begin() + idx, begin() + idx + 1);
    }

    template <typename T>
    constexpr auto Vector<T>::push_back(const T& value) noexcept -> void
    {
        if (_size >= _capacity)
        {
            realloc(_capacity * growth_factor);
        }

        new (&_data[_size]) T(value);
        ++_size;
    }

    template <typename T>
    constexpr auto Vector<T>::push_back(T&& value) noexcept -> void
    {
        if (_size >= _capacity)
        {
            realloc(_capacity * growth_factor);
        }

        new (&_data[_size]) T(std::move(value));
        ++_size;
    }

    template <typename T>
    template <typename... Args>
    constexpr auto Vector<T>::emplace_back(Args&&... args) noexcept -> T&
    {
        if (_size >= _capacity)
        {
            realloc(_capacity * growth_factor);
        }

        new (&_data[_size]) T(std::forward<Args>(args)...);
        return _data[_size++];
    }

    template <typename T>
    constexpr auto Vector<T>::pop_back() noexcept -> void
    {
        if (_size > 0)
        {
            _data[--_size].~T();
        }
    }

    template <typename T>
    constexpr auto Vector<T>::resize(usize size) noexcept -> void
    {
        resize(size, T());
    }

    template <typename T>
    constexpr auto Vector<T>::resize(usize size, const T& value) noexcept -> void
    {
        if (size < _size)
        {
            for (usize i = size; i < _size; ++i)
            {
                _data[i].~T();
            }
        }
        else if (size > _size)
        {
            if (size > _capacity)
            {
                realloc(size);
            }
            for (usize i = _size; i < size; ++i)
            {
                new (&_data[i]) T(value);
            }
        }
        _size = size;
    }

    template <typename T>
    auto Vector<T>::realloc(usize capacity) -> void
    {
        if (capacity == 0)
        {
            capacity = 2;
        }

        T* new_data = static_cast<T*>(::operator new(capacity * sizeof(T)));

        const usize elements_to_copy = (_size < capacity) ? _size : capacity;
        for (usize i = 0; i < elements_to_copy; ++i)
        {
            new (&new_data[i]) T(std::move(_data[i]));
        }

        for (usize i = 0; i < _size; ++i)
        {
            _data[i].~T();
        }
        // ::operator delete(_data, _capacity * sizeof(T));
        ::operator delete(_data);

        _data     = new_data;
        _capacity = capacity;
        _size     = (_size > capacity) ? capacity : _size;
    }
}

template <typename T>
struct std::formatter<flob::Vector<T>>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const flob::Vector<T>& vector, FormatContext& ctx) const
    {
        auto out = ctx.out();
        out      = std::format_to(ctx.out(), "[");
        if (vector.size() > 0)
        {
            auto it = vector.begin();
            out     = std::format_to(out, "{}", *it);
            for (++it; it != vector.end(); ++it)
            {
                out = std::format_to(out, ", {}", *it);
            }
        }
        return std::format_to(out, "]");
    }
};
