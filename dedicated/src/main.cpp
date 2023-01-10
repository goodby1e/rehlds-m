/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "console/text_console.h"
#include "dedicated.h"
#include <clocale>

namespace
{
    void set_locale()
    {
#ifdef _WIN32
        constexpr auto* locale = ".UTF-8";
#else
        constexpr auto* locale = "C.UTF-8";
#endif
        // NOLINTNEXTLINE(concurrency-mt-unsafe)
        if (nullptr == std::setlocale(LC_ALL, locale)) {
            rehlds::dedicated::TextConsole::print("WARNING! Could not set locale.\n");
        }
    }

    [[nodiscard]] rehlds::dedicated::CommandLine create_cmdline(const int argc, const char* const* argv)
    {
        rehlds::dedicated::CommandLine cmdline{argc, argv};

#ifdef _WIN32
        cmdline.set_param("-console");
#endif
        cmdline.set_param("-steam");

        return cmdline;
    }
}

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)
  #include <Windows.h>
  #include <WinSock2.h>

/**
 * @brief Main entry point for dedicated server (Windows).
 */
int main(const int argc, const char* const argv[])
{
    set_locale();

    constexpr auto version = MAKEWORD(2, 2);
    ::WSADATA data{};

    if (const auto error = ::WSAStartup(version, &data); error != 0) {
        rehlds::dedicated::TextConsole::print("WSAStartup failed with error: {}", error);
        return -1;
    }

    const auto cmdline = create_cmdline(argc, argv);
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
    set_locale();
    const auto cmdline = create_cmdline(argc, argv);

    return start_hlds(cmdline);
}
#endif
