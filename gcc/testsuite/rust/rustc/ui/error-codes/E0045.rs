extern "Rust" { fn foo(x: u8, ...); }   // { dg-error ".E0045." "" { target *-*-* } }

fn main() {
}

