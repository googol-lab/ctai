#include "execute/execute_code.hpp"
#include "declare_code.hpp"
#include "kernel/thread.hpp"
#include "runtime/io.hpp"


#include <iostream>
#include <iterator>
#include "kernel/io.hpp"
#include "kernel/utils.hpp"
#include "kernel/memory.hpp"
#include "kernel/mutex.hpp"

using main_code = decltype(
":main "
        //push parameters
        "mov eax , 0 "
        "push eax "
        "mov eax , 19 "
        "push eax "
        "mov eax , .thread_code "
        "push eax "

        "call .sys_create_thread "
        "add esp , 12 "
        "mov esi , eax " //store thread id


        "mov eax , 'H' "
        "call .sys_write "
        "mov eax , 'e' "
        "call .sys_write "
        "mov eax , 'l' "
        "call .sys_write "
        "mov eax , 'l' "
        "call .sys_write "
        "mov eax , 'o' "
        "call .sys_write "

        "mov eax , 'f' "
        "call .sys_write "
        "mov eax , 'r' "
        "call .sys_write "
        "mov eax , 'o' "
        "call .sys_write "
        "mov eax , 'm' "
        "call .sys_write "

        "mov eax , 'm' "
        "call .sys_write "
        "mov eax , 'a' "
        "call .sys_write "
        "mov eax , 'i' "
        "call .sys_write "
        "mov eax , 'n' "
        "call .sys_write "


        "push esi "
        "call .join_thread "
        "add esp , 4 "


        "call .sys_exit_thread"_s);

using thread_code = decltype(
":thread_code "

        "mov eax , DWORD PTR [ esp ] "
        "call .lock_mutex "

        "mov eax , 'H' "
        "call .sys_write "
        "mov eax , 'e' "
        "call .sys_write "
        "mov eax , 'l' "
        "call .sys_write "
        "mov eax , 'l' "
        "call .sys_write "
        "mov eax , 'o' "
        "call .sys_write "

        "mov eax , 'f' "
        "call .sys_write "
        "mov eax , 'r' "
        "call .sys_write "
        "mov eax , 'o' "
        "call .sys_write "
        "mov eax , 'm' "
        "call .sys_write "

        "mov eax , 't' "
        "call .sys_write "
        "mov eax , 'h' "
        "call .sys_write "
        "mov eax , 'r' "
        "call .sys_write "
        "mov eax , 'e' "
        "call .sys_write "
        "mov eax , 'a' "
        "call .sys_write "
        "mov eax , 'd' "
        "call .sys_write "
        "mov eax , 10 "
        "call .sys_write "

        "mov eax , DWORD PTR [ esp ] "
        "call .unlock_mutex "

        "call .sys_exit_thread "_s);

using code = ctai::declare_code<ctai::include::thread ,
        ctai::include::sys_write,
                                thread_code,
                                main_code>;

using main2 = decltype(
":main "
        "mov eax , 0 "
        "push eax "

        //parameters
        "mov ebx , .thread_code " //start point
        "mov ecx , 10 "//priority
        "mov edx , esp "//pointer to args

        "call .sys_create_thread "
        "mov esi , eax " //store thread id

        "call .sys_create_thread "
        "mov edi , eax " //store thread id

        "mov eax , esi "
        "call .join_thread "
        "mov eax , edi "
        "call .join_thread "

        "call .sys_exit_thread"_s);

using code2 = ctai::declare_code<ctai::include::thread,
                                 ctai::include::memory,
                                 ctai::include::io,
                                 ctai::include::utils,
                                 ctai::include::mutex,
                                 thread_code,
                                 main2>;



using execution_result = ctai::execute2::execute_code<code2>;

int main()
{
    constexpr auto output = ctai::runtime::io::make_runtime_output<execution_result::output>();

    std::cout << "Return value: " << execution_result::ret_val << "\n";

    std::cout << "execution output: \n";

    for(auto c : output)
        std::cout<<c;

    return 0;
}

#include "tests/tests_main.hpp"