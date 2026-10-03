// Make sure extern types are !Sync and !Send.

#![feature(extern_types)]

extern {
    type A;
}

fn assert_sync<T: ?Sized + Sync>() { }
fn assert_send<T: ?Sized + Send>() { }

fn main() {
    assert_sync::<A>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    assert_send::<A>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

