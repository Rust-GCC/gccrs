const SOME_CONST : i32 = 12;

fn some_other_func() {}

fn some_function() {
    SOME_CONST = 14; // { dg-error ".E0070." "" { target *-*-* } }
    1 = 3; // { dg-error ".E0070." "" { target *-*-* } }
    some_other_func() = 4; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {
}

