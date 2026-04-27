// run-pass

// We shouldn't need to rebind a moved upvar as mut if it's already
// marked as mut

pub fn main() {
    let mut x = 1;
    let _thunk = Box::new(move|| { x = 2; });
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

