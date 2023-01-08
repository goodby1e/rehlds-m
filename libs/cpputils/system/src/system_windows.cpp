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

#include "cpputils/system_windows.h"
#include <array>

namespace
{
    ::HMODULE get_ntdll_module()
    {
        static auto* const handle = cpputils::load_module<::HMODULE>("ntdll.dll");
        return handle;
    }
}

namespace cpputils
{
    std::string get_error_str(const ::DWORD error_code, const ::WORD lang, const ::WORD sublang)
    {
        std::array<::CHAR, 1024> buffer{};

        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, static_cast<DWORD>(MAKELANGID(lang, sublang)),
          buffer.data(), buffer.size(), nullptr);

        return std::string{buffer.data()};
    }

    std::string get_executable_absolute_path()
    {
        std::array<::CHAR, MAX_PATH> buffer{};
        ::GetModuleFileName(nullptr, buffer.data(), buffer.size());

        return std::string{buffer.data()};
    }

    std::string get_module_absolute_path(const std::string& filename)
    {
        std::array<::CHAR, MAX_PATH> buffer{};

        if (auto* const handle = GetModuleHandle(filename.c_str()); handle != nullptr) {
            ::GetModuleFileName(handle, buffer.data(), buffer.size());
        }

        return std::string{buffer.data()};
    }

    ::NTSTATUS set_timer_resolution(
      const ::ULONG requested_resolution, const ::BOOLEAN set, const ::PULONG actual_resolution)
    {
        using ZwSetTimerResolution = ::NTSTATUS(__stdcall*)(::ULONG, ::BOOLEAN, ::PULONG);

        static auto* const proc =
          cpputils::get_proc_address<ZwSetTimerResolution>(get_ntdll_module(), "ZwSetTimerResolution");

        return proc(requested_resolution, set, actual_resolution);
    }

    ::NTSTATUS set_timer_resolution(const ::ULONG requested_resolution, const ::BOOLEAN set)
    {
        ::ULONG actual_resolution{};
        return set_timer_resolution(requested_resolution, set, &actual_resolution);
    }

    ::NTSTATUS delay_execution(const ::BOOL alertable, const ::PLARGE_INTEGER delay_interval)
    {
        using NtDelayExecution = ::NTSTATUS(__stdcall*)(::BOOL, ::PLARGE_INTEGER);
        static auto* const proc = cpputils::get_proc_address<NtDelayExecution>(get_ntdll_module(), "NtDelayExecution");

        return proc(alertable, delay_interval);
    }
}
