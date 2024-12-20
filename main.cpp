#include <iostream>
#include <string>
// #include "1/sol_1.cpp"
// #include "2/sol_2.cpp"
// #include "3/sol_3.cpp"
// #include "4/sol_4.cpp"
// #include "5/sol_5.cpp"
// #include "6/sol_6.cpp"
// #include "7/sol_7.cpp"
// day 8 is solved with python
// #include "9/sol_9.cpp"
// #include "10/sol_10.cpp"
// #include "11/sol_11.cpp"
// #include "12/sol_12.cpp"
// #include "13/sol_13.cpp"
// #include "14/sol_14.cpp"
// #include "15/sol_15.cpp"
// #include "16/sol_16.cpp"
// #include "17/sol_17.cpp"
// #include "18/sol_18.cpp"
// #include "19/sol_19.cpp"
#include "20/sol_20.cpp"
// #include "21/sol_21.cpp"
// #include "22/sol_22.cpp"
// #include "23/sol_23.cpp"
// #include "24/sol_24.cpp"
// #include "20/sol_20.cpp"
#include <filesystem>

std::string get_input_file_name(int day)
{
    #ifdef DIR_PATH
        std::string path = DIR_PATH;
        path += "/";
        return path + std::to_string(day) + "/data.txt";
    #endif

    return "../" + std::to_string(day) + "/data.txt";
}

int main(int /*argc*/, char** /*argv*/)
{
    /*
    std::cout << sol_1_1(get_input_file_name(1)) << std::endl;
    std::cout << sol_1_2(get_input_file_name(1)) << std::endl;
    std::cout << sol_2_1(get_input_file_name(2)) << std::endl;
    std::cout << sol_2_2(get_input_file_name(2)) << std::endl;
    std::cout << sol_3_1(get_input_file_name(3)) << std::endl;
    std::cout << sol_3_2(get_input_file_name(3)) << std::endl;
    std::cout << sol_4_1(get_input_file_name(4)) << std::endl;
    std::cout << sol_4_2(get_input_file_name(4)) << std::endl;
    std::cout << sol_5_1(get_input_file_name(5)) << std::endl;
    std::cout << sol_5_2(get_input_file_name(5)) << std::endl;
    std::cout << sol_6_1(get_input_file_name(6)) << std::endl;
    std::cout << sol_6_2(get_input_file_name(6)) << std::endl;
    std::cout << sol_7_1(get_input_file_name(7)) << std::endl;
    std::cout << sol_7_2(get_input_file_name(7)) << std::endl;
    */
    /*
    std::cout << sol_9_1(get_input_file_name(9)) << std::endl;
    std::cout << sol_9_2(get_input_file_name(9)) << std::endl;
    std::cout << sol_10_1(get_input_file_name(10)) << std::endl;
    std::cout << sol_10_2(get_input_file_name(10)) << std::endl;
    std::cout << sol_11_1(get_input_file_name(11)) << std::endl;
    std::cout << sol_11_2(get_input_file_name(11)) << std::endl;
    std::cout << sol_12_1(get_input_file_name(12)) << std::endl;
    std::cout << sol_12_2(get_input_file_name(12)) << std::endl;
    std::cout << sol_13_1(get_input_file_name(13)) << std::endl;
    std::cout << sol_13_2(get_input_file_name(13)) << std::endl;
    std::cout << sol_14_1(get_input_file_name(14)) << std::endl;
    std::cout << sol_14_2(get_input_file_name(14)) << std::endl;
    std::cout << sol_15_1(get_input_file_name(15)) << std::endl;
    std::cout << sol_15_2(get_input_file_name(15)) << std::endl;
    std::cout << sol_16_1(get_input_file_name(16)) << std::endl;
    std::cout << sol_16_2(get_input_file_name(16)) << std::endl;
    std::cout << sol_17_1(get_input_file_name(17)) << std::endl;
    std::cout << sol_17_2(get_input_file_name(17)) << std::endl;
    std::cout << Day18::sol_18_1(get_input_file_name(18)) << std::endl;
    std::cout << Day18::sol_18_2(get_input_file_name(18)) << std::endl;
    std::cout << Day19::sol_19_1(get_input_file_name(19)) << std::endl;
    std::cout << Day19::sol_19_2(get_input_file_name(19)) << std::endl;
    */
    std::cout << Day20::sol_20_1(get_input_file_name(20)) << std::endl;
    std::cout << Day20::sol_20_2(get_input_file_name(20)) << std::endl;
    /*
    std::cout << Day21::sol_21_1(get_input_file_name(21)) << std::endl;
    std::cout << Day21::sol_21_2(get_input_file_name(21)) << std::endl;
    std::cout << Day22::sol_22_1(get_input_file_name(22)) << std::endl;
    std::cout << Day22::sol_22_2(get_input_file_name(22)) << std::endl;
    std::cout << Day23::sol_23_1(get_input_file_name(23)) << std::endl;
    std::cout << Day23::sol_23_2(get_input_file_name(23)) << std::endl;
    std::cout << Day24::sol_24_1(get_input_file_name(24)) << std::endl;
    std::cout << Day24::sol_24_2(get_input_file_name(24)) << std::endl;
    */

    return 0;
}
