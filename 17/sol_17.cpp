#include <string>
#include <ostream>
#include <cmath>
#include <limits>

#include "../utility.h"
#include "program.hpp"

namespace Day17
{
    Computer get_data(const std::string& file_path);
    std::string get_exp_output(const std::vector<Computer::TPNum>& program);
    Computer::TVal rec(const std::string exp_output, std::array<Computer::TVal, 3UL> registers, int idx, Computer computer);


    std::string sol_17_1(const std::string &file_path)
    {
        auto computer = get_data(file_path);

        while (computer.execute_nxt_instruction()) ;
        
        return computer.get_output();
    }


    Computer::TVal sol_17_2(const std::string &file_path)
    {
        auto computer = get_data(file_path);
        auto exp_output = get_exp_output(computer.get_program());

        auto registers = computer.get_registers();
        registers[0] = 0;
        
        // starting from initial register A value try the next 8 consecutive reg A values and start new recursion whenever a new
        // match is found, i.e., the postfix of exp_output matches the current output
        auto min_reg_a = std::numeric_limits<Computer::TVal>::max();
        for (int i=0; i<8; ++i) 
        {
            // reset computer
            computer.reset(registers);

            auto tmp = rec(exp_output, registers, 0, computer);
            if (tmp < min_reg_a) min_reg_a = tmp;
            ++registers[0];
        }

        return min_reg_a;
    }

    /*
    From analyzing the output string one can see that the output gets ever larger in cycles of 8^i. For each value of the preivous
    round a new round is started, prefixing 8 new values. 
    What we do here is: try to match the expected output from the back and whenever a match is found we multiply the current Reg A value
    by 8 (this should be the starting point, where the currently matched postfix is prefixed with the first of 8 new digits) and starting
    a new recursion. 
    */
    Computer::TVal rec(const std::string exp_output, std::array<Computer::TVal, 3UL> registers, int idx, Computer computer)
    {
        // calculate output for current reigster values
        while (computer.execute_nxt_instruction()) ;
        // get output
        auto output = computer.get_output();

        // check end condition
        if (exp_output.length() == output.length())
        {
            if (exp_output == output) 
            {
                return registers[0];
            }
            else return std::numeric_limits<Computer::TVal>::max();
        }

        // end recursion if postfix does not match 
        if (output != exp_output.substr(exp_output.length() - idx*2 - 1)) 
        {
            return std::numeric_limits<Computer::TVal>::max();
        }

        // starting from initial register A value try the next 8 consecutive reg A values and start new recursion whenever a new
        // match is found, i.e., the postfix of exp_output matches the current output
        registers[0] *= 8ull;
        auto min_reg_a = std::numeric_limits<Computer::TVal>::max();

        for (int i=0; i<8; ++i) 
        {
            // reset computer
            computer.reset(registers);

            // start new recursion
            auto tmp = rec(exp_output, registers, idx+1, computer);
            if (tmp < min_reg_a) min_reg_a = tmp;
            ++registers[0];
        }

        return min_reg_a;
    }

    Computer get_data(const std::string& file_path)
    {
        std::vector<Computer::TVal> number_vec{};
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);

        if (input_file.is_open())
        {
            std::string input_line;
            getline(input_file, input_line);
            auto num_in_line = parse_string_to_number_vec<Computer::TVal>(input_line);
            auto reg_a = num_in_line[0];

            getline(input_file, input_line);
            num_in_line = parse_string_to_number_vec<Computer::TVal>(input_line);
            auto reg_b = num_in_line[0];

            getline(input_file, input_line);
            num_in_line = parse_string_to_number_vec<Computer::TVal>(input_line);
            auto reg_c = num_in_line[0];

            getline(input_file, input_line);
            getline(input_file, input_line);
            auto program = parse_string_to_number_vec<Computer::TPNum>(input_line);
            
            input_file.close();   
            return Computer(program, reg_a, reg_b, reg_c);
        }


        throw std::runtime_error("Unable to read data from file!");
    }


    std::string get_exp_output(const std::vector<Computer::TPNum>& program) 
    { 
        if (program.size() == 0) return "";

        std::string out_str{ std::to_string(program[0]) };

        for (int i=1; i<program.size(); ++i) out_str += "," + std::to_string(program.at(i));

        return out_str; 
    }

};

void Computer::reset(std::array<TVal,3> registers)
{
    m_registers = registers;
    m_instr_ptr = 0;
    m_output = {};
}

bool Computer::execute_nxt_instruction() 
{
    if (m_instr_ptr >= m_program.size()) {
        return false;
    }

    auto op_code = m_program[m_instr_ptr];
    auto operand = m_program[m_instr_ptr+1];
    auto operand_val = get_operand_val(op_code, operand);

    switch (op_code)
    {
    case 0:
        adv_0(operand_val);
        break;
    case 1:
        bxl_1(operand_val);
        break;
    case 2:
        bst_2(operand_val);
        break;
    case 3:
        jnz_3(operand_val);
        break;
    case 4:
        bxc_4(operand_val);
        break;
    case 5:
        out_5(operand_val);
        break;
    case 6:
        bdv_6(operand_val);
        break;
    case 7:
        cdv_7(operand_val);
        break;
    
    default:
        break;
    }
    
    return true;
}

Computer::TVal Computer::get_operand_val(TPNum op_code, TPNum operand) const
{
    if (op_code == 1 || op_code == 3) return operand;
    else {
        // combo operand
        switch (operand)
        {
        case 0:
            [[fallthrough]]
        case 1:
            [[fallthrough]]
        case 2:
            [[fallthrough]]
        case 3:
            return operand;
            break;
        case 4:
            return m_registers[0];
            break;
        case 5:
            return m_registers[1];
            break;
        case 6:
            return m_registers[2];
            break;
        
        default:
            throw std::runtime_error("Following operand for combo operand not valid!" + std::to_string(operand));
            break;
        }

        return 0;
    }
}
/*
The adv instruction (opcode 0) performs division. The numerator is the value in the A register. 
The denominator is found by raising 2 to the power of the instruction's combo operand. 
(So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.) The result of the division operation is truncated to an integer and then written to the A register.
*/
void Computer::adv_0(TVal operand_val) {
    TVal denominator = 1LL << operand_val;
    m_registers[0] /= denominator;
    m_instr_ptr += 2;
}

/*
The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand, then stores the result in register B.
*/
void Computer::bxl_1(TVal operand_val) {
    m_registers[1] ^= operand_val;
    m_instr_ptr += 2;
}

/*
The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest 3 bits), then writes that value to the B register.
*/
void Computer::bst_2(TVal operand_val) {
    m_registers[1] = operand_val & 7;
    m_instr_ptr += 2;
}

/*
The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register is not zero, it jumps by setting the instruction 
pointer to the value of its literal operand; if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
*/
void Computer::jnz_3(TVal operand_val) {
    if (m_registers[0] == 0) m_instr_ptr += 2;
    else m_instr_ptr = operand_val;
}

/*
The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then stores the result in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
*/
void Computer::bxc_4(TVal operand_val) {
    m_registers[1] ^= m_registers[2];
    m_instr_ptr += 2;
}

/*
The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. (If a program outputs multiple values, they are separated by commas.)
*/
void Computer::out_5(TVal operand_val) {
    auto out = operand_val & 7;
    m_output.push_back(out);
    m_instr_ptr += 2;
}

/*
The bdv instruction (opcode 6) works exactly like the adv instruction except that the result is stored in the B register. (The numerator is still read from the A register.)
*/
void Computer::bdv_6(TVal operand_val) {
    TVal denominator = 1LL << operand_val;
    m_registers[1] = m_registers[0] / denominator;
    m_instr_ptr += 2;
}

/*
The cdv instruction (opcode 7) works exactly like the adv instruction except that the result is stored in the C register. (The numerator is still read from the A register.)
*/
void Computer::cdv_7(TVal operand_val) {
    TVal denominator = 1LL << operand_val;
    m_registers[2] = m_registers[0] / denominator;
    m_instr_ptr += 2;
}

std::string Computer::get_output() const 
{ 
    if (m_output.size() == 0) return "";

    std::string out_str{ std::to_string(m_output[0]) };

    for (int i=1; i<m_output.size(); ++i) out_str += "," + std::to_string(m_output.at(i));

    return out_str; 
}

