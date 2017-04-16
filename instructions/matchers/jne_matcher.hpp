#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"
#include "containers/values_container.hpp"
#include "eip_change.hpp"

namespace ctai
{
    namespace details
    {
        // jmp instruction_pointer or label
        template <typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jne,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JNE;

            static constexpr auto decoded = is_label_token<operand> ? 0
                                                                    : operand_decoder<operand>;

            using instruction = values_container_n::values_container<
                    inst::to_size<instruction_type>,
                    decoded>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_jne,
                    operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_jge, decltype("22"_s), string<>, string<>>>::instruction,
                values_container_n::values_container<
                        inst::to_size<inst::id_t::JGE>,
                        static_cast<size_t>(22)
                >>::value, "");
    }
}