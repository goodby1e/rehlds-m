/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/hlds_module.h"
#include <boost/format.hpp>
#include <boost/throw_exception.hpp>
#include <cstdlib>

namespace
{
    rehlds::common::ISystemBase* get_system()
    {
        return rehlds::common::get_engine_module().get_system();
    }
}

namespace boost
{
    void throw_exception(const std::exception& e)
    {
        if (auto* const system = get_system(); system != nullptr) {
            std::string error{e.what()};
            error.push_back('\n');
            system->print_error(error.c_str());
        }

        std::exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
    }

    void throw_exception(const std::exception& e, const source_location& loc)
    {
        if (auto* const system = get_system(); system != nullptr) {
            const auto error = (format("%s\n Source location: %s\n") % e.what() % loc.to_string()).str();
            system->print_error(error.c_str());
        }

        std::exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
    }
}
