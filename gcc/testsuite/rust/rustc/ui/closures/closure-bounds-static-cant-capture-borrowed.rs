fn bar<F>(blk: F) where F: FnOnce() + 'static {
}

fn foo(x: &()) {
    bar(|| {
// { dg-error ".E0621." "" { target *-*-* } .-1 }
        let _ = x;
    })
}

fn main() {
}

