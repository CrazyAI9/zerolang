---
name: zero
description: Build, inspect, test, and repair Zero programs with the current compiler CLI, examples, docs, and conformance fixtures.
---

# Zero Skill

Use this skill when working in the Zero repository or helping an agent write,
inspect, test, document, or repair Zero programs.

## Operating Rules

- Treat Zero as a working experimental language for agents. Public docs should
  describe current behavior, not planned features or implementation history.
- Prefer small, direct changes that match the existing compiler, docs, and
  example style.
- Keep examples runnable and commands copyable from the repository root.
- Use direct compiler output and JSON command surfaces for agent-facing checks.
- Build the native compiler with `make -C native/zero-c` before relying on
  native command behavior.

## Common Commands

```sh
bin/zero check --json <file-or-package>
bin/zero graph --json <file-or-package>
bin/zero size --json <file-or-package>
bin/zero explain <diagnostic-code>
bin/zero fix --plan --json <file-or-package>
```

For broader validation, use the focused check that matches the change:

```sh
npm run docs:test
npm run conformance
npm run native:test
npm run command-contracts
```

## Workflow

1. Read the nearby compiler, docs, examples, or conformance fixture before
   editing.
2. Make the smallest change that updates behavior and the matching public
   surface.
3. Add or update a focused test when behavior changes.
4. Run `bin/zero` checks for the touched input and one broader validation
   command when the change affects shared CLI behavior.

## Repository Map

- `native/zero-c/`: native compiler implementation.
- `compiler-zero/`: Zero-authored compiler sources.
- `examples/`: small runnable programs and packages.
- `conformance/`: language and CLI fixtures.
- `docs-site/`: public documentation site.
- `scripts/`: validation and release support tooling.
