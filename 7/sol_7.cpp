#include <string>
#include <array>
#include <map>
#include <algorithm>

#include "../utility.h"

namespace Day7
{
    using TValue = uint64_t;

    struct Equation{
        TValue test_value;
        std::vector<TValue> nums;
    };

    bool rec_test_pt1(const Equation& eq, int pos, TValue tmp_res);
    bool rec_test_pt2(const Equation& eq, int pos, TValue tmp_res);

    std::vector<Equation> get_data(const std::string& file_path);

    TValue sol_7_1(const std::string &file_path)
    {
        auto equ_vec = get_data(file_path);

        TValue sum{ };
        for (const auto& eq : equ_vec) {
            if (rec_test_pt1(eq, 1, eq.nums[0])){
                sum += eq.test_value;
            }
        }

        return sum;
    }


    TValue sol_7_2(const std::string &file_path)
    {
        auto equ_vec = get_data(file_path);

        TValue sum{ };
        for (const auto& eq : equ_vec) {
            if (rec_test_pt2(eq, 1, eq.nums[0])){
                sum += eq.test_value;
            }
        }

        return sum;
    }

    bool rec_test_pt1(const Equation& eq, int pos, TValue tmp_res)
    {
        if (pos == eq.nums.size()) return tmp_res == eq.test_value;

        auto test_1 = rec_test_pt1(eq, pos+1, tmp_res + eq.nums.at(pos));
        auto test_2 = rec_test_pt1(eq, pos+1, tmp_res * eq.nums.at(pos));

        return test_1 || test_2;
    }

    bool rec_test_pt2(const Equation& eq, int pos, TValue tmp_res)
    {
        if (pos == eq.nums.size()) return tmp_res == eq.test_value;

        auto test_1 = rec_test_pt2(eq, pos+1, tmp_res + eq.nums.at(pos));
        auto test_2 = rec_test_pt2(eq, pos+1, tmp_res * eq.nums.at(pos));

        auto shifted_tmp_res = tmp_res; 
        auto r_val{ eq.nums.at(pos) };
        while (r_val > 0) {
            r_val /= 10;
            shifted_tmp_res *= 10;
        }

        auto test_3 = rec_test_pt2(eq, pos+1, shifted_tmp_res + eq.nums.at(pos));

        return test_1 || test_2 || test_3;
    }

    std::vector<Equation> get_data(const std::string& file_path){
        std::vector<Equation> equ_vec{};
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                auto num_vec = parse_string_to_number_vec<TValue>(input_line);
                auto equ = Equation{ num_vec[0], std::vector<TValue>(num_vec.begin()+1, num_vec.end()) };
                equ_vec.push_back(equ);
            }
            input_file.close();
        }

        return equ_vec;
    }

}