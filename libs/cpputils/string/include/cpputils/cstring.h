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

#include "cpputils/string_const.h"
#include <cassert>
#include <cctype>
#include <climits>
#include <cstddef>
#include <cstring>

namespace cpputils
{
    /**
     * @brief Compares two strings lexicographically (case insensitive).
     *
     * @return Negative value if lhs appears before rhs in lexicographical order.
     * Zero if lhs and rhs compare equal.
     * Positive value if lhs appears after rhs in lexicographical order.
     */
    [[nodiscard]] inline int compare_ignore_case(const char* const lhs, const char* const rhs) noexcept
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);

#ifdef _WIN32
        return ::_stricmp(lhs, rhs);
#else
        return ::strcasecmp(lhs, rhs);
#endif
    }

    /**
     * @brief Compares two strings lexicographically (case insensitive).
     *
     * @return Negative value if lhs appears before rhs in lexicographical order.
     * Zero if lhs and rhs compare equal.
     * Positive value if lhs appears after rhs in lexicographical order.
     */
    [[nodiscard]] inline int compare_ignore_case(
      const char* const lhs, const char* const rhs, const std::size_t count) noexcept
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);

#ifdef _WIN32
        return ::_strnicmp(lhs, rhs, count);
#else
        return ::strncasecmp(lhs, rhs, count);
#endif
    }

    /**
     * @brief Determines whether two strings have the same value.
     */
    [[nodiscard]] inline bool equal(const char* const lhs, const char* const rhs) noexcept
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);

        return 0 == std::strcmp(lhs, rhs);
    }

    /**
     * @brief Determines whether two characters have the same value (case insensitive).
     */
    [[nodiscard]] inline bool equal_ignore_case(const char lhs, const char rhs) noexcept
    {
        return (lhs == rhs) ||
               (std::toupper(static_cast<unsigned char>(lhs)) == std::toupper(static_cast<unsigned char>(rhs)));
    }

    /**
     * @brief Determines whether two strings have the same value (case insensitive).
     */
    [[nodiscard]] inline bool equal_ignore_case(const char* const lhs, const char* const rhs) noexcept
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);

        return 0 == compare_ignore_case(lhs, rhs);
    }

    /**
     * @brief Searches the string for a specified value and returns the position of where it was found.
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the first occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] char* find(char* str, const char* value, std::size_t start = 0, std::size_t end = UINT_MAX) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the position of where it was found.
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the first occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] inline const char* find(const char* const str, const char* const value, const std::size_t start = 0,
      const std::size_t end = UINT_MAX) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return find(const_cast<char*>(str), value, start, end);
    }

    /**
     * @brief Searches the string for a specified value and returns the position
     * of where it was found (case insensitive).
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the first occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] char* find_ignore_case(
      char* str, const char* value, std::size_t start = 0, std::size_t end = UINT_MAX) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the position
     * of where it was found (case insensitive).
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the first occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] inline const char* find_ignore_case(const char* const str, const char* const value,
      const std::size_t start = 0, const std::size_t end = UINT_MAX) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return find_ignore_case(const_cast<char*>(str), value, start, end);
    }

    /**
     * @brief Searches the string for a specified value and returns the last position of where it was found.
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the last occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] char* rfind(char* str, const char* value, std::size_t start = 0, std::size_t end = UINT_MAX) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the last position of where it was found.
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the last occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] inline const char* rfind(const char* const str, const char* const value, const std::size_t start = 0,
      const std::size_t end = UINT_MAX) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return rfind(const_cast<char*>(str), value, start, end);
    }

    /**
     * @brief Searches the string for a specified value and returns the last position
     * of where it was found (case insensitive).
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the last occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] char* rfind_ignore_case(
      char* str, const char* value, std::size_t start = 0, std::size_t end = UINT_MAX) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the last position
     * of where it was found (case insensitive).
     * Returns \c nullptr if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Pointer to the last occurrence of value in string,
     * or \c nullptr if value is not part of string.
     */
    [[nodiscard]] inline const char* rfind_ignore_case(const char* const str, const char* const value,
      const std::size_t start = 0, const std::size_t end = UINT_MAX) noexcept
    {
        assert(str != nullptr);
        assert(value != nullptr);

        return rfind_ignore_case(const_cast<char*>(str), value, start, end);
    }

    /**
     * @brief Returns \c true if the specified string is \c nullptr or an empty string \c "".
     *
     * @param str String to examine.
     *
     * @return \c true, if the \c str is \c nullptr or empty; otherwise \c false.
     */
    [[nodiscard]] constexpr bool is_null_or_empty(const char* const str) noexcept
    {
        return (nullptr == str) || (EOS == *str);
    }

    /**
     * @brief Returns \c true if the specified string is \c nullptr, empty, or consists only of white-space characters.
     *
     * @param str String to examine.
     *
     * @return \c true, if the \c str is \c nullptr, empty or consists only of white-space characters; otherwise false.
     */
    [[nodiscard]] constexpr bool is_null_or_whitespace(const char* str) noexcept
    {
        if (str != nullptr) {
            while (*str != EOS) {
                if (0 == std::isspace(static_cast<unsigned char>(*str))) {
                    return false;
                }

                ++str;
            }
        }

        return true;
    }
}
