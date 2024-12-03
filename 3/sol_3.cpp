#include <string>
#include <algorithm>
#include <regex>
#include <numeric>

#include "../utility.h"

using MemNum = int64_t;


struct Mul{
    MemNum a;
    MemNum b;
};

const std::regex MUL_REGEX_PATTERN{ "mul\\((\\d{1,3}),(\\d{1,3})\\)" };
const std::regex MUL_DO_REGEX_PATTERN{ "mul\\((\\d{1,3}),(\\d{1,3})\\)|(don't)\\(\\)|(do)\\(\\)" }; // make sure to check for 'don't' before 'do' 
std::vector<Mul> get_mul_list(const std::vector<std::string> &corrupted_memory_lines);
MemNum get_mul_sum(const std::vector<std::string> &corrupted_memory_lines);

MemNum sol_3_1(const std::string &file_path)
{
    std::vector<std::string> corrupted_memory_lines = read_string_vec_from_file(file_path);
    std::vector<Mul> mul_list = get_mul_list(corrupted_memory_lines);

    std::vector<MemNum> mul_results;

    std::transform(mul_list.begin(), mul_list.end(), std::back_inserter(mul_results), [](const Mul& mul) { return mul.a * mul.b;});

    return std::accumulate(mul_results.begin(), mul_results.end(), 0);
}


MemNum sol_3_2(const std::string &file_path)
{
    std::vector<std::string> corrupted_memory_lines = read_string_vec_from_file(file_path);

    return get_mul_sum(corrupted_memory_lines);
}


MemNum get_mul_sum(const std::vector<std::string> &corrupted_memory_lines)
{
    MemNum mul_sum{ 0 };
    
    bool is_active{ true };
    for (const auto& line : corrupted_memory_lines)
    {
        auto mul_begin = std::sregex_iterator(line.begin(), line.end(), MUL_DO_REGEX_PATTERN);
        auto mul_end = std::sregex_iterator();
        for (std::sregex_iterator i = mul_begin; i != mul_end; ++i)
        {
            std::smatch match = *i;
            if (match[4] == "do") {
                is_active = true;
            }
            else if(match[3] == "don't") {
                is_active = false;
            }
            else {
                if (is_active) {
                    mul_sum += std::stoi(match[1]) * std::stoi(match[2]);
                }
            }
        }
    }

    return mul_sum;
}
std::vector<Mul> get_mul_list(const std::vector<std::string> &corrupted_memory_lines)
{
    std::vector<Mul> mul_vec;
    
    for (const auto& line : corrupted_memory_lines)
    {
        auto mul_begin = std::sregex_iterator(line.begin(), line.end(), MUL_REGEX_PATTERN);
        auto mul_end = std::sregex_iterator();
        for (std::sregex_iterator i = mul_begin; i != mul_end; ++i)
        {
            std::smatch match = *i;

            // construct Mul instances based on groups in current match
            mul_vec.push_back(Mul{ std::stoi(match[1]), std::stoi(match[2]) });
        }
    }

    return mul_vec;
}
