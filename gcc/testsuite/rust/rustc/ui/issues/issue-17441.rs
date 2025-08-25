fn main() {
    let _foo = &[1_usize, 2] as [usize];
// { dg-error ".E0620." "" { target *-*-* } .-1 }

    let _bar = Box::new(1_usize) as dyn std::fmt::Debug;
// { dg-error ".E0620." "" { target *-*-* } .-1 }

    let _baz = 1_usize as dyn std::fmt::Debug;
// { dg-error ".E0620." "" { target *-*-* } .-1 }

    let _quux = [1_usize, 2] as [usize];
// { dg-error ".E0620." "" { target *-*-* } .-1 }
}

