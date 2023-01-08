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

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)
#include <type_traits>
#include <process.h>
#include <string>
#include <utility>
#include <Windows.h>

#ifdef _WIN32
  #include <bcrypt.h> // Should be after <Windows.h>
#endif

namespace cpputils
{
    /**
     * @brief Opaque handle to the module.
     */
    class SysModule;

    /**
     * @brief Returns a textual description of the system error code.
     */
    [[nodiscard]] std::string get_error_str(
      ::DWORD error_code, ::WORD lang = LANG_ENGLISH, ::WORD sublang = SUBLANG_ENGLISH_US);

    /**
     * @brief Returns a textual description of the last system error.
     */
    [[nodiscard]] inline std::string get_last_error_str()
    {
        return get_error_str(::GetLastError());
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
        return ::_getpid();
    }

    /**
     * @brief Retrieves the address of an exported function (also known as a procedure)
     * or variable from the specified library.
     */
    template <typename Ret = void*, typename Handle>
    [[nodiscard]] Ret get_proc_address(Handle&& module, const std::string& procedure) noexcept
    {
        if constexpr (std::is_same_v<Handle, ::HMODULE>) {
            return reinterpret_cast<Ret>(::GetProcAddress(std::forward<::HMODULE>(module), procedure.c_str()));
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(
              ::GetProcAddress(reinterpret_cast<::HMODULE>(std::forward<Handle>(module)), procedure.c_str()));
        }
    }

    /**
     * @brief Sets the resolution of the system timer.
     *
     * @param requested_resolution Requested timer resolution in units of 100-nanoseconds.
     * @param set Specifies whether the requested resolution should be established or revoked.
     * @param actual_resolution Points to a variable that receives the actual timer resolution
     * in units of 100-nanoseconds.
     *
     * @return \c STATUS_SUCCESS or an error status, such as \c STATUS_TIMER_RESOLUTION_NOT_SET
     */
    ::NTSTATUS set_timer_resolution(::ULONG requested_resolution, ::BOOLEAN set, ::PULONG actual_resolution);

    /**
     * @brief Sets the resolution of the system timer.
     *
     * @param requested_resolution Requested timer resolution in units of 100-nanoseconds.
     * @param set Specifies whether the requested resolution should be established or revoked.
     *
     * @return \c STATUS_SUCCESS or an error status, such as \c STATUS_TIMER_RESOLUTION_NOT_SET
     */
    ::NTSTATUS set_timer_resolution(::ULONG requested_resolution, ::BOOLEAN set = TRUE);

    /**
     * @brief Suspends the execution of the current thread for a specified interval (similar to the \c Sleep()).
     *
     * @param alertable Specifies whether the delay can be interrupted by the delivery of a user APC.
     * @param delay_interval Points to a value that specifies the absolute
     * or relative time at which the delay is to end. A negative value specifies an
     * interval relative to the current time. The value is expressed in units of 100 nanoseconds.
     * Absolute times track any changes in the system time; relative times are not affected by system time changes.
     *
     * @return \c STATUS_SUCCESS, \c STATUS_ALERTED, \c STATUS_USER_APC, or an error status.
     */
    ::NTSTATUS delay_execution(::BOOL alertable, ::PLARGE_INTEGER delay_interval);

    /**
     * @brief Loads the specified module into the address space of the calling process.
     * The specified module may cause other modules to be loaded.
     */
    template <typename Ret = SysModule*>
    [[nodiscard]] Ret load_module(const char* const filename, const ::DWORD mode = LOAD_WITH_ALTERED_SEARCH_PATH)
    {
        if constexpr (std::is_same_v<Ret, ::HMODULE>) {
            return ::LoadLibraryEx(filename, nullptr, mode);
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(::LoadLibraryEx(filename, nullptr, mode));
        }
    }

    /**
     * @brief Frees the loaded module and, if necessary, decrements its reference count.
     * When the reference count reaches zero, the module is unloaded from the address space
     * of the calling process and the handle is no longer valid.
     */
    template <typename Handle>
    bool unload_module(Handle&& module) noexcept
    {
        if constexpr (std::is_same_v<Handle, ::HMODULE>) {
            return ::FreeLibrary(std::forward<::HMODULE>(module)) != FALSE;
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return ::FreeLibrary(reinterpret_cast<::HMODULE>(std::forward<Handle>(module))) != FALSE;
        }
    }
}
