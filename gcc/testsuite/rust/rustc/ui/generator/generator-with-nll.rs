#![feature(generators)]

fn main() {
    || {
        // The reference in `_a` is a Legal with NLL since it ends before the yield
        let _a = &mut true;
        let b = &mut true;
// { dg-error ".E0626." "" { target *-*-* } .-1 }
        yield ();
        println!("{}", b);
    };
}

