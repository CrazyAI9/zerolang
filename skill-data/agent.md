---
name: agent
description: Graph-first agent workflow for making focused Zero changes with CLI feedback.
---

# Zero Agent Workflow

Use this when editing Zero code, examples, tests, docs, or a package. `zero.graph` is the package compiler input; `.0` files are the human-readable projection. Use JSON only when another tool must parse stable fields.

## Edit Through Patch

Anchored small edits win. Do not retype a function to change one line or rewrite a `.0` file for one declaration.

1. `--replace-in-fn`: Edit semantics on one function's canonical body text.

```sh
zero patch . --replace-in-fn handleLine --old 'limit + 1' --new 'limit + 2'
```

`--old` must match the text `zero view --fn <name>` prints exactly once; misses fail with the occurrence count.

2. `--replace-fn` with a heredoc for one whole body:

```sh
zero patch . --replace-fn greet --body-file - <<'EOF'
check world.out.write("hello agent\n")
EOF
```

3. Declaration work stays in ops, call sites updated for you:

```sh
zero patch . --op 'setConst name="limit" value="64"'
zero patch . --op 'addParamTo fn="scan" name="bias" type="i32" default="0"'  # updates every call site
zero patch . --op 'setReturnType fn="scan" type="i64"'
```

4. New helpers stay graph-native:

```text
zero-program-graph-patch v1
upsertFunction handle
fn handle(request: Span<u8>, response: MutSpan<u8>) -> Maybe<Span<u8>> {
    return null
}
end
```

Use `addReturnExpr fn="maybe" expr="null"` for non-identifier returns, `appendStmt fn="main" stmt="check std.http.listen(world, 3000_u16)"` for one statement, and `addTestBody name="api add" ... end` for a test block.

A successful patch prints `validated: check-equivalent`: it already validated and saved the graph. Go straight to `zero run . -- <args>` / `zero test`. Repeat `--op` to batch edits into one revalidation. For expression rewrites and node-handle micro-edits, see `zero skills get graph`.

Scoped reads; never read a whole `.0` file for one function:

- `zero view --fn <name>`: one function's source; misses fail with close matches.
- `zero view --fn <name> --around <text>`: only the enclosing block containing the text.
- `zero view --outline <module-or-file>`: signatures plus one-line docs, no bodies.

For a new agent-authored package: `zero init`, then `zero patch --op 'addMain'`.

## zero query

```text
zero query [--json] [--fn <name>] [--find <text>] [--refs <name>] [--calls <name>]
           [--node <id>] [--depth <n>] [--full] [--handles] [graph-input|name]
```

- bare name that is not an existing path: runs `--find` against the current package
- `zero query --fn <name> --handles`: patch handles for one function
- `--find <text>`: search names, ids, types, values, and node kinds; prints matches with spans
- `--calls <name>` / `--refs <name>`: resolved calls and semantic references
- `--node <id>`: one node's span, parents, and children; short handles resolve here too

Import/export, identity recovery, structural rewrites, and merge live in the `graph` topic. Direct `.0` edits are a last resort; never delete `zero.graph`.

## Verify Before Done

After a fix works on the changed path, exercise typical and boundary inputs. Zero inserts runtime checks, so a checked program can still trap on untested inputs.

```sh
zero run . -- <typical input>
zero run . -- <empty or boundary input>
zero test
```

If behavior changed, add or update a `test` block. On a diagnostic, run `zero explain <code>` before broad refactors.

## Rules

- Treat effects as capabilities, not ambient globals: `World`, `std.fs`, `std.args`, `std.env`.
- Use `Maybe<T>`, explicit `raises` / `raises [...]`, and `check` / `rescue` instead of hidden failure.
- Do not invent syntax or CLI fields; load `language` when unsure.
- Do not hand-write parsing or validation before checking the `stdlib` topic: it ships validators such as `std.time` (RFC 3339), `std.inet`, `std.regex`, and `std.unicode`. Fetch one module's signatures with `zero skills get stdlib --topic std.time`.
