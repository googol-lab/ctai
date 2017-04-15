#pragma once

#include "memory/memory.hpp"

#include <cstddef>

namespace ctai
{
    namespace machine
    {
        template <typename memory_t,
                  typename opcodes_t,
                  typename threads_t,
                  size_t time_v>
        struct state
        {
            using memory = memory_t;
            using opcodes = opcodes_t;
            using threads = threads_t;
            static constexpr auto time = time_v;
            static constexpr auto next_time = time_v + 1;
        };

        template <typename state_t, size_t time_delta>
        using adjust_time = state<typename state_t::memory,
                typename state_t::opcodes,
                typename state_t::threads,
                                  state_t::time + time_delta>;

        template <typename state_t, typename memory>
        using set_memory = state<memory,
                                 typename state_t::opcodes,
                                 typename state_t::threads,
                                 state_t::time>;

        template <typename state_t, typename memory_block>
        using set_memory_block = set_memory<state_t,
                                            memory::set_memory_block<typename state_t::memory, memory_block>>;

        template <typename state_t, typename threads, size_t time>
        using set_threads_and_time = state<typename state_t::memory,
                                           typename state_t::opcodes,
                                           threads,
                                           time>;
    }
}
