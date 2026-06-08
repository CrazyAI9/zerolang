## Getting Started

Zero is the programming language for agents. The normal user workflow is an
agent conversation over a graph-first package, not hand-writing source text.
Ask the agent to inspect and patch `zero.graph`; export `.0` only when humans
need a readable projection for review, audit, or an occasional manual edit.

## Install The Compiler

```sh
curl -fsSL https://zerolang.ai/install.sh | bash
export PATH="$HOME/.zero/bin:$PATH"
zero --version
```

The installer downloads the latest matching binary from the GitHub release and
writes it to `$HOME/.zero/bin/zero`.

## Install The Skill

Install the Zerolang skill in your project. The skill teaches your agent the
local compiler surface and the graph-first workflow, so it knows to read and
write through the graph instead of hand-editing source:

```sh
npx skills add vercel-labs/zerolang
```

## Ask An Agent For Hello World

With the skill installed, just ask for what you want. A typical agent
conversation should look like this. Expand a command to see what it returns:

```json-render
{
  "messages": [
    {
      "role": "user",
      "text": "Create a hello world Zero program. Use the graph to read and write. .0 files are for humans only."
    },
    {
      "role": "skill",
      "name": "zerolang"
    },
    {
      "role": "assistant",
      "text": "I'll initialize a graph-first package, patch zero.graph, and run it. The patch command validates the graph and returns the new graph hash and symbols."
    },
    {
      "role": "tools",
      "calls": [
        {
          "command": "zero init",
          "output": "Created zero.toml\nCreated zero.graph  graph:e3b0c442\nInitialized package \"hello\""
        },
        {
          "command": "zero patch --op 'addMain' --op 'addCheckWrite fn=\"main\" text=\"hello from zero\\n\"'",
          "output": "Patched zero.graph\n  graph hash  graph:a7f7e689\n  symbols     main"
        },
        {
          "command": "zero run",
          "output": "hello from zero"
        }
      ]
    },
    {
      "role": "assistant",
      "text": "Done. zero.graph was patched successfully, the new graph hash is graph:a7f7e689 with symbol main, and zero run prints:"
    },
    {
      "role": "output",
      "text": "hello from zero"
    }
  ]
}
```

## What The Agent Created

The graph-first package has these important files:

```text
hello/
  zero.toml
  zero.graph
```

`zero.graph` is the repository graph store and the normal compiler input for
this package. Agents should usually inspect and patch that graph with
`zero query` and `zero patch`. Its default encoding is binary so the compiler
can load typed graph tables directly. Use `--format text` only when you
specifically want a readable repository store artifact for debugging. Binary
graph artifacts are also supported for explicit graph outputs. The standard
library already uses binary `std/*.graph` stores for compilation while keeping
`std/*.0` projections for human review.

`src/main.0` is the human-readable projection. It is deliberately readable and
bidirectional: humans can review it, and humans may edit it directly when that
is the right workflow. After a human edit, run `zero import .`
to refresh `zero.graph`. Agents should not normally hand-write `.0` files.

If you ask the agent to export a projection for human review, `src/main.0`
should look like this:

```zero
pub fn main(world: World) -> Void raises {
    check world.out.write("hello from zero\n")
}
```

That file is useful for human trust. The graph remains the normal agent
read/write surface, and most users should interact with it by asking an agent
to make a checked graph edit.

## Check, Run, And Build

From inside the graph-first package:

```sh
zero check
zero run
zero build --target linux-musl-x64 --out .zero/out/hello
```

Normal compiler commands compile from `zero.graph`. They report source
projection state, but they do not rewrite `.0` files. Export explicitly when
the human projection needs to be refreshed.

## Ask For The Next Change

Keep talking to the same agent. It uses `zero patch` operations, and for
behavior changes it runs the relevant command such as `zero test` or `zero run`:

```json-render
{
  "messages": [
    {
      "role": "user",
      "text": "Add a function add(x, y) that returns x + y, and add a test for it. Use the graph to read and write. Do not hand-edit .0; do not export a projection unless I ask."
    },
    {
      "role": "assistant",
      "text": "I'll add the function and a test with graph patches, then run the tests."
    },
    {
      "role": "tools",
      "calls": [
        {
          "command": "zero patch --op 'addFunction name=\"add\" ret=\"i32\"' --op 'addParam fn=\"add\" name=\"x\" type=\"i32\"' --op 'addParam fn=\"add\" name=\"y\" type=\"i32\"' --op 'addLetBinary fn=\"add\" name=\"sum\" type=\"i32\" operator=\"+\" left=\"x\" right=\"y\"' --op 'addReturnValue fn=\"add\" value=\"sum\" type=\"i32\"'",
          "output": "Patched zero.graph\n  graph hash  graph:5d9b2c10\n  symbols     add, main"
        },
        {
          "command": "zero test",
          "output": "running 1 test\n  test add_adds ... ok\n\n1 passed; 0 failed"
        }
      ]
    },
    {
      "role": "assistant",
      "text": "Done. add(x, y) is in the graph and its test passes. The new graph hash is graph:5d9b2c10 with symbols add and main."
    }
  ]
}
```

## Learn The Core Syntax

The examples are still useful for humans learning how Zero projections read.
Run the graph inputs, not the `.0` projection files:

```sh
zero check examples/hello.graph
zero check examples/hello-let.graph
zero check examples/functions.graph
zero check examples/branch.graph
zero check examples/point.graph
zero check examples/result-choice.graph
```

They cover:

- entry points and output
- `let` bindings
- functions and return values
- conditionals
- `type` data declarations
- `enum`, `choice`, and `match`

For agent-authored programs, use those files as projections and examples, not
as the default write path.

## Inspect A Package

Use graph query to inspect a package in slices:

```sh
zero query
zero query --fn main
zero query --find write
```

The manifest records package metadata; the checked-in `zero.graph` beside it is
the compiler input. The target `main` field points at the projection path used
for source maps and human import/export:

```toml
[package]
name = "hello"
version = "0.1.0"
license = "MIT"

[targets.cli]
kind = "exe"
main = "src/main.0"
```

After the first graph-backed build or run, Zero may also create
`.zero/cache/native/mir-*.zmir`. That file is not an authoring surface. It is a
derived final-MIR cache that the compiler memory-maps and verifies before
codegen so later graph builds can stay closer to the compiler.

## Next Steps

- Load `zero skills get agent`, `zero skills get graph`, and
  `zero skills get stdlib` before asking an agent for larger changes.
- Use the examples index to pick the next concept by projection.
- Use the CLI reference for `zero patch`, `zero query`, `zero import`, and
  `zero export`.
- Use Building From Source when you want to validate a local checkout.
