# MPES (Multi-stage Pre-threaded Epoll Server)

A high-performance, prethreaded HTTP/1.1 server built on Linux epoll.
Designed around a **staged threadpool pipeline**, where each stage of socket service is handled by a dedicated pool, keeping hot paths lean and contention minimal.

> Targets raw throughput, low per-request latency, and high connection concurrency simultaneously without sacrificing architectural clarity.

## Requirements

- **OS:** Linux 2.6.17+ (epoll is Linux-specific)
- **Compiler:** GCC 11+ or Clang 13+ with C++20 support
- **Build system:** CMake 3.22+
- **Dependencies:** None (stdlib + POSIX only)

---

## License

MIT — see [`LICENSE`](LICENSE).
