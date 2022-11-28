/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include <type_traits>
#include <array>
#include <functional>
#include <string>
#include <utility>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)
  #include <process.h>
  #include <Windows.h>

namespace rehlds::common
{
    /**
     * @brief Registers the callback to be called on normal program termination
     * (via \c std::exit() or returning from the main function).
     *
     * @param callback Lambda to be called on normal program termination.
     */
    void at_exit(std::function<void()> callback);

    /**
     * @brief Retrieves the fully qualified path for the file that contains the specified module.
     *
     * @note The module must have been loaded by the current process.
     */
    [[nodiscard]] std::string get_module_absolute_path(const char* filename);

    /**
     * @brief Returns the process ID (PID) obtained from the system.
     */
    [[nodiscard]] inline int get_pid() noexcept
    {
        return ::_getpid();
    }

    /**
     * @brief Returns a textual description of the system error code.
     */
    [[nodiscard]] inline std::string get_error_str(
      const ::DWORD error_code, const unsigned lang = LANG_ENGLISH, const unsigned sublang = SUBLANG_ENGLISH_US)
    {
        std::array<::CHAR, 1024> buffer{};

        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, static_cast<DWORD>(MAKELANGID(lang, sublang)),
          buffer.data(), buffer.size(), nullptr);

        return std::string{buffer.data()};
    }

    /**
     * @brief Returns a textual description of the last system error.
     */
    [[nodiscard]] inline std::string get_last_error_str()
    {
        return get_error_str(::GetLastError());
    }

    /**
     * @brief Loads the specified module into the address space of the calling process.
     * The specified module may cause other modules to be loaded.
     */
    template <typename Ret = class SysModule*>
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
}

#else
  #include <cerrno>
  #include <cstring>
  #include <dlfcn.h>
  #include <unistd.h>

namespace rehlds::common
{
    /**
     * @brief Retrieves the fully qualified path for the file that contains the specified module.
     *
     * @note The module must have been loaded by the current process.
     */
    [[nodiscard]] std::string get_module_absolute_path(const char* filename);

    /**
     * @brief Returns the process ID (PID) obtained from the system.
     */
    [[nodiscard]] inline int get_pid() noexcept
    {
        return ::getpid();
    }

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
}
#endif
