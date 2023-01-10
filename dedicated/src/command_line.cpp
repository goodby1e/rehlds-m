/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "command_line.h"
#include "console/text_console.h"
#include "cpputils/string.h"
#include <type_traits>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

namespace
{
    bool is_valid_param(const std::string_view param)
    {
        return (param.length() > 1) && (('-' == param.front()) || ('+' == param.front()) || ('@' == param.front()));
    }

    void append_cmdline(std::string& cmdline, std::string param)
    {
        param = cpputils::trim(param);

        if (!param.empty()) {
            if (!cmdline.empty()) {
                cmdline.push_back(' ');
            }

            cmdline.append(param);
        }
    }

    std::tuple<std::string, std::string> split_param(std::string param)
    {
        std::string name{};
        std::string values{};
        param = cpputils::trim(param);

        if (is_valid_param(param)) {
            if ('@' == param.front()) {
                name = param.front();
                values = param.substr(1);
            }
            else if (const auto space_pos = param.find_first_of(" \f\n\r\t\v"); std::string::npos == space_pos) {
                name = std::move(param);
            }
            else {
                name = param.substr(0, space_pos);
                values = param.substr(space_pos);
            }
        }

        name = cpputils::trim(name);
        values = cpputils::trim(values);

        return std::make_tuple(std::move(name), std::move(values));
    }

    template <typename Callback, typename... Args>
    void enumerate_params(std::string cmdline, Callback&& func, Args&&... args)
    {
        cmdline = cpputils::trim(cmdline);
        cmdline = cpputils::replace(cmdline, "\f", cpputils::EMPTY);
        cmdline = cpputils::replace(cmdline, "\r", cpputils::EMPTY);
        cmdline = cpputils::replace(cmdline, "\n", " ");

        const std::regex cmdline_regex{R"((^|\s+)[-+@].+?(?=\s+[-+@]|$))"};
        const auto& cmdline_begin = std::sregex_iterator(cmdline.begin(), cmdline.end(), cmdline_regex);
        const auto& cmdline_end = std::sregex_iterator();

        for (auto i = cmdline_begin; i != cmdline_end; ++i) {
            if constexpr (std::is_void_v<std::invoke_result_t<Callback, Args..., decltype(*i)>>) {
                std::invoke(std::forward<Callback>(func), std::forward<Args>(args)..., *i);
            }
            else {
                if (std::invoke(std::forward<Callback>(func), std::forward<Args>(args)..., *i)) {
                    break;
                }
            }
        }
    }
}

namespace rehlds::dedicated
{
    CommandLine::CommandLine(const int argc, const char* const* argv)
    {
        create(argc, argv);
    }

    void CommandLine::create(std::string cmdline)
    {
        cmdline_.clear();
        enumerate_params(std::move(cmdline), &CommandLine::build_cmdline, this);
    }

    void CommandLine::create(const int argc, const char* const* const argv)
    {
        assert(argv);
        std::string cmdline{};

        for (int i = 1; i < argc; ++i) {
            append_cmdline(cmdline, argv[i]);
        }

        create(std::move(cmdline));
    }

    const std::string& CommandLine::current() const
    {
        return cmdline_;
    }

    int CommandLine::count() const
    {
        auto count = 0;

        enumerate_params(current(),
          [&count](const auto&)
          {
              ++count;
          });

        return count;
    }

    bool CommandLine::find_param(std::string regex_pattern) const
    {
        regex_pattern = cpputils::trim(regex_pattern);
        const std::regex regex{regex_pattern, std::regex_constants::icase};
        auto found = false;

        enumerate_params(current(),
          [&regex, &found](const std::smatch& match)
          {
              const auto& [param_name, param_values] = split_param(match.str());
              return (found = std::regex_match(param_name, regex));
          });

        return found;
    }

    bool CommandLine::find_param(std::string regex_pattern, std::string& values) const
    {
        regex_pattern = cpputils::trim(regex_pattern);
        const std::regex regex{regex_pattern, std::regex_constants::icase};
        auto found = false;

        enumerate_params(current(),
          [&regex, &values, &found](const std::smatch& match)
          {
              const auto& [param_name, param_values] = split_param(match.str());
              found = std::regex_match(param_name, regex);

              if (found) {
                  values = param_values;
              }

              return found;
          });

        return found;
    }

    void CommandLine::remove_param(std::string param)
    {
        param = cpputils::trim(param);
        bool found{};

        do {
            found = false;
            enumerate_params(current(),
              [this, &found, &param](const std::smatch& match)
              {
                  if (const auto& [name, values] = split_param(match.str()); cpputils::equal_ignore_case(name, param)) {
                      const auto start_post = static_cast<std::string::size_type>(match.position());
                      const auto count = static_cast<std::string::size_type>(match.length());
                      found = (start_post != std::string::npos) && (count != std::string::npos);

                      if (found) {
                          cmdline_.erase(start_post, count + (0 == start_post ? 1 : 0));
                      }
                  }
              });
        }
        while (found);
    }

    void CommandLine::set_param(std::string param)
    {
        param = cpputils::trim(param);

        if (is_valid_param(param)) {
            const auto& [param_name, param_values] = split_param(param);
            remove_param(param_name);
            append_cmdline(cmdline_, param_name);
        }
    }

    void CommandLine::set_param(std::string param, std::string values)
    {
        param = cpputils::trim(param);

        if (is_valid_param(param)) {
            const auto& [param_name, param_values] = split_param(param);
            remove_param(param_name);
            append_cmdline(cmdline_, param_name);
            append_cmdline(cmdline_, std::move(values));
        }
    }

    void CommandLine::build_cmdline(const std::smatch& match)
    {
        if (auto [name, values] = split_param(match.str()); name == "@") {
            load_params_from_file(values);
        }
        else if (!name.empty()) {
            set_param(std::move(name), std::move(values));
        }
    }

    void CommandLine::load_params_from_file(const std::string& filename)
    {
        std::ifstream file_stream{filename};

        if (!file_stream.good()) {
            TextConsole::print("\n\nParameter file '{}' not found, skipping...", filename.c_str());
            return;
        }

        std::stringstream buffer{};
        buffer << file_stream.rdbuf();
        file_stream.close();

        enumerate_params(buffer.str(), &CommandLine::build_cmdline, this);
    }
}
