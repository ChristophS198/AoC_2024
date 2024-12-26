#include <string>
#include <unordered_set>

#include "../utility.h"

namespace Day10
{
    
    using THeight = char;
    using THeightMap = std::vector<std::string>;
    using TPos = Point<int>;

    std::unordered_set<TPos, TPos::HashFunction> get_endings(const THeightMap& height_map, TPos pos);
    int get_trailhead_rating(const THeightMap& height_map, TPos pos);

    size_t sol_10_1(const std::string &file_path)
    {
        auto height_map = read_string_vec_from_file(file_path);
        int sum_score{ };
        for (int row=0; row < height_map.size(); row++)
        {
            for (int col=0; col<height_map[row].size(); ++col) 
            {
                if (height_map[row][col] == '0')
                {
                    sum_score += get_endings(height_map, { row,col }).size();
                }
            }
        }
        
        return sum_score;
    }


    int sol_10_2(const std::string &file_path)
    {
        auto height_map = read_string_vec_from_file(file_path);
        int sum_score{ };
        for (int row=0; row < height_map.size(); row++)
        {
            for (int col=0; col<height_map[row].size(); ++col) 
            {
                if (height_map[row][col] == '0')
                {
                    sum_score += get_trailhead_rating(height_map, { row,col });
                }
            }
        }
        
        return sum_score;
    }

    int get_trailhead_rating(const THeightMap& height_map, TPos pos)
    {
        static const std::vector<TPos> dirs{ {1,0},{0,1},{-1,0},{0,-1} };
        if (height_map[pos.x][pos.y] == '9') return 1;

        int rating{ };

        for (const auto& dir : dirs) {
            auto neigh_pos = pos + dir;
            if (neigh_pos.x < 0 || neigh_pos.x >= height_map.size() || neigh_pos.y < 0 || neigh_pos.y >= height_map[0].size()) continue;
            auto height_diff = height_map[neigh_pos.x][neigh_pos.y] - height_map[pos.x][pos.y];
            if (height_diff == 1) {
                rating += get_trailhead_rating(height_map, pos + dir);
            }
        }

        return rating;
    }

    std::unordered_set<TPos, TPos::HashFunction> get_endings(const THeightMap& height_map, TPos pos)
    {
        static const std::vector<TPos> dirs{ {1,0},{0,1},{-1,0},{0,-1} };
        if (height_map[pos.x][pos.y] == '9') return { pos };

        std::unordered_set<TPos, TPos::HashFunction> endings{ };

        for (const auto& dir : dirs) {
            auto neigh_pos = pos + dir;
            if (neigh_pos.x < 0 || neigh_pos.x >= height_map.size() || neigh_pos.y < 0 || neigh_pos.y >= height_map[0].size()) continue;
            auto height_diff = height_map[neigh_pos.x][neigh_pos.y] - height_map[pos.x][pos.y];
            if (height_diff == 1) {
                auto tmp_endings = get_endings(height_map, pos + dir);
                endings.insert(tmp_endings.begin(), tmp_endings.end());
            }
        }

        return endings;
    }


} // namespace Day10
