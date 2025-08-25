fn id<T>(x: T) -> T { x }

fn f(_x: &isize) -> &isize {
    return &id(3); // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {
}

