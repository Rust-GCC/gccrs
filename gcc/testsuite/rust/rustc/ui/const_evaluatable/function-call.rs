// check-pass

const fn foo<T>() -> usize {
    // We might instead branch on `std::mem::size_of::<*mut T>() < 8` here,
    // which would cause this function to fail on 32 bit systems.
    if false {
        std::mem::size_of::<T>()
    } else {
        8
    }
}

fn test<T>() {
    let _ = [0; foo::<T>()];
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {}

