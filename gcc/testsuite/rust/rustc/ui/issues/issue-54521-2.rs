// run-rustfix

// This test checks that the following error is emitted and the suggestion works:
//
// ```
// let _ = Vec::<usize>>>::new();
//                     ^^ help: remove extra angle brackets
// ```

fn main() {
    let _ = Vec::<usize>>>>>::new();
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = Vec::<usize>>>>::new();
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = Vec::<usize>>>::new();
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = Vec::<usize>>::new();
// { dg-error "" "" { target *-*-* } .-1 }
}

