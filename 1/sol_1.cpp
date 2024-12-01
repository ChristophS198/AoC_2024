#include <string>
#include <algorithm>
#include<map>

#include "../utility.h"

std::pair<std::vector<int>, std::vector<int>> read_numbers_from_file(const std::string& file_path);
std::pair<std::map<int, int>, std::map<int, int>> read_numbers_to_frequency_map(const std::string& file_path);

int sol_1_1(const std::string &file_path)
{
    int total_dist{ 0 };

    auto [location_id_1, location_id_2] = read_numbers_from_file(file_path);
    
    std::sort(location_id_1.begin(), location_id_1.end());
    std::sort(location_id_2.begin(), location_id_2.end());

    for (int i=0; i<location_id_1.size(); ++i) {
        total_dist += std::abs(location_id_1[i]-location_id_2[i]);
    }

    return total_dist;
}


int sol_1_2(const std::string &file_path)
{
    int similarity_score{ 0 };

    auto [location_map_1, location_map_2] = read_numbers_to_frequency_map(file_path);

    for (const auto [num, freq] : location_map_1) {
        int freq_in_location_map_2{ 0 };

        if (location_map_2.find(num) != location_map_2.end()) {
            freq_in_location_map_2 = location_map_2[num];
        }

        similarity_score += num * freq * freq_in_location_map_2;
    }

    return similarity_score;
}

std::pair<std::vector<int>, std::vector<int>> read_numbers_from_file(const std::string& file_path)
{
    std::vector<int> number_vec_1{};
    std::vector<int> number_vec_2{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {
            std::vector<int> num_in_line = parse_string_to_number_vec<int>(input_line);
            number_vec_1.push_back(num_in_line[0]);
            number_vec_2.push_back(num_in_line[1]);
        }
        input_file.close();   
    }
    return {number_vec_1, number_vec_2};
}

std::pair<std::map<int, int>, std::map<int, int>> read_numbers_to_frequency_map(const std::string& file_path)
{
    std::map<int, int> number_map_1{};
    std::map<int, int> number_map_2{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {
            std::vector<int> num_in_line = parse_string_to_number_vec<int>(input_line);
            
            number_map_1[num_in_line[0]]++;
            number_map_2[num_in_line[1]]++;
        }
        input_file.close();   
    }
    return {number_map_1, number_map_2};
}