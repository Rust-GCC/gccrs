// Continue kindck-send-object1.rs.

fn assert_send<T:Send>() { }
trait Dummy { }

fn test50() {
    assert_send::<&'static dyn Dummy>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn test53() {
    assert_send::<Box<dyn Dummy>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

// ...unless they are properly bounded
fn test60() {
    assert_send::<&'static (dyn Dummy + Sync)>();
}
fn test61() {
    assert_send::<Box<dyn Dummy + Send>>();
}

fn main() { }

