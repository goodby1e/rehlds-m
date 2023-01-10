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

#include "cpputils/string.h"
#include <type_traits>
#include <cctype>
#include <cmath>
#include <utility>

namespace
{
    constexpr unsigned NO_FLAGS = 0;
    constexpr unsigned STARTS_WITH_REVERSE = 1 << 0;
    constexpr unsigned STARTS_WITH_IGNORE_CASE = 1 << 1;

    constexpr auto* IS_ALNUM = +[](const std::string::value_type ch) noexcept
    {
        return std::isalnum(static_cast<unsigned char>(ch)) != 0;
    };

    constexpr auto* IS_ALNUM_OR_UNDERSCORE = +[](const std::string::value_type ch) noexcept
    {
        return (std::isalnum(static_cast<unsigned char>(ch)) != 0) || ('_' == ch);
    };

    constexpr auto* IS_ALPHA = +[](const std::string::value_type ch) noexcept
    {
        return std::isalpha(static_cast<unsigned char>(ch)) != 0;
    };

    constexpr auto* IS_ASCII = +[](const std::string::value_type ch) noexcept
    {
#ifdef _WIN32
        return isascii(static_cast<unsigned char>(ch)) != 0;
#else
        return ::isascii(static_cast<unsigned char>(ch)) != 0;
#endif
    };

    constexpr auto* IS_DIGIT = +[](const std::string::value_type ch) noexcept
    {
        return std::isdigit(static_cast<unsigned char>(ch)) != 0;
    };

    constexpr auto IS_EMPTY = [](const auto& container) noexcept
    {
        return std::empty(container);
    };

    constexpr auto* IS_LOWER_OR_NOT_ALPHA = +[](const std::string::value_type ch) noexcept
    {
        return (std::islower(static_cast<unsigned char>(ch)) != 0) ||
               (0 == std::isalpha(static_cast<unsigned char>(ch)));
    };

    constexpr auto* IS_PRINT = +[](const std::string::value_type ch) noexcept
    {
        return std::isprint(static_cast<unsigned char>(ch)) != 0;
    };

    constexpr auto* IS_SPACE = +[](const std::string::value_type ch) noexcept
    {
        return std::isspace(static_cast<unsigned char>(ch)) != 0;
    };

    constexpr auto* IS_UPPER_OR_NOT_ALPHA = +[](const std::string::value_type ch) noexcept
    {
        return (std::isupper(static_cast<unsigned char>(ch)) != 0) ||
               (0 == std::isalpha(static_cast<unsigned char>(ch)));
    };

    constexpr auto* SWAP_CASE = +[](const std::string_view::value_type ch) noexcept
    {
        const auto uch = static_cast<unsigned char>(ch);
        return static_cast<std::string::value_type>(0 == std::isupper(uch) ? std::toupper(uch) : std::tolower(uch));
    };

    constexpr auto* TO_LOWER = +[](const std::string_view::value_type ch) noexcept
    {
        return static_cast<std::string::value_type>(std::tolower(static_cast<unsigned char>(ch)));
    };

    constexpr auto* TO_UPPER = +[](const std::string_view::value_type ch) noexcept
    {
        return static_cast<std::string::value_type>(std::toupper(static_cast<unsigned char>(ch)));
    };

    constexpr auto* TRIM_SPACES = +[](const std::string_view str) noexcept
    {
        return cpputils::trim(str);
    };

    template <typename Container>
    void trim_entries(Container& container)
    {
        std::transform(std::begin(container), std::end(container), std::begin(container), TRIM_SPACES);
    }

    template <typename Container>
    void remove_empty_entries(Container& container)
    {
        container.erase(std::remove_if(container.begin(), container.end(), IS_EMPTY), container.end());
    }

    template <typename Value>
    [[nodiscard]] std::size_t find(
      std::string_view str, const Value value, const std::size_t start, std::size_t end, const bool reverse) noexcept
    {
        if (end > str.length()) {
            end = str.length();
        }

        if constexpr (std::is_same_v<Value, std::string::value_type>) {
            if ((cpputils::EOS == value) || (start >= end)) {
                return std::string::npos;
            }
        }
        else {
            if (value.empty() || (start >= end)) {
                return std::string::npos;
            }
        }

        if ((start > 0) || (end < str.length())) {
            str = str.substr(start, end - start);
        }

        if (const auto pos = reverse ? str.rfind(value) : str.find(value); pos != std::string::npos) {
            return start + pos;
        }

        return std::string::npos;
    }

    [[nodiscard]] std::size_t find_ignore_case(std::string_view str, const std::string::value_type value,
      const std::size_t start, std::size_t end, const bool reverse) noexcept
    {
        if (end > str.length()) {
            end = str.length();
        }

        if ((cpputils::EOS == value) || (start >= end)) {
            return std::string::npos;
        }

        if ((start > 0) || (end < str.length())) {
            str = str.substr(start, end - start);
        }

        const auto is_chars_equal_ignore_case = [value](const std::string_view::value_type ch)
        {
            return cpputils::equal_ignore_case(ch, value);
        };

        if (reverse) {
            if (const auto& found = std::find_if(str.crbegin(), str.crend(), is_chars_equal_ignore_case);
                found != str.crend()) {
                return start + (str.length() - static_cast<std::size_t>(std::distance(str.crbegin(), found)) - 1);
            }
        }
        else {
            if (const auto& found = std::find_if(str.cbegin(), str.cend(), is_chars_equal_ignore_case);
                found != str.cend()) {
                return start + static_cast<std::size_t>(std::distance(str.cbegin(), found));
            }
        }

        return std::string::npos;
    }

    std::tuple<std::string, std::string, std::string> partition(
      const std::string_view str, const std::string_view value, const bool reverse) noexcept
    {
        const auto value_pos = reverse ? str.rfind(value) : str.find(value);

        if ((std::string_view::npos == value_pos) || value.empty()) {
            return std::make_tuple(std::string{str}, std::string{}, std::string{});
        }

        std::string first{str.substr(0, value_pos)};
        std::string second{value};
        std::string third{str.substr(value_pos + value.length())};

        return std::make_tuple(std::move(first), std::move(second), std::move(third));
    }

    std::vector<std::string> split_by_delimiter(
      std::string_view str, const std::string_view delimiter, std::size_t max_split)
    {
        std::vector<std::string> result{}; // -V826
        std::size_t pos;                   // NOLINT(cppcoreguidelines-init-variables)

        while (((pos = str.find(delimiter)) != std::string_view::npos) && (max_split != 0)) {
            result.emplace_back(str.substr(0, pos));
            str.remove_prefix(pos + delimiter.length());
            --max_split;
        }

        // The last part of the string
        result.emplace_back(str);

        return result;
    }

    std::vector<std::string> split_by_spaces(std::string_view str, std::size_t max_split)
    {
        if (cpputils::is_empty_or_whitespace(str)) {
            return {};
        }

        std::vector<std::string> result{}; // -V826
        std::size_t pos;                   // NOLINT(cppcoreguidelines-init-variables)

        while ((pos = str.find_first_of(cpputils::SPACE_CHARACTERS)) != std::string_view::npos) {
            const auto prefix_len = str.find_first_not_of(cpputils::SPACE_CHARACTERS, pos);

            if (const auto token = str.substr(0, pos); !cpputils::is_empty_or_whitespace(token)) {
                result.emplace_back(token);

                if (--max_split; (0 == max_split) || (std::string_view::npos == prefix_len)) {
                    str.remove_prefix(pos);
                    break;
                }
            }

            str.remove_prefix(prefix_len);
        }

        if (!cpputils::is_empty_or_whitespace(str)) {
            result.emplace_back(str);
        }

        return result;
    }

    std::vector<std::string> rsplit_by_delimiter(
      std::string_view str, const std::string_view delimiter, std::size_t max_split)
    {
        std::vector<std::string> result{}; // -V826
        std::size_t pos;                   // NOLINT(cppcoreguidelines-init-variables)

        while (((pos = str.rfind(delimiter)) != std::string_view::npos) && (max_split != 0)) {
            const auto token = str.substr(pos + delimiter.length());
            result.emplace(result.begin(), token);
            str.remove_suffix(delimiter.length() + token.length());
            --max_split;
        }

        // The last part of the string
        result.emplace(result.begin(), str);

        return result;
    }

    std::vector<std::string> rsplit_by_spaces(std::string_view str, std::size_t max_split)
    {
        if (cpputils::is_empty_or_whitespace(str)) {
            return {};
        }

        std::vector<std::string> result{}; // -V826
        std::size_t pos;                   // NOLINT(cppcoreguidelines-init-variables)

        while ((pos = str.find_last_of(cpputils::SPACE_CHARACTERS)) != std::string_view::npos) {
            const auto token_pos = str.find_first_not_of(cpputils::SPACE_CHARACTERS, pos);

            if (std::string_view::npos == token_pos) {
                str = str.substr(0, pos);
                continue;
            }

            result.emplace(result.begin(), str.substr(token_pos));

            if (--max_split; 0 == max_split) {
                const auto token = str.substr(token_pos);
                str.remove_suffix(token.length());
                break;
            }

            str = str.substr(0, pos);
        }

        if (!cpputils::is_empty_or_whitespace(str)) {
            result.emplace(result.begin(), str);
        }

        return result;
    }

    std::vector<std::string> split(const std::string_view str, const std::string_view delimiter,
      const cpputils::StringSplitOptions options, const std::size_t max_split, const bool reverse)
    {
        std::vector<std::string> result{}; // -V826

        if (0 == max_split) {
            result.emplace_back(str);
            return result;
        }

        if (delimiter.empty()) {
            result = reverse ? rsplit_by_spaces(str, max_split) : split_by_spaces(str, max_split);
        }
        else {
            result =
              reverse ? rsplit_by_delimiter(str, delimiter, max_split) : split_by_delimiter(str, delimiter, max_split);
        }

        switch (options) {
            case cpputils::StringSplitOptions::none: {
                break;
            }
            case cpputils::StringSplitOptions::trim_entries: {
                trim_entries(result);
                break;
            }
            case cpputils::StringSplitOptions::remove_empty_entries: {
                remove_empty_entries(result);
                break;
            }
            case cpputils::StringSplitOptions::trim_remove_empty_entries: {
                trim_entries(result);
                remove_empty_entries(result);
                break;
            }
        }

        return result;
    }

    [[nodiscard]] bool starts_with(std::string_view str, const std::string_view value, const std::size_t start,
      std::size_t end, const unsigned flags) noexcept
    {
        if (value.empty()) {
            return false;
        }

        if (end > str.length()) {
            end = str.length();
        }

        if ((start >= end) || (value.length() > (str.length() - start)) || (value.length() > (end - start))) {
            return false;
        }

        if ((start > 0) || (end < str.length())) {
            str = str.substr(start, end - start);
        }

        const auto reverse = STARTS_WITH_REVERSE == (flags & STARTS_WITH_REVERSE);
        const auto ignore_case = STARTS_WITH_IGNORE_CASE == (flags & STARTS_WITH_IGNORE_CASE);

        if (reverse) {
            return ignore_case ? cpputils::ends_with_ignore_case(std::string{str}, std::string{value})
                               : std::equal(value.crbegin(), value.crend(), str.crbegin());
        }

        return ignore_case ? cpputils::starts_with_ignore_case(std::string{str}, std::string{value})
                           : std::equal(value.cbegin(), value.cend(), str.cbegin());
    }
}

namespace cpputils
{
    std::string capitalize(const std::string_view str) noexcept
    {
        std::string result = lower(str);

        if (!result.empty()) {
            auto& ch = result[0];
            const auto uch = static_cast<unsigned char>(ch);
            ch = static_cast<std::string::value_type>(std::toupper(uch));
        }

        return result;
    }

    std::string center(
      const std::string_view str, const std::size_t length, const std::string_view::value_type character)
    {
        if ((str.length() >= length) || (EOS == character)) {
            return std::string{str};
        }

        std::string result{};
        result.reserve(length);

        const auto half_str = str.length() / 2;
        const auto half_length = static_cast<std::string::size_type>(std::round(static_cast<double>(length) / 2.));

        // Left
        for (auto i = half_length - half_str; i > 0; --i) {
            result.push_back(character);
        }

        // Center
        result.append(str);

        // Right
        for (auto i = result.length(); i < length; ++i) {
            result.push_back(character);
        }

        return result;
    }

    std::size_t count(
      std::string_view str, const std::string_view value, const std::size_t start, std::size_t end) noexcept
    {
        if (end > str.length()) {
            end = str.length();
        }

        if (value.empty() || (start >= end)) {
            return 0;
        }

        if ((start > 0) || (end < str.length())) {
            str = str.substr(start, end - start);
        }

        auto result = std::size_t{0};
        auto pos = std::size_t{0};

        while ((pos = str.find(value, pos)) != std::string::npos) {
            pos += value.length();
            ++result;
        }

        return result;
    }

    std::size_t count_ignore_case(
      const std::string& str, const std::string& value, const std::size_t start, std::size_t end) noexcept
    {
        if (end > str.length()) {
            end = str.length();
        }

        if (value.empty() || (start >= end)) {
            return 0;
        }

        auto result = std::size_t{0};
        auto pos = start;

        while ((pos = find_ignore_case(str, value, pos, end)) != std::string::npos) {
            pos += value.length();
            ++result;
        }

        return result;
    }

    bool ends_with(
      const std::string_view str, const std::string_view value, const std::size_t start, const std::size_t end) noexcept
    {
        return ::starts_with(str, value, start, end, STARTS_WITH_REVERSE);
    }

    bool ends_with_ignore_case(const std::string& str, const std::string& value, const std::size_t start) noexcept
    {
        if (value.empty() || (start >= str.length()) || ((str.length() - start) < value.length())) {
            return false;
        }

        return 0 == compare_ignore_case((str.c_str() + str.length()) - value.length(), value.c_str(), value.length());
    }

    bool ends_with_ignore_case(
      const std::string_view str, const std::string_view value, const std::size_t start, const std::size_t end) noexcept
    {
        return ::starts_with(str, value, start, end, STARTS_WITH_REVERSE | STARTS_WITH_IGNORE_CASE);
    }

    std::string expand_tabs(const std::string_view str, const std::size_t tabsize)
    {
        const std::string spaces(tabsize, ' ');
        return replace(str, "\t", spaces);
    }

    std::size_t find(const std::string_view str, const std::string::value_type value, const std::size_t start,
      const std::size_t end) noexcept
    {
        return ::find(str, value, start, end, false);
    }

    std::size_t find(
      const std::string_view str, const std::string_view value, const std::size_t start, const std::size_t end) noexcept
    {
        return ::find(str, value, start, end, false);
    }

    std::size_t find_ignore_case(const std::string_view str, const std::string::value_type value,
      const std::size_t start, const std::size_t end) noexcept
    {
        return ::find_ignore_case(str, value, start, end, false);
    }

    bool is_alphabetic(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_ALPHA);
    }

    bool is_alphanumeric(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_ALNUM);
    }

    bool is_ascii(const std::string_view str) noexcept
    {
        return std::all_of(str.cbegin(), str.cend(), IS_ASCII);
    }

    bool is_digit(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_DIGIT);
    }

    bool is_empty_or_whitespace(const std::string_view str) noexcept
    {
        return str.empty() || std::all_of(str.cbegin(), str.cend(), IS_SPACE);
    }

    bool is_identifier(const std::string_view str) noexcept
    {
        if (str.empty() || (std::isdigit(static_cast<unsigned char>(str.front())) != 0)) {
            return false;
        }

        return std::all_of(str.cbegin(), str.cend(), IS_ALNUM_OR_UNDERSCORE);
    }

    bool is_lower(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_LOWER_OR_NOT_ALPHA) &&
               std::any_of(str.cbegin(), str.cend(), IS_ALPHA);
    }

    bool is_printable(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_PRINT);
    }

    bool is_title(const std::string_view str) noexcept
    {
        for (const auto& word : split(str, EMPTY, StringSplitOptions::trim_remove_empty_entries)) {
            const auto word_len = word.length();

            for (std::size_t i = 0; i < word_len; ++i) {
                const auto uch = static_cast<unsigned char>(word[i]);

                if (0 == std::isalpha(uch)) {
                    continue;
                }

                if (0 == std::isupper(uch)) {
                    return false;
                }

                if (const auto rest = word.substr(i + 1);
                    std::any_of(rest.cbegin(), rest.cend(), IS_ALPHA) && (!is_lower(rest))) {
                    return false;
                }

                break;
            }
        }

        return std::any_of(str.cbegin(), str.cend(), IS_ALPHA);
    }

    bool is_upper(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_UPPER_OR_NOT_ALPHA) &&
               std::any_of(str.cbegin(), str.cend(), IS_ALPHA);
    }

    bool is_whitespace(const std::string_view str) noexcept
    {
        return (!str.empty()) && std::all_of(str.cbegin(), str.cend(), IS_SPACE);
    }

    std::string ljust(
      const std::string_view str, const std::size_t length, const std::string_view::value_type character)
    {
        if (str.length() >= length) {
            return std::string{str};
        }

        std::string result{};
        result.reserve(length);
        result.append(str);

        for (auto i = length - result.length(); i != 0; --i) {
            result.push_back(character);
        }

        return result;
    }

    std::string lower(const std::string_view str) noexcept
    {
        std::string result{str};
        std::transform(result.begin(), result.end(), result.begin(), TO_LOWER);

        return result;
    }

    std::tuple<std::string, std::string, std::string> partition(
      const std::string_view str, const std::string_view value) noexcept
    {
        return ::partition(str, value, false);
    }

    std::string replace(
      const std::string_view str, const std::string_view what, const std::string_view with, std::size_t count)
    {
        std::string result{str};

        if (!what.empty()) {
            auto pos = std::size_t{0};

            while (((pos = result.find(what, pos)) != std::string::npos) && (count != 0)) {
                result.replace(pos, what.length(), with);
                pos += with.length();
                --count;
            }
        }

        return result;
    }

    std::string replace_ignore_case(
      const std::string_view str, const std::string& what, const std::string_view with, std::size_t count)
    {
        std::string result{str};

        if (!what.empty()) {
            auto pos = std::size_t{0};

            while (((pos = find_ignore_case(result, what, pos)) != std::string::npos) && (count != 0)) {
                result.replace(pos, what.length(), with);
                pos += with.length();
                --count;
            }
        }

        return result;
    }

    std::size_t rfind(const std::string_view str, const std::string::value_type value, const std::size_t start,
      const std::size_t end) noexcept
    {
        return ::find(str, value, start, end, true);
    }

    std::size_t rfind(
      const std::string_view str, const std::string_view value, const std::size_t start, const std::size_t end) noexcept
    {
        return ::find(str, value, start, end, true);
    }

    std::size_t rfind_ignore_case(const std::string_view str, const std::string::value_type value,
      const std::size_t start, const std::size_t end) noexcept
    {
        return ::find_ignore_case(str, value, start, end, true);
    }

    std::string rjust(
      const std::string_view str, const std::size_t length, const std::string_view::value_type character)
    {
        if (str.length() >= length) {
            return std::string{str};
        }

        std::string result(length - str.length(), character);
        result.append(str);

        return result;
    }

    std::tuple<std::string, std::string, std::string> rpartition(
      const std::string_view str, const std::string_view value) noexcept
    {
        return ::partition(str, value, true);
    }

    std::vector<std::string> rsplit(const std::string_view str, const std::string_view delimiter,
      const StringSplitOptions options, const std::size_t max_split)
    {
        return ::split(str, delimiter, options, max_split, true);
    }

    std::vector<std::string> split(const std::string_view str, const std::string_view delimiter,
      const StringSplitOptions options, const std::size_t max_split)
    {
        return ::split(str, delimiter, options, max_split, false);
    }

    std::vector<std::string> split_lines(std::string_view str, const bool keep_line_breaks)
    {
        if (str.empty()) {
            return {};
        }

        std::vector<std::string> result{}; // -V826
        std::size_t pos;                   // NOLINT(cppcoreguidelines-init-variables)

        while ((pos = str.find('\n')) != std::string_view::npos) {
            const auto token = str.substr(0, pos + 1);
            result.emplace_back(token);
            str.remove_prefix(token.length());
        }

        if (!str.empty()) {
            result.emplace_back(str);
        }

        if (!keep_line_breaks) {
            constexpr auto* trim_line_breaks = +[](std::string& entry)
            {
                return entry = rtrim(entry, "\r\n");
            };

            std::transform(result.begin(), result.end(), result.begin(), trim_line_breaks);
        }

        return result;
    }

    bool starts_with(
      const std::string_view str, const std::string_view value, const std::size_t start, const std::size_t end) noexcept
    {
        return ::starts_with(str, value, start, end, NO_FLAGS);
    }

    bool starts_with_ignore_case(const std::string& str, const std::string& value, const std::size_t start) noexcept
    {
        if (value.empty() || (start >= str.length()) || ((str.length() - start) < value.length())) {
            return false;
        }

        return 0 == compare_ignore_case((str.c_str() + start), value.c_str(), value.length());
    }

    bool starts_with_ignore_case(
      const std::string_view str, const std::string_view value, const std::size_t start, const std::size_t end) noexcept
    {
        return ::starts_with(str, value, start, end, STARTS_WITH_IGNORE_CASE);
    }

    std::string swap_case(const std::string_view str) noexcept
    {
        std::string result{str};
        std::transform(result.begin(), result.end(), result.begin(), SWAP_CASE);

        return result;
    }

    std::string title(const std::string_view str) noexcept
    {
        auto result = lower(str);
        auto pos = std::size_t{0};

        while ((pos = result.find_first_not_of(SPACE_CHARACTERS, pos)) != std::string::npos) {
            auto uch = static_cast<unsigned char>(result[pos]);

            while (0 == std::isalpha(uch)) {
                if (++pos; pos >= result.length()) {
                    return result;
                }

                uch = static_cast<unsigned char>(result[pos]);
            }

            result[pos] = static_cast<std::string::value_type>(std::toupper(uch));
            pos = result.find_first_of(SPACE_CHARACTERS, pos);
        }

        return result;
    }

    std::string upper(const std::string_view str) noexcept
    {
        std::string result{str};
        std::transform(result.begin(), result.end(), result.begin(), TO_UPPER);

        return result;
    }
}
