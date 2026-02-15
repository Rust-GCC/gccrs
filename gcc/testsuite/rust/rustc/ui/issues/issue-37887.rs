fn main() {
    extern crate libc; // { dg-error ".E0658." "" { target *-*-* } }
    use libc::*; // { dg-error ".E0432." "" { target *-*-* } }
}

