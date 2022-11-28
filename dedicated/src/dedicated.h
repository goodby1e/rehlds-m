/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "command_line.h"

namespace rehlds::dedicated
{
    /**
     * @brief Main entry point for dedicated server, shared between win32 and linux.
     */
    int start_hlds(CommandLine& cmdline);
}
