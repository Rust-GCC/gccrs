// Check that false Copy bounds don't leak
#![feature(trivial_bounds)]

fn copy_out_string(t: &String) -> String where String: Copy {
    *t
}

fn move_out_string(t: &String) -> String {
    *t // { dg-error ".E0507." "" { target *-*-* } }
}

fn main() {}

