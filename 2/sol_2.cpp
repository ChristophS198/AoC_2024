#include <string>

#include "../utility.h"

constexpr size_t NUM_COLORS{ 3u };


bool is_report_safe(const std::vector<int>& report, int min_level_diff, int max_level_diff, int monotonicity=0);
bool is_report_safe_with_dampener(const std::vector<int>& report, int min_level_diff, int max_level_diff, int monotonicity);
std::vector<std::vector<int>> get_report_list(const std::string &file_path);

void print_vec(const std::vector<int>& vec) {
    for (const auto elem : vec) std::cout << elem << " ";
    std::cout << std::endl;
}

int sol_2_1(const std::string &file_path)
{
    std::vector<std::vector<int>> report_list = get_report_list(file_path);

    int num_safe_reports{ 0 };

    for (const auto& report : report_list) {
        if (is_report_safe(report, 1, 3)) {
            ++num_safe_reports;
        }
    }

    return num_safe_reports;
}


int sol_2_2(const std::string &file_path)
{
    std::vector<std::vector<int>> report_list = get_report_list(file_path);

    int num_safe_reports{ 0 };

    for (const auto& report : report_list) {
        if (is_report_safe_with_dampener(report, 1, 3, 1) || is_report_safe_with_dampener(report, 1, 3, -1)) {
            ++num_safe_reports;
        }
    }

    return num_safe_reports;
}

/*
Idea is to iterate through report until a non-matching level-diff is reached.
Here we have two choices: either skip first level in non-matching diff or skip second level. 
For either one the remaining report must be safe + the transition to the remaining report must be checked separately.
*/
bool is_report_safe_with_dampener(const std::vector<int>& report, int min_level_diff, int max_level_diff, int monotonicity) {
    int sign = monotonicity;
    bool joker{ true };

    for (int i=1; i<report.size(); ++i) {
        auto diff = sign * (report[i] - report[i-1]);

        if (diff <= 0 || diff < min_level_diff || diff > max_level_diff) {
            if (joker) {
                // choice 1: skip level i-1
                if (is_report_safe(std::vector<int>(report.begin() + i, report.end()), min_level_diff, max_level_diff, monotonicity)) {
                    if (i == 1) {
                        return true;
                    }
                    else {
                        diff = sign * (report[i] - report[i-2]);
                        // check transition from level i-2 to i. For "bad cases" we do not return but go on and check the second choice  
                        if (diff > 0 && diff >= min_level_diff && diff <= max_level_diff) {
                            return true;
                        }
                    }
                }
                
                // choice 2: skip level i
                if (is_report_safe(std::vector<int>(report.begin() + i + 1, report.end()), min_level_diff, max_level_diff, monotonicity)){
                    if (i == report.size()-1) {
                        return true;
                    }
                    // check transition from level i-1 to i+1
                    diff = sign * (report[i+1] - report[i-1]);
                    if (diff <= 0 || diff < min_level_diff || diff > max_level_diff) {
                        return false;
                    }
                    else {
                        return true;
                    }
                }
                return false;
            }
            else {
                return false;
            }
        }
    }

    return true;
}

bool is_report_safe(const std::vector<int>& report, int min_level_diff, int max_level_diff, int monotonicity) {
    if (report.size() < 2) return true;

    int sign = (report[1] - report[0]) > 0 ? 1 : -1;
    if (monotonicity != 0) {
        sign = monotonicity;
    }

    for (int i=1; i<report.size(); ++i) {
        auto diff = sign * (report[i] - report[i-1]);

        if (diff < 0 || diff < min_level_diff || diff > max_level_diff) {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<int>> get_report_list(const std::string &file_path)
{
    std::vector<std::vector<int>> report_list;

    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {
            report_list.push_back(parse_string_to_number_vec<int>(input_line));
        }
    }
    return report_list;
}