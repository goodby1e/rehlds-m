/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

// NOLINTNEXTLINE(clang-diagnostic-unused-macros)
#define WIN32_LEAN_AND_MEAN

#include "console/text_console_windows.h"
#include "cpputils/singleton_holder.h"
#include <array>
#include <chrono>
#include <iostream>
#include <vector>
#include <Windows.h>

namespace
{
    ::HWND handle_window{};
    ::HANDLE handle_input{};
    ::HANDLE handle_output{};

    constexpr std::array<::WORD, 80> create_status_line_attributes(const ::WORD value)
    {
        std::array<::WORD, 80> attributes{};

        for (auto& attribute : attributes) {
            attribute = value; // cppcheck-suppress useStlAlgorithm
        }

        return attributes;
    }
}

namespace rehlds::dedicated
{
    TextConsole& TextConsole::instance()
    {
        return cpputils::SingletonHolder<TextConsoleWindows>::get_instance();
    }

    TextConsoleWindows::TextConsoleWindows()
    {
        // Reserve row for status line
        print("\n\n");
    }

    TextConsoleWindows::~TextConsoleWindows()
    {
        terminate();
    }

    bool TextConsoleWindows::init()
    {
        if (initialized()) {
            return true;
        }

        if (!TextConsole::init()) {
            return false;
        }

        handle_window = ::GetConsoleWindow();
        handle_input = ::GetStdHandle(STD_INPUT_HANDLE);
        handle_output = ::GetStdHandle(STD_OUTPUT_HANDLE);
        set_title("GoldSrc Dedicated Server");

        return true;
    }

    void TextConsoleWindows::terminate()
    {
        TextConsole::terminate();
    }

    bool TextConsoleWindows::get_line(std::string& text)
    {
        ::DWORD num_events = 0;

        if (FALSE == GetNumberOfConsoleInputEvents(handle_input, &num_events) || 0 == num_events) {
            return false;
        }

        ::DWORD num_read = 0;
        std::vector<::INPUT_RECORD> input{};
        input.resize(num_events * 2 + 1);

        if (FALSE == ::ReadConsoleInput(handle_input, input.data(), input.size(), &num_read) || 0 == num_read) {
            return false;
        }

        for (const auto& [EventType, Event] : input) {
            if (EventType != KEY_EVENT || !Event.KeyEvent.bKeyDown) {
                continue;
            }

            const auto key_code = Event.KeyEvent.wVirtualKeyCode;
            const auto character = Event.KeyEvent.uChar.AsciiChar;

            if (VK_RETURN == key_code || '\r' == character || '\n' == character) {
                text = console_text();
                receive_newline();

                return true;
            }

            switch (key_code) {
                case VK_BACK: receive_backspace(); continue;
                case VK_TAB: receive_tab(); continue;
                case VK_UP: receive_up_arrow(); continue;
                case VK_DOWN: receive_down_arrow(); continue;
                case VK_LEFT: receive_left_arrow(); continue;
                case VK_RIGHT: receive_right_arrow(); continue;
                case VK_HOME: receive_home(); continue;
                case VK_END: receive_end(); continue;
                default: break;
            }

            switch (character) {
                case '\t': receive_tab(); break;
                case '\b':
                case 127: receive_backspace(); break;
                default: receive_character(character); break;
            }
        }

        return false;
    }

    int TextConsoleWindows::width() const
    {
        ::SHORT width = -1;
        ::CONSOLE_SCREEN_BUFFER_INFO info{};

        if (::GetConsoleScreenBufferInfo(handle_output, &info) != FALSE) {
            width = info.dwSize.X;
        }

        if (width < 10) {
            width = 80;
        }

        return width;
    }

    void TextConsoleWindows::set_title(const std::string& title)
    {
        ::SetConsoleTitle(title.c_str());
    }

    void TextConsoleWindows::set_status(std::string status)
    {
        constexpr auto coordinates = ::COORD{0, 0};
        constexpr auto color = ::WORD{FOREGROUND_GREEN | FOREGROUND_INTENSITY};
        constexpr auto attributes = create_status_line_attributes(color);

        status.resize(attributes.size());
        auto written = ::DWORD{0};

        ::WriteConsoleOutputAttribute(handle_output, attributes.data(), attributes.size(), coordinates, &written);
        ::WriteConsoleOutputCharacter(handle_output, status.c_str(), attributes.size(), coordinates, &written);
    }
}
