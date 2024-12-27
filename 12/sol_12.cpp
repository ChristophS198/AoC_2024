#include <string>
#include <unordered_set>
#include <unordered_map>

#include "../utility.h"


namespace Day12
{

    using TGarden = std::vector<std::string>;
    using TPos = Point<int>;
    using TRegion = std::unordered_set<TPos, TPos::HashFunction>;
    using TPerimeterMap = std::unordered_map<TPos, int64_t, TPos::HashFunction>;

    constexpr char PROCESSED{ '#' };

    uint64_t get_price(TGarden& garden, bool pt1=true);
    std::pair<uint64_t,uint64_t> get_region_area_perimeter(TGarden& garden, TRegion& region,TPerimeterMap& perimeter_set, TPos pos, char plant);
    int get_corner_count(const TRegion& region, TPos p);
    uint64_t calc_perimeter(TRegion region);

    uint64_t sol_12_1(const std::string &file_path)
    {
        auto garden = read_string_vec_from_file(file_path);

        return get_price(garden);
    }

    uint64_t sol_12_2(const std::string &file_path)
    {
        auto garden = read_string_vec_from_file(file_path);

        return get_price(garden, false);
    }

    int get_corner_count(const TRegion& region, TPos p) {
        /*
            Outer Corners:           Inner Corners:
            (X = corner, # = plant)  (X = corner, # = plant)

            1)  X.     2)  .X        5)  ##     6)  ##
                .#         #.            #X         X#

            3)  .#     4)  #.        7)  ##     8)  ##
                X.         .X            X#         #X
        */
        static const std::vector<TPos> dirs{ {0,1}, {1,0}, {0,-1}, {-1,0} };
        int num_corners{ };

        // outer corner
        for (auto [d1,d2] : std::vector<std::pair<int,int>>{{2,3}, {0,3}, {1,2}, {0,1}}) {
            if (!region.contains(p+dirs[d1]) && !region.contains(p+dirs[d2])) num_corners += 1;
        }

        // inner corner
        for (auto [d1,d2] : std::vector<std::pair<int,int>>{{2,3}, {0,3}, {1,2}, {0,1}}) {
            if (region.contains(p+dirs[d1]) && region.contains(p+dirs[d2]) && 
                !region.contains(p+dirs[d1]+dirs[d2])) num_corners += 1;
        }

        return num_corners;
    }

    uint64_t calc_perimeter(TRegion region)
    {
        static const std::vector<TPos> dirs{ {0,1}, {1,0} };
        uint64_t perimeter{ };

        for (auto p : region) {
            perimeter += get_corner_count(region,p); 
        }

        return  perimeter;
    }


    std::pair<uint64_t,uint64_t> get_region_area_perimeter(TGarden& garden, TRegion& region,TPerimeterMap& perimeter_map, TPos pos, char plant)
    {
        static const std::vector<TPos> dirs{ {-1,0}, {0,1}, {1,0}, {0,-1} };
        if (pos.x < 0 || pos.x>=garden.size() || pos.y<0 || pos.y>=garden[0].size()) {
            ++perimeter_map[pos];
            return { 0,1 };
        }
        if (region.contains(pos)) return {0,0};
        if (garden[pos.x][pos.y] != plant) {
            ++perimeter_map[pos];
            return { 0,1 };
        }

        else
        {
            uint64_t area{ 1 };
            uint64_t perimeter{ };
            garden[pos.x][pos.y] = PROCESSED;
            region.insert(pos);
            for (auto dir : dirs) 
            {
                auto [a,p] = get_region_area_perimeter(garden, region, perimeter_map, pos+dir, plant);
                area += a;
                perimeter += p;
            }
            return { area,perimeter };
        }
    }


    uint64_t get_price(TGarden& garden, bool pt1)
    {
        uint64_t price{ };

        for (int i = 0; i < garden.size(); i++)
        {
            for (int j = 0; j < garden[i].length(); j++)
            {
                if (garden[i][j] != PROCESSED)
                {
                    TRegion region;
                    TPerimeterMap perimeter_set;
                    auto [area,p] = get_region_area_perimeter(garden, region, perimeter_set, { i,j }, garden[i][j]);
                    if (pt1 == false) p = calc_perimeter(region);
                    price += (area * p);
                }
            }
        }

        return price;
    }


} // namespace Day12