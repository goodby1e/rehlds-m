/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "platform.h"
#include <cstddef>

namespace rehlds::common
{
    class NO_VTABLE IObjectContainer
    {
      public:
        IObjectContainer() = default;
        IObjectContainer(IObjectContainer&&) = default;
        IObjectContainer(const IObjectContainer&) = default;
        IObjectContainer& operator=(IObjectContainer&&) = default;
        IObjectContainer& operator=(const IObjectContainer&) = default;
        STACK_ALIGN virtual ~IObjectContainer() = default;

        virtual void init() = 0;
        virtual bool add(void* object) = 0;
        virtual bool remove(void* object) = 0;
        virtual void clear(bool free_elements_memory) = 0;
        virtual void* first() = 0;
        virtual void* next() = 0;
        [[nodiscard]] virtual std::size_t size() const = 0;
        virtual bool contains(void* object) = 0;
        [[nodiscard]] virtual bool empty() const = 0;
    };
}
