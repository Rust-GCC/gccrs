pub static X: usize = 1;

fn main() {
    match 1 {
        self::X => { },
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        _       => { },
    }
}

