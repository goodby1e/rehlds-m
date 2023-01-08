/*
 *  Copyright (C) 2023 the_hunter
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <type_traits>
#include <cerrno>
#include <cstring>
#include <dlfcn.h>
#include <string>
#include <unistd.h>
#include <utility>

namespace cpputils
{
    /**
     * @brief Opaque handle to the module.
     */
    class SysModule;

    /**
     * @brief Returns a textual description of the system error code.
     */
    [[nodiscard]] inline std::string get_error_str(const int error_code)
    {
        return std::string{std::strerror(error_code)}; // NOLINT(concurrency-mt-unsafe)
    }

    /**
     * @brief Returns a textual description of the last system error.
     */
    [[nodiscard]] inline std::string get_last_error_str()
    {
        return get_error_str(errno);
    }

    /**
     * @brief Retrieves the path of the executable file of the current process.
     */
    [[nodiscard]] std::string get_executable_absolute_path();

    /**
     * @brief Retrieves the fully qualified path for the file that contains the specified module.
     *
     * @note The module must have been loaded by the current process.
     */
    [[nodiscard]] std::string get_module_absolute_path(const std::string& filename);

    /**
     * @brief Returns the process ID (PID) obtained from the system.
     */
    [[nodiscard]] inline int get_pid() noexcept
    {
        return ::getpid();
    }

    /**
     * @brief Retrieves the address of an exported function (also known as a procedure)
     * or variable from the specified library.
     */
    template <typename Ret = void*, typename Handle>
    [[nodiscard]] Ret get_proc_address(Handle&& module, const std::string& procedure) noexcept
    {
        if constexpr (std::is_same_v<Ret, void*>) {
            return ::dlsym(std::forward<Handle>(module), procedure.c_str());
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(::dlsym(std::forward<Handle>(module), procedure.c_str()));
        }
    }

    /**
     * @brief Loads the specified module into the address space of the calling process.
     * The specified module may cause other modules to be loaded.
     */
    template <typename Ret = class SysModule*>
    [[nodiscard]] Ret load_module(const char* const filename, const int mode = RTLD_NOW)
    {
        const auto& absolute_path = get_module_absolute_path(filename);

        if constexpr (std::is_same_v<Ret, void*>) {
            return ::dlopen(absolute_path.c_str(), mode);
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(::dlopen(absolute_path.c_str(), mode));
        }
    }

    /**
     * @brief Frees the loaded module and, if necessary, decrements its reference count.
     * When the reference count reaches zero, the module is unloaded from the address space
     * of the calling process and the handle is no longer valid.
     */
    inline bool unload_module(void* const module) noexcept
    {
        return 0 == ::dlclose(module);
    }
}
