/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "console/text_console.h"

namespace rehlds::dedicated
{
    class TextConsoleUnix final : public TextConsole
    {
        friend TextConsole& TextConsole::instance();

      public:
        TextConsoleUnix() = default;
        TextConsoleUnix(TextConsoleUnix&&) = delete;
        TextConsoleUnix(const TextConsoleUnix&) = delete;
        TextConsoleUnix& operator=(TextConsoleUnix&&) = delete;
        TextConsoleUnix& operator=(const TextConsoleUnix&) = delete;
        ~TextConsoleUnix() override;

        bool init() override;
        void terminate() override;
        bool get_line(std::string& text) override;
        [[nodiscard]] int width() const override;
        void set_title(const std::string& title) override;
        void set_status(std::string status) override;
    };
}
