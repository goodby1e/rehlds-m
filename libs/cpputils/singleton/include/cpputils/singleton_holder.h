/*
 *  Copyright (C) 2023 the_hunter
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "cpputils/at_exit.h"
#include <cassert>
#include <csignal>
#include <iostream>
#include <memory>
#include <utility>

namespace cpputils::singleton
{
    /**
     * @brief Implementation of the \c CreationPolicy used by \c SingletonHolder.
     *
     * Creates objects with a \c std::make_unique<T> function.
     */
    template <typename T>
    class CreateUnique
    {
      public:
        using InstanceType = std::unique_ptr<T>;

        template <typename... Args>
        [[nodiscard]] static InstanceType create(Args&&... args)
        {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        static void destroy(InstanceType& instance) noexcept
        {
            instance.reset(nullptr);
        }
    };

    /**
     * @brief Implementation of the \c LifetimePolicy used by \c SingletonHolder.
     *
     * Schedules an object's destruction as per C++ rules.
     * Forwards to \c at_exit.
     */
    template <typename T>
    class DefaultLifetime
    {
      public:
        template <typename FuncDestroy>
        static void schedule_destruction(T&, FuncDestroy&& destroy)
        {
            at_exit(std::forward<FuncDestroy>(destroy));
        }

        static void on_dead_reference()
        {
            std::cout << "FATAL ERROR: Dead reference detected.\n" << std::flush;
            std::raise(SIGABRT); // NOLINT(cert-err33-c)
        }
    };

    /**
     * @brief Implementation of the \c LifetimePolicy used by \c SingletonHolder.
     *
     * Never destroys the object.
     */
    template <typename T>
    class NoDestroy
    {
      public:
        template <typename FuncDestroy>
        static void schedule_destruction(T*, FuncDestroy&&) noexcept
        {
        }

        static void on_dead_reference() noexcept
        {
        }
    };
}

namespace cpputils
{
    /**
     * @brief Provides Singleton amenities for a type \c T.
     */
    template <typename T, template <typename> typename CreationPolicy = singleton::CreateUnique,
      template <typename> typename LifetimePolicy = singleton::DefaultLifetime>
    class SingletonHolder
    {
      public:
        template <typename... Args>
        [[nodiscard]] static T& get_instance(Args&&... args);

      private:
        static inline typename CreationPolicy<T>::InstanceType instance{};
        static inline bool destroyed{};

        SingletonHolder() = default;
        static void destroy_singleton();
    };

    template <typename T, template <typename> class CreationPolicy, template <typename> class LifetimePolicy>
    template <typename... Args>
    T& SingletonHolder<T, CreationPolicy, LifetimePolicy>::get_instance(Args&&... args)
    {
        if (nullptr == instance) {
            if (destroyed) {
                LifetimePolicy<T>::on_dead_reference();
                destroyed = false;
            }

            instance = CreationPolicy<T>::create(std::forward<Args>(args)...);
            LifetimePolicy<T>::schedule_destruction(*instance, &destroy_singleton);
        }

        return *instance;
    }

    template <typename T, template <typename> class CreationPolicy, template <typename> class LifetimePolicy>
    void SingletonHolder<T, CreationPolicy, LifetimePolicy>::destroy_singleton()
    {
        assert(!destroyed);
        CreationPolicy<T>::destroy(instance);
        destroyed = true;
    }
}
