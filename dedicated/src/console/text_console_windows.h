/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "console/text_console.h"

namespace rehlds::dedicated
{
    class TextConsoleWindows final : public TextConsole
    {
        friend TextConsole& TextConsole::instance();

      public:
        TextConsoleWindows(TextConsoleWindows&&) = delete;
        TextConsoleWindows(const TextConsoleWindows&) = delete;
        TextConsoleWindows& operator=(TextConsoleWindows&&) = delete;
        TextConsoleWindows& operator=(const TextConsoleWindows&) = delete;
        ~TextConsoleWindows() override;

        bool init() override;
        void terminate() override;
        bool get_line(std::string& text) override;
        [[nodiscard]] int width() const override;
        void set_title(const std::string& title) override;
        void set_status(std::string status) override;

      private:
        TextConsoleWindows();
    };
}
