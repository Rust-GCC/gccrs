// these two HELPs are actually in a new line between this line and the `enum Fruit` line
enum Fruit {
    Apple(i64),
    Orange(i64),
}

fn should_return_fruit() -> Apple {
// { dg-error ".E0412." "" { target *-*-* } .-1 }
    Apple(5)
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

fn should_return_fruit_too() -> Fruit::Apple {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
    Apple(5)
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

fn foo() -> Ok {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
    Ok(())
}

fn bar() -> Variant3 {
// { dg-error ".E0412." "" { target *-*-* } .-1 }
}

fn qux() -> Some {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
    Some(1)
}

fn main() {}

mod x {
    pub enum Enum {
        Variant1,
        Variant2(),
        Variant3(usize),
        Variant4 {},
    }
}

