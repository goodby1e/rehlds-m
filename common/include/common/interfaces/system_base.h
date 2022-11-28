/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "common/interfaces/filesystem.h"
#include "common/interfaces/system_module.h"
#include "common/interfaces/vgui_module.h"
#include "common/object_container.h"
#include "common/platform.h"

namespace rehlds::common
{
    constexpr auto* INTERFACE_SYSTEM_BASE = "basesystem002";

    class NO_VTABLE ISystemBase : public virtual ISystemModule
    {
        // TODO: remove virtual inheritance here

      public:
        virtual double get_time() = 0;
        virtual unsigned int get_tick() = 0;
        virtual void set_fps(float fps) = 0;
        virtual void print(const char* format, ...) = 0;
        virtual void print_debug(const char* format, ...) = 0;
        virtual void redirect_output(char* buffer, int max_size) = 0;
        virtual IFileSystem* get_file_system() = 0;
        virtual void* load_file(const char* name, int* length) = 0;
        virtual void free_file(void* handle) = 0;
        virtual void set_title(const char* title) = 0;
        virtual void set_status(const char* status) = 0;
        virtual void show_console(bool visible) = 0;
        virtual void log_console(const char* filename) = 0;
        virtual bool init_vgui(IVGuiModule* module) = 0;
#ifdef _WIN32
        virtual class Panel* get_panel() = 0;
#endif
        virtual bool register_command(const char* name, ISystemModule* module, int command_id) = 0;
        virtual void get_command_matches(const char* string, IObjectContainer* match_list) = 0;
        virtual void execute_string(const char* commands) = 0;
        virtual void execute_file(const char* filename) = 0;
        virtual void print_error(const char* format, ...) = 0;
        virtual const char* check_param(const char* param) = 0;
        virtual bool add_module(ISystemModule* module, const char* name) = 0;
        virtual ISystemModule* get_module(const char* interface, const char* library, const char* instance) = 0;
        virtual bool remove_module(ISystemModule* module) = 0;
        virtual void stop() = 0;
        virtual const char* get_base_dir() = 0;
    };
}
