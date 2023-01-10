/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "arguments.h"
#include "common/hlds_module.h"
#include "console/text_console.h"
#include "cpputils/system.h"
#include "sleep.h"
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

using namespace rehlds::common;
using namespace rehlds::dedicated;

namespace
{
    void conclearlog(const CommandLine& cmdline)
    {
        if (cmdline.find_param("[-\\+]condebug") && cmdline.find_param("-conclearlog")) {
#ifdef _WIN32
            std::error_code error_code{};
            std::filesystem::remove("qconsole.log", error_code);
#else
            if (std::string game{}; cmdline.find_param("-game", game) && (!game.empty())) {
                std::error_code error_code{};
                std::filesystem::remove(game + "/qconsole.log", error_code);
            }
#endif
        }
    }

    void ignoresigint(const CommandLine& cmdline)
    {
        if (cmdline.find_param("-ignoresigint") && (SIG_ERR == std::signal(SIGINT, SIG_IGN))) {
            TextConsole::print("WARNING! -ignoresigint: Failed to set signal handler.\n");
        }
    }

    void pidfile(const CommandLine& cmdline)
    {
        if (std::string pid_file{}; cmdline.find_param("-pidfile", pid_file) && (!pid_file.empty())) {
            std::ofstream filestream{};
            filestream.open(pid_file, std::ios_base::out | std::ios_base::trunc);

            if (filestream.good() && filestream.is_open()) {
                filestream << cpputils::get_pid() << '\n';
                filestream.close();
            }
            else {
                TextConsole::print("Warning: unable to open PID file ({})\n", pid_file);
            }
        }
    }

    void pingboost(const CommandLine& cmdline)
    {
        auto& engine_module = get_engine_module();
        sys_sleep = &sleep_thread_millisecond;
        net_sleep = engine_module.get_proc_address<NetSleep>("NET_Sleep_Timeout");

        if (std::string ping_boost{}; cmdline.find_param("-pingboost", ping_boost) && (!ping_boost.empty())) {
            switch (std::strtol(ping_boost.c_str(), nullptr, 10)) {
#ifdef _WIN32
                case 4: {
                    cpputils::set_timer_resolution(1);
                    sys_sleep = &sleep_delay_execution;
                    break;
                }
#else
                case 1: {
                    std::signal(SIGALRM, &sigalrm_handler);
                    sys_sleep = &sleep_timer;
                    break;
                }
                case 2: {
                    sys_sleep = &sleep_poll;
                    break;
                }
                case 4: {
                    sys_sleep = &sleep_thread_microsecond;
                    break;
                }
#endif
                case 3: {
                    sys_sleep = &sleep_net;
                    break;
                }
                case 5: {
                    sys_sleep = &thread_yield;
                    break;
                }
                default: {
                    sys_sleep = &sleep_thread_millisecond;
                    break;
                }
            }
        }
    }
}

namespace rehlds::dedicated
{
    void process_cmdline_arguments(const CommandLine& cmdline)
    {
        conclearlog(cmdline);
        ignoresigint(cmdline);
        pidfile(cmdline);
        pingboost(cmdline);
    }
}
