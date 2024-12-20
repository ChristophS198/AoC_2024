#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>
#include <unordered_set>

#include "../utility.h"

namespace Day20
{
    using TDist = int;
    using TCoord = Point<int>;
    using TrackMap = std::unordered_map<TCoord, TDist, TCoord::HashFunction>; 

    constexpr char FREE{ '.' };
    constexpr char WALL{ '#' };
    constexpr int CHEAT_LIM_1{ 2 };
    constexpr int CHEAT_LIM_2{ 20 };
    constexpr int SAVED_DIST_LIM{ 100 };

    struct Data {
        std::vector<std::string> r_track;
        TCoord start;
        TCoord end;
    };
    
    Data get_data(const std::string& file_path);
    TrackMap generate_trackmap(const Data& data);
    std::vector<std::pair<TCoord, int>> get_cheat_end_pos_larger_x(const std::vector<std::string>& r_track, const TrackMap& track_map, TCoord start_pos, int l_limit, const std::vector<TCoord>& dirs);
    std::vector<TCoord> get_valid_end_pos(int max_dist);
    std::uint64_t get_cheats_larger_x(const std::vector<std::string>& r_track, const TrackMap& track_map, int l_limit, int max_dist);
    

    std::uint64_t sol_20_1(const std::string &file_path)
    {
        auto data_in = get_data(file_path);
        TrackMap track_map = generate_trackmap(data_in);

        return get_cheats_larger_x(data_in.r_track, track_map, SAVED_DIST_LIM, CHEAT_LIM_1);
    }

    size_t sol_20_2(const std::string &file_path)
    {
        auto data_in = get_data(file_path);
        TrackMap track_map = generate_trackmap(data_in);

        return get_cheats_larger_x(data_in.r_track, track_map, SAVED_DIST_LIM, CHEAT_LIM_2);
    }

    std::string p_to_str(const TCoord& p) {
        return std::to_string(p.x) + "," + std::to_string(p.y);
    }

    std::vector<TCoord> get_valid_end_pos(int max_dist) {
        std::vector<TCoord> dirs;
        for (int i=-max_dist; i<=max_dist; ++i) {
            for (int j=-max_dist; j<=max_dist; ++j) {
                if (abs(i)+abs(j) > max_dist || (i==0 && abs(j)<=1) || (j==0 && abs(i)<=1)) {
                    continue;
                }
                else {
                    dirs.push_back({ i,j });
                }
            }
        }
        return dirs;
    }

    // retrieves the end positions saving at least l_limit nanoseconds
    std::vector<std::pair<TCoord, int>> get_cheat_end_pos_larger_x(const std::vector<std::string>& r_track, const TrackMap& track_map, TCoord start_pos, int l_limit, const std::vector<TCoord>& dirs) {
        std::uint64_t cheat_ct{ 0 };
        std::vector<std::pair<TCoord, int>> cheats;
        
        int cur_dist = track_map.at(start_pos);

        // iterate through all possible end positions
        for (auto dir : dirs) {
            auto cheat_end = start_pos + dir;
            auto track_elem = track_map.find(cheat_end);
            if (track_elem != track_map.end()) {
                auto time_saving = track_elem->second - cur_dist - (abs(dir.x) + abs(dir.y));
                if (track_elem != track_map.end() && time_saving >= l_limit) {
                    ++cheat_ct;
                    cheats.push_back({ cheat_end,time_saving });
                }
            }
        }

        return cheats;
    }

    std::uint64_t get_cheats_larger_x(const std::vector<std::string>& r_track, const TrackMap& track_map, int l_limit, int max_dist) {
        std::uint64_t cheat_ct{ 0 };
        std::unordered_map<int, int> cheat_map; // debugging only

        struct CheatPatt {
            TCoord wall;
            std::vector<TCoord> free;
        };

        auto dirs = get_valid_end_pos(max_dist);

        for (const auto& pos : track_map) {
            auto cheat_start = pos.first;

            auto valid_cheat_pairs = get_cheat_end_pos_larger_x(r_track, track_map, cheat_start, l_limit, dirs);

            for (const auto& [cheat_end, saved_dist] : valid_cheat_pairs) {
                ++cheat_ct;
                cheat_map[saved_dist]++;
            }
        }

        // for (auto elem : cheat_map) {
        //     std::cout << elem.second << " cheats that save " << elem.first << " picoseconds" << std::endl;
        // }

        return cheat_ct;
    }

    TrackMap generate_trackmap(const Data& data)
    {
        const auto& r_track = data.r_track;
        TrackMap track{ };
        auto cur_pos = data.start;
        auto old_pos = cur_pos;
        static const std::vector<TCoord> dirs{ {1,0},{0,1},{-1,0},{0,-1} };
        TDist time_ct{ 0 };

        while (cur_pos != data.end) {
            track[cur_pos] = time_ct;

            for (const auto& dir : dirs) {
                auto new_pos = cur_pos + dir;
                if (new_pos.x >= 0 && new_pos.x < r_track.size() && new_pos.y >= 0 && new_pos.y < r_track.size()
                    && r_track[new_pos.x][new_pos.y] == FREE && new_pos != old_pos) {
                    old_pos = cur_pos;
                    cur_pos = new_pos;
                    break;
                }
            }

            ++time_ct;
        }

        track[cur_pos] = time_ct;
        return track;
    }

    Data get_data(const std::string& file_path)
    {
        Data data{ };
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            int i{ 0 };
            while(getline(input_file, input_line))
            {
                for (int j=0; j<input_line.length(); ++j) {
                    if (input_line[j] == 'S') {
                        data.start = TCoord{ i,j };
                        input_line[j] = FREE;
                    }
                    else if (input_line[j] == 'E') {
                        data.end = TCoord{ i,j };
                        input_line[j] = FREE;
                    }
                }
                data.r_track.push_back(input_line);
                ++i;
            }
            input_file.close();
        }
        return data;
    }

}
