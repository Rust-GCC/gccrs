pub enum T {
    T1(()),
    T2(())
}

pub enum V {
    V1(isize),
    V2(bool)
}

fn main() {
    match (T::T1(()), V::V2(true)) {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        (T::T1(()), V::V1(i)) => (),
        (T::T2(()), V::V2(b)) => ()
    }
}

