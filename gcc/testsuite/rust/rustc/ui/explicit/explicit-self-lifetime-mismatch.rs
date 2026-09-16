struct Foo<'a,'b> {
    x: &'a isize,
    y: &'b isize,
}

impl<'a,'b> Foo<'a,'b> {
    fn bar(self:
           Foo<'b,'a>
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
// { dg-error ".E0308." "" { target *-*-* } .-7 }
// { dg-error ".E0308." "" { target *-*-* } .-8 }
           ) {}
}

fn main() {}

