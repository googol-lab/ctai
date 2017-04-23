#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "containers/values_container.hpp"
#include "eip_change.hpp"

namespace ctai
{
    namespace details
    {
        // dec register
        template <typename reg_token, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_dec,
                reg_token,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::DEC;

            using instruction = values_container_n::values_container<inst::to_size<instruction_type>,
                                                                     token_to_reg_opcode<reg_token>>;

            using instruction_tokens = tuple<tokens::tok_dec,
                                             reg_token>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }
}
