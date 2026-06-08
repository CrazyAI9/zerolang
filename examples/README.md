# Zero Examples

Use these examples as the hands-on path through Zero. Start at the top, run `bin/zero check`, and open the matching docs article when you want an explanation.

## Profile And Size Checks

Use `hello.graph` and `fixed-vec.graph` to compare profile contracts and size metadata:

```sh
bin/zero build --json --profile debug --target linux-musl-x64 examples/hello.graph --out .zero/out/hello-debug
bin/zero build --json --profile fast --target linux-musl-x64 examples/hello.graph --out .zero/out/hello-fast
bin/zero build --json --profile small --target linux-musl-x64 examples/hello.graph --out .zero/out/hello-small
bin/zero size --json --profile tiny --target linux-musl-x64 examples/fixed-vec.graph
```

Build JSON reports `profileSemantics` and `profileBudget`. Size JSON adds `sizeBreakdown`, `retentionReasons`, and `optimizationHints`.

## First Programs

| Example | What it teaches | Try it |
| --- | --- | --- |
| `hello.0` | `pub fn main`, `World`, `check`, stdout | `bin/zero check examples/hello.graph` |
| `hello-let.0` | immutable `let` bindings | `bin/zero check examples/hello-let.graph` |
| `add.0` | helper functions, `return`, `if` / `else` | `bin/zero build --emit exe --target linux-musl-x64 examples/add.graph --out .zero/out/add` |
| `direct-u8-helper-call.0` | direct backend signed LEB literals, byte arrays, and helper calls | `bin/zero check examples/direct-u8-helper-call.graph` |
| `direct-array-bounds-trap.0` | direct backend stack-memory bounds traps | `bin/zero check examples/direct-array-bounds-trap.graph` |
| `direct-string-len.0` | direct backend string literal length for compiler token scans | `bin/zero check examples/direct-string-len.graph` |
| `direct-string-literal.0` | direct backend readonly string data segments and byte loads | `bin/zero check examples/direct-string-literal.graph` |
| `direct-span-read.0` | direct backend readonly string slices as byte-span views | `bin/zero check examples/direct-span-read.graph` |
| `direct-string-eql.0` | direct backend byte-span equality over readonly string views | `bin/zero check examples/direct-string-eql.graph` |
| `direct-byte-view-locals.0` | direct backend local `String`/`Span<u8>` pointer-length byte views | `bin/zero check examples/direct-byte-view-locals.graph` |
| `direct-mutspan-len.0` | direct backend local `MutSpan<u8>` pointer-length byte views | `bin/zero check examples/direct-mutspan-len.graph` |
| `direct-byte-copy-fill.0` | direct backend mutable byte copy/fill over fixed buffers | `bin/zero check examples/direct-byte-copy-fill.graph` |
| `direct-alloc-bump.0` | direct backend explicit `FixedBufAlloc` bump allocation over caller storage | `bin/zero check examples/direct-alloc-bump.graph` |
| `direct-alloc-overflow.0` | direct backend fixed-buffer allocation overflow returns `Maybe.none` without hidden heap growth | `bin/zero check examples/direct-alloc-overflow.graph` |
| `direct-token-shape.0` | direct backend stack layout for type literals, field loads, defaults, and field stores | `bin/zero check examples/direct-token-shape.graph` |
| `direct-enum-match.0` | direct backend compact enum cases and exhaustive match branches without matcher tables | `bin/zero check examples/direct-enum-match.graph` |
| `direct-raises-basic.0` | direct backend packed error-result propagation for `raise` and `check` | `bin/zero check examples/direct-raises-basic.graph` |
| `direct-rescue-basic.0` | direct backend local `rescue` fallback over a raised error | `bin/zero check examples/direct-rescue-basic.graph` |
| `direct-byte-buf.0` | direct backend monomorphic byte buffer push, length, capacity, and overflow checks | `bin/zero check examples/direct-byte-buf.graph` |
| `direct-generic-identity.0` | direct backend explicit generic function specialization without runtime metadata | `bin/zero check examples/direct-generic-identity.graph` |
| `direct-generic-fixedbuf.0` | direct backend generic storage type with concrete type and static value arguments | `bin/zero check examples/direct-generic-fixedbuf.graph` |
| `direct-generic-vec.0` | direct backend generic fixed-capacity vector layout for byte, token, and AST-node element kinds | `bin/zero check examples/direct-generic-vec.graph` |
| `direct-i64-return.0` | direct ELF64 object backend support for i64/u64 values | `bin/zero build --emit obj --target linux-musl-x64 examples/direct-i64-return.graph --out .zero/out/direct-i64-return.o` |
| `direct-byte-view-reloc.0` | direct ELF64 readonly byte-view relocations for string-backed span locals | `bin/zero build --emit obj --target linux-musl-x64 examples/direct-byte-view-reloc.graph --out .zero/out/direct-byte-view-reloc.o` |
| `functions.0` | calling functions and ignoring return values | `bin/zero check examples/functions.graph` |
| `branch.0` | booleans and branches | `bin/zero check examples/branch.graph` |
| `countdown.0` | `while` loop syntax | `bin/zero check examples/countdown.graph` |

## Data And Types

| Example | What it teaches | Try it |
| --- | --- | --- |
| `point.0` | `type`, type literals, field access | `bin/zero check examples/point.graph` |
| `result-choice.0` | `enum`, payload `choice`, exhaustive `match`, payload binding | `bin/zero check examples/result-choice.graph` |
| `primitive-language-gaps.0` | fixed arrays, `var`, assignment | `bin/zero check examples/primitive-language-gaps.graph` |
| `memory-primitives.0` | `Span`, `Maybe`, references, allocator vocabulary, `std.mem` spans | `bin/zero check examples/memory-primitives.graph` |
| `allocator-collections.0` | `NullAlloc`, explicit allocator handles, fixed-buffer allocation, `Vec`, and fixed-storage collections without a global heap | `bin/zero check examples/allocator-collections.graph && bin/zero mem --json examples/allocator-collections.graph` |
| `const-arithmetic.0` | top-level deterministic `const` values and arithmetic | `bin/zero check examples/const-arithmetic.graph` |
| `compile-time-v1.0` | bounded `meta`, target/type reflection facts, Bool and enum static values, and compile-time JSON metadata | `bin/zero check --json examples/compile-time-v1.graph` |
| `generic-pair.0` | multi-parameter generic type storage and field access | `bin/zero run examples/generic-pair.graph` |
| `static-value-params.0` | integer static value parameters and fixed-capacity generic storage | `bin/zero check examples/static-value-params.graph` |
| `fixed-vec.0` | field defaults, constructor-style type methods, receiver calls, `Self`, and static capacity | `bin/zero check examples/fixed-vec.graph` |
| `type-alias.0` | `alias Name ExistingType` as compile-time spelling | `bin/zero check examples/type-alias.graph` |
| `static-method.0` | static type method namespace calls without dispatch | `bin/zero check examples/static-method.graph` |
| `static-interface.0` | static interface constraints over generic functions with direct calls | `bin/zero check examples/static-interface.graph` |
| `fallibility.0` | `raise`, `check`, and explicit `raises [...]` error sets | `bin/zero build --emit exe --target linux-musl-x64 examples/fallibility.graph --out .zero/out/fallibility && ./.zero/out/fallibility` |
| `ownership-cleanup.0` | `owned<T>` cleanup, canonical `drop`, and `defer` at lexical scope exit | `bin/zero build --emit exe --target linux-musl-x64 examples/ownership-cleanup.graph --out .zero/out/ownership-cleanup && ./.zero/out/ownership-cleanup` |

## Standard Library

| Example | What it teaches | Try it |
| --- | --- | --- |
| `std-math.0` | pure fixed-width integer helpers and number-theory routines | `bin/zero check examples/std-math.graph` |
| `codec-varint.0` | `use std.codec`, varint length, CRC-32 | `bin/zero check examples/codec-varint.graph` |
| `parse-cursor.0` | `use std.parse`, scanner predicates | `bin/zero check examples/parse-cursor.graph` |
| `std-path-io.0` | `std.path` fixed-buffer path helpers and `std.io` caller-owned buffers | `bin/zero check examples/std-path-io.graph` |
| `grep-scan.0` | Line-oriented scanning with `std.io` and `std.str` | `bin/zero check examples/grep-scan.graph` |
| `std-str.0` | allocation-free byte-string helpers over spans and caller-owned storage | `bin/zero check examples/std-str.graph` |
| `std-testing-log.0` | `std.testing` expectations and explicit-buffer `std.log` JSON Lines output | `bin/zero test examples/std-testing-log.graph && bin/zero check examples/std-testing-log.graph` |
| `std-text-format-parse.0` | ASCII helpers, runtime parsing, caller-buffer formatting, and UTF-8 validation | `bin/zero check examples/std-text-format-parse.graph` |
| `std-data-formats.0` | codec encode/decode, JSON lookup/writing, and URL query helpers over caller-owned buffers | `bin/zero check examples/std-data-formats.graph` |
| `std-json-bytes.0` | byte-span JSON validation, parsing, and token streaming | `bin/zero run --out /tmp/zero-json-bytes examples/std-json-bytes.graph` |
| `std-http-json.0` | hosted HTTP request envelope into caller storage, then byte-span JSON parsing | `bin/zero check examples/std-http-json.graph` |
| `std-http-request.0` | hosted HTTP request envelope with custom method, headers, and body | `bin/zero check examples/std-http-request.graph` |
| `std-http-headers.0` | hosted HTTP request envelope, response buffer, and header-value lookup | `bin/zero check examples/std-http-headers.graph` |
| `json-api-client.0` | hosted JSON API client with request-envelope writing and response-body parsing | `bin/zero check examples/json-api-client.graph` |
| `json-api-router.0` | dependency-free JSON API request parsing and response-envelope writing | `bin/zero check examples/json-api-router.graph` |
| `crm-api/` | binary graph-first CRM API router with account/contact/deal CRUD, activity, health, and search routes | `bin/zero check examples/crm-api` |
| `binary-graph-store/` | graph-first package with binary `zero.graph` storage and a synced `.0` projection | `bin/zero status examples/binary-graph-store && bin/zero run examples/binary-graph-store` |
| `std-platform.0` | `std.time`, `std.rand`, `std.proc`, and `std.crypto` capability-shaped helpers | `bin/zero check examples/std-platform.graph` |
| `cli-file.0` | `std.args`, `std.env`, byte-span file writes, stderr/stdout | `bin/zero check examples/cli-file.graph` |
| `cli-config.0` | `std.cli`, `std.env`, and JSON output checks | `bin/zero check examples/cli-config.graph` |
| `file-copy.0` | `Fs`, explicit scratch storage, and hosted file copy | `bin/zero check examples/file-copy.graph` |
| `zero-hash/` | File checksum CLI with args, fixed buffers, `readAll`, and CRC-32 bytes | `bin/zero check examples/zero-hash` |

## Native Workflow Coverage

These examples are the small native workflow set used by docs and tests:

| Surface | Example | Try it |
| --- | --- | --- |
| arguments and environment | `cli-file.0` | `ZERO_CLI_FILE_MODE=verbose bin/zero check examples/cli-file.graph` |
| filesystem resources | `zero-hash/` | `bin/zero check examples/zero-hash` |
| deterministic exit status | `direct-exe-return.0` | `bin/zero build --emit exe --target linux-musl-x64 examples/direct-exe-return.graph --out .zero/out/direct-exe-return` |
| unhandled error exit path | `direct-unhandled-error-exit.0` | `bin/zero check examples/direct-unhandled-error-exit.graph` |

## Interop And Packages

| Example | What it teaches | Try it |
| --- | --- | --- |
| `config-shape.0` | `extern c`, `extern type`, C-shaped data | `bin/zero check examples/config-shape.graph` |
| `systems-package/` | `zero.toml`, multiple source files, `defer`, std helpers | `bin/zero check examples/systems-package` |
| `readall-cli/` | package-local imports, named errors, `std.fs.readAll`, explicit fixed-buffer allocation | `bin/zero check examples/readall-cli` |
| `batch3-cli/` | module graph metadata, local `rescue`, path helpers, named fs errors, explicit allocation | `bin/zero check examples/batch3-cli` |
| `resource-cli/` | args/env fallback, path joins, `std.mem.copy`/`fill`, named-error owned-file resources | `bin/zero check examples/resource-cli` |
| `memory-package/` | target-neutral package imports and byte-span helper checks without hosted file I/O | `bin/zero build --target linux-musl-x64 examples/memory-package --out .zero/out/memory-package` |
| `crm-api/` | binary repository graph multi-module HTTP API package with 10+ CRM routes over request envelopes | `bin/zero build --emit exe --profile debug --out /tmp/zero-crm-api examples/crm-api` |
| `binary-graph-store/` | binary repository graph store loaded through normal package commands; `.0` is the human-readable projection | `bin/zero check examples/binary-graph-store && bin/zero test examples/binary-graph-store` |
| `direct-package-call-order/` | direct backend package merge order and cross-module helper calls | `bin/zero check examples/direct-package-call-order` |
| `error-tour/` | copyable failing commands and repaired fixtures for common diagnostics | `bin/zero explain TAR002` |

## Build A Runnable Program

Most examples are designed for `check`. To build and run an executable, use a CLI entry point:

```sh
bin/zero dev --json --target linux-musl-x64 examples/add.graph
bin/zero build --emit exe --target linux-musl-x64 examples/add.graph --out .zero/out/add
bin/zero ship --target linux-musl-x64 examples/add.graph --out .zero/ship/add
./.zero/out/add
```

Expected output:

```text
math works
```

The larger CLI path is `examples/zero-hash/`. It seeds a small file, reads it through a fixed-buffer allocator, computes CRC-32 over the read bytes without heap allocation, and prints:

```text
zero-hash ok
```

For target-neutral direct cross builds, use `examples/memory-package/`. It exercises a multi-file package and byte-span helper checks without `std.fs`, so it can be checked and built against non-host targets:

```sh
bin/zero build --target linux-musl-x64 examples/memory-package --out .zero/out/memory-package
```

## More Guidance

Run the docs site with:

```sh
pnpm run docs:dev
```

Start with Getting Started, then Learn Zero.
