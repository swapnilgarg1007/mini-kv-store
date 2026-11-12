#pragma once
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>

// In-memory hash map backed by an append-only write-ahead log (WAL).
// Every mutation is appended to the log before the map changes; on startup the log is replayed.
// Limitation: keys cannot contain whitespace or newlines; values cannot contain newlines.
class KVStore {
public:
    explicit KVStore(const std::string& wal_path);
    void set(const std::string& key, const std::string& value);
    bool del(const std::string& key);
    std::optional<std::string> get(const std::string& key) const;
    std::size_t size() const { return data_.size(); }

private:
    void replay();
    void append(const std::string& line);

    std::string path_;
    std::unordered_map<std::string, std::string> data_;
    std::ofstream wal_;
};
