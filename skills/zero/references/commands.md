# Zero Command Reference For Agents

Run these commands from the repository root unless a task says otherwise.

## Focused Compiler Checks

```sh
bin/zero check --json <file-or-package>
bin/zero graph --json <file-or-package>
bin/zero size --json <file-or-package>
bin/zero explain <diagnostic-code>
bin/zero fix --plan --json <file-or-package>
```

## Native Compiler Build

```sh
make -C native/zero-c
```

## Validation

```sh
npm run docs:test
npm run conformance
npm run native:test
npm run command-contracts
```

Choose the narrowest validation command that covers the files and behavior you
changed. Use `command-contracts` for CLI JSON or help surface changes.
