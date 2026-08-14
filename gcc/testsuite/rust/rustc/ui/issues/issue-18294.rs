fn main() {
    const X: u32 = 1;
    const Y: usize = unsafe { &X as *const u32 as usize }; // { dg-error ".E0658." "" { target *-*-* } }
    println!("{}", Y);
}

