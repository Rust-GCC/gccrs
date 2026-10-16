struct Slice<T> {
    data: *const T,
    len: usize,
}

fn main() {
    let Slice { data: data, len: len } = "foo";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

