fn ignore<F>(_f: F) where F: for<'z> FnOnce(&'z isize) -> &'z isize {}

fn nested() {
    let y = 3;
    ignore(
        |z| {
            if false { &y } else { z }
// { dg-error ".E0597." "" { target *-*-* } .-1 }
        });
}

fn main() {}

