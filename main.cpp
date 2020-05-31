#include <iostream>

#include "flow_network.h"

#include <iostream>
#include "graph.h"

int main() {
    FlowNetwork network;

    bool run = true;
    while (run) {
        std::cout << "\n\n\nNetwork:\n";
        network.print();
        bool is_valid = network.is_valid();
        std::cout << (is_valid ? "Network is valid" : "Network is invalid") << "\n";
        std::cout << "\n\ncommands: \n 1 - rebuild flow\n 2 - add/remove edge\n 3 - open file\n other - exit\n \ninput command: ";

        int cmd;
        std::cin >> cmd;
        getchar();
        switch (cmd) {
            case 1: {
                if (is_valid) {
                    network.build_max_flow();
                } else {
                    std::cout << "Cannot rebuild max flow on invalid network!\n";
                }
                break;
            }
            case 2: {
                std::string line;
                std::cout << "Edge format: A B max_flow, where:\n A - start vertex\n B - end vertex \n max_flow - edge max flow (0 to remove edge)\ninput edge: ";
                std::getline(std::cin, line);
                if (!network.parse_edge(line)) {
                    std::cout << "failed to parse edge!\n";
                }
                break;
            }
            case 3: {
                std::string filename;
                std::cout << "input file name: ";
                std::cin >> filename;
                if (network.from_file(filename)) {
                    std::cout << "success\n";
                } else {
                    std::cout << "failed to read\n";
                }
                break;
            }
            default:
                run = false;
                break;
        }
        std::cout << "\n";
    }

    return 0;
}