fn main() {
    match "foo".to_string() {
        ['f', 'o', ..] => {}
// { dg-error ".E0529." "" { target *-*-* } .-1 }
        _ => { }
    };

    // Note that this one works with default binding modes.
    match &[0, 1, 2] {
        [..] => {}
    };

    match &[0, 1, 2] {
        &[..] => {} // ok
    };

    match [0, 1, 2] {
        [0] => {}, // { dg-error ".E0527." "" { target *-*-* } }

        [0, 1, x @ ..] => {
            let a: [_; 1] = x;
        }
        [0, 1, 2, 3, x @ ..] => {} // { dg-error ".E0528." "" { target *-*-* } }
    };

    match does_not_exist { // { dg-error ".E0425." "" { target *-*-* } }
        [] => {}
    };
}

fn another_fn_to_avoid_suppression() {
    match Default::default()
    {
        [] => {}  // { dg-error ".E0282." "" { target *-*-* } }
    };
}

