#pragma once

#include <type_traits>

namespace cai
{
    template <char ...chars>
    struct string
    {
        static constexpr auto size = sizeof...(chars);
    };


    //
    // to_string
    //
    namespace details
    {
        template <typename str>
        struct to_string_impl;

        template <char ...chars>
        struct to_string_impl<string<chars...>>
        {
            using type = string<chars...>;
        };
    }

    template <typename s>
    using to_string = typename details::to_string_impl<s>::type;

    //
    //string_eq
    //
    namespace details
    {
        template <typename, typename, bool eq_size>
        struct string_eq_impl;

        // different size
        template <char ...lchars, char ...rchars>
        struct string_eq_impl<string<lchars...>, string<rchars...>, false>
        {
            static constexpr auto value = false;
        };

        template <>
        struct string_eq_impl<string<>, string<>, true>
        {
            static constexpr auto value = true;
        };

        template <char eq_first, char ...lchars, char ...rchars>
        struct string_eq_impl<string<eq_first, lchars...>, string<eq_first, rchars...>, true>
        {
            static constexpr auto value = string_eq_impl<string<lchars...>, string<rchars...>, true>::value;
        };

        template <char lfirst, char rfirst, char ...lchars, char ...rchars>
        struct string_eq_impl<string<lfirst, lchars...>, string<rfirst, rchars...>, true>
        {
            static constexpr auto value = false;
        };
    }

    template <typename ls, typename rs>
    constexpr auto string_eq = details::string_eq_impl<ls, rs, to_string<ls>::size == to_string<rs>::size>::value;

    //
    //string_front
    //
    namespace details
    {
        template <typename>
        struct string_front_impl;

        template <char front, char ...chars>
        struct string_front_impl<string<front, chars...>>
        {
            static constexpr auto value = front;
        };
    }

    template <typename s>
    constexpr auto string_front = details::string_front_impl<s>::value;

    //
    //string_append
    //
    namespace details
    {
        template <typename, char>
        struct string_append_impl;

        template <char ...chars, char value>
        struct string_append_impl<string<chars...>, value>
        {
            using type = string<chars..., value>;
        };
    }

    template <typename s, char value>
    using string_append = typename details::string_append_impl<s, value>::type;

    //
    //string_pop_front
    //
    namespace details
    {
        template <typename>
        struct string_pop_front_impl;

        template <char front, char ...chars>
        struct string_pop_front_impl<string<front, chars...>>
        {
            using type = string<chars...>;
        };
    }

    template <typename s>
    using string_pop_front = typename details::string_pop_front_impl<s>::type;


    //
    // string_to_int
    //
    namespace details
    {
        template <typename, int sign = 1, int current_val = 0>
        struct string_to_int_impl;

        template <int sign, int current_val>
        struct string_to_int_impl<string<>, sign, current_val>
        {
            static constexpr auto value = sign * current_val;
        };

        template <int sign, int current_val, char current_char, char ...chars>
        struct string_to_int_impl<string<current_char, chars...>, sign, current_val>
        {
            static constexpr auto nex_val = current_val* 10 + (current_char - '0');
            static constexpr auto value = string_to_int_impl<string<chars...>, sign, nex_val>::value;
        };

        template <int sign, int current_val, char current_char, char ...chars>
        struct string_to_int_impl<string<'-', current_char, chars...>, sign, current_val>
        {
            static constexpr auto nex_val = current_val * 10 + (current_char - '0');
            static constexpr auto value = string_to_int_impl<string<chars...>, -1, nex_val>::value;
        };
    }

    template <typename str>
    constexpr auto string_to_int = details::string_to_int_impl<str>::value;

    //
    //string_from_int
    //
    namespace details
    {
        template <int value, typename current_string = string<>>
        struct string_from_int_impl;

        template <typename current_string>
        struct string_from_int_impl<0, current_string>
        {
            using type = current_string;
        };

        template <int value, char ...chars>
        struct string_from_int_impl<value, string<chars...>>
        {
            using type = typename string_from_int_impl<
                    value/10,
                    string<chars..., value % 10 + '0'>>::type;
        };

        template <int value>
        struct prepare_sign_and_convert
        {
            using type = typename std::conditional_t<
                    value < 0,
                    string_from_int_impl<value * -1, string<'-'>>,
                    string_from_int_impl<value>
                    >::type;
        };
    }
    template <int value>
    using string_from_int = typename details::prepare_sign_and_convert<value>::type;
}

template <typename T, T... chars>
constexpr auto operator""_s() {
    return cai::string<chars...>{};
}

namespace cai
{
    namespace tests
    {
        static_assert(string_eq<string<'a','b'>, string<'a','b'>>, "");
        static_assert(string_eq<string<'a','b'>, decltype("ab"_s)>, "");
        static_assert(!string_eq<string<'a','b'>, string<'A','b'>>, "");
        static_assert(!string_eq<string<'a','b','c'>, string<'a','b'>>, "");
        static_assert(!string_eq<string<'A','b'>, string<'a','b'>>, "");
        static_assert(!string_eq<string<'a','b'>, string<'a','b','c'>>, "");

        static_assert(string_front<decltype("abc"_s)> == 'a', "");
        static_assert(string_front<decltype(" abc"_s)> == ' ', "");

        static_assert(string_to_int<decltype("2"_s)> == 2, "");
        static_assert(string_to_int<decltype("-2"_s)> == -2, "");

    }
}