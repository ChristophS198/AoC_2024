#include <string>
#include <memory>
#include <cmath>
#include <unordered_map>

#include "../utility.h"

namespace Day11
{
    struct Pebble; 
    using TVal = std::uint64_t;
    using TPebblePtr = Pebble*;

    struct Pebble {
        TVal val{};
        TPebblePtr nxt{ nullptr };
    };
    

    TPebblePtr get_data(const std::string& file_path);
    uint64_t blink(TPebblePtr head);
    uint64_t blink_unordered(const std::vector<TVal>& values, int num_blinks);
    std::pair<TVal,TVal> split_value(TVal v, int digit_count);
    int get_digit_count(TVal val);

    uint64_t sol_11_1(const std::string& file_path)
    {
        auto values = read_numbers_from_file<TVal>(file_path);
        return blink_unordered(values, 25);
    }


    uint64_t sol_11_2(const std::string &file_path)
    {
        auto values = read_numbers_from_file<TVal>(file_path);
        return blink_unordered(values, 75);
    }

    /*
    Since order of pebbles does not matter, this approach simply uses a map to track how often each number occurs 
    before/after each blink
    */
    uint64_t blink_unordered(const std::vector<TVal>& values, int num_blinks) {
        std::unordered_map<TVal, uint64_t> pebble_map; // keeps track how often each pebble number occurs

        for (auto val : values) ++pebble_map[val];

        for (int i=0; i<num_blinks; ++i)
        {
            std::unordered_map<TVal, uint64_t> new_pebble_map;
            for (auto [val, count] : pebble_map)
            {
                if (val == 0){
                    new_pebble_map[1] += count;
                }
                else {
                    auto digit_ct = get_digit_count(val);
                    if (digit_ct%2 == 0) {
                        auto [l_val, r_val] = split_value(val, digit_ct);
                        new_pebble_map[l_val] += count;
                        new_pebble_map[r_val] += count;
                    }
                    else {
                        new_pebble_map[val * 2024] += count;
                    }
                }
            }

            pebble_map = new_pebble_map;
        }

        uint64_t pebble_count{ };
        for (auto [val, count] : pebble_map) pebble_count += count;

        return pebble_count;
    }


// ----------------------------------------- legacy approach of using a linked list to track the pebbles ------------------------------

    uint64_t blink(TPebblePtr head)
    {
        auto it_ptr = head;
        uint64_t pebble_count{ };

        while(it_ptr) {
            if (it_ptr->val == 0){
                it_ptr->val = 1;
            }
            else {
                auto digit_ct = get_digit_count(it_ptr->val);
                if (digit_ct%2 == 0) {
                    auto [l_val, r_val] = split_value(it_ptr->val, digit_ct);

                    // update and insert pebbles
                    it_ptr->val = l_val;
                    auto new_pebble = new Pebble(r_val, it_ptr->nxt);
                    it_ptr->nxt = new_pebble;
                    it_ptr = it_ptr->nxt;
                    ++pebble_count;
                }
                else {
                    it_ptr->val *= 2024;
                }
            }
            
            ++pebble_count;
            it_ptr = it_ptr->nxt;
        }

        return pebble_count;
    }  

    std::pair<TVal,TVal> split_value(TVal val, int digit_count)
    {
        TVal div = std::pow(10, (TVal) digit_count/2);
        TVal l_val = val / div;
        TVal r_val = val % div;

        return { l_val,r_val };
    }  

    int get_digit_count(TVal val) {
        int len{ };
        while ( val ) {
            len++;
            val /= 10;
        }

        return len;
    }

    TPebblePtr get_data(const std::string& file_path)
    {
        auto nums = read_numbers_from_file<TVal>(file_path);
        auto head = new Pebble(nums[0], nullptr);
        auto cur = head;

        for (int i=1; i<nums.size(); ++i) {
            cur->nxt = new Pebble(nums[i], nullptr);
            cur = cur->nxt;
        }

        return head;
    }

} // namespace Day11
