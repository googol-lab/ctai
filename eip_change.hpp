#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"

#include <cstddef>
#include <tuple.hpp>

namespace cai
{
    namespace details
    {
        template <inst::id_t instruction_opcode>
        constexpr size_t eip_change{0};

        template <> constexpr size_t eip_change<inst::id_t::ADD_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::ADD_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::CMP_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::EXIT> = 1;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::PUSH_REG> = 2;
        template <> constexpr size_t eip_change<inst::id_t::POP_REG> = 2;
    }

    template <inst::id_t instruction>
    constexpr auto get_eip_change = details::eip_change<instruction>;
}
