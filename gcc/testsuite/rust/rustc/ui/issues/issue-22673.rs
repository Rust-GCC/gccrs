trait Expr : PartialEq<Self::Item> {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
    type Item;
}

fn main() {}

