fn main() {
    let _ = vec![1, 2, 3].into_iter().collect::Vec<_>();
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = vec![1, 2, 3].into_iter().collect::Vec<_>>>>();
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = vec![1, 2, 3].into_iter().collect::Vec<_>>>();
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = vec![1, 2, 3].into_iter().collect::Vec<_>>();
// { dg-error "" "" { target *-*-* } .-1 }
}

