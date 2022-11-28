/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/object_list.h"
#include <cstdlib>
#include <iterator>

namespace rehlds::common
{
    void ObjectList::init()
    {
        current_ = 0;
        clear(false);
    }

    bool ObjectList::add(void* const object)
    {
        return add_tail(object);
    }

    bool ObjectList::remove(void* const object)
    {
        const auto& it_end = container_.end();

        for (auto it = container_.begin(); it != it_end; ++it) {
            if (*it != object) {
                continue;
            }

            if (current_ > 0) {
                if (const auto index = static_cast<std::size_t>(std::distance(container_.begin(), it));
                    index < current_) {
                    --current_;
                }
            }

            container_.erase(it);

            return true;
        }

        return false;
    }

    void ObjectList::clear(const bool free_elements_memory)
    {
        if (free_elements_memory) {
            for (auto* const element : container_) {
                std::free(element); // NOLINT(cppcoreguidelines-no-malloc)
            }
        }

        current_ = 0;
        container_.clear();
    }

    void* ObjectList::first()
    {
        void* object = nullptr;

        if (container_.empty()) {
            current_ = 0;
        }
        else {
            current_ = 1;
            object = container_.front();
        }

        return object;
    }

    void* ObjectList::next()
    {
        if (current_ >= container_.size()) {
            return nullptr;
        }

        auto* const object = container_[current_];
        ++current_;

        return object;
    }

    std::size_t ObjectList::size() const
    {
        return container_.size();
    }

    bool ObjectList::contains(void* const object)
    {
        if (const auto& it = std::find(container_.cbegin(), container_.cend(), object); it != container_.cend()) {
            current_ = static_cast<std::size_t>(std::distance(container_.cbegin(), it));
            return true;
        }

        return false;
    }

    bool ObjectList::empty() const
    {
        return container_.empty();
    }

    bool ObjectList::add_head(void* const object)
    {
        container_.push_front(object);

        if (current_ != 0) {
            ++current_;
        }

        return true;
    }

    bool ObjectList::add_tail(void* const object)
    {
        container_.push_back(object);
        return true;
    }

    void* ObjectList::remove_head()
    {
        if (container_.empty()) {
            return nullptr;
        }

        if (current_ > 0) {
            --current_;
        }

        auto* const object = container_.front();
        container_.pop_front();

        return object;
    }

    void* ObjectList::remove_tail()
    {
        if (container_.empty()) {
            return nullptr;
        }

        if ((current_ > 0) && ((container_.size() - 1) == current_)) {
            --current_;
        }

        auto* const object = container_.back();
        container_.pop_back();

        return object;
    }
}
