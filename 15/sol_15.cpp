#include <string>
#include <ostream>
#include <unordered_map>

#include "../utility.h"

namespace {

    enum TDir {
        Up, 
        Right, 
        Down, 
        Left
    };

    constexpr char FREE{ '.' };
    constexpr char WALL{ '#' };
    constexpr char BOX{ 'O' };
    constexpr char L_BOX{ '[' };
    constexpr char R_BOX{ ']' };
    constexpr std::uint32_t ROW_FACTOR{ 100ul };
    // const std::vector<std::vector<int>> DIR_VEC{ {-1,0}, {0,1}, {1,0}, {0,-1} }; // maps each of the TDirD6 enum values to a direction
    const std::unordered_map<char, Point<int>> DIR_MAP{ {'^',{-1,0}}, {'>',{0,1}}, {'v',{1,0}}, {'<',{0,-1}} }; // maps each of the direction chars to a direction

    using RobotState = Point<int>;
    using WareHouse = std::vector<std::string>;
    using BoxPos = Point<int>; 
};

std::pair<WareHouse, std::string> get_data(const std::string& file_path);
std::pair<WareHouse, std::string> get_data_2(const std::string& file_path);
RobotState get_robot_state(const WareHouse& warehouse);
uint32_t calc_gps_sum(const WareHouse &warehouse);
uint32_t calc_gps_sum_2(const WareHouse &warehouse);
RobotState do_move(WareHouse &warehouse, RobotState r_state, char move);
RobotState do_move_2(WareHouse &warehouse, RobotState r_state, char move);
bool move_hoz(WareHouse& warehouse, Point<int> p, Point<int> dir);
bool move_vert(WareHouse& warehouse, BoxPos b, Point<int> dir);
bool move_box(WareHouse& warehouse, Point<int> b, Point<int> dir);

std::ostream& print_warehouse(std::ostream &out, const WareHouse &warehouse, Point<int> pos);

uint32_t sol_15_1(const std::string &file_path)
{
    auto [warehouse, movements] = get_data(file_path);
    RobotState r_state = get_robot_state(warehouse);
    // marks start state as free
    warehouse[r_state.x][r_state.y] = FREE;

    for (auto move : movements) {
        r_state = do_move(warehouse, r_state, move);
    }

    return calc_gps_sum(warehouse);
}


uint32_t sol_15_2(const std::string &file_path)
{
    auto [warehouse, movements] = get_data_2(file_path);
    RobotState r_state = get_robot_state(warehouse);
    // marks start state as free
    warehouse[r_state.x][r_state.y] = FREE;

    for (auto move : movements) {
        r_state = do_move_2(warehouse, r_state, move);
    }

    // print_warehouse(std::cout, warehouse, r_state);
    return calc_gps_sum_2(warehouse);
}

bool move_box(WareHouse& warehouse, Point<int> b, Point<int> dir) {
    if (dir.x == 0) {   
        if (move_hoz(warehouse, b, dir)) {
            warehouse[b.x][b.y] = FREE; 
            return true;
        }
    }
    else {
        if (warehouse[b.x][b.y] == R_BOX) {
            b.y -= 1; // box is identified by its left coord
        }
        WareHouse copy{ warehouse };
        if (move_vert(warehouse, b, dir)) {
            warehouse[b.x][b.y] = FREE; 
            warehouse[b.x][b.y+1] = FREE; // also closing bracket is freed
            return true;
        }
        else warehouse = copy;
    }
    return false;
}

bool move_vert(WareHouse& warehouse, BoxPos b, Point<int> dir) {
    auto nxt_l = b + dir;
    auto nxt_r = BoxPos{ nxt_l.x, nxt_l.y+1 };

    if (warehouse[nxt_l.x][nxt_l.y] == FREE) {
        if (warehouse[nxt_r.x][nxt_r.y] == FREE || 
            (warehouse[nxt_r.x][nxt_r.y] == L_BOX && move_vert(warehouse, nxt_r, dir))) {
            if (warehouse[nxt_r.x][nxt_r.y] == L_BOX) {
                warehouse[nxt_r.x][nxt_r.y+1] = FREE;
            }
            warehouse[nxt_l.x][nxt_l.y] = L_BOX;
            warehouse[nxt_r.x][nxt_r.y] = R_BOX;
            return true;
        }
        else return false;
    }
    else if (warehouse[nxt_l.x][nxt_l.y] == R_BOX && move_vert(warehouse, BoxPos(nxt_l.x, nxt_l.y-1), dir)) { 
        if (warehouse[nxt_r.x][nxt_r.y] ==  FREE || 
            (warehouse[nxt_r.x][nxt_r.y] == L_BOX && move_vert(warehouse, nxt_r, dir))) {
            if (warehouse[nxt_r.x][nxt_r.y] == L_BOX) {
                warehouse[nxt_r.x][nxt_r.y+1] = FREE;
            }
            warehouse[nxt_l.x][nxt_l.y-1] = FREE;
            warehouse[nxt_l.x][nxt_l.y] = L_BOX;
            warehouse[nxt_r.x][nxt_r.y] = R_BOX;
            return true;            
        }
        else return false;
    }
    else if (warehouse[nxt_l.x][nxt_l.y] == L_BOX && move_vert(warehouse, nxt_l, dir)) {
        warehouse[nxt_l.x][nxt_l.y] = L_BOX;
        warehouse[nxt_r.x][nxt_r.y] = R_BOX;
        return true;            
    }

    return false;
}

bool move_hoz(WareHouse& warehouse, Point<int> p, Point<int> dir) {
    auto nxt_pos = p + dir;

    if (warehouse[nxt_pos.x][nxt_pos.y] == FREE) {
        warehouse[nxt_pos.x][nxt_pos.y] = warehouse[p.x][p.y];
        return true;
    }
    
    if (warehouse[nxt_pos.x][nxt_pos.y] == L_BOX || warehouse[nxt_pos.x][nxt_pos.y] == R_BOX) {
        if (move_hoz(warehouse, nxt_pos, dir)) {
            warehouse[nxt_pos.x][nxt_pos.y] = warehouse[p.x][p.y];
            return true;
        }
        else return false;
    }
    else return false;
}

RobotState do_move_2(WareHouse &warehouse, RobotState r_state, char move)
{
    auto dir = DIR_MAP.at(move);
    auto nxt_pos{ r_state + dir };
    auto tmp{ r_state };

    switch (warehouse[nxt_pos.x][nxt_pos.y])
    {
    case FREE:
        return nxt_pos;
        break;
    case R_BOX:
        [[fallthrough]]
    case L_BOX:
        // check in current direction which tile comes after all boxes
        if (move_box(warehouse, nxt_pos, dir)) {
            return nxt_pos;
        }
        else return r_state;
        break;
    case WALL:
        return r_state;
        break;
    
    default:
        std::cout << warehouse[nxt_pos.x][nxt_pos.y] << std::endl;
        throw std::runtime_error("Invalid tile in WareHouse found: " + warehouse[nxt_pos.x][nxt_pos.y]);
        break;
    }

}

RobotState do_move(WareHouse &warehouse, RobotState r_state, char move)
{
    auto dir = DIR_MAP.at(move);
    auto nxt_pos{ r_state + dir };
    auto tmp{ r_state };

    switch (warehouse[nxt_pos.x][nxt_pos.y])
    {
    case FREE:
        return nxt_pos;
        break;
    case BOX:
        // check in current direction which tile comes after all boxes
        tmp = nxt_pos + dir;
        while (warehouse[tmp.x][tmp.y] == BOX) tmp = tmp + dir;
        if (warehouse[tmp.x][tmp.y] == WALL) return r_state;
        else {
            // FREE: boxes are moved
            warehouse[tmp.x][tmp.y] = BOX;
            warehouse[nxt_pos.x][nxt_pos.y] = FREE;
            return nxt_pos;
        }
        break;
    case WALL:
        return r_state;
        break;
    
    default:
        std::cout << warehouse[nxt_pos.x][nxt_pos.y] << std::endl;
        throw std::runtime_error("Invalid tile in WareHouse found: " + warehouse[nxt_pos.x][nxt_pos.y]);
        break;
    }

}

uint32_t calc_gps_sum(const WareHouse &warehouse)
{
    std::uint32_t gps_sum{ 0ul };
    for (size_t row=0; row<warehouse.size(); ++row)
    {
        for (size_t col=0; col<warehouse[row].length(); ++col)
        {
            if (BOX == warehouse[row][col]) gps_sum += ROW_FACTOR*row + col;
        }
    }

    return gps_sum;
}
uint32_t calc_gps_sum_2(const WareHouse &warehouse)
{
    std::uint32_t gps_sum{ 0ul };
    for (size_t row=0; row<warehouse.size(); ++row)
    {
        for (size_t col=0; col<warehouse[row].length(); ++col)
        {
            if (L_BOX == warehouse[row][col]) gps_sum += ROW_FACTOR*row + col;
        }
    }

    return gps_sum;
}

RobotState get_robot_state(const WareHouse& warehouse) {
    for (int i=0; i<warehouse.size(); ++i) {
        for (int j=0; j<warehouse.at(i).size(); ++j) {
            if (warehouse.at(i).at(j) == '@') {
                return RobotState{ i,j };
            }
        }
    }

    throw std::runtime_error("Failed to find start position!");
}

std::ostream& print_warehouse(std::ostream &out, const WareHouse &warehouse, Point<int> pos)
{
    out << "\n";
    WareHouse cp{ warehouse };
    cp[pos.x][pos.y] = '@';
    for (const auto & row : cp)
    {
        out << row << "\n";
    }
    out << "\n";
}

std::pair<WareHouse, std::string> get_data_2(const std::string& file_path)
{
    WareHouse warehouse_layout{};
    std::string movements{ };
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        // read warehouse layout
        while(getline(input_file, input_line))
        {
            if (input_line == "") break;
            std::string tmp{ };
            for (auto c : input_line) {
                switch (c)
                {
                case BOX:
                    tmp.push_back(L_BOX);
                    tmp.push_back(R_BOX);
                    break;
                case '@':
                    tmp.push_back(c);
                    tmp.push_back(FREE);
                    break;
                
                default:
                    tmp.push_back(c);
                    tmp.push_back(c);
                }
            }
            warehouse_layout.push_back(tmp);
        }

        // read robot movements
        while(getline(input_file, input_line))
        {
            movements.append(input_line);
        }
        input_file.close();
    }

    return { warehouse_layout, movements };
}

std::pair<WareHouse, std::string> get_data(const std::string& file_path)
{
    WareHouse warehouse_layout{};
    std::string movements{ };
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        // read warehouse layout
        while(getline(input_file, input_line))
        {
            if (input_line == "") break;
            warehouse_layout.push_back(input_line);
        }

        // read robot movements
        while(getline(input_file, input_line))
        {
            movements.append(input_line);
        }
        input_file.close();
    }

    return { warehouse_layout, movements };
}