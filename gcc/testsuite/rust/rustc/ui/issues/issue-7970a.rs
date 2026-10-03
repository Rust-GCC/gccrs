macro_rules! one_arg_macro {
    ($fmt:expr) => (print!(concat!($fmt, "\n")));
}

fn main() {
    one_arg_macro!();
// { dg-error "" "" { target *-*-* } .-1 }
}

