#pragma once

#include <cstdint>

namespace cai
{
    template <uint32_t val>
    struct reg
    {
        static constexpr auto l = val & 0xff;
        static constexpr auto h = val & 0xff00;
        static constexpr auto x = val & 0xffff;
        static constexpr auto ex = val;
    };

    template <uint32_t val>
    struct simple_reg
    {
        static constexpr auto low_16 = val & 0xffff;
        static constexpr auto value = val;
    };

    template <uint32_t eax_val,
            uint32_t ebx_val,
            uint32_t ecx_val,
            uint32_t edx_val,
            uint32_t esp_val,
            uint32_t ebp_val,
            uint32_t esi_val,
            uint32_t edi_val>
    struct registers_state
    {
        static constexpr reg<eax_val> eax{};
        static constexpr reg<ebx_val> ebx{};
        static constexpr reg<ecx_val> ecx{};
        static constexpr reg<edx_val> edx{};

        static constexpr simple_reg<esp_val> esp{};
        static constexpr simple_reg<ebp_val> ebp{};
        static constexpr simple_reg<esi_val> esi{};
        static constexpr simple_reg<edi_val> edi{};
    };

    using startup_registers_state = registers_state<0,0,0,0,0,0,0,0>;

    namespace details
    {
        template <typename reg_state>
        struct to_register_state_impl;


        template <uint32_t eax_val,
                uint32_t ebx_val,
                uint32_t ecx_val,
                uint32_t edx_val,
                uint32_t esp_val,
                uint32_t ebp_val,
                uint32_t esi_val,
                uint32_t edi_val>
        struct to_register_state_impl<
                registers_state<
                        eax_val,
                        ebx_val,
                        ecx_val,
                        edx_val,
                        esp_val,
                        ebp_val,
                        esi_val,
                        edi_val>>
        {
            using type = typename registers_state<
                    eax_val,
                    ebx_val,
                    ecx_val,
                    edx_val,
                    esp_val,
                    ebp_val,
                    esi_val,
                    edi_val>;
        };
    }

    template <typename reg_state>
    using to_register_state = typename details::to_register_state_impl<reg_state>::type;

    namespace tests
    {
        static_assert(reg<0xddccbbaa>{}.l == 0xaa, "");
        static_assert(reg<0xddccbbaa>{}.h == 0xbb00, "");
        static_assert(reg<0xddccbbaa>{}.x == 0xbbaa, "");
        static_assert(reg<0xddccbbaa>{}.ex == 0xddccbbaa, "");

        static_assert(simple_reg<0xddccbbaa>{}.low_16 == 0xbbaa, "");
        static_assert(simple_reg<0xddccbbaa>{}.value == 0xddccbbaa, "");
    }

}
