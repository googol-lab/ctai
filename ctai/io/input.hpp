#pragma once

#include "ctai/string.hpp"
#include "ctai/declare_code.hpp"

namespace ctai
{
    namespace io
    {
        namespace input
        {
            template <char ...chars>
            using buffer = ctai::string<chars...>;

            template <typename buffer_t>
            constexpr char front = ctai::string_front<buffer_t>;

            template <typename buffer_t>
            using pop = ctai::string_pop_front<buffer_t>;

            template<typename ...parts>
            using declare_input = declare_code<parts...>;
        }
    }
}
