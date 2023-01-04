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

#include "strtools/string.h"
#include <gtest/gtest.h>
#include <array>

#ifdef _WIN32
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
#endif

namespace strtools::test
{
    TEST(String, Capitalize)
    {
        auto str = capitalize(EMPTY);
        ASSERT_TRUE(str.empty());

        str = capitalize(SPACE_CHARACTERS);
        ASSERT_STREQ(str.c_str(), SPACE_CHARACTERS);

        str = capitalize("galaxy");
        ASSERT_STREQ(str.c_str(), "Galaxy");

        str = capitalize("i stated an opinion on what happened.");
        ASSERT_STREQ(str.c_str(), "I stated an opinion on what happened.");

        str = capitalize("Don't overrate what really happened.");
        ASSERT_STREQ(str.c_str(), "Don't overrate what really happened.");

        str = capitalize("kal-EL Is tOO FAsT For Sonic.");
        ASSERT_STREQ(str.c_str(), "Kal-el is too fast for sonic.");
    }

    TEST(String, Center)
    {
        auto str = center(EMPTY, 0);
        ASSERT_TRUE(str.empty());

        str = center(EMPTY, 1);
        ASSERT_STREQ(str.c_str(), " ");

        str = center("banana", 20, EOS);
        ASSERT_STREQ(str.c_str(), "banana");

        str = center("banana", 20);
        ASSERT_STREQ(str.c_str(), "       banana       ");

        str = center("banana", 20, '-');
        ASSERT_STREQ(str.c_str(), "-------banana-------");

        str = center("banana", 19, 'X');
        ASSERT_STREQ(str.c_str(), "XXXXXXXbananaXXXXXX");

        str = center("banana", 7, '0');
        ASSERT_STREQ(str.c_str(), "0banana");

        str = center("banana", 6, '0');
        ASSERT_STREQ(str.c_str(), "banana");

        str = center("banana", 5, '0');
        ASSERT_STREQ(str.c_str(), "banana");
    }

    TEST(String, CompareIgnoreCase)
    {
        std::string lhs{};
        std::string rhs{};
        ASSERT_EQ(compare_ignore_case(lhs, rhs), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        rhs = "VALuE";
        ASSERT_EQ(compare_ignore_case(lhs, rhs), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        lhs = "hIS JOkES weRE FunNY.";
        rhs = "his JApEs wErE hySTerICaL.";
        ASSERT_EQ(compare_ignore_case(lhs, rhs), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        lhs = "ThEY bOTh DId IT at THE saME Time.";
        rhs = "THEY boTh dID IT AT thE saMe timE.";
        ASSERT_EQ(compare_ignore_case(lhs, rhs), ::strcasecmp(lhs.c_str(), rhs.c_str()));

        lhs.clear();
        rhs.clear();
        ASSERT_EQ(compare_ignore_case(lhs, rhs, 5), ::strncasecmp(lhs.c_str(), rhs.c_str(), 5));

        rhs = "vaLuE";
        ASSERT_EQ(compare_ignore_case(lhs, rhs, 999'999), ::strncasecmp(lhs.c_str(), rhs.c_str(), 999'999));

        lhs = "His JoKeS wEre funNY.";
        rhs = "His jApes WErE hySTeriCaL.";
        ASSERT_EQ(compare_ignore_case(lhs, rhs, 5), ::strncasecmp(lhs.c_str(), rhs.c_str(), 5));

        lhs = "THey BOtH DId iT AT The SaME TIME.";
        rhs = "they both did it at the same time.";
        ASSERT_EQ(compare_ignore_case(lhs, rhs, 12), ::strncasecmp(lhs.c_str(), rhs.c_str(), 12));
    }

    TEST(String, Contains)
    {
        ASSERT_FALSE(contains(EMPTY, EMPTY));
        ASSERT_FALSE(contains(EMPTY, "value"));
        ASSERT_FALSE(contains("His account has been disabled due to TOS violations.", EMPTY));

        std::string str{"They're going to make fun of my hat later when I see them."};
        ASSERT_TRUE(contains(str, "'"));
        ASSERT_TRUE(contains(str, "T"));
        ASSERT_TRUE(contains(str, "."));
        ASSERT_TRUE(contains(str, "I see"));
        ASSERT_FALSE(contains(str, "x"));
        ASSERT_FALSE(contains(str, "xxx"));

        str = "I gave strength to her and that enabled her to perform better.";
        ASSERT_TRUE(contains(str, "I", 0));
        ASSERT_TRUE(contains(str, ".", 55));
        ASSERT_FALSE(contains(str, "x", 55));
        ASSERT_TRUE(contains(str, " to ", 40));
    }

    TEST(String, ContainsIgnoreCase)
    {
        ASSERT_FALSE(contains_ignore_case(EMPTY, EMPTY));
        ASSERT_FALSE(contains_ignore_case(EMPTY, "vaLUE"));
        ASSERT_FALSE(contains_ignore_case("HIs aCcouNT HAS bEen DisabLeD DUE TO ToS VIOLAtionS.", EMPTY));

        std::string str{"theY'Re gOINg To MAKe FUn Of MY HaT laTeR WhEN i see tHem."};
        ASSERT_TRUE(contains_ignore_case(str, "'"));
        ASSERT_TRUE(contains_ignore_case(str, "T"));
        ASSERT_TRUE(contains_ignore_case(str, "."));
        ASSERT_TRUE(contains_ignore_case(str, "I see"));
        ASSERT_FALSE(contains_ignore_case(str, "x"));
        ASSERT_FALSE(contains_ignore_case(str, "xXx"));

        str = "i gAVE StReNGTh TO HeR AND ThAt enaBleD HER tO perFORM BetTEr.";
        ASSERT_TRUE(contains_ignore_case(str, "I", 0, 1));
        ASSERT_TRUE(contains_ignore_case(str, ".", 55));
        ASSERT_FALSE(contains_ignore_case(str, "e", 55, 10));
        ASSERT_TRUE(contains_ignore_case(str, " To ", 40, 55));
    }

    TEST(String, Count)
    {
        ASSERT_EQ(count(EMPTY, "got"), 0);
        ASSERT_EQ(count("His account has been disabled due to TOS violations.", EMPTY), 0);
        ASSERT_EQ(count("  Don't take her kindness for weakness. ", " "), 8);
        ASSERT_EQ(count("Be very careful with the path that you choose to take.", "e"), 6);

        std::string str{"The algorithm takes care to create text that looks similar to an ordinary book."};
        ASSERT_EQ(count(str, " to "), 2);
        ASSERT_EQ(count(str, " to ", 24), 2);

        str = "The President and the Secretary cut the cake.";
        ASSERT_EQ(count(str, " the ", 9999), 0);
        ASSERT_EQ(count(str, " the ", 17, 22), 1);
        ASSERT_EQ(count(str, " the ", 30, 10), 0);
    }

    TEST(String, CountIgnoreCase)
    {
        ASSERT_EQ(count_ignore_case(EMPTY, "got"), 0);
        ASSERT_EQ(count_ignore_case("his acCOUNt hAS BEEn DIsAbLEd dUe To TOs ViolATIons.", EMPTY), 0);
        ASSERT_EQ(count_ignore_case("  don't tAKe her kinDNeSs fOr WEaKnESs. ", " "), 8);
        ASSERT_EQ(count_ignore_case("BE very CaREfUl WITH thE PATH thAt YOu chOosE To taKE.", "e"), 6);

        std::string str{"tHe AlGoritHm taKeS CAre To creATe tEXT THAt lOOkS SimIlar tO aN ORdINaRy Book."};
        ASSERT_EQ(count_ignore_case(str, " to "), 2);
        ASSERT_EQ(count_ignore_case(str, " TO ", 24), 2);

        str = "tHe PReSiDENT and tHe SEcrETary CUt THe CaKE.";
        ASSERT_EQ(count_ignore_case(str, " the ", 9999), 0);
        ASSERT_EQ(count_ignore_case(str, " thE ", 17, 22), 1);
        ASSERT_EQ(count_ignore_case(str, " tHe ", 30, 10), 0);
    }

    TEST(String, EndsWith)
    {
        std::string str{};
        ASSERT_FALSE(ends_with(str, EMPTY));
        ASSERT_FALSE(ends_with(str, ".ext"));

        str = "on my plate";
        ASSERT_FALSE(ends_with(str, EMPTY));
        ASSERT_TRUE(ends_with(str, "plate"));

        str = "That wasn't what was implied was it?";
        ASSERT_TRUE(ends_with(str, "?"));
        ASSERT_FALSE(ends_with(str, "."));

        str = "I finally got a new bike.";
        ASSERT_FALSE(ends_with(str, ".", 9999));
        ASSERT_TRUE(ends_with(str, "t", 10, 13));
        ASSERT_FALSE(ends_with(str, "t", 13, 10));
    }

    TEST(String, EndsWithIgnoreCase)
    {
        std::string str{};
        ASSERT_FALSE(ends_with_ignore_case(str, EMPTY));
        ASSERT_FALSE(ends_with_ignore_case(str, ".eXt"));

        str = "on my plate";
        ASSERT_FALSE(ends_with_ignore_case(str, EMPTY));
        ASSERT_TRUE(ends_with_ignore_case(str, "plate"));

        str = "tHAt wAsN'T WHaT WaS ImPLied was It?";
        ASSERT_TRUE(ends_with_ignore_case(str, "iT?"));
        ASSERT_FALSE(ends_with_ignore_case(str, "."));

        str = "I FiNAlly goT a nEw BiKE.";
        ASSERT_FALSE(ends_with_ignore_case(str, ".", 9999));
        ASSERT_TRUE(ends_with_ignore_case(str, "t", 10, 13));
        ASSERT_FALSE(ends_with_ignore_case(str, "t", 13, 10));
    }

    TEST(String, EqualIgnoreCase)
    {
        ASSERT_FALSE(equal_ignore_case(EMPTY, " "));
        ASSERT_TRUE(equal_ignore_case(EMPTY, EMPTY));

        std::string lhs{"tHeY WERE EXtrEMeLy hAppY ABOUt WHAT HAPPENeD."};
        std::string rhs{"ThEy WERE eXTRemelY HAppy AbouT whAt HApPEneD."};
        ASSERT_TRUE(equal_ignore_case(lhs, rhs));

        lhs = "Think carefully about what you're doing.";
        rhs = "     I was the winner of the chess game.";
        ASSERT_FALSE(equal_ignore_case(lhs, rhs));
    }

    TEST(String, ExpandTabs)
    {
        auto str = expand_tabs(EMPTY);
        ASSERT_TRUE(str.empty());

        str = expand_tabs("\t\t", 0);
        ASSERT_TRUE(str.empty());

        str = expand_tabs("\t\t", 1);
        ASSERT_STREQ(str.c_str(), "  ");

        str = expand_tabs("\tThey are widely known around\t\tthe planet.\t", 3);
        ASSERT_STREQ(str.c_str(), "   They are widely known around      the planet.   ");
    }

    TEST(String, Find)
    {
        std::string str{};
        ASSERT_EQ(find(str, 'v'), std::string::npos);
        ASSERT_EQ(find(str, "value"), std::string::npos);

        str = "His account has been disabled due to TOS violations.";
        ASSERT_EQ(find(str, EOS), std::string::npos);
        ASSERT_EQ(find(str, EMPTY), std::string::npos);

        str = "They're going to make fun of my hat later when I see them.";
        ASSERT_EQ(find(str, '\''), 4);
        ASSERT_EQ(find(str, 'T'), 0);
        ASSERT_EQ(find(str, '.'), 57);
        ASSERT_EQ(find(str, 'I'), 47);
        ASSERT_EQ(find(str, 'x'), std::string::npos);
        ASSERT_EQ(find(str, "'"), 4);
        ASSERT_EQ(find(str, "T"), 0);
        ASSERT_EQ(find(str, "."), 57);
        ASSERT_EQ(find(str, "I see"), 47);
        ASSERT_EQ(find(str, "x"), std::string::npos);
        ASSERT_EQ(find(str, "xxx"), std::string::npos);

        str = "I gave strength to her and that enabled her to perform better.";
        ASSERT_EQ(find(str, 'I', 9999), std::string::npos);
        ASSERT_EQ(find(str, 'I', 0, 1), 0);
        ASSERT_EQ(find(str, '.', 55), 61);
        ASSERT_EQ(find(str, 'e', 55, 10), std::string::npos);
        ASSERT_EQ(find(str, 't', 40, 55), 44);
        ASSERT_EQ(find(str, "I", 9999), std::string::npos);
        ASSERT_EQ(find(str, "I", 0, 1), 0);
        ASSERT_EQ(find(str, ".", 55), 61);
        ASSERT_EQ(find(str, "e", 55, 10), std::string::npos);
        ASSERT_EQ(find(str, " to ", 40, 55), 43);
    }

    TEST(String, FindIgnoreCase)
    {
        std::string str{};
        ASSERT_EQ(find_ignore_case(str, 'v'), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, "VaLUe"), std::string::npos);

        str = "HIS aCCouNt hAs been DiSabled DUE tO tOs VIolATioNS.";
        // ASSERT_EQ(find_ignore_case(str, EOS), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, EMPTY), std::string::npos);

        str = "THey'rE GOIng to MAKE fUN of mY HaT LatER wHEN I sEE tHEM.";
        ASSERT_EQ(find_ignore_case(str, '\''), 4);
        ASSERT_EQ(find_ignore_case(str, 't'), 0);
        ASSERT_EQ(find_ignore_case(str, '.'), 57);
        ASSERT_EQ(find_ignore_case(str, 'i'), 10);
        ASSERT_EQ(find_ignore_case(str, 'x'), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, "'"), 4);
        ASSERT_EQ(find_ignore_case(str, "t"), 0);
        ASSERT_EQ(find_ignore_case(str, "."), 57);
        ASSERT_EQ(find_ignore_case(str, "i sEe"), 47);
        ASSERT_EQ(find_ignore_case(str, "x"), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, "xXx"), std::string::npos);

        str = "i GAVE strengtH To heR aND THaT eNABled HeR TO perforM bEttER.";
        ASSERT_EQ(find_ignore_case(str, 'I', 9999), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, 'I', 0, 1), 0);
        ASSERT_EQ(find_ignore_case(str, '.', 55), 61);
        ASSERT_EQ(find_ignore_case(str, 'e', 55, 10), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, 't', 40, 55), 44);
        ASSERT_EQ(find_ignore_case(str, "I", 9999), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, "I", 0, 1), 0);
        ASSERT_EQ(find_ignore_case(str, ".", 55), 61);
        ASSERT_EQ(find_ignore_case(str, "e", 55, 10), std::string::npos);
        ASSERT_EQ(find_ignore_case(str, " to ", 40, 55), 43);
    }

    TEST(String, IsAlphabetic)
    {
        ASSERT_FALSE(is_alphabetic(EMPTY));
        ASSERT_FALSE(is_alphabetic(SPACE_CHARACTERS));
        ASSERT_FALSE(is_alphabetic("0123456789"));
        ASSERT_FALSE(is_alphabetic("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz "));
        ASSERT_FALSE(is_alphabetic("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_"));
        ASSERT_TRUE(is_alphabetic("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"));
    }

    TEST(String, IsAlphanumeric)
    {
        ASSERT_FALSE(is_alphanumeric(EMPTY));
        ASSERT_FALSE(is_alphanumeric(SPACE_CHARACTERS));
        ASSERT_FALSE(is_alphanumeric("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz 0123456789"));
        ASSERT_FALSE(is_alphanumeric("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_0123456789"));
        ASSERT_TRUE(is_alphanumeric("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789"));
    }

    TEST(String, IsAscii)
    {
        ASSERT_TRUE(is_ascii(EMPTY));
        ASSERT_TRUE(is_ascii(SPACE_CHARACTERS));
        ASSERT_TRUE(is_ascii("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz +-_@ 0123456789"));
        ASSERT_FALSE(is_ascii("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz $© 0123456789"));
    }

    TEST(String, IsDigit)
    {
        ASSERT_FALSE(is_digit(EMPTY));
        ASSERT_FALSE(is_digit(SPACE_CHARACTERS));
        ASSERT_FALSE(is_digit("0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"));
        ASSERT_FALSE(is_digit("0123456789 "));
        ASSERT_TRUE(is_digit("0123456789"));
    }

    TEST(String, IsEmptyOrWhitespace)
    {
        ASSERT_TRUE(is_empty_or_whitespace(EMPTY));
        ASSERT_TRUE(is_empty_or_whitespace(SPACE_CHARACTERS));
        ASSERT_FALSE(is_empty_or_whitespace("."));
        ASSERT_FALSE(is_empty_or_whitespace("   X   "));
        ASSERT_FALSE(is_empty_or_whitespace("0123456789"));
    }

    TEST(String, IsIdentifier)
    {
        ASSERT_FALSE(is_identifier(EMPTY));
        ASSERT_FALSE(is_identifier(SPACE_CHARACTERS));
        ASSERT_FALSE(is_identifier("X "));
        ASSERT_FALSE(is_identifier("0123456789"));
        ASSERT_FALSE(is_identifier("0AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_0123456789"));
        ASSERT_FALSE(is_identifier("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_0123456789@"));
        ASSERT_TRUE(is_identifier("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_0123456789"));
    }

    TEST(String, IsLower)
    {
        ASSERT_FALSE(is_lower(EMPTY));
        ASSERT_FALSE(is_lower(SPACE_CHARACTERS));
        ASSERT_FALSE(is_lower(" X "));
        ASSERT_TRUE(is_lower(" x "));
        ASSERT_FALSE(is_lower("0123456789"));
        ASSERT_FALSE(is_lower("01234-56789"));
        ASSERT_FALSE(is_lower("0123456789 "));
        ASSERT_TRUE(is_lower("0123456789 x"));
        ASSERT_FALSE(is_lower("You will continue to improve as long as you stay focused."));
        ASSERT_TRUE(is_lower("i want to live in peace and happiness."));
    }

    TEST(String, IsPrintable)
    {
        ASSERT_FALSE(is_printable(EMPTY));
        ASSERT_FALSE(is_printable(SPACE_CHARACTERS));
        ASSERT_TRUE(is_printable("An alien incursion transpired in the northern part of the galaxy."));
        ASSERT_FALSE(is_printable("An alien incursion transpired in the northern part of the galaxy.\r\n"));
    }

    TEST(String, IsTitle)
    {
        ASSERT_FALSE(is_title(EMPTY));
        ASSERT_FALSE(is_title(SPACE_CHARACTERS));
        ASSERT_FALSE(is_title("123"));
        ASSERT_FALSE(is_title("- !! 321 @$% "));
        ASSERT_TRUE(is_title("A"));
        ASSERT_TRUE(is_title("A0"));
        ASSERT_TRUE(is_title("\rA\n B2 \tC D"));
        ASSERT_FALSE(is_title("Hello, And Welcome TO My World!"));
        ASSERT_TRUE(is_title("Hello, And Welcome To My World!"));
        ASSERT_TRUE(is_title("Hello"));
        ASSERT_TRUE(is_title("22 Names"));
        ASSERT_TRUE(is_title("This Is %'!?"));
    }

    TEST(String, IsUpper)
    {
        ASSERT_FALSE(is_upper(EMPTY));
        ASSERT_FALSE(is_upper(SPACE_CHARACTERS));
        ASSERT_FALSE(is_upper(" x "));
        ASSERT_TRUE(is_upper(" X "));
        ASSERT_FALSE(is_upper("0123456789"));
        ASSERT_FALSE(is_upper("01234-56789"));
        ASSERT_FALSE(is_upper("0123456789 "));
        ASSERT_TRUE(is_upper("0123456789 X"));
        ASSERT_FALSE(is_upper("yOU WILL CONTINUE TO IMPROVE AS LONG AS YOU STAY FOCUSED."));
        ASSERT_TRUE(is_upper("DON'T MAKE A MOCKERY OF ME AND WHAT I HAVE DONE."));
    }

    TEST(String, IsWhitespace)
    {
        ASSERT_FALSE(is_whitespace(EMPTY));
        ASSERT_TRUE(is_whitespace(SPACE_CHARACTERS));
        ASSERT_FALSE(is_whitespace("   x   "));
    }

    TEST(String, Join)
    {
        std::vector<std::string> string_container{};
        auto str = join(string_container, " ");
        ASSERT_TRUE(str.empty());

        string_container.emplace_back("one");
        str = join(string_container, " ");
        ASSERT_STREQ(str.c_str(), "one");

        string_container.emplace_back("two ");
        string_container.emplace_back("three");
        str = join(string_container, " ");
        ASSERT_STREQ(str.c_str(), "one two  three");

        constexpr std::array<double, 5> double_container{526.57, 241.356, 140.894, 517.764, 242.075};
        str = join(double_container, ", ");
        ASSERT_STREQ(str.c_str(), "526.57, 241.356, 140.894, 517.764, 242.075");

        str = join(std::string{"abcd"}, ", ");
        ASSERT_STREQ(str.c_str(), "a, b, c, d");
    }

    TEST(String, LJust)
    {
        auto str = ljust(EMPTY, 0);
        ASSERT_TRUE(str.empty());

        str = ljust(str, 2);
        ASSERT_STREQ(str.c_str(), "  ");

        str = ljust(" banana", 10);
        ASSERT_STREQ(str.c_str(), " banana   ");
    }

    TEST(String, Lower)
    {
        auto str = lower(EMPTY);
        ASSERT_TRUE(str.empty());

        str = lower(SPACE_CHARACTERS);
        ASSERT_STREQ(str.c_str(), SPACE_CHARACTERS);

        str = lower("ThaT WASn't wHAt WAS IMPLIeD Was iT?");
        ASSERT_STREQ(str.c_str(), "that wasn't what was implied was it?");
    }

    TEST(String, LTrim)
    {
        std::string str{ltrim(EMPTY, EOS)};
        ASSERT_TRUE(str.empty());

        str = ltrim(" ", ' ');
        ASSERT_TRUE(str.empty());

        str = ltrim("  \t\t  x   \r\n", ' ');
        ASSERT_STREQ(str.c_str(), "\t\t  x   \r\n");

        str = ltrim(str, '\t');
        str = ltrim(str, ' ');
        ASSERT_STREQ(str.c_str(), "x   \r\n");

        str = ltrim(" \n  He will regain consciousness when the time is right.", '\n');
        ASSERT_STREQ(str.c_str(), " \n  He will regain consciousness when the time is right.");

        str = ltrim(EMPTY);
        ASSERT_TRUE(str.empty());

        str = ltrim(SPACE_CHARACTERS);
        ASSERT_TRUE(str.empty());

        str = ltrim("  \t\t  x   \r\n");
        ASSERT_STREQ(str.c_str(), "x   \r\n");

        str = ltrim("   \t\t \r  \n  He will regain consciousness when the time is right.");
        ASSERT_STREQ(str.c_str(), "He will regain consciousness when the time is right.");

        str = ltrim("  \t\t  xxx   \r\n", " \r\t\nx");
        ASSERT_TRUE(str.empty());

        str = ltrim("   He will regain consciousness when the time is right.", " He");
        ASSERT_STREQ(str.c_str(), "will regain consciousness when the time is right.");
    }

    TEST(String, Partition)
    {
        auto parts = partition(EMPTY, EMPTY);
        ASSERT_TRUE(std::get<0>(parts).empty());
        ASSERT_TRUE(std::get<1>(parts).empty());
        ASSERT_TRUE(std::get<2>(parts).empty());

        parts = partition(EMPTY, "value");
        ASSERT_TRUE(std::get<0>(parts).empty());
        ASSERT_TRUE(std::get<1>(parts).empty());
        ASSERT_TRUE(std::get<2>(parts).empty());

        parts = partition("value", EMPTY);
        ASSERT_STREQ(std::get<0>(parts).c_str(), "value");
        ASSERT_TRUE(std::get<1>(parts).empty());
        ASSERT_TRUE(std::get<2>(parts).empty());

        parts = partition("I could eat bananas all day", "bananas");
        ASSERT_STREQ(std::get<0>(parts).c_str(), "I could eat ");
        ASSERT_STREQ(std::get<1>(parts).c_str(), "bananas");
        ASSERT_STREQ(std::get<2>(parts).c_str(), " all day");

        parts = partition("I could eat bananas all day", "day");
        ASSERT_STREQ(std::get<0>(parts).c_str(), "I could eat bananas all ");
        ASSERT_STREQ(std::get<1>(parts).c_str(), "day");
        ASSERT_STREQ(std::get<2>(parts).c_str(), EMPTY);

        parts = partition("I could eat bananas all day", "I");
        ASSERT_STREQ(std::get<0>(parts).c_str(), EMPTY);
        ASSERT_STREQ(std::get<1>(parts).c_str(), "I");
        ASSERT_STREQ(std::get<2>(parts).c_str(), " could eat bananas all day");

        parts = partition("I could eat bananas all day, bananas are my favorite fruit", "bananas");
        ASSERT_STREQ(std::get<0>(parts).c_str(), "I could eat ");
        ASSERT_STREQ(std::get<1>(parts).c_str(), "bananas");
        ASSERT_STREQ(std::get<2>(parts).c_str(), " all day, bananas are my favorite fruit");
    }

    TEST(String, Replace)
    {
        auto str = replace(EMPTY, EMPTY, "-");
        ASSERT_TRUE(str.empty());

        str = replace("I got rid of the garbage that was in the basement.", "a", "o", 4);
        ASSERT_STREQ(str.c_str(), "I got rid of the gorboge thot wos in the basement.");

        str = replace(str, "bo", "BADoomS");
        ASSERT_STREQ(str.c_str(), "I got rid of the gorBADoomSge thot wos in the basement.");

        str = replace(str, "attitude", "o_O");
        ASSERT_STREQ(str.c_str(), "I got rid of the gorBADoomSge thot wos in the basement.");

        str = replace("I am unaffected by your verbal attacks.", "unaffected", "impervious");
        ASSERT_STREQ(str.c_str(), "I am impervious by your verbal attacks.");
    }

    TEST(String, ReplaceIgnoreCase)
    {
        auto str = replace_ignore_case(EMPTY, EMPTY, "-");
        ASSERT_TRUE(str.empty());

        str = replace_ignore_case("i gOt rId of thE GArBAgE THAT WaS IN THE BAseMeNt.", "a", "o", 4);
        ASSERT_STRCASEEQ(str.c_str(), "I got rid of the gorboge thot wos in the basement.");

        str = replace_ignore_case(str, "bO", "BADoomS");
        ASSERT_STRCASEEQ(str.c_str(), "I got rid of the gorbadoomsge thot wos in the basement.");

        str = replace_ignore_case(str, "AtTitUdE", "o_O");
        ASSERT_STRCASEEQ(str.c_str(), "I got rid of the gorbadoomsge thot wos in the basement.");

        str = replace_ignore_case("i AM uNAFfEcTED By YoUr VeRbaL aTtaCKS.", "unaffected", "impervious");
        ASSERT_STRCASEEQ(str.c_str(), "I am impervious by your verbal attacks.");
    }

    TEST(String, RFind)
    {
        std::string str{};
        ASSERT_EQ(rfind(str, 'v'), std::string::npos);
        ASSERT_EQ(rfind(str, "value"), std::string::npos);

        str = "His account has been disabled due to TOS violations.";
        ASSERT_EQ(rfind(str, EOS), std::string::npos);
        ASSERT_EQ(rfind(str, EMPTY), std::string::npos);

        str = "They're going to make fun of my hat later when I see them.";
        ASSERT_EQ(rfind(str, '\''), 4);
        ASSERT_EQ(rfind(str, 'T'), 0);
        ASSERT_EQ(rfind(str, '.'), 57);
        ASSERT_EQ(rfind(str, 'I'), 47);
        ASSERT_EQ(rfind(str, 'x'), std::string::npos);
        ASSERT_EQ(rfind(str, "'"), 4);
        ASSERT_EQ(rfind(str, "T"), 0);
        ASSERT_EQ(rfind(str, "."), 57);
        ASSERT_EQ(rfind(str, "I see"), 47);
        ASSERT_EQ(rfind(str, "x"), std::string::npos);
        ASSERT_EQ(rfind(str, "xxx"), std::string::npos);

        str = "I gave strength to her and that enabled her to perform better.";
        ASSERT_EQ(rfind(str, 'I', 0, 1), 0);
        ASSERT_EQ(rfind(str, '.', 55), 61);
        ASSERT_EQ(rfind(str, 'e', 55, 10), std::string::npos);
        ASSERT_EQ(rfind(str, 't'), 58);
        ASSERT_EQ(rfind(str, 't', 10, 20), 16);
        ASSERT_EQ(rfind(str, "I", 0, 1), 0);
        ASSERT_EQ(rfind(str, ".", 55), 61);
        ASSERT_EQ(rfind(str, "e", 55, 10), std::string::npos);
        ASSERT_EQ(rfind(str, " to "), 43);
        ASSERT_EQ(rfind(str, " to ", 10, 20), 15);
    }

    TEST(String, RFindIgnoreCase)
    {
        std::string str{};
        ASSERT_EQ(rfind_ignore_case(str, 'v'), std::string::npos);
        ASSERT_EQ(rfind_ignore_case(str, "VAluE"), std::string::npos);

        str = "his AccoUnT HaS BEeN DisablEd Due TO tOs VIoLaTIOns.";
        // ASSERT_EQ(rfind_ignore_case(str, EOS), std::string::npos);
        ASSERT_EQ(rfind_ignore_case(str, EMPTY), std::string::npos);

        str = "ThEY'Re gOING TO makE FUn Of mY haT LAtER wHEN i SeE tHEm.";
        ASSERT_EQ(rfind_ignore_case(str, 'h'), 54);
        ASSERT_EQ(rfind_ignore_case(str, 't'), 53);
        ASSERT_EQ(rfind_ignore_case(str, "hE"), 54);
        ASSERT_EQ(rfind_ignore_case(str, "tHeY'"), 0);
        ASSERT_EQ(rfind_ignore_case(str, '.'), 57);
        ASSERT_EQ(rfind_ignore_case(str, 'i'), 47);
        ASSERT_EQ(rfind_ignore_case(str, 'X'), std::string::npos);
        ASSERT_EQ(rfind_ignore_case(str, "."), 57);
        ASSERT_EQ(rfind_ignore_case(str, "i See"), 47);
        ASSERT_EQ(rfind_ignore_case(str, "X"), std::string::npos);
        ASSERT_EQ(rfind_ignore_case(str, "xXx"), std::string::npos);

        str = "i gavE STREnGth TO her AND tHAT eNabLed heR To PerFoRm bEtTeR.";
        ASSERT_EQ(rfind_ignore_case(str, 'I', 0, 1), 0);
        ASSERT_EQ(rfind_ignore_case(str, 'h', 15, 40), 28);
        ASSERT_EQ(rfind_ignore_case(str, '.', 55), 61);
        ASSERT_EQ(rfind_ignore_case(str, 'e', 55, 10), std::string::npos);
        ASSERT_EQ(rfind_ignore_case(str, "I", 0, 1), 0);
        ASSERT_EQ(rfind_ignore_case(str, "her", 15, 40), 19);
        ASSERT_EQ(rfind_ignore_case(str, ".", 55), 61);
        ASSERT_EQ(rfind_ignore_case(str, "e", 55, 10), std::string::npos);
    }

    TEST(String, RJust)
    {
        auto str = rjust(EMPTY, 0);
        ASSERT_TRUE(str.empty());

        str = rjust(str, 2);
        ASSERT_STREQ(str.c_str(), "  ");

        str = rjust(" banana ", 10);
        ASSERT_STREQ(str.c_str(), "   banana ");
    }

    TEST(String, RPartition)
    {
        auto parts = rpartition(EMPTY, EMPTY);
        ASSERT_TRUE(std::get<0>(parts).empty());
        ASSERT_TRUE(std::get<1>(parts).empty());
        ASSERT_TRUE(std::get<2>(parts).empty());

        parts = rpartition(EMPTY, "value");
        ASSERT_TRUE(std::get<0>(parts).empty());
        ASSERT_TRUE(std::get<1>(parts).empty());
        ASSERT_TRUE(std::get<2>(parts).empty());

        parts = rpartition("value", EMPTY);
        ASSERT_STREQ(std::get<0>(parts).c_str(), "value");
        ASSERT_TRUE(std::get<1>(parts).empty());
        ASSERT_TRUE(std::get<2>(parts).empty());

        parts = rpartition("I could eat bananas all day", "bananas");
        ASSERT_STREQ(std::get<0>(parts).c_str(), "I could eat ");
        ASSERT_STREQ(std::get<1>(parts).c_str(), "bananas");
        ASSERT_STREQ(std::get<2>(parts).c_str(), " all day");

        parts = rpartition("I could eat bananas all day", "day");
        ASSERT_STREQ(std::get<0>(parts).c_str(), "I could eat bananas all ");
        ASSERT_STREQ(std::get<1>(parts).c_str(), "day");
        ASSERT_STREQ(std::get<2>(parts).c_str(), EMPTY);

        parts = rpartition("I could eat bananas all day", "I");
        ASSERT_STREQ(std::get<0>(parts).c_str(), EMPTY);
        ASSERT_STREQ(std::get<1>(parts).c_str(), "I");
        ASSERT_STREQ(std::get<2>(parts).c_str(), " could eat bananas all day");

        parts = rpartition("I could eat bananas all day, bananas are my favorite fruit", "bananas");
        ASSERT_STREQ(std::get<0>(parts).c_str(), "I could eat bananas all day, ");
        ASSERT_STREQ(std::get<1>(parts).c_str(), "bananas");
        ASSERT_STREQ(std::get<2>(parts).c_str(), " are my favorite fruit");
    }

    TEST(String, RSplit)
    {
        auto vec = rsplit(EMPTY);
        ASSERT_TRUE(vec.empty());

        vec = rsplit(SPACE_CHARACTERS);
        ASSERT_TRUE(vec.empty());

        vec = rsplit(SPACE_CHARACTERS, EMPTY, StringSplitOptions::none, 0);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), SPACE_CHARACTERS);

        std::string str{"  \t Welcome \r\tto\n   the  \t\t\n\n   jungle!\r\n    "};
        vec = rsplit(str);
        ASSERT_EQ(vec.size(), 4);
        ASSERT_STREQ(vec[0].c_str(), "Welcome");
        ASSERT_STREQ(vec[1].c_str(), "to");
        ASSERT_STREQ(vec[2].c_str(), "the");
        ASSERT_STREQ(vec[3].c_str(), "jungle!");

        vec = rsplit(str, EMPTY, StringSplitOptions::none, 2);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "  \t Welcome \r\tto\n   ");
        ASSERT_STREQ(vec[1].c_str(), "the");
        ASSERT_STREQ(vec[2].c_str(), "jungle!");

        vec = rsplit(str, EMPTY, StringSplitOptions::trim_entries, 2);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "Welcome \r\tto");
        ASSERT_STREQ(vec[1].c_str(), "the");
        ASSERT_STREQ(vec[2].c_str(), "jungle!");

        vec = rsplit(EMPTY, "\n");
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), EMPTY);

        str = "  xyz ";
        vec = rsplit(str);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), "xyz");

        vec = rsplit(str, " ");
        ASSERT_EQ(vec.size(), 4);
        ASSERT_STREQ(vec[0].c_str(), EMPTY);
        ASSERT_STREQ(vec[1].c_str(), EMPTY);
        ASSERT_STREQ(vec[2].c_str(), "xyz");
        ASSERT_STREQ(vec[3].c_str(), EMPTY);

        vec = rsplit(str, " ", StringSplitOptions::trim_entries);
        ASSERT_EQ(vec.size(), 4);

        vec = rsplit(str, " ", StringSplitOptions::remove_empty_entries);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), "xyz");

        vec = rsplit(str, " ", StringSplitOptions::trim_remove_empty_entries);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), "xyz");

        str = SPACE_CHARACTERS;
        vec = rsplit(str, EMPTY, StringSplitOptions::trim_remove_empty_entries);
        ASSERT_TRUE(vec.empty());

        str = "  one two three ";
        vec = rsplit(str, " ");
        ASSERT_EQ(vec.size(), 6);
        ASSERT_STREQ(vec[0].c_str(), EMPTY);
        ASSERT_STREQ(vec[1].c_str(), EMPTY);
        ASSERT_STREQ(vec[2].c_str(), "one");
        ASSERT_STREQ(vec[3].c_str(), "two");
        ASSERT_STREQ(vec[4].c_str(), "three");
        ASSERT_STREQ(vec[5].c_str(), EMPTY);

        vec = rsplit(str);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three");

        str = "  one, two, three ";
        vec = rsplit(str, ", ");
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "  one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three ");

        str = "one two three";
        vec = rsplit(str, " ");
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three");

        str = "one, two, three, ";
        vec = rsplit(str, ", ");
        ASSERT_EQ(vec.size(), 4);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three");
        ASSERT_STREQ(vec[3].c_str(), EMPTY);

        str = "one, two, three, ";
        vec = rsplit(str, ", ", StringSplitOptions::none, 1);
        ASSERT_EQ(vec.size(), 2);
        ASSERT_STREQ(vec[0].c_str(), "one, two, three");
        ASSERT_STREQ(vec[1].c_str(), EMPTY);

        vec = rsplit(str, ", ", StringSplitOptions::none, 2);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "one, two");
        ASSERT_STREQ(vec[1].c_str(), "three");
        ASSERT_STREQ(vec[2].c_str(), EMPTY);
    }

    TEST(String, RTrim)
    {
        std::string str{rtrim(EMPTY)};
        ASSERT_TRUE(str.empty());

        str = rtrim(SPACE_CHARACTERS);
        ASSERT_TRUE(str.empty());

        str = rtrim("  \t\t  x   \r\n");
        ASSERT_STREQ(str.c_str(), "  \t\t  x");

        str = rtrim("He will regain consciousness when the time is right.   \t\t \r  \n  ");
        ASSERT_STREQ(str.c_str(), "He will regain consciousness when the time is right.");

        str = rtrim("  \t\t  xxx   \r\n", "x \r\t\n");
        ASSERT_TRUE(str.empty());

        str = rtrim("He will regain consciousness when the time is right.   ", " is right.");
        ASSERT_STREQ(str.c_str(), "He will regain consciousness when the time");
    }

    TEST(String, Split)
    {
        auto vec = split(EMPTY);
        ASSERT_TRUE(vec.empty());

        vec = split(SPACE_CHARACTERS);
        ASSERT_TRUE(vec.empty());

        vec = split(SPACE_CHARACTERS, EMPTY, StringSplitOptions::none, 0);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), SPACE_CHARACTERS);

        std::string str{"  \t Welcome \r\tto\n   the  \t\t\n\n   jungle!\r\n    "};
        vec = split(str);
        ASSERT_EQ(vec.size(), 4);
        ASSERT_STREQ(vec[0].c_str(), "Welcome");
        ASSERT_STREQ(vec[1].c_str(), "to");
        ASSERT_STREQ(vec[2].c_str(), "the");
        ASSERT_STREQ(vec[3].c_str(), "jungle!");

        vec = split(str, EMPTY, StringSplitOptions::none, 2);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "Welcome");
        ASSERT_STREQ(vec[1].c_str(), "to");
        ASSERT_STREQ(vec[2].c_str(), "\n   the  \t\t\n\n   jungle!\r\n    ");

        vec = split(str, EMPTY, StringSplitOptions::trim_entries, 2);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "Welcome");
        ASSERT_STREQ(vec[1].c_str(), "to");
        ASSERT_STREQ(vec[2].c_str(), "the  \t\t\n\n   jungle!");

        vec = split(EMPTY, "\n");
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), EMPTY);

        str = "  xyz ";
        vec = split(str);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), "xyz");

        vec = split(str, " ");
        ASSERT_EQ(vec.size(), 4);
        ASSERT_STREQ(vec[0].c_str(), EMPTY);
        ASSERT_STREQ(vec[1].c_str(), EMPTY);
        ASSERT_STREQ(vec[2].c_str(), "xyz");
        ASSERT_STREQ(vec[3].c_str(), EMPTY);

        vec = split(str, " ", StringSplitOptions::trim_entries);
        ASSERT_EQ(vec.size(), 4);

        vec = split(str, " ", StringSplitOptions::remove_empty_entries);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), "xyz");

        vec = split(str, " ", StringSplitOptions::trim_remove_empty_entries);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_STREQ(vec[0].c_str(), "xyz");

        str = SPACE_CHARACTERS;
        vec = split(str, EMPTY, StringSplitOptions::trim_remove_empty_entries);
        ASSERT_TRUE(vec.empty());

        str = "  one two three ";
        vec = split(str, " ");
        ASSERT_EQ(vec.size(), 6);
        ASSERT_STREQ(vec[0].c_str(), EMPTY);
        ASSERT_STREQ(vec[1].c_str(), EMPTY);
        ASSERT_STREQ(vec[2].c_str(), "one");
        ASSERT_STREQ(vec[3].c_str(), "two");
        ASSERT_STREQ(vec[4].c_str(), "three");
        ASSERT_STREQ(vec[5].c_str(), EMPTY);

        vec = split(str);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three");

        str = "  one, two, three ";
        vec = split(str, ", ");
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "  one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three ");

        str = "one two three";
        vec = split(str, " ");
        ASSERT_EQ(vec.size(), 3);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three");

        str = "one, two, three, ";
        vec = split(str, ", ");
        ASSERT_EQ(vec.size(), 4);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two");
        ASSERT_STREQ(vec[2].c_str(), "three");
        ASSERT_STREQ(vec[3].c_str(), EMPTY);

        str = "one, two, three, ";
        vec = split(str, ", ", StringSplitOptions::none, 1);
        ASSERT_EQ(vec.size(), 2);
        ASSERT_STREQ(vec[0].c_str(), "one");
        ASSERT_STREQ(vec[1].c_str(), "two, three, ");
    }

    TEST(String, SplitLines)
    {
        auto lines = split_lines(EMPTY);
        ASSERT_TRUE(lines.empty());

        lines = split_lines(" ");
        ASSERT_EQ(lines.size(), 1);
        ASSERT_STREQ(lines[0].c_str(), " ");

        lines = split_lines("Thank you for the music\nWelcome to the jungle");
        ASSERT_EQ(lines.size(), 2);
        ASSERT_STREQ(lines[0].c_str(), "Thank you for the music");
        ASSERT_STREQ(lines[1].c_str(), "Welcome to the jungle");

        lines = split_lines("Thank you for the music\nWelcome to the jungle", true);
        ASSERT_EQ(lines.size(), 2);
        ASSERT_STREQ(lines[0].c_str(), "Thank you for the music\n");
        ASSERT_STREQ(lines[1].c_str(), "Welcome to the jungle");

        lines = split_lines("\r\nThank you for the music\n\nWelcome to the jungle\n\r");
        ASSERT_EQ(lines.size(), 5);
        ASSERT_STREQ(lines[0].c_str(), EMPTY);
        ASSERT_STREQ(lines[1].c_str(), "Thank you for the music");
        ASSERT_STREQ(lines[2].c_str(), EMPTY);
        ASSERT_STREQ(lines[3].c_str(), "Welcome to the jungle");
        ASSERT_STREQ(lines[4].c_str(), EMPTY);

        lines = split_lines("\n\nThank you for the music\n\nWelcome to the jungle\r\n");
        ASSERT_EQ(lines.size(), 5);
        ASSERT_STREQ(lines[0].c_str(), EMPTY);
        ASSERT_STREQ(lines[1].c_str(), EMPTY);
        ASSERT_STREQ(lines[2].c_str(), "Thank you for the music");
        ASSERT_STREQ(lines[3].c_str(), EMPTY);
        ASSERT_STREQ(lines[4].c_str(), "Welcome to the jungle");

        lines = split_lines("\r\nThank you for the music\n\nWelcome to the jungle\n\r", true);
        ASSERT_EQ(lines.size(), 5);
        ASSERT_STREQ(lines[0].c_str(), "\r\n");
        ASSERT_STREQ(lines[1].c_str(), "Thank you for the music\n");
        ASSERT_STREQ(lines[2].c_str(), "\n");
        ASSERT_STREQ(lines[3].c_str(), "Welcome to the jungle\n");
        ASSERT_STREQ(lines[4].c_str(), "\r");

        lines = split_lines("\n\nThank you for the music\n\nWelcome to the jungle\r\n", true);
        ASSERT_EQ(lines.size(), 5);
        ASSERT_STREQ(lines[0].c_str(), "\n");
        ASSERT_STREQ(lines[1].c_str(), "\n");
        ASSERT_STREQ(lines[2].c_str(), "Thank you for the music\n");
        ASSERT_STREQ(lines[3].c_str(), "\n");
        ASSERT_STREQ(lines[4].c_str(), "Welcome to the jungle\r\n");
    }

    TEST(String, StartsWith)
    {
        ASSERT_FALSE(starts_with(EMPTY, EMPTY));
        ASSERT_FALSE(starts_with(EMPTY, "start"));

        std::string str{"on my plate"};
        ASSERT_FALSE(starts_with(str, EMPTY));
        ASSERT_TRUE(starts_with(str, "on"));

        str = "That wasn't what was implied was it?";
        ASSERT_TRUE(starts_with(str, "That wasn't"));
        ASSERT_FALSE(starts_with(str, "That wasnt"));

        str = "I finally got a new bike.";
        ASSERT_TRUE(starts_with(str, "y", 8, 15));
        ASSERT_FALSE(starts_with(str, "y", 15, 8));
    }

    TEST(String, StartsWithIgnoreCase)
    {
        ASSERT_FALSE(starts_with_ignore_case(EMPTY, EMPTY));
        ASSERT_FALSE(starts_with_ignore_case(EMPTY, "sTARt"));

        std::string str{"oN mY PlAte"};
        ASSERT_FALSE(starts_with_ignore_case(str, EMPTY));
        ASSERT_TRUE(starts_with_ignore_case(str, "on"));

        str = "ThaT WASn't WhaT was ImpLied waS it?";
        ASSERT_TRUE(starts_with_ignore_case(str, "That wasn't"));
        ASSERT_FALSE(starts_with_ignore_case(str, "That wasnt"));

        str = "I fiNaLLY goT a new biKe.";
        ASSERT_TRUE(starts_with_ignore_case(str, "y", 8, 15));
        ASSERT_FALSE(starts_with_ignore_case(str, "Y", 15, 8));
    }

    TEST(String, SwapCase)
    {
        auto str = swap_case(EMPTY);
        ASSERT_TRUE(str.empty());

        str = swap_case(SPACE_CHARACTERS);
        ASSERT_STREQ(str.c_str(), SPACE_CHARACTERS);

        str = swap_case("they are gaining a lot of knowledge.");
        ASSERT_STREQ(str.c_str(), "THEY ARE GAINING A LOT OF KNOWLEDGE.");

        str = swap_case(str);
        ASSERT_STREQ(str.c_str(), "they are gaining a lot of knowledge.");

        str = swap_case("i FiNALlY wenT bEYOnd MY PreVioUS LIMitAtiOns.");
        ASSERT_STREQ(str.c_str(), "I fInalLy WENt BeyoND my pREvIOus limITaTIoNS.");
    }

    TEST(String, Title)
    {
        auto str = title(EMPTY);
        ASSERT_TRUE(str.empty());

        str = title(SPACE_CHARACTERS);
        ASSERT_STREQ(str.c_str(), SPACE_CHARACTERS);

        str = title("1 \t23\r\t\n \r\n");
        ASSERT_STREQ(str.c_str(), "1 \t23\r\t\n \r\n");

        str = title("a B c");
        ASSERT_STREQ(str.c_str(), "A B C");

        str = title("\n12a xx 321");
        ASSERT_STREQ(str.c_str(), "\n12A Xx 321");

        str = title("weLcOmE tO My WoRLD");
        ASSERT_STREQ(str.c_str(), "Welcome To My World");
        ASSERT_TRUE(is_title(str));
    }

    TEST(String, Trim)
    {
        auto str = trim(EMPTY);
        ASSERT_TRUE(str.empty());

        str = trim(SPACE_CHARACTERS);
        ASSERT_TRUE(str.empty());

        str = trim("  \t\t  x   \r\n");
        ASSERT_STREQ(str.c_str(), "x");

        str = trim("   \t\t \r  \n  He will regain consciousness when the time is right.   \t\t \r  \n  ");
        ASSERT_STREQ(str.c_str(), "He will regain consciousness when the time is right.");

        str = trim("  \t\t  xxx   \r\n", " \nx\t\r");
        ASSERT_TRUE(str.empty());

        str = trim(" \n\n  He will regain consciousness when the time is right.  \r  ", " He.\r\n");
        ASSERT_STREQ(str.c_str(), "will regain consciousness when the time is right");
    }

    TEST(String, Upper)
    {
        auto str = upper(EMPTY);
        ASSERT_TRUE(str.empty());

        str = upper(SPACE_CHARACTERS);
        ASSERT_STREQ(str.c_str(), SPACE_CHARACTERS);

        str = upper("ThaT WASn't wHAt WAS IMPLIeD Was iT?");
        ASSERT_STREQ(str.c_str(), "THAT WASN'T WHAT WAS IMPLIED WAS IT?");
    }
}
