#include "kvstore.hpp"
#include <sstream>

KVStore::KVStore(const std::string& wal_path) : path_(wal_path) {
    replay();
    wal_.open(path_, std::ios::app);
}

void KVStore::replay() {
    std::ifstream in(path_);
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string op, key;
        ss >> op >> key;
        if (op == "SET") {
            std::string value;
            std::getline(ss, value);
            if (!value.empty() && value[0] == ' ') value.erase(0, 1);
            data_[key] = value;
        } else if (op == "DEL") {
            data_.erase(key);
        }
        // A truncated or malformed last line (crash mid-write) is ignored.
    }
}

void KVStore::append(const std::string& line) {
    wal_ << line << '\n';
    wal_.flush();  // flushes to the OS; a real fsync is on the roadmap
}

void KVStore::set(const std::string& key, const std::string& value) {
    append("SET " + key + " " + value);
    data_[key] = value;
}

bool KVStore::del(const std::string& key) {
    if (!data_.count(key)) return false;
    append("DEL " + key);
    data_.erase(key);
    return true;
}

std::optional<std::string> KVStore::get(const std::string& key) const {
    auto it = data_.find(key);
    if (it == data_.end()) return std::nullopt;
    return it->second;
}
