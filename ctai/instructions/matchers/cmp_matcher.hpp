#pragma once

#include "ctai/tuple.hpp"
#include "ctai/tokenize/tokens.hpp"
#include "ctai/instructions/ids_vaules.hpp"
#include "operand_decoder.hpp"
#include "ctai/containers/values_container.hpp"

namespace ctai
{
    namespace details
    {
        // add reg, reg/val
        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_cmp,
                reg_token,
                tokens::tok_comma,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<operand> ? inst::id_t::CMP_REG_REG
                                                                           : inst::id_t::CMP_REG_VAL;

            using instruction = values_container_n::values_container<inst::to_size<instruction_type>,
                                                                     token_to_reg_opcode<reg_token>,
                                                                     operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<tokens::tok_cmp,
                                             reg_token,
                                             tokens::tok_comma,
                                             operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_cmp, tokens::tok_ebx, tokens::tok_comma, tokens::tok_eax , string<>, string<>>>::instruction,
                                   values_container_n::values_container<
                        inst::to_size<inst::id_t::CMP_REG_REG>,
                        regs::to_size<regs::id_t::EBX>,
                        regs::to_size<regs::id_t::EAX>
                >>::value, "");
    }
}