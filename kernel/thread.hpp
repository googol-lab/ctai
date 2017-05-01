#pragma once

#include "string.hpp"

namespace ctai
{
    namespace include
    {
        using sys_exit_thread = decltype(
        ":sys_exit_thread "
                "sys_exit_thread "_s);

        using sys_create_thread = decltype(
        ":sys_create_thread "
                "push ebx "
                "push ecx "
                "push edx "
                "mov eax , 'C' "
                "call .sys_write "

                "mov ebx , DWORD PTR [ esp + 12 ] "//void* start_point\n
                "mov ecx , DWORD PTR [ esp + 8 ] "//priority
                "mov edx , DWORD PTR [ esp + 4 ] "//arg

                "sys_create_thread "

                "pop edx "
                "pop ecx "
                "pop ebx "

                "ret "_s);

        using thread = ctai::declare_code<sys_exit_thread,
                                          sys_create_thread>;

    }

}
