#include <vector>
#include <array>
#include <iostream>




class Computer {
    public:
        using TVal = uint64_t;
        using TOpCode = uint8_t;
        using TOperand = uint8_t;
        using TPNum = uint8_t;

        Computer(const std::vector<TPNum>& program, TVal r_a, TVal r_b, TVal r_c) 
            : m_program{ program }, m_registers{ r_a,r_b,r_c }, m_instr_ptr{ 0 }, m_output{} {};
        bool execute_nxt_instruction();
        std::string get_output() const;
        std::vector<TPNum> get_program() const { return m_program; }
        auto get_registers() const { return m_registers; }
        void reset(std::array<TVal,3> registers);
    private:
        std::vector<TPNum> m_program{ }; // program consisting of 3-bit numbers (0...7)
        std::array<TVal,3> m_registers{ }; // the three registers A,B,C
        size_t m_instr_ptr{ }; // instruction pointer
        std::vector<TVal> m_output{ };

        void adv_0(TVal operand_val);
        void bxl_1(TVal operand_val);
        void bst_2(TVal operand_val);
        void jnz_3(TVal operand_val);
        void bxc_4(TVal operand_val);
        void out_5(TVal operand_val);
        void bdv_6(TVal operand_val);
        void cdv_7(TVal operand_val);

        TVal get_operand_val(TPNum op_code, TPNum operand) const;
};
