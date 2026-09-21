# Evidence inventory assembled before report drafting

Audit date: 2026-09-21. Repository HEAD and local `origin/master`:
`de2588c201627ca5f95623d78ad1738104833834`. No fetch, checkout, compiler
modification, or compiler rebuild performed. See `refs.txt`, `branches.txt`,
`working-tree.txt`, and `branch-relationships.json`.

## Evidence handling

- Repository evidence is source inspection plus tracked tests and history, not a
  claim that the complete DejaGNU suite was run.
- Exploratory compilations used the existing adjacent `gccrs-build/gcc/crab1`.
  Its hash is in `binary-provenance.txt`; its exact source revision was not
  established. Results corroborate mechanisms but cannot certify branch status.
- Original commands and outputs are preserved in JSON and `.log` files. Some
  command paths refer to the original `/tmp/gccrs-compat-audit` scratch tree.
  Probe sources and output logs are copied here; binaries and assembly are not.
- Linux source: 451 files downloaded from commit
  `93f51579e7df248780214094418f205253383cc5` (7.3-rc4); source hashes in
  `external-source-sha256.json`. Downloaded source remains in
  `/tmp/gccrs-compat-audit/linux`. Rust feature tables and release notes are also
  retained in that scratch directory. External sources were read, not executed.

## Cross-checked claim inventory

| Claim | Primary evidence | Cross-check / qualification |
| --- | --- | --- |
| Supplied repository core log is not the older reproducer log | `core-summary.json`, `core-diagnostics.tsv` | 3,050 lines; 902 errors plus one ICE; 211 warnings. README's 268-error count and dereference ICE are stale for this artifact. |
| Log aborts while visiting `cmp` | log line 3006; `resolve_fn_trait_call` in `rust-hir-type-check-expr.cc` | Assertion compares an existing resolved definition with the new callable-trait candidate. No claim of independent root cause. |
| Intrinsic support exists but core's inventory is incomplete | `rust-hir-type-check-intrinsic.cc`, `rust-compile-intrinsic.cc`, `rust-intrinsic-handlers.cc`, `rust-builtins.cc` | 107 distinct rejected names in log, including old atomic spelling mismatches and genuinely absent operations. |
| GAT semantics exist | `253edd7c2fc`; `compile/gat1..8.rs`; `execute/torture/gat1.rs`; projection substitution | Sample GAT execution succeeds. Lifetime GAT test is largely declarative; not full lending-iterator certification. |
| Const-generic semantics and ADT values exist | `rust-tyty.h`, `rust-unify.cc`, `rust-tyty-subst.cc`; const-generics compile/execute tests | Struct-value impl selection and enum-value selection execute; distinct enum values reject; `K.n` fails while `let k: Key = K; k.n` executes. No full ConstParamTy contract established. |
| Legacy attribute description overstates exact implementation | `36860cbddb7`; `Function::get_legacy_const_generic_indexes` | Reads only `RUSTC_ARGS_REQUIRED_CONST`. Actual legacy spelling fails probe; ordinary required-const argument fails too. Generic shim succeeds. |
| APIT and ordinary RPIT have codegen | desugar APIT, lowering, TyTy; execute tests | All seven APIT/RPIT sample programs execute; RPIT in trait declaration rejects. |
| Unsizing / boxed dyn dispatch exists | `rust-coercion.cc`, DST backend, `8965fd12996` | Three representative execution tests succeed. Dynamic drop glue remains null in vtable construction. |
| Trait/method failures are not absent traits or closures | dot operator, autoderef, projection solver; reduction/control pairs | Four ordinary closure tests execute. Complex iterator controls differ between available binary and branch README; do not transfer results between revisions. |
| Metadata is an AST-text prototype | metadata exporter and session import; extern-crate lowering/typechecking | Three existing two-crate pairs compile, link, execute. No typed HIR/TyTy serialization located. |
| Exported macro round trip is broken in a concrete case | AST whole-crate dump plus exported-macro append | Tiny macro import fails with duplicate definition; emitted `.rox` retained in `cross-crate/`. Do not claim all macros are stripped. |
| Proc macro implementation is real but Linux compatibility unverified | `rust-proc-macro.cc`, expansion callbacks, `libgrust/libproc_macro*` | gccrs-specific dynamic-loader ABI; negative declaration tests alone do not validate Linux host macro crate graph. |
| Debug info exists | `gcc/testsuite/rust/debug/` | DWARF language/type/section tests; no kernel BTF/gendwarfksyms validation. |
| Borrow/drop semantics incomplete | optional borrowcheck, BIR visitors, drop backend | Scope-drop machinery exists; closure/match/unsafe borrow paths and dynamic drop glue remain incomplete. |
| Formatting is partial, not absent | `rust-expand-format-args.cc`, builtins handler | AST expansion exists; HIR FormatArgs stub is not the only path. Named/explicit positions unsupported; Debug derive emits stub. |
| Current libcore files are stock 1.49 source | import commit `ee87c5df0ac`; byte comparison to adjacent extracted Rust 1.49 source | All `.rs` files under core/src match; provenance of adjacent extraction is not independently authenticated. |
| Core harness is not a full compilation success claim | `gcc/testsuite/rust/core/core.exp`, session compile stage checks | `compile-until=lowering` returns before AST-to-HIR lowering in this checkout. |

## Branch ledger

- Both `phil/libcore-dev` and `phil/libcore-dev-2` fork at `e0a7a91b661`.
  Four and eleven branch-only commits respectively; both miss the three newer
  master commits. `git cherry` reports no patch-equivalent commits in master
  for these tip-only series.
- `libcore-dev-2.diff` preserves the complete current development series.
  `d446da1ebae`, `d9eece4a6e8`, `e4c2817797b`, `7a9ef57db8b` improve bound
  scoping/substitution/impl validation. `8095b924234` revises the probe cache.
- `7a181086031` adds associated-type-default syntax and HIR storage; no
  typechecker consumption found. Its test explicitly supplies the impl type.
  Its comment about core arithmetic traits using `type Output = Self` is not
  correct for the inspected 1.49 source (generic `Rhs = Self` is different).
- `e1d8bc0913e` adds pattern unification. `1748abcf8fc` contains a lifetime
  stack fix AND a commented-out dereference assertion; the latter is not a
  semantic solution. The README's description of an uncommitted lifetime fix
  is stale; the separate forward-lifetime regression remains untracked.
- Reflog documents libcore development from August 23 through September 21.
  Seven exact patch-ID matches are in `rebase-patch-comparisons.json`, including
  five historical development patches now in master. Other matching subjects
  with changed patches are reported as successors, not identical changes.
- Historical `dont commit` / WIP patches relax checks or add recursion guards;
  no evidence that all were accepted fixes. `origin/core/dev` is an older 2025
  expansion/prelude experiment and includes an explicit assembly stub.
- `origin/derive-coerce-pointee` and `origin/derive-coerce-pointee-base-impl`
  both explicitly return no generated implementation. Framework only.
- `origin/recognize-used-attr` contains `a4e373675aa` and `01c3963e310`:
  attribute recognition plus GCC preserve/retain handling and assembly tests.
  Branch-only implementation; full retention semantics still need review.

## Linux requirements ledger

- Pinned `kernel/lib.rs`: arbitrary_self_types, derive_coerce_pointee,
  used_with_arg; transitional stabilized unsigned_is_multiple_of and
  generic_arg_infer; conditional file_with_nul.
- `bindings` / `uapi`: cfi_encoding. gccrs explicitly ignores it while waiting
  for GCC KCFI support. compiler_builtins internal marker also used.
- Host macros: extract_if and conditional proc_macro_span; dependencies include
  proc-macro2, quote, syn, pin-init-internal and zerocopy-derive.
- Kernel generic const arguments found are scalar forms; no adt_const_params
  gate or ConstParamTy use found in the downloaded Rust subtree. Associated
  constants with struct/enum types are not ADT generic parameters.
- Stable language use includes GAT Borrowed<'a>, impl Trait, raw references,
  C literals, offset_of!, generics, closures, Drop, Pin and unsafe C FFI.
- Kbuild compiles core from the selected rustc sysroot, with no_fp_fmt_parse;
  1.85 baseline, 1.95 powerpc, 1.96 s390 in pinned minimum-version script.
  Core edition 2024 for rustc >=1.87, otherwise 2021. gccrs accepts editions
  2015/2018/2021 only. A 1.49-core success is not a modern Linux-core success.
- Linux Kconfig excludes RANDSTRUCT; requires gendwarfksyms for Rust with
  MODVERSIONS, constrains BTF/LTO, and conditionally requires CFI. These are
  configuration/toolchain issues, not language features.

## Validation limits and follow-up

24 selected repository execute tests compiled, linked and returned zero with
the available compiler; five required correcting this audit's host-link command
to `gcc -no-pie`. Three positive ADT-const research probes also executed.
Negative and incomplete-feature probes, metadata probes, and 17 reduction/control
compilations are recorded separately. This is not a complete testsuite run or
independent confirmation of all branch README results. No complete new core or
kernel build was attempted, and no percentage-complete claim is defensible.
