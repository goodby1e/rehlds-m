/*
 *  Copyright (C) 2022 the_hunter
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

namespace strtools
{
    /**
     * @brief Empty string.
     */
    constexpr auto* EMPTY = "";

    /**
     * @brief End of string character.
     */
    constexpr auto EOS = '\0';

    /**
     * @brief Line feed character.
     */
    constexpr auto LINE_FEED = '\n';

    /**
     * @brief Carriage return character.
     */
    constexpr auto CARRIAGE_RETURN = '\r';

    /**
     * @brief A set of space characters.
     */
    constexpr auto* SPACE_CHARACTERS = " \f\n\r\t\v";

#ifdef _WIN32
    /**
     * @brief Correct path separator of the current OS
     * i.e. '\' on Windows and '/' on Linux.
     */
    constexpr auto CORRECT_PATH_SEPARATOR = '\\';

    /**
     * @brief Incorrect path separator of the current OS
     * i.e. '/' on Windows and '\' on Linux.
     */
    constexpr auto INCORRECT_PATH_SEPARATOR = '/';

    /**
     * @brief Correct path separator of the current OS
     * i.e. "\" on Windows and "/" on Linux.
     */
    constexpr auto* CORRECT_PATH_SEPARATOR_S = "\\";

    /**
     * @brief Incorrect path separator of the current OS
     * i.e. "/" on Windows and "\" on Linux.
     */
    constexpr auto* INCORRECT_PATH_SEPARATOR_S = "/";
#else
    /**
     * @brief Correct path separator of the current OS
     * i.e. '\' on Windows and '/' on Linux.
     */
    constexpr auto CORRECT_PATH_SEPARATOR = '/';

    /**
     * @brief Incorrect path separator of the current OS
     * i.e. '/' on Windows and '\' on Linux.
     */
    constexpr auto INCORRECT_PATH_SEPARATOR = '\\';

    /**
     * @brief Correct path separator of the current OS
     * i.e. "\" on Windows and "/" on Linux.
     */
    constexpr auto* CORRECT_PATH_SEPARATOR_S = "/";

    /**
     * @brief Incorrect path separator of the current OS
     * i.e. "/" on Windows and "\" on Linux.
     */
    constexpr auto* INCORRECT_PATH_SEPARATOR_S = "\\";
#endif
}
