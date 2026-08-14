fn foo() -> i32 {
    42
}

fn bar(a: i64) -> i64 {
    43
}

enum Foo {
    Bar(usize),
}

fn main() {
    foo > 12;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    bar > 13;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    foo > foo;
// { dg-error ".E0369." "" { target *-*-* } .-1 }

    foo > bar;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    let i = Foo::Bar;
    assert_eq!(Foo::Bar, i);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
}

