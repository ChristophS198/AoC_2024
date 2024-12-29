#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <limits>

#include "../utility.h"

namespace Day16
{
    using TPos = Point<int>;
    using TMaze = std::vector<std::string>;

    constexpr char FREE{ '.' };
    constexpr char WALL{ '#' };
    constexpr uint64_t ROT_SCORE{ 1'000 };
    constexpr uint64_t MOVE_SCORE{ 1 };

    enum Rot {
        East = 0,
        South,
        West,
        North
    };

    const std::unordered_map<Rot,TPos> dirs{ {Rot::East,{0,1}},{Rot::South,{1,0}},{Rot::West,{0,-1}},{Rot::North, {-1,0}} };

    struct State {
        TPos pos{};
        Rot rot{};
        uint64_t score{};
        std::vector<TPos> path{};
        bool operator<(const State& s) const { return this->score < s.score; }
    };

    std::pair<State, State> get_start_end(const TMaze& maze);
    std::vector<State> get_min_paths(TMaze maze, State start);


    uint64_t sol_16_1(const std::string &file_path)
    {
        auto maze = read_string_vec_from_file(file_path);
        auto [start,end] = get_start_end(maze);

        return get_min_paths(maze, start)[0].score;
    }


    uint64_t sol_16_2(const std::string &file_path)
    {
        auto maze = read_string_vec_from_file(file_path);
        auto [start,end] = get_start_end(maze);
        auto min_paths = get_min_paths(maze, start);

        std::unordered_set<TPos, TPos::HashFunction> unique_tiles_set;
        for (const auto& state : min_paths) {
            unique_tiles_set.insert(state.path.begin(), state.path.end());
        }

        return unique_tiles_set.size() + 1;  // +1 due to end tile
    }

    std::vector<State> get_min_paths(TMaze maze, State start) 
    {
        auto cmp = [](State s1, State s2) { return s1.score > s2.score; };

        std::priority_queue<State,std::vector<State>, decltype(cmp)> queue(cmp);
        queue.push(start);
        auto num_rows = maze.size();
        auto num_cols = maze[0].length();
        std::unordered_map<TPos,std::array<uint64_t,4>, TPos::HashFunction> score_map;
        std::vector<State> min_path_states;
        bool min_len_found{ false };

        while (!queue.empty()) {
            auto cur_state = queue.top();
            queue.pop();

            // handle invalid moves
            if (cur_state.pos.x < 0 || cur_state.pos.y < 0 || cur_state.pos.x >= num_rows || cur_state.pos.y >= num_cols) continue;
            if (maze[cur_state.pos.x][cur_state.pos.y] == WALL) continue;

            // end condition
            if (maze[cur_state.pos.x][cur_state.pos.y] == 'E') {
                if (min_len_found) {
                    if (cur_state.score > min_path_states[0].score) return min_path_states;
                    else min_path_states.push_back(cur_state);
                }
                else {
                    min_path_states.push_back(cur_state);
                    min_len_found = true;
                }
            }

            // check if this state has already been reached -> if so, the current score must be larger
            if (score_map.contains(cur_state.pos)) {
                if (score_map[cur_state.pos][cur_state.rot] != std::numeric_limits<uint64_t>::max()) {
                    if (cur_state.score < score_map[cur_state.pos][cur_state.rot]) {
                        std::cout << "Should not happen" << std::endl;
                    }
                    if (cur_state.score > score_map[cur_state.pos][cur_state.rot]) {
                        continue;
                    }
                    if (cur_state.score > score_map[cur_state.pos][cur_state.rot]) {
                        // through this we allow multiple paths that reach the same score, which is relevant for pt2
                        ;
                    }
                }
                score_map[cur_state.pos][cur_state.rot] = cur_state.score;
            }
            else {
                score_map[cur_state.pos].fill(std::numeric_limits<uint64_t>::max());
                score_map[cur_state.pos][cur_state.rot] = cur_state.score;
            }

            for (auto [rot,dir] : dirs) {
                auto nxt_state{ cur_state };

                // we do not turn around 180° and move back
                if (abs(rot-cur_state.rot) == 2) continue;

                // if we change direction for the current move -> add ROT_SCORE and update to new rotation
                if (rot != cur_state.rot) {
                    nxt_state.rot = rot;
                    nxt_state.score += ROT_SCORE;
                }
                else {
                    nxt_state.score += MOVE_SCORE;
                    nxt_state.pos = nxt_state.pos + dir;
                    nxt_state.path.push_back(cur_state.pos); // add predecessor to this path
                }

                queue.push(nxt_state);
            }
        }

        return {};
    }

    std::pair<State, State> get_start_end(const TMaze& maze)
    {
        State start{};
        State end{ };

        for (int i=0; i<maze.size(); ++i) {
            for (int j=0; j<maze[i].length(); ++j) {
                if (maze.at(i).at(j) == 'S') start.pos = { i,j };
                if (maze.at(i).at(j) == 'E') end.pos = { i,j };
            }
        }
        start.rot = Rot::East;

        return { start,end };
    }


};