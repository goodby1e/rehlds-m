/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include "common/interface.h"
#include "common/interfaces/system_base.h"
#include "cpputils/singleton_holder.h"
#include "cpputils/system.h"
#include <unordered_map>
#include <cassert>
#include <string>
#include <utility>

namespace rehlds::common
{
#ifdef _WIN32
    constexpr auto* ENGINE_MODULE_FILE = "swds.dll";
    constexpr auto* GAME_MODULE_FILE = "cstrike\\dlls\\mp.dll";
    constexpr auto* FILESYSTEM_MODULE_FILE = "FileSystem_Stdio.dll";
#else
    constexpr auto* ENGINE_MODULE_FILE = "engine_i486.so";
    constexpr auto* GAME_MODULE_FILE = "cstrike/dlls/cs.so";
    constexpr auto* FILESYSTEM_MODULE_FILE = "filesystem_stdio.so";
#endif

    class HldsModule
    {
      public:
        /**
         * @brief Constructor.
         *
         * @param name Module filename.
         */
        explicit HldsModule(std::string name);

        HldsModule(HldsModule&&) = delete;
        HldsModule(const HldsModule&) = delete;
        HldsModule& operator=(HldsModule&&) = delete;
        HldsModule& operator=(const HldsModule&) = delete;

        /**
         * @brief Virtual destructor.
         */
        virtual ~HldsModule();

        /**
         * @brief Returns true if the handle is not null.
         */
        [[nodiscard]] bool is_loaded() const noexcept;

        /**
         * @brief Loads a DLL/component from disk.
         *
         * @return \c true if loaded successfully, otherwise \c false
         */
        bool load();

        /**
         * @brief Unloads a DLL/component.
         */
        void unload() noexcept;

        /**
         *  @brief Returns a pointer to a CreateInterface function.
         */
        [[nodiscard]] CreateInterfaceFn get_factory() noexcept;

        /**
         * @brief Returns a pointer to a function.
         *
         * @param procedure Procedure name.
         *
         * @return Pointer to a function (procedure).
         */
        template <typename T = void*>
        [[nodiscard]] T get_proc_address(const std::string& procedure) noexcept;

        /**
         *  @brief Returns a pointer to the specified interface from a module.
         */
        template <typename T>
        [[nodiscard]] T* get_interface(const std::string& name, CreateInterfaceStatus* status, bool cache = true);

        /**
         *  @brief Returns a pointer to the specified interface from a module.
         */
        template <typename T>
        [[nodiscard]] T* get_interface(const std::string& name, bool cache = true);

      private:
        /* Finds an interface in the cache. */
        [[nodiscard]] IBaseInterface* find_interface(const std::string& name) const;

        /* Caches the specified interface. */
        void cache_interface(const std::string& name, IBaseInterface* interface);

        /* Module name. */
        const std::string name_;

        /* Module handle. */
        cpputils::SysModule* handle_{};

        /* Cached interfaces map. */
        std::unordered_map<std::string, IBaseInterface*> interfaces_{};
    };

    inline HldsModule::HldsModule(std::string name) : name_(std::move(name))
    {
        assert(!name_.empty());
    }

    inline HldsModule::~HldsModule()
    {
        unload();
    }

    inline bool HldsModule::is_loaded() const noexcept
    {
        return handle_ != nullptr;
    }

    inline bool HldsModule::load()
    {
        if (is_loaded()) {
            return true;
        }

        interfaces_.clear();
        handle_ = cpputils::load_module(name_.c_str());

        return is_loaded();
    }

    inline void HldsModule::unload() noexcept
    {
        if (is_loaded() && unload_module(handle_)) {
            interfaces_.clear();
            handle_ = nullptr;
        }
    }

    inline CreateInterfaceFn HldsModule::get_factory() noexcept
    {
        return get_proc_address<CreateInterfaceFn>(CREATE_INTERFACE_PROC_NAME);
    }

    template <typename T>
    T HldsModule::get_proc_address(const std::string& procedure) noexcept
    {
        return load() ? cpputils::get_proc_address<T>(handle_, procedure) : nullptr;
    }

    template <typename T>
    T* HldsModule::get_interface(const std::string& name, CreateInterfaceStatus* const status, const bool cache)
    {
        T* interface = cache ? static_cast<T*>(find_interface(name)) : nullptr;

        if (interface != nullptr) {
            if (status != nullptr) {
                *status = CreateInterfaceStatus::succeeded;
            }

            return interface;
        }

        if (status != nullptr) {
            *status = CreateInterfaceStatus::failed;
        }

        auto* const factory = get_factory();
        interface = nullptr == factory ? nullptr : static_cast<T*>(factory(name.c_str(), status));

        if ((interface != nullptr) && cache) {
            cache_interface(name, interface);
        }

        return interface;
    }

    template <typename T>
    T* HldsModule::get_interface(const std::string& name, const bool cache)
    {
        auto status = CreateInterfaceStatus::failed;
        auto* const interface = get_interface<T>(name, &status, cache);

        return status == CreateInterfaceStatus::succeeded ? interface : nullptr;
    }

    inline IBaseInterface* HldsModule::find_interface(const std::string& name) const
    {
        const auto& it = interfaces_.find(name);
        return interfaces_.end() == it ? nullptr : it->second;
    }

    inline void HldsModule::cache_interface(const std::string& name, IBaseInterface* const interface)
    {
        assert(!name.empty());
        assert(interface != nullptr);
        interfaces_[name] = interface;
    }

    class HldsEngineModule : public HldsModule
    {
      public:
        explicit HldsEngineModule(std::string name);
        ISystemBase* get_system();
    };

    inline HldsEngineModule::HldsEngineModule(std::string name) : HldsModule(std::move(name))
    {
    }

    inline ISystemBase* HldsEngineModule::get_system()
    {
        auto* const system_module = get_interface<ISystemModule>(INTERFACE_SYSTEM_BASE);
        return nullptr == system_module ? nullptr : system_module->get_system();
    }
}

namespace rehlds::common
{
    /**
     * @brief Returns an HLDS engine module wrapper instance.
     */
    [[nodiscard]] inline HldsEngineModule& get_engine_module()
    {
        return cpputils::SingletonHolder<HldsEngineModule>::get_instance(ENGINE_MODULE_FILE);
    }

    /**
     * @brief Returns an HLDS game module wrapper instance.
     */
    [[nodiscard]] inline HldsModule& get_game_module()
    {
        return cpputils::SingletonHolder<HldsModule>::get_instance(GAME_MODULE_FILE);
    }

    /**
     * @brief Returns an HLDS filesystem module wrapper instance.
     */
    [[nodiscard]] inline HldsModule& get_filesystem_module()
    {
        return cpputils::SingletonHolder<HldsModule>::get_instance(FILESYSTEM_MODULE_FILE);
    }
}
