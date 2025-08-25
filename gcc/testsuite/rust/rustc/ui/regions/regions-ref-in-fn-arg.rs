#![feature(box_patterns)]
#![feature(box_syntax)]

fn arg_item(box ref x: Box<isize>) -> &'static isize {
    x // { dg-error ".E0515." "" { target *-*-* } }
}

fn with<R, F>(f: F) -> R where F: FnOnce(Box<isize>) -> R { f(box 3) }

fn arg_closure() -> &'static isize {
    with(|box ref x| x) // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {}

