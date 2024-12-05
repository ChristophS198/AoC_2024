#include <string>
#include <array>
#include <unordered_set>
#include <numeric>

#include "../utility.h"

int get_num_words(const std::vector<std::vector<char>>& word_search, const std::string& word);
int get_num_words_at(const std::vector<std::vector<char>>& word_search, const std::string& word, int row, int col);

int sol_4_1(const std::string &file_path)
{
    std::vector<std::vector<char>> word_search = read_2d_vec_from_file<char>(file_path);


    return 0;
}


int sol_4_2(const std::string &file_path)
{
    std::vector<std::vector<char>> word_search = read_2d_vec_from_file<char>(file_path);
    

    return 0;
}

int get_num_words_at(const std::vector<std::vector<char>>& word_search, const std::string& word, int row, int col) {
    std::vector<std::vector<int>> dirs{ {-1,-1}, {-1,1}, {1,-1}, {1,1}, {1,0}, {0,1}, {-1,0}, {0,-1}};
    int rows = word_search.size();
    int cols = word_search[0].size();
    int word_count{ };

    for (auto dir : dirs) {
        bool is_match{ true };
        for (int i=0; i<word.length(); ++i) {
            auto cur_char = word_search.at(dir[0]*i).at(dir[1]*i);
            if (cur_char != word[i]) {
                is_match = false;
                break;
            }
        }
        if (is_match) ++word_count;
    }

    return word_count;
}

int get_num_words(const std::vector<std::vector<char>>& word_search, const std::string& word) {
    int word_count{ };
    int rows = word_search.size();
    int cols = word_search[0].size();

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            word_count += get_num_words_at(word_search, word, i, j);
        }
    }

    return word_count;
}