fn ignore<T>(t: T) {}

fn nested<'x>(x: &'x isize) {
    let y = 3;
    let mut ay = &y; // { dg-error ".E0495." "" { target *-*-* } }

    ignore::<Box<dyn for<'z> FnMut(&'z isize)>>(Box::new(|z| {
        ay = x;
        ay = &y;
        ay = z;
    }));

    ignore::< Box<dyn for<'z> FnMut(&'z isize) -> &'z isize>>(Box::new(|z| {
        if false { return x; } // { dg-error ".E0312." "" { target *-*-* } }
        if false { return ay; }
        return z;
    }));
}

fn main() {}

