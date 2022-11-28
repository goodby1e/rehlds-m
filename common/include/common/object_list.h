/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "object_container.h"
#include "platform.h"
#include <algorithm>
#include <deque>
#include <utility>

namespace rehlds::common
{
    class ObjectList final : public IObjectContainer
    {
      public:
        using Iterator = std::deque<void*>::iterator;
        using ConstIterator = std::deque<void*>::const_iterator;

        STACK_ALIGN void init() override;
        STACK_ALIGN bool add(void* object) override;
        STACK_ALIGN bool remove(void* object) override;
        STACK_ALIGN void clear(bool free_elements_memory) override;
        STACK_ALIGN void* first() override;
        STACK_ALIGN void* next() override;
        [[nodiscard]] STACK_ALIGN std::size_t size() const override;
        STACK_ALIGN bool contains(void* object) override;
        [[nodiscard]] STACK_ALIGN bool empty() const override;

        bool add_head(void* object);
        bool add_tail(void* object);
        void* remove_head();
        void* remove_tail();

        [[nodiscard]] void*& operator[](std::size_t index);
        [[nodiscard]] void*& operator[](int index);
        [[nodiscard]] void* operator[](std::size_t index) const;
        [[nodiscard]] void* operator[](int index) const;

        [[nodiscard]] auto begin() noexcept;
        [[nodiscard]] auto cbegin() const noexcept;
        [[nodiscard]] auto rbegin() noexcept;
        [[nodiscard]] auto crbegin() const noexcept;

        [[nodiscard]] auto end() noexcept;
        [[nodiscard]] auto cend() const noexcept;
        [[nodiscard]] auto rend() noexcept;
        [[nodiscard]] auto crend() const noexcept;

        Iterator erase(const ConstIterator& pos);
        Iterator erase(const ConstIterator& first, const ConstIterator& last);

        template <typename Compare>
        void sort(Compare&& compare);

      private:
        std::size_t current_{};
        std::deque<void*> container_{};
    };

    [[nodiscard]] inline void*& ObjectList::operator[](const std::size_t index)
    {
        return container_[index];
    }

    [[nodiscard]] inline void*& ObjectList::operator[](const int index)
    {
        return operator[](static_cast<std::size_t>(index));
    }

    [[nodiscard]] inline void* ObjectList::operator[](const std::size_t index) const
    {
        return container_[index];
    }

    [[nodiscard]] inline void* ObjectList::operator[](const int index) const
    {
        return operator[](static_cast<std::size_t>(index));
    }

    [[nodiscard]] inline auto ObjectList::begin() noexcept
    {
        return container_.begin();
    }

    [[nodiscard]] inline auto ObjectList::cbegin() const noexcept
    {
        return container_.cbegin();
    }

    [[nodiscard]] inline auto ObjectList::rbegin() noexcept
    {
        return container_.rbegin();
    }

    [[nodiscard]] inline auto ObjectList::crbegin() const noexcept
    {
        return container_.crbegin();
    }

    [[nodiscard]] inline auto ObjectList::end() noexcept
    {
        return container_.end();
    }

    [[nodiscard]] inline auto ObjectList::cend() const noexcept
    {
        return container_.cend();
    }

    [[nodiscard]] inline auto ObjectList::rend() noexcept
    {
        return container_.rend();
    }

    [[nodiscard]] inline auto ObjectList::crend() const noexcept
    {
        return container_.crend();
    }

    inline ObjectList::Iterator ObjectList::erase(const ConstIterator& pos)
    {
        current_ = 0;
        return container_.erase(pos);
    }

    inline ObjectList::Iterator ObjectList::erase(const ConstIterator& first, const ConstIterator& last)
    {
        current_ = 0;
        return container_.erase(first, last);
    }

    template <typename Compare>
    void ObjectList::sort(Compare&& compare)
    {
        current_ = 0;
        std::sort(container_.begin(), container_.end(), std::forward<Compare>(compare));
    }
}
