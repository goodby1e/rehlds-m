/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "common/interface.h"
#include "common/platform.h"

namespace rehlds::common
{
    class NO_VTABLE ISystemModule : public IBaseInterface
    {
      public:
        virtual bool init(class ISystemBase* system, int serial, const char* name) = 0;
        virtual void run_frame(double time) = 0;
        virtual void receive_signal(ISystemModule* module, unsigned int signal, void* data) = 0;
        virtual void execute_command(int command_id, const char* command_line) = 0;
        virtual void register_listener(ISystemModule* module) = 0;
        virtual void remove_listener(ISystemModule* module) = 0;
        virtual ISystemBase* get_system() = 0;
        virtual int get_serial() = 0;
        virtual char* get_status_line() = 0;
        virtual char* get_type() = 0;
        virtual char* get_name() = 0;
        virtual int get_state() = 0;
        virtual int get_version() = 0;
        virtual void shutdown() = 0;
    };
}
