// Check that you can't dereference raw pointers in constants.

fn main() {
    static C: u64 = unsafe {*(0xdeadbeef as *const u64)};
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    println!("{}", C);
}

