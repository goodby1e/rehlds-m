/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "console/text_console.h"
#include "common/hlds_module.h"
#include "common/interfaces/dedicated_serverapi.h"
#include "cpputils/string.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cstring>
#include <iostream>
#include <utility>

using namespace rehlds::common;

namespace
{
    constexpr auto MAX_BUFFER_LINES = 255;
    IDedicatedServerApi* engine_api{};

    std::pair<std::string_view, std::string_view> get_smallest_longest_commands(const ObjectList& commands)
    {
        assert(!commands.empty());

        const auto& [smallest, longest] = std::minmax_element(commands.cbegin(), commands.cend(),
          [](const void* const lhs, const void* const rhs)
          {
              return std::strlen(static_cast<const char*>(lhs)) < std::strlen(static_cast<const char*>(rhs));
          });

        return std::make_pair(
          std::string_view{static_cast<const char*>(*smallest)}, std::string_view{static_cast<const char*>(*longest)});
    }
}

namespace rehlds::dedicated
{
    namespace detail
    {
        ISystemBase* system{};
    }

    bool TextConsole::init()
    {
        if (initialized_) {
            return true;
        }

        initialized_ = true;
        console_text_.clear();
        saved_console_text_.clear();
        line_buffer_.clear();
        cursor_position_ = 0;
        browse_line_ = 0;
        detail::system = get_engine_module().get_system();

        auto& engine_module = get_engine_module();
        engine_api = engine_module.get_interface<IDedicatedServerApi>(INTERFACE_DEDICATED_SERVER_API);
        initialized_ = engine_api != nullptr;

        return initialized_;
    }

    void TextConsole::terminate()
    {
        initialized_ = false;
    }

    void TextConsole::update_status(const bool force)
    {
        constexpr std::chrono::milliseconds update_interval{500};
        static std::chrono::time_point<std::chrono::system_clock> time_last_update{};

        if ((!force) && ((std::chrono::system_clock::now() - time_last_update) < update_interval)) {
            return;
        }

        auto fps = 0.F;
        auto active_players = 0;
        auto maximum_players = 0;
        std::array<char, 32> map_name{};
        engine_api->update_status(&fps, &active_players, &maximum_players, map_name.data());

        const auto status = cpputils::format(
          "FPS: {:.1f} | Players: {:d}/{:d} | Map: {}", fps, active_players, maximum_players, map_name.data());

        set_status(status);
        time_last_update = std::chrono::system_clock::now();
    }

    void TextConsole::delete_typed_line()
    {
        if (!console_text_.empty()) {
            receive_end();

            for (auto i = console_text_.length(); i > 0; --i) {
                std::cout << "\b \b";
            }
        }
    }

    void TextConsole::receive_newline()
    {
        std::cout << '\n';

        // Cache line in buffer, but only if it's not a duplicate of the previous line.
        if ((!console_text_.empty()) && (line_buffer_.empty() || (line_buffer_.back() != console_text_))) {
            if (MAX_BUFFER_LINES == line_buffer_.size()) {
                line_buffer_.pop_front();
            }

            line_buffer_.push_back(console_text_);
        }

        cursor_position_ = 0;
        console_text_.clear();
        browse_line_ = line_buffer_.size();
    }

    void TextConsole::receive_backspace()
    {
        if ((0 == cursor_position_) || console_text_.empty()) {
            return;
        }

        --cursor_position_;
        console_text_.erase(cursor_position_, 1);

        std::cout << '\b';
        std::cout << (console_text_.c_str() + cursor_position_);
        std::cout << ' ';

        for (auto i = console_text_.length() + 1; i > cursor_position_; --i) {
            std::cout << '\b';
        }
    }

    void TextConsole::receive_tab()
    {
        if ((nullptr == detail::system) || console_text_.empty()) {
            return;
        }

        ObjectList matches{};
        detail::system->get_command_matches(console_text_.c_str(), &matches);

        if (matches.empty()) {
            return;
        }

        if (1 == matches.size()) {
            process_single_command_match(matches);
        }
        else {
            process_multiple_command_matches(matches);
        }

        cursor_position_ = console_text_.length();
    }

    void TextConsole::receive_up_arrow()
    {
        if ((0 == browse_line_) || line_buffer_.empty()) {
            return;
        }

        if (line_buffer_.size() == browse_line_) {
            saved_console_text_ = console_text_;
        }

        delete_typed_line();
        --browse_line_;

        console_text_ = line_buffer_[browse_line_];
        std::cout << console_text_;

        cursor_position_ = console_text_.length();
    }

    void TextConsole::receive_down_arrow()
    {
        if (line_buffer_.empty() || (browse_line_ >= line_buffer_.size())) {
            return;
        }

        delete_typed_line();
        ++browse_line_;

        if (line_buffer_.size() == browse_line_) {
            console_text_ = saved_console_text_;
        }
        else {
            console_text_ = line_buffer_[browse_line_];
        }

        std::cout << console_text_;
        cursor_position_ = console_text_.length();
    }

    void TextConsole::receive_left_arrow()
    {
        if (0 == cursor_position_) {
            return;
        }

        std::cout << '\b';
        --cursor_position_;
    }

    void TextConsole::receive_right_arrow()
    {
        if (console_text_.length() == cursor_position_) {
            return;
        }

        std::cout << console_text_[cursor_position_];
        ++cursor_position_;
    }

    void TextConsole::receive_home()
    {
        while (cursor_position_ > 0) {
            receive_left_arrow();
        }
    }

    void TextConsole::receive_end()
    {
        while (cursor_position_ < console_text_.length()) {
            receive_right_arrow();
        }
    }

    void TextConsole::receive_escape_sequence(const std::array<std::string::value_type, 3>& sequence)
    {
        switch (sequence[2]) {
        case 'A': receive_up_arrow(); break;
        case 'B': receive_down_arrow(); break;
        case 'C': receive_right_arrow(); break;
        case 'D': receive_left_arrow(); break;
        case 'F': receive_end(); break;
        case 'H': receive_home(); break;
        default: break;
        }
    }

    void TextConsole::receive_character(const std::string::value_type character)
    {
        if (std::isprint(static_cast<unsigned char>(character)) != 0) {
            console_text_.insert(cursor_position_, 1, character);

            std::cout << (console_text_.c_str() + cursor_position_);
            ++cursor_position_;

            for (auto i = console_text_.length(); i > cursor_position_; --i) {
                std::cout << '\b';
            }
        }
    }

    void TextConsole::receive_text(const std::string_view text)
    {
        for (const auto character : text) {
            receive_character(character);
        }
    }

    void TextConsole::process_single_command_match(const ObjectList& matches)
    {
        assert(1 == matches.size());

        const auto* const match = static_cast<const char*>(matches[0]);
        const auto* const rest = match + console_text_.length();

        console_text_.append(rest);
        std::cout << rest;

        console_text_.push_back(' ');
        std::cout << ' ';
    }

    void TextConsole::process_multiple_command_matches(ObjectList& matches)
    {
        assert(matches.size() > 1);
        const auto& [smallest, longest] = get_smallest_longest_commands(matches);

        delete_typed_line();
        std::cout << '\n';

        std::string common{smallest};
        common = cpputils::lower(common);

        const auto total_columns = static_cast<std::size_t>(width() - 1) / (longest.length() + 1);
        std::size_t current_column = 0;

        for (const auto* const match : matches) {
            ++current_column;

            if (current_column > total_columns) {
                current_column = 1;
                std::cout << '\n';
            }

            std::string command{static_cast<const char*>(match)};
            command = cpputils::lower(command);
            std::cout << cpputils::format("{:<{}}  ", command, longest.length());

            const auto& [mismatch1, mismatch2] = std::mismatch(common.cbegin(), common.cend(), command.cbegin());
            common.erase(mismatch1, common.cend());
        }

        std::cout << '\n';
        console_text_ = std::move(common);
        std::cout << console_text_;
    }
}
