/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "../src/command_line.h"
#include <gtest/gtest.h>
#include <array>
#include <filesystem>
#include <fstream>

namespace rehlds::dedicated::test
{
    class CommandLineTest : public testing::Test
    {
      public:
        static void SetUpTestSuite()
        {
            std::ofstream hlds_params1{"hlds_params1"};
            hlds_params1 << "+maxplayers 31\r\n@@hlds params2\n+map\t \nde_dust" << '\n';
            hlds_params1.close();

            std::ofstream hlds_params2{"@hlds params2"};
            hlds_params2 << "@@hlds params3 -num_edicts 2048" << '\n';
            hlds_params2.close();

            std::ofstream hlds_params3{"@hlds params3"};
            hlds_params3.close();
        }

        static void TearDownTestSuite()
        {
            std::filesystem::remove("hlds_params1");
            std::filesystem::remove("@hlds params2");
            std::filesystem::remove("@hlds params3");
        }

        void SetUp() override
        {
            auto& cmdline = CommandLine::instance();
            cmdline.create("-game CStrike -insecure -NoIPX -Bots +MaxPlayers 32 +map de_dust2-2x2 +sys_TicRate 1000");
        }
    };

    TEST_F(CommandLineTest, CurrentCmdlineReturnValue)
    {
        for (int i = 0; i < 111; ++i) {
            const auto& cmdline = CommandLine::instance();
            ASSERT_FALSE(cmdline.current().empty());
        }
    }

    TEST_F(CommandLineTest, CreateFromString)
    {
        auto& cmdline = CommandLine::instance();

        cmdline.create("");
        ASSERT_TRUE(cmdline.current().empty());

        cmdline.create("hlds.exe param1 value");
        ASSERT_TRUE(cmdline.current().empty());

        cmdline.create("hlds.exe param1 value \t\r\n -dev\f\r\n\f");
        ASSERT_TRUE("-dev" == cmdline.current());

        cmdline.create("hlds.exe -port   27015\f");
        ASSERT_TRUE("-port 27015" == cmdline.current());

        cmdline.create("hlds.exe -port  \r\n\f   27015\n");
        ASSERT_TRUE("-port 27015" == cmdline.current());

        cmdline.create("hlds.exe -param\r\n\f value1\f\r\n  value2");
        ASSERT_TRUE("-param value1   value2" == cmdline.current());

        cmdline.create("hlds-linux -dev -port 27015 +map de_dust+2-@\n");
        ASSERT_TRUE("-dev -port 27015 +map de_dust+2-@" == cmdline.current());

        cmdline.create("-game cstrike -num_edicts 4096 -dev @hlds_params1 -insecure +maxplayers 32 +map de_dust2");
        ASSERT_TRUE("-game cstrike -dev -num_edicts 2048 -insecure +maxplayers 32 +map de_dust2" == cmdline.current());

        cmdline.create("-game cstrike -num_edicts 4096 -dev -insecure +maxplayers 32 +map de_dust2 @hlds_params1");
        ASSERT_TRUE("-game cstrike -dev -insecure +maxplayers 31 -num_edicts 2048 +map de_dust" == cmdline.current());
    }

    TEST_F(CommandLineTest, CreateFromArguments)
    {
        auto& cmdline = CommandLine::instance();

        constexpr std::array<const char*, 0> args1{};
        cmdline.create(args1.size(), args1.data());
        ASSERT_TRUE(cmdline.current().empty());

        constexpr std::array<const char*, 2> args2 = {"hlds.exe", "param value"};
        cmdline.create(args2.size(), args2.data());
        ASSERT_TRUE(cmdline.current().empty());

        constexpr std::array<const char*, 3> args3 = {"hlds.exe", " -game  ", "\tcstrike\r\n"};
        cmdline.create(args3.size(), args3.data());
        ASSERT_TRUE("-game cstrike" == cmdline.current());

        constexpr std::array<const char*, 4> args4 = {"hlds.exe", " -game  ", "\tcstrike\r\n", "\f@hlds_params1\n"};
        cmdline.create(args4.size(), args4.data());
        ASSERT_TRUE("-game cstrike +maxplayers 31 -num_edicts 2048 +map de_dust" == cmdline.current());
    }

    TEST_F(CommandLineTest, FindParam)
    {
        const auto& cmdline = CommandLine::instance();
        std::string values{};

        bool found_param = cmdline.find_param("");
        ASSERT_FALSE(found_param);

        found_param = cmdline.find_param("\t\t -gAmE \r\n");
        ASSERT_TRUE(found_param);

        found_param = cmdline.find_param("\t\t -gAmE \r\n", values);
        ASSERT_TRUE(found_param);
        ASSERT_STRCASEEQ(values.c_str(), "cstrike");

        found_param = cmdline.find_param("+sys_ticrate \r\n", values);
        ASSERT_TRUE(found_param);
        ASSERT_TRUE("1000" == values);

        found_param = cmdline.find_param("\t\t +maxplayers", values);
        ASSERT_TRUE(found_param);
        ASSERT_TRUE("32" == values);
    }

    TEST_F(CommandLineTest, RemoveParam)
    {
        auto& cmdline = CommandLine::instance();

        cmdline.remove_param("");
        ASSERT_TRUE("-game CStrike -insecure -NoIPX -Bots +MaxPlayers 32 +map de_dust2-2x2 +sys_TicRate 1000" ==
                    cmdline.current());

        cmdline.remove_param("-GAME");
        ASSERT_STRCASEEQ(
          cmdline.current().c_str(), "-insecure -noipx -bots +maxplayers 32 +map de_dust2-2x2 +sys_ticrate 1000");

        cmdline.remove_param("  \r\n\f+sys_ticrate   ");
        ASSERT_STRCASEEQ(cmdline.current().c_str(), "-insecure -noipx -bots +maxplayers 32 +map de_dust2-2x2");

        cmdline.remove_param("+maxplayers\r");
        ASSERT_STRCASEEQ(cmdline.current().c_str(), "-insecure -noipx -bots +map de_dust2-2x2");

        cmdline.remove_param("-noipx\n \t");
        ASSERT_STRCASEEQ(cmdline.current().c_str(), "-insecure -bots +map de_dust2-2x2");

        cmdline.remove_param("\t\r-bots \f");
        ASSERT_STRCASEEQ(cmdline.current().c_str(), "-insecure +map de_dust2-2x2");

        cmdline.remove_param("-InSecurE");
        ASSERT_STRCASEEQ(cmdline.current().c_str(), "+map de_dust2-2x2");

        cmdline.remove_param("+mAp");
        ASSERT_TRUE(cmdline.current().empty());
    }

    TEST_F(CommandLineTest, SetParam)
    {
        auto& cmdline = CommandLine::instance();
        cmdline.create("");

        cmdline.set_param("");
        ASSERT_TRUE(cmdline.current().empty());

        cmdline.set_param("bots");
        ASSERT_TRUE(cmdline.current().empty());

        cmdline.set_param("-bots");
        ASSERT_TRUE("-bots" == cmdline.current());

        cmdline.set_param("+map", "de_dust2-2x2");
        ASSERT_TRUE("-bots +map de_dust2-2x2" == cmdline.current());

        cmdline.set_param("maxplayers", "33");
        ASSERT_TRUE("-bots +map de_dust2-2x2" == cmdline.current());

        cmdline.set_param("+maxplayers", "32");
        ASSERT_TRUE("-bots +map de_dust2-2x2 +maxplayers 32" == cmdline.current());

        cmdline.set_param("maxplayers", 24);
        ASSERT_TRUE("-bots +map de_dust2-2x2 +maxplayers 32" == cmdline.current());

        cmdline.set_param("+maxplayers", 24);
        ASSERT_TRUE("-bots +map de_dust2-2x2 +maxplayers 24" == cmdline.current());

        cmdline.set_param("+map", "cs_assault");
        ASSERT_TRUE("-bots +maxplayers 24 +map cs_assault" == cmdline.current());
    }
}
