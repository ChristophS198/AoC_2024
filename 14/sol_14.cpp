#include <string>
#include <unordered_set>
#include <fstream>

#include "../utility.h"

namespace Day14 {

    struct Robot;
    using TPos = Point<int>;
    using TVel = Point<int>;
    using RobotVec = std::vector<Robot>;

    constexpr uint32_t WIDTH{ 101 };
    // constexpr uint32_t WIDTH{ 11 };
    constexpr uint32_t HEIGHT{ 103 };
    // constexpr uint32_t HEIGHT{ 7 };

    struct Robot {
        TPos pos;
        TVel vel;
    };

    RobotVec get_data(const std::string& file_path);
    void move_robots(RobotVec& r_vec);
    uint64_t calc_safety_factor(const RobotVec& r_vec);
    std::ostream& print_map(std::ostream& out, const RobotVec& r_vec);
    std::string to_str(const RobotVec& r_vec);

    uint64_t sol_14_1(const std::string &file_path)
    {
        auto r_vec = get_data(file_path);

        for (int i=0; i<100; ++i)
        {
            move_robots(r_vec);
        }
        
        return calc_safety_factor(r_vec);
    }


    uint64_t sol_14_2(const std::string &file_path)
    {
        auto r_vec = get_data(file_path);
        // std::ofstream out("14.txt");

        for (int i=0; i<WIDTH*HEIGHT; ++i)
        {
            move_robots(r_vec);
            auto str = to_str(r_vec);
            // search for a larger block of robots in neighboring positions
            if (str.find("#######################") != std::string::npos) {
                return i+1;
            }
            // out << "Run: " << i << "\n";
            // print_map(out, r_vec);
            // out << "\n";
        }
        // out.close();
        std::cout << "Christmas Tree not found!" << std::endl;
        return 0;
    }

    std::string to_str(const RobotVec& r_vec)
    {
        std::string str(WIDTH*HEIGHT,'.');

        for (const auto& r : r_vec) 
        {
            str[r.pos.y*WIDTH + r.pos.x] = '#';
        }
        return str;
    }

    void move_robots(RobotVec& r_vec)
    {
        for (auto& robot : r_vec)
        {
            auto new_pos = robot.pos + robot.vel;
            if (new_pos.x < 0) new_pos.x = WIDTH + new_pos.x;
            else if (new_pos.x >= WIDTH) new_pos.x = new_pos.x % WIDTH;
            if (new_pos.y < 0) new_pos.y = HEIGHT + new_pos.y;
            else if (new_pos.y >= HEIGHT) new_pos.y = new_pos.y % HEIGHT;
            robot.pos = new_pos;
        }
    }

    uint64_t calc_safety_factor(const RobotVec& r_vec)
    {
        std::vector<uint64_t> quadrant_count(4,0);

        for (const auto& robot : r_vec) {
            if (robot.pos.x < WIDTH/2 && robot.pos.y < HEIGHT/2) {
                ++quadrant_count[0];
            }
            else if (robot.pos.x > WIDTH/2 && robot.pos.y < HEIGHT/2) {
                ++quadrant_count[1];
            }
            else if (robot.pos.x < WIDTH/2 && robot.pos.y > HEIGHT/2) {
                ++quadrant_count[2];
            }
            else if (robot.pos.x > WIDTH/2 && robot.pos.y > HEIGHT/2) {
                ++quadrant_count[3];
            }
        }

        return quadrant_count[0] * quadrant_count[1] * quadrant_count[2] * quadrant_count[3];
    }

    RobotVec get_data(const std::string& file_path)
    {
        RobotVec r_vec;
        uint32_t id{ 0 };

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                auto num_in_line = parse_string_to_number_vec<TPos::value_type>(input_line);
                r_vec.push_back(Robot{ {num_in_line[0],num_in_line[1]}, {num_in_line[2],num_in_line[3]} }) ;
                ++id;
            }
            input_file.close();   
        }

        return r_vec;
    }

    std::ostream& print_map(std::ostream& out, const RobotVec& r_vec) {
        std::unordered_set<TPos,TPos::HashFunction> r_pos_set;
        for (const auto& r : r_vec) r_pos_set.insert(r.pos);

        for (int i=0; i<WIDTH; ++i) {
            for (int j=0; j<HEIGHT; ++j) {
                if (r_pos_set.contains(TPos{ i,j })) out << '#';
                else out << '.';
            }
            out << "\n";
        }
        out << "\n";

        return out;
    }

};
