// Test for ICE from issue 57989

fn f(x: &i32) {
    let g = &x;
    *x = 0;     // { dg-error ".E0506." "" { target *-*-* } }
// { dg-error ".E0506." "" { target *-*-* } .-2 }
    g;
}

fn main() {}

