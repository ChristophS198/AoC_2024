#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "../utility.h"

namespace Day18
{
    using TCoord = Point<int>;

    constexpr int GRID_SIZE{ 71 };
    constexpr int BYTE_NUM{ 1024 };
 

    std::vector<TCoord> get_data_18(const std::string& file_path);
    int min_steps(const std::unordered_set<TCoord, TCoord::HashFunction>& byte_pos_set);

    std::uint32_t sol_18_1(const std::string &file_path)
    {
        auto data = get_data_18(file_path);

        std::unordered_set<TCoord, TCoord::HashFunction> byte_pos_set{data.begin(), data.begin()+BYTE_NUM};

        return min_steps(byte_pos_set);
    }


    std::string sol_18_2(const std::string &file_path)
    {
        auto data = get_data_18(file_path);

        std::unordered_set<TCoord, TCoord::HashFunction> byte_pos_set{data.begin(), data.begin()+BYTE_NUM};

        std::string res = "Not found";

        for (int i=BYTE_NUM; i<data.size(); ++i) {
            byte_pos_set.insert(data[i]);
            if (min_steps(byte_pos_set) == -1) {
                res = std::to_string(data[i].x) + "," + std::to_string(data[i].y);
                break;
            }
        }

        return res;
    }

    std::vector<TCoord> get_data_18(const std::string& file_path) 
    {
        std::vector<TCoord> number_vec{};
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                auto num_vec = parse_string_to_number_vec<int>(input_line);
                number_vec.push_back(TCoord(num_vec[0], num_vec[1]));
            }
            input_file.close();
        }
        return number_vec;
    }

    int min_steps(const std::unordered_set<TCoord, TCoord::HashFunction>& byte_pos_set)
    {
        struct Node {
            TCoord coord;
            int dist;
        };

        std::vector<TCoord> dirs{ {-1,0}, {0,1}, {1,0}, {0,-1} };

        std::unordered_map<TCoord, int, TCoord::HashFunction> shortest_path_map;
        TCoord start{ 0,0 };
        TCoord target{ GRID_SIZE-1, GRID_SIZE-1 };
        std::queue<Node> new_reachable{ };
        new_reachable.push(Node{ start, 0 });

        while (!new_reachable.empty()) {
            auto current = new_reachable.front();
            new_reachable.pop();


            // add current node to shortest path map
            if (shortest_path_map.find(current.coord) == shortest_path_map.end()) {
                shortest_path_map[current.coord] = current.dist;
            }
            else {
                // this node is already reachable via a shorter path -> has already been processed
                continue;
            }

            // add neighbors no newly reachable nodes
            for (auto dir : dirs) {
                auto new_coord = current.coord + dir;
                // check if new_coord is valid and we do not know its shortest path
                if (new_coord.x >= 0 && new_coord.x < GRID_SIZE && new_coord.y >= 0 && new_coord.y < GRID_SIZE &&
                    shortest_path_map.find(new_coord) == shortest_path_map.end()) {
                    if (new_coord == target) {
                        return current.dist+1;
                    }
                    // add node only if it is not corrupted
                    if (byte_pos_set.find(new_coord) == byte_pos_set.end()) {
                        new_reachable.push( Node{ new_coord, current.dist+1 });
                    }
                }
            }

        }
        
        return -1;
    }

}
