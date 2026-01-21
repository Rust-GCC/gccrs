extern crate core;

fn assert_send<T:Send>() { }

fn test71<'a>() {
    assert_send::<*mut &'a isize>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
}

