#include <string>
#include <unordered_map>
#include <algorithm>

#include "../utility.h"

namespace Day22
{
    using TNum = std::uint64_t;
    using TChange = int;
    using TChangeMap = std::unordered_map<std::string, int>;

    constexpr TNum EVOLVE_NUM{ 2000 };

    TNum evolve_num(TNum num);
    TNum mix_nums(TNum n1, TNum n2);
    TNum prune_num(TNum n1, TNum prune_factor=16777216ull);
    TChangeMap& merge_price_change_sequence_maps(TChangeMap& change_seq_map_1, TChangeMap& change_seq_map_2);
    TChangeMap get_price_change_sequence_map(const std::vector<TChange>& p_vec);


    TNum sol_22_1(const std::string &file_path)
    {
        auto secret_nums = read_numbers_from_file<TNum>(file_path);

        TNum summed_nums{ 0 };

        for (auto num : secret_nums) {
            for (int i=0; i<EVOLVE_NUM; ++i) {
                num = evolve_num(num);
            }
            summed_nums += num;
        }
        return summed_nums;
    }


    // too low: 1733
    TNum sol_22_2(const std::string &file_path)
    {
        auto secret_nums = read_numbers_from_file<TNum>(file_path);

        TNum summed_nums{ 0 };
        TChangeMap sum_change_map{};

        std::vector<TChange> p_vec(EVOLVE_NUM+1, 0);
        for (auto num : secret_nums) {
            for (int i=0; i<EVOLVE_NUM; ++i) {
                p_vec[i] = num % 10;
                num = evolve_num(num);
            }
            p_vec[EVOLVE_NUM] = num % 10;
            TChangeMap tmp_change_map = get_price_change_sequence_map(p_vec);
            merge_price_change_sequence_maps(sum_change_map, tmp_change_map);
            summed_nums += num;
        }

        auto max_elem = std::max_element(std::begin(sum_change_map), std::end(sum_change_map),
            [] (const decltype(sum_change_map)::value_type & p1, const decltype(sum_change_map)::value_type & p2) {
                return p1.second < p2.second;
            }
        );

        return max_elem->second;
    }

    std::string get_id(const std::vector<TChange>& changes) {
        std::string res{};

        for (auto num : changes) {
            res += std::to_string(num) + ",";
        }
        return res.substr(0,res.length()-1);
    }

    TChangeMap& merge_price_change_sequence_maps(TChangeMap& change_seq_map_1, TChangeMap& change_seq_map_2) {
        // merge elements only existing in map 2 into map 1
        change_seq_map_1.merge(change_seq_map_2);

        // add remaining elements existing in both maps
        for (auto [id, price] : change_seq_map_2) {
            change_seq_map_1[id] += price;
        }

        return change_seq_map_1;
    }

    TChangeMap get_price_change_sequence_map(const std::vector<TChange>& p_vec) {
        TChangeMap change_seq_map;
        std::vector<TChange> changes{ p_vec[1]-p_vec[0],p_vec[2]-p_vec[1],p_vec[3]-p_vec[2], p_vec[4]-p_vec[3] };
        std::string id = get_id(changes);
        change_seq_map.insert({ id,p_vec[4] });

        // use insert for map, so we only insert the first occurence of a unique change string
        for (int i=5; i<p_vec.size(); ++i) {
            // convert to string
            changes[0] = changes[1];
            changes[1] = changes[2];
            changes[2] = changes[3];
            changes[3] = p_vec[i] - p_vec[i-1];
            id = get_id(changes);
            change_seq_map.insert({ id,p_vec[i] });
        }

        return change_seq_map;
    }

    TNum evolve_num(TNum num)
    {
        TNum new_num{ num };

        // multiply by 64
        new_num <<= 6;
        num = mix_nums(num, new_num);
        num = prune_num(num);

        // divide by 32
        new_num = num;
        new_num >>= 5;
        num = mix_nums(num, new_num);
        num = prune_num(num);

        // multiply with 2048
        new_num = num;
        new_num <<= 11;
        num = mix_nums(num, new_num);
        num = prune_num(num);

        return num;
    }

    TNum mix_nums(TNum n1, TNum n2)
    {
        return n1 ^ n2;
    }

    TNum prune_num(TNum n1, TNum prune_factor)
    {
        return n1 % prune_factor;
    }

}