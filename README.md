# byte-to-cloud

A backend system built from first principles, from raw bytes in memory to a service running on AWS.

Each layer is built twice:

1. **From scratch**: the smallest implementation that exposes the mechanism (C, system calls, plain files).
2. **With the production tool**: what the tool does for you, and how.
3. **Measured**: what the tool hides, what it costs, and how it fails when that hidden detail leaks.

Every experiment states a prediction before it runs, then deliberately breaks the system and fixes it. Findings are recorded in each commit message (`git log`).

## Components

Order: **Foundations → Database → System design → Language & software design → Cloud**. Computer-science fundamentals first; cloud comes last, as the place where everything above runs.

A daily **data structures & algorithms** track runs alongside all phases.

### F · Foundations

| # | Component | From scratch | Production equivalent | Status |
|---|---|---|---|---|
| F1 | Data representation | Byte-level inspection of integers, IEEE 754 floats, endianness, UTF-8 | JS `number`/`string`, PostgreSQL types | ✅ |
| F2 | Memory model | Stack vs heap, pointers; six ways to break memory: dangling pointer, stack overflow, leak, use-after-free, buffer overflow, double free | Garbage-collected runtimes, AddressSanitizer | ✅ |
| F3 | Network & concurrency | Blocking socket server in C, data race between threads, mutex | `node:http`, database locks | ✅ |
| F4 | Event loop | Non-blocking server in C on `kqueue` | Node.js / libuv | ✅ |

### D · Database

| # | Component | From scratch | Production equivalent | Status |
|---|---|---|---|---|
| D1 | Relational modeling | | Entities, keys, constraints in PostgreSQL | |
| D2 | Storage & durability | `write` vs `fsync` measured; append-only log with crash recovery | Pages, WAL, `fsync` | |
| D3 | Indexing & query plans | Full scan | B-tree, join strategies, N+1, `EXPLAIN ANALYZE` on 1M rows | |
| D4 | Transactions | Lost update and double booking across two sessions | MVCC, isolation levels, write skew, deadlocks, `SELECT … FOR UPDATE` | |
| D5 | Money & time | Float rounding errors | `NUMERIC`, `TIMESTAMPTZ`, append-only ledger, snapshots | |
| D6 | Schema evolution & multi-tenancy | | Non-blocking migrations (expand → migrate → contract), `tenant_id`, row-level security | |
| D7 | Caching | | Redis cache-aside, invalidation, stampede protection | |
| D8 | Replication & backup | | Read replicas, replication lag, point-in-time recovery, partitioning | |

### S · System design

| # | Component | Focus | Status |
|---|---|---|---|
| S1 | Requirements & estimation | Functional / non-functional requirements, back-of-the-envelope numbers, Little's Law | |
| S2 | Reliability | Timeouts, retries with backoff, idempotency keys, transactional outbox, `SKIP LOCKED` workers, reconciliation | |
| S3 | Scale | Stateless services, load balancing, queues, backpressure, consistency models | |
| S4 | API design, observability & security | REST (resources, status codes, pagination, versioning, idempotency), WebSockets, JWT/OAuth basics, HMAC webhook signatures, authn/authz, logs, metrics, p95 latency, alarms | |
| S5 | Capstone | End-to-end design of a webhook → job pipeline, trade-offs, behaviour at 10× load | |

### L · Language & software design

| # | Component | Focus | Status |
|---|---|---|---|
| L1 | Node.js runtime & V8 | V8 pipeline (parser → Ignition bytecode → Maglev/TurboFan JIT), deoptimization, hidden classes & inline caches, generational GC and pauses; event loop phases, microtasks vs macrotasks, streams & backpressure, `worker_threads`, memory leaks and heap snapshots | |
| L2 | TypeScript | Structural typing, generics, narrowing, `unknown` vs `any`, type erasure and runtime validation | |
| L3 | Software design & frameworks | Coupling and cohesion, OOP vs FP, SOLID, composition over inheritance; NestJS (dependency injection, modules, request lifecycle), React (rendering, reconciliation, hooks) | |

### C · Cloud (AWS)

| # | Component | From scratch | Production equivalent | Status |
|---|---|---|---|---|
| C1 | Compute | EC2 provisioned by hand, `systemd`, security groups | | |
| C2 | Managed data & access | | RDS, IAM, VPC, Secrets Manager | |
| C3 | Production topology | | Load balancer with 2 instances, SQS, CloudWatch alarms, full teardown | |

### A · Data structures & algorithms (daily, 30 minutes)

| # | Component | Focus | Status |
|---|---|---|---|
| A1 | Complexity | Big-O, measuring instead of guessing | |
| A2 | Arrays & hash maps | Hash function, collisions, resizing; two pointers, sliding window | |
| A3 | Stacks & queues | Ring buffer, BFS (Breadth-First Search), monotonic stack | |
| A4 | Heaps | Priority queue, top-K, scheduling | |
| A5 | Trees | Binary search tree, B-tree (link to D3), DFS (Depth-First Search) | |
| A6 | Graphs | Adjacency lists, BFS/DFS, topological sort | |
| A7 | Sorting & searching | Merge sort, quicksort, binary search | |

Each structure is implemented from scratch in TypeScript, then practised with one easy/medium interview problem per day.

## Running

Requires macOS or Linux with `cc`, `xxd`, `nc`, `curl`. Node.js 24 from F4 and the A track, Docker (PostgreSQL, Redis) from D1, a personal AWS account from C1.

```bash
cc -Wall app/01-bytes.c -o app/bytes
./app/bytes
```

## Ground rules

- All data and business scenarios are fictional. No code, data or information from any employer.
- No secrets in the repository: `.env` files and cloud credentials are git-ignored.
- Build outputs are not committed; they are always reproducible from source.
