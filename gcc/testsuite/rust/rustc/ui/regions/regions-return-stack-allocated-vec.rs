// Test that we cannot return a stack allocated slice

fn function(x: isize) -> &'static [isize] {
    &[x] // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {
    let x = function(1);
    let y = x[0];
}

