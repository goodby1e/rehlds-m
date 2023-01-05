/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "common/object_list.h"
#include "strformat/format.h"
#include <array>
#include <cstdio>
#include <deque>
#include <string>
#include <utility>

namespace rehlds::dedicated
{
    /**
     * @brief Text console wrapper.
     */
    class TextConsole
    {
      public:
        TextConsole(TextConsole&&) = delete;
        TextConsole(const TextConsole&) = delete;
        TextConsole& operator=(TextConsole&&) = delete;
        TextConsole& operator=(const TextConsole&) = delete;
        virtual ~TextConsole() = default;

        [[nodiscard]] static TextConsole& instance();

        template <typename... Args>
        static int print(std::string format, Args&&... args);

        virtual bool init();
        virtual void terminate();
        virtual bool get_line(std::string& text) = 0;
        [[nodiscard]] virtual int width() const = 0;
        virtual void set_title(const std::string& title) = 0;
        virtual void set_status(std::string status) = 0;
        virtual void update_status(bool force);
        void update_status();

        [[nodiscard]] bool initialized() const;
        [[nodiscard]] const std::string& console_text() const;

      protected:
        TextConsole() = default;
        void delete_typed_line();
        void receive_newline();
        void receive_backspace();
        void receive_tab();
        void receive_up_arrow();
        void receive_down_arrow();
        void receive_left_arrow();
        void receive_right_arrow();
        void receive_home();
        void receive_end();
        void receive_escape_sequence(const std::array<std::string::value_type, 3>& sequence);
        void receive_character(std::string::value_type character);
        void receive_text(std::string_view text);

      private:
        using LineBuffer = std::deque<std::string>;

        /* Is the console already initialized? */
        bool initialized_{};

        /* Console text buffer. */
        std::string console_text_{};

        /* Saved input data when scrolling back through command history text buffer. */
        std::string saved_console_text_{};

        /* Command buffer last commands. */
        LineBuffer line_buffer_{};

        /* Current buffer line for up/down arrow. */
        LineBuffer::size_type browse_line_{};

        /* Position in the current input line. */
        std::string::size_type cursor_position_{};

        /* Prints the single command match to console. */
        void process_single_command_match(const common::ObjectList& matches);

        /* Prints the multiple command matches to console. */
        void process_multiple_command_matches(common::ObjectList& matches);
    };

    template <typename... Args>
    int TextConsole::print(std::string format, Args&&... args)
    {
        strformat::print(std::move(format), std::forward<Args>(args)...);
        return std::fflush(stdout);
    }

    inline void TextConsole::update_status()
    {
        update_status(false);
    }

    [[nodiscard]] inline bool TextConsole::initialized() const
    {
        return initialized_;
    }

    [[nodiscard]] inline const std::string& TextConsole::console_text() const
    {
        return console_text_;
    }
}
