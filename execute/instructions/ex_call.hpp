#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "values/values.hpp"
#include "execute/eip_adjust.hpp"
#include "thread/thread.hpp"

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t,
                  typename machine_state_t,
                  size_t ip,
                  size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                              inst::to_size<inst::id_t::CALL_VAL>,
                              ip,
                              rest_of_opcodes...>
        {
            static constexpr auto current_esp = get_reg<typename thread_t::registers, regs::id_t::ESP>;
            static constexpr auto next_esp = current_esp - 4;

            using registers_after_eip = adjust_eip<typename thread_t::registers, inst::id_t::CALL_VAL>;
            static constexpr auto eip_after_ret = get_reg<registers_after_eip, regs::id_t::EIP>;

            using next_registers = set_reg<typename thread_t::registers, regs::id_t::ESP, next_esp>;

            using final_registers = set_reg<next_registers, regs::id_t::EIP, static_cast<uint32_t>(ip)>;

            using result_thread = thread::thread<thread_t::finished,
                                                 thread_t::id,
                                                 thread_t::priority,
                                                 final_registers,
                                                 typename thread_t::flags>;

            using splitted_value = values::split_to_byte_values_container<eip_after_ret>;
            using result_memory = values_container_n::set_from_container<typename machine_state_t::memory::memory_block_t,
                                                                         next_esp,
                                                                         splitted_value>;

            using result_machine_state = machine::set_memory_block<machine_state_t, result_memory>;
        };
    }
}