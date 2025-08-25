// check-pass

// this tests the `unknown_lint` lint, especially the suggestions

// the suggestion only appears if a lint with the lowercase name exists
#[allow(FOO_BAR)]
// { dg-warning "" "" { target *-*-* } .-1 }

// the suggestion appears on all-uppercase names
#[warn(DEAD_CODE)]
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

// the suggestion appears also on mixed-case names
#[deny(Warnings)]
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

fn main() {
    unimplemented!();
}

