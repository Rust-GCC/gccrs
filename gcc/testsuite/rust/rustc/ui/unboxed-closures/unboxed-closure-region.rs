// Test that an unboxed closure that captures a free variable by
// reference cannot escape the region of that variable.


fn main() {
    let _f = {
        let x = 0;
        || x // { dg-error ".E0597." "" { target *-*-* } }
    };
    _f;
}

