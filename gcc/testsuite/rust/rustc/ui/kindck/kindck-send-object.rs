// Test which of the builtin types are considered sendable. The tests
// in this file all test the "kind" violates detected during kindck.
// See all `regions-bounded-by-send.rs`

fn assert_send<T:Send>() { }
trait Dummy { }
trait Message : Send { }

// careful with object types, who knows what they close over...

fn object_ref_with_static_bound_not_ok() {
    assert_send::<&'static (dyn Dummy + 'static)>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn box_object_with_no_bound_not_ok<'a>() {
    assert_send::<Box<dyn Dummy>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn object_with_send_bound_ok() {
    assert_send::<&'static (dyn Dummy + Sync)>();
    assert_send::<Box<dyn Dummy + Send>>();
}

fn main() { }

