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

#include "cpputils/cstring.h"

#ifdef _WIN32
  #include <Shlwapi.h>
  #pragma comment(lib, "ShLwApi.Lib")
#endif

namespace
{
    char* find(char* const str, const char* const value, const std::size_t start, std::size_t end,
      const bool ignore_case) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);
        const auto str_len = std::strlen(str);

        if (end > str_len) {
            end = str_len;
        }

        if ((*value != cpputils::EOS) && (start < end)) {
#ifdef _WIN32
            auto* const result = ignore_case ? ::StrStrI(str + start, value) : std::strstr(str + start, value);
#else
            auto* const result = ignore_case ? ::strcasestr(str + start, value) : std::strstr(str + start, value);
#endif
            if ((result != nullptr) && (end < str_len)) {
                if (const auto pos = static_cast<std::size_t>(result - str); (pos + std::strlen(value)) > end) {
                    return nullptr;
                }
            }

            return result;
        }

        return nullptr;
    }

    char* rfind(char* const str, const char* const value, std::size_t start, const std::size_t end,
      const bool ignore_case) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        const auto value_len = std::strlen(value);
        char* result = nullptr;
        char* found = nullptr;

        while ((found = find(str, value, start, end, ignore_case)) != nullptr) {
            result = found;
            start += value_len;
        }

        return result;
    }
}

namespace cpputils
{
    char* find(char* const str, const char* const value, const std::size_t start, const std::size_t end) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return ::find(str, value, start, end, false);
    }

    char* find_ignore_case(
      char* const str, const char* const value, const std::size_t start, const std::size_t end) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return ::find(str, value, start, end, true);
    }

    char* rfind(char* const str, const char* const value, const std::size_t start, const std::size_t end) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return ::rfind(str, value, start, end, false);
    }

    char* rfind_ignore_case(
      char* const str, const char* const value, const std::size_t start, const std::size_t end) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return ::rfind(str, value, start, end, true);
    }
}
