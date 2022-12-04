/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/interfaces/dedicated_exports.h"
#include "common/interface.h"
#include "common/platform.h"
#include "console/text_console.h"

using namespace rehlds::common;
using namespace rehlds::dedicated;

namespace
{
    class DedicatedExports final : public IDedicatedExports
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
            TextConsole::print(text);
        }
    }
}

EXPOSE_INTERFACE_FN(&DedicatedExports::instance, IDedicatedExports, INTERFACE_DEDICATED_EXPORTS)
