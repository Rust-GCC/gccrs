#![deny(unreachable_patterns)]

fn main() {
    match "world" {
        "hello" => {}
        _ => {},
    }

    match "world" {
        ref _x if false => {}
        "hello" => {}
        "hello" => {} // { dg-error "" "" { target *-*-* } }
        _ => {},
    }
}

