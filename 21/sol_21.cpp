#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>

#include "../utility.h"

namespace Day21
{
    using KeyPad = std::vector<std::vector<char>>;
    using TransitMap = std::unordered_map<std::string, std::vector<std::string>>;
    using TCoord = Point<int>;
    using TCompl = std::uint64_t;
    using Memo = std::vector<std::unordered_map<std::string,uint64_t>>;

    constexpr char DEPTH_1{ 2 };
    constexpr char DEPTH_2{ 25 };

    TransitMap get_transition_map(const KeyPad& pad);
    std::string get_unique_id(char start, char end);
    std::uint64_t translate_numpad(const TransitMap& transit_map, const TransitMap& robot_map, const std::string& code, Memo& memo, int level);
    TCompl calc_complexity(const std::string& init_code, const std::string& end_code);
    TCompl calc_complexity(const std::string& init_code, uint64_t len);
    bool is_valid_move(const std::string& move);
    uint64_t translate_robo_code(const TransitMap& transit_map, const std::string& code, Memo& memo, int level);

    std::ostream& print(const std::vector<std::string> &garden, std::ostream& out);


    TCompl sol_21_1(const std::string &file_path)
    {
        std::vector<std::string> code_vec = read_string_vec_from_file(file_path);

        std::vector<std::vector<char>> door_pad{ {'7','8','9'}, {'4','5','6'}, {'1','2','3'}, {'-', '0', 'A'} };
        std::vector<std::vector<char>> robot_pad{ {'-','^','A'}, {'<','v','>'} };

        auto door_transit_map = get_transition_map(door_pad);
        auto robot_transit_map = get_transition_map(robot_pad);
        TCompl sum_complexity{};
        Memo memo(DEPTH_1+1, std::unordered_map<std::string,uint64_t>{});

        for (auto door_code : code_vec) {
            auto robot_code = translate_numpad(door_transit_map, robot_transit_map, door_code, memo, DEPTH_1);
            sum_complexity += calc_complexity(door_code, robot_code);
        }

        return sum_complexity;
    }

    TCompl sol_21_2(const std::string &file_path)
    {
        std::vector<std::string> code_vec = read_string_vec_from_file(file_path);

        std::vector<std::vector<char>> door_pad{ {'7','8','9'}, {'4','5','6'}, {'1','2','3'}, {'-', '0', 'A'} };
        std::vector<std::vector<char>> robot_pad{ {'-','^','A'}, {'<','v','>'} };

        auto door_transit_map = get_transition_map(door_pad);
        auto robot_transit_map = get_transition_map(robot_pad);
        TCompl sum_complexity{};
        Memo memo(DEPTH_2+1, std::unordered_map<std::string,uint64_t>{});

        for (auto door_code : code_vec) {
            auto robot_code = translate_numpad(door_transit_map, robot_transit_map, door_code, memo, DEPTH_2);
            sum_complexity += calc_complexity(door_code, robot_code);
        }

        return sum_complexity;
    }

    /*
    For the robo-codes each string can be broken into a pattern A*****A, so we always start at an A and end at an A
    This can be used in a recursive approach where each pattern A****A is calculated at most once per level and then 
    stored in a buffer for later lookup (memoization)
    */
    uint64_t translate_robo_code(const TransitMap& transit_map, const std::string& code, Memo& memo, int level)
    {
        auto item = memo[level].find(code);
        if (item != memo[level].end()) return memo[level][code];
        if (level == 0) return memo[level][code] = code.length();

        char start = 'A';
        uint64_t min_sum{ };

        for (int i=0; i<code.length(); ++i) {
            auto id = get_unique_id(start, code[i]);

            size_t min_size_new_code{ std::string::npos };

            // sometimes multiple transit codes of same length are valid -> test which results in a shorter code at the bottom level
            for (auto transit_code : transit_map.at(id)) {
                auto res = translate_robo_code(transit_map, transit_code + 'A', memo, level-1);
                min_size_new_code = std::min(min_size_new_code, res);
            }
            min_sum += min_size_new_code;
            start = code[i];
        }

        return memo[level][code] = min_sum;
    }

    std::uint64_t translate_numpad(const TransitMap& transit_map, const TransitMap& robot_map, const std::string& code, Memo& memo, int level)
    {
        auto item = memo[level].find(code);
        if (item != memo[level].end()) return memo[level][code];
        if (level == 0) return memo[level][code] = code.length();

        char start = 'A';

        std::uint64_t sum{ };

        // iterate through each number pair and calculate the minimal code-representation at the bottom level through recursion
        for (int i=0; i<code.length(); ++i) {
            auto id = get_unique_id(start, code[i]);

            size_t min_size_new_code{ std::string::npos };

            // if multiple equally long transit codes exist -> test which results in a shorter code at the bottom level
            for (auto transit_code : transit_map.at(id)) {
                auto res = translate_robo_code(robot_map, transit_code + 'A', memo, level);
                min_size_new_code = std::min(min_size_new_code, res);
            }
            start = code[i];
            sum += min_size_new_code;
        }
        

        return memo[level][code] = sum;
    }

    TransitMap get_shortest_paths(const KeyPad& pad, TCoord start)
    {
        TransitMap shortest_path_map;
        auto start_tile = pad[start.x][start.y];
        // static std::vector<std::pair<Point<int>,char>> dirs{ {{-1,0},'^'},{{0,1},'>'}, {{1,0},'v'},{{0,-1},'<'} };
        static std::vector<std::pair<Point<int>,char>> dirs{ {{0,-1},'<'}, {{-1,0},'^'}, {{1,0},'v'}, {{0,1},'>'} };
        struct Node {
            TCoord p;
            std::string moves;
        };
        std::queue<Node> qu{};
        qu.push({ start,"" });
        shortest_path_map[get_unique_id(start_tile, start_tile)] = {""};
        
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
                    auto elem = shortest_path_map.find(id);
                    auto nxt_moves = moves+dir_char;
                    if (elem == shortest_path_map.end()) {
                        if (is_valid_move(nxt_moves)) {
                            shortest_path_map[id].push_back(nxt_moves);
                            qu.push({ nxt_p,nxt_moves });
                        }
                    }
                    else if (elem->second[0].length() == nxt_moves.length() && is_valid_move(nxt_moves)) {
                        shortest_path_map[id].push_back(nxt_moves);
                        qu.push({ nxt_p, nxt_moves });
                    }
                }
            }
        }

        return shortest_path_map;
    }

    bool is_valid_move(const std::string& move) {
        // check that a move-string contains at max one character change in its sequence -> <<^^ and ^^<< are valid but <^<^ or <^^< are not valid
        int num_changes{ 0 };

        for (int i=0; i<move.length()-1; ++i) {
            if (move[i] != move[i+1]) {
                ++num_changes;
            }
        }
        return num_changes < 2;
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
        complexity *= end_code.length();

        return complexity;
    }

    TCompl calc_complexity(const std::string& init_code, uint64_t len)
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
        complexity *= len;

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