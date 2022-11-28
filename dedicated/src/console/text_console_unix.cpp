/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "console/text_console_unix.h"
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <array>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <streambuf>
#include <termios.h>
#include <unistd.h>

namespace
{
    ::termios termios_new{};
    ::termios termios_stored{};

    std::ofstream tty{};
    std::streambuf* buffer_stored{};

    struct BlockTty
    {
        BlockTty()
        {
            ::sigemptyset(&sig_set_);
            ::sigaddset(&sig_set_, SIGTTOU);
            ::sigaddset(&sig_set_, SIGTTIN);
            ::sigprocmask(SIG_BLOCK, &sig_set_, nullptr);
        }

        BlockTty(BlockTty&&) = delete;
        BlockTty(const BlockTty&) = delete;
        BlockTty& operator=(BlockTty&&) = delete;
        BlockTty& operator=(const BlockTty&) = delete;

        ~BlockTty()
        {
            ::sigprocmask(SIG_UNBLOCK, &sig_set_, nullptr);
        }

      private:
        ::sigset_t sig_set_{};
    };

    void init_term()
    {
        ::tcgetattr(STDIN_FILENO, &termios_stored);
        std::memcpy(&termios_new, &termios_stored, sizeof(termios));

        // Disable canonical mode, echo, and set the buffer size to 1 byte
        termios_new.c_lflag &= ~static_cast<::tcflag_t>(ICANON | ECHO);
        termios_new.c_cc[VMIN] = 1;
        termios_new.c_cc[VTIME] = 0;

        ::tcsetattr(STDIN_FILENO, TCSANOW, &termios_new);
    }

    void init_tty()
    {
        // This code is for echo-ing key presses to the connected tty (which is != STDOUT)
        const auto* term_id = ::ctermid(nullptr); // cppcheck-suppress ctermidCalled

        if ((nullptr == term_id) || ('\0' == term_id[0]) || (0 == ::isatty(STDIN_FILENO))) {
            term_id = "/dev/null";
        }

        // Turn buffering off
        tty.rdbuf()->pubsetbuf(nullptr, 0);
        tty.open(term_id, std::ios_base::out);

        if (tty.good() && tty.is_open()) {
            buffer_stored = std::cout.rdbuf(tty.rdbuf());
        }
        else {
            rehlds::dedicated::TextConsole::print("Unable to open TTY (%s) for output.\n", term_id);
        }
    }

    bool keyboard_hit()
    {
        std::array<pollfd, 1> descriptor{{{0, POLLIN, 0}}};
        return ::poll(descriptor.data(), 1, 0) > 0;
    }
}

namespace rehlds::dedicated
{
    TextConsole& TextConsole::instance()
    {
        static const auto instance = std::unique_ptr<TextConsoleUnix>{new TextConsoleUnix{}}; //-V824
        return *instance;
    }

    TextConsoleUnix::~TextConsoleUnix()
    {
        terminate();
    }

    bool TextConsoleUnix::init()
    {
        if (initialized()) {
            return true;
        }

        if (!TextConsole::init()) {
            return false;
        }

        const BlockTty block_tty{};
        init_term();
        init_tty();

        return true;
    }

    void TextConsoleUnix::terminate()
    {
        if (initialized()) {
            const BlockTty block_tty{};
            ::tcsetattr(STDIN_FILENO, TCSANOW, &termios_stored);

            if (nullptr != buffer_stored) {
                std::cout.rdbuf(buffer_stored);
                buffer_stored = nullptr;
            }

            if (tty.is_open()) {
                tty.close();
            }
        }

        TextConsole::terminate();
    }

    bool TextConsoleUnix::get_line(std::string& text)
    {
        // Early return for 99.999% case :)
        if (!keyboard_hit()) {
            return false;
        }

        const BlockTty block_tty{};
        char character{};

        while (keyboard_hit() && (::read(STDIN_FILENO, &character, 1) > 0)) {
            switch (character) {
            case '\n': {
                text = console_text();
                receive_newline();
                return true;
            }

            case '\x1B': {
                if (std::array<std::string::value_type, 3> sequence{'\x1B', '\0', '\0'};
                    ::read(STDIN_FILENO, &sequence[1], sequence.size() - 1) > 0) {
                    receive_escape_sequence(sequence);
                }
                break;
            }

            case 127:
            case '\b': receive_backspace(); break;
            case '\t': receive_tab(); break;
            case '\0': continue;
            default: receive_character(character); break;
            }
        }

        return false;
    }

    int TextConsoleUnix::width() const
    {
        errno = 0;
        auto width = -1;

        if (winsize win_size{}; 0 == ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size)) {
            width = win_size.ws_col;
        }

        if (width <= 0) {
            if (const auto* const columns = ::getenv("COLUMNS"); (columns != nullptr) && (columns[0] != '\0')) {
                width = static_cast<int>(std::strtol(columns, nullptr, 10));
            }
        }

        if ((errno != 0) || (width < 10)) {
            width = 80;
        }

        return width;
    }

    void TextConsoleUnix::set_title(const std::string& /* unused */)
    {
    }

    void TextConsoleUnix::set_status(const std::string /* unused */) //-V801
    {
    }
}
