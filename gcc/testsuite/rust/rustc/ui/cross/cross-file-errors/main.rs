#[macro_use]
mod underscore;

fn main() {
    underscore!();
// { dg-error "" "" { target *-*-* } .-1 }
}

