/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "dedicated.h"

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)
  #include "console/text_console.h"
  #include <Windows.h>
  #include <WinSock2.h>

/**
 * @brief Main entry point for dedicated server (Windows).
 */
int main(const int argc, const char* const argv[])
{
    constexpr auto version = MAKEWORD(2, 2);
    ::WSADATA data{};

    if (const auto error = ::WSAStartup(version, &data); error != 0) {
        rehlds::dedicated::TextConsole::print("WSAStartup failed with error: %d", error);
        return -1;
    }

    auto& cmdline = rehlds::dedicated::CommandLine::instance();
    cmdline.create(argc, argv);

    const auto exit_code = start_hlds(cmdline);
    ::WSACleanup();

    return exit_code;
}
#else
/**
 * @brief Main entry point for dedicated server (Linux).
 */
int main(const int argc, const char* const argv[])
{
    auto& cmdline = rehlds::dedicated::CommandLine::instance();
    cmdline.create(argc, argv);

    return start_hlds(cmdline);
}
#endif
