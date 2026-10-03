mod MyMod {}

fn main() {
    let myVar = MyMod { T: 0 };
// { dg-error ".E0574." "" { target *-*-* } .-1 }
}

