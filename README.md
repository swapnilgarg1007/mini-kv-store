# mini-kv-store

A small key-value store in C++17: an in-memory hash map made durable by an append-only write-ahead log (WAL). Writes go to the log first, and the log is replayed on startup to rebuild state.

## Build and run
```bash
cmake -S . -B build && cmake --build build
./build/kvcli            # interactive: SET k v | GET k | DEL k | EXIT
cd build && ctest        # run tests
```

## Design
1. `set` / `del` append a line to the WAL, then update the map.
2. On startup the WAL is replayed in order; the last write per key wins.
3. A malformed trailing line (crash mid-write) is ignored during replay.

## Limitations
- Keys can't contain whitespace; values can't contain newlines.
- `flush()` pushes to the OS, not to disk (no fsync yet).
- Log grows forever (no compaction), single-threaded, no network interface.

## Roadmap
- [ ] fsync option and a durability benchmark
- [ ] Log compaction / snapshots
- [ ] TCP server with a simple text protocol
- [ ] Length-prefixed WAL format for arbitrary keys and values
- [ ] Throughput benchmarks in this README

## License
MIT
