// { dg-do compile }
// { dg-options "-frust-incomplete-and-experimental-compiler-do-not-use" }
// { dg-prune-output "failed to locate crate" }
// { dg-prune-output "unknown crate" }

// ICE regression test for canonical path lookup
// Inline items inside const generic block expressions
// must not require canonical paths.

struct Expr<const N: u32>;

trait Trait0 {
    fn required(_: Expr<{
        struct Type;
        0
    }>);
}

fn main() {}
