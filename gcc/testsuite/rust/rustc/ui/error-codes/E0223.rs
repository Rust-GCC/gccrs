trait MyTrait { type X; }

fn main() {
    let foo: MyTrait::X;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
}

