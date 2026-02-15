// The `?` operator is still not const-evaluatable because it calls `From::from` on the error
// variant.

const fn opt() -> Option<i32> {
    let x = Some(2);
    x?; // { dg-error ".E0744." "" { target *-*-* } }
    None
}

fn main() {}

