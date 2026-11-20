fn main() {
    format!("{foo}");                // { dg-error "" "" { target *-*-* } }

    // panic! doesn't hit format_args! unless there are two or more arguments.
    panic!("{foo} {bar}", bar=1);    // { dg-error "" "" { target *-*-* } }
}

