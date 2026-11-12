#include "kvstore.hpp"
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
    KVStore store(argc > 1 ? argv[1] : "kv.wal");
    std::string line;
    std::cout << "mini-kv-store (SET k v | GET k | DEL k | EXIT)\n> " << std::flush;
    while (std::getline(std::cin, line)) {
        std::istringstream ss(line);
        std::string cmd, key;
        ss >> cmd >> key;
        if (cmd == "SET") {
            std::string value;
            std::getline(ss, value);
            if (!value.empty() && value[0] == ' ') value.erase(0, 1);
            store.set(key, value);
            std::cout << "OK\n";
        } else if (cmd == "GET") {
            auto v = store.get(key);
            std::cout << (v ? *v : "(nil)") << "\n";
        } else if (cmd == "DEL") {
            std::cout << (store.del(key) ? "1" : "0") << "\n";
        } else if (cmd == "EXIT") {
            break;
        } else if (!cmd.empty()) {
            std::cout << "ERR unknown command\n";
        }
        std::cout << "> " << std::flush;
    }
}
