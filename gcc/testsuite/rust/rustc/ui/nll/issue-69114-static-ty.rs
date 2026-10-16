// Check that borrowck ensures that `static` items have the expected type.

static FOO: &'static (dyn Fn(&'static u8) + Send + Sync) = &drop;

fn main() {
    let n = 42;
    FOO(&n);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
}

