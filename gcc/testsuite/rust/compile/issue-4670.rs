// { dg-options "-frust-crate-attr=no_core" }

pub enum Something {
    Cat(u64),
    Dog(u64),
}

fn main() {
    let my_int: i32 = 1;

    match my_int {
        x if x == Something::Foo as i32 => {}, // { dg-error "" }
        _ => {},
    }
}
