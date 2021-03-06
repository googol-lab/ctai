#pragma once

#include "ctai/tuple.hpp"
#include "ctai/tokenize/tokens.hpp"
#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/eip_change.hpp"
#include "ctai/containers/values_container.hpp"

namespace ctai
{
    namespace details
    {
        template <typename tokens>
        struct matcher_impl;

        template <typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_exit, rest_of_tokens...>>
        {
            using instruction = values_container_n::values_container<inst::to_size<inst::id_t::EXIT>>;
            static constexpr auto eip_change = get_eip_change<inst::id_t::EXIT>;

            using instruction_tokens = tuple<tokens::tok_exit>;
            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;
}

#include "push_matcher.hpp"
#include "pusha_matcher.hpp"
#include "pop_matcher.hpp"
#include "popa_matcher.hpp"
#include "mov_matcher.hpp"
#include "add_matcher.hpp"
#include "mul_matcher.hpp"
#include "sub_matcher.hpp"
#include "jmp_matcher.hpp"
#include "jge_matcher.hpp"
#include "inc_matcher.hpp"
#include "dec_matcher.hpp"
#include "jne_matcher.hpp"
#include "jg_matcher.hpp"
#include "jl_matcher.hpp"
#include "je_matcher.hpp"
#include "cmp_matcher.hpp"
#include "call_matcher.hpp"
#include "ret_matcher.hpp"
#include "div_matcher.hpp"
#include "sys_exit_thread_matcher.hpp"
#include "sys_create_thread_matcher.hpp"
#include "sys_write_matcher.hpp"
#include "sys_read_matcher.hpp"
#include "sys_is_thread_running_matcher.hpp"
#include "sys_malloc_matcher.hpp"
#include "sys_free_matcher.hpp"
#include "sys_try_lock_mutex_matcher.hpp"

namespace ctai
{
    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_push, tokens::tok_eax, string<>, string<>>>::instruction,
                                   values_container_n::values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}