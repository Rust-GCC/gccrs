enum Fruit {
    Apple(String, String),
    Pear(u32),
    Orange((String, String)),
    Banana(()),
}

fn main() {
    let x = Fruit::Apple(String::new(), String::new());
    match x {
        Fruit::Apple(a) => {}, // { dg-error ".E0023." "" { target *-*-* } }
        Fruit::Apple(a, b, c) => {}, // { dg-error ".E0023." "" { target *-*-* } }
        Fruit::Pear(1, 2) => {}, // { dg-error ".E0023." "" { target *-*-* } }
        Fruit::Orange(a, b) => {}, // { dg-error ".E0023." "" { target *-*-* } }
        Fruit::Banana() => {}, // { dg-error ".E0023." "" { target *-*-* } }
    }
}

