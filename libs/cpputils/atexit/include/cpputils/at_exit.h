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

#include <functional>

namespace cpputils
{
    /**
     * @brief Registers the callback to be called on normal and quick program termination
     * (via \c std::exit(), \c std::quick_exit() or returning from the main function).
     *
     * @param callback Lambda to be called on normal program termination.
     */
    void at_exit(std::function<void()> callback);
}
