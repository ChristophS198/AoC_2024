#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../utility.h"

namespace Day23
{
    using TComMap = std::unordered_map<std::string, std::vector<std::string>>;
    using TComNet = std::vector<std::string>;
    using TConnectionSet = std::unordered_set<std::string>;

    std::pair<TComMap, TConnectionSet>  get_computer_map(const std::string& file_path);
    std::vector<TComNet> get_com_nets_of_size(const TComMap& com_map, const TConnectionSet& conn_set);
    std::vector<TComNet> filter_com_nets(const std::vector<TComNet>& com_nets, char s);

    int sol_23_1(const std::string &file_path)
    {
        auto [com_map, conn_set] = get_computer_map(file_path);
        std::vector<TComNet> com_nets = get_com_nets_of_size(com_map, conn_set);
        auto filtered_com_nets = filter_com_nets(com_nets, 't');

        return filtered_com_nets.size();
    }


    int sol_23_2(const std::string &file_path)
    {


        return 0;
    }

    To get the optimal result I have to enlarge iteratively and add each enlarged network to my set of networks

    /*
    Idea is to add one computer to an existing network, by checking if this computer is also connected to all
    other computers already in the network. 
    It is sufficient to check all connected computers of a single computer already in the network
    */
    std::vector<TComNet> enlarge_net(const TComMap& com_map, const TConnectionSet& conn_set, TComNet com_net) {
        // pick any computer already in the network
        std::string com{ com_net[0] };

        // check for each computer connected to the first computer of com_net if a neighboring computer is also connected to all other pcs in the network
        for (const auto& connected_com : com_map.at(com)) {
            bool is_connected_to_all{ true };

            // check if this neighboring computer is connected to all other computer in the network
            for (const auto& net_com : com_net) {
                std::string connection{ connected_com + '-' + net_com };
                if (!conn_set.contains(connection)) {
                    is_connected_to_all = false;
                    break;
                }
            }
            if (is_connected_to_all) {
                com_net.push_back(connected_com);
            }
        }
    }

    std::vector<TComNet> filter_com_nets(const std::vector<TComNet>& com_nets, char s)
    {
        std::vector<TComNet> filtered_com_nets;

        for (const auto& com_net : com_nets) {
            for (const auto& com : com_net) {
                if (s == com.at(0)) {
                    filtered_com_nets.push_back(com_net);
                    break;
                }
            }
        }

        return filtered_com_nets;
    }

    std::vector<TComNet> get_com_nets_of_size(const TComMap& com_map, const TConnectionSet& conn_set)
    {
        std::vector<TComNet> com_nets;
        std::unordered_set<std::string> com_net_names;

        for (const auto& [com, connected_coms] : com_map) {
            if (connected_coms.size() >= 2) {
                for (int i=0; i<connected_coms.size(); ++i) {
                    for (int j=i+1; j<connected_coms.size(); ++j) {
                        if (conn_set.contains(connected_coms[i] + '-' + connected_coms[j])) {
                            std::string name{};
                            TComNet com_net{ com, connected_coms[i], connected_coms[j] };
                            std::sort(com_net.begin(), com_net.end());
                            for (const auto& elem : com_net) name += elem + '-';
                            name = name.substr(0,name.length()-1);
                            if (com_net_names.insert(name).second) {
                                com_nets.push_back(com_net);
                                // std::cout << name << std::endl;
                            }
                        }
                    }
                }
            }
        }

        return com_nets;
    }


    std::pair<TComMap, TConnectionSet> get_computer_map(const std::string& file_path) {
        TComMap com_map;
        TConnectionSet conn_set;
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                auto str_split = split_string(input_line, "-");
                com_map[str_split[0]].push_back(str_split[1]);
                com_map[str_split[1]].push_back(str_split[0]);
                conn_set.insert(input_line);
                conn_set.insert(str_split[1] + '-' + str_split[0]);
            }
            input_file.close();
        }

        return { com_map,conn_set };
    }

}