#![feature(never_type)]

fn main() {
    // The `if false` expressions are simply to
    // make sure we don't avoid checking everything
    // simply because a few expressions are unreachable.

    if false {
        let _: ! = { // { dg-error ".E0308." "" { target *-*-* } }
            'a: while break 'a {};
        };
    }

    if false {
        let _: ! = {
            while false { // { dg-error ".E0308." "" { target *-*-* } }
                break
            }
        };
    }

    if false {
        let _: ! = {
            while false { // { dg-error ".E0308." "" { target *-*-* } }
                return
            }
        };
    }
}

