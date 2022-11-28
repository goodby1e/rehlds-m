/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/interfaces/dedicated_exports.h"
#include "common/interface.h"
#include "common/platform.h"
#include <cstdio>

namespace
{
    class DedicatedExports final : public rehlds::common::IDedicatedExports
    {
      public:
        [[nodiscard]] static IDedicatedExports* instance()
        {
            static DedicatedExports instance{};
            return &instance;
        }

        STACK_ALIGN void print(const char* text) override;

      private:
        DedicatedExports() = default;
    };

    void DedicatedExports::print(const char* const text)
    {
        if ((text != nullptr) && (*text != '\0')) {
            std::printf("%s", text);
            std::fflush(stdout);
        }
    }
}

EXPOSE_INTERFACE_FN(&DedicatedExports::instance, IDedicatedExports, rehlds::common::INTERFACE_DEDICATED_EXPORTS)
