#include <string>
#include <algorithm>
#include <vector>
#include <set>

#include "../utility.h"


enum TDirD6 {
    Up, 
    Right, 
    Down, 
    Left
};

constexpr char OBSTRUCTION_D6{ '#' };
constexpr char MARKED_D6{ 'X' };
constexpr char FREE_D6{ '0' };
const std::vector<std::vector<int>> DIR_VEC{ {-1,0}, {0,1}, {1,0}, {0,-1} }; // maps each of the TDirD6 enum values to a direction

struct GuardStateD6 {
    Point<int> pos;
    TDirD6 heading;
};

GuardStateD6 get_guard_state(const std::vector<std::vector<char>>& tile_map_d6);
std::vector<GuardStateD6> get_unique_pos(GuardStateD6 guard_state, std::vector<std::vector<char>> tile_map_d6);
int num_loop_pos(GuardStateD6 guard_state, const std::vector<GuardStateD6>& unique_pos_vec, std::vector<std::vector<char>> tile_map_d6);
std::vector<std::vector<char>> read_tile_map_from_file(const std::string& file_path);
bool is_cyclic(GuardStateD6 guard_state, std::vector<std::vector<char>>& tile_map_d6);

int sol_6_1(const std::string &file_path)
{
    std::vector<std::vector<char>> tile_map_d6 = read_tile_map_from_file(file_path);
    GuardStateD6 start_state = get_guard_state(tile_map_d6);

    // too high: 2024
    return get_unique_pos(start_state, tile_map_d6).size();
}


int sol_6_2(const std::string &file_path)
{
    std::vector<std::vector<char>> tile_map_d6 = read_tile_map_from_file(file_path);
    GuardStateD6 start_state = get_guard_state(tile_map_d6);
    std::vector<GuardStateD6> unique_pos_vec = get_unique_pos(start_state, tile_map_d6);

    return num_loop_pos(start_state, unique_pos_vec, tile_map_d6);
}

std::ostream& print_map(std::ostream &out,const std::vector<std::vector<char>>& terr) {
    for (const auto &r : terr)
    {
        for (const auto &elem : r)
        {
            if (elem == OBSTRUCTION_D6) {
                out << elem;
            }
            else if (elem == FREE_D6) {
                out << '.';
            }
            else if (elem & 1 || elem & (1<<2)) {
                if (elem & (1<<1) || elem & (1<<3)) {
                    out << '+';
                }
                else out << '|';
            }
            else if (elem & (1<<1) || elem & (1<<3)) {
                    out << '-';
                }
        }
        out << "\n";
    }
    out << "\n";
    return out;
}


int num_loop_pos(GuardStateD6 guard_state, const std::vector<GuardStateD6>& unique_pos_vec, std::vector<std::vector<char>> tile_map_d6) {
    int loop_ct{ };

    auto& g_pos = guard_state.pos;
    auto old_pos = guard_state.pos;
    tile_map_d6[g_pos.x][g_pos.y] = FREE_D6;

    std::set<std::string> loop_pos{};

    // start iterating from idx 1, because start position of guard is no valid obstacle position 
    for (int i=1; i<unique_pos_vec.size(); ++i) {
        const auto& unique_pos = unique_pos_vec[i];
        auto tile_copy = tile_map_d6;
        tile_copy[unique_pos.pos.x][unique_pos.pos.y] = OBSTRUCTION_D6;
        if (is_cyclic(guard_state, tile_copy)) {
                ++loop_ct;
        }
    }

    return loop_ct;
}

// idea is to use the last 4 bits of a Free tile to indicate the direction the guard already patrolled through this tile
bool is_cyclic(GuardStateD6 guard_state, std::vector<std::vector<char>>& tile_map_d6) {
    auto& g_pos = guard_state.pos;
    auto dir = DIR_VEC[guard_state.heading];

    auto old_pos = guard_state.pos;

    while (g_pos.x >= 0 && g_pos.y >= 0 && g_pos.x < tile_map_d6.size() && g_pos.y < tile_map_d6[0].size()) {

        // check current location
        if (tile_map_d6[g_pos.x][g_pos.y] == OBSTRUCTION_D6)
        {
            // retrieve old pos and rotate guard
            guard_state.pos = old_pos;
            tile_map_d6[g_pos.x][g_pos.y] |= (1 << guard_state.heading);
            guard_state.heading = TDirD6((guard_state.heading + 1) % 4);
        }

        if(tile_map_d6[g_pos.x][g_pos.y] & (1 << guard_state.heading)) 
        {            
            return true;
        }
        else 
        {
            dir = DIR_VEC[guard_state.heading];
            old_pos = g_pos;
            // set specific bit for each direction
            tile_map_d6[g_pos.x][g_pos.y] |= (1 << guard_state.heading);
            g_pos = g_pos + Point<int>{ dir[0], dir[1] };
        }
    }


    return false;
}

std::vector<GuardStateD6> get_unique_pos(GuardStateD6 guard_state, std::vector<std::vector<char>> tile_map_d6) {
    std::vector<GuardStateD6> unique_pos_vec{ guard_state }; // current position is already counted
    auto& g_pos = guard_state.pos;
    auto old_pos = guard_state.pos;
    tile_map_d6[g_pos.x][g_pos.y] = MARKED_D6;

    while (g_pos.x >= 0 && g_pos.y >= 0 && g_pos.x < tile_map_d6.size() && g_pos.y < tile_map_d6[0].size()) {
        auto dir = DIR_VEC[guard_state.heading];

        // check current location
        switch (tile_map_d6[g_pos.x][g_pos.y])
        {
        case OBSTRUCTION_D6:
            // retrieve old pos and rotate guard
            guard_state.pos = old_pos;
            guard_state.heading = TDirD6((guard_state.heading + 1) % 4);
            break;
        case MARKED_D6:
            // move on
            old_pos = guard_state.pos;
            g_pos = g_pos + Point<int>{ dir[0], dir[1] };
            break;
        case FREE_D6:
            // mark position, increase counter and move on
            old_pos = guard_state.pos;
            tile_map_d6[g_pos.x][g_pos.y] = MARKED_D6;
            unique_pos_vec.push_back(guard_state);
            g_pos = g_pos + Point<int>{ dir[0], dir[1] };
            break;
        
        default:
            throw std::runtime_error("Hit unknown tile type: " + tile_map_d6[g_pos.x][g_pos.y]);
            break;
        }

    }

    return unique_pos_vec;
}

GuardStateD6 get_guard_state(const std::vector<std::vector<char>>& tile_map_d6) {
    for (int i=0; i<tile_map_d6.size(); ++i) {
        for (int j=0; j<tile_map_d6.at(i).size(); ++j) {
            // as far as I know the start pos is always indicated by a ^
            if (tile_map_d6.at(i).at(j) == '^') {
                return GuardStateD6{ {i,j}, TDirD6::Up };
            }
        }
    }

    throw std::runtime_error("Failed to find start position!");
}

std::vector<std::vector<char>> read_tile_map_from_file(const std::string& file_path)
{
    std::vector<std::vector<char>> char_vec_2d{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {
            std::replace(input_line.begin(), input_line.end(), '.', FREE_D6); // replace '.' with any char that has none of the 4 smallest bits set
            char_vec_2d.push_back(std::vector<char>(input_line.begin(), input_line.end()));
        }
        input_file.close();
    }
    return char_vec_2d;
}