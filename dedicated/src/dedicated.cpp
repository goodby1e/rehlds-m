/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "dedicated.h"
#include "arguments.h"
#include "command_line.h"
#include "common/hlds_module.h"
#include "common/interfaces/dedicated_serverapi.h"
#include "common/interfaces/filesystem.h"
#include "common/platform.h"
#include "console/text_console.h"
#include "sleep.h"
#include <cassert>
#include <string>

using namespace rehlds::common;
using namespace rehlds::dedicated;

namespace
{
    [[nodiscard]] bool load_modules()
    {
        if (!get_engine_module().load()) {
            TextConsole::print("Unable to load engine module, image is corrupt.\n");
            return false;
        }

        if (!get_filesystem_module().load()) {
            TextConsole::print("Unable to load filesystem module, image is corrupt.\n");
            return false;
        }

        return true;
    }

    [[nodiscard]] IDedicatedServerApi* get_engine_api(HldsModule& engine_module)
    {
        auto* const engine_api = engine_module.get_interface<IDedicatedServerApi>(INTERFACE_DEDICATED_SERVER_API);

        if (nullptr == engine_api) {
            TextConsole::print("Failed to retrieve \"{}\" interface.\n", INTERFACE_DEDICATED_SERVER_API);
        }

        return engine_api;
    }

    [[nodiscard]] IFileSystem* get_filesystem(HldsModule& filesystem_module)
    {
        auto* const filesystem = filesystem_module.get_interface<IFileSystem>(INTERFACE_FILESYSTEM);

        if (nullptr == filesystem) {
            TextConsole::print("Failed to retrieve \"{}\" interface.\n", INTERFACE_FILESYSTEM);
        }

        return filesystem;
    }

    void init_cmdline(CommandLine& cmdline)
    {
#ifdef _WIN32
        cmdline.set_param("-console");
#endif
        cmdline.set_param("-steam");
    }

    bool init_console(TextConsole& console)
    {
        if (!console.init()) {
            TextConsole::print("Failed to initialize console.\n");
            return false;
        }

        return true;
    }

    bool init_engine(IDedicatedServerApi* const engine_api, const char* const cmdline,
      const CreateInterfaceFn launcher_factory, const CreateInterfaceFn filesystem_factory)
    {
        if (!engine_api->init(".", cmdline, launcher_factory, filesystem_factory)) {
            TextConsole::print("Failed to initialize engine API.\n");
            return false;
        }

        return true;
    }

    /**
     * @brief Server loop.
     */
    ATTR_OPTIMIZE_HOT NO_INLINE void run_server(IDedicatedServerApi* const engine_api)
    {
        assert(engine_api);

        std::string text{};
        auto& console = TextConsole::instance();

        do {
            if (console.get_line(text) && (!text.empty())) {
                text.push_back('\n');
                engine_api->add_console_text(text.c_str());
            }

#ifdef _WIN32
            console.update_status();
#endif
            sys_sleep();
        }
        while (engine_api->run_frame());
    }
}

namespace rehlds::dedicated
{
    int start_hlds(CommandLine& cmdline)
    {
        if (!load_modules()) {
            return -1;
        }

        auto& console = TextConsole::instance();
        auto& engine_module = get_engine_module();
        auto& filesystem_module = get_filesystem_module();
        auto* const engine_api = get_engine_api(engine_module);
        auto* const filesystem = get_filesystem(filesystem_module);

        if ((nullptr == engine_api) || (nullptr == filesystem)) {
            return -1;
        }

        filesystem->mount();
        init_cmdline(cmdline);
        process_cmdline_arguments(cmdline);

        auto* const launcher_factory = get_factory_this();
        auto* const filesystem_factory = filesystem_module.get_factory();
        const auto* const current_cmdline = cmdline.current().c_str();

        if (init_engine(engine_api, current_cmdline, launcher_factory, filesystem_factory) && init_console(console)) {
            run_server(engine_api);
            engine_api->shutdown();
        }

        filesystem->unmount();
        console.terminate();

        return 0;
    }
}
