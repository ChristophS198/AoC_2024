#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../utility.h"

namespace Day24
{
    using TWire = std::string;
    using TWireMap = std::unordered_map<TWire, bool>; 
    using TId = std::uint16_t;

    enum class TOperation {
        AND = 0,
        OR,
        XOR
    };

    struct Gate {
        TId id;
        TWire in1;
        TWire in2;
        TOperation op;
        TWire out;
        struct HashFunction {
            size_t operator()(const Gate& g) { (size_t)g.id; } // simply return the id
        };
    };

    std::pair<TWireMap, std::vector<Gate>> get_data(const std::string& file_path);
    void calc_gates(TWireMap& wire_map, std::vector<Gate>& gate_vec);
    std::uint64_t get_decimal_num(const TWireMap& wire_map);
    
    std::uint64_t sol_24_1(const std::string &file_path)
    {
        auto [wire_map, gate_vec] = get_data(file_path);
        calc_gates(wire_map, gate_vec);

        

        return get_decimal_num(wire_map);
    }


    int sol_24_2(const std::string &file_path)
    {

        return 0;
    }

    void calc_gates(TWireMap& wire_map, std::vector<Gate>& gate_vec)
    {
        int num_calcs{ 1 };
        std::vector<TId> to_be_processed;
        for (TId id=0; id<gate_vec.size(); ++id) to_be_processed.push_back(id);

        while (num_calcs > 0) 
        {
            num_calcs = 0;
            std::vector<TId> tmp_to_be_processed;

            for (auto g_id : to_be_processed) {
                auto& gate = gate_vec[g_id];
                auto in1 = wire_map.find(gate.in1);
                auto in2 = wire_map.find(gate.in2);
                if (in1 != wire_map.end() && in2 != wire_map.end()) {
                    switch (gate.op)
                    {
                    case TOperation::AND:
                        wire_map[gate.out] = in1->second && in2->second;
                        break;
                    case TOperation::OR:
                        wire_map[gate.out] = in1->second || in2->second;
                        break;
                    case TOperation::XOR:
                        wire_map[gate.out] = in1->second != in2->second;
                        break;
                    }

                    ++num_calcs;
                }
                else {
                    // keep for later processing
                    tmp_to_be_processed.push_back(g_id);
                }

            }

            // so only unprocessed gates are check in next loop
            to_be_processed = tmp_to_be_processed;
        }
    }


    std::pair<TWireMap, std::vector<Gate>> get_data(const std::string& file_path)
    {
        TWireMap wire_map;
        std::vector<Gate> gate_vec;

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;

            // read system input
            while(getline(input_file, input_line))
            {
                if (input_line == "") break;
                auto str_split = split_string(input_line, ": ");
                // wire_map[str_split[0]] = std::stoi(str_split[1]);
                wire_map[str_split[0]] = str_split[1] == "1";
            }

            // read gates
            TId gate_id{ };
            while(getline(input_file, input_line))
            {
                if (input_line == "") break;
                auto str_split = split_string(input_line, " ");
                TOperation op;
                if (str_split[1] == "AND") op = TOperation::AND;
                else if (str_split[1] == "OR") op = TOperation::OR;
                else if (str_split[1] == "XOR") op = TOperation::XOR;
                gate_vec.push_back(Gate{ gate_id, str_split[0],str_split[2],op,str_split[4] });

                // // all other inputs and outputs are set to -1
                // wire_map.insert({ str_split[0],-1 });
                // wire_map.insert({ str_split[1],-1 });
                // wire_map.insert({ str_split[4],-1 });

                ++gate_id;
            }
            input_file.close();
        }

        return { wire_map, gate_vec };
    }

    std::uint64_t get_decimal_num(const TWireMap& wire_map) {
        std::string dec_num_str{ };
        std::uint64_t dec_num{ };

        for (int i=0; i<wire_map.size(); ++i) {
            char str[3];
            snprintf (str, 3, "%02d", i);
            auto out = "z" + std::string(str);
            auto elem = wire_map.find(out);
            if (elem != wire_map.end()) {
                dec_num_str += std::to_string(elem->second);
            }
            else {
                break;
            }
        }

        std::reverse(dec_num_str.begin(), dec_num_str.end());

        for (auto c : dec_num_str)
        {
            dec_num <<= 1;
            dec_num += c - '0';
        }
        return dec_num;
    }

}