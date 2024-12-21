#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>

#include "../utility.h"

namespace Day21
{
    using KeyPad = std::vector<std::vector<char>>;
    using TransitMap = std::unordered_map<std::string, std::string>;
    using TCoord = Point<int>;
    using TCompl = std::uint64_t;

    constexpr char GARDEN_PLOT{ '.' };
    constexpr char ROCK{ '#' };
    constexpr char START_POS{ 'S' };
    constexpr char REACHABLE{ 'O' };

    TransitMap get_transition_map(const KeyPad& pad);
    std::string get_unique_id(char start, char end);
    std::string translate_to_code(const TransitMap& transit_map, const std::string& code);
    TCompl calc_complexity(const std::string& init_code, const std::string& end_code);

    std::ostream& print(const std::vector<std::string> &garden, std::ostream& out);

    TCompl sol_21_1(const std::string &file_path)
    {
        std::vector<std::string> code_vec = read_string_vec_from_file(file_path);

        std::vector<std::vector<char>> door_pad{ {'7','8','9'}, {'4','5','6'}, {'1','2','3'}, {'-', '0', 'A'} };
        std::vector<std::vector<char>> robot_pad{ {'-','^','A'}, {'<','v','>'} };

        auto door_transit_map = get_transition_map(door_pad);
        auto robot_transit_map = get_transition_map(robot_pad);
        TCompl sum_complexity{};

        for (auto door_code : code_vec) {
            auto code = "A" + door_code;
            auto robot_code = translate_to_code(door_transit_map, code);
            std::cout << robot_code << std::endl;
            for (int i=0; i<2; ++i) {
                robot_code = "A" + robot_code;
                robot_code = translate_to_code(robot_transit_map, robot_code);
                std::cout << robot_code << std::endl;
            }
            sum_complexity += calc_complexity(door_code, robot_code);
        }

        return sum_complexity;
    }


    int sol_21_2(const std::string &file_path)
    {

        return 0;
    }

    std::string translate_to_code(const TransitMap& transit_map, const std::string& code)
    {
        std::string new_code{ };

        for (int i=0; i<code.length()-1; ++i) {
            auto id = get_unique_id(code[i], code[i+1]);
            new_code.append(transit_map.at(id));
            new_code += 'A';
        }

        return new_code;
    }


    TransitMap get_shortest_paths(const KeyPad& pad, TCoord start)
    {
        TransitMap shortest_path_map;
        auto start_tile = pad[start.x][start.y];
        static std::vector<std::pair<Point<int>,char>> dirs{ {{0,1},'>'},{{-1,0},'^'}, {{1,0},'v'},{{0,-1},'<'} };
        struct Node {
            TCoord p;
            std::string moves;
        };
        std::queue<Node> qu{};
        qu.push({ start,"" });
        shortest_path_map[get_unique_id(start_tile, start_tile)] = "";
        
        while (!qu.empty()) {
            auto [p, moves] = qu.front();
            qu.pop();

            for (auto [dir, dir_char] : dirs) {
                auto nxt_p = p + dir;
                if (nxt_p.x<0 || nxt_p.x>= pad.size() || nxt_p.y<0 || nxt_p.y >= pad.at(0).size()
                    ||  pad.at(nxt_p.x)[nxt_p.y] == '-') {
                    continue; // not valid
                }
                else {
                    auto nxt_tile = pad.at(nxt_p.x)[nxt_p.y];
                    auto id = get_unique_id(start_tile, nxt_tile);  // unique id for each transition
                    if (shortest_path_map.insert({ id,moves+dir_char }).second) {
                        // if insertion was successful, we did not know the shortest path to nxt_p -> add the node to queue
                        qu.push({ nxt_p,moves+dir_char });
                    }
                }
            }
        }

        return shortest_path_map;
    }

    TransitMap get_transition_map(const KeyPad& pad)
    {
        TransitMap transit_map;
        int n_row{ pad.size() };
        int n_col{ pad.at(0).size() };

        for (int row=0; row<n_row; ++row)
        {
            for (int col=0; col<n_col; ++col)
            {
                if ('-' != pad.at(row).at(col))
                {
                    auto tmp_map = get_shortest_paths(pad, TCoord{row,col});
                    transit_map.merge(tmp_map);
                }
            }
        }

        // for (auto [id, moves] : transit_map) {
        //     std::cout << id << ": " << moves << std::endl;
        // }
        return transit_map;
    }

    TCompl calc_complexity(const std::string& init_code, const std::string& end_code)
    {
        TCompl complexity{ };
        int i = 0;
        while (init_code[i] == '0') ++i;
        for (; i< init_code.length(); ++i) {
            auto c = init_code[i];
            if (std::isdigit(static_cast<unsigned char>(c)))
            {
                complexity = complexity*10ull + c - '0';
            }
        }
        std::cout << std::to_string(complexity) << " * " << std::to_string(end_code.length()) << std::endl;
        complexity *= end_code.length();

        return complexity;
    }

    std::string get_unique_id(char start, char end)
    {
        return std::string(1,start) + "," + end;
    }

    std::ostream& print(const std::vector<std::string> &garden, std::ostream& out)
    {
        for (const auto &row : garden)
        {
            for (const auto &e : row)
            {
                out << e;
            }
            out << "\n";
        }
        out << "\n";
        return out;
    }
}