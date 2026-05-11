# MPES (Multi-stage Pre-threaded Epoll Server)

A high-performance, prethreaded HTTP/1.1 server built on Linux epoll.
Designed around a **staged threadpool pipeline**, where each stage of socket service is handled by a dedicated pool, keeping hot paths lean and contention minimal.

> Targets raw throughput, low per-request latency, and high connection concurrency simultaneously without sacrificing architectural clarity.

**Notes**

For this project, I'm adopting the style guide of Canonical's Mir display server.

https://canonical.com/mir/docs/stable/contributing/reference/cppguide/

I was looking around the Mir repo and the style caught me off guard. Then I found the guide and thought it would be a nice muscle to flex, as my return to GitHub is with the sole intent of appealing to prospective employers like Canonical.

## Working Logs

<details>
<summary>Sunday May 10, 2026</summary>

Looking over the old codebase which I wrote years ago (and which MPES is based on), I'm seeing a lot of regrettable and embarrassing design decisions which I would never reveal to anyone.. I intend for MPES to be the best socket server I can possibly design, and, as such, I cannot just repaste this repo back on GitHub for the sake of having something on public exhibit.

In the old codebase, my ThreadPool class used a Worker subclass to associate threads with their own managed collection for tasks (delegated round robin), among other extensions. This was to avoid having threads fighting over the lock to the queue which they all rely on for tasks. This was an idea I stole from somewhere, but pretended like I came up with it and misunderstood it as being totally bulletproof, non-blocking, christ-like, etc. A more appropriate collection for this particular application is without question a ring buffer.

Ring buffers are not staple topics of programming podcasts for no good old reason. They are the collection of choice in audio, video, and high-performance, high-concurrency financial / blockchain systems. Like all collections, the semantics about their design should be tailored to the specific application to achieve excellence. You've probably heard the line that in concurrent single producer / single consumer use-cases, ring buffers can be safely implemented locklessly and zero-copy, and achieve devilish performance benchmarks which defy mortal cognizance.

It's pretty late at the time of writing this log, but if I commit one thing to this repo tonight, it will be a bare bones ring buffer class which I can tweak to my needs. My thoughts on the matter are:
 - If I'm shooting for ultimate performance and concurrency, I should really bias hard for the lockless design even though this is more of a mpsc case.. If I have enough threads in each threadpool, I may be able to control the "gap" between writes upon one worker's task buffer. LOL. It sounds stupid but, whatever. If it fails, whatever, I'll lock the write head or something whatever.
 - As long as my ring buffers are provided a large enough allocation to exceed a target benchmark, the possibility of the write head lapping the read head can be made negligible, and I don't have to worry about expansion logic or any other buffer-is-full behaviors which require blocking.
</details>

## Requirements

- **OS:** Linux 2.6.17+ (epoll is Linux-specific)
- **Compiler:** GCC 11+ or Clang 13+ with C++20 support
- **Build system:** CMake 3.22+
- **Dependencies:** None (stdlib + POSIX only)

---

## License

MIT - see [`LICENSE`](LICENSE).
