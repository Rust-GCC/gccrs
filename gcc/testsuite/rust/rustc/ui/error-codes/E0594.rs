static NUM: i32 = 18;

fn main() {
    NUM = 20; // { dg-error ".E0594." "" { target *-*-* } }
}

