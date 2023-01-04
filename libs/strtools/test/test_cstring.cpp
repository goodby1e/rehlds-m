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

#include "strtools/cstring.h"
#include <gtest/gtest.h>
#include <string>

#ifdef _WIN32
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
#endif

namespace strtools::test
{
    TEST(CString, CompareIgnoreCase)
    {
        std::string lhs{};
        std::string rhs{};
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str()), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        rhs = "VALuE";
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str()), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        lhs = "hIS JOkES weRE FunNY.";
        rhs = "his JApEs wErE hySTerICaL.";
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str()), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        lhs = "ThEY bOTh DId IT at THE saME Time.";
        rhs = "THEY boTh dID IT AT thE saMe timE.";
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str()), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        lhs.clear();
        rhs.clear();
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str(), 5), ::strncasecmp(lhs.c_str(), rhs.c_str(), 5));

        rhs = "vaLuE";
        ASSERT_EQ(
          compare_ignore_case(lhs.c_str(), rhs.c_str(), 999'999), ::strncasecmp(lhs.c_str(), rhs.c_str(), 999'999));

        lhs = "His JoKeS wEre funNY.";
        rhs = "His jApes WErE hySTeriCaL.";
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str(), 5), ::strncasecmp(lhs.c_str(), rhs.c_str(), 5));

        lhs = "THey BOtH DId iT AT The SaME TIME.";
        rhs = "they both did it at the same time.";
        ASSERT_EQ(compare_ignore_case(lhs.c_str(), rhs.c_str(), 12), ::strncasecmp(lhs.c_str(), rhs.c_str(), 12));
    }

    TEST(CString, Equal)
    {
        std::string lhs{};
        std::string rhs{};
        ASSERT_FALSE(equal(lhs.c_str(), " "));
        ASSERT_TRUE(equal(lhs.c_str(), rhs.c_str()));

        lhs = "They were extremely happy about what happened.";
        rhs = "They were extremely happy about what happened.";
        ASSERT_TRUE(equal(lhs.c_str(), rhs.c_str()));

        lhs = "They were extremely happy about what happened.";
        rhs = "THey were extremely happy about what happened.";
        ASSERT_FALSE(equal(lhs.c_str(), rhs.c_str()));

        lhs = "Think carefully about what you're doing.";
        rhs = "Cerebrate punctiliously about what you're doing.";
        ASSERT_FALSE(equal(lhs.c_str(), rhs.c_str()));
    }

    TEST(CString, EqualIgnoreCase)
    {
        ASSERT_TRUE(equal_ignore_case('a', 'A'));
        ASSERT_TRUE(equal_ignore_case('Z', 'z'));
        ASSERT_TRUE(equal_ignore_case('-', '-'));
        ASSERT_FALSE(equal_ignore_case('f', 'x'));

        std::string lhs{};
        std::string rhs{};
        ASSERT_FALSE(equal_ignore_case(lhs.c_str(), " "));
        ASSERT_TRUE(equal_ignore_case(lhs.c_str(), rhs.c_str()));

        lhs = "tHeY WERE EXtrEMeLy hAppY ABOUt WHAT HAPPENeD.";
        rhs = "ThEy WERE eXTRemelY HAppy AbouT whAt HApPEneD.";
        ASSERT_TRUE(equal_ignore_case(lhs.c_str(), rhs.c_str()));

        lhs = "Think carefully about what you're doing.";
        rhs = "     I was the winner of the chess game.";
        ASSERT_FALSE(equal_ignore_case(lhs.c_str(), rhs.c_str()));
    }

    TEST(CString, Find)
    {
        std::string str{};
        ASSERT_TRUE(nullptr == find(str.data(), "value"));
        ASSERT_TRUE(nullptr == find(str.c_str(), "value"));

        str = "His account has been disabled due to TOS violations.";
        ASSERT_TRUE(nullptr == find(str.data(), EMPTY));
        ASSERT_TRUE(nullptr == find(str.c_str(), EMPTY));

        str = "They're going to make fun of my hat later when I see them.";
        ASSERT_STREQ(find(str.data(), "'"), "'re going to make fun of my hat later when I see them.");
        ASSERT_STREQ(find(str.c_str(), "'"), "'re going to make fun of my hat later when I see them.");
        ASSERT_STREQ(find(str.data(), "T"), "They're going to make fun of my hat later when I see them.");
        ASSERT_STREQ(find(str.c_str(), "T"), "They're going to make fun of my hat later when I see them.");
        ASSERT_STREQ(find(str.data(), "."), ".");
        ASSERT_STREQ(find(str.c_str(), "."), ".");
        ASSERT_STREQ(find(str.data(), "I see"), "I see them.");
        ASSERT_STREQ(find(str.c_str(), "I see"), "I see them.");
        ASSERT_TRUE(nullptr == find(str.data(), "x"));
        ASSERT_TRUE(nullptr == find(str.c_str(), "x"));
        ASSERT_TRUE(nullptr == find(str.data(), "xxx"));
        ASSERT_TRUE(nullptr == find(str.c_str(), "xxx"));

        str = "I gave strength to her and that enabled her to perform better.";
        ASSERT_STREQ(find(str.data(), "I", 0, 1), "I gave strength to her and that enabled her to perform better.");
        ASSERT_STREQ(find(str.c_str(), "I", 0, 1), "I gave strength to her and that enabled her to perform better.");
        ASSERT_STREQ(find(str.data(), ".", 55), ".");
        ASSERT_STREQ(find(str.c_str(), ".", 55), ".");
        ASSERT_TRUE(nullptr == find(str.data(), "e", 55, 10));
        ASSERT_TRUE(nullptr == find(str.c_str(), "e", 55, 10));
        ASSERT_STREQ(find(str.data(), " to ", 40, 55), " to perform better.");
        ASSERT_STREQ(find(str.c_str(), " to ", 40, 55), " to perform better.");
    }

    TEST(CString, FindIgnoreCase)
    {
        std::string str{};
        ASSERT_TRUE(nullptr == find_ignore_case(str.data(), "vAlUe"));
        ASSERT_TRUE(nullptr == find_ignore_case(str.c_str(), "vaLuE"));

        str = "I'm GoinG To EAT YogURT.";
        ASSERT_TRUE(nullptr == find_ignore_case(str.data(), EMPTY));
        ASSERT_TRUE(nullptr == find_ignore_case(str.c_str(), EMPTY));

        str = "i'M GOinG To eAt yoGURt.";
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), "'"), "'m going to eat yogurt.");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), "'"), "'m going to eat yogurt.");
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), "I"), "I'm going to eat yogurt.");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), "i"), "I'm going to eat yogurt.");
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), "."), ".");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), "."), ".");
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), "TO EAT"), "to eat yogurt.");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), "to eat"), "to eat yogurt.");
        ASSERT_TRUE(nullptr == find_ignore_case(str.data(), "X"));
        ASSERT_TRUE(nullptr == find_ignore_case(str.c_str(), "x"));
        ASSERT_TRUE(nullptr == find_ignore_case(str.data(), "XXX"));
        ASSERT_TRUE(nullptr == find_ignore_case(str.c_str(), "xxx"));

        str = "my TUrTLE cAn Be oPInIonatED.";
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), "M", 0, 1), "My turtle can be opinionated.");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), "m", 0, 1), "My turtle can be opinionated.");
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), ".", 28), ".");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), ".", 28), ".");
        ASSERT_TRUE(nullptr == find_ignore_case(str.data(), "x", 28, 10));
        ASSERT_TRUE(nullptr == find_ignore_case(str.c_str(), "X", 28, 10));
        ASSERT_STRCASEEQ(find_ignore_case(str.data(), " be ", 10, 28), " be opinionated.");
        ASSERT_STRCASEEQ(find_ignore_case(str.c_str(), " BE ", 13, 29), " be opinionated.");
    }

    TEST(CString, RFind)
    {
        std::string str{};
        ASSERT_TRUE(nullptr == rfind(str.data(), "value"));
        ASSERT_TRUE(nullptr == rfind(str.c_str(), "value"));

        str = "His account has been disabled due to TOS violations.";
        ASSERT_TRUE(nullptr == rfind(str.data(), EMPTY));
        ASSERT_TRUE(nullptr == rfind(str.c_str(), EMPTY));

        str = "They're going to make fun of my hat later when I see them.";
        ASSERT_STREQ(rfind(str.data(), "he"), "hem.");
        ASSERT_STREQ(rfind(str.c_str(), "he"), "hem.");
        ASSERT_STREQ(rfind(str.data(), "T"), "They're going to make fun of my hat later when I see them.");
        ASSERT_STREQ(rfind(str.c_str(), "T"), "They're going to make fun of my hat later when I see them.");
        ASSERT_STREQ(rfind(str.data(), "."), ".");
        ASSERT_STREQ(rfind(str.c_str(), "."), ".");
        ASSERT_STREQ(rfind(str.data(), "I see"), "I see them.");
        ASSERT_STREQ(rfind(str.c_str(), "I see"), "I see them.");
        ASSERT_TRUE(nullptr == rfind(str.data(), "x"));
        ASSERT_TRUE(nullptr == rfind(str.c_str(), "x"));
        ASSERT_TRUE(nullptr == rfind(str.data(), "xxx"));
        ASSERT_TRUE(nullptr == rfind(str.c_str(), "xxx"));

        str = "I gave strength to her and that enabled her to perform better.";
        ASSERT_STREQ(rfind(str.data(), "I", 0, 1), "I gave strength to her and that enabled her to perform better.");
        ASSERT_STREQ(rfind(str.c_str(), "I", 0, 1), "I gave strength to her and that enabled her to perform better.");
        ASSERT_STREQ(rfind(str.data(), "her", 15, 40), "her and that enabled her to perform better.");
        ASSERT_STREQ(rfind(str.c_str(), "her", 15, 40), "her and that enabled her to perform better.");
        ASSERT_STREQ(rfind(str.data(), ".", 55), ".");
        ASSERT_STREQ(rfind(str.c_str(), ".", 55), ".");
        ASSERT_TRUE(nullptr == rfind(str.data(), "e", 55, 10));
        ASSERT_TRUE(nullptr == rfind(str.c_str(), "e", 55, 10));
    }

    TEST(CString, RFindIgnoreCase)
    {
        std::string str{};
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.data(), "VAluE"));
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.c_str(), "vaLUE"));

        str = "his AccoUnT HaS BEeN DisablEd Due TO tOs VIoLaTIOns.";
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.data(), EMPTY));
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.c_str(), EMPTY));

        str = "ThEY'Re gOING TO makE FUn Of mY haT LAtER wHEN i SeE tHEm.";
        ASSERT_STRCASEEQ(rfind_ignore_case(str.data(), "hE"), "hem.");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.c_str(), "He"), "hem.");

        ASSERT_STRCASEEQ(
          rfind_ignore_case(str.data(), "tHeY'"), "They're going to make fun of my hat later when I see them.");

        ASSERT_STRCASEEQ(
          rfind_ignore_case(str.c_str(), "they'"), "They're going to make fun of my hat later when I see them.");

        ASSERT_STRCASEEQ(rfind_ignore_case(str.data(), "."), ".");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.c_str(), "."), ".");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.data(), "i See"), "I see them.");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.c_str(), "i sEe"), "I see them.");
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.data(), "X"));
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.c_str(), "x"));
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.data(), "xXx"));
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.c_str(), "xxx"));

        str = "i gavE STREnGth TO her AND tHAT eNabLed heR To PerFoRm bEtTeR.";

        ASSERT_STRCASEEQ(
          rfind_ignore_case(str.data(), "I", 0, 1), "I gave strength to her and that enabled her to perform better.");

        ASSERT_STRCASEEQ(
          rfind_ignore_case(str.c_str(), "I", 0, 1), "I gave strength to her and that enabled her to perform better.");

        ASSERT_STRCASEEQ(rfind_ignore_case(str.data(), "her", 15, 40), "her and that enabled her to perform better.");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.c_str(), "her", 15, 40), "her and that enabled her to perform better.");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.data(), ".", 55), ".");
        ASSERT_STRCASEEQ(rfind_ignore_case(str.c_str(), ".", 55), ".");
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.data(), "e", 55, 10));
        ASSERT_TRUE(nullptr == rfind_ignore_case(str.c_str(), "e", 55, 10));
    }

    TEST(CString, IsNullOrEmpty)
    {
        ASSERT_TRUE(is_null_or_empty(nullptr));
        ASSERT_TRUE(is_null_or_empty(EMPTY));
        ASSERT_FALSE(is_null_or_empty(" "));
        ASSERT_FALSE(is_null_or_empty("x"));
        ASSERT_FALSE(is_null_or_empty("  x  "));
        ASSERT_FALSE(is_null_or_empty(SPACE_CHARACTERS));
    }

    TEST(CString, IsNullOrWhitespace)
    {
        ASSERT_TRUE(is_null_or_whitespace(nullptr));
        ASSERT_TRUE(is_null_or_whitespace(EMPTY));
        ASSERT_TRUE(is_null_or_whitespace(" "));
        ASSERT_FALSE(is_null_or_whitespace("x"));
        ASSERT_FALSE(is_null_or_whitespace("  x  "));
        ASSERT_TRUE(is_null_or_whitespace(SPACE_CHARACTERS));
    }
}
