struct Monster {
    damage: isize
}


fn main() {
    let _m = Monster();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

