// build-fail
// normalize-stderr-test: ".nll/" -> "/"

fn main() {
    rec(Empty);
}

struct Empty;

impl Iterator for Empty {
    type Item = ();
    fn next<'a>(&'a mut self) -> core::option::Option<()> {
        None
    }
}

fn identity<T>(x: T) -> T {
    x
}

fn rec<T>(mut it: T)
where
    T: Iterator,
{
    if () == () {
        T::count(it);
    } else {
        rec(identity(&mut it))
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

