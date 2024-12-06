#include <string>
#include <vector>
#include "../utility.h"


enum TDirD6 {
    Up, 
    Right, 
    Down, 
    Left
};

constexpr char OBSTRUCTION_D6{ '#' };
constexpr char MARKED_D6{ 'X' };
constexpr char FREE_D6{ '.' };
const std::vector<std::vector<int>> DIR_VEC{ {-1,0}, {0,1}, {1,0}, {0,-1} }; // maps each of the TDirD6 enum values to a direction

struct GuardStateD6 {
    Point<int> pos;
    TDirD6 heading;
};

GuardStateD6 get_guard_state(const std::vector<std::vector<char>>& tile_map_d6);
int get_num_unique_pos(GuardStateD6 guard_state, std::vector<std::vector<char>> tile_map_d6);

int sol_6_1(const std::string &file_path)
{
    std::vector<std::vector<char>> tile_map_d6 = read_2d_vec_from_file<char>(file_path);
    GuardStateD6 start_state = get_guard_state(tile_map_d6);

    return get_num_unique_pos(start_state, tile_map_d6);
}


int sol_6_2(const std::string &file_path)
{

    return 0;
}

int get_num_unique_pos(GuardStateD6 guard_state, std::vector<std::vector<char>> tile_map_d6) {
    int unique_pos_sum{ 1 }; // current position is already counted
    auto& g_pos = guard_state.pos;
    auto old_pos = guard_state.pos;
    tile_map_d6[g_pos.x][g_pos.y] = MARKED_D6;

    while (g_pos.x >= 0 && g_pos.y >= 0 && g_pos.x < tile_map_d6.size() && g_pos.y < tile_map_d6[0].size()) {
        // std::cout << g_pos.x << "," << g_pos.y << " - " << guard_state.heading << std::endl;
        auto dir = DIR_VEC[guard_state.heading];
        auto new_pos = g_pos + Point<int>{ dir[0], dir[1] };

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
            ++unique_pos_sum;
            g_pos = g_pos + Point<int>{ dir[0], dir[1] };
            break;
        
        default:
            throw std::runtime_error("Hit unknown tile type: " + tile_map_d6[g_pos.x][g_pos.y]);
            break;
        }

    }

    return unique_pos_sum;
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