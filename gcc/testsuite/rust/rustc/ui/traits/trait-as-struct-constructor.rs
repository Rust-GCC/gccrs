trait TraitNotAStruct {}

fn main() {
    TraitNotAStruct{ value: 0 };
// { dg-error ".E0574." "" { target *-*-* } .-1 }
}

