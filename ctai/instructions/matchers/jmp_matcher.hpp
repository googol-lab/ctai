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
        // jmp instruction_pointer
        template <typename ip, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jmp,
                ip,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JMP;

            using instruction = values_container_n::values_container<
                    inst::to_size<instruction_type>,
                    operand_decoder<ip>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_jmp,
                    ip>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };

        //TODO remove this
        //jmp .label_name
        template <char ...label_chars, typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_jmp,
                                  string<'.', label_chars...>,
                                  rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JMP;

            using instruction = values_container_n::values_container<inst::to_size<instruction_type>,
                                                                     0>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<tokens::tok_jmp,
                                             string<'.', label_chars...>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_jmp, decltype("22"_s), string<>, string<>>>::instruction,
                      values_container_n::values_container<
                        inst::to_size<inst::id_t::JMP>,
                        static_cast<size_t>(22)
                >>::value, "");
    }
}