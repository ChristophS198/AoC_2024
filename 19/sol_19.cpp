#include <string>
#include <unordered_map>

#include "../utility.h"


namespace Day19
{

    using TowelMap=std::unordered_map<std::string,bool>; // value denotes whether its a substring (false) or a valid pattern (true)

    struct Data {
        TowelMap exist_towel;
        std::vector<std::string> desired_towel{};
    };

    Data get_data(const std::string &file_path);
    uint64_t get_num_designs(const TowelMap& exist, const std::string& towel, std::unordered_map<std::string, uint64_t>& memo);

    int sol_19_1(const std::string &file_path)
    {
        auto data_in = get_data(file_path);
        int valid_ct{ 0 };

        for (auto& patt : data_in.desired_towel) {
            std::unordered_map<std::string, uint64_t> memo;
            if (get_num_designs(data_in.exist_towel, patt, memo) > 0) {
                ++valid_ct;
            }
        }

        return valid_ct;
    }


    uint64_t sol_19_2(const std::string &file_path)
    {
        auto data_in = get_data(file_path);

        uint64_t valid_ct{ 0 };

        for (auto& patt : data_in.desired_towel) {
            std::unordered_map<std::string, uint64_t> memo;
            valid_ct += get_num_designs(data_in.exist_towel, patt, memo);
        }

        return valid_ct;
    }

    uint64_t get_num_designs(const TowelMap& exist, const std::string& towel, std::unordered_map<std::string, uint64_t>& memo) {
        
        if (towel.length() == 0) return 1;
        auto tmp = memo.find(towel);
        if (tmp != memo.end()) return tmp->second;

        uint64_t num_designs{ 0 };
        for (int i=1; i<=towel.length(); ++i) {
            auto prefix = towel.substr(0,i);
            auto elem = exist.find(prefix);

            if (elem == exist.end()) {
                break;
            }

            if (elem->second) {
                // matches an existing towel pattern
                num_designs += get_num_designs(exist, towel.substr(i), memo);
            }
        }

        return memo[towel] = num_designs;
    }

    Data get_data(const std::string &file_path)
    {
        Data data{ };

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;

            // get existing towel patterns
            getline(input_file, input_line);
            auto exist_towel_vec = split_string(input_line, ", ");
            getline(input_file, input_line);

            // insert each substring of an existing towel
            for (auto &towel : exist_towel_vec) {
                for (int i=1; i<towel.length(); ++i) {
                    data.exist_towel.insert( {towel.substr(0,i), false}); // use insert so we do not overwrite any existing towels
                }
                data.exist_towel[towel] =true;
            }

            // get desired towel patterns
            while(getline(input_file, input_line))
            {  
                data.desired_towel.push_back(input_line);

            }
            input_file.close();
        }
        return data;
    }
}