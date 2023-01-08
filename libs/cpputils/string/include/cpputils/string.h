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

#include "cpputils/cstring.h"
#include "cpputils/string_const.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace cpputils
{
    enum class StringSplitOptions
    {
        /**
         * @brief Use the default options when splitting strings.
         */
        none,

        /**
         * @brief Trim whitespace characters from each substring in the result.
         */
        trim_entries,

        /**
         * @brief Omit array elements that contain an empty string from the result.
         */
        remove_empty_entries,

        /**
         * @brief Trim whitespace characters and omit array elements that contain an empty string from the result.
         */
        trim_remove_empty_entries
    };

    /**
     * @brief Converts the first character to uppercase and the rest to lowercase.
     *
     * @param str String to convert.
     *
     * @return String where the first character is upper case, and the rest is lower case.
     */
    [[nodiscard]] std::string capitalize(std::string_view str) noexcept;

    /**
     * @brief Returns a centered string.
     *
     * @param str String to convert.
     * @param length Length of the returned string.
     * @param character Character to fill the missing space on each side. Default is ' ' (space).
     *
     * @return Centered string that uses the specified character as the fill character.
     */
    [[nodiscard]] std::string center(
      std::string_view str, std::size_t length, std::string_view::value_type character = ' ');

    /**
     * @brief Compares two strings lexicographically (case insensitive).
     *
     * @return Negative value if lhs appears before rhs in lexicographical order.
     * Zero if lhs and rhs compare equal.
     * Positive value if lhs appears after rhs in lexicographical order.
     */
    [[nodiscard]] inline int compare_ignore_case(const std::string& lhs, const std::string& rhs) noexcept
    {
        return compare_ignore_case(lhs.c_str(), rhs.c_str());
    }

    /**
     * @brief Compares two strings lexicographically (case insensitive).
     *
     * @return Negative value if lhs appears before rhs in lexicographical order.
     * Zero if lhs and rhs compare equal.
     * Positive value if lhs appears after rhs in lexicographical order.
     */
    [[nodiscard]] inline int compare_ignore_case(
      const std::string& lhs, const std::string& rhs, const std::size_t count) noexcept
    {
        return compare_ignore_case(lhs.c_str(), rhs.c_str(), count);
    }

    /**
     * @brief Returns \c true if the specified substring occurs in the string.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Position to start the search. Default is 0.
     *
     * @return \c true if the specified substring occurs in the string; otherwise \c false.
     */
    [[nodiscard]] constexpr bool contains(
      const std::string_view str, const std::string_view value, const std::size_t start = 0) noexcept
    {
        return (!value.empty()) && (str.find(value, start) != std::string_view::npos);
    }

    /**
     * @brief Returns \c true if the specified substring occurs in the string (case insensitive).
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return \c true if the specified substring occurs in the string; otherwise \c false.
     */
    [[nodiscard]] inline bool contains_ignore_case(const std::string& str, const std::string& value,
      const std::size_t start = 0, const std::size_t end = std::string::npos) noexcept
    {
        return (!value.empty()) && (find_ignore_case(str.c_str(), value.c_str(), start, end) != nullptr);
    }

    /**
     * @brief Returns the number of times a specified value occurs in a string.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return Number of times a \c value occurs in a \c str.
     */
    [[nodiscard]] std::size_t count(std::string_view str, std::string_view value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Returns the number of times a specified value occurs in a string (case insensitive).
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return Number of times a \c value occurs in a \c str.
     */
    [[nodiscard]] std::size_t count_ignore_case(const std::string& str, const std::string& value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Returns \c true if the string ends with the specified value.
     *
     * @param str String to examine.
     * @param value Value to check if the string ends with.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return \c true, if the \c str ends with the \c value; otherwise \c false.
     */
    [[nodiscard]] bool ends_with(std::string_view str, std::string_view value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Returns \c true if the string ends with the specified value (case insensitive).
     *
     * @param str String to examine.
     * @param value Value to check if the string ends with.
     * @param start Position to start the search. Default is 0.
     *
     * @return \c true, if the \c str ends with the \c value; otherwise \c false.
     */
    [[nodiscard]] bool ends_with_ignore_case(
      const std::string& str, const std::string& value, std::size_t start = 0) noexcept;

    /**
     * @brief Returns \c true if the string ends with the specified value (case insensitive).
     *
     * @param str String to examine.
     * @param value Value to check if the string ends with.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return \c true, if the \c str ends with the \c value; otherwise \c false.
     */
    [[nodiscard]] bool ends_with_ignore_case(
      std::string_view str, std::string_view value, std::size_t start, std::size_t end) noexcept;

    /**
     * @brief Determines whether two strings have the same value (case insensitive).
     */
    [[nodiscard]] inline bool equal_ignore_case(const std::string& lhs, const std::string& rhs) noexcept
    {
        return equal_ignore_case(lhs.c_str(), rhs.c_str());
    }

    /**
     * @brief Sets the tab size of the string.
     *
     * @param str String.
     * @param tabsize Number specifying the tabsize. Default tabsize is 4.
     *
     * @return String where the tabulation characters are replaced by the specified number of spaces.
     */
    [[nodiscard]] std::string expand_tabs(std::string_view str, std::size_t tabsize = 4);

    /**
     * @brief Searches the string for a specified value and returns the position of where it was found.
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the first occurrence of the value in the string,
     * or \c std::string::npos if the \c value is empty or not part of a string.
     */
    [[nodiscard]] std::size_t find(std::string_view str, std::string::value_type value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the position of where it was found.
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the first occurrence of the value in the string,
     * or \c std::string::npos if the \c value is empty or not part of a string.
     */
    [[nodiscard]] std::size_t find(std::string_view str, std::string_view value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Searches the string for a specified value and returns
     * the position of where it was found (case insensitive).
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the first occurrence of the value in the string,
     * or \c std::string::npos if the \c value is empty or not part of a string.
     */
    [[nodiscard]] std::size_t find_ignore_case(std::string_view str, std::string::value_type value,
      std::size_t start = 0, std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Searches the string for a specified value and returns
     * the position of where it was found (case insensitive).
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the first occurrence of the value in the string,
     * or \c std::string::npos if the \c value is empty or not part of a string.
     */
    [[nodiscard]] inline std::size_t find_ignore_case(const std::string& str, const std::string& value,
      const std::size_t start = 0, const std::size_t end = std::string::npos) noexcept
    {
        const auto* const result = find_ignore_case(str.c_str(), value.c_str(), start, end);
        return nullptr == result ? std::string::npos : static_cast<std::string::size_type>(result - str.c_str());
    }

    /**
     * @brief Returns \c true if all characters in the string are in the alphabet.
     */
    [[nodiscard]] bool is_alphabetic(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are alphanumeric.
     */
    [[nodiscard]] bool is_alphanumeric(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are ASCII characters.
     */
    [[nodiscard]] bool is_ascii(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are digits.
     */
    [[nodiscard]] bool is_digit(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if the specified string empty or consists only of white-space characters.
     */
    [[nodiscard]] bool is_empty_or_whitespace(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if the string is an identifier.
     * A string is considered a valid identifier if it only contains alphanumeric
     * letters (a-z) and (0-9), or underscores (_).
     * A valid identifier cannot start with a number, or contain any spaces.
     */
    [[nodiscard]] bool is_identifier(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are lower case.
     *
     * @note Numbers, symbols and spaces are not checked, only alphabet characters.
     */
    [[nodiscard]] bool is_lower(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are printable.
     *
     * @note Example of none printable character can be carriage return and line feed.
     */
    [[nodiscard]] bool is_printable(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all words in a text start with an upper case letter,
     * and the rest of the word are lower case letters.
     * Symbols and numbers are ignored.
     */
    [[nodiscard]] bool is_title(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are whitespaces.
     *
     * @note Numbers, symbols and spaces are not checked, only alphabet characters.
     */
    [[nodiscard]] bool is_upper(std::string_view str) noexcept;

    /**
     * @brief Returns \c true if all characters in the string are whitespaces.
     */
    [[nodiscard]] bool is_whitespace(std::string_view str) noexcept;

    /**
     * @brief Converts the elements of an iterable into a string.
     */
    template <typename Container, typename ValueType = typename Container::value_type>
    [[nodiscard]] std::string join(const Container& container, const std::string& delimiter)
    {
        auto begin = std::begin(container);
        auto end = std::end(container);

        if (begin == end) {
            return {};
        }

        std::ostringstream string_stream{};
        std::copy(begin, std::prev(end), std::ostream_iterator<ValueType>(string_stream, delimiter.c_str()));
        begin = std::prev(end);

        if (begin != end) {
            string_stream << *begin;
        }

        return string_stream.str();
    }

    /**
     * @brief Returns a left justified version of the string.
     *
     * @param str String.
     * @param length Length of the returned string.
     * @param character Character to fill the missing space (to the right of the string). Default is ' ' (space).
     *
     * @return A left-justified string that uses the specified character as the fill character.
     */
    [[nodiscard]] std::string ljust(
      std::string_view str, std::size_t length, std::string_view::value_type character = ' ');

    /**
     * @brief Converts a string into lower case.
     *
     * @param str String to convert.
     *
     * @return String where all characters are lower case.
     */
    [[nodiscard]] std::string lower(std::string_view str) noexcept;

    /**
     * @brief Removes all leading occurrences of a specified characters from the specified string.
     *
     * @param str String.
     * @param chars Character to remove as leading characters.
     *
     * @return Copy of the string with leading characters removed.
     */
    [[nodiscard]] inline std::string_view ltrim(const std::string_view str, const std::string::value_type ch) noexcept
    {
        const auto pos = str.find_first_not_of(ch);
        return std::string_view::npos == pos ? std::string_view{EMPTY} : str.substr(pos, str.length() - pos);
    }

    /**
     * @brief Removes all leading occurrences of a specified characters from the specified string.
     *
     * @param str String.
     * @param chars Set of characters to remove as leading characters. Default is space characters.
     *
     * @return Copy of the string with leading characters removed.
     */
    [[nodiscard]] inline std::string_view ltrim(
      const std::string_view str, const std::string_view chars = SPACE_CHARACTERS) noexcept
    {
        const auto pos = str.find_first_not_of(chars);
        return std::string_view::npos == pos ? std::string_view{EMPTY} : str.substr(pos, str.length() - pos);
    }

    /**
     * @brief Searches for a specified string, and splits the string into a tuple containing three elements.
     * The first element contains the part before the specified string.
     * The second element contains the specified string.
     * The third element contains the part after the string.
     *
     * @param str String.
     * @param value String to search for.
     *
     * @return Tuple containing three elements: the part before the specified string,
     * the specified string, and the part after the string.
     *
     * @note This function searches for the first occurrence of the specified string.
     */
    [[nodiscard]] std::tuple<std::string, std::string, std::string> partition(
      std::string_view str, std::string_view value) noexcept;

    /**
     * @brief Replaces all occurrences of a specified string in the specified string with another specified string.
     *
     * @param str String.
     * @param what String to search for.
     * @param with String to replace the old value with.
     * @param count Number specifying how many occurrences of the old value you want to replace.
     * Default is all occurrences.
     *
     * @return String where a specified value is replaced with a specified value.
     */
    [[nodiscard]] std::string replace(
      std::string_view str, std::string_view what, std::string_view with, std::size_t count = std::string::npos);

    /**
     * @brief Replaces all occurrences of a specified string in the specified string
     * with another specified string (case insensitive).
     *
     * @param str String.
     * @param what String to search for.
     * @param with String to replace the old value with.
     * @param count Number specifying how many occurrences of the old value you want to replace.
     * Default is all occurrences.
     *
     * @return String where a specified value is replaced with a specified value.
     */
    [[nodiscard]] std::string replace_ignore_case(
      std::string_view str, const std::string& what, std::string_view with, std::size_t count = std::string::npos);

    /**
     * @brief Searches the string for a specified value and returns the last position of where it was found.
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the last occurrence of the value in the string,
     * or \c std::string::npos if value is not part of string.
     * If \c value is an empty string, 0 is returned.
     */
    [[nodiscard]] std::size_t rfind(std::string_view str, std::string::value_type value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the last position of where it was found.
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the last occurrence of the value in the string,
     * or \c std::string::npos if value is not part of string.
     * If \c value is an empty string, 0 is returned.
     */
    [[nodiscard]] std::size_t rfind(std::string_view str, std::string_view value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the last position
     * of where it was found (case insensitive).
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the last occurrence of the value in the string,
     * or \c std::string::npos if value is not part of string.
     * If \c value is an empty string, 0 is returned.
     */
    [[nodiscard]] std::size_t rfind_ignore_case(std::string_view str, std::string::value_type value,
      std::size_t start = 0, std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Searches the string for a specified value and returns the last position
     * of where it was found (case insensitive).
     * Returns \c std::string::npos if the value is not found.
     *
     * @param str String to examine.
     * @param value Value to search for.
     * @param start Where to start the search. Default is 0.
     * @param end Where to end the search. Default is to the end of the string.
     *
     * @return Position of the last occurrence of the value in the string,
     * or \c std::string::npos if value is not part of string.
     * If \c value is an empty string, 0 is returned.
     */
    [[nodiscard]] inline std::size_t rfind_ignore_case(const std::string& str, const std::string& value,
      const std::size_t start = 0, const std::size_t end = std::string::npos) noexcept
    {
        const auto* const result = rfind_ignore_case(str.c_str(), value.c_str(), start, end);
        return nullptr == result ? std::string::npos : static_cast<std::string::size_type>(result - str.c_str());
    }

    /**
     * @brief Returns a right justified version of the string.
     *
     * @param str String.
     * @param length Length of the returned string.
     * @param character Character to fill the missing space (to the right of the string). Default is ' ' (space).
     *
     * @return Right-justified string that uses the specified character as the fill character.
     */
    [[nodiscard]] std::string rjust(
      std::string_view str, std::size_t length, std::string_view::value_type character = ' ');

    /**
     * @brief Searches for a specified string, and splits the string into a tuple containing three elements.
     * The first element contains the part before the specified string.
     * The second element contains the specified string.
     * The third element contains the part after the string.
     *
     * @param str String.
     * @param value String to search for.
     *
     * @return Tuple containing three elements: the part before the specified string,
     * the specified string, and the part after the string.
     *
     * @note This function searches for the last occurrence of the specified string.
     */
    [[nodiscard]] std::tuple<std::string, std::string, std::string> rpartition(
      std::string_view str, std::string_view value) noexcept;

    /**
     * @brief Splits the string at the specified delimiter (starting from the right), and returns a list.
     *
     * @param str String to split.
     * @param delimiter Specifies the delimiter to use when splitting the string.
     * By default, any whitespace is a delimiter.
     * @param options Enumeration values that specifies whether to trim substrings and include empty substrings.
     * @param max_split Specifies how many splits to do. By default, all occurrences.
     *
     * @return List of substrings separated by delimiter.
     *
     * @note When \c max_split is specified, the list will contain the specified number of elements plus one.
     */
    [[nodiscard]] std::vector<std::string> rsplit(std::string_view str, std::string_view delimiter = EMPTY,
      StringSplitOptions options = StringSplitOptions::none, std::size_t max_split = std::string::npos);

    /**
     * @brief Removes all trailing occurrences of a specified character from the specified string.
     *
     * @param str String to strip.
     * @param chars Set of characters to remove as trailing characters. Default is space characters.
     *
     * @return Copy of the string with trailing characters removed.
     */
    [[nodiscard]] inline std::string_view rtrim(
      const std::string_view str, const std::string_view chars = SPACE_CHARACTERS) noexcept
    {
        const auto pos = str.find_last_not_of(chars);
        return std::string_view::npos == pos ? std::string_view{EMPTY} : str.substr(0, pos + 1);
    }

    /**
     * @brief Splits the string at the specified delimiter, and returns a list.
     *
     * @param str String to split.
     * @param delimiter Specifies the delimiter to use when splitting the string.
     * By default, any whitespace is a delimiter.
     * @param options Enumeration values that specifies whether to trim substrings and include empty substrings.
     * @param max_split Specifies how many splits to do. By default, all occurrences.
     *
     * @return List of substrings delimited by delimiter.
     *
     * @note When \c max_split is specified, the list will contain the specified number of elements plus one.
     */
    [[nodiscard]] std::vector<std::string> split(std::string_view str, std::string_view delimiter = EMPTY,
      StringSplitOptions options = StringSplitOptions::none, std::size_t max_split = std::string::npos);

    /**
     * @brief Splits the string at line breaks and returns a list.
     *
     * @param str String to split.
     * @param keep_line_breaks Specifies if the line breaks should be included (\c true), or not (\c false).
     * Default value is \c false.
     *
     * @return List of lines separated by line breaks.
     */
    [[nodiscard]] std::vector<std::string> split_lines(std::string_view str, bool keep_line_breaks = false);

    /**
     * @brief Returns \c true if the string starts with the specified value.
     *
     * @param str String to examine.
     * @param value Value to check if the string starts with.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return \c true, if the \c str starts with the \c value; otherwise \c false.
     */
    [[nodiscard]] bool starts_with(std::string_view str, std::string_view value, std::size_t start = 0,
      std::size_t end = std::string::npos) noexcept;

    /**
     * @brief Returns \c true if the string starts with the specified value (case insensitive).
     *
     * @param str String to examine.
     * @param value Value to check if the string starts with.
     * @param start Position to start the search. Default is 0.
     *
     * @return \c true, if the \c str starts with the \c value; otherwise \c false.
     */
    [[nodiscard]] bool starts_with_ignore_case(
      const std::string& str, const std::string& value, std::size_t start = 0) noexcept;

    /**
     * @brief Returns \c true if the string starts with the specified value (case insensitive).
     *
     * @param str String to examine.
     * @param value Value to check if the string starts with.
     * @param start Position to start the search. Default is 0.
     * @param end Position to end the search. Default is the end of the string.
     *
     * @return \c true, if the \c str starts with the \c value; otherwise \c false.
     */
    [[nodiscard]] bool starts_with_ignore_case(
      std::string_view str, std::string_view value, std::size_t start, std::size_t end) noexcept;

    /**
     * @brief Swaps cases, lower case becomes upper case and vice versa.
     *
     * @param str String to convert.
     *
     * @return String where all the upper case letters are lower case and vice versa.
     */
    [[nodiscard]] std::string swap_case(std::string_view str) noexcept;

    /**
     * @brief Converts the first character of each word to upper case.
     *
     * @note If the word contains a number or a symbol, the first letter after that will be converted to upper case.
     *
     * @param str String to convert.
     *
     * @return String where the first character in every word is upper case. Like a header, or a title.
     */
    [[nodiscard]] std::string title(std::string_view str) noexcept;

    /**
     * @brief Removes all leading and trailing occurrences of the specified character from the specified string.
     *
     * @param str String to strip.
     * @param chars A set of characters to remove as leading/trailing characters.
     * Default is space characters.
     *
     * @return A copy of the string with leading/trailing characters removed.
     */
    [[nodiscard]] inline std::string trim(
      const std::string_view str, const std::string_view chars = SPACE_CHARACTERS) noexcept
    {
        return std::string{rtrim(ltrim(str, chars), chars)};
    }

    /**
     * @brief Converts a string into upper case.
     *
     * @param str String to convert.
     *
     * @return String where all characters are upper case.
     */
    [[nodiscard]] std::string upper(std::string_view str) noexcept;
}
