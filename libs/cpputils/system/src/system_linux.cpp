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

#include "cpputils/system_linux.h"
#include <filesystem>

namespace cpputils
{
    std::string get_executable_absolute_path()
    {
        return canonical(std::filesystem::path{"/proc/self/exe"}).string();
    }

    std::string get_module_absolute_path(const std::string& filename)
    {
        std::filesystem::path path{filename};
        path.make_preferred();

        if (!path.is_absolute()) {
            auto absolute_path = std::filesystem::current_path();
            path = (absolute_path /= path);
        }

        return canonical(path).string();
    }
}
