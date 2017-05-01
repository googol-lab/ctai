#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "tuple.hpp"
#include "machine_state.hpp"
#include "execute/ex_instruction.hpp"
#include "execute/ex_push.hpp"
#include "tokenize/tokenizer.hpp"
#include "assembler/assembler.hpp"
#include "assembler/label_substitute.hpp"
#include "assembler/label_extract.hpp"

#include "thread/thread.hpp"
#include "thread/scheduler_round_robin.hpp"
#include "execute/execute_thread.hpp"
#include "machine/machine_state.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename machine_state>
            struct execute_impl;

            template <typename memory_t,
                      typename opcodes_t,
                      size_t time_v>
            struct execute_impl<machine::state<memory_t,
                                               opcodes_t,
                                               tuple_n::tuple<>,
                                               time_v>>
            {
                static constexpr auto ret_val = time_v;
            };

            template <typename machine_state>
            struct execute_impl
            {
                using scheduler_result = thread::schedule::next<typename machine_state::threads>;
                using thread_to_execute = typename scheduler_result::result_thread;
                using thread_execution_result = execute2::execute_thread<thread_to_execute,
                                                                         typename machine_state::memory,
                                                                         typename machine_state::opcodes>;

                using next_threads_queue = std::conditional_t<thread_execution_result::result_thread::finished,
                                                              typename scheduler_result::result_threads_queue,
                                                              queue::push<typename scheduler_result::result_threads_queue,
                                                                          typename thread_execution_result::result_thread>>;

                using next_machine_state = machine::state<typename thread_execution_result::result_memory,
                                                          typename machine_state::opcodes,
                                                          next_threads_queue,
                                                          machine_state::time + thread_execution_result::executed_instructions_count>;

                static constexpr auto ret_val = execute_impl<next_machine_state>::ret_val;
            };
        }

        namespace details
        {
            template <typename code>
            struct prepare_and_execute
            {
                using tokens = tokenize<code>;
                using extract_labels_result = extract_labels<tokens>;
                using tokens_without_labels = typename extract_labels_result::tokens;
                using labels_metadata = typename extract_labels_result::labels;
                using tokens_after_labels_substitution = substutite_labels<tokens_without_labels, labels_metadata>;
                using opcodes = assemble<tokens_after_labels_substitution>;

                static constexpr auto main_ip = labels_get_ip<labels_metadata, string<'.', 'm', 'a', 'i', 'n'>>;

                using root_thread = thread::create<100, //priority
                                                   0,   //id
                                                   main_ip,   //eip
                                                   15>;  //esp

                using memory_t = memory::memory_create<16>;


                using machine_state = machine::state<memory_t,
                                                     opcodes,
                                                     tuple_n::tuple<root_thread>,
                                                     0>; //time

                static constexpr auto ret_val = execute_impl<machine_state>::ret_val;
            };
        }

        template <typename code>
        constexpr auto execute_code = details::prepare_and_execute<code>::ret_val;
    }
}
