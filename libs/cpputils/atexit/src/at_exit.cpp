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

#include "cpputils/at_exit.h"
#include <cstdlib>
#include <memory>
#include <stack>

namespace
{
    std::unique_ptr<std::stack<std::function<void()>>> at_exit_callbacks{};

    constexpr auto* RUN_EXIT_CALLBACKS = +[]
    {
        if (nullptr == at_exit_callbacks) {
            return;
        }

        while (!at_exit_callbacks->empty()) {
            at_exit_callbacks->top()();
            at_exit_callbacks->pop();
        }

        at_exit_callbacks.reset(nullptr);
    };
}

namespace cpputils
{
    void at_exit(std::function<void()> callback)
    {
        if (nullptr == at_exit_callbacks) {
            at_exit_callbacks = std::make_unique<std::stack<std::function<void()>>>();

            // NOLINTNEXTLINE(cert-err33-c)
            std::atexit(RUN_EXIT_CALLBACKS); // cppcheck-suppress ignoredReturnValue

            // NOLINTNEXTLINE(cert-err33-c)
            std::at_quick_exit(RUN_EXIT_CALLBACKS); // cppcheck-suppress ignoredReturnValue
        }

        at_exit_callbacks->emplace(std::move(callback));
    }
}
