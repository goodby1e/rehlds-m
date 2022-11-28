/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/system.h"
#include <cstdlib>
#include <memory>
#include <vector>

namespace rehlds::common
{
    void at_exit(std::function<void()> callback)
    {
        static std::unique_ptr<std::vector<std::function<void()>>> at_exit_callbacks{};

        constexpr auto* run_callbacks = +[]
        {
            if (nullptr == at_exit_callbacks) {
                return;
            }

            for (const auto& function : *at_exit_callbacks) {
                function();
            }

            at_exit_callbacks.reset();
        };

        if (nullptr == at_exit_callbacks) {
            at_exit_callbacks = std::make_unique<std::vector<std::function<void()>>>();
            std::atexit(run_callbacks); // NOLINT(cert-err33-c)
        }

        at_exit_callbacks->emplace_back(std::move(callback));
    }
}

#ifdef _WIN32
namespace rehlds::common
{
    std::string get_module_absolute_path(const char* const filename)
    {
        std::array<::CHAR, MAX_PATH> buffer{};

        if (nullptr == filename) {
            ::GetModuleFileName(nullptr, buffer.data(), buffer.size());
        }
        else if (auto* const handle = GetModuleHandle(filename); handle != nullptr) {
            ::GetModuleFileName(handle, buffer.data(), buffer.size());
        }

        return std::string{buffer.data()};
    }
}

#else
  #include <filesystem>

namespace rehlds::common
{
    std::string get_module_absolute_path(const char* const filename)
    {
        if (nullptr == filename) {
            return canonical(std::filesystem::path{"/proc/self/exe"}).string();
        }

        std::filesystem::path path{filename};
        path.make_preferred();

        if (!path.is_absolute()) {
            auto absolute_path = std::filesystem::current_path();
            path = (absolute_path /= path);
        }

        return canonical(path).string();
    }
}
#endif
