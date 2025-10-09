pub enum Attribute {
    Code {attr_name_idx: u16},
}

pub enum Foo {
    Bar
}

fn test(a: Foo) {
    println!("{}", a.b); // { dg-error ".E0609." "" { target *-*-* } }
}

fn main() {
    let x = Attribute::Code {
        attr_name_idx: 42,
    };
    let z = (&x).attr_name_idx; // { dg-error ".E0609." "" { target *-*-* } }
    let y = x.attr_name_idx; // { dg-error ".E0609." "" { target *-*-* } }
}

