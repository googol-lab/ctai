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
                "push ebp "
                "mov ebp , esp "
                "push ebx "
                "push ecx "
                "push edx "
                "mov eax , 'C' "
                "call .sys_write "

                "mov ebx , DWORD PTR [ ebp + 16 ] "//void* start_point\n
                "mov ecx , DWORD PTR [ ebp + 12 ] "//priority
                "mov edx , DWORD PTR [ ebp + 8 ] "//arg

                "sys_create_thread "

                "pop edx "
                "pop ecx "
                "pop ebx "
                "pop ebp "

                "ret "_s);

        using sys_is_thread_running = decltype(
        ":sys_is_thread_running "
                "mov eax , DWORD PTR [ esp + 4 ] "//thread id
                "sys_is_thread_running "
                "ret "_s
        );

        using join_thread = decltype(
        ":join_thread "
                "mov eax , DWORD PTR [ esp + 4 ] " //thread id
                "push eax "

                ":_jt_check_loop " //wait while thread is running
                    "mov eax , 'J' "
                    "call .sys_write "
                    "call .sys_is_thread_running "
                    "cmp eax , 0 "
                    "jne ._jt_check_loop "

                "add esp , 4 " //cleanup stack
                "ret "_s
        );

        using thread = ctai::declare_code<sys_exit_thread,
                                          sys_create_thread,
                                          sys_is_thread_running,
                                          join_thread>;

    }

}
