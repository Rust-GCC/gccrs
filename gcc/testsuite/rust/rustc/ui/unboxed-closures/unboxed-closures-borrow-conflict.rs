// Test that an unboxed closure that mutates a free variable will
// cause borrow conflicts.



fn main() {
    let mut x = 0;
    let f = || x += 1;
    let _y = x; // { dg-error ".E0503." "" { target *-*-* } }
    f;
}

