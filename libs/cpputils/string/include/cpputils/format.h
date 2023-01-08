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

#include <fmt/core.h>
#include <cstdio>
#include <string>
#include <utility>

namespace cpputils
{
    template <typename... Args>
    std::string format(std::string format, Args&&... args)
    {
        return fmt::format(std::move(format), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void print(std::string format, Args&&... args)
    {
        fmt::print(std::move(format), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void print(std::FILE* const file, std::string format, Args&&... args)
    {
        fmt::print(file, std::move(format), std::forward<Args>(args)...);
    }
}
