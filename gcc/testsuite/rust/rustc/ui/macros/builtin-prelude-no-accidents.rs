// Names of public modules in libstd and libcore don't accidentally get into prelude
// because macros with the same names are in prelude.

fn main() {
    env::current_dir; // { dg-error ".E0433." "" { target *-*-* } }
    type A = panic::PanicInfo; // { dg-error ".E0433." "" { target *-*-* } }
    type B = vec::Vec<u8>; // { dg-error ".E0433." "" { target *-*-* } }
}

