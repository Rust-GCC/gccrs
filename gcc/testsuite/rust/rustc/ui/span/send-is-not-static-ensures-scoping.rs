struct Guard<'a> {
    f: Box<dyn Fn() + Send + 'a>,
}

fn scoped<'a, F: Fn() + Send + 'a>(f: F) -> Guard<'a> {
    Guard { f: Box::new(f) }
}

impl<'a> Guard<'a> {
    fn join(self) {}
}

fn main() {
    let bad = {
        let x = 1;
        let y = &x;
// { dg-error ".E0597." "" { target *-*-* } .-1 }

        scoped(|| {
            let _z = y;
// { dg-error ".E0597." "" { target *-*-* } .-1 }
        })
    };

    bad.join();
}

