#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <limits>

#include "../utility.h"

struct InputDataDay5_1
{
    std::unordered_set<Point<int>, Point<int>::HashFunction> reversed_ordering_rules; // contains the ordering rules in reversed form
    std::vector<std::vector<int>> page_updates;
};

InputDataDay5_1 get_input_5_1(const std::string &file_path);
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> get_correct_page_updates(const InputDataDay5_1 &input_data) ;
std::vector<int> fix_update(std::vector<int> update_nums, const InputDataDay5_1& input_data);

int sol_5_1(const std::string &file_path)
{
    uint64_t middle_num_sum{ };
    InputDataDay5_1 data_in = get_input_5_1(file_path);
    auto [correct_updates,incorrect_updates] = get_correct_page_updates(data_in);
    for (const auto& update_nums : correct_updates) {
        middle_num_sum += update_nums[update_nums.size()/2];
    }

    return middle_num_sum;
}


int sol_5_2(const std::string &file_path)
{
    uint64_t middle_num_sum{ };
    InputDataDay5_1 data_in = get_input_5_1(file_path);
    auto [correct_updates,incorrect_updates] = get_correct_page_updates(data_in);

    for (auto update_nums : incorrect_updates) {
        auto fixed_update_nums = fix_update(update_nums, data_in);
        middle_num_sum += fixed_update_nums[update_nums.size()/2];
    }
    return middle_num_sum;
}

// switch numbers whenever a order rule is violated
std::vector<int> fix_update(std::vector<int> update_nums, const InputDataDay5_1& input_data) {

    for (int i=0; i<update_nums.size(); ++i) {
        for (int j=i+1; j<update_nums.size(); ++j) {
            Point<int> page_pair{ update_nums[i], update_nums[j] };

            // search for rule violations and switch the two numbers
            if (input_data.reversed_ordering_rules.find(page_pair) != input_data.reversed_ordering_rules.end()) {
                update_nums[i] = update_nums[j];
                update_nums[j] = page_pair.x;
            }
        }
    }

    return update_nums;
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> get_correct_page_updates(const InputDataDay5_1 &input_data) 
{
    std::vector<std::vector<int>> correct_updates{ };
    std::vector<std::vector<int>> incorrect_updates{ };

    for (const auto& update_nums : input_data.page_updates) {
        bool is_correct{ true };
        for (int i=0; i<update_nums.size() && is_correct; ++i) {
            for (int j=i+1; j<update_nums.size(); ++j) {
                Point<int> page_pair{ update_nums[i], update_nums[j] };

                // search for a rule violation
                if (input_data.reversed_ordering_rules.find(page_pair) != input_data.reversed_ordering_rules.end()) {
                    is_correct = false;
                    break;
                }
            }
        }

        // store vector in respective container
        if (is_correct) correct_updates.push_back(update_nums);
        else incorrect_updates.push_back(update_nums);
    }

    return { correct_updates, incorrect_updates };
}

InputDataDay5_1 get_input_5_1(const std::string &file_path)
{
    InputDataDay5_1 input_data{ };

    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line{ "start" };

        // get page orderings until an empty line is hit
        while(getline(input_file, input_line) && input_line != "")
        {
            auto nums = parse_string_to_number_vec<int>(input_line);
            input_data.reversed_ordering_rules.insert({ nums[1], nums[0] });
        }

        // get updates
        while(getline(input_file, input_line))
        { 
            auto nums = parse_string_to_number_vec<int>(input_line);
            input_data.page_updates.push_back(nums);
        }

        input_file.close();   
    }

    return input_data;
}