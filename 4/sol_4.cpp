#include <string>
#include <array>
#include <unordered_set>
#include <unordered_map>

#include "../utility.h"

int pt_1(const std::vector<std::vector<char>>& word_search, const std::string& word);
int pt_2(const std::vector<std::vector<char>>& word_search, const std::string& word);
int get_num_words_at(const std::vector<std::vector<char>>& word_search, const std::string& word, int row, int col);
std::vector<Point<int>> get_a_locations_at(const std::vector<std::vector<char>>& word_search, const std::string& word, int row, int col);

int sol_4_1(const std::string &file_path)
{
    std::vector<std::vector<char>> word_search = read_2d_vec_from_file<char>(file_path);


    return pt_1(word_search, "XMAS");
;
}


int sol_4_2(const std::string &file_path)
{
    std::vector<std::vector<char>> word_search = read_2d_vec_from_file<char>(file_path);
    

    return pt_2(word_search, "MAS");
}

std::vector<Point<int>> get_a_locations_at(const std::vector<std::vector<char>>& word_search, const std::string& word, int row, int col) {
    std::vector<std::vector<int>> dirs{ {-1,-1}, {-1,1}, {1,-1}, {1,1} };
    int rows = word_search.size();
    int cols = word_search[0].size();
    std::vector<Point<int>> a_locations{ };

    for (auto dir : dirs) {
        bool is_match{ true };
        Point<int> a_location{ };

        for (int i=0; i<word.length(); ++i) {
            int n_row = row+dir[0]*i;
            int n_col = col+dir[1]*i;
            if (word.at(i) == 'A') {
                a_location = {n_row, n_col};
            }
            if (n_row<0 || n_col<0 || n_row>=rows || n_col >= cols) {
                is_match = false;
                break;
            }
            auto cur_char = word_search.at(n_row).at(n_col);
            if (cur_char != word.at(i)) {
                is_match = false;
                break;
            }
        }
        if (is_match) a_locations.push_back(a_location);
    }

    return a_locations;
}

int pt_2(const std::vector<std::vector<char>>& word_search, const std::string& word) {
    std::unordered_map<Point<int>, int, Point<int>::HashFunction> a_locations;
    int x_mas_count{ };
    int rows = word_search.size();
    int cols = word_search[0].size();

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            std::vector<Point<int>> new_a_locations = get_a_locations_at(word_search, word, i, j);
            for (const auto& loc : new_a_locations) {
                a_locations[loc] += 1;
            }
        }
    }

    for (const auto& loc : a_locations) 
    {
        if (loc.second > 1) {
            ++x_mas_count;
            std::cout << loc.first.x << "," << loc.first.y << std::endl;
        }
    }
    return x_mas_count;
}

int get_num_words_at(const std::vector<std::vector<char>>& word_search, const std::string& word, int row, int col) {
    std::vector<std::vector<int>> dirs{ {-1,-1}, {-1,1}, {1,-1}, {1,1}, {1,0}, {0,1}, {-1,0}, {0,-1}};
    int rows = word_search.size();
    int cols = word_search[0].size();
    int word_count{ };

    for (auto dir : dirs) {
        bool is_match{ true };
        for (int i=0; i<word.length(); ++i) {
            int n_row = row+dir[0]*i;
            int n_col = col+dir[1]*i;
            if (n_row<0 || n_col<0 || n_row>=rows || n_col >= cols) {
                is_match = false;
                break;
            }
            auto cur_char = word_search.at(n_row).at(n_col);
            if (cur_char != word.at(i)) {
                is_match = false;
                break;
            }
        }
        if (is_match) ++word_count;
    }

    return word_count;
}

int pt_1(const std::vector<std::vector<char>>& word_search, const std::string& word) {
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