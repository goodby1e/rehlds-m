/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#pragma once

#include <chrono>
#include <thread>

namespace rehlds::dedicated
{
    using SysSleep = void (*)();
    inline SysSleep sys_sleep = nullptr;

    using NetSleep = int (*)();
    inline NetSleep net_sleep = nullptr;

    /**
     * @brief -pingboost default
     */
    inline void sleep_thread_millisecond()
    {
        constexpr std::chrono::milliseconds millisecond{1};
        std::this_thread::sleep_for(millisecond);
    }

    /**
     * @brief -pingboost 3
     */
    inline void sleep_net()
    {
        if (nullptr == net_sleep) {
            sleep_thread_millisecond();
        }
        else {
            net_sleep();
        }
    }

    /**
     * @brief -pingboost 5
     */
    inline void thread_yield()
    {
        std::this_thread::yield();
    }
}

#ifdef _WIN32
  #include <bcrypt.h>

namespace rehlds::dedicated
{
    using ZwSetTimerResolution = ::NTSTATUS(__stdcall*)(
      ::ULONG requested_resolution, ::BOOLEAN set, ::PULONG actual_resolution);

    using NtDelayExecution = ::NTSTATUS(__stdcall*)(::BOOL alertable, ::PLARGE_INTEGER delay_interval);

    inline ZwSetTimerResolution set_timer_resolution{};
    inline NtDelayExecution delay_execution{};

    /**
     * @brief -pingboost 4
     */
    inline void sleep_delay_execution()
    {
        ::LARGE_INTEGER interval;
        interval.QuadPart = -1LL;
        delay_execution(FALSE, &interval);
    }
}
#else
  #include <sys/poll.h>
  #include <sys/time.h>
  #include <csignal>
  #include <unistd.h>

namespace rehlds::dedicated
{
    inline volatile std::sig_atomic_t paused = 0;

    inline void sigalrm_handler(const int /* unused */)
    {
        std::signal(SIGALRM, &sigalrm_handler);

        if (0 != paused) {
            return;
        }

        constexpr itimerval timer_val{
          {0, 0   },
          {0, 1000}
        };

        ::setitimer(::ITIMER_REAL, &timer_val, nullptr);
    }

    /**
     * @brief -pingboost 1
     */
    inline void sleep_timer()
    {
        paused = 0;

        constexpr itimerval timer_val{
          {0, 0   },
          {0, 1000}
        };

        if (0 == ::setitimer(::ITIMER_REAL, &timer_val, nullptr)) {
            ::pause();
        }

        paused = 1;
    }

    /**
     * @brief -pingboost 2
     */
    inline void sleep_poll()
    {
        ::poll(nullptr, 0, 1);
    }

    /**
     * @brief -pingboost 4
     */
    inline void sleep_thread_microsecond()
    {
        constexpr std::chrono::microseconds microseconds{3};
        std::this_thread::sleep_for(microseconds);
    }
}
#endif
