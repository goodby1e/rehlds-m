/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "common/interface.h"
#include "common/platform.h"

namespace rehlds::common
{
    constexpr auto* INTERFACE_DEDICATED_EXPORTS = "VENGINE_DEDICATEDEXPORTS_API_VERSION001";

    class NO_VTABLE IDedicatedExports : public IBaseInterface
    {
      public:
        virtual void print(const char* text) = 0;
    };
}
