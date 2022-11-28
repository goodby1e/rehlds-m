/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#ifdef _MSC_VER
  #define COMPILER_MSVC
#elif defined(__INTEL_LLVM_COMPILER)
  #define COMPILER_INTEL_LLVM
#elif defined(__INTEL_COMPILER)
  #define COMPILER_INTEL
#elif !defined(__llvm__) && (defined(__GNUC__) || defined(__GNUG__))
  #define COMPILER_GCC // probably
#elif defined(__clang__) && defined(__llvm__)
  #define COMPILER_CLANG // probably
#endif

#ifdef _WIN32
  #define NO_VTABLE __declspec(novtable)
  #define NO_INLINE __declspec(noinline)
  #define FORCE_INLINE __forceinline
  #define DLL_EXPORT __declspec(dllexport)
  #define STACK_ALIGN NO_INLINE
#else
  #define NO_VTABLE
  #define NO_INLINE [[gnu::noinline]]
  #define FORCE_INLINE [[gnu::always_inline]]
  #define DLL_EXPORT [[gnu::force_align_arg_pointer, gnu::noinline, gnu::visibility("default")]]
  #define STACK_ALIGN [[gnu::force_align_arg_pointer, gnu::noinline, gnu::used]]
#endif

#ifdef NDEBUG
  #ifndef _MSC_VER
    #define ATTR_CONST [[gnu::const]]
  #else
    #define ATTR_CONST
  #endif

  #ifdef COMPILER_GCC
    #define ATTR_MINSIZE [[gnu::optimize("-Os")]]
  #elif defined(COMPILER_CLANG) || defined(COMPILER_INTEL_LLVM)
    #define ATTR_MINSIZE [[clang::minsize]]
  #else
    #define ATTR_MINSIZE
  #endif

  #ifdef COMPILER_GCC
    #define ATTR_MINSIZE_INLINE [[gnu::always_inline, gnu::optimize("-Os")]]
  #elif defined(COMPILER_CLANG) || defined(COMPILER_INTEL_LLVM)
    #define ATTR_MINSIZE_INLINE [[gnu::always_inline, clang::minsize]]
  #elif !defined(COMPILER_MSVC)
    #define ATTR_MINSIZE_INLINE [[gnu::always_inline]]
  #else
    #define ATTR_MINSIZE_INLINE FORCE_INLINE
  #endif

  #ifdef COMPILER_GCC
    #define ATTR_OPTIMIZE [[gnu::optimize("-O3")]]
  #else
    #define ATTR_OPTIMIZE
  #endif

  #ifdef COMPILER_GCC
    #define ATTR_OPTIMIZE_INLINE [[gnu::always_inline, gnu::optimize("-O3")]]
  #elif !defined(COMPILER_MSVC)
    #define ATTR_OPTIMIZE_INLINE [[gnu::always_inline]]
  #else
    #define ATTR_OPTIMIZE_INLINE FORCE_INLINE
  #endif

  #ifdef COMPILER_GCC
    #define ATTR_OPTIMIZE_HOT [[gnu::optimize("-O3"), gnu::hot]]
  #elif !defined(COMPILER_MSVC)
    #define ATTR_OPTIMIZE_HOT [[gnu::hot]]
  #else
    #define ATTR_OPTIMIZE_HOT
  #endif

  #ifdef COMPILER_GCC
    #define ATTR_OPTIMIZE_HOT_INLINE [[gnu::always_inline, gnu::optimize("-O3"), gnu::hot]]
  #elif !defined(COMPILER_MSVC)
    #define ATTR_OPTIMIZE_HOT_INLINE [[gnu::always_inline, gnu::hot]]
  #else
    #define ATTR_OPTIMIZE_HOT_INLINE FORCE_INLINE
  #endif
#else
  #define ATTR_CONST
  #define ATTR_MINSIZE
  #define ATTR_MINSIZE_INLINE
  #define ATTR_OPTIMIZE
  #define ATTR_OPTIMIZE_INLINE
  #define ATTR_OPTIMIZE_HOT
  #define ATTR_OPTIMIZE_HOT_INLINE
#endif
