# byte-to-cloud

A backend system built from first principles, from raw bytes in memory to a service running on AWS.

Each layer is built twice:

1. **From scratch**: the smallest implementation that exposes the mechanism (C, system calls, plain files).
2. **With the production tool**: what the tool does for you, and how.
3. **Measured**: what the tool hides, what it costs, and how it fails when that hidden detail leaks.

Every experiment states a prediction before it runs, then deliberately breaks the system and fixes it. Findings are recorded in each commit message (`git log`).

## Components

| # | Component | From scratch | Production equivalent | Status |
|---|---|---|---|---|
| 01 | Data representation | Byte-level inspection of integers, IEEE 754 floats, endianness, UTF-8 | JS `number`/`string`, PostgreSQL types | ✅ |
| 02 | Memory model | Stack, heap, pointers, use-after-free | Garbage-collected runtimes | |
| 03 | Durability | `write` vs `fsync`, measured | Database commit path | |
| 04 | TCP/HTTP server | Blocking socket server in C | `node:http` | |
| 05 | Concurrency primitives | Data race between threads, mutex | Database locks, atomic operations | |
| 06 | Storage engine | Append-only log with crash recovery | PostgreSQL WAL | |
| 07 | Relational queries | | PostgreSQL, `psql` | |
| 08 | Transactions | Lost update and double booking across two sessions | Isolation levels, `SELECT … FOR UPDATE` | |
| 09 | Indexing | Full scan | B-tree index, `EXPLAIN ANALYZE` on 1M rows | |
| 10 | Data modeling | | Invariants enforced as constraints | |
| 11 | Event loop | Non-blocking server in C on `kqueue` | Node.js / libuv | |
| 12 | Service layer | | Node.js HTTP + PostgreSQL connection pool | |
| 13 | Webhook pipeline | | Idempotency keys, transactional outbox, `SKIP LOCKED` workers, reconciliation | |
| 14 | UI rendering | Mini React: virtual DOM, reconciliation, hooks | React | |
| 15 | Deployment | EC2 provisioned by hand, `systemd` | RDS, IAM, security groups | |

## Running

Requires macOS or Linux with `cc`, `xxd`, `nc`, `curl`. Docker (PostgreSQL) from component 07, Node.js 24 from component 11.

```bash
cc -Wall app/01-bytes.c -o app/bytes
./app/bytes
```

## Ground rules

- All data and business scenarios are fictional. No code, data or information from any employer.
- No secrets in the repository: `.env` files and cloud credentials are git-ignored.
- Build outputs are not committed; they are always reproducible from source.
