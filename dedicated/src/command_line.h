/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "cpputils/singleton_holder.h"
#include <regex>
#include <string>
#include <utility>

namespace rehlds::dedicated
{
    /**
     * @brief Implements ICommandLine.
     */
    class CommandLine
    {
      public:
        /**
         * @brief Returns a pointer to the instance of the CommandLine class.
         */
        [[nodiscard]] static CommandLine& instance()
        {
            return cpputils::SingletonHolder<CommandLine>::get_instance();
        }

        /**
         * @brief Creates a command line from the passed in string.
         *
         * @note If you pass in a @@filename, then the routine will read settings
         * from a file instead of the command line.
         */
        void create(std::string cmdline);

        /**
         * @brief Creates a command line from the arguments passed in.
         */
        void create(int argc, const char* const* argv);

        /**
         * @brief Returns current command line.
         */
        [[nodiscard]] const std::string& current() const;

        /**
         * @brief Returns the number of command line arguments.
         */
        [[nodiscard]] int count() const;

        /**
         * @brief Search for the parameter in the current commandline.
         */
        [[nodiscard]] bool find_param(std::string regex_pattern) const;

        /**
         * @brief Search for the parameter in the current commandline.
         */
        bool find_param(std::string regex_pattern, std::string& values) const;

        /**
         * @brief Remove specified string (and any args attached to it) from command line.
         */
        void remove_param(std::string param);

        /**
         * @brief Removes parameter from the command line and then appending it with the new value.
         */
        void set_param(std::string param);

        /**
         * @brief Removes parameter from the command line and then appending it with the new value.
         */
        void set_param(std::string param, std::string values);

        /**
         * @brief Removes parameter from the command line and then appending it with the new value.
         */
        void set_param(std::string param, const int value)
        {
            auto value_string = std::to_string(value);
            set_param(std::move(param), std::move(value_string));
        }

      private:
        /* Actual command line. */
        std::string cmdline_{};

        /* Callback function that is called for each parameter in the command line. */
        void build_cmdline(const std::smatch& match);

        /* When the commandline contains @filename, it reads the parameters from that file. */
        void load_params_from_file(const std::string& filename);
    };
}
