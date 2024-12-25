#include <string>
#include <array>
#include <map>
#include <numeric>

#include "../utility.h"

namespace Day25
{
    using TKey = std::array<uint8_t, 5>;
    using TLock = std::array<uint8_t, 5>;

    constexpr TKey::value_type MAX_HEIGHT{ 5 };

    std::pair<std::vector<TKey>, std::vector<TLock>> get_data(const std::string& file_path); 
    uint64_t get_num_non_overlaps(const std::vector<TKey>& key_vec, const std::vector<TLock> lock_vec);


    uint64_t sol_25_1(const std::string &file_path)
    {
        auto [key_vec, lock_vec] = get_data(file_path);

        return get_num_non_overlaps(key_vec, lock_vec);
    }


    int sol_25_2(const std::string &file_path)
    {

        return 0;
    }

    uint64_t get_num_non_overlaps(const std::vector<TKey>& key_vec, const std::vector<TLock> lock_vec)
    {
        uint64_t num_non_overlaps{ };

        for (const auto& key : key_vec) {
            for (const auto& lock : lock_vec) {
                bool do_fit{ true };
                for (int i=0; i<key.size(); ++i) {
                    if (key[i] + lock[i] > MAX_HEIGHT) {
                        do_fit = false;
                    }
                }
                if (do_fit) ++num_non_overlaps;
            }
        }

        return num_non_overlaps;
    }


    std::pair<std::vector<TKey>, std::vector<TLock>> get_data(const std::string& file_path)
    {
        std::vector<TKey> key_vec;
        std::vector<TLock> lock_vec;

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line{};
            bool is_lock{};
            while(getline(input_file, input_line))
            {
                // read next key or lock
                TKey key_lock{};
                if (input_line == "#####") is_lock = true;
                else is_lock = false;

                while (input_line != "" && getline(input_file, input_line)) 
                {
                    for (int i=0; i<input_line.length(); ++i) {
                        if (input_line[i] == '#') ++key_lock[i];
                    }
                }

                // insert new key or lock in respective vector
                if (is_lock) lock_vec.push_back(key_lock);
                else {
                    for (int i=0; i<key_lock.size(); ++i) {
                        --key_lock[i];
                    }
                    key_vec.push_back(key_lock);
                }
            }
            input_file.close();
        }

        return { key_vec, lock_vec };
    }


} // namespace Day25
