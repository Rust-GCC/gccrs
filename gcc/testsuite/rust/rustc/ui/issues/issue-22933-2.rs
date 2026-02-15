enum Delicious {
    Pie      = 0x1,
    Apple    = 0x2,
    ApplePie = Delicious::Apple as isize | Delicious::PIE as isize,
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn main() {}

