#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../utility.h"

namespace Day8
{

    using TPos = Point<int>;
    using AntennaMap = std::unordered_map<char, std::vector<TPos>>;
    using AntinodeSet = std::unordered_set<TPos, TPos::HashFunction>;

    constexpr char FREE{ '.' };

    struct Data {
        AntennaMap antenna_map;
        size_t num_rows;
        size_t num_cols;
    };

    Data get_data(const std::string& file_path);
    std::vector<TPos> calc_antinode_pos(TPos p1, TPos p2, size_t num_rows, size_t num_cols);
    std::vector<TPos> calc_antinode_pos_2(TPos p1, TPos p2, size_t num_rows, size_t num_cols);
    AntinodeSet get_antinodes(const AntennaMap& antenna_map, size_t num_rows, size_t num_cols, bool pt_1=true);

    int sol_8_1(const std::string &file_path)
    {
        auto [antenna_map, num_rows, num_cols] = get_data(file_path);
        auto antinode_set = get_antinodes(antenna_map, num_rows, num_cols);

        return antinode_set.size();
    }


    int sol_8_2(const std::string &file_path)
    {
        auto [antenna_map, num_rows, num_cols] = get_data(file_path);
        auto antinode_set = get_antinodes(antenna_map, num_rows, num_cols, false);

        return antinode_set.size();
    }

    AntinodeSet get_antinodes(const AntennaMap& antenna_map, size_t num_rows, size_t num_cols, bool pt_1) {
        AntinodeSet antenna_set{};

        for (const auto& [id, antennas] : antenna_map) {
            for (int i=0; i<antennas.size(); ++i) {
                for (int j=i+1; j<antennas.size(); ++j) {
                    std::vector<TPos> antinode_pos;
                    if (pt_1) {
                        antinode_pos = calc_antinode_pos(antennas[i], antennas[j], num_rows, num_cols);
                    }
                    else {
                        antinode_pos = calc_antinode_pos_2(antennas[i], antennas[j], num_rows, num_cols);
                    }
                    antenna_set.insert(antinode_pos.begin(), antinode_pos.end());
                }
            }
        }

        return antenna_set;
    }

    std::vector<TPos> calc_antinode_pos_2(TPos p1, TPos p2, size_t num_rows, size_t num_cols)
    {
        std::vector<TPos> antinodes;
        TPos dist = p2 - p1;

        // check first direction
        auto a1 = p1;
        while (a1.x >= 0 && a1.x < num_rows && a1.y >= 0 && a1.y < num_cols) {
            antinodes.push_back(a1);
            a1 = a1 - dist;
        }

        // check second direction
        auto a2 = p2;
        while (a2.x >= 0 && a2.x < num_rows && a2.y >= 0 && a2.y < num_cols) {
            antinodes.push_back(a2);
            a2 = a2 + dist;
        }

        return antinodes;
    }

    std::vector<TPos> calc_antinode_pos(TPos p1, TPos p2, size_t num_rows, size_t num_cols)
    {
        std::vector<TPos> antinodes;
        TPos dist = p2 - p1;

        auto a1 = p1 - dist;
        auto a2 = p2 + dist;
        if (a1.x >= 0 && a1.x < num_rows && a1.y >= 0 && a1.y < num_cols) antinodes.push_back(a1);
        if (a2.x >= 0 && a2.x < num_rows && a2.y >= 0 && a2.y < num_cols) antinodes.push_back(a2);

        return antinodes;
    }


    Data get_data(const std::string& file_path)
    {
        AntennaMap antenna_map{};
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        int row{ 0 };
        int num_cols{ 0 };
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                for (int col=0; col<input_line.size(); ++col) {
                    if (input_line[col] != FREE) antenna_map[input_line[col]].push_back({ row,col });
                }
                ++row;
            }
            num_cols = input_line.length();
            input_file.close();
        }

        return { antenna_map,row,num_cols };
    }
}