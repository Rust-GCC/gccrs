struct A;

impl A {
// { dg-note "" "" { target *-*-* } .-1 }
    fn banana(&mut self) {
        fn peach(this: &Self) {
// { dg-error ".E0401." "" { target *-*-* } .-1 }
// { dg-note ".E0401." "" { target *-*-* } .-2 }
// { dg-note ".E0401." "" { target *-*-* } .-3 }
        }
    }
}

fn main() {}

