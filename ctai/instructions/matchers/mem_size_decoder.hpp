#pragma once

#include "ctai/tokenize/tokens.hpp"
#include "ctai/instructions/ids_vaules.hpp"

#include <type_traits>

namespace ctai
{
    namespace details
    {

        template <typename>
        struct mem_size_decoder_impl;

        template <typename str>
        struct mem_size_decoder_impl
        {
            static constexpr auto value = std::is_same<str, tokens::tok_byte>::value ? memory::id_t::s_8
                                                                                     : std::is_same<str, tokens::tok_word>::value ? memory::id_t::s_16
                                                                                                                                  : memory::id_t::s_32;
        };
    }

    template <typename token>
    constexpr auto mem_size_decoder = details::mem_size_decoder_impl<token>::value;
}
