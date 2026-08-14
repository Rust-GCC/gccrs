struct Slice<T> {
    data: *const T,
    len: usize,
}

fn main() {
    match () {
        Slice { data: data, len: len } => (),
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
        _ => unreachable!()
    }
}

