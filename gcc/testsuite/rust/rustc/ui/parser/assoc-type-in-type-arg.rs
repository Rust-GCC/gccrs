trait Tr {
    type TrSubtype;
}

struct Bar<'a, Item: Tr, <Item as Tr>::TrSubtype: 'a> {
// { dg-error "" "" { target *-*-* } .-1 }
    item: Item,
    item_sub: &'a <Item as Tr>::TrSubtype,
}

fn main() {}

