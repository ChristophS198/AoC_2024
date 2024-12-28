#include <string>
#include <array>
#include <map>
#include <numeric>

#include "../utility.h"

namespace Day13
{
    using TVal = int64_t;

    constexpr TVal OFFSET{ 10'000'000'000'000 };
    
    struct ClawMachine {
        Point<TVal> buttonA;
        Point<TVal> buttonB;
        Point<TVal> price;
    };

    std::vector<ClawMachine> get_data(const std::string& file_path);
    Point<int64_t> get_button_pushes(const ClawMachine& claw);
    int64_t get_cheapest_sol(const ClawMachine& claw, int p1, int p2);


    // too low: 14762
    int64_t sol_13_1(const std::string &file_path)
    {
        auto claw_vec = get_data(file_path);
        int64_t sum_price{ };

        for (const auto& claw : claw_vec) {
            sum_price += get_cheapest_sol(claw, 3, 1);
        }

        return sum_price;
    }


    int64_t sol_13_2(const std::string &file_path)
    {
        auto claw_vec = get_data(file_path);
        int64_t sum_price{ };

        for (auto claw : claw_vec) {
            claw.price = claw.price + Point<TVal>{ OFFSET,OFFSET };
            sum_price += get_cheapest_sol(claw, 3, 1);
        }

        return sum_price;
    }

    /**
     * returns the factor so b1 = b2 * fact or b2 = b1 * fact or returns -1 if there exists no integer multiple
     */
    int get_fact(Point<TVal> b1, Point<TVal> b2) {
        auto l{ b1 };
        auto s{ b2 };

        if (b1.x < b2.x) {
            l = b2;
            s = b1;
        }

        if (l.x % s.x == 0 && l.y % s.y == 0) {
            int fact = l.x / s.x;
            if (fact * s.y == l.y) return fact;
        }

        return -1;
    }

    int64_t get_cheapest_sol(const ClawMachine& claw, int p1, int p2)
    {
        auto b_pushes = get_button_pushes(claw);
        auto fact = get_fact(claw.buttonA, claw.buttonB);
        if (fact > 0) {
            // this apparently does not happen in my input -> I will not cover this special case
            std::cout << "Multiple found: " << claw.buttonA << ", " << claw.buttonB << std::endl;
        }

        return p1 * b_pushes.x + p2 * b_pushes.y;
    }

    Point<int64_t> get_button_pushes(const ClawMachine& claw)
    {
        auto dividend = claw.buttonA.x * claw.price.y - claw.buttonA.y * claw.price.x;
        auto divisor = claw.buttonA.x * claw.buttonB.y - claw.buttonA.y * claw.buttonB.x;

        if (dividend % divisor == 0) {
            TVal b_pushes = dividend / divisor;
            TVal dividend_2 = claw.price.x - b_pushes * claw.buttonB.x;
            if (dividend_2 % claw.buttonA.x == 0) {
                TVal a_pushes = (dividend_2) / claw.buttonA.x;
                return { a_pushes, b_pushes };
            }
        }

        // in all other cases no valid solution exists
        return { 0,0 };
    }

    std::vector<ClawMachine> get_data(const std::string& file_path)
    {
        std::vector<ClawMachine> claw_data;

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                ClawMachine claw{};
                // button A nums
                auto nums = parse_string_to_number_vec<TVal>(input_line);
                claw.buttonA = { nums[0],nums[1] };

                // button B nums
                getline(input_file, input_line);
                nums = parse_string_to_number_vec<TVal>(input_line);
                claw.buttonB = { nums[0],nums[1] };

                // price nums
                getline(input_file, input_line);
                nums = parse_string_to_number_vec<TVal>(input_line);
                claw.price = { nums[0],nums[1] };
                
                getline(input_file, input_line);
                
                claw_data.push_back(claw);
            }
            input_file.close();   
        }

        return claw_data;
    }

} // namespace Day13