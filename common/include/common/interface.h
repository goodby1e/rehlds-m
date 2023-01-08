/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 *
 *
 *  This header defines the interface convention used in the valve engine.
 *
 *  To make an interface and expose it:
 *    1. Derive from IBaseInterface.
 *    2. The interface must be ALL pure virtual, and have no data members.
 *    3. Define a name for it.
 *    4. In its implementation file, use EXPOSE_INTERFACE or EXPOSE_SINGLE_INTERFACE.
 *
 *  Versioning
 *
 *  There are two versioning cases that are handled by this:
 *    1. You add functions to the end of an interface, so it is binary compatible with the previous interface.
 *       In this case, you need two EXPOSE_INTERFACE's: one to expose your class as the old interface
 *       and one to expose it as the new interface.
 *
 *    2. You update an interface, so it's not compatible anymore (but you still want to be able to
 *       expose the old for legacy code). In this case, you need to make a new version name for your new interface,
 *       and make a wrapper interface and expose it for the old interface.
 */

#pragma once

#include "common/platform.h"
#include <functional>
#include <string>

namespace rehlds::common
{
    /**
     * @brief Name of the interface factory function.
     */
    constexpr auto* CREATE_INTERFACE_PROC_NAME = "CreateInterface";

    /**
     * @brief Interface return status.
     */
    enum class CreateInterfaceStatus : int
    {
        succeeded = 0,
        failed
    };

    /**
     * @brief Interface factory function prototype.
     */
    using CreateInterfaceFn = class IBaseInterface* (*)(const char* name, CreateInterfaceStatus* return_code);

    /**
     * @brief All interfaces derive from this.
     */
    class NO_VTABLE IBaseInterface
    {
      public:
        IBaseInterface() = default;
        IBaseInterface(IBaseInterface&&) = default;
        IBaseInterface(const IBaseInterface&) = default;
        IBaseInterface& operator=(IBaseInterface&&) = default;
        IBaseInterface& operator=(const IBaseInterface&) = default;
        STACK_ALIGN virtual ~IBaseInterface() = default;
    };

    /**
     * @brief Used internally to register interfaces.
     */
    struct InterfaceReg final
    {
        using InstantiateFunc = std::function<IBaseInterface*()>;
        InterfaceReg(InstantiateFunc instantiate_func, const std::string& name);
    };

    /**
     *  @brief Returns a pointer to the CreateInterface function.
     */
    [[nodiscard]] CreateInterfaceFn get_factory_this() noexcept;
}

/**
 * @brief Use this to expose an interface that can have multiple instances.
 *
 * e.g.: \c EXPOSE_INTERFACE(InterfaceImp, IInterface, "MyInterface001")
 * This will expose a class called InterfaceImp that implements IInterface (a pure class).
 * Clients can receive a pointer to this class by calling \c CreateInterface("MyInterface001").
 *
 * In practice, the shared header file defines the interface (IInterface) and version name ("MyInterface001"),
 * so that each component can use these names/vtables to communicate.
 *
 * A single class can support multiple interfaces through multiple inheritance.
 */
#define EXPOSE_INTERFACE(class_name, interface_name, version_name)                                                     \
  namespace                                                                                                            \
  {                                                                                                                    \
    rehlds::common::IBaseInterface* __Create##class_name##_interface()                                                 \
    {                                                                                                                  \
      return new class_name{}; /* NOLINT(bugprone-macro-parentheses) */                                                \
    }                                                                                                                  \
    rehlds::common::InterfaceReg __g_Create##class_name##_reg{&__Create##class_name##_interface, (version_name)};      \
  }

/**
 * @brief Use this if you want to write the factory function.
 */
#define EXPOSE_INTERFACE_FN(function_name, interface_name, version_name)                                               \
  namespace                                                                                                            \
  {                                                                                                                    \
    rehlds::common::InterfaceReg __g_Create##interface_name##_reg{(function_name), (version_name)};                    \
  }

/**
 * @brief Use this to expose a singleton interface with a global variable you've created.
 */
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(class_name, interface_name, version_name, global_var_name)                   \
  namespace                                                                                                            \
  {                                                                                                                    \
    rehlds::common::IBaseInterface* __Create##class_name##interface_name##_interface()                                 \
    {                                                                                                                  \
      return &(global_var_name);                                                                                       \
    }                                                                                                                  \
    rehlds::common::InterfaceReg __g_Create##class_name##interface_name##_reg{                                         \
      &__Create##class_name##interface_name##_interface, (version_name)};                                              \
  }

/**
 * @brief Use this to expose a singleton interface. This creates the global variable for you automatically.
 */
#define EXPOSE_SINGLE_INTERFACE(class_name, interface_name, version_name)                                              \
  namespace                                                                                                            \
  {                                                                                                                    \
    class_name __g_##class_name##_singleton{}; /* NOLINT(bugprone-macro-parentheses) */                                \
    EXPOSE_SINGLE_INTERFACE_GLOBALVAR(class_name, interface_name, (version_name), __g_##class_name##_singleton)        \
  }
