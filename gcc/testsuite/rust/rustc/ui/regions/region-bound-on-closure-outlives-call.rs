fn call_rec<F>(mut f: F) -> usize where F: FnMut(usize) -> usize {
// { dg-warning "" "" { target *-*-* } .-1 }
    (|x| f(x))(call_rec(f)) // { dg-error ".E0505." "" { target *-*-* } }
}

fn main() {}

