#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "../utility.h"

namespace Day24
{
    using TWire = std::string;
    using TWireStateMap = std::unordered_map<TWire, bool>; 
    using TId = std::uint16_t;
    using TWireGateMap = std::unordered_map<TWire, std::vector<TId>>; 

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

    struct Data {
        TWireStateMap wire_state_map; 
        std::vector<Gate> gate_vec;
        TWireGateMap wire_gate_map;
    };

    Data get_data(const std::string& file_path);
    TWireStateMap& calc_gates(TWireStateMap& wire_map, std::vector<Gate>& gate_vec);
    std::uint64_t calc_gates_2(TWireStateMap wire_map, std::vector<Gate>& gate_vec, TWireGateMap& wire_gate_map, std::vector<int> gate_input_ct, std::queue<TId> to_be_processed);
    std::uint64_t get_decimal_num(const TWireStateMap& wire_map, std::string prefix="z");
    
    std::uint64_t sol_24_1(const std::string &file_path)
    {
        auto [wire_map, gate_vec, wire_gate_map] = get_data(file_path);

        std::vector<int> gate_input_ct(gate_vec.size(), 0); // counts for each gate the number of available inputs
        std::queue<TId> to_be_processed; // queues all gates for which both inputs are available -> value in gate_input_ct is == 2

        // iterate through all known wire values and increase the "available input count" for the connected gates
        for (auto elem : wire_map) {
            for (auto gate_id : wire_gate_map[elem.first]) {
                if (++gate_input_ct[gate_id] == 2) to_be_processed.push(gate_id);
            }
        }

        auto res = funcTime<std::uint64_t>(calc_gates_2, wire_map, gate_vec, wire_gate_map, gate_input_ct, to_be_processed);
        std::cout << "Duration: " << res.first << " ns" << std::endl;

        return get_decimal_num(wire_map);
    }


    /*
    I think the approach must be to calculate the wire states only once and then by switching some output wires, we just re-calculate 
    all gate outputs coming afterwards and only as long as there is a change on the wire. 
    By this some wire swaps can be skipped entirely if they do not lead to a change on the wire.
    -> swap two outputs and add the connected gates to the queue for re-calculation
    */
    std::string sol_24_2(const std::string &file_path)
    {
        auto [wire_map, gate_vec, wire_gate_map] = get_data(file_path);

        std::vector<int> gate_input_ct(gate_vec.size(), 0); // counts for each gate the number of available inputs
        std::queue<TId> to_be_processed; // queues all gates for which both inputs are available -> value in gate_input_ct is == 2

        // iterate through all known wire values and increase the "available input count" for the connected gates
        for (auto elem : wire_map) {
            for (auto gate_id : wire_gate_map[elem.first]) {
                if (++gate_input_ct[gate_id] == 2) to_be_processed.push(gate_id);
            }
        }

        auto res = funcTime<std::uint64_t>(calc_gates_2, wire_map, gate_vec, wire_gate_map, gate_input_ct, to_be_processed);
        std::cout << "Duration: " << res.first << " ns" << std::endl;

        auto dec_in1 = get_decimal_num(wire_map, "x");
        auto dec_in2 = get_decimal_num(wire_map, "y");
        auto exp_result = dec_in1 + dec_in2;

        auto num_gates = gate_vec.size();
        for (int i1=0; i1<num_gates; ++i1) 
        {
            for (int i2 = 1; i2 < num_gates; ++i2)
            {
                // std::cout << "i2: " << i2 << std::endl;
                // if (wire_map[gate_vec[i1].out] == wire_map[gate_vec[i2].out]) {
                //     continue;
                // }
                auto tmp = gate_vec[i1].out;
                gate_vec[i1].out = gate_vec[i2].out;
                gate_vec[i2].out = tmp;
                for (int i3 = 2; i3 < num_gates; ++i3)
                {
                    for (int i4 = 3; i4 < num_gates; ++i4)
                    {
                        auto tmp = gate_vec[i3].out;
                        gate_vec[i3].out = gate_vec[i4].out;
                        gate_vec[i4].out = tmp;
                        for (int i5 = 4; i5 < num_gates; ++i5)
                        {
                            for (int i6 = 5; i6 < num_gates; ++i6)
                            {
                                auto tmp = gate_vec[i5].out;
                                gate_vec[i5].out = gate_vec[i6].out;
                                gate_vec[i6].out = tmp;
                                for (int i7 = 6; i7 < num_gates; ++i7)
                                {
                                    for (int i8 = 7; i8 < num_gates; ++i8)
                                    {
                                        auto tmp = gate_vec[i7].out;
                                        gate_vec[i7].out = gate_vec[i8].out;
                                        gate_vec[i8].out = tmp;
                                        auto res = calc_gates_2(wire_map, gate_vec, wire_gate_map, gate_input_ct, to_be_processed);
                                        if (res == exp_result) {
                                            std::vector<std::string> wires{ };
                                            wires.push_back(gate_vec[i1].out);
                                            wires.push_back(gate_vec[i2].out);
                                            wires.push_back(gate_vec[i3].out);
                                            wires.push_back(gate_vec[i4].out);
                                            wires.push_back(gate_vec[i5].out);
                                            wires.push_back(gate_vec[i6].out);
                                            wires.push_back(gate_vec[i7].out);
                                            wires.push_back(gate_vec[i8].out);
                                            std::sort(wires.begin(), wires.end());
                                            std::string wire_str{ };
                                            for (const auto& str : wires) wire_str += str + ",";
                                            return wire_str.substr(0, wire_str.length()-1);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return "result not found";
    }

    std::uint64_t calc_gates_2(TWireStateMap wire_map, std::vector<Gate>& gate_vec, TWireGateMap& wire_gate_map, 
                                std::vector<int> gate_input_ct, std::queue<TId> to_be_processed)
    {
        while (!to_be_processed.empty()) 
        {
            auto nxt_gate_id = to_be_processed.front();
            to_be_processed.pop();

            // process this gate
            auto& gate = gate_vec[nxt_gate_id];
            switch (gate.op)
            {
            case TOperation::AND:
                wire_map[gate.out] = wire_map[gate.in1] && wire_map[gate.in2];
                break;
            case TOperation::OR:
                wire_map[gate.out] = wire_map[gate.in1] || wire_map[gate.in2];
                break;
            case TOperation::XOR:
                wire_map[gate.out] = wire_map[gate.in1] != wire_map[gate.in2];
                break;
            }

            // update "available input count" for gates connected to this output
            for (auto gate_id : wire_gate_map[gate.out]) {
                if (++gate_input_ct[gate_id] == 2) to_be_processed.push(gate_id);
            }

        }
        return get_decimal_num(wire_map);
    }

    TWireStateMap& calc_gates(TWireStateMap& wire_map, std::vector<Gate>& gate_vec)
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
        return wire_map;
    }

    Data get_data(const std::string& file_path)
    {
        TWireStateMap wire_map;
        std::vector<Gate> gate_vec;
        TWireGateMap wire_gate_map;

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

                wire_gate_map[str_split[0]].push_back(gate_id);
                wire_gate_map[str_split[2]].push_back(gate_id);

                ++gate_id;
            }
            input_file.close();
        }

        return { wire_map, gate_vec, wire_gate_map };
    }

    std::uint64_t get_decimal_num(const TWireStateMap& wire_map, std::string prefix) {
        std::string dec_num_str{ };
        std::uint64_t dec_num{ };

        for (int i=0; i<wire_map.size(); ++i) {
            char str[3];
            snprintf (str, 3, "%02d", i);
            auto out = prefix + std::string(str);
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