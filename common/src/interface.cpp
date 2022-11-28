/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/interface.h"
#include <unordered_map>
#include <cassert>
#include <memory>
#include <utility>

namespace
{
    using namespace rehlds::common;

    // Implementation of the "Construct On First Use" idiom
    [[nodiscard]] auto& get_interface_regs()
    {
        static const auto instance = std::make_unique<std::unordered_map<std::string, InterfaceReg::InstantiateFunc>>();
        return *instance;
    }

    // NOLINTNEXTLINE(readability-identifier-naming)
    extern "C" DLL_EXPORT IBaseInterface* CreateInterface(const char* const name, CreateInterfaceStatus* const status)
    {
        IBaseInterface* interface = nullptr;

        if ((name != nullptr) && (*name != '\0')) {
            const auto& interface_regs = get_interface_regs();

            if (const auto& it = interface_regs.find(name); it != interface_regs.end()) {
                interface = it->second();
            }
        }

        if (status != nullptr) {
            *status = nullptr == interface ? CreateInterfaceStatus::failed : CreateInterfaceStatus::succeeded;
        }

        return interface;
    }
}

namespace rehlds::common
{
    InterfaceReg::InterfaceReg(InstantiateFunc instantiate_func, const std::string& name)
    {
        assert(!name.empty());
        auto& interface_regs = get_interface_regs();
        interface_regs[name] = std::move(instantiate_func);
    }

    CreateInterfaceFn get_factory_this() noexcept
    {
        return &CreateInterface;
    }
}
