// Regression test for #66270, fixed by #66246

struct Bug {
    incorrect_field: 0,
// { dg-error "" "" { target *-*-* } .-1 }
}

struct Empty {}

fn main() {
    let Bug {
        any_field: Empty {},
    } = Bug {};
}

