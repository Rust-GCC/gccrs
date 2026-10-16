fn vindictive() -> bool { true }

fn perfidy() -> (i32, i32) {
    vindictive() // { dg-error ".E0618." "" { target *-*-* } }
    (1, 2)
}

fn main() {}

