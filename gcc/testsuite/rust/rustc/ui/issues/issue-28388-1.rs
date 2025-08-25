// Prefix in imports with empty braces should be resolved and checked privacy, stability, etc.

use foo::{}; // { dg-error ".E0432." "" { target *-*-* } }

fn main() {}

