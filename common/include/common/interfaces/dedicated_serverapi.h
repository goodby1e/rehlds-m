/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "common/interface.h"
#include "common/platform.h"

namespace rehlds::common
{
    constexpr auto* INTERFACE_DEDICATED_SERVER_API = "VENGINE_HLDS_API_VERSION002";

    /**
     * @brief This is the interface exported by the engine to allow a dedicated server front end application to host it.
     */
    class NO_VTABLE IDedicatedServerApi : public IBaseInterface
    {
      public:
        /**
         * @brief Initialize the engine with the specified base directory and interface factories.
         */
        virtual bool init(const char* basedir, const char* cmdline, CreateInterfaceFn launcher_factory,
          CreateInterfaceFn filesystem_factory) = 0;

        /**
         * @brief Shutdown the engine.
         */
        virtual int shutdown() = 0;

        /**
         * @brief Run a frame.
         */
        virtual bool run_frame() = 0;

        /**
         * @brief Insert text into console.
         */
        virtual void add_console_text(const char* text) = 0;

        /**
         * @brief Get current status to display in the HLDS UI (console window title bar, e.g.).
         */
        virtual void update_status(float* fps, int* active_players, int* max_players, char* current_map) = 0;
    };
}
