"use client";

import { useState } from "react";

const STEPS = [
  {
    command: "zero init",
    output: 'Created zero.toml\nCreated zero.graph  graph:e3b0c442\nInitialized package "hello"',
  },
  {
    command: 'zero patch --op addMain --op \'addCheckWrite fn="main" text="hello from zero\\n"\'',
    output: "Patched zero.graph\n  graph hash  graph:a7f7e689\n  symbols     main",
  },
  {
    command: "zero run",
    output: "hello from zero",
  },
];

function ChevronIcon({ open }: { open: boolean }) {
  return (
    <svg
      width="12"
      height="12"
      viewBox="0 0 16 16"
      fill="none"
      className={`shrink-0 text-muted transition-transform duration-200 ${open ? "rotate-90" : ""}`}
    >
      <path
        d="M6 4l4 4-4 4"
        stroke="currentColor"
        strokeWidth="1.75"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
    </svg>
  );
}

export function ChatToolRuns() {
  const [open, setOpen] = useState<number[]>([]);

  const toggle = (i: number) =>
    setOpen((prev) => (prev.includes(i) ? prev.filter((n) => n !== i) : [...prev, i]));

  return (
    <div className="flex flex-col gap-2">
      {STEPS.map((step, i) => {
        const isOpen = open.includes(i);
        return (
          <div
            key={step.command}
            className="overflow-hidden rounded-lg border border-border bg-surface"
          >
            <button
              type="button"
              onClick={() => toggle(i)}
              aria-expanded={isOpen}
              className="flex w-full cursor-pointer items-center gap-2 px-3 py-2 font-mono text-[0.78125rem] leading-relaxed text-muted transition-colors hover:bg-surface-muted"
            >
              <ChevronIcon open={isOpen} />
              <span
                className={`min-w-0 flex-1 text-left text-fg/80 ${
                  isOpen ? "whitespace-pre-wrap break-all" : "truncate"
                }`}
              >
                {step.command}
              </span>
            </button>
            {isOpen && (
              <pre className="m-0 overflow-x-auto border-t border-border/50 bg-bg px-3 py-2.5 text-[0.78125rem] leading-relaxed text-fg/70">
                {step.output}
              </pre>
            )}
          </div>
        );
      })}
    </div>
  );
}
