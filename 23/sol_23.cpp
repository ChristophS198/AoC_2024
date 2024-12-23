#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "../utility.h"

namespace Day23
{
    using TComMap = std::unordered_map<std::string, std::vector<std::string>>;
    // using TComNet = std::unordered_set<std::string>;
    using TConnectionSet = std::unordered_set<std::string>;

    struct TComNet {
        // std::vector<std::string> com_vec;
        std::set<std::string> com_set;
        std::string get_name() const { 
            if (com_set.size() == 0) return "";
            std::string name{ };
            for (const auto& com : com_set) name += com + '-';    
            return name.substr(0, name.length()-1);
        }
        bool operator<(const TComNet& n) const { return this->get_name() < n.get_name(); }
        bool operator==(const TComNet& n) const { return this->get_name() == n.get_name(); }
        struct HashFunction 
        {
            size_t operator()(const TComNet &n) const { return std::hash<std::string>{}(n.get_name()); };
        };
    };
    bool operator<(const TComNet& n, const TComNet& n2) {
        return n.get_name() < n2.get_name();
    }

    std::pair<TComMap, TConnectionSet>  get_computer_map(const std::string& file_path);
    std::unordered_set<TComNet, TComNet::HashFunction> get_com_nets_of_size_2(const TComMap& com_map, const TConnectionSet& conn_set);
    std::unordered_set<TComNet, TComNet::HashFunction> filter_com_nets(const std::unordered_set<TComNet, TComNet::HashFunction>& com_nets, char s);
    std::vector<TComNet> enlarge_net(const TComMap& com_map, const TConnectionSet& conn_set, TComNet com_net);

    int sol_23_1(const std::string &file_path)
    {
        auto [com_map, conn_set] = get_computer_map(file_path);
        auto com_nets = get_com_nets_of_size_2(com_map, conn_set);
        auto filtered_com_nets = filter_com_nets(com_nets, 't');

        return filtered_com_nets.size();
    }


    std::string sol_23_2(const std::string &file_path)
    {

        auto [com_map, conn_set] = get_computer_map(file_path);
        auto com_nets = get_com_nets_of_size_2(com_map, conn_set);

        while (com_nets.size() > 1) {
            std::unordered_set<TComNet, TComNet::HashFunction> new_com_nets;
            // To get the optimal result one has to enlarge iteratively and add each enlarged network to the set of networks
            // which get enlarged in the next cycle
            for (const auto& com_net : com_nets) {
                auto enlarged_com_net = enlarge_net(com_map, conn_set, com_net);
                new_com_nets.insert(enlarged_com_net.begin(), enlarged_com_net.end());
            }
            com_nets = new_com_nets;
        }

        auto name = com_nets.begin()->get_name();
        std::replace( name.begin(), name.end(), '-', ',');
        
        return name;
    }

    /*
    Idea is to add one computer to an existing network, by checking if this computer is also connected to all
    other computers already in the network. 
    It is sufficient to check all connected computers of a single computer already in the network
    */
    std::vector<TComNet> enlarge_net(const TComMap& com_map, const TConnectionSet& conn_set, TComNet com_net) {
        std::vector<TComNet> enlarged_nets;

        // pick any computer already in the network
        std::string com{ *com_net.com_set.begin() };

        // check for each computer connected to the first computer of com_net if a neighboring computer is also connected to all other pcs in the network
        for (const auto& connected_com : com_map.at(com)) {
            if (com_net.com_set.contains(connected_com)) continue;

            bool is_connected_to_all{ true };

            // check if this neighboring computer is connected to all other computer in the network
            for (const auto& net_com : com_net.com_set) {
                std::string connection{ connected_com + '-' + net_com };
                if (!conn_set.contains(connection)) {
                    is_connected_to_all = false;
                    break;
                }
            }
            if (is_connected_to_all) {
                auto tmp_net = com_net;
                tmp_net.com_set.insert(connected_com);
                enlarged_nets.push_back(tmp_net);
            }
        }

        return enlarged_nets;
    }

    std::unordered_set<TComNet, TComNet::HashFunction> filter_com_nets(const std::unordered_set<TComNet,TComNet::HashFunction>& com_nets, char s)
    {
        std::unordered_set<TComNet, TComNet::HashFunction> filtered_com_nets;

        for (const auto& com_net : com_nets) {
            for (const auto& com : com_net.com_set) {
                if (s == com.at(0)) {
                    filtered_com_nets.insert(com_net);
                    break;
                }
            }
        }

        return filtered_com_nets;
    }

    std::unordered_set<TComNet, TComNet::HashFunction> get_com_nets_of_size_2(const TComMap& com_map, const TConnectionSet& conn_set)
    {
        std::unordered_set<TComNet, TComNet::HashFunction> com_nets;

        for (const auto& [com, connected_coms] : com_map) {
            if (connected_coms.size() >= 2) {
                for (int i=0; i<connected_coms.size(); ++i) {
                    for (int j=i+1; j<connected_coms.size(); ++j) {
                        if (conn_set.contains(connected_coms[i] + '-' + connected_coms[j])) {
                            std::string name{ };
                            TComNet com_net{ };
                            com_net.com_set.insert(com);
                            com_net.com_set.insert(connected_coms[i]);
                            com_net.com_set.insert(connected_coms[j]);
                            com_nets.insert(com_net);
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