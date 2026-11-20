// compile-flags:--extern extern_prelude
// aux-build:extern-prelude.rs

// Extern prelude names are not available by absolute paths

fn main() {
    use extern_prelude::S; // { dg-error ".E0432." "" { target *-*-* } }
    let s = ::extern_prelude::S; // { dg-error ".E0433." "" { target *-*-* } }
}

